/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ObjectMgr.h"
#include "Common.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "MapManager.h"
#include "GuildMgr.h"
#include "GroupMgr.h"
#include "SpellMgr.h"
#include "UpdateMask.h"
#include "World.h"
#include "Arena.h"
#include "Transport.h"
#include "Language.h"
#include "GameEventMgr.h"
#include "Spell.h"
#include "Chat.h"
#include "AccountMgr.h"
#include "InstanceSaveMgr.h"
#include "SpellAuras.h"
#include "Util.h"
#include "WaypointManager.h"
#include "GossipDef.h"
#include "Vehicle.h"
#include "AchievementMgr.h"
#include "DisableMgr.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "PoolMgr.h"
#include "DB2Structure.h"
#include "DB2Stores.h"
#include "Configuration/Config.h"
#include "Log.h"
#include "BattlePet.h"

namespace {

template <typename T>
void loadInvisibilityHelper(T &invisContainer, char const *query)
{
    invisContainer.clear();

    auto result = WorldDatabase.Query(query);
    if (!result)
        return;

    do
    {
        auto const fields = result->Fetch();

        auto const id = fields[0].GetUInt32();
        auto const type = static_cast<InvisibilityType>(fields[1].GetUInt8());
        auto const amount = fields[2].GetInt32();

        invisContainer.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(id),
            std::forward_as_tuple(type, amount));
    }
    while (result->NextRow());
}

} // namespace

ScriptMapMap sQuestEndScripts;
ScriptMapMap sQuestStartScripts;
ScriptMapMap sSpellScripts;
ScriptMapMap sGameObjectScripts;
ScriptMapMap sEventScripts;
ScriptMapMap sWaypointScripts;

ScriptMapMap* GetScriptsMapByType(ScriptsType type)
{
    ScriptMapMap* res = NULL;
    switch (type)
    {
        case SCRIPTS_QUEST_END:     res = &sQuestEndScripts;    break;
        case SCRIPTS_QUEST_START:   res = &sQuestStartScripts;  break;
        case SCRIPTS_SPELL:         res = &sSpellScripts;       break;
        case SCRIPTS_GAMEOBJECT:    res = &sGameObjectScripts;  break;
        case SCRIPTS_EVENT:         res = &sEventScripts;       break;
        case SCRIPTS_WAYPOINT:      res = &sWaypointScripts;    break;
        default: break;
    }
    return res;
}

bool normalizePlayerName(std::string& name)
{
    if (name.empty())
        return false;

    if (name[0] == -61 && name[1] == -97) // Interdiction d'utiliser ce caractere au debut, il fait planter l'affichage cote client
        return false;

    wchar_t wstr_buf[MAX_INTERNAL_PLAYER_NAME+1];
    size_t wstr_len = MAX_INTERNAL_PLAYER_NAME;

    if (!Utf8toWStr(name, &wstr_buf[0], wstr_len))
        return false;

    wstr_buf[0] = wcharToUpper(wstr_buf[0]);
    for (size_t i = 1; i < wstr_len; ++i)
        wstr_buf[i] = wcharToLower(wstr_buf[i]);

    if (!WStrToUtf8(wstr_buf, wstr_len, name))
        return false;

    return true;
}

bool checkMailText(std::string _text)
{
    std::string temp_str(_text);
    std::transform(temp_str.begin(), temp_str.end(), temp_str.begin(), tolower);

    bool bOk = true;

    // Check for special symbols
    bOk = (temp_str.find("|tinterface") == std::string::npos);

    return bOk;
}

LanguageDesc lang_description[LANGUAGES_COUNT] =
{
    { LANG_ADDON,           0, 0                       },
    { LANG_UNIVERSAL,       0, 0                       },
    { LANG_ORCISH,        669, SKILL_LANG_ORCISH       },
    { LANG_DARNASSIAN,    671, SKILL_LANG_DARNASSIAN   },
    { LANG_TAURAHE,       670, SKILL_LANG_TAURAHE      },
    { LANG_DWARVISH,      672, SKILL_LANG_DWARVEN      },
    { LANG_COMMON,        668, SKILL_LANG_COMMON       },
    { LANG_DEMONIC,       815, SKILL_LANG_DEMON_TONGUE },
    { LANG_TITAN,         816, SKILL_LANG_TITAN        },
    { LANG_THALASSIAN,    813, SKILL_LANG_THALASSIAN   },
    { LANG_DRACONIC,      814, SKILL_LANG_DRACONIC     },
    { LANG_KALIMAG,       817, SKILL_LANG_OLD_TONGUE   },
    { LANG_GNOMISH,      7340, SKILL_LANG_GNOMISH      },
    { LANG_TROLL,        7341, SKILL_LANG_TROLL        },
    { LANG_GUTTERSPEAK, 17737, SKILL_LANG_GUTTERSPEAK  },
    { LANG_DRAENEI,     29932, SKILL_LANG_DRAENEI      },
    { LANG_ZOMBIE,          0, 0                       },
    { LANG_GNOMISH_BINARY,  0, 0                       },
    { LANG_GOBLIN_BINARY,   0, 0                       },
    { LANG_WORGEN,      69270, SKILL_LANG_WORGEN       },
    { LANG_GOBLIN,      69269, SKILL_LANG_GOBLIN       },
    { LANG_PANDAREN_N,  108127,  SKILL_LANG_PANDAREN_N },
    { LANG_PANDAREN_H,  108130,  SKILL_LANG_PANDAREN_A },
    { LANG_PANDAREN_A,  108131,  SKILL_LANG_PANDAREN_H }

};

LanguageDesc const* GetLanguageDescByID(uint32 lang)
{
    for (uint8 i = 0; i < LANGUAGES_COUNT; ++i)
    {
        if (uint32(lang_description[i].lang_id) == lang)
            return &lang_description[i];
    }

    return NULL;
}

bool SpellClickInfo::IsFitToRequirements(Unit const* clicker, Unit const* clickee) const
{
    Player const* playerClicker = clicker->ToPlayer();
    if (!playerClicker)
        return true;

    Unit const* summoner = NULL;
    // Check summoners for party
    if (clickee->IsSummon())
        summoner = clickee->ToTempSummon()->GetSummoner();
    if (!summoner)
        summoner = clickee;

    // This only applies to players
    switch (userType)
    {
        case SPELL_CLICK_USER_FRIEND:
            if (!playerClicker->IsFriendlyTo(summoner))
                return false;
            break;
        case SPELL_CLICK_USER_RAID:
            if (!playerClicker->IsInRaidWith(summoner))
                return false;
            break;
        case SPELL_CLICK_USER_PARTY:
            if (!playerClicker->IsInPartyWith(summoner))
                return false;
            break;
        default:
            break;
    }

    return true;
}

ObjectMgr::ObjectMgr() : _auctionId(1), _equipmentSetGuid(1),
    _itemTextId(1), _mailId(1), _hiPetNumber(1), _voidItemId(1), _hiCharGuid(1),
    _hiCreatureGuid(1), _hiPetGuid(1), _hiVehicleGuid(1), _hiItemGuid(1),
    _hiGoGuid(1), _hiDoGuid(1), _hiCorpseGuid(1), _hiAreaTriggerGuid(1),
    _hiMoTransGuid(1), _battlePetId(1)
{ }

ObjectMgr::~ObjectMgr()
{
    for (QuestMap::iterator i = _questTemplates.begin(); i != _questTemplates.end(); ++i)
        delete i->second;

    for (PetLevelInfoContainer::iterator i = _petInfoStore.begin(); i != _petInfoStore.end(); ++i)
        delete[] i->second;

    for (int race = 0; race < MAX_RACES; ++race)
        for (int class_ = 0; class_ < MAX_CLASSES; ++class_)
            delete[] _playerInfo[race][class_].levelInfo;

    for (CacheVendorItemContainer::iterator itr = _cacheVendorItemStore.begin(); itr != _cacheVendorItemStore.end(); ++itr)
        itr->second.Clear();

    _cacheTrainerSpellStore.clear();

    for (DungeonEncounterContainer::iterator itr =_dungeonEncounterStore.begin(); itr != _dungeonEncounterStore.end(); ++itr)
        for (DungeonEncounterList::iterator encounterItr = itr->second.begin(); encounterItr != itr->second.end(); ++encounterItr)
            delete *encounterItr;
}

void ObjectMgr::AddLocaleString(std::string const& s, LocaleConstant locale, StringVector& data)
{
    if (!s.empty())
    {
        if (data.size() <= size_t(locale))
            data.resize(locale + 1);

        data[locale] = s;
    }
}

void ObjectMgr::LoadCreatureLocales()
{
    uint32 oldMSTime = getMSTime();

    _creatureLocaleStore.clear();                              // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, name_loc1, subname_loc1, name_loc2, subname_loc2, name_loc3, subname_loc3, name_loc4, subname_loc4, name_loc5, subname_loc5, name_loc6, subname_loc6, name_loc7, subname_loc7, name_loc8, subname_loc8, name_loc9, subname_loc9, name_loc10, subname_loc10 FROM locales_creature");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        CreatureLocale& data = _creatureLocaleStore[entry];

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
        {
            LocaleConstant locale = (LocaleConstant) i;
            AddLocaleString(fields[1 + 2 * (i - 1)].GetString(), locale, data.Name);
            AddLocaleString(fields[1 + 2 * (i - 1) + 1].GetString(), locale, data.SubName);
        }
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %lu creature locale strings in %u ms", (unsigned long)_creatureLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadGossipMenuItemsLocales()
{
    uint32 oldMSTime = getMSTime();

    _gossipMenuItemsLocaleStore.clear();                              // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT menu_id, id, "
        "option_text_loc1, box_text_loc1, option_text_loc2, box_text_loc2, "
        "option_text_loc3, box_text_loc3, option_text_loc4, box_text_loc4, "
        "option_text_loc5, box_text_loc5, option_text_loc6, box_text_loc6, "
        "option_text_loc7, box_text_loc7, option_text_loc8, box_text_loc8, "
        "option_text_loc9, box_text_loc9, option_text_loc10, box_text_loc10 "
        "FROM locales_gossip_menu_option");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint16 menuId   = fields[0].GetUInt32();
        uint16 id       = fields[1].GetUInt16();

        GossipMenuItemsLocale& data = _gossipMenuItemsLocaleStore[MAKE_PAIR32(menuId, id)];

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
        {
            LocaleConstant locale = (LocaleConstant) i;
            AddLocaleString(fields[2 + 2 * (i - 1)].GetString(), locale, data.OptionText);
            AddLocaleString(fields[2 + 2 * (i - 1) + 1].GetString(), locale, data.BoxText);
        }
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %lu gossip_menu_option locale strings in %u ms", (unsigned long)_gossipMenuItemsLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadPointOfInterestLocales()
{
    uint32 oldMSTime = getMSTime();

    _pointOfInterestLocaleStore.clear();                              // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, icon_name_loc1, icon_name_loc2, icon_name_loc3, icon_name_loc4, icon_name_loc5, icon_name_loc6, icon_name_loc7, icon_name_loc8, icon_name_loc9, icon_name_loc10 FROM locales_points_of_interest");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        PointOfInterestLocale& data = _pointOfInterestLocaleStore[entry];

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
            AddLocaleString(fields[i].GetString(), LocaleConstant(i), data.IconName);
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %lu points_of_interest locale strings in %u ms", (unsigned long)_pointOfInterestLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadCreatureTemplates()
{
    uint32 oldMSTime = getMSTime();

    //                                                 0              1                 2                  3                 4                      5                   6
    QueryResult result = WorldDatabase.Query("SELECT entry, difficulty_entry_1, difficulty_entry_2, difficulty_entry_3, difficulty_entry_4, difficulty_entry_5, difficulty_entry_6, "
    //                                                 7                  8                    9                  10                11                     12
                                             "difficulty_entry_7, difficulty_entry_8, difficulty_entry_9, difficulty_entry_10, difficulty_entry_11, difficulty_entry_12, "
    //                                                  13                  14              15                   16           17          18       19          20
                                             "difficulty_entry_13, difficulty_entry_14, difficulty_entry_15, KillCredit1, KillCredit2, modelid1, modelid2, modelid3, "
    //                                           21      22      23       24           25           26        27     28      29        30        31         32         33        34         35
                                             "modelid4, name, subname, IconName, gossip_menu_id, minlevel, maxlevel, exp, exp_unk, faction_A, faction_H, npcflag, npcflag2, speed_walk, speed_run, "
    //                                             36      37    38     39     40        41           42            43              44               45            46         47          48
                                             "speed_fly, scale, rank, mindmg, maxdmg, dmgschool, attackpower, dmg_multiplier, baseattacktime, rangeattacktime, unit_class, unit_flags, unit_flags2, "
    //                                             49         50         51             52             53             54          55           56              57           58
                                             "dynamicflags, family, trainer_type, trainer_spell, trainer_class, trainer_race, minrangedmg, maxrangedmg, rangedattackpower, type, "
    //                                            59           60        61         62            63          64          65           66           67           68           69
                                             "type_flags, type_flags2, lootid, pickpocketloot, skinloot, resistance1, resistance2, resistance3, resistance4, resistance5, resistance6, "
    //                                          70      71      72      73      74      75      76      77         78            79       80       81       82         83
                                             "spell1, spell2, spell3, spell4, spell5, spell6, spell7, spell8, PetSpellDataId, VehicleId, mingold, maxgold, AIName, MovementType, "
    //                                             84          85          86         87            88            89          90           91          92          93           94          95
                                             "InhabitType, HoverHeight, Health_mod, Mana_mod, Mana_mod_extra, Armor_mod, RacialLeader, questItem1, questItem2, questItem3, questItem4, questItem5, "
    //                                            96           97            98         99               100                  101         102
                                             "questItem6, movementId, RegenHealth, equipment_id, mechanic_immune_mask, flags_extra, ScriptName "
                                             "FROM creature_template");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 creature template definitions. DB table `creature_template` is empty.");
        return;
    }

    _creatureTemplateStore.rehash(result->GetRowCount());
    uint32 count = 0;
    do
    {
        uint8 index = 0;
        Field* fields = result->Fetch();

        uint32 entry = fields[index++].GetUInt32();

        CreatureTemplate& creatureTemplate = _creatureTemplateStore[entry];

        creatureTemplate.Entry = entry;

        for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
            creatureTemplate.DifficultyEntry[i] = fields[index++].GetUInt32();

        for (uint8 i = 0; i < MAX_KILL_CREDIT; ++i)
            creatureTemplate.KillCredit[i] = fields[index++].GetUInt32();

        creatureTemplate.Modelid1          = fields[index++].GetUInt32();
        creatureTemplate.Modelid2          = fields[index++].GetUInt32();
        creatureTemplate.Modelid3          = fields[index++].GetUInt32();
        creatureTemplate.Modelid4          = fields[index++].GetUInt32();
        creatureTemplate.Name              = fields[index++].GetString();
        creatureTemplate.SubName           = fields[index++].GetString();
        creatureTemplate.IconName          = fields[index++].GetString();
        creatureTemplate.GossipMenuId      = fields[index++].GetUInt32();
        creatureTemplate.minlevel          = fields[index++].GetUInt8();
        creatureTemplate.maxlevel          = fields[index++].GetUInt8();
        creatureTemplate.expansion         = fields[index++].GetUInt16();
        creatureTemplate.expansionUnknown  = fields[index++].GetUInt16();
        creatureTemplate.faction_A         = fields[index++].GetUInt16();
        creatureTemplate.faction_H         = fields[index++].GetUInt16();
        creatureTemplate.npcflag           = fields[index++].GetUInt32();
        creatureTemplate.npcflag2          = fields[index++].GetUInt32();
        creatureTemplate.speed_walk        = fields[index++].GetFloat();
        creatureTemplate.speed_run         = fields[index++].GetFloat();
        creatureTemplate.speed_fly         = fields[index++].GetFloat();
        creatureTemplate.scale             = fields[index++].GetFloat();
        creatureTemplate.rank              = fields[index++].GetUInt8();
        creatureTemplate.mindmg            = fields[index++].GetFloat();
        creatureTemplate.maxdmg            = fields[index++].GetFloat();
        creatureTemplate.dmgschool         = fields[index++].GetInt8();
        creatureTemplate.attackpower       = fields[index++].GetUInt32();
        creatureTemplate.dmg_multiplier    = fields[index++].GetFloat();
        creatureTemplate.baseattacktime    = fields[index++].GetUInt32();
        creatureTemplate.rangeattacktime   = fields[index++].GetUInt32();
        creatureTemplate.unit_class        = fields[index++].GetUInt8();
        creatureTemplate.unit_flags        = fields[index++].GetUInt32();
        creatureTemplate.unit_flags2       = fields[index++].GetUInt32();
        creatureTemplate.dynamicflags      = fields[index++].GetUInt32();
        creatureTemplate.family            = fields[index++].GetUInt32();
        creatureTemplate.trainer_type      = fields[index++].GetUInt8();
        creatureTemplate.trainer_spell     = fields[index++].GetUInt32();
        creatureTemplate.trainer_class     = fields[index++].GetUInt8();
        creatureTemplate.trainer_race      = fields[index++].GetUInt8();
        creatureTemplate.minrangedmg       = fields[index++].GetFloat();
        creatureTemplate.maxrangedmg       = fields[index++].GetFloat();
        creatureTemplate.rangedattackpower = fields[index++].GetUInt16();
        creatureTemplate.type              = fields[index++].GetUInt8();
        creatureTemplate.type_flags        = fields[index++].GetUInt32();
        creatureTemplate.type_flags2       = fields[index++].GetUInt32();
        creatureTemplate.lootid            = fields[index++].GetUInt32();
        creatureTemplate.pickpocketLootId  = fields[index++].GetUInt32();
        creatureTemplate.SkinLootId        = fields[index++].GetUInt32();

        for (uint8 i = SPELL_SCHOOL_HOLY; i < MAX_SPELL_SCHOOL; ++i)
            creatureTemplate.resistance[i] = fields[index++].GetInt16();

        for (uint8 i = 0; i < CREATURE_MAX_SPELLS; ++i)
            creatureTemplate.spells[i] = fields[index++].GetUInt32();

        creatureTemplate.PetSpellDataId = fields[index++].GetUInt32();
        creatureTemplate.VehicleId      = fields[index++].GetUInt32();
        creatureTemplate.mingold        = fields[index++].GetUInt32();
        creatureTemplate.maxgold        = fields[index++].GetUInt32();
        creatureTemplate.AIName         = fields[index++].GetString();
        creatureTemplate.MovementType   = fields[index++].GetUInt8();
        creatureTemplate.InhabitType    = fields[index++].GetUInt8();
        creatureTemplate.HoverHeight    = fields[index++].GetFloat();
        creatureTemplate.ModHealth      = fields[index++].GetFloat();
        creatureTemplate.ModMana        = fields[index++].GetFloat();
        creatureTemplate.ModManaExtra   = fields[index++].GetFloat();
        creatureTemplate.ModArmor       = fields[index++].GetFloat();
        creatureTemplate.RacialLeader   = fields[index++].GetBool();

        for (uint8 i = 0; i < MAX_CREATURE_QUEST_ITEMS; ++i)
            creatureTemplate.questItems[i] = fields[index++].GetUInt32();

        creatureTemplate.movementId         = fields[index++].GetUInt32();
        creatureTemplate.RegenHealth        = fields[index++].GetBool();
        creatureTemplate.equipmentId        = fields[index++].GetUInt32();
        creatureTemplate.MechanicImmuneMask = fields[index++].GetUInt32();
        creatureTemplate.flags_extra        = fields[index++].GetUInt32();
        creatureTemplate.ScriptID           = GetScriptId(fields[index++].GetCString());

        ++count;
    }
    while (result->NextRow());

    // Checking needs to be done after loading because of the difficulty self referencing
    for (CreatureTemplateContainer::iterator itr = _creatureTemplateStore.begin(); itr != _creatureTemplateStore.end(); ++itr)
        FixCreatureTemplate(itr->second);

    TC_LOG_INFO("server.loading", ">> Loaded %u creature definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadCreatureTemplateAddons()
{
    uint32 oldMSTime = getMSTime();

    LoadCreatureAddonPaths("entry", "creature_template_path", _creatureTemplateAddonStore);
    LoadCreatureAddonMounts("entry", "creature_template_mount", _creatureTemplateAddonStore);
    LoadCreatureAddonBytes("entry", "creature_template_bytes", _creatureTemplateAddonStore);
    LoadCreatureAddonEmotes("entry", "creature_template_emote", _creatureTemplateAddonStore);
    LoadCreatureAddonAuras("entry", "creature_template_aura", _creatureTemplateAddonStore);

    TC_LOG_INFO("server.loading", ">> Loaded " UI64FMTD " creature template addons in %u ms", _creatureTemplateAddonStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::FixCreatureTemplate(CreatureTemplate &cInfo)
{
    bool ok = true;                                     // bool to allow continue outside this loop
    for (uint32 diff = 0; diff < MAX_DIFFICULTY - 1 && ok; ++diff)
    {
        if (!cInfo.DifficultyEntry[diff])
            continue;
        ok = false;                                     // will be set to true at the end of this loop again

        CreatureTemplate const* difficultyInfo = GetCreatureTemplate(cInfo.DifficultyEntry[diff]);
        if (!difficultyInfo)
        {
            TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has `difficulty_entry_%u`=%u but creature entry %u does not exist.",
                cInfo.Entry, diff + 1, cInfo.DifficultyEntry[diff], cInfo.DifficultyEntry[diff]);
            continue;
        }

        bool ok2 = true;
        for (uint32 diff2 = 0; diff2 < MAX_DIFFICULTY - 1 && ok2; ++diff2)
        {
            ok2 = false;
            if (_difficultyEntries[diff2].find(cInfo.Entry) != _difficultyEntries[diff2].end())
            {
                TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) is listed as `difficulty_entry_%u` of another creature, but itself lists %u in `difficulty_entry_%u`.",
                    cInfo.Entry, diff2 + 1, cInfo.DifficultyEntry[diff], diff + 1);
                continue;
            }

            /*if (_difficultyEntries[diff2].find(cInfo.DifficultyEntry[diff]) != _difficultyEntries[diff2].end())
            {
                TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) already listed as `difficulty_entry_%u` for another entry.", cInfo.DifficultyEntry[diff], diff2 + 1);
                continue;
            }*/

            if (_hasDifficultyEntries[diff2].find(cInfo.DifficultyEntry[diff]) != _hasDifficultyEntries[diff2].end())
            {
                TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has `difficulty_entry_%u`=%u but creature entry %u has itself a value in `difficulty_entry_%u`.",
                    cInfo.Entry, diff + 1, cInfo.DifficultyEntry[diff], cInfo.DifficultyEntry[diff], diff2 + 1);
                continue;
            }
            ok2 = true;
        }
        if (!ok2)
            continue;

        if (cInfo.unit_class != difficultyInfo->unit_class)
        {
            TC_LOG_ERROR("sql.sql", "Creature (Entry: %u, class %u) has different `unit_class` in difficulty %u mode (Entry: %u, class %u).",
                cInfo.Entry, cInfo.unit_class, diff + 1, cInfo.DifficultyEntry[diff], difficultyInfo->unit_class);
            continue;
        }

        if (cInfo.npcflag != difficultyInfo->npcflag)
        {
            TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has different `npcflag` in difficulty %u mode (Entry: %u).", cInfo.Entry, diff + 1, cInfo.DifficultyEntry[diff]);
            continue;
        }

        if (cInfo.trainer_class != difficultyInfo->trainer_class)
        {
            TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has different `trainer_class` in difficulty %u mode (Entry: %u).", cInfo.Entry, diff + 1, cInfo.DifficultyEntry[diff]);
            continue;
        }

        if (cInfo.trainer_race != difficultyInfo->trainer_race)
        {
            TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has different `trainer_race` in difficulty %u mode (Entry: %u).", cInfo.Entry, diff + 1, cInfo.DifficultyEntry[diff]);
            continue;
        }

        if (cInfo.trainer_type != difficultyInfo->trainer_type)
        {
            TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has different `trainer_type` in difficulty %u mode (Entry: %u).", cInfo.Entry, diff + 1, cInfo.DifficultyEntry[diff]);
            continue;
        }

        if (cInfo.trainer_spell != difficultyInfo->trainer_spell)
        {
            TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has different `trainer_spell` in difficulty %u mode (Entry: %u).", cInfo.Entry, diff + 1, cInfo.DifficultyEntry[diff]);
            continue;
        }

        if (!difficultyInfo->AIName.empty())
        {
            TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) lists difficulty %u mode entry %u with `AIName` filled in. `AIName` of difficulty 0 mode creature is always used instead.",
                cInfo.Entry, diff + 1, cInfo.DifficultyEntry[diff]);
            continue;
        }

        if (difficultyInfo->ScriptID)
        {
            TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) lists difficulty %u mode entry %u with `ScriptName` filled in. `ScriptName` of difficulty 0 mode creature is always used instead.",
                cInfo.Entry, diff + 1, cInfo.DifficultyEntry[diff]);
            continue;
        }

        _hasDifficultyEntries[diff].insert(cInfo.Entry);
        _difficultyEntries[diff].insert(cInfo.DifficultyEntry[diff]);
        ok = true;
    }

    FactionTemplateEntry const* factionTemplate = sFactionTemplateStore.LookupEntry(cInfo.faction_A);
    if (!factionTemplate)
        TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has non-existing faction_A template (%u).", cInfo.Entry, cInfo.faction_A);

    factionTemplate = sFactionTemplateStore.LookupEntry(cInfo.faction_H);
    if (!factionTemplate)
        TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has non-existing faction_H template (%u).", cInfo.Entry, cInfo.faction_H);

    // used later for scale
    CreatureDisplayInfoEntry const* displayScaleEntry = NULL;

    if (cInfo.Modelid1)
    {
        CreatureDisplayInfoEntry const* displayEntry = sCreatureDisplayInfoStore.LookupEntry(cInfo.Modelid1);
        if (!displayEntry)
        {
            TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) lists non-existing Modelid1 id (%u), this can crash the client.", cInfo.Entry, cInfo.Modelid1);
            cInfo.Modelid1 = 0;
        }
        else if (!displayScaleEntry)
            displayScaleEntry = displayEntry;

        CreatureModelInfo const* modelInfo = GetCreatureModelInfo(cInfo.Modelid1);
        if (!modelInfo)
            TC_LOG_ERROR("sql.sql", "No model data exist for `Modelid1` = %u listed by creature (Entry: %u).", cInfo.Modelid1, cInfo.Entry);
    }

    if (cInfo.Modelid2)
    {
        CreatureDisplayInfoEntry const* displayEntry = sCreatureDisplayInfoStore.LookupEntry(cInfo.Modelid2);
        if (!displayEntry)
        {
            TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) lists non-existing Modelid2 id (%u), this can crash the client.", cInfo.Entry, cInfo.Modelid2);
            cInfo.Modelid2 = 0;
        }
        else if (!displayScaleEntry)
            displayScaleEntry = displayEntry;

        CreatureModelInfo const* modelInfo = GetCreatureModelInfo(cInfo.Modelid2);
        if (!modelInfo)
            TC_LOG_ERROR("sql.sql", "No model data exist for `Modelid2` = %u listed by creature (Entry: %u).", cInfo.Modelid2, cInfo.Entry);
    }

    if (cInfo.Modelid3)
    {
        CreatureDisplayInfoEntry const* displayEntry = sCreatureDisplayInfoStore.LookupEntry(cInfo.Modelid3);
        if (!displayEntry)
        {
            TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) lists non-existing Modelid3 id (%u), this can crash the client.", cInfo.Entry, cInfo.Modelid3);
            cInfo.Modelid3 = 0;
        }
        else if (!displayScaleEntry)
            displayScaleEntry = displayEntry;

        CreatureModelInfo const* modelInfo = GetCreatureModelInfo(cInfo.Modelid3);
        if (!modelInfo)
            TC_LOG_ERROR("sql.sql", "No model data exist for `Modelid3` = %u listed by creature (Entry: %u).", cInfo.Modelid3, cInfo.Entry);
    }

    if (cInfo.Modelid4)
    {
        CreatureDisplayInfoEntry const* displayEntry = sCreatureDisplayInfoStore.LookupEntry(cInfo.Modelid4);
        if (!displayEntry)
        {
            TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) lists non-existing Modelid4 id (%u), this can crash the client.", cInfo.Entry, cInfo.Modelid4);
            cInfo.Modelid4 = 0;
        }
        else if (!displayScaleEntry)
            displayScaleEntry = displayEntry;

        CreatureModelInfo const* modelInfo = GetCreatureModelInfo(cInfo.Modelid4);
        if (!modelInfo)
            TC_LOG_ERROR("sql.sql", "No model data exist for `Modelid4` = %u listed by creature (Entry: %u).", cInfo.Modelid4, cInfo.Entry);
    }

    if (!displayScaleEntry)
        TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) does not have any existing display id in Modelid1/Modelid2/Modelid3/Modelid4.", cInfo.Entry);

    for (int k = 0; k < MAX_KILL_CREDIT; ++k)
    {
        if (cInfo.KillCredit[k])
        {
            if (!GetCreatureTemplate(cInfo.KillCredit[k]))
            {
                TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) lists non-existing creature entry %u in `KillCredit%d`.", cInfo.Entry, cInfo.KillCredit[k], k + 1);
                cInfo.KillCredit[k] = 0;
            }
        }
    }

    if (!cInfo.unit_class || ((1 << (cInfo.unit_class-1)) & CLASSMASK_ALL_CREATURES) == 0)
    {
        TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has invalid unit_class (%u) in creature_template. Set to 1 (UNIT_CLASS_WARRIOR).", cInfo.Entry, cInfo.unit_class);
        cInfo.unit_class = UNIT_CLASS_WARRIOR;
    }

    if (cInfo.dmgschool >= MAX_SPELL_SCHOOL)
    {
        TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has invalid spell school value (%u) in `dmgschool`.", cInfo.Entry, cInfo.dmgschool);
        cInfo.dmgschool = SPELL_SCHOOL_NORMAL;
    }

    if (cInfo.baseattacktime == 0)
        cInfo.baseattacktime  = BASE_ATTACK_TIME;

    if (cInfo.rangeattacktime == 0)
        cInfo.rangeattacktime = BASE_ATTACK_TIME;

    if ((cInfo.npcflag & UNIT_NPC_FLAG_TRAINER) && cInfo.trainer_type >= MAX_TRAINER_TYPE)
        TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has wrong trainer type %u.", cInfo.Entry, cInfo.trainer_type);

    if (cInfo.type && !sCreatureTypeStore.LookupEntry(cInfo.type))
    {
        TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has invalid creature type (%u) in `type`.", cInfo.Entry, cInfo.type);
        cInfo.type = CREATURE_TYPE_HUMANOID;
    }

    // must exist or used hidden but used in data horse case
    if (cInfo.family && !sCreatureFamilyStore.LookupEntry(cInfo.family) && cInfo.family != CREATURE_FAMILY_HORSE_CUSTOM)
    {
        TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has invalid creature family (%u) in `family`.", cInfo.Entry, cInfo.family);
        cInfo.family = 0;
    }

    if (cInfo.InhabitType <= 0 || cInfo.InhabitType > INHABIT_ANYWHERE)
    {
        TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has wrong value (%u) in `InhabitType`, creature will not correctly walk/swim/fly.", cInfo.Entry, cInfo.InhabitType);
        cInfo.InhabitType = INHABIT_ANYWHERE;
    }

    if (cInfo.HoverHeight < 0.0f)
    {
        TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has wrong value (%f) in `HoverHeight`", cInfo.Entry, cInfo.HoverHeight);
        cInfo.HoverHeight = 1.0f;
    }

    if (cInfo.VehicleId)
    {
        VehicleEntry const* vehId = sVehicleStore.LookupEntry(cInfo.VehicleId);
        if (!vehId)
        {
             TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has a non-existing VehicleId (%u). This *WILL* cause the client to freeze!", cInfo.Entry, cInfo.VehicleId);
             cInfo.VehicleId = 0;
        }
    }

    if (cInfo.PetSpellDataId)
    {
        CreatureSpellDataEntry const* spellDataId = sCreatureSpellDataStore.LookupEntry(cInfo.PetSpellDataId);
        if (!spellDataId)
            TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has non-existing PetSpellDataId (%u).", cInfo.Entry, cInfo.PetSpellDataId);
    }

    for (uint8 j = 0; j < CREATURE_MAX_SPELLS; ++j)
    {
        if (cInfo.spells[j] && !sSpellMgr->GetSpellInfo(cInfo.spells[j]))
        {
            WorldDatabase.PExecute("UPDATE creature_template SET spell%d = 0 WHERE entry = %u", j+1, cInfo.Entry);
            TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has non-existing Spell%d (%u), set to 0.", cInfo.Entry, j+1, cInfo.spells[j]);
            cInfo.spells[j] = 0;
        }
    }

    if (cInfo.MovementType >= MAX_DB_MOTION_TYPE)
    {
        TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) has wrong movement generator type (%u), ignored and set to IDLE.", cInfo.Entry, cInfo.MovementType);
        cInfo.MovementType = IDLE_MOTION_TYPE;
    }

    if (cInfo.equipmentId > 0)                          // 0 no equipment
    {
        if (!GetEquipmentInfo(cInfo.equipmentId))
        {
            TC_LOG_ERROR("sql.sql", "Table `creature_template` lists creature (Entry: %u) with `equipment_id` %u not found in table `creature_equip_template`, set to no equipment.", cInfo.Entry, cInfo.equipmentId);
            cInfo.equipmentId = 0;
        }
    }

    /// if not set custom creature scale then load scale from CreatureDisplayInfo.dbc
    if (cInfo.scale <= 0.0f)
    {
        if (displayScaleEntry)
            cInfo.scale = displayScaleEntry->scale;
        else
            cInfo.scale = 1.0f;
    }

    if (cInfo.expansion > MAX_CREATURE_BASE_HP)
    {
        TC_LOG_ERROR("sql.sql", "Table `creature_template` lists creature (Entry: %u) with `exp` %u. Ignored and set to 0.", cInfo.Entry, cInfo.expansion);
        cInfo.expansion = 0;
    }

    if (cInfo.expansionUnknown > MAX_CREATURE_BASE_HP)
    {
        TC_LOG_ERROR("sql.sql", "Table `creature_template` lists creature (Entry: %u) with `exp_unk` %u. Ignored and set to 0.", cInfo.Entry, cInfo.expansionUnknown);
        cInfo.expansionUnknown = 0;
    }

    if (uint32 badFlags = (cInfo.flags_extra & ~CREATURE_FLAG_EXTRA_DB_ALLOWED))
    {
        TC_LOG_ERROR("sql.sql", "Table `creature_template` lists creature (Entry: %u) with disallowed `flags_extra` %u, removing incorrect flag.", cInfo.Entry, badFlags);
        cInfo.flags_extra &= CREATURE_FLAG_EXTRA_DB_ALLOWED;
    }

    cInfo.dmg_multiplier *= Creature::_GetDamageMod(cInfo.rank);
}

void ObjectMgr::LoadCreatureAddons()
{
    uint32 oldMSTime = getMSTime();

    LoadCreatureAddonPaths("guid", "creature_path", _creatureAddonStore);
    LoadCreatureAddonMounts("guid", "creature_mount", _creatureAddonStore);
    LoadCreatureAddonBytes("guid", "creature_bytes", _creatureAddonStore);
    LoadCreatureAddonEmotes("guid", "creature_emote", _creatureAddonStore);
    LoadCreatureAddonAuras("guid", "creature_aura", _creatureAddonStore);

    TC_LOG_INFO("server.loading", ">> Loaded " UI64FMTD " creature addons in %u ms", _creatureAddonStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadCreatureScriptNames()
{
    uint32 oldMSTime = getMSTime();
    uint32 count = 0;

    QueryResult result = WorldDatabase.Query("SELECT guid, ScriptName FROM creature_script_names");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 guid = fields[0].GetUInt32();

            _creatureScriptnameStore[guid] = GetScriptId(fields[1].GetCString());
            ++count;
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u creature script names in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

uint32 const ObjectMgr::GetCreatureScriptNameId(uint32 lowguid) const
{
    CreatureScriptnameContainer::const_iterator itr = _creatureScriptnameStore.find(lowguid);
    if (itr != _creatureScriptnameStore.end())
        return itr->second;

    return 0;
}

void ObjectMgr::loadCreatureTemplateInvisibility()
{
    auto const start = getMSTime();
    loadInvisibilityHelper(creatureTemplateInvisibilityStore_, "SELECT entry, type, amount FROM creature_template_invisibility");
    TC_LOG_INFO("server.loading", ">> Loaded " SIZEFMTD " creature template invisibilities in %u ms",
                creatureTemplateInvisibilityStore_.size(), GetMSTimeDiffToNow(start));
}

void ObjectMgr::loadCreatureInvisibility()
{
    auto const start = getMSTime();
    loadInvisibilityHelper(creatureInvisibilityStore_, "SELECT guid, type, amount FROM creature_invisibility");
    TC_LOG_INFO("server.loading", ">> Loaded " SIZEFMTD " creature invisibilities in %u ms",
                creatureInvisibilityStore_.size(), GetMSTimeDiffToNow(start));
}

CreatureAddon const* ObjectMgr::GetCreatureAddon(uint32 lowguid)
{
    CreatureAddonContainer::const_iterator itr = _creatureAddonStore.find(lowguid);
    if (itr != _creatureAddonStore.end())
        return &(itr->second);

    return NULL;
}

CreatureAddon const* ObjectMgr::GetCreatureTemplateAddon(uint32 entry)
{
    CreatureAddonContainer::const_iterator itr = _creatureTemplateAddonStore.find(entry);
    if (itr != _creatureTemplateAddonStore.end())
        return &(itr->second);

    return NULL;
}

ObjectInvisibility const * ObjectMgr::gameObjectTemplateInvisibility(uint32 entry) const
{
    auto const itr = gameObjectTemplateInvisibilityStore_.find(entry);
    return itr != gameObjectTemplateInvisibilityStore_.end() ? &itr->second : nullptr;
}

ObjectInvisibility const * ObjectMgr::gameObjectInvisibility(uint32 guid) const
{
    auto const itr = gameObjectInvisibilityStore_.find(guid);
    return itr != gameObjectInvisibilityStore_.end() ? &itr->second : nullptr;
}

ObjectInvisibility const * ObjectMgr::creatureTemplateInvisibility(uint32 entry) const
{
    auto const itr = creatureTemplateInvisibilityStore_.find(entry);
    return itr != creatureTemplateInvisibilityStore_.end() ? &itr->second : nullptr;
}

ObjectInvisibility const * ObjectMgr::creatureInvisibility(uint32 guid) const
{
    auto const itr = creatureInvisibilityStore_.find(guid);
    return itr != creatureInvisibilityStore_.end() ? &itr->second : nullptr;
}

EquipmentInfo const* ObjectMgr::GetEquipmentInfo(uint32 entry)
{
    EquipmentInfoContainer::const_iterator itr = _equipmentInfoStore.find(entry);
    if (itr != _equipmentInfoStore.end())
        return &(itr->second);

    return NULL;
}

void ObjectMgr::LoadEquipmentTemplates()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT entry, itemEntry1, itemEntry2, itemEntry3 FROM creature_equip_template");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 creature equipment templates. DB table `creature_equip_template` is empty!");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint16 entry = fields[0].GetUInt16();

        EquipmentInfo& equipmentInfo = _equipmentInfoStore[entry];

        equipmentInfo.ItemEntry[0] = fields[1].GetUInt32();
        equipmentInfo.ItemEntry[1] = fields[2].GetUInt32();
        equipmentInfo.ItemEntry[2] = fields[3].GetUInt32();

        for (uint8 i = 0; i < MAX_EQUIPMENT_ITEMS; ++i)
        {
            if (!equipmentInfo.ItemEntry[i])
                continue;

            ItemEntry const* dbcItem = sItemStore.LookupEntry(equipmentInfo.ItemEntry[i]);

            if (!dbcItem)
            {
                TC_LOG_ERROR("sql.sql", "Unknown item (entry=%u) in creature_equip_template.itemEntry%u for entry = %u, forced to 0.",
                    equipmentInfo.ItemEntry[i], i+1, entry);
                equipmentInfo.ItemEntry[i] = 0;
                continue;
            }

            if (dbcItem->InventoryType != INVTYPE_WEAPON &&
                dbcItem->InventoryType != INVTYPE_SHIELD &&
                dbcItem->InventoryType != INVTYPE_RANGED &&
                dbcItem->InventoryType != INVTYPE_2HWEAPON &&
                dbcItem->InventoryType != INVTYPE_WEAPONMAINHAND &&
                dbcItem->InventoryType != INVTYPE_WEAPONOFFHAND &&
                dbcItem->InventoryType != INVTYPE_HOLDABLE &&
                dbcItem->InventoryType != INVTYPE_THROWN &&
                dbcItem->InventoryType != INVTYPE_RANGEDRIGHT)
            {
                TC_LOG_ERROR("sql.sql", "Item (entry=%u) in creature_equip_template.itemEntry%u for entry = %u is not equipable in a hand, forced to 0.",
                    equipmentInfo.ItemEntry[i], i+1, entry);
                equipmentInfo.ItemEntry[i] = 0;
            }
        }

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u equipment templates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

CreatureModelInfo const* ObjectMgr::GetCreatureModelInfo(uint32 modelId)
{
    CreatureModelContainer::const_iterator itr = _creatureModelStore.find(modelId);
    if (itr != _creatureModelStore.end())
        return &(itr->second);

    return NULL;
}

uint32 ObjectMgr::ChooseDisplayId(uint32 /*team*/, const CreatureTemplate* cinfo, const CreatureData* data /*= NULL*/)
{
    // Load creature model (display id)
    uint32 display_id = 0;

    if (!data || data->displayid == 0)
    {
        display_id = cinfo->GetRandomValidModelId();
    }
    else
        return data->displayid;

    return display_id;
}

void ObjectMgr::ChooseCreatureFlags(const CreatureTemplate* cinfo, uint32& npcflag, uint32& unit_flags, uint32& unit_flags2, uint32& dynamicflags, const CreatureData* data /*= NULL*/)
{
    npcflag = cinfo->npcflag;
    unit_flags = cinfo->unit_flags;
    unit_flags2 = cinfo->unit_flags2;
    dynamicflags = cinfo->dynamicflags;

    if (data)
    {
        if (data->npcflag)
            npcflag = data->npcflag;

        if (data->unit_flags)
            unit_flags = data->unit_flags;

        if (data->unit_flags2)
            unit_flags2 = data->unit_flags2;

        if (data->dynamicflags)
            dynamicflags = data->dynamicflags;
    }
}

CreatureModelInfo const* ObjectMgr::GetCreatureModelRandomGender(uint32* displayID)
{
    CreatureModelInfo const* modelInfo = GetCreatureModelInfo(*displayID);
    if (!modelInfo)
        return NULL;

    // If a model for another gender exists, 50% chance to use it
    if (modelInfo->modelid_other_gender != 0 && urand(0, 1) == 0)
    {
        CreatureModelInfo const* minfo_tmp = GetCreatureModelInfo(modelInfo->modelid_other_gender);
        if (!minfo_tmp)
            TC_LOG_ERROR("sql.sql", "Model (Entry: %u) has modelid_other_gender %u not found in table `creature_model_info`. ", *displayID, modelInfo->modelid_other_gender);
        else
        {
            // Model ID changed
            *displayID = modelInfo->modelid_other_gender;
            return minfo_tmp;
        }
    }

    return modelInfo;
}

void ObjectMgr::LoadCreatureModelInfo()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT modelid, bounding_radius, combat_reach, gender, modelid_other_gender FROM creature_model_info");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 creature model definitions. DB table `creature_model_info` is empty.");
        return;
    }

    //_creatureModelStore.rehash(result->GetRowCount());
    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 modelId = fields[0].GetUInt32();

        CreatureModelInfo& modelInfo = _creatureModelStore[modelId];

        modelInfo.bounding_radius      = fields[1].GetFloat();
        modelInfo.combat_reach         = fields[2].GetFloat();
        modelInfo.gender               = fields[3].GetUInt8();
        modelInfo.modelid_other_gender = fields[4].GetUInt32();

        // Checks

        if (!sCreatureDisplayInfoStore.LookupEntry(modelId))
            TC_LOG_ERROR("sql.sql", "Table `creature_model_info` has model for not existed display id (%u).", modelId);

        if (modelInfo.gender > GENDER_NONE)
        {
            TC_LOG_ERROR("sql.sql", "Table `creature_model_info` has wrong gender (%u) for display id (%u).", uint32(modelInfo.gender), modelId);
            modelInfo.gender = GENDER_MALE;
        }

        if (modelInfo.modelid_other_gender && !sCreatureDisplayInfoStore.LookupEntry(modelInfo.modelid_other_gender))
        {
            TC_LOG_ERROR("sql.sql", "Table `creature_model_info` has not existed alt.gender model (%u) for existed display id (%u).", modelInfo.modelid_other_gender, modelId);
            modelInfo.modelid_other_gender = 0;
        }

        if (modelInfo.combat_reach < 0.1f)
            modelInfo.combat_reach = DEFAULT_COMBAT_REACH;

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u creature model based info in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadLinkedRespawn()
{
    uint32 oldMSTime = getMSTime();

    _linkedRespawnStore.clear();
    //                                                 0        1          2
    QueryResult result = WorldDatabase.Query("SELECT guid, linkedGuid, linkType FROM linked_respawn ORDER BY guid ASC");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 linked respawns. DB table `linked_respawn` is empty.");

        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 guidLow = fields[0].GetUInt32();
        uint32 linkedGuidLow = fields[1].GetUInt32();
        uint8  linkType = fields[2].GetUInt8();

        uint64 guid = 0, linkedGuid = 0;
        bool error = false;
        switch (linkType)
        {
            case CREATURE_TO_CREATURE:
            {
                const CreatureData* slave = GetCreatureData(guidLow);
                if (!slave)
                {
                    TC_LOG_ERROR("sql.sql", "Couldn't get creature data for GUIDLow %u", guidLow);
                    error = true;
                    break;
                }

                const CreatureData* master = GetCreatureData(linkedGuidLow);
                if (!master)
                {
                    TC_LOG_ERROR("sql.sql", "Couldn't get creature data for GUIDLow %u", linkedGuidLow);
                    error = true;
                    break;
                }

                const MapEntry* const map = sMapStore.LookupEntry(master->mapid);
                if (!map || !map->Instanceable() || (master->mapid != slave->mapid))
                {
                    TC_LOG_ERROR("sql.sql", "Creature '%u' linking to '%u' on an unpermitted map.", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                if (!(master->spawnMask & slave->spawnMask))  // they must have a possibility to meet (normal/heroic difficulty)
                {
                    TC_LOG_ERROR("sql.sql", "LinkedRespawn: Creature '%u' linking to '%u' with not corresponding spawnMask", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                guid = MAKE_NEW_GUID(guidLow, slave->id, HIGHGUID_UNIT);
                linkedGuid = MAKE_NEW_GUID(linkedGuidLow, master->id, HIGHGUID_UNIT);
                break;
            }
            case CREATURE_TO_GO:
            {
                const CreatureData* slave = GetCreatureData(guidLow);
                if (!slave)
                {
                    TC_LOG_ERROR("sql.sql", "Couldn't get creature data for GUIDLow %u", guidLow);
                    error = true;
                    break;
                }

                const GameObjectData* master = GetGOData(linkedGuidLow);
                if (!master)
                {
                    TC_LOG_ERROR("sql.sql", "Couldn't get gameobject data for GUIDLow %u", linkedGuidLow);
                    error = true;
                    break;
                }

                const MapEntry* const map = sMapStore.LookupEntry(master->mapid);
                if (!map || !map->Instanceable() || (master->mapid != slave->mapid))
                {
                    TC_LOG_ERROR("sql.sql", "Creature '%u' linking to '%u' on an unpermitted map.", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                if (!(master->spawnMask & slave->spawnMask))  // they must have a possibility to meet (normal/heroic difficulty)
                {
                    TC_LOG_ERROR("sql.sql", "LinkedRespawn: Creature '%u' linking to '%u' with not corresponding spawnMask", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                guid = MAKE_NEW_GUID(guidLow, slave->id, HIGHGUID_UNIT);
                linkedGuid = MAKE_NEW_GUID(linkedGuidLow, master->id, HIGHGUID_GAMEOBJECT);
                break;
            }
            case GO_TO_GO:
            {
                const GameObjectData* slave = GetGOData(guidLow);
                if (!slave)
                {
                    TC_LOG_ERROR("sql.sql", "Couldn't get gameobject data for GUIDLow %u", guidLow);
                    error = true;
                    break;
                }

                const GameObjectData* master = GetGOData(linkedGuidLow);
                if (!master)
                {
                    TC_LOG_ERROR("sql.sql", "Couldn't get gameobject data for GUIDLow %u", linkedGuidLow);
                    error = true;
                    break;
                }

                const MapEntry* const map = sMapStore.LookupEntry(master->mapid);
                if (!map || !map->Instanceable() || (master->mapid != slave->mapid))
                {
                    TC_LOG_ERROR("sql.sql", "Creature '%u' linking to '%u' on an unpermitted map.", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                if (!(master->spawnMask & slave->spawnMask))  // they must have a possibility to meet (normal/heroic difficulty)
                {
                    TC_LOG_ERROR("sql.sql", "LinkedRespawn: Creature '%u' linking to '%u' with not corresponding spawnMask", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                guid = MAKE_NEW_GUID(guidLow, slave->id, HIGHGUID_GAMEOBJECT);
                linkedGuid = MAKE_NEW_GUID(linkedGuidLow, master->id, HIGHGUID_GAMEOBJECT);
                break;
            }
            case GO_TO_CREATURE:
            {
                const GameObjectData* slave = GetGOData(guidLow);
                if (!slave)
                {
                    TC_LOG_ERROR("sql.sql", "Couldn't get gameobject data for GUIDLow %u", guidLow);
                    error = true;
                    break;
                }

                const CreatureData* master = GetCreatureData(linkedGuidLow);
                if (!master)
                {
                    TC_LOG_ERROR("sql.sql", "Couldn't get creature data for GUIDLow %u", linkedGuidLow);
                    error = true;
                    break;
                }

                const MapEntry* const map = sMapStore.LookupEntry(master->mapid);
                if (!map || !map->Instanceable() || (master->mapid != slave->mapid))
                {
                    TC_LOG_ERROR("sql.sql", "Creature '%u' linking to '%u' on an unpermitted map.", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                if (!(master->spawnMask & slave->spawnMask))  // they must have a possibility to meet (normal/heroic difficulty)
                {
                    TC_LOG_ERROR("sql.sql", "LinkedRespawn: Creature '%u' linking to '%u' with not corresponding spawnMask", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                guid = MAKE_NEW_GUID(guidLow, slave->id, HIGHGUID_GAMEOBJECT);
                linkedGuid = MAKE_NEW_GUID(linkedGuidLow, master->id, HIGHGUID_UNIT);
                break;
            }
        }

        if (!error)
            _linkedRespawnStore[guid] = linkedGuid;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded " UI64FMTD " linked respawns in %u ms", uint64(_linkedRespawnStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

bool ObjectMgr::SetCreatureLinkedRespawn(uint32 guidLow, uint32 linkedGuidLow)
{
    if (!guidLow)
        return false;

    const CreatureData* master = GetCreatureData(guidLow);
    uint64 guid = MAKE_NEW_GUID(guidLow, master->id, HIGHGUID_UNIT);

    if (!linkedGuidLow) // we're removing the linking
    {
        _linkedRespawnStore.erase(guid);
        PreparedStatement *stmt = WorldDatabase.GetPreparedStatement(WORLD_DEL_CRELINKED_RESPAWN);
        stmt->setUInt32(0, guidLow);
        WorldDatabase.Execute(stmt);
        return true;
    }

    const CreatureData* slave = GetCreatureData(linkedGuidLow);

    const MapEntry* const map = sMapStore.LookupEntry(master->mapid);
    if (!map || !map->Instanceable() || (master->mapid != slave->mapid))
    {
        TC_LOG_ERROR("sql.sql", "Creature '%u' linking to '%u' on an unpermitted map.", guidLow, linkedGuidLow);
        return false;
    }

    if (!(master->spawnMask & slave->spawnMask))  // they must have a possibility to meet (normal/heroic difficulty)
    {
        TC_LOG_ERROR("sql.sql", "LinkedRespawn: Creature '%u' linking to '%u' with not corresponding spawnMask", guidLow, linkedGuidLow);
        return false;
    }

    uint64 linkedGuid = MAKE_NEW_GUID(linkedGuidLow, slave->id, HIGHGUID_UNIT);

    _linkedRespawnStore[guid] = linkedGuid;
    PreparedStatement *stmt = WorldDatabase.GetPreparedStatement(WORLD_REP_CREATURE_LINKED_RESPAWN);
    stmt->setUInt32(0, guidLow);
    stmt->setUInt32(1, linkedGuidLow);
    WorldDatabase.Execute(stmt);
    return true;
}

void ObjectMgr::LoadTempSummons()
{
    uint32 oldMSTime = getMSTime();

    //                                               0           1             2        3      4           5           6           7            8           9
    QueryResult result = WorldDatabase.Query("SELECT summonerId, summonerType, groupId, entry, position_x, position_y, position_z, orientation, summonType, summonTime FROM creature_summon_groups");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 temp summons. DB table `creature_summon_groups` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 summonerId               = fields[0].GetUInt32();
        SummonerType summonerType       = SummonerType(fields[1].GetUInt8());
        uint8 group                     = fields[2].GetUInt8();

        switch (summonerType)
        {
            case SUMMONER_TYPE_CREATURE:
                if (!GetCreatureTemplate(summonerId))
                {
                    TC_LOG_ERROR("sql.sql", "Table `creature_summon_groups` has summoner with non existing entry %u for creature summoner type, skipped.", summonerId);
                    continue;
                }
                break;
            case SUMMONER_TYPE_GAMEOBJECT:
                if (!GetGameObjectTemplate(summonerId))
                {
                    TC_LOG_ERROR("sql.sql", "Table `creature_summon_groups` has summoner with non existing entry %u for gameobject summoner type, skipped.", summonerId);
                    continue;
                }
                break;
            case SUMMONER_TYPE_MAP:
                if (!sMapStore.LookupEntry(summonerId))
                {
                    TC_LOG_ERROR("sql.sql", "Table `creature_summon_groups` has summoner with non existing entry %u for map summoner type, skipped.", summonerId);
                    continue;
                }
                break;
            default:
                TC_LOG_ERROR("sql.sql", "Table `creature_summon_groups` has unhandled summoner type %u for summoner %u, skipped.", summonerType, summonerId);
                continue;
        }

        TempSummonData data;
        data.entry                      = fields[3].GetUInt32();

        if (!GetCreatureTemplate(data.entry))
        {
            TC_LOG_ERROR("sql.sql", "Table `creature_summon_groups` has creature in group [Summoner ID: %u, Summoner Type: %u, Group ID: %u] with non existing creature entry %u, skipped.", summonerId, summonerType, group, data.entry);
            continue;
        }

        float posX                      = fields[4].GetFloat();
        float posY                      = fields[5].GetFloat();
        float posZ                      = fields[6].GetFloat();
        float orientation               = fields[7].GetFloat();

        data.pos.Relocate(posX, posY, posZ, orientation);

        data.type                       = TempSummonType(fields[8].GetUInt8());

        if (data.type > TEMPSUMMON_MANUAL_DESPAWN)
        {
            TC_LOG_ERROR("sql.sql", "Table `creature_summon_groups` has unhandled temp summon type %u in group [Summoner ID: %u, Summoner Type: %u, Group ID: %u] for creature entry %u, skipped.", data.type, summonerId, summonerType, group, data.entry);
            continue;
        }

        data.time                       = fields[9].GetUInt32();

        uint64 const key = MAKE_PAIR64(summonerId, MAKE_PAIR32(summonerType, group));
        _tempSummonDataStore[key].push_back(data);

        ++count;

    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u temp summons in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadCreatures()
{
    uint32 oldMSTime = getMSTime();

    //                                               0              1   2    3        4             5           6           7           8            9              10
    QueryResult result = WorldDatabase.Query("SELECT creature.guid, id, map, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, "
    //   11               12         13       14            15         16         17          18          19                20                   21                     22                     23
        "currentwaypoint, curhealth, curmana, MovementType, spawnMask, phaseMask, eventEntry, pool_entry, creature.npcflag, creature.unit_flags, creature.unit_flags2,  creature.dynamicflags, creature.isActive "
        "FROM creature "
        "LEFT OUTER JOIN game_event_creature ON creature.guid = game_event_creature.guid "
        "LEFT OUTER JOIN pool_creature ON creature.guid = pool_creature.guid");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 creatures. DB table `creature` is empty.");

        return;
    }

    // Build single time for check spawnmask
    std::map<uint32, uint32> spawnMasks;
    for (uint32 i = 0; i < sMapStore.GetNumRows(); ++i)
        if (sMapStore.LookupEntry(i))
            for (int k = 0; k < MAX_DIFFICULTY; ++k)
                if (GetMapDifficultyData(i, Difficulty(k)))
                    spawnMasks[i] |= (1 << k);

    _creatureDataStore.rehash(result->GetRowCount());
    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint8 index = 0;

        uint32 guid         = fields[index++].GetUInt32();
        uint32 entry        = fields[index++].GetUInt32();

        CreatureTemplate const* cInfo = GetCreatureTemplate(entry);
        if (!cInfo)
        {
            TC_LOG_ERROR("sql.sql", "Table `creature` has creature (GUID: %u) with non existing creature entry %u, skipped.", guid, entry);
            continue;
        }

        CreatureData& data = _creatureDataStore[guid];
        data.id             = entry;
        data.mapid          = fields[index++].GetUInt16();
        data.displayid      = fields[index++].GetUInt32();
        data.equipmentId    = fields[index++].GetInt32();
        data.posX           = fields[index++].GetFloat();
        data.posY           = fields[index++].GetFloat();
        data.posZ           = fields[index++].GetFloat();
        data.orientation    = fields[index++].GetFloat();
        data.spawntimesecs  = fields[index++].GetUInt32();
        data.spawndist      = fields[index++].GetFloat();
        data.currentwaypoint= fields[index++].GetUInt32();
        data.curhealth      = fields[index++].GetUInt32();
        data.curmana        = fields[index++].GetUInt32();
        data.movementType   = fields[index++].GetUInt8();
        data.spawnMask      = fields[index++].GetUInt32();
        data.phaseMask      = fields[index++].GetUInt32();
        int16 gameEvent     = fields[index++].GetInt8();
        uint32 PoolId       = fields[index++].GetUInt32();
        data.npcflag        = fields[index++].GetUInt32();
        data.unit_flags     = fields[index++].GetUInt32();
        data.unit_flags2    = fields[index++].GetUInt32();
        data.dynamicflags   = fields[index++].GetUInt32();
        data.isActive       = fields[index++].GetBool();

        MapEntry const* mapEntry = sMapStore.LookupEntry(data.mapid);
        if (!mapEntry)
        {
            TC_LOG_ERROR("sql.sql", "Table `creature` have creature (GUID: %u) that spawned at not existed map (Id: %u), skipped.", guid, data.mapid);
            continue;
        }

        if (data.spawnMask & ~spawnMasks[data.mapid])
            TC_LOG_ERROR("sql.sql", "Table `creature` have creature (GUID: %u) that have wrong spawn mask %u including not supported difficulty modes for map (Id: %u) spawnMasks[data.mapid]: %u.", guid, data.spawnMask, data.mapid, spawnMasks[data.mapid]);

        bool ok = true;
        for (uint32 diff = 0; diff < MAX_DIFFICULTY - 1 && ok; ++diff)
        {
            if (_difficultyEntries[diff].find(data.id) != _difficultyEntries[diff].end())
            {
                TC_LOG_ERROR("sql.sql", "Table `creature` have creature (GUID: %u) that listed as difficulty %u template (entry: %u) in `creature_template`, skipped.",
                    guid, diff + 1, data.id);
                ok = false;
            }
        }
        if (!ok)
            continue;

        // -1 no equipment, 0 use default
        if (data.equipmentId > 0)
        {
            if (!GetEquipmentInfo(data.equipmentId))
            {
                TC_LOG_ERROR("sql.sql", "Table `creature` have creature (Entry: %u) with equipment_id %u not found in table `creature_equip_template`, set to no equipment.", data.id, data.equipmentId);
                data.equipmentId = -1;
            }
        }

        if (cInfo->flags_extra & CREATURE_FLAG_EXTRA_INSTANCE_BIND)
        {
            if (!mapEntry || !mapEntry->IsDungeon())
                TC_LOG_ERROR("sql.sql", "Table `creature` have creature (GUID: %u Entry: %u) with `creature_template`.`flags_extra` including CREATURE_FLAG_EXTRA_INSTANCE_BIND but creature are not in instance.", guid, data.id);
        }

        if (data.spawndist < 0.0f)
        {
            TC_LOG_ERROR("sql.sql", "Table `creature` have creature (GUID: %u Entry: %u) with `spawndist`< 0, set to 0.", guid, data.id);
            data.spawndist = 0.0f;
        }
        else if (data.movementType == RANDOM_MOTION_TYPE)
        {
            if (data.spawndist == 0.0f)
            {
                TC_LOG_ERROR("sql.sql", "Table `creature` have creature (GUID: %u Entry: %u) with `MovementType`=1 (random movement) but with `spawndist`=0, replace by idle movement type (0).", guid, data.id);
                data.movementType = IDLE_MOTION_TYPE;
            }
        }
        else if (data.movementType == IDLE_MOTION_TYPE)
        {
            if (data.spawndist != 0.0f)
            {
                TC_LOG_ERROR("sql.sql", "Table `creature` have creature (GUID: %u Entry: %u) with `MovementType`=0 (idle) have `spawndist`<>0, set to 0.", guid, data.id);
                data.spawndist = 0.0f;
            }
        }

        if (data.phaseMask == 0)
        {
            TC_LOG_ERROR("sql.sql", "Table `creature` have creature (GUID: %u Entry: %u) with `phaseMask`=0 (not visible for anyone), set to 1.", guid, data.id);
            data.phaseMask = 1;
        }

        // Add to grid if not managed by the game event or pool system
        if (gameEvent == 0 && PoolId == 0)
            AddCreatureToGrid(guid, &data);

        ++count;

    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u creatures in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::AddCreatureToGrid(uint32 guid, CreatureData const* data)
{
    uint32 mask = data->spawnMask;
    for (uint32 i = 0; mask != 0; i++, mask >>= 1)
    {
        if (mask & 1)
        {
            CellCoord cellCoord = Trinity::ComputeCellCoord(data->posX, data->posY);
            MapObjectWriteGuard guard(_mapObjectGuidsStoreLock);
            CellObjectGuids& cell_guids = _mapObjectGuidsStore[MAKE_PAIR32(data->mapid, i)][cellCoord.GetId()];
            cell_guids.creatures.insert(guid);
        }
    }
}

void ObjectMgr::RemoveCreatureFromGrid(uint32 guid, CreatureData const* data)
{
    uint32 mask = data->spawnMask;
    for (uint32 i = 0; mask != 0; i++, mask >>= 1)
    {
        if (mask & 1)
        {
            CellCoord cellCoord = Trinity::ComputeCellCoord(data->posX, data->posY);
            MapObjectWriteGuard guard(_mapObjectGuidsStoreLock);
            CellObjectGuids& cell_guids = _mapObjectGuidsStore[MAKE_PAIR32(data->mapid, i)][cellCoord.GetId()];
            cell_guids.creatures.erase(guid);
        }
    }
}

uint32 ObjectMgr::AddGOData(uint32 entry, uint32 mapId, float x, float y, float z, float o, uint32 spawntimedelay, float rotation0, float rotation1, float rotation2, float rotation3)
{
    GameObjectTemplate const* goinfo = GetGameObjectTemplate(entry);
    if (!goinfo)
        return 0;

    Map* map = sMapMgr->CreateBaseMap(mapId);
    if (!map)
        return 0;

    uint32 guid = GenerateLowGuid(HIGHGUID_GAMEOBJECT);
    GameObjectData& data = NewGOData(guid);
    data.id             = entry;
    data.mapid          = mapId;
    data.posX           = x;
    data.posY           = y;
    data.posZ           = z;
    data.orientation    = o;
    data.rotation0      = rotation0;
    data.rotation1      = rotation1;
    data.rotation2      = rotation2;
    data.rotation3      = rotation3;
    data.spawntimesecs  = spawntimedelay;
    data.animprogress   = 100;
    data.spawnMask      = 1;
    data.go_state       = GO_STATE_READY;
    data.phaseMask      = PHASEMASK_NORMAL;
    data.artKit         = goinfo->type == GAMEOBJECT_TYPE_CAPTURE_POINT ? 21 : 0;
    data.dbData = false;

    AddGameobjectToGrid(guid, &data);

    // Spawn if necessary (loaded grids only)
    // We use spawn coords to spawn
    if (!map->Instanceable() && map->IsGridLoaded(x, y))
    {
        GameObject* go = new GameObject;
        if (!go->LoadGameObjectFromDB(guid, map))
        {
            TC_LOG_ERROR("misc", "AddGOData: cannot add gameobject entry %u to map", entry);
            delete go;
            return 0;
        }
    }

    TC_LOG_DEBUG("maps", "AddGOData: dbguid %u entry %u map %u x %f y %f z %f o %f", guid, entry, mapId, x, y, z, o);

    return guid;
}

bool ObjectMgr::MoveCreData(uint32 guid, uint32 mapId, Position pos)
{
    CreatureData& data = NewOrExistCreatureData(guid);
    if (!data.id)
        return false;

    RemoveCreatureFromGrid(guid, &data);
    if (data.posX == pos.GetPositionX() && data.posY == pos.GetPositionY() && data.posZ == pos.GetPositionZ())
        return true;
    data.posX = pos.GetPositionX();
    data.posY = pos.GetPositionY();
    data.posZ = pos.GetPositionZ();
    data.orientation = pos.GetOrientation();
    AddCreatureToGrid(guid, &data);

    // Spawn if necessary (loaded grids only)
    if (Map* map = sMapMgr->CreateBaseMap(mapId))
    {
        // We use spawn coords to spawn
        if (!map->Instanceable() && map->IsGridLoaded(data.posX, data.posY))
        {
            Creature* creature = new Creature;
            if (!creature->LoadCreatureFromDB(guid, map))
            {
                TC_LOG_ERROR("misc", "AddCreature: cannot add creature entry %u to map", guid);
                delete creature;
                return false;
            }
        }
    }
    return true;
}

uint32 ObjectMgr::AddCreData(uint32 entry, uint32 /*team*/, uint32 mapId, float x, float y, float z, float o, uint32 spawntimedelay)
{
    CreatureTemplate const* cInfo = GetCreatureTemplate(entry);
    if (!cInfo)
        return 0;

    uint32 level = cInfo->minlevel == cInfo->maxlevel ? cInfo->minlevel : urand(cInfo->minlevel, cInfo->maxlevel); // Only used for extracting creature base stats
    CreatureBaseStats const* stats = GetCreatureBaseStats(level, cInfo->unit_class);

    uint32 guid = GenerateLowGuid(HIGHGUID_UNIT);
    CreatureData& data = NewOrExistCreatureData(guid);
    data.id = entry;
    data.mapid = mapId;
    data.displayid = 0;
    data.equipmentId = cInfo->equipmentId;
    data.posX = x;
    data.posY = y;
    data.posZ = z;
    data.orientation = o;
    data.spawntimesecs = spawntimedelay;
    data.spawndist = 0;
    data.currentwaypoint = 0;
    data.curhealth = stats->GenerateHealth(cInfo);
    data.curmana = stats->GenerateMana(cInfo);
    data.movementType = cInfo->MovementType;
    data.spawnMask = 1;
    data.phaseMask = PHASEMASK_NORMAL;
    data.dbData = false;
    data.npcflag = cInfo->npcflag;
    data.unit_flags = cInfo->unit_flags;
    data.unit_flags2 = cInfo->unit_flags2;
    data.dynamicflags = cInfo->dynamicflags;

    AddCreatureToGrid(guid, &data);

    // Spawn if necessary (loaded grids only)
    if (Map* map = sMapMgr->CreateBaseMap(mapId))
    {
        // We use spawn coords to spawn
        if (!map->Instanceable() && !map->IsRemovalGrid(x, y))
        {
            Creature* creature = new Creature;
            if (!creature->LoadCreatureFromDB(guid, map))
            {
                TC_LOG_ERROR("misc", "AddCreature: cannot add creature entry %u to map", entry);
                delete creature;
                return 0;
            }
        }
    }

    return guid;
}

void ObjectMgr::LoadGameobjects()
{
    uint32 oldMSTime = getMSTime();

    uint32 count = 0;

    //                                                0               1   2    3           4           5           6
    QueryResult result = WorldDatabase.Query("SELECT gameobject.guid, id, map, position_x, position_y, position_z, orientation, "
    //   7          8          9          10         11             12            13     14        15         16         17          18
        "rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state, isActive, spawnMask, phaseMask, eventEntry, pool_entry "
        "FROM gameobject LEFT OUTER JOIN game_event_gameobject ON gameobject.guid = game_event_gameobject.guid "
        "LEFT OUTER JOIN pool_gameobject ON gameobject.guid = pool_gameobject.guid");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 gameobjects. DB table `gameobject` is empty.");

        return;
    }

    // build single time for check spawnmask
    std::map<uint32, uint32> spawnMasks;
    for (uint32 i = 0; i < sMapStore.GetNumRows(); ++i)
        if (sMapStore.LookupEntry(i))
            for (int k = 0; k < MAX_DIFFICULTY; ++k)
                if (GetMapDifficultyData(i, Difficulty(k)))
                    spawnMasks[i] |= (1 << k);

    _gameObjectDataStore.rehash(result->GetRowCount());

    do
    {
        Field* fields = result->Fetch();

        uint8 index = 0;

        uint32 guid         = fields[index++].GetUInt32();
        uint32 entry        = fields[index++].GetUInt32();

        GameObjectTemplate const* gInfo = GetGameObjectTemplate(entry);
        if (!gInfo)
        {
            TC_LOG_ERROR("sql.sql", "Table `gameobject` has gameobject (GUID: %u) with non existing gameobject entry %u, skipped.", guid, entry);
            continue;
        }

        if (!gInfo->displayId)
        {
            switch (gInfo->type)
            {
                case GAMEOBJECT_TYPE_TRAP:
                case GAMEOBJECT_TYPE_SPELL_FOCUS:
                    break;
                default:
                    TC_LOG_ERROR("sql.sql", "Gameobject (GUID: %u Entry %u GoType: %u) doesn't have a displayId (%u), not loaded.", guid, entry, gInfo->type, gInfo->displayId);
                    break;
            }
        }

        if (gInfo->displayId && !sGameObjectDisplayInfoStore.LookupEntry(gInfo->displayId))
        {
            TC_LOG_ERROR("sql.sql", "Gameobject (GUID: %u Entry %u GoType: %u) has an invalid displayId (%u), not loaded.", guid, entry, gInfo->type, gInfo->displayId);
            continue;
        }

        GameObjectData& data = _gameObjectDataStore[guid];

        data.id             = entry;
        data.mapid          = fields[index++].GetUInt16();
        data.posX           = fields[index++].GetFloat();
        data.posY           = fields[index++].GetFloat();
        data.posZ           = fields[index++].GetFloat();
        data.orientation    = fields[index++].GetFloat();
        data.rotation0      = fields[index++].GetFloat();
        data.rotation1      = fields[index++].GetFloat();
        data.rotation2      = fields[index++].GetFloat();
        data.rotation3      = fields[index++].GetFloat();
        data.spawntimesecs  = fields[index++].GetInt32();

        MapEntry const* mapEntry = sMapStore.LookupEntry(data.mapid);
        if (!mapEntry)
        {
            TC_LOG_ERROR("sql.sql", "Table `gameobject` has gameobject (GUID: %u Entry: %u) spawned on a non-existed map (Id: %u), skip", guid, data.id, data.mapid);
            continue;
        }

        if (data.spawntimesecs == 0 && gInfo->IsDespawnAtAction())
        {
            TC_LOG_ERROR("sql.sql", "Table `gameobject` has gameobject (GUID: %u Entry: %u) with `spawntimesecs` (0) value, but the gameobejct is marked as despawnable at action.", guid, data.id);
        }

        data.animprogress   = fields[index++].GetUInt8();
        data.artKit         = 0;

        uint32 go_state     = fields[index++].GetUInt8();
        if (go_state >= MAX_GO_STATE)
        {
            TC_LOG_ERROR("sql.sql", "Table `gameobject` has gameobject (GUID: %u Entry: %u) with invalid `state` (%u) value, skip", guid, data.id, go_state);
            continue;
        }

        data.go_state       = GOState(go_state);
        data.isActive       = fields[index++].GetBool();
        data.spawnMask      = fields[index++].GetUInt32();

        if (data.spawnMask & ~spawnMasks[data.mapid])
            TC_LOG_ERROR("sql.sql", "Table `gameobject` has gameobject (GUID: %u Entry: %u) that has wrong spawn mask %u including not supported difficulty modes for map (Id: %u), skip", guid, data.id, data.spawnMask, data.mapid);

        data.phaseMask      = fields[index++].GetUInt32();
        int16 gameEvent     = fields[index++].GetInt8();
        uint32 poolId       = fields[index++].GetUInt32();

        if (data.rotation2 < -1.0f || data.rotation2 > 1.0f)
        {
            TC_LOG_ERROR("sql.sql", "Table `gameobject` has gameobject (GUID: %u Entry: %u) with invalid rotation2 (%f) value, skip", guid, data.id, data.rotation2);
            continue;
        }

        if (data.rotation3 < -1.0f || data.rotation3 > 1.0f)
        {
            TC_LOG_ERROR("sql.sql", "Table `gameobject` has gameobject (GUID: %u Entry: %u) with invalid rotation3 (%f) value, skip", guid, data.id, data.rotation3);
            continue;
        }

        if (!MapManager::IsValidMapCoord(data.mapid, data.posX, data.posY, data.posZ, data.orientation))
        {
            TC_LOG_ERROR("sql.sql", "Table `gameobject` has gameobject (GUID: %u Entry: %u) with invalid coordinates, skip", guid, data.id);
            continue;
        }

        if (data.phaseMask == 0)
        {
            TC_LOG_ERROR("sql.sql", "Table `gameobject` has gameobject (GUID: %u Entry: %u) with `phaseMask`=0 (not visible for anyone), set to 1.", guid, data.id);
            data.phaseMask = 1;
        }

        // if not this is to be managed by GameEvent System or Pool system
        if (gameEvent == 0 && poolId == 0)
            AddGameobjectToGrid(guid, &data);

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %lu gameobjects in %u ms", (unsigned long)_gameObjectDataStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::loadGameObjectTemplateInvisibility()
{
    auto const start = getMSTime();
    loadInvisibilityHelper(gameObjectTemplateInvisibilityStore_, "SELECT entry, type, amount FROM gameobject_template_invisibility");
    TC_LOG_INFO("server.loading", ">> Loaded " SIZEFMTD " gameobject template invisibilities in %u ms",
                gameObjectTemplateInvisibilityStore_.size(), GetMSTimeDiffToNow(start));
}

void ObjectMgr::loadGameObjectInvisibility()
{
    auto const start = getMSTime();
    loadInvisibilityHelper(gameObjectInvisibilityStore_, "SELECT guid, type, amount FROM gameobject_invisibility");
    TC_LOG_INFO("server.loading", ">> Loaded " SIZEFMTD " gameobject invisibilities in %u ms",
                gameObjectInvisibilityStore_.size(), GetMSTimeDiffToNow(start));
}

void ObjectMgr::AddGameobjectToGrid(uint32 guid, GameObjectData const* data)
{
    uint32 mask = data->spawnMask;
    for (uint32 i = 0; mask != 0; i++, mask >>= 1)
    {
        if (mask & 1)
        {
            CellCoord cellCoord = Trinity::ComputeCellCoord(data->posX, data->posY);
            MapObjectWriteGuard guard(_mapObjectGuidsStoreLock);
            CellObjectGuids& cell_guids = _mapObjectGuidsStore[MAKE_PAIR32(data->mapid, i)][cellCoord.GetId()];
            cell_guids.gameobjects.insert(guid);
        }
    }
}

void ObjectMgr::RemoveGameobjectFromGrid(uint32 guid, GameObjectData const* data)
{
    uint32 mask = data->spawnMask;
    for (uint32 i = 0; mask != 0; i++, mask >>= 1)
    {
        if (mask & 1)
        {
            CellCoord cellCoord = Trinity::ComputeCellCoord(data->posX, data->posY);
            MapObjectWriteGuard guard(_mapObjectGuidsStoreLock);
            CellObjectGuids& cell_guids = _mapObjectGuidsStore[MAKE_PAIR32(data->mapid, i)][cellCoord.GetId()];
            cell_guids.gameobjects.erase(guid);
        }
    }
}

Player* ObjectMgr::GetPlayerByLowGUID(uint32 lowguid) const
{
    uint64 guid = MAKE_NEW_GUID(lowguid, 0, HIGHGUID_PLAYER);
    return ObjectAccessor::FindPlayer(guid);
}

// name must be checked to correctness (if received) before call this function
uint64 ObjectMgr::GetPlayerGUIDByName(std::string const &name) const
{
    uint64 guid = 0;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_BY_NAME);

    stmt->setString(0, name);

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
        guid = MAKE_NEW_GUID((*result)[0].GetUInt32(), 0, HIGHGUID_PLAYER);

    return guid;
}

bool ObjectMgr::GetPlayerNameByGUID(uint64 guid, std::string &name) const
{
    // prevent DB access for online player
    if (Player* player = ObjectAccessor::FindPlayer(guid))
    {
        name = player->GetName();
        return true;
    }

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_NAME);

    stmt->setUInt32(0, GUID_LOPART(guid));

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        name = (*result)[0].GetString();
        return true;
    }

    return false;
}

uint32 ObjectMgr::GetPlayerTeamByGUID(uint64 guid) const
{
    // prevent DB access for online player
    if (Player* player = ObjectAccessor::FindPlayer(guid))
    {
        return Player::TeamForRace(player->getRace());
    }

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_RACE);

    stmt->setUInt32(0, GUID_LOPART(guid));

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        uint8 race = (*result)[0].GetUInt8();
        return Player::TeamForRace(race);
    }

    return 0;
}

uint32 ObjectMgr::GetPlayerAccountIdByGUID(uint64 guid) const
{
    // prevent DB access for online player
    if (Player* player = ObjectAccessor::FindPlayer(guid))
    {
        return player->GetSession()->GetAccountId();
    }

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_BY_GUID);

    stmt->setUInt32(0, GUID_LOPART(guid));

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        uint32 acc = (*result)[0].GetUInt32();
        return acc;
    }

    return 0;
}

uint32 ObjectMgr::GetPlayerAccountIdByPlayerName(const std::string& name) const
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_BY_NAME);

    stmt->setString(0, name);

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        uint32 acc = (*result)[0].GetUInt32();
        return acc;
    }

    return 0;
}

void ObjectMgr::LoadItemLocales()
{
    uint32 oldMSTime = getMSTime();

    _itemLocaleStore.clear();                                 // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, name_loc1, description_loc1, name_loc2, description_loc2, name_loc3, description_loc3, name_loc4, description_loc4, name_loc5, description_loc5, name_loc6, description_loc6, name_loc7, description_loc7, name_loc8, description_loc8, name_loc9, description_loc9, name_loc10, description_loc10 FROM locales_item");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        ItemLocale& data = _itemLocaleStore[entry];

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
        {
            LocaleConstant locale = (LocaleConstant) i;
            AddLocaleString(fields[1 + 2 * (i - 1)].GetString(), locale, data.Name);
            AddLocaleString(fields[1 + 2 * (i - 1) + 1].GetString(), locale, data.Description);
        }
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %lu Item locale strings in %u ms", (unsigned long)_itemLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

void FillItemDamageFields(float* minDamage, float* maxDamage, float* dps, uint32 itemLevel, uint32 itemClass, uint32 itemSubClass, uint32 quality, uint32 delay, float statScalingFactor, uint32 inventoryType, uint32 flags2)
{
    *minDamage = *maxDamage = *dps = 0.0f;
    if (itemClass != ITEM_CLASS_WEAPON || quality > ITEM_QUALITY_ARTIFACT)
        return;

    DBCStorage<ItemDamageEntry>* store = NULL;
    // get the right store here
    if (inventoryType > 0xD + 13)
        return;

    switch (inventoryType)
    {
        case INVTYPE_AMMO:
            store = &sItemDamageAmmoStore;
            break;
        case INVTYPE_2HWEAPON:
            if (flags2 & ITEM_FLAGS_EXTRA_CASTER_WEAPON)
                store = &sItemDamageTwoHandCasterStore;
            else
                store = &sItemDamageTwoHandStore;
            break;
        case INVTYPE_RANGED:
        case INVTYPE_THROWN:
        case INVTYPE_RANGEDRIGHT:
            switch (itemSubClass)
            {
                case ITEM_SUBCLASS_WEAPON_WAND:
                    store = &sItemDamageWandStore;
                    break;
                case ITEM_SUBCLASS_WEAPON_THROWN:
                    store = &sItemDamageThrownStore;
                    break;
                case ITEM_SUBCLASS_WEAPON_BOW:
                case ITEM_SUBCLASS_WEAPON_GUN:
                case ITEM_SUBCLASS_WEAPON_CROSSBOW:
                    store = &sItemDamageRangedStore;
                    break;
                default:
                    return;
            }
            break;
        case INVTYPE_WEAPON:
        case INVTYPE_WEAPONMAINHAND:
        case INVTYPE_WEAPONOFFHAND:
            if (flags2 & ITEM_FLAGS_EXTRA_CASTER_WEAPON)
                store = &sItemDamageOneHandCasterStore;
            else
                store = &sItemDamageOneHandStore;
            break;
        default:
            return;
    }

    if (!store)
        return;

    ItemDamageEntry const* damageInfo = store->LookupEntry(itemLevel);
    if (!damageInfo)
        return;

    *dps = damageInfo->DPS[quality];
    float avgDamage = *dps * delay * 0.001f;
    *minDamage = (statScalingFactor * -0.5f + 1.0f) * avgDamage;
    *maxDamage = floor(float(avgDamage * (statScalingFactor * 0.5f + 1.0f) + 0.5f));
}

uint32 FillItemArmor(uint32 itemlevel, uint32 itemClass, uint32 itemSubclass, uint32 quality, uint32 inventoryType)
{
    if (quality > ITEM_QUALITY_ARTIFACT)
        return 0;

    if (itemClass != ITEM_CLASS_ARMOR)
        return 0;

    // all items but shields
    if (itemSubclass != ITEM_SUBCLASS_ARMOR_SHIELD)
    {
        ItemArmorQualityEntry const* armorQuality = sItemArmorQualityStore.LookupEntry(itemlevel);
        ItemArmorTotalEntry const* armorTotal = sItemArmorTotalStore.LookupEntry(itemlevel);
        if (!armorQuality || !armorTotal)
            return 0;

        if (inventoryType == INVTYPE_ROBE)
            inventoryType = INVTYPE_CHEST;

        ArmorLocationEntry const* location = sArmorLocationStore.LookupEntry(inventoryType);
        if (!location)
            return 0;

        if (itemSubclass < ITEM_SUBCLASS_ARMOR_CLOTH)
            return 0;

        return uint32(armorQuality->Value[quality] * armorTotal->Value[itemSubclass - 1] * location->Value[itemSubclass - 1] + 0.5f);
    }

    // shields
    ItemArmorShieldEntry const* shield = sItemArmorShieldStore.LookupEntry(itemlevel);
    if (!shield)
        return 0;

    return uint32(shield->Value[quality] + 0.5f);
}

uint32 FillMaxDurability(uint32 itemClass, uint32 itemSubClass, uint32 inventoryType, uint32 quality, uint32 itemLevel)
{
    if (itemClass != ITEM_CLASS_ARMOR && itemClass != ITEM_CLASS_WEAPON)
        return 0;

    static float const qualityMultipliers[MAX_ITEM_QUALITY] =
    {
        1.0f, 1.0f, 1.0f, 1.17f, 1.37f, 1.68f, 0.0f, 0.0f
    };

    static float const armorMultipliers[MAX_INVTYPE] =
    {
        0.00f, // INVTYPE_NON_EQUIP
        0.59f, // INVTYPE_HEAD
        0.00f, // INVTYPE_NECK
        0.59f, // INVTYPE_SHOULDERS
        0.00f, // INVTYPE_BODY
        1.00f, // INVTYPE_CHEST
        0.35f, // INVTYPE_WAIST
        0.75f, // INVTYPE_LEGS
        0.49f, // INVTYPE_FEET
        0.35f, // INVTYPE_WRISTS
        0.35f, // INVTYPE_HANDS
        0.00f, // INVTYPE_FINGER
        0.00f, // INVTYPE_TRINKET
        0.00f, // INVTYPE_WEAPON
        1.00f, // INVTYPE_SHIELD
        0.00f, // INVTYPE_RANGED
        0.00f, // INVTYPE_CLOAK
        0.00f, // INVTYPE_2HWEAPON
        0.00f, // INVTYPE_BAG
        0.00f, // INVTYPE_TABARD
        1.00f, // INVTYPE_ROBE
        0.00f, // INVTYPE_WEAPONMAINHAND
        0.00f, // INVTYPE_WEAPONOFFHAND
        0.00f, // INVTYPE_HOLDABLE
        0.00f, // INVTYPE_AMMO
        0.00f, // INVTYPE_THROWN
        0.00f, // INVTYPE_RANGEDRIGHT
        0.00f, // INVTYPE_QUIVER
        0.00f, // INVTYPE_RELIC
    };

    static float const weaponMultipliers[MAX_ITEM_SUBCLASS_WEAPON] =
    {
        0.89f, // ITEM_SUBCLASS_WEAPON_AXE
        1.03f, // ITEM_SUBCLASS_WEAPON_AXE2
        0.77f, // ITEM_SUBCLASS_WEAPON_BOW
        0.77f, // ITEM_SUBCLASS_WEAPON_GUN
        0.89f, // ITEM_SUBCLASS_WEAPON_MACE
        1.03f, // ITEM_SUBCLASS_WEAPON_MACE2
        1.03f, // ITEM_SUBCLASS_WEAPON_POLEARM
        0.89f, // ITEM_SUBCLASS_WEAPON_SWORD
        1.03f, // ITEM_SUBCLASS_WEAPON_SWORD2
        0.00f, // ITEM_SUBCLASS_WEAPON_Obsolete
        1.03f, // ITEM_SUBCLASS_WEAPON_STAFF
        0.00f, // ITEM_SUBCLASS_WEAPON_EXOTIC
        0.00f, // ITEM_SUBCLASS_WEAPON_EXOTIC2
        0.64f, // ITEM_SUBCLASS_WEAPON_FIST_WEAPON
        0.00f, // ITEM_SUBCLASS_WEAPON_MISCELLANEOUS
        0.64f, // ITEM_SUBCLASS_WEAPON_DAGGER
        0.64f, // ITEM_SUBCLASS_WEAPON_THROWN
        0.00f, // ITEM_SUBCLASS_WEAPON_SPEAR
        0.77f, // ITEM_SUBCLASS_WEAPON_CROSSBOW
        0.64f, // ITEM_SUBCLASS_WEAPON_WAND
        0.64f, // ITEM_SUBCLASS_WEAPON_FISHING_POLE
    };

    float levelPenalty = 1.0f;
    if (itemLevel <= 28)
        levelPenalty = 0.966f - float(28u - itemLevel) / 54.0f;

    if (itemClass == ITEM_CLASS_ARMOR)
    {
        if (inventoryType > INVTYPE_ROBE)
            return 0;

        return 5 * uint32(23.0f * qualityMultipliers[quality] * armorMultipliers[inventoryType] * levelPenalty + 0.5f);
    }

    return 5 * uint32(17.0f * qualityMultipliers[quality] * weaponMultipliers[itemSubClass] * levelPenalty + 0.5f);
};

void FillDisenchantFields(uint32* disenchantID, uint32* requiredDisenchantSkill, ItemTemplate const& itemTemplate)
{
    *disenchantID = 0;
    *(int32*)requiredDisenchantSkill = -1;
    if ((itemTemplate.Flags & (ITEM_PROTO_FLAG_CONJURED | ITEM_PROTO_FLAG_UNK6)) ||
        itemTemplate.Bonding == BIND_QUEST_ITEM || itemTemplate.Area || itemTemplate.Map ||
        itemTemplate.Stackable > 1 ||
        itemTemplate.Quality < ITEM_QUALITY_UNCOMMON || itemTemplate.Quality > ITEM_QUALITY_EPIC ||
        !(itemTemplate.Class == ITEM_CLASS_ARMOR || itemTemplate.Class == ITEM_CLASS_WEAPON) ||
        !(Item::GetSpecialPrice(&itemTemplate) || sItemCurrencyCostStore.LookupEntry(itemTemplate.ItemId)))
        return;

    for (uint32 i = 0; i < sItemDisenchantLootStore.GetNumRows(); ++i)
    {
        ItemDisenchantLootEntry const* disenchant = sItemDisenchantLootStore.LookupEntry(i);
        if (!disenchant)
            continue;

        if (disenchant->ItemClass == itemTemplate.Class &&
            disenchant->ItemQuality == itemTemplate.Quality &&
            disenchant->MinItemLevel <= itemTemplate.ItemLevel &&
            disenchant->MaxItemLevel >= itemTemplate.ItemLevel)
        {
            if (disenchant->Id == 60 || disenchant->Id == 61)   // epic item disenchant ilvl range 66-99 (classic)
            {
                if (itemTemplate.RequiredLevel > 60 || itemTemplate.RequiredSkillRank > 300)
                    continue;                                   // skip to epic item disenchant ilvl range 90-199 (TBC)
            }
            else if (disenchant->Id == 66 || disenchant->Id == 67)  // epic item disenchant ilvl range 90-199 (TBC)
            {
                if (itemTemplate.RequiredLevel <= 60 || (itemTemplate.RequiredSkill && itemTemplate.RequiredSkillRank <= 300))
                    continue;
            }

            *disenchantID = disenchant->Id;
            *requiredDisenchantSkill = disenchant->RequiredDisenchantSkill;
            return;
        }
    }
}

void ObjectMgr::LoadItemTemplates()
{
    uint32 oldMSTime = getMSTime();
    uint32 sparseCount = 0;
    uint32 dbCount = 0;

    for (uint32 itemId = 0; itemId < sItemSparseStore.GetNumRows(); ++itemId)
    {
        ItemSparseEntry const* sparse = sItemSparseStore.LookupEntry(itemId);
        ItemEntry const* db2Data = sItemStore.LookupEntry(itemId);
        if (!sparse || !db2Data)
            continue;

        ItemTemplate& itemTemplate = _itemTemplateStore[itemId];

        itemTemplate.ItemId = itemId;
        itemTemplate.Class = db2Data->Class;
        itemTemplate.SubClass = db2Data->SubClass;
        itemTemplate.SoundOverrideSubclass = db2Data->SoundOverrideSubclass;
        itemTemplate.Name1 = sparse->Name;
        itemTemplate.DisplayInfoID = db2Data->DisplayId;
        itemTemplate.Quality = sparse->Quality;
        itemTemplate.Flags = sparse->Flags;
        itemTemplate.Flags2 = sparse->Flags2;
        itemTemplate.Flags3 = sparse->Unk540_1;
        itemTemplate.Unk430_1 = sparse->Unk430_1;
        itemTemplate.Unk430_2 = sparse->Unk430_2;
        itemTemplate.BuyCount = std::max(sparse->BuyCount, 1u);
        itemTemplate.BuyPrice = sparse->BuyPrice;
        itemTemplate.SellPrice = sparse->SellPrice;
        itemTemplate.InventoryType = db2Data->InventoryType;
        itemTemplate.AllowableClass = sparse->AllowableClass;
        itemTemplate.AllowableRace = sparse->AllowableRace;
        itemTemplate.ItemLevel = sparse->ItemLevel;
        itemTemplate.RequiredLevel = sparse->RequiredLevel;
        itemTemplate.RequiredSkill = sparse->RequiredSkill;
        itemTemplate.RequiredSkillRank = sparse->RequiredSkillRank;
        itemTemplate.RequiredSpell = sparse->RequiredSpell;
        itemTemplate.RequiredHonorRank = sparse->RequiredHonorRank;
        itemTemplate.RequiredCityRank = sparse->RequiredCityRank;
        itemTemplate.RequiredReputationFaction = sparse->RequiredReputationFaction;
        itemTemplate.RequiredReputationRank = sparse->RequiredReputationRank;
        itemTemplate.MaxCount = sparse->MaxCount;
        itemTemplate.Stackable = sparse->Stackable;
        itemTemplate.ContainerSlots = sparse->ContainerSlots;
        for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
        {
            itemTemplate.ItemStat[i].ItemStatType = sparse->ItemStatType[i];
            itemTemplate.ItemStat[i].ItemStatValue = sparse->ItemStatValue[i];
            itemTemplate.ItemStat[i].ItemStatUnk1 = sparse->ItemStatUnk1[i];
            itemTemplate.ItemStat[i].ItemStatUnk2 = sparse->ItemStatUnk2[i];
        }

        itemTemplate.ScalingStatDistribution = sparse->ScalingStatDistribution;

        // cache item damage
        FillItemDamageFields(&itemTemplate.DamageMin, &itemTemplate.DamageMax, &itemTemplate.DPS, sparse->ItemLevel,
                             db2Data->Class, db2Data->SubClass, sparse->Quality, sparse->Delay, sparse->StatScalingFactor,
                             sparse->InventoryType, sparse->Flags2);

        itemTemplate.DamageType = sparse->DamageType;
        itemTemplate.Armor = FillItemArmor(sparse->ItemLevel, db2Data->Class, db2Data->SubClass, sparse->Quality, sparse->InventoryType);
        itemTemplate.Delay = sparse->Delay;
        itemTemplate.RangedModRange = sparse->RangedModRange;
        for (uint32 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
        {
            itemTemplate.Spells[i].SpellId = sparse->SpellId[i];
            itemTemplate.Spells[i].SpellTrigger = sparse->SpellTrigger[i];
            itemTemplate.Spells[i].SpellCharges = sparse->SpellCharges[i];
            itemTemplate.Spells[i].SpellCooldown = sparse->SpellCooldown[i];
            itemTemplate.Spells[i].SpellCategory = sparse->SpellCategory[i];
            itemTemplate.Spells[i].SpellCategoryCooldown = sparse->SpellCategoryCooldown[i];
        }

        itemTemplate.SpellPPMRate = 0.0f;
        itemTemplate.Bonding = sparse->Bonding;
        itemTemplate.Description = sparse->Description;
        itemTemplate.PageText = sparse->PageText;
        itemTemplate.LanguageID = sparse->LanguageID;
        itemTemplate.PageMaterial = sparse->PageMaterial;
        itemTemplate.StartQuest = sparse->StartQuest;
        itemTemplate.LockID = sparse->LockID;
        itemTemplate.Material = sparse->Material;
        itemTemplate.Sheath = sparse->Sheath;
        itemTemplate.RandomProperty = sparse->RandomProperty;
        itemTemplate.RandomSuffix = sparse->RandomSuffix;
        itemTemplate.ItemSet = sparse->ItemSet;
        itemTemplate.MaxDurability = FillMaxDurability(db2Data->Class, db2Data->SubClass, sparse->InventoryType, sparse->Quality, sparse->ItemLevel);
        itemTemplate.Area = sparse->Area;
        itemTemplate.Map = sparse->Map;
        itemTemplate.BagFamily = sparse->BagFamily;
        itemTemplate.TotemCategory = sparse->TotemCategory;
        for (uint32 i = 0; i < MAX_ITEM_PROTO_SOCKETS; ++i)
        {
            itemTemplate.Socket[i].Color = sparse->Color[i];
            itemTemplate.Socket[i].Content = sparse->Content[i];
        }

        itemTemplate.socketBonus = sparse->SocketBonus;
        itemTemplate.GemProperties = sparse->GemProperties;
        FillDisenchantFields(&itemTemplate.DisenchantID, &itemTemplate.RequiredDisenchantSkill, itemTemplate);

        itemTemplate.ArmorDamageModifier = sparse->ArmorDamageModifier;
        itemTemplate.Duration = sparse->Duration;
        itemTemplate.ItemLimitCategory = sparse->ItemLimitCategory;
        itemTemplate.HolidayId = sparse->HolidayId;
        itemTemplate.StatScalingFactor = sparse->StatScalingFactor;
        itemTemplate.CurrencySubstitutionId = sparse->CurrencySubstitutionId;
        itemTemplate.CurrencySubstitutionCount = sparse->CurrencySubstitutionCount;
        itemTemplate.ScriptId = 0;
        itemTemplate.FoodType = 0;
        itemTemplate.MinMoneyLoot = 0;
        itemTemplate.MaxMoneyLoot = 0;
        ++sparseCount;
    }

    // Load missing items from item_template AND overwrite data from Item-sparse.db2 (item_template is supposed to contain Item-sparse.adb data)
    //                                               0      1      2         3     4     5          6        7      8           9         10        11        12        13
    QueryResult result = WorldDatabase.Query("SELECT entry, Class, SubClass, SoundOverrideSubClass, Name, DisplayId, Quality, Flags, FlagsExtra, Unk430_1, Unk430_2, BuyCount, BuyPrice, SellPrice, "
    //                                        14             15              16             17         18             19             20                 21
                                             "InventoryType, AllowableClass, AllowableRace, ItemLevel, RequiredLevel, RequiredSkill, RequiredSkillRank, RequiredSpell, "
    //                                        22                 23                24                         25                      26        27         28
                                             "RequiredHonorRank, RequiredCityRank, RequiredReputationFaction, RequiredReputationRank, MaxCount, Stackable, ContainerSlots, "
    //                                        29          30           31           32           33          34           35           36
                                             "stat_type1, stat_value1, stat_unk1_1, stat_unk2_1, stat_type2, stat_value2, stat_unk1_2, stat_unk2_2, "
    //                                        37          38           39           40           41          42           43           44
                                             "stat_type3, stat_value3, stat_unk1_3, stat_unk2_3, stat_type4, stat_value4, stat_unk1_4, stat_unk2_4, "
    //                                        45          46           47           48           49          50           51           52
                                             "stat_type5, stat_value5, stat_unk1_5, stat_unk2_5, stat_type6, stat_value6, stat_unk1_6, stat_unk2_6, "
    //                                        53          54           55           56           57          58           59           60
                                             "stat_type7, stat_value7, stat_unk1_7, stat_unk2_7, stat_type8, stat_value8, stat_unk1_8, stat_unk2_8, "
    //                                        61          62           63           64           65           66            67            68
                                             "stat_type9, stat_value9, stat_unk1_9, stat_unk2_9, stat_type10, stat_value10, stat_unk1_10, stat_unk2_10, "
    //                                        69                       70          71     72
                                             "ScalingStatDistribution, DamageType, Delay, RangedModRange, "
    //                                        73         74              75              76               77               78
                                             "spellid_1, spelltrigger_1, spellcharges_1, spellcooldown_1, spellcategory_1, spellcategorycooldown_1, "
    //                                        79         80              81              82               83               84
                                             "spellid_2, spelltrigger_2, spellcharges_2, spellcooldown_2, spellcategory_2, spellcategorycooldown_2, "
    //                                        85         86              87              88               89               90
                                             "spellid_3, spelltrigger_3, spellcharges_3, spellcooldown_3, spellcategory_3, spellcategorycooldown_3, "
    //                                        91         92              93              94               95               96
                                             "spellid_4, spelltrigger_4, spellcharges_4, spellcooldown_4, spellcategory_4, spellcategorycooldown_4, "
    //                                        97         98              99              100              101              102
                                             "spellid_5, spelltrigger_5, spellcharges_5, spellcooldown_5, spellcategory_5, spellcategorycooldown_5, "
    //                                        103      104          105       106         107           108         109     110
                                             "Bonding, Description, PageText, LanguageID, PageMaterial, StartQuest, LockID, Material, "
    //                                        111     112             113           114      115   116  117        118
                                             "Sheath, RandomProperty, RandomSuffix, ItemSet, Area, Map, BagFamily, TotemCategory, "
    //                                        119            120              121            122              123            124              125
                                             "SocketColor_1, SocketContent_1, SocketColor_2, SocketContent_2, SocketColor_3, SocketContent_3, SocketBonus, "
    //                                        126            127                  128       129                130        131
                                             "GemProperties, ArmorDamageModifier, Duration, ItemLimitCategory, HolidayId, StatScalingFactor, "
    //                                        132                     133
                                             "CurrencySubstitutionId, CurrencySubstitutionCount "
                                             "FROM item_template");

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 itemId = fields[0].GetUInt32();
            if (_itemTemplateStore.find(itemId) != _itemTemplateStore.end())
                continue;

            ItemTemplate& itemTemplate = _itemTemplateStore[itemId];

            itemTemplate.ItemId                    = itemId;
            itemTemplate.Class                     = uint32(fields[1].GetUInt8());
            itemTemplate.SubClass                  = uint32(fields[2].GetUInt8());
            itemTemplate.SoundOverrideSubclass     = int32(fields[3].GetInt8());
            itemTemplate.Name1                     = fields[4].GetString();
            itemTemplate.DisplayInfoID             = fields[5].GetUInt32();
            itemTemplate.Quality                   = uint32(fields[6].GetUInt8());
            itemTemplate.Flags                     = uint32(fields[7].GetInt64());
            itemTemplate.Flags2                    = fields[8].GetUInt32();
            itemTemplate.Unk430_1                  = fields[9].GetFloat();
            itemTemplate.Unk430_2                  = fields[10].GetFloat();
            itemTemplate.BuyCount                  = uint32(fields[11].GetUInt8());
            itemTemplate.BuyPrice                  = int32(fields[12].GetInt64());
            itemTemplate.SellPrice                 = fields[13].GetUInt32();

            itemTemplate.InventoryType             = uint32(fields[14].GetUInt8());
            itemTemplate.AllowableClass            = fields[15].GetInt32();
            itemTemplate.AllowableRace             = fields[16].GetInt32();
            itemTemplate.ItemLevel                 = uint32(fields[17].GetUInt16());
            itemTemplate.RequiredLevel             = uint32(fields[18].GetUInt8());
            itemTemplate.RequiredSkill             = uint32(fields[19].GetUInt16());
            itemTemplate.RequiredSkillRank         = uint32(fields[20].GetUInt16());
            itemTemplate.RequiredSpell             = fields[21].GetUInt32();
            itemTemplate.RequiredHonorRank         = fields[22].GetUInt32();
            itemTemplate.RequiredCityRank          = fields[23].GetUInt32();
            itemTemplate.RequiredReputationFaction = uint32(fields[24].GetUInt16());
            itemTemplate.RequiredReputationRank    = uint32(fields[25].GetUInt16());
            itemTemplate.MaxCount                  = fields[26].GetInt32();
            itemTemplate.Stackable                 = fields[27].GetInt32();
            itemTemplate.ContainerSlots            = uint32(fields[28].GetUInt8());
            for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
            {
                itemTemplate.ItemStat[i].ItemStatType  = uint32(fields[29 + i * 4 + 0].GetUInt8());
                itemTemplate.ItemStat[i].ItemStatValue = int32(fields[29 + i * 4 + 1].GetInt16());
                itemTemplate.ItemStat[i].ItemStatUnk1  = fields[29 + i * 4 + 2].GetInt32();
                itemTemplate.ItemStat[i].ItemStatUnk2  = fields[29 + i * 4 + 3].GetInt32();
            }

            itemTemplate.ScalingStatDistribution = uint32(fields[69].GetUInt16());

            // cache item damage
            FillItemDamageFields(&itemTemplate.DamageMin, &itemTemplate.DamageMax, &itemTemplate.DPS, itemTemplate.ItemLevel,
                                 itemTemplate.Class, itemTemplate.SubClass, itemTemplate.Quality, fields[71].GetUInt16(),
                                 fields[131].GetFloat(), itemTemplate.InventoryType, itemTemplate.Flags2);

            itemTemplate.DamageType                = fields[70].GetUInt8();
            itemTemplate.Armor                     = FillItemArmor(itemTemplate.ItemLevel, itemTemplate.Class,
                                                                   itemTemplate.SubClass, itemTemplate.Quality,
                                                                   itemTemplate.InventoryType);

            itemTemplate.Delay                     = fields[71].GetUInt16();
            itemTemplate.RangedModRange            = fields[72].GetFloat();
            for (uint32 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
            {
                itemTemplate.Spells[i].SpellId               = fields[73 + 6 * i + 0].GetInt32();
                itemTemplate.Spells[i].SpellTrigger          = uint32(fields[73 + 6 * i + 1].GetUInt8());
                itemTemplate.Spells[i].SpellCharges          = int32(fields[73 + 6 * i + 2].GetInt16());
                itemTemplate.Spells[i].SpellCooldown         = fields[73 + 6 * i + 3].GetInt32();
                itemTemplate.Spells[i].SpellCategory         = uint32(fields[73 + 6 * i + 4].GetUInt16());
                itemTemplate.Spells[i].SpellCategoryCooldown = fields[73 + 6 * i + 5].GetInt32();
            }

            itemTemplate.SpellPPMRate   = 0.0f;
            itemTemplate.Bonding        = uint32(fields[103].GetUInt8());
            itemTemplate.Description    = fields[104].GetString();
            itemTemplate.PageText       = fields[105].GetUInt32();
            itemTemplate.LanguageID     = uint32(fields[106].GetUInt8());
            itemTemplate.PageMaterial   = uint32(fields[107].GetUInt8());
            itemTemplate.StartQuest     = fields[108].GetUInt32();
            itemTemplate.LockID         = fields[109].GetUInt32();
            itemTemplate.Material       = int32(fields[110].GetInt8());
            itemTemplate.Sheath         = uint32(fields[111].GetUInt8());
            itemTemplate.RandomProperty = fields[112].GetUInt32();
            itemTemplate.RandomSuffix   = fields[113].GetInt32();
            itemTemplate.ItemSet        = fields[114].GetUInt32();
            itemTemplate.MaxDurability  = FillMaxDurability(itemTemplate.Class, itemTemplate.SubClass,
                itemTemplate.InventoryType, itemTemplate.Quality, itemTemplate.ItemLevel);

            itemTemplate.Area           = fields[115].GetUInt32();
            itemTemplate.Map            = uint32(fields[116].GetUInt16());
            itemTemplate.BagFamily      = fields[117].GetUInt32();
            itemTemplate.TotemCategory  = fields[118].GetUInt32();
            for (uint32 i = 0; i < MAX_ITEM_PROTO_SOCKETS; ++i)
            {
                itemTemplate.Socket[i].Color   = uint32(fields[119 + i*2].GetUInt8());
                itemTemplate.Socket[i].Content = fields[119 + i * 2 + 1].GetUInt32();
            }

            itemTemplate.socketBonus         = fields[125].GetUInt32();
            itemTemplate.GemProperties       = fields[126].GetUInt32();
            FillDisenchantFields(&itemTemplate.DisenchantID, &itemTemplate.RequiredDisenchantSkill, itemTemplate);

            itemTemplate.ArmorDamageModifier       = fields[127].GetFloat();
            itemTemplate.Duration                  = fields[128].GetUInt32();
            itemTemplate.ItemLimitCategory         = uint32(fields[129].GetInt16());
            itemTemplate.HolidayId                 = fields[130].GetUInt32();
            itemTemplate.StatScalingFactor         = fields[131].GetFloat();
            itemTemplate.CurrencySubstitutionId    = fields[132].GetInt32();
            itemTemplate.CurrencySubstitutionCount = fields[133].GetInt32();
            itemTemplate.ScriptId                  = 0;
            itemTemplate.FoodType                  = 0;
            itemTemplate.MinMoneyLoot              = 0;
            itemTemplate.MaxMoneyLoot              = 0;
            ++dbCount;
        }
        while (result->NextRow());
    }

    // Check if item templates for DBC referenced character start outfit are present
    std::set<uint32> notFoundOutfit;
    for (uint32 i = 1; i < sCharStartOutfitStore.GetNumRows(); ++i)
    {
        CharStartOutfitEntry const* entry = sCharStartOutfitStore.LookupEntry(i);
        if (!entry)
            continue;

        for (int j = 0; j < MAX_OUTFIT_ITEMS; ++j)
        {
            if (entry->ItemId[j] <= 0)
                continue;

            uint32 item_id = entry->ItemId[j];

            if (!GetItemTemplate(item_id))
                notFoundOutfit.insert(item_id);
        }
    }

    for (std::set<uint32>::const_iterator itr = notFoundOutfit.begin(); itr != notFoundOutfit.end(); ++itr)
        TC_LOG_ERROR("sql.sql", "Item (Entry: %u) does not exist in `item_template` but is referenced in `CharStartOutfit.dbc`", *itr);

    TC_LOG_INFO("server.loading", ">> Loaded %u item templates from Item-sparse.db2 and %u from database in %u ms", sparseCount, dbCount, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadItemSpecialisation()
{
    uint32 oldMSTime = getMSTime();
    uint32 count = 0;

    QueryResult result = WorldDatabase.Query("SELECT id, specialisation FROM item_specialisation");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 itemId           = fields[0].GetUInt32();
            uint32 specialisationId = fields[1].GetUInt32();

            if (!GetItemTemplate(itemId))
            {
                TC_LOG_ERROR("sql.sql", "Item %u specified in `item_specialisation` does not exist, skipped.", itemId);
                continue;
            }

            auto specialisation = sChrSpecializationsStore.LookupEntry(specialisationId);
            if (!specialisation)
            {
                TC_LOG_ERROR("sql.sql", "Class specialisation %u specified in `item_specialisation` does not exist, skipped.", specialisationId);
                continue;
            }

            auto itemSpecialisation = new ItemSpecialisation();
            itemSpecialisation->Class          = specialisation->classId;
            itemSpecialisation->Specialisation = specialisationId;

            auto itemSpecialisationEntry = m_itemSpecialisation.find(itemId);
            if (itemSpecialisationEntry == m_itemSpecialisation.end())
            {
                m_itemSpecialisation.insert(std::make_pair(itemId, ItemSpecialisationSet()));
                itemSpecialisationEntry = m_itemSpecialisation.find(itemId);
            }

            itemSpecialisationEntry->second.insert(itemSpecialisation);
        }
        while (result->NextRow());
    }
}

void ObjectMgr::LoadSpellAreaTriggerTemplates()
{
    uint32 oldMSTime = getMSTime();
    uint32 count = 0;

    _areaTriggerTemplateStore.clear();
    QueryResult result = WorldDatabase.Query("SELECT Entry, Flags, CollisionType, Radius, ScaleX, ScaleY, ScriptName FROM spell_areatrigger_template");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            AreaTriggerTemplate& atTemplate = _areaTriggerTemplateStore[fields[0].GetUInt32()];
            atTemplate.Entry = fields[0].GetUInt32();
            atTemplate.Flags = fields[1].GetUInt32();
            atTemplate.CollisionType = fields[2].GetUInt32();
            atTemplate.Radius = fields[3].GetFloat();
            atTemplate.ScaleX = fields[4].GetFloat();
            atTemplate.ScaleY = fields[5].GetFloat();
            atTemplate.ScriptId = GetScriptId(fields[6].GetCString());
            ++count;
        }
        while (result->NextRow());
    }
    TC_LOG_INFO("server.loading", ">> Loaded %u area trigger templates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadItemTemplateAddon()
{
    uint32 oldMSTime = getMSTime();
    uint32 count = 0;

    QueryResult result = WorldDatabase.Query("SELECT Id, FlagsCu, FoodType, MinMoneyLoot, MaxMoneyLoot, SpellPPMChance FROM item_template_addon");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 itemId = fields[0].GetUInt32();
            if (!GetItemTemplate(itemId))
            {
                TC_LOG_ERROR("sql.sql", "Item %u specified in `item_template_addon` does not exist, skipped.", itemId);
                continue;
            }

            uint32 minMoneyLoot = fields[3].GetUInt32();
            uint32 maxMoneyLoot = fields[4].GetUInt32();
            if (minMoneyLoot > maxMoneyLoot)
            {
                TC_LOG_ERROR("sql.sql", "Minimum money loot specified in `item_template_addon` for item %u was greater than maximum amount, swapping.", itemId);
                std::swap(minMoneyLoot, maxMoneyLoot);
            }
            ItemTemplate& itemTemplate = _itemTemplateStore[itemId];
            itemTemplate.FlagsCu = fields[1].GetUInt32();
            itemTemplate.FoodType = fields[2].GetUInt8();
            itemTemplate.MinMoneyLoot = minMoneyLoot;
            itemTemplate.MaxMoneyLoot = maxMoneyLoot;
            itemTemplate.SpellPPMRate = fields[5].GetFloat();
            ++count;
        }
        while (result->NextRow());
    }
    TC_LOG_INFO("server.loading", ">> Loaded %u item addon templates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadItemScriptNames()
{
    uint32 oldMSTime = getMSTime();
    uint32 count = 0;

    QueryResult result = WorldDatabase.Query("SELECT Id, ScriptName FROM item_script_names");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 itemId = fields[0].GetUInt32();
            if (!GetItemTemplate(itemId))
            {
                TC_LOG_ERROR("sql.sql", "Item %u specified in `item_script_names` does not exist, skipped.", itemId);
                continue;
            }

            _itemTemplateStore[itemId].ScriptId = GetScriptId(fields[1].GetCString());
            ++count;
        }
        while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u item script names in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}
ItemTemplate const* ObjectMgr::GetItemTemplate(uint32 entry)
{
    ItemTemplateContainer::const_iterator itr = _itemTemplateStore.find(entry);
    if (itr != _itemTemplateStore.end())
        return &(itr->second);
    return NULL;
}

void ObjectMgr::LoadVehicleTemplateAccessories()
{
    uint32 oldMSTime = getMSTime();

    _vehicleTemplateAccessoryStore.clear();                           // needed for reload case

    uint32 count = 0;

    //                                                  0             1              2          3           4             5
    QueryResult result = WorldDatabase.Query("SELECT `entry`, `accessory_entry`, `seat_id`, `minion`, `summontype`, `summontimer` FROM `vehicle_template_accessory`");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 vehicle template accessories. DB table `vehicle_template_accessory` is empty.");

        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 uiEntry      = fields[0].GetUInt32();
        uint32 uiAccessory  = fields[1].GetUInt32();
        int8   uiSeat       = int8(fields[2].GetInt8());
        bool   bMinion      = fields[3].GetBool();
        uint8  uiSummonType = fields[4].GetUInt8();
        uint32 uiSummonTimer= fields[5].GetUInt32();

        if (!sObjectMgr->GetCreatureTemplate(uiEntry))
        {
            TC_LOG_ERROR("sql.sql", "Table `vehicle_template_accessory`: creature template entry %u does not exist.", uiEntry);
            continue;
        }

        if (!sObjectMgr->GetCreatureTemplate(uiAccessory))
        {
            TC_LOG_ERROR("sql.sql", "Table `vehicle_template_accessory`: Accessory %u does not exist.", uiAccessory);
            continue;
        }

        if (_spellClickInfoStore.find(uiEntry) == _spellClickInfoStore.end())
        {
            TC_LOG_ERROR("sql.sql", "Table `vehicle_template_accessory`: creature template entry %u has no data in npc_spellclick_spells", uiEntry);
            continue;
        }

        _vehicleTemplateAccessoryStore[uiEntry].push_back(VehicleAccessory(uiAccessory, uiSeat, bMinion, uiSummonType, uiSummonTimer));

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u Vehicle Template Accessories in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadVehicleAccessories()
{
    uint32 oldMSTime = getMSTime();

    _vehicleAccessoryStore.clear();                           // needed for reload case

    uint32 count = 0;

    //                                                  0             1             2          3           4             5
    QueryResult result = WorldDatabase.Query("SELECT `guid`, `accessory_entry`, `seat_id`, `minion`, `summontype`, `summontimer` FROM `vehicle_accessory`");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 Vehicle Accessories in %u ms", GetMSTimeDiffToNow(oldMSTime));
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 uiGUID       = fields[0].GetUInt32();
        uint32 uiAccessory  = fields[1].GetUInt32();
        int8   uiSeat       = int8(fields[2].GetInt16());
        bool   bMinion      = fields[3].GetBool();
        uint8  uiSummonType = fields[4].GetUInt8();
        uint32 uiSummonTimer= fields[5].GetUInt32();

        if (!sObjectMgr->GetCreatureTemplate(uiAccessory))
        {
            TC_LOG_ERROR("sql.sql", "Table `vehicle_accessory`: Accessory %u does not exist.", uiAccessory);
            continue;
        }

        _vehicleAccessoryStore[uiGUID].push_back(VehicleAccessory(uiAccessory, uiSeat, bMinion, uiSummonType, uiSummonTimer));

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u Vehicle Accessories in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadPetLevelInfo()
{
    uint32 oldMSTime = getMSTime();

    //                                                 0               1      2   3     4    5    6    7     8    9
    QueryResult result = WorldDatabase.Query("SELECT creature_entry, level, hp, mana, str, agi, sta, inte, spi, armor FROM pet_levelstats");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 level pet stats definitions. DB table `pet_levelstats` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 creature_id = fields[0].GetUInt32();
        if (!sObjectMgr->GetCreatureTemplate(creature_id))
        {
            TC_LOG_ERROR("sql.sql", "Wrong creature id %u in `pet_levelstats` table, ignoring.", creature_id);
            continue;
        }

        uint32 current_level = fields[1].GetUInt8();
        if (current_level > sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
        {
            if (current_level > STRONG_MAX_LEVEL)        // hardcoded level maximum
                TC_LOG_ERROR("sql.sql", "Wrong (> %u) level %u in `pet_levelstats` table, ignoring.", STRONG_MAX_LEVEL, current_level);
            else
            {
                TC_LOG_INFO("misc", "Unused (> MaxPlayerLevel in worldserver.conf) level %u in `pet_levelstats` table, ignoring.", current_level);
                ++count;                                // make result loading percent "expected" correct in case disabled detail mode for example.
            }
            continue;
        }
        else if (current_level < 1)
        {
            TC_LOG_ERROR("sql.sql", "Wrong (<1) level %u in `pet_levelstats` table, ignoring.", current_level);
            continue;
        }

        PetLevelInfo*& pInfoMapEntry = _petInfoStore[creature_id];

        if (pInfoMapEntry == NULL)
            pInfoMapEntry = new PetLevelInfo[sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL)];

        // data for level 1 stored in [0] array element, ...
        PetLevelInfo* pLevelInfo = &pInfoMapEntry[current_level-1];

        pLevelInfo->health = fields[2].GetUInt32();
        pLevelInfo->mana   = fields[3].GetUInt32();
        pLevelInfo->armor  = fields[9].GetUInt32();

        for (int i = 0; i < MAX_STATS; i++)
        {
            pLevelInfo->stats[i] = fields[i+4].GetUInt16();
        }

        ++count;
    }
    while (result->NextRow());

    // Fill gaps and check integrity
    for (PetLevelInfoContainer::iterator itr = _petInfoStore.begin(); itr != _petInfoStore.end(); ++itr)
    {
        PetLevelInfo* pInfo = itr->second;

        // fatal error if no level 1 data
        if (!pInfo || pInfo[0].health == 0)
        {
            TC_LOG_ERROR("sql.sql", "Creature %u does not have pet stats data for Level 1!", itr->first);
            exit(1);
        }

        // fill level gaps
        for (uint8 level = 1; level < sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL); ++level)
        {
            if (pInfo[level].health == 0)
            {
                TC_LOG_ERROR("sql.sql", "Creature %u has no data for Level %i pet stats data, using data of Level %i.", itr->first, level+1, level);
                pInfo[level] = pInfo[level-1];
            }
        }
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u level pet stats definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

PetLevelInfo const* ObjectMgr::GetPetLevelInfo(uint32 creature_id, uint8 level) const
{
    if (level > sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
        level = sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL);

    PetLevelInfoContainer::const_iterator itr = _petInfoStore.find(creature_id);
    if (itr == _petInfoStore.end())
        return NULL;

    return &itr->second[level-1];                           // data for level 1 stored in [0] array element, ...
}

void ObjectMgr::PlayerCreateInfoAddItemHelper(uint32 race_, uint32 class_, uint32 itemId, int32 count)
{
    if (count > 0)
        _playerInfo[race_][class_].item.push_back(PlayerCreateInfoItem(itemId, count));
    else
    {
        if (count < -1)
            TC_LOG_ERROR("sql.sql", "Invalid count %i specified on item %u be removed from original player create info (use -1)!", count, itemId);

        uint32 RaceClass = (race_) | (class_ << 8);
        bool doneOne = false;
        for (uint32 i = 1; i < sCharStartOutfitStore.GetNumRows(); ++i)
        {
            if (CharStartOutfitEntry const* entry = sCharStartOutfitStore.LookupEntry(i))
            {
                if (entry->RaceClassGender == RaceClass || entry->RaceClassGender == (RaceClass | (1 << 16)))
                {
                    bool found = false;
                    for (uint8 x = 0; x < MAX_OUTFIT_ITEMS; ++x)
                    {
                        if (entry->ItemId[x] > 0 && uint32(entry->ItemId[x]) == itemId)
                        {
                            found = true;
                            const_cast<CharStartOutfitEntry*>(entry)->ItemId[x] = 0;
                            break;
                        }
                    }

                    if (!found)
                        TC_LOG_ERROR("sql.sql", "Item %u specified to be removed from original create info not found in dbc!", itemId);

                    if (!doneOne)
                        doneOne = true;
                    else
                        break;
                }
            }
        }
    }
}

void ObjectMgr::LoadPlayerInfo()
{
    // Load playercreate
    {
        uint32 oldMSTime = getMSTime();
        //                                                0     1      2    3        4          5           6
        QueryResult result = WorldDatabase.Query("SELECT race, class, map, zone, position_x, position_y, position_z, orientation FROM playercreateinfo");

        if (!result)
        {

            TC_LOG_ERROR("server.loading", ">> Loaded 0 player create definitions. DB table `playercreateinfo` is empty.");
            exit(1);
        }
        else
        {
            uint32 count = 0;

            do
            {
                Field* fields = result->Fetch();

                uint32 current_race  = fields[0].GetUInt8();
                uint32 current_class = fields[1].GetUInt8();
                uint32 mapId         = fields[2].GetUInt16();
                uint32 areaId        = fields[3].GetUInt32(); // zone
                float  positionX     = fields[4].GetFloat();
                float  positionY     = fields[5].GetFloat();
                float  positionZ     = fields[6].GetFloat();
                float  orientation   = fields[7].GetFloat();

                if (current_race >= MAX_RACES)
                {
                    TC_LOG_ERROR("sql.sql", "Wrong race %u in `playercreateinfo` table, ignoring.", current_race);
                    continue;
                }

                ChrRacesEntry const* rEntry = sChrRacesStore.LookupEntry(current_race);
                if (!rEntry)
                {
                    TC_LOG_ERROR("sql.sql", "Wrong race %u in `playercreateinfo` table, ignoring.", current_race);
                    continue;
                }

                if (current_class >= MAX_CLASSES)
                {
                    TC_LOG_ERROR("sql.sql", "Wrong class %u in `playercreateinfo` table, ignoring.", current_class);
                    continue;
                }

                if (!sChrClassesStore.LookupEntry(current_class))
                {
                    TC_LOG_ERROR("sql.sql", "Wrong class %u in `playercreateinfo` table, ignoring.", current_class);
                    continue;
                }

                // accept DB data only for valid position (and non instanceable)
                if (!MapManager::IsValidMapCoord(mapId, positionX, positionY, positionZ, orientation))
                {
                    TC_LOG_ERROR("sql.sql", "Wrong home position for class %u race %u pair in `playercreateinfo` table, ignoring.", current_class, current_race);
                    continue;
                }

                if (sMapStore.LookupEntry(mapId)->Instanceable())
                {
                    TC_LOG_ERROR("sql.sql", "Home position in instanceable map for class %u race %u pair in `playercreateinfo` table, ignoring.", current_class, current_race);
                    continue;
                }

                PlayerInfo* pInfo = &_playerInfo[current_race][current_class];

                pInfo->mapId       = mapId;
                pInfo->areaId      = areaId;
                pInfo->positionX   = positionX;
                pInfo->positionY   = positionY;
                pInfo->positionZ   = positionZ;
                pInfo->orientation = orientation;

                pInfo->displayId_m = rEntry->model_m;
                pInfo->displayId_f = rEntry->model_f;

                ++count;
            }
            while (result->NextRow());

            TC_LOG_INFO("server.loading", ">> Loaded %u player create definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    // Load playercreate items
    TC_LOG_INFO("server.loading", "Loading Player Create Items Data...");
    {
        uint32 oldMSTime = getMSTime();
        //                                                0     1      2       3
        QueryResult result = WorldDatabase.Query("SELECT race, class, itemid, amount FROM playercreateinfo_item");

        if (!result)
        {
            TC_LOG_INFO("server.loading", ">> Loaded 0 custom player create items. DB table `playercreateinfo_item` is empty.");
        }
        else
        {
            uint32 count = 0;

            do
            {
                Field* fields = result->Fetch();

                uint32 current_race = fields[0].GetUInt8();
                if (current_race >= MAX_RACES)
                {
                    TC_LOG_ERROR("sql.sql", "Wrong race %u in `playercreateinfo_item` table, ignoring.", current_race);
                    continue;
                }

                uint32 current_class = fields[1].GetUInt8();
                if (current_class >= MAX_CLASSES)
                {
                    TC_LOG_ERROR("sql.sql", "Wrong class %u in `playercreateinfo_item` table, ignoring.", current_class);
                    continue;
                }

                uint32 item_id = fields[2].GetUInt32();

                if (!GetItemTemplate(item_id))
                {
                    TC_LOG_ERROR("sql.sql", "Item id %u (race %u class %u) in `playercreateinfo_item` table but not listed in `item_template`, ignoring.", item_id, current_race, current_class);
                    continue;
                }

                int32 amount   = fields[3].GetInt8();

                if (!amount)
                {
                    TC_LOG_ERROR("sql.sql", "Item id %u (class %u race %u) have amount == 0 in `playercreateinfo_item` table, ignoring.", item_id, current_race, current_class);
                    continue;
                }

                if (!current_race || !current_class)
                {
                    uint32 min_race = current_race ? current_race : 1;
                    uint32 max_race = current_race ? current_race + 1 : MAX_RACES;
                    uint32 min_class = current_class ? current_class : 1;
                    uint32 max_class = current_class ? current_class + 1 : MAX_CLASSES;
                    for (uint32 r = min_race; r < max_race; ++r)
                        for (uint32 c = min_class; c < max_class; ++c)
                            PlayerCreateInfoAddItemHelper(r, c, item_id, amount);
                }
                else
                    PlayerCreateInfoAddItemHelper(current_race, current_class, item_id, amount);

                ++count;
            }
            while (result->NextRow());

            TC_LOG_INFO("server.loading", ">> Loaded %u custom player create items in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    // Load playercreate spells
    TC_LOG_INFO("server.loading", "Loading Player Create Spell Data...");
    {
        uint32 oldMSTime = getMSTime();

        std::string tableName = sWorld->getBoolConfig(CONFIG_START_ALL_SPELLS) ? "playercreateinfo_spell_custom" : "playercreateinfo_spell";
        QueryResult result = WorldDatabase.PQuery("SELECT race, class, Spell FROM %s", tableName.c_str());

        if (!result)
        {
            TC_LOG_ERROR("server.loading", ">> Loaded 0 player create spells. DB table `%s` is empty.", sWorld->getBoolConfig(CONFIG_START_ALL_SPELLS) ? "playercreateinfo_spell_custom" : "playercreateinfo_spell");

        }
        else
        {
            uint32 count = 0;

            do
            {
                Field* fields = result->Fetch();

                uint32 current_race = fields[0].GetUInt8();
                if (current_race >= MAX_RACES)
                {
                    TC_LOG_ERROR("sql.sql", "Wrong race %u in `playercreateinfo_spell` table, ignoring.", current_race);
                    continue;
                }

                uint32 current_class = fields[1].GetUInt8();
                if (current_class >= MAX_CLASSES)
                {
                    TC_LOG_ERROR("sql.sql", "Wrong class %u in `playercreateinfo_spell` table, ignoring.", current_class);
                    continue;
                }

                if (!current_race || !current_class)
                {
                    uint32 min_race = current_race ? current_race : 1;
                    uint32 max_race = current_race ? current_race + 1 : MAX_RACES;
                    uint32 min_class = current_class ? current_class : 1;
                    uint32 max_class = current_class ? current_class + 1 : MAX_CLASSES;
                    for (uint32 r = min_race; r < max_race; ++r)
                        for (uint32 c = min_class; c < max_class; ++c)
                            _playerInfo[r][c].spell.push_back(fields[2].GetUInt32());
                }
                else
                    _playerInfo[current_race][current_class].spell.push_back(fields[2].GetUInt32());

                ++count;
            }
            while (result->NextRow());

            TC_LOG_INFO("server.loading", ">> Loaded %u player create spells in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    // Load playercreate actions
    TC_LOG_INFO("server.loading", "Loading Player Create Action Data...");
    {
        uint32 oldMSTime = getMSTime();

        //                                                0     1      2       3       4
        QueryResult result = WorldDatabase.Query("SELECT race, class, button, action, type FROM playercreateinfo_action");

        if (!result)
        {
            TC_LOG_ERROR("server.loading", ">> Loaded 0 player create actions. DB table `playercreateinfo_action` is empty.");

        }
        else
        {
            uint32 count = 0;

            do
            {
                Field* fields = result->Fetch();

                uint32 current_race = fields[0].GetUInt8();
                if (current_race >= MAX_RACES)
                {
                    TC_LOG_ERROR("sql.sql", "Wrong race %u in `playercreateinfo_action` table, ignoring.", current_race);
                    continue;
                }

                uint32 current_class = fields[1].GetUInt8();
                if (current_class >= MAX_CLASSES)
                {
                    TC_LOG_ERROR("sql.sql", "Wrong class %u in `playercreateinfo_action` table, ignoring.", current_class);
                    continue;
                }

                PlayerInfo* pInfo = &_playerInfo[current_race][current_class];
                pInfo->action.push_back(PlayerCreateInfoAction(fields[2].GetUInt16(), fields[3].GetUInt32(), fields[4].GetUInt16()));

                ++count;
            }
            while (result->NextRow());

            TC_LOG_INFO("server.loading", ">> Loaded %u player create actions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    // Loading levels data (class/race dependent)
    TC_LOG_INFO("server.loading", "Loading Player Create Level Stats Data...");
    {
        uint32 oldMSTime = getMSTime();

        //                                                 0     1      2      3    4    5    6    7
        QueryResult result  = WorldDatabase.Query("SELECT race, class, level, str, agi, sta, inte, spi FROM player_levelstats");

        if (!result)
        {
            TC_LOG_ERROR("server.loading", ">> Loaded 0 level stats definitions. DB table `player_levelstats` is empty.");

            exit(1);
        }

        uint32 count = 0;

        do
        {
            Field* fields = result->Fetch();

            uint32 current_race = fields[0].GetUInt8();
            if (current_race >= MAX_RACES)
            {
                TC_LOG_ERROR("sql.sql", "Wrong race %u in `player_levelstats` table, ignoring.", current_race);
                continue;
            }

            uint32 current_class = fields[1].GetUInt8();
            if (current_class >= MAX_CLASSES)
            {
                TC_LOG_ERROR("sql.sql", "Wrong class %u in `player_levelstats` table, ignoring.", current_class);
                continue;
            }

            uint32 current_level = fields[2].GetUInt8();
            if (current_level > sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
            {
                if (current_level > STRONG_MAX_LEVEL)        // hardcoded level maximum
                    TC_LOG_ERROR("sql.sql", "Wrong (> %u) level %u in `player_levelstats` table, ignoring.", STRONG_MAX_LEVEL, current_level);
                else
                {
                    TC_LOG_INFO("misc", "Unused (> MaxPlayerLevel in worldserver.conf) level %u in `player_levelstats` table, ignoring.", current_level);
                    ++count;                                // make result loading percent "expected" correct in case disabled detail mode for example.
                }
                continue;
            }

            PlayerInfo* pInfo = &_playerInfo[current_race][current_class];

            if (!pInfo->levelInfo)
                pInfo->levelInfo = new PlayerLevelInfo[sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL)];

            PlayerLevelInfo* pLevelInfo = &pInfo->levelInfo[current_level-1];

            for (int i = 0; i < MAX_STATS; i++)
            {
                pLevelInfo->stats[i] = fields[i+3].GetUInt8();
            }

            ++count;
        }
        while (result->NextRow());

        // Fill gaps and check integrity
        for (int race = 0; race < MAX_RACES; ++race)
        {
            // skip non existed races
            if (!sChrRacesStore.LookupEntry(race))
                continue;

            for (int class_ = 0; class_ < MAX_CLASSES; ++class_)
            {
                // skip non existed classes
                if (!sChrClassesStore.LookupEntry(class_))
                    continue;

                PlayerInfo* pInfo = &_playerInfo[race][class_];

                // skip non loaded combinations
                if (!pInfo->displayId_m || !pInfo->displayId_f)
                    continue;

                // skip expansion races if not playing with expansion
                if (sWorld->getIntConfig(CONFIG_EXPANSION) < 1 && (race == RACE_BLOODELF || race == RACE_DRAENEI))
                    continue;

                // skip expansion classes if not playing with expansion
                if (sWorld->getIntConfig(CONFIG_EXPANSION) < 2 && class_ == CLASS_DEATH_KNIGHT)
                    continue;

                // skip expansion classes / races if not playing with expansion
                if (sWorld->getIntConfig(CONFIG_EXPANSION) < 3 && (race == RACE_GOBLIN || race == RACE_WORGEN))

                // skip expansion classes / races if not playing with expansion
                if (sWorld->getIntConfig(CONFIG_EXPANSION) < 4 && (class_ == CLASS_MONK || race == RACE_PANDAREN_NEUTRAL || race == RACE_PANDAREN_ALLI || race == RACE_PANDAREN_HORDE))

                // fatal error if no level 1 data
                if (!pInfo->levelInfo || pInfo->levelInfo[0].stats[0] == 0)
                {
                    TC_LOG_ERROR("sql.sql", "Race %i Class %i Level 1 does not have stats data!", race, class_);
                    exit(1);
                }

                // fill level gaps
                for (uint8 level = 1; level < sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL); ++level)
                {
                    if (pInfo->levelInfo[level].stats[0] == 0)
                    {
                        TC_LOG_ERROR("sql.sql", "Race %i Class %i Level %i does not have stats data. Using stats data of level %i.", race, class_, level+1, level);
                        pInfo->levelInfo[level] = pInfo->levelInfo[level-1];
                    }
                }
            }
        }

        TC_LOG_INFO("server.loading", ">> Loaded %u level stats definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    }

    // Loading xp per level data
    TC_LOG_INFO("server.loading", "Loading Player Create XP Data...");
    {
        uint32 oldMSTime = getMSTime();

        _playerXPperLevel.resize(sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL));
        for (uint8 level = 0; level < sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL); ++level)
            _playerXPperLevel[level] = 0;

        //                                                 0    1
        QueryResult result  = WorldDatabase.Query("SELECT lvl, xp_for_next_level FROM player_xp_for_level");

        if (!result)
        {
            TC_LOG_ERROR("server.loading", ">> Loaded 0 xp for level definitions. DB table `player_xp_for_level` is empty.");

            exit(1);
        }

        uint32 count = 0;

        do
        {
            Field* fields = result->Fetch();

            uint32 current_level = fields[0].GetUInt8();
            uint32 current_xp    = fields[1].GetUInt32();

            if (current_level >= sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
            {
                if (current_level > STRONG_MAX_LEVEL)        // hardcoded level maximum
                    TC_LOG_ERROR("sql.sql", "Wrong (> %u) level %u in `player_xp_for_level` table, ignoring.", STRONG_MAX_LEVEL, current_level);
                else
                {
                    TC_LOG_INFO("misc", "Unused (> MaxPlayerLevel in worldserver.conf) level %u in `player_xp_for_levels` table, ignoring.", current_level);
                    ++count;                                // make result loading percent "expected" correct in case disabled detail mode for example.
                }
                continue;
            }
            //PlayerXPperLevel
            _playerXPperLevel[current_level] = current_xp;
            ++count;
        }
        while (result->NextRow());

        // fill level gaps
        for (uint8 level = 1; level < sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL); ++level)
        {
            if (_playerXPperLevel[level] == 0)
            {
                TC_LOG_ERROR("sql.sql", "Level %i does not have XP for level data. Using data of level [%i] + 100.", level+1, level);
                _playerXPperLevel[level] = _playerXPperLevel[level-1]+100;
            }
        }

        TC_LOG_INFO("server.loading", ">> Loaded %u xp for level definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    }
}

void ObjectMgr::GetPlayerClassLevelInfo(uint32 class_, uint8 level, uint32& baseHP, uint32& baseMana) const
{
    if (level < 1 || class_ >= MAX_CLASSES)
        return;

    if (level > sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
        level = sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL);

    GtOCTBaseHPByClassEntry const* hp = sGtOCTBaseHPByClassStore.LookupEntry((class_-1) * GT_MAX_LEVEL + level-1);
    GtOCTBaseMPByClassEntry const* mp = sGtOCTBaseMPByClassStore.LookupEntry((class_-1) * GT_MAX_LEVEL + level-1);

    if (!hp || !mp)
    {
        TC_LOG_ERROR("misc", "Tried to get non-existant Class-Level combination data for base hp/mp. Class %u Level %u", class_, level);
        return;
    }

    baseHP = uint32(hp->ratio);
    baseMana = uint32(mp->ratio);
}

void ObjectMgr::GetPlayerLevelInfo(uint32 race, uint32 class_, uint8 level, PlayerLevelInfo* info) const
{
    if (level < 1 || race >= MAX_RACES || class_ >= MAX_CLASSES)
        return;

    PlayerInfo const* pInfo = &_playerInfo[race][class_];
    if (pInfo->displayId_m == 0 || pInfo->displayId_f == 0)
        return;

    if (level <= sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
        *info = pInfo->levelInfo[level-1];
    else
        BuildPlayerLevelInfo(race, class_, level, info);
}

void ObjectMgr::BuildPlayerLevelInfo(uint8 race, uint8 _class, uint8 level, PlayerLevelInfo* info) const
{
    // base data (last known level)
    *info = _playerInfo[race][_class].levelInfo[sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL)-1];

    // if conversion from uint32 to uint8 causes unexpected behaviour, change lvl to uint32
    for (uint8 lvl = sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL)-1; lvl < level; ++lvl)
    {
        switch (_class)
        {
            case CLASS_WARRIOR:
                info->stats[STAT_STRENGTH]  += (lvl > 23 ? 2: (lvl > 1  ? 1: 0));
                info->stats[STAT_STAMINA]   += (lvl > 23 ? 2: (lvl > 1  ? 1: 0));
                info->stats[STAT_AGILITY]   += (lvl > 36 ? 1: (lvl > 6 && (lvl%2) ? 1: 0));
                info->stats[STAT_INTELLECT] += (lvl > 9 && !(lvl%2) ? 1: 0);
                info->stats[STAT_SPIRIT]    += (lvl > 9 && !(lvl%2) ? 1: 0);
                break;
            case CLASS_PALADIN:
                info->stats[STAT_STRENGTH]  += (lvl > 3  ? 1: 0);
                info->stats[STAT_STAMINA]   += (lvl > 33 ? 2: (lvl > 1 ? 1: 0));
                info->stats[STAT_AGILITY]   += (lvl > 38 ? 1: (lvl > 7 && !(lvl%2) ? 1: 0));
                info->stats[STAT_INTELLECT] += (lvl > 6 && (lvl%2) ? 1: 0);
                info->stats[STAT_SPIRIT]    += (lvl > 7 ? 1: 0);
                break;
            case CLASS_HUNTER:
                info->stats[STAT_STRENGTH]  += (lvl > 4  ? 1: 0);
                info->stats[STAT_STAMINA]   += (lvl > 4  ? 1: 0);
                info->stats[STAT_AGILITY]   += (lvl > 33 ? 2: (lvl > 1 ? 1: 0));
                info->stats[STAT_INTELLECT] += (lvl > 8 && (lvl%2) ? 1: 0);
                info->stats[STAT_SPIRIT]    += (lvl > 38 ? 1: (lvl > 9 && !(lvl%2) ? 1: 0));
                break;
            case CLASS_ROGUE:
                info->stats[STAT_STRENGTH]  += (lvl > 5  ? 1: 0);
                info->stats[STAT_STAMINA]   += (lvl > 4  ? 1: 0);
                info->stats[STAT_AGILITY]   += (lvl > 16 ? 2: (lvl > 1 ? 1: 0));
                info->stats[STAT_INTELLECT] += (lvl > 8 && !(lvl%2) ? 1: 0);
                info->stats[STAT_SPIRIT]    += (lvl > 38 ? 1: (lvl > 9 && !(lvl%2) ? 1: 0));
                break;
            case CLASS_PRIEST:
                info->stats[STAT_STRENGTH]  += (lvl > 9 && !(lvl%2) ? 1: 0);
                info->stats[STAT_STAMINA]   += (lvl > 5  ? 1: 0);
                info->stats[STAT_AGILITY]   += (lvl > 38 ? 1: (lvl > 8 && (lvl%2) ? 1: 0));
                info->stats[STAT_INTELLECT] += (lvl > 22 ? 2: (lvl > 1 ? 1: 0));
                info->stats[STAT_SPIRIT]    += (lvl > 3  ? 1: 0);
                break;
            case CLASS_SHAMAN:
                info->stats[STAT_STRENGTH]  += (lvl > 34 ? 1: (lvl > 6 && (lvl%2) ? 1: 0));
                info->stats[STAT_STAMINA]   += (lvl > 4 ? 1: 0);
                info->stats[STAT_AGILITY]   += (lvl > 7 && !(lvl%2) ? 1: 0);
                info->stats[STAT_INTELLECT] += (lvl > 5 ? 1: 0);
                info->stats[STAT_SPIRIT]    += (lvl > 4 ? 1: 0);
                break;
            case CLASS_MAGE:
                info->stats[STAT_STRENGTH]  += (lvl > 9 && !(lvl%2) ? 1: 0);
                info->stats[STAT_STAMINA]   += (lvl > 5  ? 1: 0);
                info->stats[STAT_AGILITY]   += (lvl > 9 && !(lvl%2) ? 1: 0);
                info->stats[STAT_INTELLECT] += (lvl > 24 ? 2: (lvl > 1 ? 1: 0));
                info->stats[STAT_SPIRIT]    += (lvl > 33 ? 2: (lvl > 2 ? 1: 0));
                break;
            case CLASS_WARLOCK:
                info->stats[STAT_STRENGTH]  += (lvl > 9 && !(lvl%2) ? 1: 0);
                info->stats[STAT_STAMINA]   += (lvl > 38 ? 2: (lvl > 3 ? 1: 0));
                info->stats[STAT_AGILITY]   += (lvl > 9 && !(lvl%2) ? 1: 0);
                info->stats[STAT_INTELLECT] += (lvl > 33 ? 2: (lvl > 2 ? 1: 0));
                info->stats[STAT_SPIRIT]    += (lvl > 38 ? 2: (lvl > 3 ? 1: 0));
                break;
            case CLASS_DRUID:
                info->stats[STAT_STRENGTH]  += (lvl > 38 ? 2: (lvl > 6 && (lvl%2) ? 1: 0));
                info->stats[STAT_STAMINA]   += (lvl > 32 ? 2: (lvl > 4 ? 1: 0));
                info->stats[STAT_AGILITY]   += (lvl > 38 ? 2: (lvl > 8 && (lvl%2) ? 1: 0));
                info->stats[STAT_INTELLECT] += (lvl > 38 ? 3: (lvl > 4 ? 1: 0));
                info->stats[STAT_SPIRIT]    += (lvl > 38 ? 3: (lvl > 5 ? 1: 0));
            case CLASS_MONK:
                info->stats[STAT_STRENGTH]  += (lvl > 38 ? 2 : (lvl > 6 && (lvl % 2) ? 1 : 0));
                info->stats[STAT_STAMINA]   += (lvl > 32 ? 2 : (lvl > 4 ? 1 : 0));
                info->stats[STAT_AGILITY]   += (lvl > 38 ? 2 : (lvl > 8 && (lvl % 2) ? 1 : 0));
                info->stats[STAT_INTELLECT] += (lvl > 38 ? 3 : (lvl > 4 ? 1 : 0));
                info->stats[STAT_SPIRIT]    += (lvl > 38 ? 3 : (lvl > 5 ? 1 : 0));
                break;
        }
    }
}

/* Need to add new fields
        RequiredFactionKills        after RequiredFactionValue2
        RequiredFactionKillsCount   after RequiredFactionKills
        RequiredPetBattleWith       after RequiredFactionKillsCount
        RewardXPId                  after RewardXPId
        move EndText                after RequestItemsText
        Add RequiredNpcOrGo 5 to 10 after RequiredNpcOrGo4
        RequiredNpcOrGoCount 5 to 10 after RequiredNpcOrGoCount4
        RequiredItemId 7 to 10      after RequiredItemId6
        RequiredItemCount 7 to 10   after RequiredItemCount6
        RequiredSpellCast 5 to 6    after RequiredSpellCast4
        ObjectiveText 5 to 10       after ObjectiveText4
        remove QuestGiverTargetName
*/
void ObjectMgr::LoadQuests()
{
    uint32 oldMSTime = getMSTime();

    // For reload case
    for (auto &questTemplate : _questTemplates)
        delete questTemplate.second;

    _questTemplates.clear();

    mExclusiveQuestGroups.clear();

    QueryResult result = WorldDatabase.Query("SELECT "
        //0     1      2        3        4           5       6            7             8           9              10              11             12                 13
        "Id, Method, Level, MinLevel, MaxLevel, ZoneOrSort, Type, SuggestedPlayers, LimitTime, RequiredTeam, RequiredClasses, RequiredRaces, RequiredSkillId, RequiredSkillPoints, "
        //         14                    15                    16                   17               18            19            20               21             22           23               24
        "RequiredMinRepFaction, RequiredMaxRepFaction, RequiredMinRepValue, RequiredMaxRepValue, PrevQuestId, NextQuestId, ExclusiveGroup, NextQuestIdChain, RewardXPId, RewardMoney, RewardMoneyMaxLevel, "
        //    25             26             27                28                    29                  20             30             31               32        33         34               35              26             27             28
        "RewardSpell, RewardSpellCast, RewardHonor, RewardHonorMultiplier, RewardMailTemplateId, RewardMailDelay, SourceItemId, SourceItemCount, SourceSpellId, Flags, SpecialFlags, MinimapTargetMark, RewardTitleId, RewardTalents, RewardPackage, "
        //      29            30                    31                    32                  33               34             35              36             37                38                39                40               41
        "RewardSkillId, RewardSkillPoints, RewardReputationMask, QuestGiverPortrait, QuestTurnInPortrait, RewardItemId1, RewardItemId2, RewardItemId3, RewardItemId4, RewardItemCount1, RewardItemCount2, RewardItemCount3, RewardItemCount4, "
        //         42                  43                  44                    45                    46                   47                      48                  49                        50                       51                      52                      53
        "RewardChoiceItemId1, RewardChoiceItemId2, RewardChoiceItemId3, RewardChoiceItemId4, RewardChoiceItemId5, RewardChoiceItemId6, RewardChoiceItemCount1, RewardChoiceItemCount2, RewardChoiceItemCount3, RewardChoiceItemCount4, RewardChoiceItemCount5, RewardChoiceItemCount6, "
        //        54                55                56                57             58                   59                      60                     61                    62                      63
        "RewardFactionId1, RewardFactionId2, RewardFactionId3, RewardFactionId4, RewardFactionId5, RewardFactionValueId1, RewardFactionValueId2, RewardFactionValueId3, RewardFactionValueId4, RewardFactionValueId5, "
        //                64                          65                           66                              67                           68
        "RewardFactionValueIdOverride1, RewardFactionValueIdOverride2, RewardFactionValueIdOverride3, RewardFactionValueIdOverride4, RewardFactionValueIdOverride5, "
        //    69        70      71      72          73       74        75       76           77              78              79
        "PointMapId, PointX, PointY, PointOption, Title, Objectives, Details, EndText, CompletedText, OfferRewardText, RequestItemsText, "
        //         80                     81                    82                     83                       84                        85                        86                        87
        "RequiredSourceItemId1, RequiredSourceItemId2, RequiredSourceItemId3, RequiredSourceItemId4, RequiredSourceItemCount1, RequiredSourceItemCount2, RequiredSourceItemCount3, RequiredSourceItemCount4, "
        //      88                   88                89                90                   91                   92                      93                    94
        "RewardCurrencyId1, RewardCurrencyId2, RewardCurrencyId3, RewardCurrencyId4, RewardCurrencyCount1, RewardCurrencyCount2, RewardCurrencyCount3, RewardCurrencyCount4, "
        //       95                   96                     97                   98                99          100
        "QuestGiverTextWindow, QuestGiverTargetName, QuestTurnTextWindow, QuestTurnTargetName, SoundAccept, SoundTurnIn, "
        //      101          102           103            104               105                106                  107                  108                109             110
        "DetailsEmote1, DetailsEmote2, DetailsEmote3, DetailsEmote4, DetailsEmoteDelay1, DetailsEmoteDelay2, DetailsEmoteDelay3, DetailsEmoteDelay4, EmoteOnIncomplete, EmoteOnComplete, "
        //      111                 112               113                114                   115                      116                     117                  118
        "OfferRewardEmote1, OfferRewardEmote2, OfferRewardEmote3, OfferRewardEmote4, OfferRewardEmoteDelay1, OfferRewardEmoteDelay2, OfferRewardEmoteDelay3, OfferRewardEmoteDelay4, "
        //    119           120           121
        "StartScript, CompleteScript, WDBVerified"
        " FROM quest_template");
    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 quests definitions. DB table `quest_template` is empty.");

        return;
    }

    // Create multimap previous quest for each existed quest
    // some quests can have many previous maps set by NextQuestId in previous quest
    // for example set of race quests can lead to single not race specific quest
    do
    {
        Field* fields = result->Fetch();

        Quest* newQuest = new Quest(fields);
        _questTemplates[newQuest->GetQuestId()] = newQuest;
    }
    while (result->NextRow());

    std::map<uint32, uint32> usedMailTemplates;

    // Post processing
    for (QuestMap::iterator iter = _questTemplates.begin(); iter != _questTemplates.end(); ++iter)
    {
        // Skip post-loading checks for disabled quests
        if (DisableMgr::IsDisabledFor(DISABLE_TYPE_QUEST, iter->first, NULL))
            continue;

        Quest * const qinfo = iter->second;

        // Additional quest integrity checks (GO, creature_template and item_template must be loaded already)

        if (auto const packageId = qinfo->GetRewardPackage())
        {
            auto const range = GetQuestPackageItems(packageId);
            if (range.first == range.second)
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `RewardPackage` = %u that does not exist.", qinfo->GetQuestId(), packageId);
                qinfo->RewardPackage = 0;
            }
        }

        if (qinfo->GetQuestMethod() >= 3)
            TC_LOG_ERROR("sql.sql", "Quest %u has `Method` = %u, expected values are 0, 1 or 2.", qinfo->GetQuestId(), qinfo->GetQuestMethod());

        if (qinfo->SpecialFlags & ~QUEST_SPECIAL_FLAGS_DB_ALLOWED)
        {
            TC_LOG_ERROR("sql.sql", "Quest %u has `SpecialFlags` = %u > max allowed value. Correct `SpecialFlags` to value <= %u",
                qinfo->GetQuestId(), qinfo->SpecialFlags, QUEST_SPECIAL_FLAGS_DB_ALLOWED);
            qinfo->SpecialFlags &= QUEST_SPECIAL_FLAGS_DB_ALLOWED;
        }

        if (qinfo->Flags & QUEST_FLAGS_DAILY && qinfo->Flags & QUEST_FLAGS_WEEKLY)
        {
            TC_LOG_ERROR("sql.sql", "Weekly Quest %u is marked as daily quest in `Flags`, removed daily flag.", qinfo->GetQuestId());
            qinfo->Flags &= ~QUEST_FLAGS_DAILY;
        }

        if (qinfo->Flags & QUEST_FLAGS_DAILY)
        {
            if (!(qinfo->SpecialFlags & QUEST_SPECIAL_FLAGS_REPEATABLE))
            {
                TC_LOG_ERROR("sql.sql", "Daily Quest %u not marked as repeatable in `SpecialFlags`, added.", qinfo->GetQuestId());
                qinfo->SpecialFlags |= QUEST_SPECIAL_FLAGS_REPEATABLE;
            }
        }

        if (qinfo->Flags & QUEST_FLAGS_WEEKLY)
        {
            if (!(qinfo->SpecialFlags & QUEST_SPECIAL_FLAGS_REPEATABLE))
            {
                TC_LOG_ERROR("sql.sql", "Weekly Quest %u not marked as repeatable in `SpecialFlags`, added.", qinfo->GetQuestId());
                qinfo->SpecialFlags |= QUEST_SPECIAL_FLAGS_REPEATABLE;
            }
        }

        if (qinfo->SpecialFlags & QUEST_SPECIAL_FLAGS_MONTHLY)
        {
            if (!(qinfo->SpecialFlags & QUEST_SPECIAL_FLAGS_REPEATABLE))
            {
                TC_LOG_ERROR("sql.sql", "Monthly quest %u not marked as repeatable in `SpecialFlags`, added.", qinfo->GetQuestId());
                qinfo->SpecialFlags |= QUEST_SPECIAL_FLAGS_REPEATABLE;
            }
        }

        if (qinfo->Flags & QUEST_FLAGS_TRACKING)
        {
            // At auto-reward can be rewarded only RewardChoiceItemId[0]
            for (int j = 1; j < QUEST_REWARD_CHOICES_COUNT; ++j)
            {
                if (uint32 id = qinfo->RewardChoiceItemId[j])
                {
                    TC_LOG_ERROR("sql.sql", "Quest %u has `RewardChoiceItemId%d` = %u but item from `RewardChoiceItemId%d` can't be rewarded with quest flag QUEST_FLAGS_TRACKING.",
                        qinfo->GetQuestId(), j + 1, id, j + 1);
                    // No changes, quest ignore this data
                }
            }
        }

        if (qinfo->MinLevel == uint32(-1) || qinfo->MinLevel > DEFAULT_MAX_LEVEL)
        {
            TC_LOG_ERROR("sql.sql", "Quest %u should be disabled because `MinLevel` = %i", qinfo->GetQuestId(), int32(qinfo->MinLevel));
            // No changes needed, sending -1 in SMSG_QUEST_QUERY_RESPONSE is valid
        }

        // Client quest log visual (area case)
        if (qinfo->ZoneOrSort > 0)
        {
            if (!GetAreaEntryByAreaID(qinfo->ZoneOrSort))
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `ZoneOrSort` = %u (zone case) but zone with this id does not exist.",
                    qinfo->GetQuestId(), qinfo->ZoneOrSort);
                // No changes, quest not dependent from this value but can have problems at client
            }
        }
        // Client quest log visual (sort case)
        if (qinfo->ZoneOrSort < 0)
        {
            QuestSortEntry const* qSort = sQuestSortStore.LookupEntry(-int32(qinfo->ZoneOrSort));
            if (!qSort)
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `ZoneOrSort` = %i (sort case) but quest sort with this id does not exist.",
                    qinfo->GetQuestId(), qinfo->ZoneOrSort);
                // No changes, quest not dependent from this value but can have problems at client (note some may be 0, we must allow this so no check)
            }
            // Check for proper RequiredSkillId value (skill case)
            if (uint32 skill_id = SkillByQuestSort(-int32(qinfo->ZoneOrSort)))
            {
                if (qinfo->RequiredSkillId != skill_id)
                {
                    TC_LOG_ERROR("sql.sql", "Quest %u has `ZoneOrSort` = %i but `RequiredSkillId` does not have a corresponding value (%d).",
                        qinfo->GetQuestId(), qinfo->ZoneOrSort, skill_id);
                    // Override, and force proper value here?
                }
            }
        }

        // RequiredClasses, can be 0/CLASSMASK_ALL_PLAYABLE to allow any class
        if (qinfo->RequiredClasses)
        {
            if (!(qinfo->RequiredClasses & CLASSMASK_ALL_PLAYABLE))
            {
                TC_LOG_ERROR("sql.sql", "Quest %u does not contain any playable classes in `RequiredClasses` (%u), value set to 0 (all classes).", qinfo->GetQuestId(), qinfo->RequiredClasses);
                qinfo->RequiredClasses = 0;
            }
        }
        // RequiredRaces, can be 0/RACEMASK_ALL_PLAYABLE to allow any race
        if (qinfo->RequiredRaces)
        {
            if (!(qinfo->RequiredRaces & RACEMASK_ALL_PLAYABLE))
            {
                TC_LOG_ERROR("sql.sql", "Quest %u does not contain any playable races in `RequiredRaces` (%u), value set to 0 (all races).", qinfo->GetQuestId(), qinfo->RequiredRaces);
                qinfo->RequiredRaces = 0;
            }
        }
        // RequiredSkillId, can be 0
        if (qinfo->RequiredSkillId)
        {
            if (!sSkillLineStore.LookupEntry(qinfo->RequiredSkillId))
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `RequiredSkillId` = %u but this skill does not exist",
                    qinfo->GetQuestId(), qinfo->RequiredSkillId);
            }
        }

        if (qinfo->RequiredSkillPoints)
        {
            if (qinfo->RequiredSkillPoints > sWorld->GetConfigMaxSkillValue())
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `RequiredSkillPoints` = %u but max possible skill is %u, quest can't be done.",
                    qinfo->GetQuestId(), qinfo->RequiredSkillPoints, sWorld->GetConfigMaxSkillValue());
                // No changes, quest can't be done for this requirement
            }
        }
        // Else Skill quests can have 0 skill level, this is ok

        if (qinfo->RequiredMinRepFaction && !sFactionStore.LookupEntry(qinfo->RequiredMinRepFaction))
        {
            TC_LOG_ERROR("sql.sql", "Quest %u has `RequiredMinRepFaction` = %u but faction template %u does not exist, quest can't be done.",
                qinfo->GetQuestId(), qinfo->RequiredMinRepFaction, qinfo->RequiredMinRepFaction);
            // No changes, quest can't be done for this requirement
        }

        if (qinfo->RequiredMaxRepFaction && !sFactionStore.LookupEntry(qinfo->RequiredMaxRepFaction))
        {
            TC_LOG_ERROR("sql.sql", "Quest %u has `RequiredMaxRepFaction` = %u but faction template %u does not exist, quest can't be done.",
                qinfo->GetQuestId(), qinfo->RequiredMaxRepFaction, qinfo->RequiredMaxRepFaction);
            // No changes, quest can't be done for this requirement
        }

        if (qinfo->RequiredMinRepValue && qinfo->RequiredMinRepValue > ReputationMgr::Reputation_Cap)
        {
            TC_LOG_ERROR("sql.sql", "Quest %u has `RequiredMinRepValue` = %d but max reputation is %u, quest can't be done.",
                qinfo->GetQuestId(), qinfo->RequiredMinRepValue, ReputationMgr::Reputation_Cap);
            // No changes, quest can't be done for this requirement
        }

        if (qinfo->RequiredMinRepValue && qinfo->RequiredMaxRepValue && qinfo->RequiredMaxRepValue <= qinfo->RequiredMinRepValue)
        {
            TC_LOG_ERROR("sql.sql", "Quest %u has `RequiredMaxRepValue` = %d and `RequiredMinRepValue` = %d, quest can't be done.",
                qinfo->GetQuestId(), qinfo->RequiredMaxRepValue, qinfo->RequiredMinRepValue);
            // No changes, quest can't be done for this requirement
        }

        if (!qinfo->RequiredMinRepFaction && qinfo->RequiredMinRepValue != 0)
        {
            TC_LOG_ERROR("sql.sql", "Quest %u has `RequiredMinRepValue` = %d but `RequiredMinRepFaction` is 0, value has no effect",
                qinfo->GetQuestId(), qinfo->RequiredMinRepValue);
            // Warning
        }

        if (!qinfo->RequiredMaxRepFaction && qinfo->RequiredMaxRepValue != 0)
        {
            TC_LOG_ERROR("sql.sql", "Quest %u has `RequiredMaxRepValue` = %d but `RequiredMaxRepFaction` is 0, value has no effect",
                qinfo->GetQuestId(), qinfo->RequiredMaxRepValue);
            // Warning
        }

        if (qinfo->RewardTitleId && !sCharTitlesStore.LookupEntry(qinfo->RewardTitleId))
        {
            TC_LOG_ERROR("sql.sql", "Quest %u has `RewardTitleId` = %u but CharTitle Id %u does not exist, quest can't be rewarded with title.",
                qinfo->GetQuestId(), qinfo->GetCharTitleId(), qinfo->GetCharTitleId());
            qinfo->RewardTitleId = 0;
            // Quest can't reward this title
        }

        if (qinfo->SourceItemId)
        {
            if (!sObjectMgr->GetItemTemplate(qinfo->SourceItemId))
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `SourceItemId` = %u but item with entry %u does not exist, quest can't be done.",
                    qinfo->GetQuestId(), qinfo->SourceItemId, qinfo->SourceItemId);
                qinfo->SourceItemId = 0;                       // Qquest can't be done for this requirement
            }
            else if (qinfo->SourceItemIdCount == 0)
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `SourceItemId` = %u but `SourceItemIdCount` = 0, set to 1 but need fix in DB.",
                    qinfo->GetQuestId(), qinfo->SourceItemId);
                qinfo->SourceItemIdCount = 1;                  // Update to 1 for allow quest work for backward compatibility with DB
            }
        }
        else if (qinfo->SourceItemIdCount > 0)
        {
            TC_LOG_ERROR("sql.sql", "Quest %u has `SourceItemId` = 0 but `SourceItemIdCount` = %u, useless value.",
                qinfo->GetQuestId(), qinfo->SourceItemIdCount);
            qinfo->SourceItemIdCount = 0;                      // No quest work changes in fact
        }

        if (qinfo->SourceSpellid)
        {
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(qinfo->SourceSpellid);
            if (!spellInfo)
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `SourceSpellid` = %u but spell %u doesn't exist, quest can't be done.",
                    qinfo->GetQuestId(), qinfo->SourceSpellid, qinfo->SourceSpellid);
                qinfo->SourceSpellid = 0;                      // Quest can't be done for this requirement
            }
            else if (!sSpellMgr->IsSpellValid(spellInfo))
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `SourceSpellid` = %u but spell %u is broken, quest can't be done.",
                    qinfo->GetQuestId(), qinfo->SourceSpellid, qinfo->SourceSpellid);
                qinfo->SourceSpellid = 0;                      // Quest can't be done for this requirement
            }
        }

        for (uint8 j = 0; j < QUEST_SOURCE_ITEM_IDS_COUNT; ++j)
        {
            uint32 id = qinfo->RequiredSourceItemId[j];
            if (id)
            {
                if (!sObjectMgr->GetItemTemplate(id))
                {
                    TC_LOG_ERROR("sql.sql", "Quest %u has `RequiredSourceItemId%d` = %u but item with entry %u does not exist, quest can't be done.",
                        qinfo->GetQuestId(), j+1, id, id);
                    // No changes, quest can't be done for this requirement
                }
            }
            else
            {
                if (qinfo->RequiredSourceItemCount[j]>0)
                {
                    TC_LOG_ERROR("sql.sql", "Quest %u has `RequiredSourceItemId%d` = 0 but `RequiredSourceItemCount%d` = %u.",
                        qinfo->GetQuestId(), j+1, j+1, qinfo->RequiredSourceItemCount[j]);
                    // No changes, quest ignore this data
                }
            }
        }

        for (uint8 j = 0; j < QUEST_REWARD_CHOICES_COUNT; ++j)
        {
            uint32 id = qinfo->RewardChoiceItemId[j];
            if (id)
            {
                if (!sObjectMgr->GetItemTemplate(id))
                {
                    TC_LOG_ERROR("sql.sql", "Quest %u has `RewardChoiceItemId%d` = %u but item with entry %u does not exist, quest will not reward this item.",
                        qinfo->GetQuestId(), j+1, id, id);
                    qinfo->RewardChoiceItemId[j] = 0;          // No changes, quest will not reward this
                }

                if (!qinfo->RewardChoiceItemCount[j])
                {
                    TC_LOG_ERROR("sql.sql", "Quest %u has `RewardChoiceItemId%d` = %u but `RewardChoiceItemCount%d` = 0, quest can't be done.",
                        qinfo->GetQuestId(), j+1, id, j+1);
                    // No changes, quest can't be done
                }
            }
            else if (qinfo->RewardChoiceItemCount[j]>0)
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `RewardChoiceItemId%d` = 0 but `RewardChoiceItemCount%d` = %u.",
                    qinfo->GetQuestId(), j+1, j+1, qinfo->RewardChoiceItemCount[j]);
                // No changes, quest ignore this data
            }
        }

        for (uint8 j = 0; j < QUEST_REWARDS_COUNT; ++j)
        {
            uint32 id = qinfo->RewardItemId[j];
            if (id)
            {
                if (!sObjectMgr->GetItemTemplate(id))
                {
                    TC_LOG_ERROR("sql.sql", "Quest %u has `RewardItemId%d` = %u but item with entry %u does not exist, quest will not reward this item.",
                        qinfo->GetQuestId(), j+1, id, id);
                    qinfo->RewardItemId[j] = 0;                // No changes, quest will not reward this item
                }

                if (!qinfo->RewardItemIdCount[j])
                {
                    TC_LOG_ERROR("sql.sql", "Quest %u has `RewardItemId%d` = %u but `RewardItemIdCount%d` = 0, quest will not reward this item.",
                        qinfo->GetQuestId(), j+1, id, j+1);
                    // No changes
                }
            }
            else if (qinfo->RewardItemIdCount[j]>0)
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `RewardItemId%d` = 0 but `RewardItemIdCount%d` = %u.",
                    qinfo->GetQuestId(), j+1, j+1, qinfo->RewardItemIdCount[j]);
                // No changes, quest ignore this data
            }
        }

        for (uint8 j = 0; j < QUEST_REPUTATIONS_COUNT; ++j)
        {
            if (qinfo->RewardFactionId[j])
            {
                if (abs(qinfo->RewardFactionValueId[j]) > 9)
                {
               TC_LOG_ERROR("sql.sql", "Quest %u has RewardFactionValueId%d = %i. That is outside the range of valid values (-9 to 9).", qinfo->GetQuestId(), j+1, qinfo->RewardFactionValueId[j]);
                }
                if (!sFactionStore.LookupEntry(qinfo->RewardFactionId[j]))
                {
                    TC_LOG_ERROR("sql.sql", "Quest %u has `RewardFactionId%d` = %u but raw faction (faction.dbc) %u does not exist, quest will not reward reputation for this faction.", qinfo->GetQuestId(), j+1, qinfo->RewardFactionId[j], qinfo->RewardFactionId[j]);
                    qinfo->RewardFactionId[j] = 0;             // Quest will not reward this
                }
            }

            else if (qinfo->RewardFactionValueIdOverride[j] != 0)
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `RewardFactionId%d` = 0 but `RewardFactionValueIdOverride%d` = %i.",
                    qinfo->GetQuestId(), j+1, j+1, qinfo->RewardFactionValueIdOverride[j]);
                // No changes, quest ignore this data
            }
        }

        if (qinfo->RewardSpell)
        {
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(qinfo->RewardSpell);

            if (!spellInfo)
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `RewardSpell` = %u but spell %u does not exist, spell removed as display reward.",
                    qinfo->GetQuestId(), qinfo->RewardSpell, qinfo->RewardSpell);
                qinfo->RewardSpell = 0;                        // No spell reward will display for this quest
            }
            else if (!sSpellMgr->IsSpellValid(spellInfo))
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `RewardSpell` = %u but spell %u is broken, quest will not have a spell reward.",
                    qinfo->GetQuestId(), qinfo->RewardSpell, qinfo->RewardSpell);
                qinfo->RewardSpell = 0;                        // No spell reward will display for this quest
            }
            else if (sSpellMgr->IsTalent(spellInfo->Id))
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `RewardSpell` = %u but spell %u is talent, quest will not have a spell reward.",
                    qinfo->GetQuestId(), qinfo->RewardSpell, qinfo->RewardSpell);
                qinfo->RewardSpell = 0;                        // No spell reward will display for this quest
            }
        }

        if (qinfo->RewardSpellCast > 0)
        {
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(qinfo->RewardSpellCast);

            if (!spellInfo)
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `RewardSpellCast` = %u but spell %u does not exist, quest will not have a spell reward.",
                    qinfo->GetQuestId(), qinfo->RewardSpellCast, qinfo->RewardSpellCast);
                qinfo->RewardSpellCast = 0;                    // No spell will be casted on player
            }
            else if (!sSpellMgr->IsSpellValid(spellInfo))
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `RewardSpellCast` = %u but spell %u is broken, quest will not have a spell reward.",
                    qinfo->GetQuestId(), qinfo->RewardSpellCast, qinfo->RewardSpellCast);
                qinfo->RewardSpellCast = 0;                    // No spell will be casted on player
            }
            else if (sSpellMgr->IsTalent(spellInfo->Id))
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `RewardSpellCast` = %u but spell %u is talent, quest will not have a spell reward.",
                    qinfo->GetQuestId(), qinfo->RewardSpellCast, qinfo->RewardSpellCast);
                qinfo->RewardSpellCast = 0;                    // No spell will be casted on player
            }
        }

        if (qinfo->RewardMailTemplateId)
        {
            if (!sMailTemplateStore.LookupEntry(qinfo->RewardMailTemplateId))
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `RewardMailTemplateId` = %u but mail template  %u does not exist, quest will not have a mail reward.",
                    qinfo->GetQuestId(), qinfo->RewardMailTemplateId, qinfo->RewardMailTemplateId);
                qinfo->RewardMailTemplateId = 0;               // No mail will send to player
                qinfo->RewardMailDelay      = 0;               // No mail will send to player
            }
            else if (usedMailTemplates.find(qinfo->RewardMailTemplateId) != usedMailTemplates.end())
            {
                std::map<uint32, uint32>::const_iterator used_mt_itr = usedMailTemplates.find(qinfo->RewardMailTemplateId);
                TC_LOG_ERROR("sql.sql", "Quest %u has `RewardMailTemplateId` = %u but mail template  %u already used for quest %u, quest will not have a mail reward.",
                    qinfo->GetQuestId(), qinfo->RewardMailTemplateId, qinfo->RewardMailTemplateId, used_mt_itr->second);
                qinfo->RewardMailTemplateId = 0;               // No mail will send to player
                qinfo->RewardMailDelay      = 0;               // No mail will send to player
            }
            else
                usedMailTemplates[qinfo->RewardMailTemplateId] = qinfo->GetQuestId();
        }

        if (qinfo->NextQuestIdChain)
        {
            QuestMap::iterator qNextItr = _questTemplates.find(qinfo->NextQuestIdChain);
            if (qNextItr == _questTemplates.end())
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `NextQuestIdChain` = %u but quest %u does not exist, quest chain will not work.",
                    qinfo->GetQuestId(), qinfo->NextQuestIdChain, qinfo->NextQuestIdChain);
                qinfo->NextQuestIdChain = 0;
            }
            else
                qNextItr->second->prevChainQuests.push_back(qinfo->GetQuestId());
        }

        for (uint8 j = 0; j < QUEST_REWARD_CURRENCY_COUNT; ++j)
        {
            if (qinfo->RewardCurrencyId[j])
            {
                if (qinfo->RewardCurrencyCount[j] == 0)
                {
                    TC_LOG_ERROR("sql.sql", "Quest %u has `RewardCurrencyId%d` = %u but `RewardCurrencyCount%d` = 0, quest can't be done.",
                        qinfo->GetQuestId(), j + 1, qinfo->RewardCurrencyId[j], j + 1);
                    // No changes, quest can't be done for this requirement
                }

                if (!sCurrencyTypesStore.LookupEntry(qinfo->RewardCurrencyId[j]))
                {
                    TC_LOG_ERROR("sql.sql", "Quest %u has `RewardCurrencyId%d` = %u but currency with entry %u does not exist, quest can't be done.",
                        qinfo->GetQuestId(), j + 1, qinfo->RewardCurrencyId[j], qinfo->RewardCurrencyId[j]);
                    qinfo->RewardCurrencyCount[j] = 0;         // Prevent incorrect work of quest
                }
            }
            else if (qinfo->RewardCurrencyCount[j] > 0)
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `RewardCurrencyId%d` = 0 but `RewardCurrencyCount%d` = %u, quest can't be done.",
                    qinfo->GetQuestId(), j + 1, j + 1, qinfo->RewardCurrencyCount[j]);
                qinfo->RewardCurrencyCount[j] = 0;             // Prevent incorrect work of quest
            }
        }

        if (qinfo->SoundAccept)
        {
            if (!sSoundEntriesStore.LookupEntry(qinfo->SoundAccept))
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `SoundAccept` = %u but sound %u does not exist, set to 0.",
                    qinfo->GetQuestId(), qinfo->SoundAccept, qinfo->SoundAccept);
                qinfo->SoundAccept = 0;                        // No sound will be played
            }
        }

        if (qinfo->SoundTurnIn)
        {
            if (!sSoundEntriesStore.LookupEntry(qinfo->SoundTurnIn))
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `SoundTurnIn` = %u but sound %u does not exist, set to 0.",
                    qinfo->GetQuestId(), qinfo->SoundTurnIn, qinfo->SoundTurnIn);
                qinfo->SoundTurnIn = 0;                        // No sound will be played
            }
        }

        if (qinfo->RewardSkillId)
        {
            if (!sSkillLineStore.LookupEntry(qinfo->RewardSkillId))
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `RewardSkillId` = %u but this skill does not exist",
                    qinfo->GetQuestId(), qinfo->RewardSkillId);
            }
            if (!qinfo->RewardSkillPoints)
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `RewardSkillId` = %u but `RewardSkillPoints` is 0",
                    qinfo->GetQuestId(), qinfo->RewardSkillId);
            }
        }

        if (qinfo->RewardSkillPoints)
        {
            if (qinfo->RewardSkillPoints > sWorld->GetConfigMaxSkillValue())
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `RewardSkillPoints` = %u but max possible skill is %u, quest can't be done.",
                    qinfo->GetQuestId(), qinfo->RewardSkillPoints, sWorld->GetConfigMaxSkillValue());
                // No changes, quest can't be done for this requirement
            }
            if (!qinfo->RewardSkillId)
            {
                TC_LOG_ERROR("sql.sql", "Quest %u has `RewardSkillPoints` = %u but `RewardSkillId` is 0",
                    qinfo->GetQuestId(), qinfo->RewardSkillPoints);
            }
        }

        // Fill additional data stores
        if (qinfo->PrevQuestId)
        {
            if (_questTemplates.find(abs(qinfo->GetPrevQuestId())) == _questTemplates.end())
                TC_LOG_ERROR("sql.sql", "Quest %d has PrevQuestId %i, but no such quest", qinfo->GetQuestId(), qinfo->GetPrevQuestId());
            else
                qinfo->prevQuests.push_back(qinfo->PrevQuestId);
        }

        if (qinfo->NextQuestId)
        {
            QuestMap::iterator qNextItr = _questTemplates.find(abs(qinfo->GetNextQuestId()));
            if (qNextItr == _questTemplates.end())
                TC_LOG_ERROR("sql.sql", "Quest %d has NextQuestId %i, but no such quest", qinfo->GetQuestId(), qinfo->GetNextQuestId());
            else
            {
                int32 signedQuestId = qinfo->NextQuestId < 0 ? -int32(qinfo->GetQuestId()) : int32(qinfo->GetQuestId());
                qNextItr->second->prevQuests.push_back(signedQuestId);
            }
        }

        if (qinfo->ExclusiveGroup)
            mExclusiveQuestGroups.insert(std::pair<int32, uint32>(qinfo->ExclusiveGroup, qinfo->GetQuestId()));
        if (qinfo->LimitTime)
            qinfo->SetSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED);
    }

    // Check QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT for spell with SPELL_EFFECT_QUEST_COMPLETE
    for (uint32 i = 0; i < sSpellMgr->GetSpellInfoStoreSize(); ++i)
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(i);
        if (!spellInfo)
            continue;

        for (auto const &spellEffect : spellInfo->Effects)
        {
            if (spellEffect.Effect != SPELL_EFFECT_QUEST_COMPLETE)
                continue;

            uint32 quest_id = spellEffect.MiscValue;

            Quest const* quest = GetQuestTemplate(quest_id);

            // Some quest referenced in spells not exist (outdated spells)
            if (!quest)
                continue;

            if (!quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT))
            {
                TC_LOG_ERROR("sql.sql", "Spell (id: %u) have SPELL_EFFECT_QUEST_COMPLETE for quest %u, but quest not have flag QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT. Quest flags must be fixed, quest modified to enable objective.", spellInfo->Id, quest_id);

                // This will prevent quest completing without objective
                const_cast<Quest*>(quest)->SetSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT);
            }
        }
    }

    TC_LOG_INFO("server.loading", ">> Loaded %lu quests definitions in %u ms", (unsigned long)_questTemplates.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadQuestObjectives()
{
    uint32 oldMSTime = getMSTime();

    // reload case
    for (auto &questObjective : m_questObjectives)
        delete questObjective.second;

    m_questObjectives.clear();

    QueryResult result = WorldDatabase.Query("SELECT `questId`, `id`, `index`, `type`, `objectId`, `amount`, `flags`, `description` FROM `quest_objective` ORDER BY `questId` ASC");
    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 Quest Objectives. DB table `quest_objective` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 questId          = fields[0].GetUInt32();
        uint32 id               = fields[1].GetUInt32();
        uint8 index             = fields[2].GetUInt8();
        uint8 type              = fields[3].GetUInt8();
        uint32 objectId         = fields[4].GetUInt32();
        int32 amount            = fields[5].GetInt32();
        uint32 flags            = fields[6].GetUInt32();
        std::string description = fields[7].GetString();

        if (!GetQuestTemplate(questId))
        {
            TC_LOG_ERROR("sql.sql", "Quest Objective %u has non existant Quest Id %u! Skipping.", id, questId);
            continue;
        }

        if (DisableMgr::IsDisabledFor(DISABLE_TYPE_QUEST, questId, NULL))
            continue;

        if (type >= QUEST_OBJECTIVE_TYPE_END)
        {
            TC_LOG_ERROR("sql.sql", "Quest Objective %u has invalid type %u! Skipping.", id, type);
            continue;
        }

        // quest existance was checked above, this shouldn't cause an issue
        Quest* quest = _questTemplates.find(questId)->second;

        switch (type)
        {
            case QUEST_OBJECTIVE_TYPE_NPC:
            case QUEST_OBJECTIVE_TYPE_NPC_INTERACT:
            {
                if (!GetCreatureTemplate(objectId))
                {
                    TC_LOG_ERROR("sql.sql", "Quest Objective %u has non existant Creature Id %u! Skipping.", id, objectId);
                    continue;
                }

                if (amount <= 0)
                {
                    TC_LOG_ERROR("sql.sql", "Quest Objective %u has valid Creature Id %u but amount %u is invalid! Skipping.", id, objectId, amount);
                    continue;
                }

                break;
            }
            case QUEST_OBJECTIVE_TYPE_ITEM:
            {
                if (!GetItemTemplate(objectId))
                {
                    TC_LOG_ERROR("sql.sql", "Quest Objective %u has non existant Item Id %u! Skipping.", id, objectId);
                    continue;
                }

                if (amount <= 0)
                {
                    TC_LOG_ERROR("sql.sql", "Quest Objective %u has valid Item Id %u but amount %u is invalid! Skipping.", id, objectId, amount);
                    continue;
                }

                break;
            }
            case QUEST_OBJECTIVE_TYPE_GO:
            {
                if (!GetGameObjectTemplate(objectId))
                {
                    TC_LOG_ERROR("sql.sql", "Quest Objective %u has non existant GameObject Id %u! Skipping.", id, objectId);
                    continue;
                }

                if (amount <= 0)
                {
                    TC_LOG_ERROR("sql.sql", "Quest Objective %u has valid GameObject Id %u but amount %u is invalid! Skipping.", id, objectId, amount);
                    continue;
                }

                break;
            }
            case QUEST_OBJECTIVE_TYPE_CURRENCY:
            {
                if (!sCurrencyTypesStore.LookupEntry(objectId))
                {
                    TC_LOG_ERROR("sql.sql", "Quest Objective %u has non existant Currency Id %u! Skipping.", id, objectId);
                    continue;
                }

                break;
            }
            case QUEST_OBJECTIVE_TYPE_SPELL:
            {
                if (!sSpellMgr->GetSpellInfo(objectId))
                {
                    TC_LOG_ERROR("sql.sql", "Quest Objective %u has non existant Spell Id %u! Skipping.", id, objectId);
                    continue;
                }

                if (amount <= 0)
                {
                    TC_LOG_ERROR("sql.sql", "Quest Objective %u has valid Spell Id %u but amount %u is invalid! Skipping.", id, objectId, amount);
                    continue;
                }

                break;
            }
            case QUEST_OBJECTIVE_TYPE_FACTION_REP:
            case QUEST_OBJECTIVE_TYPE_FACTION_REP2:
            {
                if (!sFactionStore.LookupEntry(objectId))
                {
                    TC_LOG_ERROR("sql.sql", "Quest Objective %u has non existant Faction Id %u! Skipping.", id, objectId);
                    continue;
                }

                break;
            }
            case QUEST_OBJECTIVE_TYPE_MONEY:
            {
                if (quest->GetQuestObjectiveCountType(type) >= 1)
                {
                    TC_LOG_ERROR("sql.sql", "Quest Objective %u is invalid, Quest %u already has the max amount of Quest Objective type %u! Skipping.", id, questId, type);
                    continue;
                }

                break;
            }
            default:
                break;
        }

        QuestObjective* questObjective = new QuestObjective(id, index, type, objectId, amount, flags, description, quest->GetQuestId());
        quest->m_questObjectives.insert(questObjective);
        quest->m_questObjecitveTypeCount[type]++;

        if (m_questObjectives.find(id) == m_questObjectives.end())
            m_questObjectives.insert(std::make_pair(id, questObjective));

        count++;

    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u Quest Objectives in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadQuestObjectiveVisualEffects()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT `objectiveId`, `visualEffect` FROM `quest_objective_effects` ORDER BY `objectiveId` ASC");
    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 Quest Objective visual effects. DB table `quest_objective_effects` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 objectiveId  = fields[0].GetUInt32();
        uint32 visualEffect = fields[1].GetUInt32();

        auto itr = m_questObjectives.find(objectiveId);
        if (itr == m_questObjectives.end())
        {
            TC_LOG_ERROR("sql.sql", "Visual effect %u has non existant Quest Objective Id %u! Skipping.", visualEffect, objectiveId);
            continue;
        }

        itr->second->VisualEffects.push_back(visualEffect);
        count++;

    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u Quest Objective visual effects in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadQuestRelations()
{
    TC_LOG_INFO("server.loading", "Loading GO Start Quest Data...");
    LoadGameobjectQuestStarters();

    TC_LOG_INFO("server.loading", "Loading GO End Quest Data...");
    LoadGameobjectQuestEnders();

    TC_LOG_INFO("server.loading", "Loading Creature Start Quest Data...");
    LoadCreatureQuestStarters();

    TC_LOG_INFO("server.loading", "Loading Creature End Quest Data...");
    LoadCreatureQuestEnders();
}

void ObjectMgr::LoadQuestLocales()
{
    uint32 oldMSTime = getMSTime();

    _questLocaleStore.clear();                                 // Need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, "
        "Title_loc1, Details_loc1, Objectives_loc1, OfferRewardText_loc1, RequestItemsText_loc1, EndText_loc1, CompletedText_loc1, QuestGiverTextWindow_loc1, QuestGiverTargetName_loc1, QuestTurnTextWindow_loc1, QuestTurnTargetName_loc1,"
        "Title_loc2, Details_loc2, Objectives_loc2, OfferRewardText_loc2, RequestItemsText_loc2, EndText_loc2, CompletedText_loc2, QuestGiverTextWindow_loc2, QuestGiverTargetName_loc2, QuestTurnTextWindow_loc2, QuestTurnTargetName_loc2,"
        "Title_loc3, Details_loc3, Objectives_loc3, OfferRewardText_loc3, RequestItemsText_loc3, EndText_loc3, CompletedText_loc3, QuestGiverTextWindow_loc3, QuestGiverTargetName_loc3, QuestTurnTextWindow_loc3, QuestTurnTargetName_loc3,"
        "Title_loc4, Details_loc4, Objectives_loc4, OfferRewardText_loc4, RequestItemsText_loc4, EndText_loc4, CompletedText_loc4, QuestGiverTextWindow_loc4, QuestGiverTargetName_loc4, QuestTurnTextWindow_loc4, QuestTurnTargetName_loc4,"
        "Title_loc5, Details_loc5, Objectives_loc5, OfferRewardText_loc5, RequestItemsText_loc5, EndText_loc5, CompletedText_loc5, QuestGiverTextWindow_loc5, QuestGiverTargetName_loc5, QuestTurnTextWindow_loc5, QuestTurnTargetName_loc5,"
        "Title_loc6, Details_loc6, Objectives_loc6, OfferRewardText_loc6, RequestItemsText_loc6, EndText_loc6, CompletedText_loc6, QuestGiverTextWindow_loc6, QuestGiverTargetName_loc6, QuestTurnTextWindow_loc6, QuestTurnTargetName_loc6,"
        "Title_loc7, Details_loc7, Objectives_loc7, OfferRewardText_loc7, RequestItemsText_loc7, EndText_loc7, CompletedText_loc7, QuestGiverTextWindow_loc7, QuestGiverTargetName_loc7, QuestTurnTextWindow_loc7, QuestTurnTargetName_loc7,"
        "Title_loc8, Details_loc8, Objectives_loc8, OfferRewardText_loc8, RequestItemsText_loc8, EndText_loc8, CompletedText_loc8, QuestGiverTextWindow_loc8, QuestGiverTargetName_loc8, QuestTurnTextWindow_loc8, QuestTurnTargetName_loc8,"
        "Title_loc9, Details_loc9, Objectives_loc9, OfferRewardText_loc9, RequestItemsText_loc9, EndText_loc9, CompletedText_loc9, QuestGiverTextWindow_loc9, QuestGiverTargetName_loc9, QuestTurnTextWindow_loc9, QuestTurnTargetName_loc9,"
        "Title_loc10, Details_loc10, Objectives_loc10, OfferRewardText_loc10, RequestItemsText_loc10, EndText_loc10, CompletedText_loc10, QuestGiverTextWindow_loc10, QuestGiverTargetName_loc10, QuestTurnTextWindow_loc10, QuestTurnTargetName_loc10"
        " FROM locales_quest");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        QuestLocale& data = _questLocaleStore[entry];

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
        {
            LocaleConstant locale = (LocaleConstant) i;

            AddLocaleString(fields[1 + 11 * (i - 1)].GetString(), locale, data.Title);
            AddLocaleString(fields[1 + 11 * (i - 1) + 1].GetString(), locale, data.Details);
            AddLocaleString(fields[1 + 11 * (i - 1) + 2].GetString(), locale, data.Objectives);
            AddLocaleString(fields[1 + 11 * (i - 1) + 3].GetString(), locale, data.OfferRewardText);
            AddLocaleString(fields[1 + 11 * (i - 1) + 4].GetString(), locale, data.RequestItemsText);
            AddLocaleString(fields[1 + 11 * (i - 1) + 5].GetString(), locale, data.EndText);
            AddLocaleString(fields[1 + 11 * (i - 1) + 6].GetString(), locale, data.CompletedText);
            AddLocaleString(fields[1 + 11 * (i - 1) + 7].GetString(), locale, data.QuestGiverTextWindow);
            AddLocaleString(fields[1 + 11 * (i - 1) + 8].GetString(), locale, data.QuestGiverTargetName);
            AddLocaleString(fields[1 + 11 * (i - 1) + 9].GetString(), locale, data.QuestTurnTextWindow);
            AddLocaleString(fields[1 + 11 * (i - 1) + 10].GetString(), locale, data.QuestTurnTargetName);
        }
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %lu Quest locale strings in %u ms", (unsigned long)_questLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadScripts(ScriptsType type)
{
    // function cannot be called when scripts are in use.
    if (sScriptMgr->IsScriptScheduled())
        return;

    uint32 oldMSTime = getMSTime();

    ScriptMapMap* scripts = GetScriptsMapByType(type);
    if (!scripts)
        return;

    auto const tableName = GetScriptsTableNameByType(type);

    TC_LOG_INFO("server.loading", "Loading %s...", tableName);

    // need for reload support
    scripts->clear();

    bool isSpellScriptTable = (type == SCRIPTS_SPELL);
    //                                                 0    1       2         3         4          5    6  7  8  9
    QueryResult result = WorldDatabase.PQuery("SELECT id, delay, command, datalong, datalong2, dataint, x, y, z, o%s FROM %s",
                                              isSpellScriptTable ? ", effIndex" : "", tableName);

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 script definitions. DB table `%s` is empty!", tableName);
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        ScriptInfo tmp;
        tmp.type      = type;
        tmp.id           = fields[0].GetUInt32();
        if (isSpellScriptTable)
            tmp.id      |= fields[10].GetUInt8() << 24;
        tmp.delay        = fields[1].GetUInt32();
        tmp.command      = ScriptCommands(fields[2].GetUInt32());
        tmp.Raw.nData[0] = fields[3].GetUInt32();
        tmp.Raw.nData[1] = fields[4].GetUInt32();
        tmp.Raw.nData[2] = fields[5].GetInt32();
        tmp.Raw.fData[0] = fields[6].GetFloat();
        tmp.Raw.fData[1] = fields[7].GetFloat();
        tmp.Raw.fData[2] = fields[8].GetFloat();
        tmp.Raw.fData[3] = fields[9].GetFloat();

        // generic command args check
        switch (tmp.command)
        {
            case SCRIPT_COMMAND_TALK:
            {
                if (tmp.Talk.ChatType > CHAT_TYPE_WHISPER && tmp.Talk.ChatType != CHAT_MSG_RAID_BOSS_WHISPER)
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has invalid talk type (datalong = %u) in SCRIPT_COMMAND_TALK for script id %u",
                        tableName, tmp.Talk.ChatType, tmp.id);
                    continue;
                }
                if (!tmp.Talk.TextID)
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has invalid talk text id (dataint = %i) in SCRIPT_COMMAND_TALK for script id %u",
                        tableName, tmp.Talk.TextID, tmp.id);
                    continue;
                }
                if (tmp.Talk.TextID < MIN_DB_SCRIPT_STRING_ID || tmp.Talk.TextID >= MAX_DB_SCRIPT_STRING_ID)
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has out of range text id (dataint = %i expected %u-%u) in SCRIPT_COMMAND_TALK for script id %u",
                        tableName, tmp.Talk.TextID, MIN_DB_SCRIPT_STRING_ID, MAX_DB_SCRIPT_STRING_ID, tmp.id);
                    continue;
                }

                break;
            }

            case SCRIPT_COMMAND_EMOTE:
            {
                if (!sEmotesStore.LookupEntry(tmp.Emote.EmoteID))
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has invalid emote id (datalong = %u) in SCRIPT_COMMAND_EMOTE for script id %u",
                        tableName, tmp.Emote.EmoteID, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_TELEPORT_TO:
            {
                if (!sMapStore.LookupEntry(tmp.TeleportTo.MapID))
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has invalid map (Id: %u) in SCRIPT_COMMAND_TELEPORT_TO for script id %u",
                        tableName, tmp.TeleportTo.MapID, tmp.id);
                    continue;
                }

                if (!Trinity::IsValidMapCoord(tmp.TeleportTo.DestX, tmp.TeleportTo.DestY, tmp.TeleportTo.DestZ, tmp.TeleportTo.Orientation))
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has invalid coordinates (X: %f Y: %f Z: %f O: %f) in SCRIPT_COMMAND_TELEPORT_TO for script id %u",
                        tableName, tmp.TeleportTo.DestX, tmp.TeleportTo.DestY, tmp.TeleportTo.DestZ, tmp.TeleportTo.Orientation, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_QUEST_EXPLORED:
            {
                Quest const* quest = GetQuestTemplate(tmp.QuestExplored.QuestID);
                if (!quest)
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has invalid quest (ID: %u) in SCRIPT_COMMAND_QUEST_EXPLORED in `datalong` for script id %u",
                        tableName, tmp.QuestExplored.QuestID, tmp.id);
                    continue;
                }

                if (!quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT))
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has quest (ID: %u) in SCRIPT_COMMAND_QUEST_EXPLORED in `datalong` for script id %u, but quest not have flag QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT in quest flags. Script command or quest flags wrong. Quest modified to require objective.",
                        tableName, tmp.QuestExplored.QuestID, tmp.id);

                    // this will prevent quest completing without objective
                    const_cast<Quest*>(quest)->SetSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT);

                    // continue; - quest objective requirement set and command can be allowed
                }

                if (float(tmp.QuestExplored.Distance) > DEFAULT_VISIBILITY_DISTANCE)
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has too large distance (%u) for exploring objective complete in `datalong2` in SCRIPT_COMMAND_QUEST_EXPLORED in `datalong` for script id %u",
                        tableName, tmp.QuestExplored.Distance, tmp.id);
                    continue;
                }

                if (tmp.QuestExplored.Distance && float(tmp.QuestExplored.Distance) > DEFAULT_VISIBILITY_DISTANCE)
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has too large distance (%u) for exploring objective complete in `datalong2` in SCRIPT_COMMAND_QUEST_EXPLORED in `datalong` for script id %u, max distance is %f or 0 for disable distance check",
                        tableName, tmp.QuestExplored.Distance, tmp.id, DEFAULT_VISIBILITY_DISTANCE);
                    continue;
                }

                if (tmp.QuestExplored.Distance && float(tmp.QuestExplored.Distance) < INTERACTION_DISTANCE)
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has too small distance (%u) for exploring objective complete in `datalong2` in SCRIPT_COMMAND_QUEST_EXPLORED in `datalong` for script id %u, min distance is %f or 0 for disable distance check",
                        tableName, tmp.QuestExplored.Distance, tmp.id, INTERACTION_DISTANCE);
                    continue;
                }

                break;
            }

            case SCRIPT_COMMAND_KILL_CREDIT:
            {
                if (!GetCreatureTemplate(tmp.KillCredit.CreatureEntry))
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has invalid creature (Entry: %u) in SCRIPT_COMMAND_KILL_CREDIT for script id %u",
                        tableName, tmp.KillCredit.CreatureEntry, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_RESPAWN_GAMEOBJECT:
            {
                GameObjectData const* data = GetGOData(tmp.RespawnGameobject.GOGuid);
                if (!data)
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has invalid gameobject (GUID: %u) in SCRIPT_COMMAND_RESPAWN_GAMEOBJECT for script id %u",
                        tableName, tmp.RespawnGameobject.GOGuid, tmp.id);
                    continue;
                }

                GameObjectTemplate const* info = GetGameObjectTemplate(data->id);
                if (!info)
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has gameobject with invalid entry (GUID: %u Entry: %u) in SCRIPT_COMMAND_RESPAWN_GAMEOBJECT for script id %u",
                        tableName, tmp.RespawnGameobject.GOGuid, data->id, tmp.id);
                    continue;
                }

                if (info->type == GAMEOBJECT_TYPE_FISHINGNODE ||
                    info->type == GAMEOBJECT_TYPE_FISHINGHOLE ||
                    info->type == GAMEOBJECT_TYPE_DOOR        ||
                    info->type == GAMEOBJECT_TYPE_BUTTON      ||
                    info->type == GAMEOBJECT_TYPE_TRAP)
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` have gameobject type (%u) unsupported by command SCRIPT_COMMAND_RESPAWN_GAMEOBJECT for script id %u",
                        tableName, info->entry, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_TEMP_SUMMON_CREATURE:
            {
                if (!Trinity::IsValidMapCoord(tmp.TempSummonCreature.PosX, tmp.TempSummonCreature.PosY, tmp.TempSummonCreature.PosZ, tmp.TempSummonCreature.Orientation))
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has invalid coordinates (X: %f Y: %f Z: %f O: %f) in SCRIPT_COMMAND_TEMP_SUMMON_CREATURE for script id %u",
                        tableName, tmp.TempSummonCreature.PosX, tmp.TempSummonCreature.PosY, tmp.TempSummonCreature.PosZ, tmp.TempSummonCreature.Orientation, tmp.id);
                    continue;
                }

                if (!GetCreatureTemplate(tmp.TempSummonCreature.CreatureEntry))
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has invalid creature (Entry: %u) in SCRIPT_COMMAND_TEMP_SUMMON_CREATURE for script id %u",
                        tableName, tmp.TempSummonCreature.CreatureEntry, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_OPEN_DOOR:
            case SCRIPT_COMMAND_CLOSE_DOOR:
            {
                GameObjectData const* data = GetGOData(tmp.ToggleDoor.GOGuid);
                if (!data)
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has invalid gameobject (GUID: %u) in %s for script id %u",
                        tableName, tmp.ToggleDoor.GOGuid, GetScriptCommandName(tmp.command), tmp.id);
                    continue;
                }

                GameObjectTemplate const* info = GetGameObjectTemplate(data->id);
                if (!info)
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has gameobject with invalid entry (GUID: %u Entry: %u) in %s for script id %u",
                        tableName, tmp.ToggleDoor.GOGuid, data->id, GetScriptCommandName(tmp.command), tmp.id);
                    continue;
                }

                if (info->type != GAMEOBJECT_TYPE_DOOR)
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has gameobject type (%u) non supported by command %s for script id %u",
                        tableName, info->entry, GetScriptCommandName(tmp.command), tmp.id);
                    continue;
                }

                break;
            }

            case SCRIPT_COMMAND_REMOVE_AURA:
            {
                if (!sSpellMgr->GetSpellInfo(tmp.RemoveAura.SpellID))
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` using non-existent spell (id: %u) in SCRIPT_COMMAND_REMOVE_AURA for script id %u",
                        tableName, tmp.RemoveAura.SpellID, tmp.id);
                    continue;
                }
                if (tmp.RemoveAura.Flags & ~0x1)                    // 1 bits (0, 1)
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` using unknown flags in datalong2 (%u) in SCRIPT_COMMAND_REMOVE_AURA for script id %u",
                        tableName, tmp.RemoveAura.Flags, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_CAST_SPELL:
            {
                if (!sSpellMgr->GetSpellInfo(tmp.CastSpell.SpellID))
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` using non-existent spell (id: %u) in SCRIPT_COMMAND_CAST_SPELL for script id %u",
                        tableName, tmp.CastSpell.SpellID, tmp.id);
                    continue;
                }
                if (tmp.CastSpell.Flags > 4)                      // targeting type
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` using unknown target in datalong2 (%u) in SCRIPT_COMMAND_CAST_SPELL for script id %u",
                        tableName, tmp.CastSpell.Flags, tmp.id);
                    continue;
                }
                if (tmp.CastSpell.Flags != 4 && tmp.CastSpell.CreatureEntry & ~0x1)                      // 1 bit (0, 1)
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` using unknown flags in dataint (%u) in SCRIPT_COMMAND_CAST_SPELL for script id %u",
                        tableName, tmp.CastSpell.CreatureEntry, tmp.id);
                    continue;
                }
                else if (tmp.CastSpell.Flags == 4 && !GetCreatureTemplate(tmp.CastSpell.CreatureEntry))
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` using invalid creature entry in dataint (%u) in SCRIPT_COMMAND_CAST_SPELL for script id %u",
                        tableName, tmp.CastSpell.CreatureEntry, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_CREATE_ITEM:
            {
                if (!GetItemTemplate(tmp.CreateItem.ItemEntry))
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` has nonexistent item (entry: %u) in SCRIPT_COMMAND_CREATE_ITEM for script id %u",
                        tableName, tmp.CreateItem.ItemEntry, tmp.id);
                    continue;
                }
                if (!tmp.CreateItem.Amount)
                {
                    TC_LOG_ERROR("sql.sql", "Table `%s` SCRIPT_COMMAND_CREATE_ITEM but amount is %u for script id %u",
                        tableName, tmp.CreateItem.Amount, tmp.id);
                    continue;
                }
                break;
            }
            default:
                break;
        }

        if (scripts->find(tmp.id) == scripts->end())
        {
            ScriptMap emptyMap;
            (*scripts)[tmp.id] = emptyMap;
        }
        (*scripts)[tmp.id].insert(std::pair<uint32, ScriptInfo>(tmp.delay, tmp));

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u script definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadGameObjectScripts()
{
    LoadScripts(SCRIPTS_GAMEOBJECT);

    // check ids
    for (ScriptMapMap::const_iterator itr = sGameObjectScripts.begin(); itr != sGameObjectScripts.end(); ++itr)
    {
        if (!GetGOData(itr->first))
            TC_LOG_ERROR("sql.sql", "Table `gameobject_scripts` has not existing gameobject (GUID: %u) as script id", itr->first);
    }
}

void ObjectMgr::LoadQuestEndScripts()
{
    LoadScripts(SCRIPTS_QUEST_END);

    // check ids
    for (ScriptMapMap::const_iterator itr = sQuestEndScripts.begin(); itr != sQuestEndScripts.end(); ++itr)
    {
        if (!GetQuestTemplate(itr->first))
            TC_LOG_ERROR("sql.sql", "Table `quest_end_scripts` has not existing quest (Id: %u) as script id", itr->first);
    }
}

void ObjectMgr::LoadQuestStartScripts()
{
    LoadScripts(SCRIPTS_QUEST_START);

    // check ids
    for (ScriptMapMap::const_iterator itr = sQuestStartScripts.begin(); itr != sQuestStartScripts.end(); ++itr)
    {
        if (!GetQuestTemplate(itr->first))
            TC_LOG_ERROR("sql.sql", "Table `quest_start_scripts` has not existing quest (Id: %u) as script id", itr->first);
    }
}

void ObjectMgr::LoadSpellScripts()
{
    LoadScripts(SCRIPTS_SPELL);

    // check ids
    for (ScriptMapMap::const_iterator itr = sSpellScripts.begin(); itr != sSpellScripts.end(); ++itr)
    {
        uint32 spellId = uint32(itr->first) & 0x00FFFFFF;
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);

        if (!spellInfo)
        {
            TC_LOG_ERROR("sql.sql", "Table `spell_scripts` has not existing spell (Id: %u) as script id", spellId);
            continue;
        }

        uint8 const i = (uint8)((uint32(itr->first) >> 24) & 0x000000FF);

        // check for correct spellEffect
        auto const &spellEffect = spellInfo->Effects.at(i);
        if (!spellEffect.Effect || (spellEffect.Effect != SPELL_EFFECT_SCRIPT_EFFECT && spellEffect.Effect != SPELL_EFFECT_DUMMY))
            TC_LOG_ERROR("sql.sql", "Table `spell_scripts` - spell %u effect %u is not SPELL_EFFECT_SCRIPT_EFFECT or SPELL_EFFECT_DUMMY", spellId, i);
    }
}

void ObjectMgr::LoadEventScripts()
{
    LoadScripts(SCRIPTS_EVENT);

    std::set<uint32> evt_scripts;
    // Load all possible script entries from gameobjects
    GameObjectTemplateContainer const* gotc = sObjectMgr->GetGameObjectTemplates();
    for (GameObjectTemplateContainer::const_iterator itr = gotc->begin(); itr != gotc->end(); ++itr)
        if (uint32 eventId = itr->second.GetEventScriptId())
            evt_scripts.insert(eventId);

    // Load all possible script entries from spells
    for (uint32 i = 1; i < sSpellMgr->GetSpellInfoStoreSize(); ++i)
    {
        auto const spell = sSpellMgr->GetSpellInfo(i);
        if (!spell)
            continue;

        for (auto const &spellEffect : spell->Effects)
            if (spellEffect.Effect == SPELL_EFFECT_SEND_EVENT && spellEffect.MiscValue)
                evt_scripts.insert(spellEffect.MiscValue);
    }

    for (size_t path_idx = 0; path_idx < sTaxiPathNodesByPath.size(); ++path_idx)
    {
        for (size_t node_idx = 0; node_idx < sTaxiPathNodesByPath[path_idx].size(); ++node_idx)
        {
            TaxiPathNodeEntry const& node = sTaxiPathNodesByPath[path_idx][node_idx];

            if (node.arrivalEventID)
                evt_scripts.insert(node.arrivalEventID);

            if (node.departureEventID)
                evt_scripts.insert(node.departureEventID);
        }
    }

    // Then check if all scripts are in above list of possible script entries
    for (ScriptMapMap::const_iterator itr = sEventScripts.begin(); itr != sEventScripts.end(); ++itr)
    {
        std::set<uint32>::const_iterator itr2 = evt_scripts.find(itr->first);
        if (itr2 == evt_scripts.end())
            TC_LOG_ERROR("sql.sql", "Table `event_scripts` has script (Id: %u) not referring to any gameobject_template type 10 data2 field, type 3 data6 field, type 13 data 2 field or any spell effect %u",
                itr->first, SPELL_EFFECT_SEND_EVENT);
    }
}

//Load WP Scripts
void ObjectMgr::LoadWaypointScripts()
{
    LoadScripts(SCRIPTS_WAYPOINT);

    std::set<uint32> actionSet;

    for (ScriptMapMap::const_iterator itr = sWaypointScripts.begin(); itr != sWaypointScripts.end(); ++itr)
        actionSet.insert(itr->first);

    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_WAYPOINT_DATA_ACTION);
    PreparedQueryResult result = WorldDatabase.Query(stmt);

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 action = fields[0].GetUInt32();

            actionSet.erase(action);
        }
        while (result->NextRow());
    }

    for (std::set<uint32>::iterator itr = actionSet.begin(); itr != actionSet.end(); ++itr)
        TC_LOG_ERROR("sql.sql", "There is no waypoint which links to the waypoint script %u", *itr);
}

void ObjectMgr::LoadSpellScriptNames()
{
    uint32 oldMSTime = getMSTime();

    _spellScriptsStore.clear();                            // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT spell_id, ScriptName FROM spell_script_names");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 spell script names. DB table `spell_script_names` is empty!");
        return;
    }

    uint32 count = 0;

    do
    {

        Field* fields = result->Fetch();

        int32 spellId          = fields[0].GetInt32();
        const char *scriptName = fields[1].GetCString();

        bool allRanks = false;
        if (spellId <= 0)
        {
            allRanks = true;
            spellId = -spellId;
        }

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        if (!spellInfo)
        {
            TC_LOG_ERROR("sql.sql", "Scriptname:`%s` spell (spell_id:%d) does not exist in `Spell.dbc`.", scriptName, fields[0].GetInt32());
            continue;
        }

        if (allRanks)
        {
            if (sSpellMgr->GetFirstSpellInChain(spellId) != uint32(spellId))
            {
                TC_LOG_ERROR("sql.sql", "Scriptname:`%s` spell (spell_id:%d) is not first rank of spell.", scriptName, fields[0].GetInt32());
                continue;
            }
            while (spellInfo)
            {
                _spellScriptsStore.insert(SpellScriptsContainer::value_type(spellInfo->Id, GetScriptId(scriptName)));
                spellInfo = sSpellMgr->GetSpellInfo(spellInfo->Id)->GetNextRankSpell();
            }
        }
        else
            _spellScriptsStore.insert(SpellScriptsContainer::value_type(spellInfo->Id, GetScriptId(scriptName)));
        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u spell script names in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::ValidateSpellScripts()
{
    uint32 oldMSTime = getMSTime();

    if (_spellScriptsStore.empty())
    {
        TC_LOG_INFO("server.loading", ">> Validated 0 scripts.");
        return;
    }

    uint32 count = 0;

    for (SpellScriptsContainer::iterator itr = _spellScriptsStore.begin(); itr != _spellScriptsStore.end();)
    {
        SpellInfo const* spellEntry = sSpellMgr->GetSpellInfo(itr->first);
        std::vector<std::pair<SpellScriptLoader *, SpellScriptsContainer::iterator> > SpellScriptLoaders;
        sScriptMgr->CreateSpellScriptLoaders(itr->first, SpellScriptLoaders);
        itr = _spellScriptsStore.upper_bound(itr->first);

        for (std::vector<std::pair<SpellScriptLoader *, SpellScriptsContainer::iterator> >::iterator sitr = SpellScriptLoaders.begin(); sitr != SpellScriptLoaders.end(); ++sitr)
        {
            SpellScript* spellScript = sitr->first->GetSpellScript();
            AuraScript* auraScript = sitr->first->GetAuraScript();
            bool valid = true;
            if (!spellScript && !auraScript)
            {
                TC_LOG_ERROR("scripts", "Functions GetSpellScript() and GetAuraScript() of script `%s` do not return objects - script skipped",  GetScriptName(sitr->second->second));
                valid = false;
            }
            if (spellScript)
            {
                spellScript->_Init(&sitr->first->GetName(), spellEntry->Id);
                spellScript->_Register();
                if (!spellScript->_Validate(spellEntry))
                    valid = false;
                delete spellScript;
            }
            if (auraScript)
            {
                auraScript->_Init(&sitr->first->GetName(), spellEntry->Id);
                auraScript->_Register();
                if (!auraScript->_Validate(spellEntry))
                    valid = false;
                delete auraScript;
            }
            if (!valid)
            {
                _spellScriptsStore.erase(sitr->second);
            }
        }
        ++count;
    }

    TC_LOG_INFO("server.loading", ">> Validated %u scripts in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadPageTexts()
{
    uint32 oldMSTime = getMSTime();

    //                                                 0      1       2
    QueryResult result = WorldDatabase.Query("SELECT entry, text, next_page FROM page_text");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 page texts. DB table `page_text` is empty!");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        PageText& pageText = _pageTextStore[fields[0].GetUInt32()];

        pageText.Text     = fields[1].GetString();
        pageText.NextPage = fields[2].GetUInt32();

        ++count;
    }
    while (result->NextRow());

    for (PageTextContainer::const_iterator itr = _pageTextStore.begin(); itr != _pageTextStore.end(); ++itr)
    {
        if (itr->second.NextPage)
        {
            PageTextContainer::const_iterator itr2 = _pageTextStore.find(itr->second.NextPage);
            if (itr2 == _pageTextStore.end())
                TC_LOG_ERROR("sql.sql", "Page text (Id: %u) has not existing next page (Id: %u)", itr->first, itr->second.NextPage);

        }
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u page texts in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

PageText const* ObjectMgr::GetPageText(uint32 pageEntry)
{
    PageTextContainer::const_iterator itr = _pageTextStore.find(pageEntry);
    if (itr != _pageTextStore.end())
        return &(itr->second);

    return NULL;
}

void ObjectMgr::LoadPageTextLocales()
{
    uint32 oldMSTime = getMSTime();

    _pageTextLocaleStore.clear();                             // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, text_loc1, text_loc2, text_loc3, text_loc4, text_loc5, text_loc6, text_loc7, text_loc8, text_loc9, text_loc10 FROM locales_page_text");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        PageTextLocale& data = _pageTextLocaleStore[entry];

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
            AddLocaleString(fields[i].GetString(), LocaleConstant(i), data.Text);
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %lu PageText locale strings in %u ms", (unsigned long)_pageTextLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadInstanceTemplate()
{
    uint32 oldMSTime = getMSTime();

    //                                                0     1       2        4
    QueryResult result = WorldDatabase.Query("SELECT map, parent, script, allowMount FROM instance_template");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 instance templates. DB table `page_text` is empty!");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint16 mapID = fields[0].GetUInt16();

        if (!MapManager::IsValidMAP(mapID, true))
        {
            TC_LOG_ERROR("sql.sql", "ObjectMgr::LoadInstanceTemplate: bad mapid %d for template!", mapID);
            continue;
        }

        InstanceTemplate instanceTemplate;

        instanceTemplate.AllowMount = fields[3].GetBool();
        instanceTemplate.Parent     = uint32(fields[1].GetUInt16());
        instanceTemplate.ScriptId   = sObjectMgr->GetScriptId(fields[2].GetCString());

        _instanceTemplateStore[mapID] = instanceTemplate;

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u instance templates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

InstanceTemplate const* ObjectMgr::GetInstanceTemplate(uint32 mapID)
{
    InstanceTemplateContainer::const_iterator itr = _instanceTemplateStore.find(uint16(mapID));
    if (itr != _instanceTemplateStore.end())
        return &(itr->second);

    return NULL;
}

void ObjectMgr::LoadInstanceEncounters()
{
    uint32 oldMSTime = getMSTime();

    //                                                0      1        2             3       4       5        6
    QueryResult result = WorldDatabase.Query("SELECT `map`, `index`, `difficulty`, `name`, `type`, `entry`, `dungeon` FROM `instance_encounters`");
    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 instance encounters, table is empty!");
        return;
    }

    uint32 count = 0;
    std::map<uint32, DungeonEncounter const*> dungeonLastBosses;
    do
    {
        Field* fields = result->Fetch();

        std::unique_ptr<DungeonEncounter> dungeonEncounter(new DungeonEncounter);

        dungeonEncounter->mapId = fields[0].GetUInt32();
        dungeonEncounter->encounterIndex = fields[1].GetUInt32();
        dungeonEncounter->difficulty = fields[2].GetInt32();
        dungeonEncounter->encounterName = fields[3].GetCString();
        dungeonEncounter->creditType = EncounterCreditType(fields[4].GetUInt8());
        dungeonEncounter->creditEntry = fields[5].GetUInt32();
        dungeonEncounter->dungeon = fields[6].GetUInt32();

        if (dungeonEncounter->dungeon && !sLFGDungeonStore.LookupEntry(dungeonEncounter->dungeon))
        {
            TC_LOG_ERROR("sql.sql", "Table `instance_encounters` has an encounter (%s Map: %u, difficulty %u) marked as final for invalid dungeon id %u, skipped!",
                dungeonEncounter->encounterName.c_str(), dungeonEncounter->mapId, dungeonEncounter->difficulty, dungeonEncounter->dungeon);
            continue;
        }

        std::map<uint32, DungeonEncounter const*>::const_iterator itr = dungeonLastBosses.find(dungeonEncounter->dungeon);
        if (dungeonEncounter->dungeon)
        {
            if (itr != dungeonLastBosses.end())
            {
                TC_LOG_ERROR("sql.sql", "Table `instance_encounters` specified encounter (%s Map: %u, difficulty %u) but %u (%s) is already marked as one, skipped!",
                    dungeonEncounter->encounterName.c_str(), dungeonEncounter->mapId, dungeonEncounter->difficulty, dungeonEncounter->dungeon, itr->second->encounterName.c_str());
                continue;
            }

            dungeonLastBosses[dungeonEncounter->dungeon] = dungeonEncounter.get();
        }

        switch (dungeonEncounter->creditType)
        {
            case ENCOUNTER_CREDIT_KILL_CREATURE:
            {
                CreatureTemplate *creatureInfo = GetMutableCreatureTemplate(dungeonEncounter->creditEntry);
                if (!creatureInfo)
                {
                    TC_LOG_ERROR("sql.sql", "Table `instance_encounters` has an invalid creature (entry %u) linked to the encounter (%s Map: %u, difficulty %u), skipped!",
                        dungeonEncounter->creditEntry, dungeonEncounter->encounterName.c_str(), dungeonEncounter->mapId, dungeonEncounter->difficulty);
                    continue;
                }

                creatureInfo->flags_extra |= CREATURE_FLAG_EXTRA_DUNGEON_BOSS;
                for (auto const &diffEntry : creatureInfo->DifficultyEntry)
                    if (diffEntry != 0)
                        GetMutableCreatureTemplate(diffEntry)->flags_extra |= CREATURE_FLAG_EXTRA_DUNGEON_BOSS;

                break;
            }
            case ENCOUNTER_CREDIT_CAST_SPELL:
                if (!sSpellMgr->GetSpellInfo(dungeonEncounter->creditEntry))
                {
                    TC_LOG_ERROR("sql.sql", "Table `instance_encounters` has an invalid spell (entry %u) linked to the encounter (%s Map: %u, difficulty %u), skipped!",
                        dungeonEncounter->creditEntry, dungeonEncounter->encounterName.c_str(), dungeonEncounter->mapId, dungeonEncounter->difficulty);
                    continue;
                }
                break;
            default:
                TC_LOG_ERROR("sql.sql", "Table `instance_encounters` has an invalid credit type (%u) for encounter (%s Map: %u, difficulty %u), skipped!",
                    dungeonEncounter->creditEntry, dungeonEncounter->encounterName.c_str(), dungeonEncounter->mapId, dungeonEncounter->difficulty);
                continue;
        }

        DungeonEncounterList& encounters = _dungeonEncounterStore[MAKE_PAIR32(dungeonEncounter->mapId, dungeonEncounter->difficulty)];
        encounters.push_back(dungeonEncounter.release());
        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("misc", ">> Loaded %u instance encounters in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

}

void ObjectMgr::LoadLFRLootBinds()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT `id`, `type`, `entry`, `linkGroup` FROM `lfr_loot_binds`");
    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 LFR loot binds, table is empty!");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        uint32 id       = fields[0].GetUInt32();
        uint8 type      = fields[1].GetUInt8();
        uint32 entry    = fields[2].GetUInt32();
        uint8 linkGroup = fields[3].GetUInt8();

        if (type > TYPEID_GAMEOBJECT)
        {
            TC_LOG_ERROR("sql.sql", "Table `lfr_loot_binds` has an invalid LFR loot bind type (type %u) for entry (entry %u), skipped!", type, entry);
            continue;
        }

        switch (type)
        {
            case TYPEID_UNIT:
            {
                if (!GetCreatureTemplate(entry))
                {
                    TC_LOG_ERROR("sql.sql", "Table `lfr_loot_binds` has an invalid creature template (entry %u), skipped!", entry);
                    continue;
                }

                break;
            }
            case TYPEID_GAMEOBJECT:
            {
                if (!GetGameObjectTemplate(entry))
                {
                    TC_LOG_ERROR("sql.sql", "Table `lfr_loot_binds` has an invalid gameobject template (entry %u), skipped!", entry);
                    continue;
                }

                break;
            }
            default:
                break;
        }

        auto lootBind = new LFRLootBind();
        lootBind->Id = id;
        lootBind->Entry = entry;
        lootBind->Type = type;
        lootBind->LinkedGroup = linkGroup;

        m_lfrLootBinds.insert(lootBind);
        if (linkGroup)
            m_lfrLootBindsLinked[linkGroup].insert(lootBind);

        count++;
    } while (result->NextRow());

    TC_LOG_INFO("misc", ">> Loaded %u LFR loot binds in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

GossipText const* ObjectMgr::GetGossipText(uint32 Text_ID) const
{
    GossipTextContainer::const_iterator itr = _gossipTextStore.find(Text_ID);
    if (itr != _gossipTextStore.end())
        return &itr->second;
    return NULL;
}

void ObjectMgr::LoadGossipText()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT * FROM npc_text");

    int count = 0;
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded %u npc texts", count);
        return;
    }
    //_gossipTextStore.rehash(result->GetRowCount());

    int cic;

    do
    {
        ++count;
        cic = 0;

        Field* fields = result->Fetch();

        uint32 Text_ID    = fields[cic++].GetUInt32();
        if (!Text_ID)
        {
            TC_LOG_ERROR("sql.sql", "Table `npc_text` has record wit reserved id 0, ignore.");
            continue;
        }

        GossipText& gText = _gossipTextStore[Text_ID];

        for (int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; i++)
        {
            gText.Options[i].Text_0           = fields[cic++].GetString();
            gText.Options[i].Text_1           = fields[cic++].GetString();

            gText.Options[i].Language         = fields[cic++].GetUInt8();
            gText.Options[i].Probability      = fields[cic++].GetFloat();

            for (uint8 j=0; j < MAX_GOSSIP_TEXT_EMOTES; ++j)
            {
                gText.Options[i].Emotes[j]._Delay  = fields[cic++].GetUInt16();
                gText.Options[i].Emotes[j]._Emote  = fields[cic++].GetUInt16();
            }
        }
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u npc texts in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadNpcTextLocales()
{
    uint32 oldMSTime = getMSTime();

    _npcTextLocaleStore.clear();                              // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, "
        "Text0_0_loc1,  Text0_1_loc1,  Text1_0_loc1,  Text1_1_loc1,  Text2_0_loc1,  Text2_1_loc1,  Text3_0_loc1,  Text3_1_loc1,  Text4_0_loc1,  Text4_1_loc1,  Text5_0_loc1,  Text5_1_loc1,  Text6_0_loc1,  Text6_1_loc1,  Text7_0_loc1,  Text7_1_loc1,  Text8_0_loc1,   Text8_1_loc1,   Text9_0_loc1,   Text9_1_loc1, "
        "Text0_0_loc2,  Text0_1_loc2,  Text1_0_loc2,  Text1_1_loc2,  Text2_0_loc2,  Text2_1_loc2,  Text3_0_loc2,  Text3_1_loc1,  Text4_0_loc2,  Text4_1_loc2,  Text5_0_loc2,  Text5_1_loc2,  Text6_0_loc2,  Text6_1_loc2,  Text7_0_loc2,  Text7_1_loc2,  Text8_0_loc2,   Text8_1_loc2,   Text9_0_loc2,   Text9_1_loc2, "
        "Text0_0_loc3,  Text0_1_loc3,  Text1_0_loc3,  Text1_1_loc3,  Text2_0_loc3,  Text2_1_loc3,  Text3_0_loc3,  Text3_1_loc1,  Text4_0_loc3,  Text4_1_loc3,  Text5_0_loc3,  Text5_1_loc3,  Text6_0_loc3,  Text6_1_loc3,  Text7_0_loc3,  Text7_1_loc3,  Text8_0_loc3,   Text8_1_loc3,   Text9_0_loc3,   Text9_1_loc3, "
        "Text0_0_loc4,  Text0_1_loc4,  Text1_0_loc4,  Text1_1_loc4,  Text2_0_loc4,  Text2_1_loc4,  Text3_0_loc4,  Text3_1_loc1,  Text4_0_loc4,  Text4_1_loc4,  Text5_0_loc4,  Text5_1_loc4,  Text6_0_loc4,  Text6_1_loc4,  Text7_0_loc4,  Text7_1_loc4,  Text8_0_loc4,   Text8_1_loc4,   Text9_0_loc4,   Text9_1_loc4, "
        "Text0_0_loc5,  Text0_1_loc5,  Text1_0_loc5,  Text1_1_loc5,  Text2_0_loc5,  Text2_1_loc5,  Text3_0_loc5,  Text3_1_loc1,  Text4_0_loc5,  Text4_1_loc5,  Text5_0_loc5,  Text5_1_loc5,  Text6_0_loc5,  Text6_1_loc5,  Text7_0_loc5,  Text7_1_loc5,  Text8_0_loc5,   Text8_1_loc5,   Text9_0_loc5,   Text9_1_loc5, "
        "Text0_0_loc6,  Text0_1_loc6,  Text1_0_loc6,  Text1_1_loc6,  Text2_0_loc6,  Text2_1_loc6,  Text3_0_loc6,  Text3_1_loc1,  Text4_0_loc6,  Text4_1_loc6,  Text5_0_loc6,  Text5_1_loc6,  Text6_0_loc6,  Text6_1_loc6,  Text7_0_loc6,  Text7_1_loc6,  Text8_0_loc6,   Text8_1_loc6,   Text9_0_loc6,   Text9_1_loc6, "
        "Text0_0_loc7,  Text0_1_loc7,  Text1_0_loc7,  Text1_1_loc7,  Text2_0_loc7,  Text2_1_loc7,  Text3_0_loc7,  Text3_1_loc1,  Text4_0_loc7,  Text4_1_loc7,  Text5_0_loc7,  Text5_1_loc7,  Text6_0_loc7,  Text6_1_loc7,  Text7_0_loc7,  Text7_1_loc7,  Text8_0_loc7,   Text8_1_loc7,   Text9_0_loc7,   Text9_1_loc7, "
        "Text0_0_loc8,  Text0_1_loc8,  Text1_0_loc8,  Text1_1_loc8,  Text2_0_loc8,  Text2_1_loc8,  Text3_0_loc8,  Text3_1_loc1,  Text4_0_loc8,  Text4_1_loc8,  Text5_0_loc8,  Text5_1_loc8,  Text6_0_loc8,  Text6_1_loc8,  Text7_0_loc8,  Text7_1_loc8,  Text8_0_loc8,   Text8_1_loc8,   Text9_0_loc8,   Text9_1_loc8, "
        "Text0_0_loc9,  Text0_1_loc9,  Text1_0_loc9,  Text1_1_loc9,  Text2_0_loc9,  Text2_1_loc9,  Text3_0_loc9,  Text3_1_loc9,  Text4_0_loc9,  Text4_1_loc9,  Text5_0_loc9,  Text5_1_loc9,  Text6_0_loc9,  Text6_1_loc9,  Text7_0_loc9,  Text7_1_loc9,  Text8_0_loc9,   Text8_1_loc9,   Text9_0_loc9,   Text9_1_loc9, "
        "Text0_0_loc10, Text0_1_loc10, Text1_0_loc10, Text1_1_loc10, Text2_0_loc10, Text2_1_loc10, Text3_0_loc10, Text3_1_loc10, Text4_0_loc10, Text4_1_loc10, Text5_0_loc10, Text5_1_loc10, Text6_0_loc10, Text6_1_loc10, Text7_0_loc10, Text7_1_loc10, Text8_0_loc10,  Text8_1_loc10,  Text9_0_loc10,  Text9_1_loc10 "
        " FROM locales_npc_text");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        NpcTextLocale& data = _npcTextLocaleStore[entry];

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
        {
            LocaleConstant locale = (LocaleConstant) i;
            for (uint8 j = 0; j < MAX_LOCALES; ++j)
            {
                AddLocaleString(fields[1 + 8 * 2 * (i - 1) + 2 * j].GetString(), locale, data.Text_0[j]);
                AddLocaleString(fields[1 + 8 * 2 * (i - 1) + 2 * j + 1].GetString(), locale, data.Text_1[j]);
            }
        }
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %lu NpcText locale strings in %u ms", (unsigned long)_npcTextLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

//not very fast function but it is called only once a day, or on starting-up
void ObjectMgr::ReturnOrDeleteOldMails(bool serverUp)
{
    uint32 oldMSTime = getMSTime();

    time_t curTime = time(NULL);
    tm* lt = localtime(&curTime);
    uint64 basetime(curTime);
    TC_LOG_INFO("misc", "Returning mails current time: hour: %d, minute: %d, second: %d ", lt->tm_hour, lt->tm_min, lt->tm_sec);

    // Delete all old mails without item and without body immediately, if starting server
    if (!serverUp)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_EMPTY_EXPIRED_MAIL);
        stmt->setUInt64(0, basetime);
        CharacterDatabase.DirectExecute(stmt);
    }

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_EXPIRED_MAIL);
    stmt->setUInt64(0, basetime);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> No expired mails found.");
        return;                                             // any mails need to be returned or deleted
    }

    std::map<uint32 /*messageId*/, MailItemInfoVec> itemsCache;
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_EXPIRED_MAIL_ITEMS);
    stmt->setUInt32(0, (uint32)basetime);
    if (PreparedQueryResult items = CharacterDatabase.Query(stmt))
    {
        MailItemInfo item;
        do
        {
            Field* fields = items->Fetch();
            item.item_guid = fields[0].GetUInt32();
            item.item_template = fields[1].GetUInt32();
            uint32 mailId = fields[2].GetUInt32();
            itemsCache[mailId].push_back(item);
        }
        while (items->NextRow());
    }

    uint32 deletedCount = 0;
    uint32 returnedCount = 0;
    do
    {
        Field* fields = result->Fetch();
        Mail* m = new Mail;
        m->messageID      = fields[0].GetUInt32();
        m->messageType    = fields[1].GetUInt8();
        m->sender         = fields[2].GetUInt32();
        m->receiver       = fields[3].GetUInt32();
        bool has_items    = fields[4].GetBool();
        m->expire_time    = time_t(fields[5].GetUInt32());
        m->deliver_time   = 0;
        m->COD            = fields[6].GetUInt64();
        m->checked        = fields[7].GetUInt8();
        m->mailTemplateId = fields[8].GetInt16();

        Player* player = NULL;
        if (serverUp)
            player = ObjectAccessor::FindPlayer(MAKE_NEW_GUID(m->receiver, 0, HIGHGUID_PLAYER));

        if (player && player->m_mailsLoaded)
        {                                                   // this code will run very improbably (the time is between 4 and 5 am, in game is online a player, who has old mail
            // his in mailbox and he has already listed his mails)
            delete m;
            continue;
        }

        SQLTransaction trans = CharacterDatabase.BeginTransaction();

        // Delete or return mail
        if (has_items)
        {
            // read items from cache
            m->items.swap(itemsCache[m->messageID]);

            // if it is mail from non-player, or if it's already return mail, it shouldn't be returned, but deleted
            if (m->messageType != MAIL_NORMAL
                    || (m->checked & (MAIL_CHECK_MASK_COD_PAYMENT | MAIL_CHECK_MASK_RETURNED))
                    || !GetPlayerAccountIdByGUID(MAKE_NEW_GUID(m->sender, 0, HIGHGUID_PLAYER)))
            {
                // mail open and then not returned
                for (MailItemInfoVec::iterator itr2 = m->items.begin(); itr2 != m->items.end(); ++itr2)
                {
                    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_INSTANCE);
                    stmt->setUInt32(0, itr2->item_guid);
                    trans->Append(stmt);
                }
            }
            else
            {
                // Mail will be returned
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_MAIL_RETURNED);
                stmt->setUInt32(0, m->receiver);
                stmt->setUInt32(1, m->sender);
                stmt->setUInt32(2, basetime + 30 * DAY);
                stmt->setUInt32(3, basetime);
                stmt->setUInt8 (4, uint8(MAIL_CHECK_MASK_RETURNED));
                stmt->setUInt32(5, m->messageID);
                trans->Append(stmt);

                // Better to have synchronous execution at startup, as in case of long
                // downtime there may be a lot of expired mails
                if (!serverUp)
                    CharacterDatabase.DirectCommitTransaction(trans);
                else
                    CharacterDatabase.CommitTransaction(trans);

                delete m;
                ++returnedCount;
                continue;
            }
        }

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL_BY_ID);
        stmt->setUInt32(0, m->messageID);
        trans->Append(stmt);

        // Better to have synchronous execution at startup, as in case of long
        // downtime there may be a lot of expired mails
        if (!serverUp)
            CharacterDatabase.DirectCommitTransaction(trans);
        else
            CharacterDatabase.CommitTransaction(trans);

        delete m;
        ++deletedCount;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Processed %u expired mails: %u deleted and %u returned in %u ms", deletedCount + returnedCount, deletedCount, returnedCount, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadQuestAreaTriggers()
{
    uint32 oldMSTime = getMSTime();

    _questAreaTriggerStore.clear();                           // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT id, quest FROM areatrigger_involvedrelation");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 quest trigger points. DB table `areatrigger_involvedrelation` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        ++count;

        Field* fields = result->Fetch();

        uint32 trigger_ID = fields[0].GetUInt32();
        uint32 quest_ID   = fields[1].GetUInt32();

        AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(trigger_ID);
        if (!atEntry)
        {
            TC_LOG_ERROR("sql.sql", "Area trigger (ID:%u) does not exist in `AreaTrigger.dbc`.", trigger_ID);
            continue;
        }

        Quest const* quest = GetQuestTemplate(quest_ID);

        if (!quest)
        {
            TC_LOG_ERROR("sql.sql", "Table `areatrigger_involvedrelation` has record (id: %u) for not existing quest %u", trigger_ID, quest_ID);
            continue;
        }

        if (!quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT))
        {
            TC_LOG_ERROR("sql.sql", "Table `areatrigger_involvedrelation` has record (id: %u) for not quest %u, but quest not have flag QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT. Trigger or quest flags must be fixed, quest modified to require objective.", trigger_ID, quest_ID);

            // this will prevent quest completing without objective
            const_cast<Quest*>(quest)->SetSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT);

            // continue; - quest modified to required objective and trigger can be allowed.
        }

        _questAreaTriggerStore[trigger_ID] = quest_ID;

    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u quest trigger points in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadTavernAreaTriggers()
{
    uint32 oldMSTime = getMSTime();

    _tavernAreaTriggerStore.clear();                          // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT id FROM areatrigger_tavern");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 tavern triggers. DB table `areatrigger_tavern` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        ++count;

        Field* fields = result->Fetch();

        uint32 Trigger_ID      = fields[0].GetUInt32();

        AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(Trigger_ID);
        if (!atEntry)
        {
            TC_LOG_ERROR("sql.sql", "Area trigger (ID:%u) does not exist in `AreaTrigger.dbc`.", Trigger_ID);
            continue;
        }

        _tavernAreaTriggerStore.insert(Trigger_ID);
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u tavern triggers in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadAreaTriggerScripts()
{
    uint32 oldMSTime = getMSTime();

    _areaTriggerScriptStore.clear();                            // need for reload case
    QueryResult result = WorldDatabase.Query("SELECT entry, ScriptName FROM areatrigger_scripts");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 areatrigger scripts. DB table `areatrigger_scripts` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        ++count;

        Field* fields = result->Fetch();

        uint32 Trigger_ID      = fields[0].GetUInt32();
        const char *scriptName = fields[1].GetCString();

        AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(Trigger_ID);
        if (!atEntry)
        {
            TC_LOG_ERROR("sql.sql", "Area trigger (ID:%u) does not exist in `AreaTrigger.dbc`.", Trigger_ID);
            continue;
        }
        _areaTriggerScriptStore[Trigger_ID] = GetScriptId(scriptName);
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u areatrigger scripts in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

uint32 ObjectMgr::GetNearestTaxiNode(float x, float y, float z, uint32 mapid, uint32 team)
{
    bool found = false;
    float dist = 10000;
    uint32 id = 0;

    for (uint32 i = 1; i < sTaxiNodesStore.GetNumRows(); ++i)
    {
        TaxiNodesEntry const* node = sTaxiNodesStore.LookupEntry(i);

        if (!node || node->map_id != mapid || (!node->MountCreatureID[team == ALLIANCE ? 1 : 0] && node->MountCreatureID[0] != 32981)) // dk flight
            continue;

        uint8  field   = (uint8)((i - 1) / 8);
        uint32 submask = 1 << ((i-1) % 8);

        // skip not taxi network nodes
        if ((sTaxiNodesMask[field] & submask) == 0)
            continue;

        float dist2 = (node->x - x)*(node->x - x)+(node->y - y)*(node->y - y)+(node->z - z)*(node->z - z);
        if (found)
        {
            if (dist2 < dist)
            {
                dist = dist2;
                id = i;
            }
        }
        else
        {
            found = true;
            dist = dist2;
            id = i;
        }
    }

    return id;
}

void ObjectMgr::GetTaxiPath(uint32 source, uint32 destination, uint32 &path, uint32 &cost)
{
    TaxiPathSetBySource::iterator src_i = sTaxiPathSetBySource.find(source);
    if (src_i == sTaxiPathSetBySource.end())
    {
        path = 0;
        cost = 0;
        return;
    }

    TaxiPathSetForSource& pathSet = src_i->second;

    TaxiPathSetForSource::iterator dest_i = pathSet.find(destination);
    if (dest_i == pathSet.end())
    {
        path = 0;
        cost = 0;
        return;
    }

    cost = dest_i->second.price;
    path = dest_i->second.ID;
}

uint32 ObjectMgr::GetTaxiMountDisplayId(uint32 id, uint32 team, bool allowed_alt_team /* = false */)
{
    uint32 mount_id = 0;

    // select mount creature id
    TaxiNodesEntry const* node = sTaxiNodesStore.LookupEntry(id);
    if (node)
    {
        uint32 mount_entry = 0;
        if (team == ALLIANCE)
            mount_entry = node->MountCreatureID[1];
        else
            mount_entry = node->MountCreatureID[0];

        // Fix for Alliance not being able to use Acherus taxi
        // only one mount type for both sides
        if (mount_entry == 0 && allowed_alt_team)
        {
            // Simply reverse the selection. At least one team in theory should have a valid mount ID to choose.
            mount_entry = team == ALLIANCE ? node->MountCreatureID[0] : node->MountCreatureID[1];
        }

        CreatureTemplate const* mount_info = GetCreatureTemplate(mount_entry);
        if (mount_info)
        {
            mount_id = mount_info->GetRandomValidModelId();
            if (!mount_id)
            {
                TC_LOG_ERROR("sql.sql", "No displayid found for the taxi mount with the entry %u! Can't load it!", mount_entry);
                return false;
            }
        }
    }

    // minfo is not actually used but the mount_id was updated
    GetCreatureModelRandomGender(&mount_id);

    return mount_id;
}

void ObjectMgr::LoadGraveyardZones()
{
    uint32 oldMSTime = getMSTime();

    GraveYardStore.clear();                                  // need for reload case

    //                                                0       1         2
    QueryResult result = WorldDatabase.Query("SELECT id, ghost_zone, faction FROM game_graveyard_zone");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 graveyard-zone links. DB table `game_graveyard_zone` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        ++count;

        Field* fields = result->Fetch();

        uint32 safeLocId = fields[0].GetUInt32();
        uint32 zoneId = fields[1].GetUInt32();
        uint32 team   = fields[2].GetUInt16();

        WorldSafeLocsEntry const* entry = sWorldSafeLocsStore.LookupEntry(safeLocId);
        if (!entry)
        {
            TC_LOG_ERROR("sql.sql", "Table `game_graveyard_zone` has a record for not existing graveyard (WorldSafeLocs.dbc id) %u, skipped.", safeLocId);
            continue;
        }

        AreaTableEntry const* areaEntry = GetAreaEntryByAreaID(zoneId);
        if (!areaEntry)
        {
            TC_LOG_ERROR("sql.sql", "Table `game_graveyard_zone` has a record for not existing zone id (%u), skipped.", zoneId);
            continue;
        }

        if (areaEntry->zone != 0 && zoneId != 33 && zoneId != 5144 && zoneId != 5287 && zoneId != 6170 && zoneId != 6176 && zoneId != 6450 && zoneId != 6451
                             && zoneId != 6452 && zoneId != 6453 && zoneId != 6454 && zoneId != 6455 && zoneId != 6456 && zoneId != 4755)
        {
            TC_LOG_ERROR("sql.sql", "Table `game_graveyard_zone` has a record for subzone id (%u) instead of zone, skipped.", zoneId);
            continue;
        }

        if (team != 0 && team != HORDE && team != ALLIANCE)
        {
            TC_LOG_ERROR("sql.sql", "Table `game_graveyard_zone` has a record for non player faction (%u), skipped.", team);
            continue;
        }

        if (!AddGraveYardLink(safeLocId, zoneId, team, false))
            TC_LOG_ERROR("sql.sql", "Table `game_graveyard_zone` has a duplicate record for Graveyard (ID: %u) and Zone (ID: %u), skipped.", safeLocId, zoneId);
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u graveyard-zone links in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

WorldSafeLocsEntry const* ObjectMgr::GetDefaultGraveYard(uint32 team)
{
    enum DefaultGraveyard
    {
        HORDE_GRAVEYARD = 10, // Crossroads
        ALLIANCE_GRAVEYARD = 4, // Westfall
    };

    if (team == HORDE)
        return sWorldSafeLocsStore.LookupEntry(HORDE_GRAVEYARD);
    else if (team == ALLIANCE)
        return sWorldSafeLocsStore.LookupEntry(ALLIANCE_GRAVEYARD);
    else return NULL;
}

WorldSafeLocsEntry const* ObjectMgr::GetClosestGraveYard(float x, float y, float z, uint32 MapId, uint32 team)
{
    // search for zone associated closest graveyard
    uint32 zoneId = sMapMgr->GetZoneId(MapId, x, y, z);

    if (!zoneId)
    {
        if (z > -500)
        {
            TC_LOG_ERROR("misc", "ZoneId not found for map %u coords (%f, %f, %f)", MapId, x, y, z);
            return GetDefaultGraveYard(team);
        }
    }

    // Simulate std. algorithm:
    //   found some graveyard associated to (ghost_zone, ghost_map)
    //
    //   if mapId == graveyard.mapId (ghost in plain zone or city or battleground) and search graveyard at same map
    //     then check faction
    //   if mapId != graveyard.mapId (ghost in instance) and search any graveyard associated
    //     then check faction
    GraveYardContainer::const_iterator graveLow  = GraveYardStore.lower_bound(zoneId);
    GraveYardContainer::const_iterator graveUp   = GraveYardStore.upper_bound(zoneId);
    MapEntry const* map = sMapStore.LookupEntry(MapId);
    // not need to check validity of map object; MapId _MUST_ be valid here

    if (graveLow == graveUp && !map->IsBattleArena())
    {
        TC_LOG_ERROR("sql.sql", "Table `game_graveyard_zone` incomplete: Zone %u Team %u does not have a linked graveyard.", zoneId, team);
        return GetDefaultGraveYard(team);
    }

    // at corpse map
    bool foundNear = false;
    float distNear = 10000;
    WorldSafeLocsEntry const* entryNear = NULL;

    // at entrance map for corpse map
    bool foundEntr = false;
    float distEntr = 10000;
    WorldSafeLocsEntry const* entryEntr = NULL;

    // some where other
    WorldSafeLocsEntry const* entryFar = NULL;

    MapEntry const* mapEntry = sMapStore.LookupEntry(MapId);

    for (GraveYardContainer::const_iterator itr = graveLow; itr != graveUp; ++itr)
    {
        GraveYardData const& data = itr->second;

        WorldSafeLocsEntry const* entry = sWorldSafeLocsStore.LookupEntry(data.safeLocId);
        if (!entry)
        {
            TC_LOG_ERROR("sql.sql", "Table `game_graveyard_zone` has record for not existing graveyard (WorldSafeLocs.dbc id) %u, skipped.", data.safeLocId);
            continue;
        }

        // skip enemy faction graveyard
        // team == 0 case can be at call from .neargrave
        if (data.team != 0 && team != 0 && data.team != team)
            continue;

        // find now nearest graveyard at other map
        if (MapId != entry->map_id)
        {
            // if find graveyard at different map from where entrance placed (or no entrance data), use any first
            if (!mapEntry
                || mapEntry->entrance_map < 0
                || uint32(mapEntry->entrance_map) != entry->map_id
                || (mapEntry->entrance_x == 0 && mapEntry->entrance_y == 0))
            {
                // not have any corrdinates for check distance anyway
                entryFar = entry;
                continue;
            }

            // at entrance map calculate distance (2D);
            float dist2 = (entry->x - mapEntry->entrance_x)*(entry->x - mapEntry->entrance_x)
                +(entry->y - mapEntry->entrance_y)*(entry->y - mapEntry->entrance_y);
            if (foundEntr)
            {
                if (dist2 < distEntr)
                {
                    distEntr = dist2;
                    entryEntr = entry;
                }
            }
            else
            {
                foundEntr = true;
                distEntr = dist2;
                entryEntr = entry;
            }
        }
        // find now nearest graveyard at same map
        else
        {
            float dist2 = (entry->x - x)*(entry->x - x)+(entry->y - y)*(entry->y - y)+(entry->z - z)*(entry->z - z);
            if (foundNear)
            {
                if (dist2 < distNear)
                {
                    distNear = dist2;
                    entryNear = entry;
                }
            }
            else
            {
                foundNear = true;
                distNear = dist2;
                entryNear = entry;
            }
        }
    }

    if (entryNear)
        return entryNear;

    if (entryEntr)
        return entryEntr;

    return entryFar;
}

GraveYardData const* ObjectMgr::FindGraveYardData(uint32 id, uint32 zoneId)
{
    GraveYardContainer::const_iterator graveLow  = GraveYardStore.lower_bound(zoneId);
    GraveYardContainer::const_iterator graveUp   = GraveYardStore.upper_bound(zoneId);

    for (GraveYardContainer::const_iterator itr = graveLow; itr != graveUp; ++itr)
    {
        if (itr->second.safeLocId == id)
            return &itr->second;
    }

    return NULL;
}

bool ObjectMgr::AddGraveYardLink(uint32 id, uint32 zoneId, uint32 team, bool persist /*= true*/)
{
    if (FindGraveYardData(id, zoneId))
        return false;

    // add link to loaded data
    GraveYardData data;
    data.safeLocId = id;
    data.team = team;

    GraveYardStore.insert(GraveYardContainer::value_type(zoneId, data));

    // add link to DB
    if (persist)
    {
        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_INS_GRAVEYARD_ZONE);

        stmt->setUInt32(0, id);
        stmt->setUInt32(1, zoneId);
        stmt->setUInt16(2, uint16(team));

        WorldDatabase.Execute(stmt);
    }

    return true;
}

void ObjectMgr::RemoveGraveYardLink(uint32 id, uint32 zoneId, uint32 team, bool persist /*= false*/)
{
    GraveYardContainer::iterator graveLow  = GraveYardStore.lower_bound(zoneId);
    GraveYardContainer::iterator graveUp   = GraveYardStore.upper_bound(zoneId);
    if (graveLow == graveUp)
    {
        //TC_LOG_ERROR("sql.sql", "Table `game_graveyard_zone` incomplete: Zone %u Team %u does not have a linked graveyard.", zoneId, team);
        return;
    }

    bool found = false;

    GraveYardContainer::iterator itr;

    for (itr = graveLow; itr != graveUp; ++itr)
    {
        GraveYardData & data = itr->second;

        // skip not matching safezone id
        if (data.safeLocId != id)
            continue;

        // skip enemy faction graveyard at same map (normal area, city, or battleground)
        // team == 0 case can be at call from .neargrave
        if (data.team != 0 && team != 0 && data.team != team)
            continue;

        found = true;
        break;
    }

    // no match, return
    if (!found)
        return;

    // remove from links
    GraveYardStore.erase(itr);

    // remove link from DB
    if (persist)
    {
        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_DEL_GRAVEYARD_ZONE);

        stmt->setUInt32(0, id);
        stmt->setUInt32(1, zoneId);
        stmt->setUInt16(2, uint16(team));

        WorldDatabase.Execute(stmt);
    }
}

void ObjectMgr::LoadAreaTriggerTeleports()
{
    uint32 oldMSTime = getMSTime();

    _areaTriggerStore.clear();                                  // need for reload case

    //                                                        0            1                  2                  3                  4                   5
    QueryResult result = WorldDatabase.Query("SELECT id,  target_map, target_position_x, target_position_y, target_position_z, target_orientation FROM areatrigger_teleport");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 area trigger teleport definitions. DB table `areatrigger_teleport` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        ++count;

        uint32 Trigger_ID = fields[0].GetUInt32();

        AreaTriggerStruct at;

        at.target_mapId             = fields[1].GetUInt16();
        at.target_X                 = fields[2].GetFloat();
        at.target_Y                 = fields[3].GetFloat();
        at.target_Z                 = fields[4].GetFloat();
        at.target_Orientation       = fields[5].GetFloat();

        AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(Trigger_ID);
        if (!atEntry)
        {
            TC_LOG_ERROR("sql.sql", "Area trigger (ID:%u) does not exist in `AreaTrigger.dbc`.", Trigger_ID);
            continue;
        }

        MapEntry const* mapEntry = sMapStore.LookupEntry(at.target_mapId);
        if (!mapEntry)
        {
            TC_LOG_ERROR("sql.sql", "Area trigger (ID:%u) target map (ID: %u) does not exist in `Map.dbc`.", Trigger_ID, at.target_mapId);
            continue;
        }

        if (at.target_X == 0 && at.target_Y == 0 && at.target_Z == 0)
        {
            TC_LOG_ERROR("sql.sql", "Area trigger (ID:%u) target coordinates not provided.", Trigger_ID);
            continue;
        }

        _areaTriggerStore[Trigger_ID] = at;

    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u area trigger teleport definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadAccessRequirements()
{
    uint32 oldMSTime = getMSTime();

    _accessRequirementStore.clear();                                  // need for reload case

    //                                               0      1           2          3          4     5      6             7             8                      9              10             11
    QueryResult result = WorldDatabase.Query("SELECT mapid, difficulty, level_min, level_max, item, item2, quest_done_A, quest_done_H, completed_achievement, itemlevel_min, itemlevel_max, quest_failed_text FROM access_requirement");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 access requirement definitions. DB table `access_requirement` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        ++count;

        uint32 mapid = fields[0].GetUInt32();
        uint8 difficulty = fields[1].GetUInt8();
        uint32 requirement_ID = MAKE_PAIR32(mapid, difficulty);

        AccessRequirement ar;

        ar.levelMin                 = fields[2].GetUInt8();
        ar.levelMax                 = fields[3].GetUInt8();
        ar.item                     = fields[4].GetUInt32();
        ar.item2                    = fields[5].GetUInt32();
        ar.quest_A                  = fields[6].GetUInt32();
        ar.quest_H                  = fields[7].GetUInt32();
        ar.achievement              = fields[8].GetUInt32();
        ar.itemlevelMin             = fields[9].GetUInt32();
        ar.itemlevelMax             = fields[10].GetUInt32();
        ar.questFailedText          = fields[11].GetString();

        if (ar.item)
        {
            ItemTemplate const* pProto = GetItemTemplate(ar.item);
            if (!pProto)
            {
                TC_LOG_ERROR("misc", "Key item %u does not exist for map %u difficulty %u, removing key requirement.", ar.item, mapid, difficulty);
                ar.item = 0;
            }
        }

        if (ar.item2)
        {
            ItemTemplate const* pProto = GetItemTemplate(ar.item2);
            if (!pProto)
            {
                TC_LOG_ERROR("misc", "Second item %u does not exist for map %u difficulty %u, removing key requirement.", ar.item2, mapid, difficulty);
                ar.item2 = 0;
            }
        }

        if (ar.quest_A)
        {
            if (!GetQuestTemplate(ar.quest_A))
            {
                TC_LOG_ERROR("sql.sql", "Required Alliance Quest %u not exist for map %u difficulty %u, remove quest done requirement.", ar.quest_A, mapid, difficulty);
                ar.quest_A = 0;
            }
        }

        if (ar.quest_H)
        {
            if (!GetQuestTemplate(ar.quest_H))
            {
                TC_LOG_ERROR("sql.sql", "Required Horde Quest %u not exist for map %u difficulty %u, remove quest done requirement.", ar.quest_H, mapid, difficulty);
                ar.quest_H = 0;
            }
        }

        if (ar.achievement)
        {
            if (!sAchievementStore.LookupEntry(ar.achievement))
            {
                TC_LOG_ERROR("sql.sql", "Required Achievement %u not exist for map %u difficulty %u, remove quest done requirement.", ar.achievement, mapid, difficulty);
                ar.achievement = 0;
            }
        }

        _accessRequirementStore[requirement_ID] = ar;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u access requirement definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

/*
 * Searches for the areatrigger which teleports players out of the given map with instance_template.parent field support
 */
AreaTriggerStruct const* ObjectMgr::GetGoBackTrigger(uint32 Map) const
{
    bool useParentDbValue = false;
    uint32 parentId = 0;
    const MapEntry* mapEntry = sMapStore.LookupEntry(Map);
    if (!mapEntry || mapEntry->entrance_map < 0)
        return NULL;

    if (mapEntry->IsDungeon())
    {
        const InstanceTemplate* iTemplate = sObjectMgr->GetInstanceTemplate(Map);

        if (!iTemplate)
            return NULL;

        parentId = iTemplate->Parent;
        useParentDbValue = true;
    }

    uint32 entrance_map = uint32(mapEntry->entrance_map);
    for (AreaTriggerContainer::const_iterator itr = _areaTriggerStore.begin(); itr != _areaTriggerStore.end(); ++itr)
        if ((!useParentDbValue && itr->second.target_mapId == entrance_map) || (useParentDbValue && itr->second.target_mapId == parentId))
        {
            AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(itr->first);
            if (atEntry && atEntry->mapid == Map)
                return &itr->second;
        }
    return NULL;
}

/**
 * Searches for the areatrigger which teleports players to the given map
 */
AreaTriggerStruct const* ObjectMgr::GetMapEntranceTrigger(uint32 Map) const
{
    for (AreaTriggerContainer::const_iterator itr = _areaTriggerStore.begin(); itr != _areaTriggerStore.end(); ++itr)
    {
        if (itr->second.target_mapId == Map)
        {
            AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(itr->first);
            if (atEntry)
                return &itr->second;
        }
    }
    return NULL;
}

void ObjectMgr::SetHighestGuids()
{
    QueryResult result = CharacterDatabase.Query("SELECT MAX(guid) FROM characters");
    if (result)
        _hiCharGuid = (*result)[0].GetUInt32()+1;

    result = WorldDatabase.Query("SELECT MAX(guid) FROM creature");
    if (result)
        _hiCreatureGuid = (*result)[0].GetUInt32()+1;

    result = CharacterDatabase.Query("SELECT MAX(guid) FROM item_instance");
    if (result)
        _hiItemGuid = (*result)[0].GetUInt32()+1;

    result = WorldDatabase.Query("SELECT MAX(guid) FROM gameobject");
    if (result)
        _hiGoGuid = (*result)[0].GetUInt32()+1;

    result = WorldDatabase.Query("SELECT MAX(guid) FROM transports");
    if (result)
        _hiMoTransGuid = (*result)[0].GetUInt32()+1;

    result = CharacterDatabase.Query("SELECT MAX(id) FROM auctionhouse");
    if (result)
        _auctionId = (*result)[0].GetUInt32()+1;

    result = CharacterDatabase.Query("SELECT MAX(id) FROM mail");
    if (result)
        _mailId = (*result)[0].GetUInt32()+1;

    result = CharacterDatabase.Query("SELECT MAX(corpseGuid) FROM corpse");
    if (result)
        _hiCorpseGuid = (*result)[0].GetUInt32()+1;

    result = CharacterDatabase.Query("SELECT MAX(setguid) FROM character_equipmentsets");
    if (result)
        _equipmentSetGuid = (*result)[0].GetUInt64()+1;

    result = CharacterDatabase.Query("SELECT MAX(guildId) FROM guild");
    if (result)
        sGuildMgr->SetNextGuildId((*result)[0].GetUInt32()+1);

    result = CharacterDatabase.Query("SELECT MAX(guid) FROM groups");
    if (result)
        sGroupMgr->SetNextGroupId((*result)[0].GetUInt32()+1);

    result = CharacterDatabase.Query("SELECT MAX(itemId) from character_void_storage");
    if (result)
        _voidItemId = (*result)[0].GetUInt64()+1;

    QueryResult authResult = LoginDatabase.Query("SELECT MAX(id) FROM account_battle_pet");
    if (result)
        _battlePetId = (*authResult)[0].GetUInt64() + 1;
}

uint32 ObjectMgr::GenerateAuctionID()
{
    if (_auctionId >= 0xFFFFFFFE)
    {
        TC_LOG_ERROR("misc", "Auctions ids overflow!! Can't continue, shutting down server. ");
        sWorld->StopNow(ERROR_EXIT_CODE);
    }
    return _auctionId++;
}

uint64 ObjectMgr::GenerateEquipmentSetGuid()
{
    if (_equipmentSetGuid >= uint64(0xFFFFFFFFFFFFFFFELL))
    {
        TC_LOG_ERROR("misc", "EquipmentSet guid overflow!! Can't continue, shutting down server. ");
        sWorld->StopNow(ERROR_EXIT_CODE);
    }
    return _equipmentSetGuid++;
}

uint32 ObjectMgr::GenerateMailID()
{
    if (_mailId >= 0xFFFFFFFE)
    {
        TC_LOG_ERROR("misc", "Mail ids overflow!! Can't continue, shutting down server. ");
        sWorld->StopNow(ERROR_EXIT_CODE);
    }
    return _mailId++;
}

uint32 ObjectMgr::GenerateLowGuid(HighGuid guidhigh)
{
    switch (guidhigh)
    {
        case HIGHGUID_ITEM:
        {
            ASSERT(_hiItemGuid < 0xFFFFFFFE && "Item guid overflow!");
            return _hiItemGuid++;
        }
        case HIGHGUID_UNIT:
        {
            ASSERT(_hiCreatureGuid < 0x00FFFFFE && "Creature guid overflow!");
            return _hiCreatureGuid++;
        }
        case HIGHGUID_PET:
        {
            ASSERT(_hiPetGuid < 0x00FFFFFE && "Pet guid overflow!");
            return _hiPetGuid++;
        }
        case HIGHGUID_VEHICLE:
        {
            ASSERT(_hiVehicleGuid < 0x00FFFFFF && "Vehicle guid overflow!");
            return _hiVehicleGuid++;
        }
        case HIGHGUID_PLAYER:
        {
            ASSERT(_hiCharGuid < 0xFFFFFFFE && "Player guid overflow!");
            return _hiCharGuid++;
        }
        case HIGHGUID_GAMEOBJECT:
        {
            ASSERT(_hiGoGuid < 0x00FFFFFE && "Gameobject guid overflow!");
            return _hiGoGuid++;
        }
        case HIGHGUID_CORPSE:
        {
            ASSERT(_hiCorpseGuid < 0xFFFFFFFE && "Corpse guid overflow!");
            return _hiCorpseGuid++;
        }
        case HIGHGUID_DYNAMICOBJECT:
        {
            ASSERT(_hiDoGuid < 0xFFFFFFFE && "DynamicObject guid overflow!");
            return _hiDoGuid++;
        }
        case HIGHGUID_AREATRIGGER:
        {
            ASSERT(_hiAreaTriggerGuid < 0xFFFFFFFE && "AreaTrigger guid overflow!");
            return _hiAreaTriggerGuid++;
        }
        case HIGHGUID_MO_TRANSPORT:
        {
            ASSERT(_hiMoTransGuid < 0xFFFFFFFE && "MO Transport guid overflow!");
            return _hiMoTransGuid++;
        }
        default:
            ASSERT(false && "ObjectMgr::GenerateLowGuid - Unknown HIGHGUID type");
            return 0;
    }
}

void ObjectMgr::LoadGameObjectLocales()
{
    uint32 oldMSTime = getMSTime();

    _gameObjectLocaleStore.clear();                           // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, "
        "name_loc1, name_loc2, name_loc3, name_loc4, name_loc5, name_loc6, name_loc7, name_loc8, name_loc9, name_loc10, "
        "castbarcaption_loc1, castbarcaption_loc2, castbarcaption_loc3, castbarcaption_loc4, "
        "castbarcaption_loc5, castbarcaption_loc6, castbarcaption_loc7, castbarcaption_loc8, castbarcaption_loc9, castbarcaption_loc10 FROM locales_gameobject");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        GameObjectLocale& data = _gameObjectLocaleStore[entry];

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
            AddLocaleString(fields[i].GetString(), LocaleConstant(i), data.Name);

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
            AddLocaleString(fields[i + (TOTAL_LOCALES - 1)].GetString(), LocaleConstant(i), data.CastBarCaption);
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %lu gameobject locale strings in %u ms", (unsigned long)_gameObjectLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

inline void CheckGOLockId(GameObjectTemplate const* goInfo, uint32 dataN, uint32 N)
{
    if (sLockStore.LookupEntry(dataN))
        return;

    TC_LOG_ERROR("sql.sql", "Gameobject (Entry: %u GoType: %u) have data%d=%u but lock (Id: %u) not found.",
        goInfo->entry, goInfo->type, N, goInfo->door.lockId, goInfo->door.lockId);
}

inline void CheckGOLinkedTrapId(GameObjectTemplate const* goInfo, uint32 dataN, uint32 N)
{
    if (GameObjectTemplate const* trapInfo = sObjectMgr->GetGameObjectTemplate(dataN))
    {
        if (trapInfo->type != GAMEOBJECT_TYPE_TRAP)
            TC_LOG_ERROR("sql.sql", "Gameobject (Entry: %u GoType: %u) have data%d=%u but GO (Entry %u) have not GAMEOBJECT_TYPE_TRAP (%u) type.",
            goInfo->entry, goInfo->type, N, dataN, dataN, GAMEOBJECT_TYPE_TRAP);
    }
}

inline void CheckGOSpellId(GameObjectTemplate const* goInfo, uint32 dataN, uint32 N)
{
    if (sSpellMgr->GetSpellInfo(dataN))
        return;

    TC_LOG_ERROR("sql.sql", "Gameobject (Entry: %u GoType: %u) have data%d=%u but Spell (Entry %u) not exist.",
        goInfo->entry, goInfo->type, N, dataN, dataN);
}

inline void CheckAndFixGOChairHeightId(GameObjectTemplate const* goInfo, uint32 const& dataN, uint32 N)
{
    if (dataN <= (UNIT_STAND_STATE_SIT_HIGH_CHAIR-UNIT_STAND_STATE_SIT_LOW_CHAIR))
        return;

    TC_LOG_ERROR("sql.sql", "Gameobject (Entry: %u GoType: %u) have data%d=%u but correct chair height in range 0..%i.",
        goInfo->entry, goInfo->type, N, dataN, UNIT_STAND_STATE_SIT_HIGH_CHAIR-UNIT_STAND_STATE_SIT_LOW_CHAIR);

    // prevent client and server unexpected work
    const_cast<uint32&>(dataN) = 0;
}

inline void CheckGONoDamageImmuneId(GameObjectTemplate* goTemplate, uint32 dataN, uint32 N)
{
    // 0/1 correct values
    if (dataN <= 1)
        return;

    TC_LOG_ERROR("sql.sql", "Gameobject (Entry: %u GoType: %u) have data%d=%u but expected boolean (0/1) noDamageImmune field value.", goTemplate->entry, goTemplate->type, N, dataN);
}

inline void CheckGOConsumable(GameObjectTemplate const* goInfo, uint32 dataN, uint32 N)
{
    // 0/1 correct values
    if (dataN <= 1)
        return;

    TC_LOG_ERROR("sql.sql", "Gameobject (Entry: %u GoType: %u) have data%d=%u but expected boolean (0/1) consumable field value.",
        goInfo->entry, goInfo->type, N, dataN);
}

void ObjectMgr::LoadGameObjectTemplate()
{
    uint32 oldMSTime = getMSTime();

    //                                                 0      1      2        3       4             5          6      7       8     9        10         11          12
    QueryResult result = WorldDatabase.Query("SELECT entry, type, displayId, name, IconName, castBarCaption, unk1, faction, flags, size, questItem1, questItem2, questItem3, "
    //                                            13          14          15       16     17     18     19     20     21     22     23     24     25      26      27      28
                                             "questItem4, questItem5, questItem6, data0, data1, data2, data3, data4, data5, data6, data7, data8, data9, data10, data11, data12, "
    //                                          29      30      31      32      33      34      35      36      37      38      39      40      41      42      43      44
                                             "data13, data14, data15, data16, data17, data18, data19, data20, data21, data22, data23, data24, data25, data26, data27, data28, "
    //                                          45      46      47       48       49        50          51          52
                                             "data29, data30, data31, unkInt32, AIName, ScriptName, currencyId, currencyCnt "
                                             "FROM gameobject_template");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 gameobject definitions. DB table `gameobject_template` is empty.");
        return;
    }

    //_gameObjectTemplateStore.rehash(result->GetRowCount());
    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        GameObjectTemplate& got = _gameObjectTemplateStore[entry];

        got.entry          = entry;
        got.type           = uint32(fields[1].GetUInt8());
        got.displayId      = fields[2].GetUInt32();
        got.name           = fields[3].GetString();
        got.IconName       = fields[4].GetString();
        got.castBarCaption = fields[5].GetString();
        got.unk1           = fields[6].GetString();
        got.faction        = uint32(fields[7].GetUInt16());
        got.flags          = fields[8].GetUInt32();
        got.size           = fields[9].GetFloat();

        for (uint8 i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS; ++i)
            got.questItems[i] = fields[10 + i].GetUInt32();

        for (uint8 i = 0; i < MAX_GAMEOBJECT_DATA; ++i)
            got.raw.data[i] = fields[16 + i].GetUInt32();

        got.unkInt32 = fields[48].GetInt32();
        got.AIName = fields[49].GetString();
        got.ScriptId = GetScriptId(fields[50].GetCString());

        got.currencyId     = fields[51].GetUInt32();
        got.currencyCnt    = fields[52].GetUInt32();

        // Checks

        switch (got.type)
        {
            case GAMEOBJECT_TYPE_DOOR:                      //0
            {
                if (got.door.lockId)
                    CheckGOLockId(&got, got.door.lockId, 1);
                CheckGONoDamageImmuneId(&got, got.door.noDamageImmune, 3);
                break;
            }
            case GAMEOBJECT_TYPE_BUTTON:                    //1
            {
                if (got.button.lockId)
                    CheckGOLockId(&got, got.button.lockId, 1);
                CheckGONoDamageImmuneId(&got, got.button.noDamageImmune, 4);
                break;
            }
            case GAMEOBJECT_TYPE_QUESTGIVER:                //2
            {
                if (got.questgiver.lockId)
                    CheckGOLockId(&got, got.questgiver.lockId, 0);
                CheckGONoDamageImmuneId(&got, got.questgiver.noDamageImmune, 5);
                break;
            }
            case GAMEOBJECT_TYPE_CHEST:                     //3
            {
                if (got.chest.lockId)
                    CheckGOLockId(&got, got.chest.lockId, 0);

                CheckGOConsumable(&got, got.chest.consumable, 3);

                if (got.chest.linkedTrapId)              // linked trap
                    CheckGOLinkedTrapId(&got, got.chest.linkedTrapId, 7);
                break;
            }
            case GAMEOBJECT_TYPE_TRAP:                      //6
            {
                if (got.trap.lockId)
                    CheckGOLockId(&got, got.trap.lockId, 0);
                break;
            }
            case GAMEOBJECT_TYPE_CHAIR:                     //7
                CheckAndFixGOChairHeightId(&got, got.chair.height, 1);
                break;
            case GAMEOBJECT_TYPE_SPELL_FOCUS:               //8
            {
                if (got.spellFocus.focusId)
                {
                    if (!sSpellFocusObjectStore.LookupEntry(got.spellFocus.focusId))
                        TC_LOG_ERROR("sql.sql", "GameObject (Entry: %u GoType: %u) have data0=%u but SpellFocus (Id: %u) not exist.",
                        entry, got.type, got.spellFocus.focusId, got.spellFocus.focusId);
                }

                if (got.spellFocus.linkedTrapId)        // linked trap
                    CheckGOLinkedTrapId(&got, got.spellFocus.linkedTrapId, 2);
                break;
            }
            case GAMEOBJECT_TYPE_GOOBER:                    //10
            {
                if (got.goober.lockId)
                    CheckGOLockId(&got, got.goober.lockId, 0);

                CheckGOConsumable(&got, got.goober.consumable, 3);

                if (got.goober.pageId)                  // pageId
                {
                    if (!GetPageText(got.goober.pageId))
                        TC_LOG_ERROR("sql.sql", "GameObject (Entry: %u GoType: %u) have data7=%u but PageText (Entry %u) not exist.",
                        entry, got.type, got.goober.pageId, got.goober.pageId);
                }
                CheckGONoDamageImmuneId(&got, got.goober.noDamageImmune, 11);
                if (got.goober.linkedTrapId)            // linked trap
                    CheckGOLinkedTrapId(&got, got.goober.linkedTrapId, 12);
                break;
            }
            case GAMEOBJECT_TYPE_AREADAMAGE:                //12
            {
                if (got.areadamage.lockId)
                    CheckGOLockId(&got, got.areadamage.lockId, 0);
                break;
            }
            case GAMEOBJECT_TYPE_CAMERA:                    //13
            {
                if (got.camera.lockId)
                    CheckGOLockId(&got, got.camera.lockId, 0);
                break;
            }
            case GAMEOBJECT_TYPE_MO_TRANSPORT:              //15
            {
                if (got.moTransport.taxiPathId)
                {
                    if (got.moTransport.taxiPathId >= sTaxiPathNodesByPath.size() || sTaxiPathNodesByPath[got.moTransport.taxiPathId].empty())
                        TC_LOG_ERROR("sql.sql", "GameObject (Entry: %u GoType: %u) have data0=%u but TaxiPath (Id: %u) not exist.",
                        entry, got.type, got.moTransport.taxiPathId, got.moTransport.taxiPathId);
                }
                break;
            }
            case GAMEOBJECT_TYPE_SUMMONING_RITUAL:          //18
                break;
            case GAMEOBJECT_TYPE_SPELLCASTER:               //22
            {
                // always must have spell
                CheckGOSpellId(&got, got.spellcaster.spellId, 0);
                break;
            }
            case GAMEOBJECT_TYPE_FLAGSTAND:                 //24
            {
                if (got.flagstand.lockId)
                    CheckGOLockId(&got, got.flagstand.lockId, 0);
                CheckGONoDamageImmuneId(&got, got.flagstand.noDamageImmune, 5);
                break;
            }
            case GAMEOBJECT_TYPE_FISHINGHOLE:               //25
            {
                if (got.fishinghole.lockId)
                    CheckGOLockId(&got, got.fishinghole.lockId, 4);
                break;
            }
            case GAMEOBJECT_TYPE_FLAGDROP:                  //26
            {
                if (got.flagdrop.lockId)
                    CheckGOLockId(&got, got.flagdrop.lockId, 0);
                CheckGONoDamageImmuneId(&got, got.flagdrop.noDamageImmune, 3);
                break;
            }
            case GAMEOBJECT_TYPE_BARBER_CHAIR:              //32
                CheckAndFixGOChairHeightId(&got, got.barberChair.chairheight, 0);
                break;
        }

       ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u game object templates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadExplorationBaseXP()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT level, basexp FROM exploration_basexp");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 BaseXP definitions. DB table `exploration_basexp` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        uint8 level  = fields[0].GetUInt8();
        uint32 basexp = fields[1].GetInt32();
        _baseXPTable[level] = basexp;
        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u BaseXP definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

uint32 ObjectMgr::GetBaseXP(uint8 level)
{
    return _baseXPTable[level] ? _baseXPTable[level] : 0;
}

uint32 ObjectMgr::GetXPForLevel(uint8 level) const
{
    if (level < _playerXPperLevel.size())
        return _playerXPperLevel[level];
    return 0;
}

void ObjectMgr::LoadPetNames()
{
    uint32 oldMSTime = getMSTime();
    //                                                0     1      2
    QueryResult result = WorldDatabase.Query("SELECT word, entry, half FROM pet_name_generation");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 pet name parts. DB table `pet_name_generation` is empty!");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        std::string word = fields[0].GetString();
        uint32 entry     = fields[1].GetUInt32();
        bool   half      = fields[2].GetBool();
        if (half)
            _petHalfName1[entry].push_back(word);
        else
            _petHalfName0[entry].push_back(word);
        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u pet name parts in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadPetNumber()
{
    uint32 oldMSTime = getMSTime();

    PreparedQueryResult result = CharacterDatabase.Query(CharacterDatabase.GetPreparedStatement(CHAR_SEL_NEXT_PET_NUMBER));
    if (result)
    {
        Field* fields = result->Fetch();

        if (!fields[0].IsNull())
            _hiPetNumber = fields[0].GetUInt32();
    }

    TC_LOG_INFO("server.loading", ">> Loaded next pet number: %d in %u ms", _hiPetNumber, GetMSTimeDiffToNow(oldMSTime));
}

std::string ObjectMgr::GeneratePetName(uint32 entry)
{
    StringVector & list0 = _petHalfName0[entry];
    StringVector & list1 = _petHalfName1[entry];

    if (list0.empty() || list1.empty())
    {
        CreatureTemplate const* cinfo = GetCreatureTemplate(entry);
        const char* petname = GetPetName(cinfo->family, sWorld->GetDefaultDbcLocale());
        if (!petname)
            return cinfo->Name;

        return std::string(petname);
    }

    return *(list0.begin()+urand(0, list0.size()-1)) + *(list1.begin()+urand(0, list1.size()-1));
}

uint32 ObjectMgr::GeneratePetNumber()
{
    return ++_hiPetNumber;
}

uint64 ObjectMgr::GenerateVoidStorageItemId()
{
    return ++_voidItemId;
}

uint64 ObjectMgr::GenerateBattlePetId()
{
    return _battlePetId++;
}

void ObjectMgr::LoadCorpses()
{
    //        0     1     2     3            4      5          6          7       8       9      10        11    12          13          14          15         16
    // SELECT posX, posY, posZ, orientation, mapId, displayId, itemCache, bytes1, bytes2, flags, dynFlags, time, corpseType, instanceId, phaseMask, corpseGuid, guid FROM corpse WHERE corpseType <> 0

    uint32 oldMSTime = getMSTime();

    PreparedQueryResult result = CharacterDatabase.Query(CharacterDatabase.GetPreparedStatement(CHAR_SEL_CORPSES));
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 corpses. DB table `corpse` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        uint32 guid = fields[16].GetUInt32();
        CorpseType type = CorpseType(fields[12].GetUInt8());
        if (type >= MAX_CORPSE_TYPE)
        {
            TC_LOG_ERROR("misc", "Corpse (guid: %u) have wrong corpse type (%u), not loading.", guid, type);
            continue;
        }

        Corpse* corpse = new Corpse(type);
        if (!corpse->LoadCorpseFromDB(guid, fields))
        {
            delete corpse;
            continue;
        }

        sObjectAccessor->AddCorpse(corpse);
        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u corpses in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadReputationRewardRate()
{
    uint32 oldMSTime = getMSTime();

    _repRewardRateStore.clear();                             // for reload case

    uint32 count = 0; //                                0          1            2             3
    QueryResult result = WorldDatabase.Query("SELECT faction, quest_rate, creature_rate, spell_rate FROM reputation_reward_rate");

    if (!result)
    {
        TC_LOG_ERROR("sql.sql", ">> Loaded `reputation_reward_rate`, table is empty!");

        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 factionId        = fields[0].GetUInt32();

        RepRewardRate repRate;

        repRate.quest_rate      = fields[1].GetFloat();
        repRate.creature_rate   = fields[2].GetFloat();
        repRate.spell_rate      = fields[3].GetFloat();

        FactionEntry const* factionEntry = sFactionStore.LookupEntry(factionId);
        if (!factionEntry)
        {
            TC_LOG_ERROR("sql.sql", "Faction (faction.dbc) %u does not exist but is used in `reputation_reward_rate`", factionId);
            continue;
        }

        if (repRate.quest_rate < 0.0f)
        {
            TC_LOG_ERROR("sql.sql", "Table reputation_reward_rate has quest_rate with invalid rate %f, skipping data for faction %u", repRate.quest_rate, factionId);
            continue;
        }

        if (repRate.creature_rate < 0.0f)
        {
            TC_LOG_ERROR("sql.sql", "Table reputation_reward_rate has creature_rate with invalid rate %f, skipping data for faction %u", repRate.creature_rate, factionId);
            continue;
        }

        if (repRate.spell_rate < 0.0f)
        {
            TC_LOG_ERROR("sql.sql", "Table reputation_reward_rate has spell_rate with invalid rate %f, skipping data for faction %u", repRate.spell_rate, factionId);
            continue;
        }

        _repRewardRateStore[factionId] = repRate;

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u reputation_reward_rate in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadReputationOnKill()
{
    uint32 oldMSTime = getMSTime();

    // For reload case
    _repOnKillStore.clear();

    uint32 count = 0;

    //                                                0            1                     2
    QueryResult result = WorldDatabase.Query("SELECT creature_id, RewOnKillRepFaction1, RewOnKillRepFaction2, "
    //   3             4             5                   6             7             8                   9
        "IsTeamAward1, MaxStanding1, RewOnKillRepValue1, IsTeamAward2, MaxStanding2, RewOnKillRepValue2, TeamDependent "
        "FROM creature_onkill_reputation");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 creature award reputation definitions. DB table `creature_onkill_reputation` is empty.");

        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 creature_id = fields[0].GetUInt32();

        ReputationOnKillEntry repOnKill;
        repOnKill.RepFaction1          = fields[1].GetInt16();
        repOnKill.RepFaction2          = fields[2].GetInt16();
        repOnKill.IsTeamAward1        = fields[3].GetBool();
        repOnKill.ReputationMaxCap1  = fields[4].GetUInt8();
        repOnKill.RepValue1            = fields[5].GetInt32();
        repOnKill.IsTeamAward2        = fields[6].GetBool();
        repOnKill.ReputationMaxCap2  = fields[7].GetUInt8();
        repOnKill.RepValue2            = fields[8].GetInt32();
        repOnKill.TeamDependent       = fields[9].GetUInt8();

        if (!GetCreatureTemplate(creature_id))
        {
            TC_LOG_ERROR("sql.sql", "Table `creature_onkill_reputation` have data for not existed creature entry (%u), skipped", creature_id);
            continue;
        }

        if (repOnKill.RepFaction1)
        {
            FactionEntry const* factionEntry1 = sFactionStore.LookupEntry(repOnKill.RepFaction1);
            if (!factionEntry1)
            {
                TC_LOG_ERROR("sql.sql", "Faction (faction.dbc) %u does not exist but is used in `creature_onkill_reputation`", repOnKill.RepFaction1);
                continue;
            }
        }

        if (repOnKill.RepFaction2)
        {
            FactionEntry const* factionEntry2 = sFactionStore.LookupEntry(repOnKill.RepFaction2);
            if (!factionEntry2)
            {
                TC_LOG_ERROR("sql.sql", "Faction (faction.dbc) %u does not exist but is used in `creature_onkill_reputation`", repOnKill.RepFaction2);
                continue;
            }
        }

        _repOnKillStore[creature_id] = repOnKill;

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u creature award reputation definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadReputationSpilloverTemplate()
{
    uint32 oldMSTime = getMSTime();

    _repSpilloverTemplateStore.clear();                      // for reload case

    uint32 count = 0; //                                0         1        2       3        4       5       6         7        8      9        10       11     12        13       14     15
    QueryResult result = WorldDatabase.Query("SELECT faction, faction1, rate_1, rank_1, faction2, rate_2, rank_2, faction3, rate_3, rank_3, faction4, rate_4, rank_4, faction5, rate_5, rank_5 FROM reputation_spillover_template");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded `reputation_spillover_template`, table is empty.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 factionId                = fields[0].GetUInt16();

        RepSpilloverTemplate repTemplate;

        repTemplate.faction[0]          = fields[1].GetUInt16();
        repTemplate.faction_rate[0]     = fields[2].GetFloat();
        repTemplate.faction_rank[0]     = fields[3].GetUInt8();
        repTemplate.faction[1]          = fields[4].GetUInt16();
        repTemplate.faction_rate[1]     = fields[5].GetFloat();
        repTemplate.faction_rank[1]     = fields[6].GetUInt8();
        repTemplate.faction[2]          = fields[7].GetUInt16();
        repTemplate.faction_rate[2]     = fields[8].GetFloat();
        repTemplate.faction_rank[2]     = fields[9].GetUInt8();
        repTemplate.faction[3]          = fields[10].GetUInt16();
        repTemplate.faction_rate[3]     = fields[11].GetFloat();
        repTemplate.faction_rank[3]     = fields[12].GetUInt8();
        repTemplate.faction[4]          = fields[13].GetUInt16();
        repTemplate.faction_rate[4]     = fields[14].GetFloat();
        repTemplate.faction_rank[4]     = fields[15].GetUInt8();

        FactionEntry const* factionEntry = sFactionStore.LookupEntry(factionId);

        if (!factionEntry)
        {
            TC_LOG_ERROR("sql.sql", "Faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template`", factionId);
            continue;
        }

        if (factionEntry->team == 0)
        {
            TC_LOG_ERROR("sql.sql", "Faction (faction.dbc) %u in `reputation_spillover_template` does not belong to any team, skipping", factionId);
            continue;
        }

        for (uint32 i = 0; i < MAX_SPILLOVER_FACTIONS; ++i)
        {
            if (repTemplate.faction[i])
            {
                FactionEntry const* factionSpillover = sFactionStore.LookupEntry(repTemplate.faction[i]);

                if (!factionSpillover)
                {
                    TC_LOG_ERROR("sql.sql", "Spillover faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template` for faction %u, skipping", repTemplate.faction[i], factionId);
                    continue;
                }

                if (factionSpillover->reputationListID < 0)
                {
                    TC_LOG_ERROR("sql.sql", "Spillover faction (faction.dbc) %u for faction %u in `reputation_spillover_template` can not be listed for client, and then useless, skipping", repTemplate.faction[i], factionId);
                    continue;
                }

                if (repTemplate.faction_rank[i] >= MAX_REPUTATION_RANK)
                {
                    TC_LOG_ERROR("sql.sql", "Rank %u used in `reputation_spillover_template` for spillover faction %u is not valid, skipping", repTemplate.faction_rank[i], repTemplate.faction[i]);
                    continue;
                }
            }
        }

        FactionEntry const* factionEntry0 = sFactionStore.LookupEntry(repTemplate.faction[0]);
        if (repTemplate.faction[0] && !factionEntry0)
        {
            TC_LOG_ERROR("sql.sql", "Faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template`", repTemplate.faction[0]);
            continue;
        }
        FactionEntry const* factionEntry1 = sFactionStore.LookupEntry(repTemplate.faction[1]);
        if (repTemplate.faction[1] && !factionEntry1)
        {
            TC_LOG_ERROR("sql.sql", "Faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template`", repTemplate.faction[1]);
            continue;
        }
        FactionEntry const* factionEntry2 = sFactionStore.LookupEntry(repTemplate.faction[2]);
        if (repTemplate.faction[2] && !factionEntry2)
        {
            TC_LOG_ERROR("sql.sql", "Faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template`", repTemplate.faction[2]);
            continue;
        }
        FactionEntry const* factionEntry3 = sFactionStore.LookupEntry(repTemplate.faction[3]);
        if (repTemplate.faction[3] && !factionEntry3)
        {
            TC_LOG_ERROR("sql.sql", "Faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template`", repTemplate.faction[3]);
            continue;
        }
        FactionEntry const* factionEntry4 = sFactionStore.LookupEntry(repTemplate.faction[4]);
        if (repTemplate.faction[4] && !factionEntry4)
        {
            TC_LOG_ERROR("sql.sql", "Faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template`", repTemplate.faction[4]);
            continue;
        }

        _repSpilloverTemplateStore[factionId] = repTemplate;

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u reputation_spillover_template in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadPointsOfInterest()
{
    uint32 oldMSTime = getMSTime();

    _pointsOfInterestStore.clear();                              // need for reload case

    uint32 count = 0;

    //                                                  0   1  2   3      4     5       6
    QueryResult result = WorldDatabase.Query("SELECT entry, x, y, icon, flags, data, icon_name FROM points_of_interest");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 Points of Interest definitions. DB table `points_of_interest` is empty.");

        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 point_id = fields[0].GetUInt32();

        PointOfInterest POI;
        POI.x = fields[1].GetFloat();
        POI.y = fields[2].GetFloat();
        POI.icon = fields[3].GetUInt32();
        POI.flags = fields[4].GetUInt32();
        POI.data = fields[5].GetUInt32();
        POI.icon_name = fields[6].GetString();

        if (!Trinity::IsValidMapCoord(POI.x, POI.y))
        {
            TC_LOG_ERROR("sql.sql", "Table `points_of_interest` (Entry: %u) have invalid coordinates (X: %f Y: %f), ignored.", point_id, POI.x, POI.y);
            continue;
        }

        _pointsOfInterestStore[point_id] = POI;

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u Points of Interest definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadQuestPOI()
{
    uint32 oldMSTime = getMSTime();

    _questPOIStore.clear();                              // need for reload case

    uint32 count = 0;

    //                                               0        1   2         3      4               5        6     7
    QueryResult result = WorldDatabase.Query("SELECT questId, id, objIndex, mapid, WorldMapAreaId, FloorId, unk3, unk4 FROM quest_poi order by questId");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 quest POI definitions. DB table `quest_poi` is empty.");

        return;
    }

    //                                                0       1   2  3
    QueryResult points = WorldDatabase.Query("SELECT questId, id, x, y FROM quest_poi_points ORDER BY questId DESC, idx");

    std::vector<std::vector<std::vector<QuestPOIPoint> > > POIs;

    if (points)
    {
        // The first result should have the highest questId
        Field* fields = points->Fetch();
        uint32 questIdMax = fields[0].GetUInt32();
        POIs.resize(questIdMax + 1);

        do
        {
            fields = points->Fetch();

            uint32 questId            = fields[0].GetUInt32();
            uint32 id                 = fields[1].GetUInt32();
            int32  x                  = fields[2].GetInt32();
            int32  y                  = fields[3].GetInt32();

            if (POIs[questId].size() <= id + 1)
                POIs[questId].resize(id + 10);

            QuestPOIPoint point(x, y);
            POIs[questId][id].push_back(point);
        }
        while (points->NextRow());
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 questId            = fields[0].GetUInt32();
        uint32 id                 = fields[1].GetUInt32();
        int32 objIndex            = fields[2].GetInt32();
        uint32 mapId              = fields[3].GetUInt32();
        uint32 WorldMapAreaId     = fields[4].GetUInt32();
        uint32 FloorId            = fields[5].GetUInt32();
        uint32 unk3               = fields[6].GetUInt32();
        uint32 unk4               = fields[7].GetUInt32();

        QuestPOI POI(id, objIndex, mapId, WorldMapAreaId, FloorId, unk3, unk4);
        POI.points = POIs[questId][id];

        _questPOIStore[questId].push_back(POI);

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u quest POI definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadNPCSpellClickSpells()
{
    uint32 oldMSTime = getMSTime();

    _spellClickInfoStore.clear();
    //                                                0          1         2            3
    QueryResult result = WorldDatabase.Query("SELECT npc_entry, spell_id, cast_flags, user_type FROM npc_spellclick_spells");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 spellclick spells. DB table `npc_spellclick_spells` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 npc_entry = fields[0].GetUInt32();
        auto const cInfo = GetMutableCreatureTemplate(npc_entry);
        if (!cInfo)
        {
            TC_LOG_ERROR("sql.sql", "Table npc_spellclick_spells references unknown creature_template %u. Skipping entry.", npc_entry);
            continue;
        }

        uint32 spellid = fields[1].GetUInt32();
        SpellInfo const* spellinfo = sSpellMgr->GetSpellInfo(spellid);
        if (!spellinfo)
        {
            TC_LOG_ERROR("sql.sql", "Table npc_spellclick_spells references unknown spellid %u. Skipping entry.", spellid);
            continue;
        }

        if (!(cInfo->npcflag & UNIT_NPC_FLAG_SPELLCLICK))
            cInfo->npcflag |= UNIT_NPC_FLAG_SPELLCLICK;

        uint8 userType = fields[3].GetUInt16();
        if (userType >= SPELL_CLICK_USER_MAX)
            TC_LOG_ERROR("sql.sql", "Table npc_spellclick_spells references unknown user type %u. Skipping entry.", uint32(userType));

        uint8 castFlags = fields[2].GetUInt8();
        SpellClickInfo info;
        info.spellId = spellid;
        info.castFlags = castFlags;
        info.userType = SpellClickUserTypes(userType);
        _spellClickInfoStore.insert(SpellClickInfoContainer::value_type(npc_entry, info));

        ++count;
    }
    while (result->NextRow());

    // all spellclick data loaded, now we check if there are creatures with NPC_FLAG_SPELLCLICK but with no data
    // NOTE: It *CAN* be the other way around: no spellclick flag but with spellclick data, in case of creature-only vehicle accessories
    for (CreatureTemplateContainer::iterator itr = _creatureTemplateStore.begin(); itr != _creatureTemplateStore.end(); ++itr)
    {
        if ((itr->second.npcflag & UNIT_NPC_FLAG_SPELLCLICK) && _spellClickInfoStore.find(itr->second.Entry) == _spellClickInfoStore.end())
        {
            TC_LOG_ERROR("sql.sql", "npc_spellclick_spells: Creature template %u has UNIT_NPC_FLAG_SPELLCLICK but no data in spellclick table! Removing flag", itr->second.Entry);
            itr->second.npcflag &= ~UNIT_NPC_FLAG_SPELLCLICK;
        }
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u spellclick definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::DeleteCreatureData(uint32 guid)
{
    // remove mapid*cellid -> guid_set map
    CreatureData const* data = GetCreatureData(guid);
    if (data)
        RemoveCreatureFromGrid(guid, data);

    _creatureDataStore.erase(guid);
}

void ObjectMgr::DeleteGOData(uint32 guid)
{
    // remove mapid*cellid -> guid_set map
    GameObjectData const* data = GetGOData(guid);
    if (data)
        RemoveGameobjectFromGrid(guid, data);

    _gameObjectDataStore.erase(guid);
}

void ObjectMgr::AddCorpseCellData(uint32 mapid, uint32 cellid, uint32 player_guid, uint32 instance)
{
    MapObjectWriteGuard guard(_mapObjectGuidsStoreLock);

    // corpses are always added to spawn mode 0 and they are spawned by their instance id
    CellObjectGuids& cell_guids = _mapObjectGuidsStore[MAKE_PAIR32(mapid, 0)][cellid];
    cell_guids.corpses[player_guid] = instance;
}

void ObjectMgr::DeleteCorpseCellData(uint32 mapid, uint32 cellid, uint32 player_guid)
{
    MapObjectWriteGuard guard(_mapObjectGuidsStoreLock);

    // corpses are always added to spawn mode 0 and they are spawned by their instance id
    CellObjectGuids& cell_guids = _mapObjectGuidsStore[MAKE_PAIR32(mapid, 0)][cellid];
    cell_guids.corpses.erase(player_guid);
}

void ObjectMgr::LoadQuestRelationsHelper(QuestRelations& map, std::string table, bool starter, bool go)
{
    uint32 oldMSTime = getMSTime();

    map.clear();                                            // need for reload case

    uint32 count = 0;

    QueryResult result = WorldDatabase.PQuery("SELECT id, quest, pool_entry FROM %s qr LEFT JOIN pool_quest pq ON qr.quest = pq.entry", table.c_str());

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 quest relations from `%s`, table is empty.", table.c_str());

        return;
    }

    PooledQuestRelation* poolRelationMap = go ? &sPoolMgr->mQuestGORelation : &sPoolMgr->mQuestCreatureRelation;
    if (starter)
        poolRelationMap->clear();

    do
    {
        uint32 id     = result->Fetch()[0].GetUInt32();
        uint32 quest  = result->Fetch()[1].GetUInt32();
        uint32 poolId = result->Fetch()[2].GetUInt32();

        if (_questTemplates.find(quest) == _questTemplates.end())
        {
            TC_LOG_ERROR("sql.sql", "Table `%s`: Quest %u listed for entry %u does not exist.", table.c_str(), quest, id);
            continue;
        }

        if (!poolId || !starter)
            map.insert(QuestRelations::value_type(id, quest));
        else if (starter)
            poolRelationMap->insert(PooledQuestRelation::value_type(quest, id));

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u quest relations from %s in %u ms", count, table.c_str(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadGameobjectQuestStarters()
{
    LoadQuestRelationsHelper(_goQuestRelations, "gameobject_queststarter", true, true);

    for (QuestRelations::iterator itr = _goQuestRelations.begin(); itr != _goQuestRelations.end(); ++itr)
    {
        GameObjectTemplate const* goInfo = GetGameObjectTemplate(itr->first);
        if (!goInfo)
            TC_LOG_ERROR("sql.sql", "Table `gameobject_queststarter` have data for not existed gameobject entry (%u) and existed quest %u", itr->first, itr->second);
        else if (goInfo->type != GAMEOBJECT_TYPE_QUESTGIVER)
            TC_LOG_ERROR("sql.sql", "Table `gameobject_queststarter` have data gameobject entry (%u) for quest %u, but GO is not GAMEOBJECT_TYPE_QUESTGIVER", itr->first, itr->second);
    }
}

void ObjectMgr::LoadGameobjectQuestEnders()
{
    LoadQuestRelationsHelper(_goQuestInvolvedRelations, "gameobject_questender", false, true);

    for (QuestRelations::iterator itr = _goQuestInvolvedRelations.begin(); itr != _goQuestInvolvedRelations.end(); ++itr)
    {
        GameObjectTemplate const* goInfo = GetGameObjectTemplate(itr->first);
        if (!goInfo)
            TC_LOG_ERROR("sql.sql", "Table `gameobject_questender` have data for not existed gameobject entry (%u) and existed quest %u", itr->first, itr->second);
        else if (goInfo->type != GAMEOBJECT_TYPE_QUESTGIVER)
            TC_LOG_ERROR("sql.sql", "Table `gameobject_questender` have data gameobject entry (%u) for quest %u, but GO is not GAMEOBJECT_TYPE_QUESTGIVER", itr->first, itr->second);
    }
}

void ObjectMgr::LoadCreatureQuestStarters()
{
    LoadQuestRelationsHelper(_creatureQuestRelations, "creature_queststarter", true, false);

    for (QuestRelations::iterator itr = _creatureQuestRelations.begin(); itr != _creatureQuestRelations.end(); ++itr)
    {
        CreatureTemplate const* cInfo = GetCreatureTemplate(itr->first);
        if (!cInfo)
            TC_LOG_ERROR("sql.sql", "Table `creature_queststarter` have data for not existed creature entry (%u) and existed quest %u", itr->first, itr->second);
        else if (!(cInfo->npcflag & UNIT_NPC_FLAG_QUESTGIVER))
            TC_LOG_ERROR("sql.sql", "Table `creature_queststarter` has creature entry (%u) for quest %u, but npcflag does not include UNIT_NPC_FLAG_QUESTGIVER", itr->first, itr->second);
    }
}

void ObjectMgr::LoadCreatureQuestEnders()
{
    LoadQuestRelationsHelper(_creatureQuestInvolvedRelations, "creature_questender", false, false);

    for (QuestRelations::iterator itr = _creatureQuestInvolvedRelations.begin(); itr != _creatureQuestInvolvedRelations.end(); ++itr)
    {
        CreatureTemplate const* cInfo = GetCreatureTemplate(itr->first);
        if (!cInfo)
            TC_LOG_ERROR("sql.sql", "Table `creature_questender` have data for not existed creature entry (%u) and existed quest %u", itr->first, itr->second);
        else if (!(cInfo->npcflag & UNIT_NPC_FLAG_QUESTGIVER))
            TC_LOG_ERROR("sql.sql", "Table `creature_questender` has creature entry (%u) for quest %u, but npcflag does not include UNIT_NPC_FLAG_QUESTGIVER", itr->first, itr->second);
    }
}

void ObjectMgr::LoadReservedPlayersNames()
{
    uint32 oldMSTime = getMSTime();

    _reservedNamesStore.clear();                                // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT name FROM reserved_name");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 reserved player names. DB table `reserved_name` is empty!");
        return;
    }

    uint32 count = 0;

    Field* fields;
    do
    {
        fields = result->Fetch();
        std::string name= fields[0].GetString();

        std::wstring wstr;
        if (!Utf8toWStr (name, wstr))
        {
            TC_LOG_ERROR("misc", "Table `reserved_name` have invalid name: %s", name.c_str());
            continue;
        }

        wstrToLower(wstr);

        _reservedNamesStore.insert(wstr);
        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u reserved player names in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

bool ObjectMgr::IsReservedName(const std::string& name) const
{
    std::wstring wstr;
    if (!Utf8toWStr (name, wstr))
        return false;

    wstrToLower(wstr);

    return _reservedNamesStore.find(wstr) != _reservedNamesStore.end();
}

enum LanguageType
{
    LT_BASIC_LATIN    = 0x0000,
    LT_EXTENDEN_LATIN = 0x0001,
    LT_CYRILLIC       = 0x0002,
    LT_EAST_ASIA      = 0x0004,
    LT_ANY            = 0xFFFF
};

static LanguageType GetRealmLanguageType(bool create)
{
    switch (sWorld->getIntConfig(CONFIG_REALM_ZONE))
    {
        case REALM_ZONE_UNKNOWN:                            // any language
        case REALM_ZONE_DEVELOPMENT:
        case REALM_ZONE_TEST_SERVER:
        case REALM_ZONE_QA_SERVER:
            return LT_ANY;
        case REALM_ZONE_UNITED_STATES:                      // extended-Latin
        case REALM_ZONE_OCEANIC:
        case REALM_ZONE_LATIN_AMERICA:
        case REALM_ZONE_ENGLISH:
        case REALM_ZONE_GERMAN:
        case REALM_ZONE_FRENCH:
        case REALM_ZONE_SPANISH:
            return LT_EXTENDEN_LATIN;
        case REALM_ZONE_KOREA:                              // East-Asian
        case REALM_ZONE_TAIWAN:
        case REALM_ZONE_CHINA:
            return LT_EAST_ASIA;
        case REALM_ZONE_RUSSIAN:                            // Cyrillic
            return LT_CYRILLIC;
        default:
            return create ? LT_BASIC_LATIN : LT_ANY;        // basic-Latin at create, any at login
    }
}

bool isValidString(std::wstring wstr, uint32 strictMask, bool numericOrSpace, bool create = false)
{
    if (strictMask == 0)                                       // any language, ignore realm
    {
        if (isExtendedLatinString(wstr, numericOrSpace))
            return true;
        if (isCyrillicString(wstr, numericOrSpace))
            return true;
        if (isEastAsianString(wstr, numericOrSpace))
            return true;
        return false;
    }

    if (strictMask & 0x2)                                    // realm zone specific
    {
        LanguageType lt = GetRealmLanguageType(create);
        if (lt & LT_EXTENDEN_LATIN)
            if (isExtendedLatinString(wstr, numericOrSpace))
                return true;
        if (lt & LT_CYRILLIC)
            if (isCyrillicString(wstr, numericOrSpace))
                return true;
        if (lt & LT_EAST_ASIA)
            if (isEastAsianString(wstr, numericOrSpace))
                return true;
    }

    if (strictMask & 0x1)                                    // basic Latin
    {
        if (isBasicLatinString(wstr, numericOrSpace))
            return true;
    }

    return false;
}

uint8 ObjectMgr::CheckPlayerName(const std::string& name, bool create)
{
    std::wstring wname;
    if (!Utf8toWStr(name, wname))
        return CHAR_NAME_INVALID_CHARACTER;

    if (wname.size() > MAX_PLAYER_NAME)
        return CHAR_NAME_TOO_LONG;

    uint32 minName = sWorld->getIntConfig(CONFIG_MIN_PLAYER_NAME);
    if (wname.size() < minName)
        return CHAR_NAME_TOO_SHORT;

    uint32 strictMask = sWorld->getIntConfig(CONFIG_STRICT_PLAYER_NAMES);
    if (!isValidString(wname, strictMask, false, create))
        return CHAR_NAME_MIXED_LANGUAGES;

    wstrToLower(wname);
    for (size_t i = 2; i < wname.size(); ++i)
        if (wname[i] == wname[i-1] && wname[i] == wname[i-2])
            return CHAR_NAME_THREE_CONSECUTIVE;

    return CHAR_NAME_SUCCESS;
}

bool ObjectMgr::IsValidCharterName(const std::string& name)
{
    std::wstring wname;
    if (!Utf8toWStr(name, wname))
        return false;

    if (wname.size() > MAX_CHARTER_NAME)
        return false;

    uint32 minName = sWorld->getIntConfig(CONFIG_MIN_CHARTER_NAME);
    if (wname.size() < minName)
        return false;

    uint32 strictMask = sWorld->getIntConfig(CONFIG_STRICT_CHARTER_NAMES);

    return isValidString(wname, strictMask, true);
}

PetNameInvalidReason ObjectMgr::CheckPetName(const std::string& name)
{
    std::wstring wname;
    if (!Utf8toWStr(name, wname))
        return PET_NAME_INVALID;

    if (wname.size() > MAX_PET_NAME)
        return PET_NAME_TOO_LONG;

    uint32 minName = sWorld->getIntConfig(CONFIG_MIN_PET_NAME);
    if (wname.size() < minName)
        return PET_NAME_TOO_SHORT;

    uint32 strictMask = sWorld->getIntConfig(CONFIG_STRICT_PET_NAMES);
    if (!isValidString(wname, strictMask, false))
        return PET_NAME_MIXED_LANGUAGES;

    return PET_NAME_SUCCESS;
}

void ObjectMgr::LoadGameObjectForQuests()
{
    uint32 oldMSTime = getMSTime();

    _gameObjectForQuestStore.clear();                         // need for reload case

    if (sObjectMgr->GetGameObjectTemplates()->empty())
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 GameObjects for quests");
        return;
    }

    uint32 count = 0;

    // collect GO entries for GO that must activated
    GameObjectTemplateContainer const* gotc = sObjectMgr->GetGameObjectTemplates();
    for (GameObjectTemplateContainer::const_iterator itr = gotc->begin(); itr != gotc->end(); ++itr)
    {
        switch (itr->second.type)
        {
            // scan GO chest with loot including quest items
            case GAMEOBJECT_TYPE_CHEST:
            {
                uint32 loot_id = (itr->second.GetLootId());

                // find quest loot for GO
                if (itr->second.chest.questId || LootTemplates_Gameobject.HaveQuestLootFor(loot_id))
                {
                    _gameObjectForQuestStore.insert(itr->second.entry);
                    ++count;
                }
                break;
            }
            case GAMEOBJECT_TYPE_GENERIC:
            {
                if (itr->second._generic.questID > 0)            //quests objects
                {
                    _gameObjectForQuestStore.insert(itr->second.entry);
                    count++;
                }
                break;
            }
            case GAMEOBJECT_TYPE_GOOBER:
            {
                if (itr->second.goober.questId > 0)              //quests objects
                {
                    _gameObjectForQuestStore.insert(itr->second.entry);
                    count++;
                }
                break;
            }
            default:
                break;
        }
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u GameObjects for quests in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

bool ObjectMgr::LoadTrinityStrings(const char* table, int32 min_value, int32 max_value)
{
    uint32 oldMSTime = getMSTime();

    int32 start_value = min_value;
    int32 end_value   = max_value;
    // some string can have negative indexes range
    if (start_value < 0)
    {
        if (end_value >= start_value)
        {
            TC_LOG_ERROR("sql.sql", "Table '%s' attempt loaded with invalid range (%d - %d), strings not loaded.", table, min_value, max_value);
            return false;
        }

        // real range (max+1, min+1) exaple: (-10, -1000) -> -999...-10+1
        std::swap(start_value, end_value);
        ++start_value;
        ++end_value;
    }
    else
    {
        if (start_value >= end_value)
        {
            TC_LOG_ERROR("sql.sql", "Table '%s' attempt loaded with invalid range (%d - %d), strings not loaded.", table, min_value, max_value);
            return false;
        }
    }

    // cleanup affected map part for reloading case
    for (TrinityStringLocaleContainer::iterator itr = _trinityStringLocaleStore.begin(); itr != _trinityStringLocaleStore.end();)
    {
        if (itr->first >= start_value && itr->first < end_value)
            _trinityStringLocaleStore.erase(itr++);
        else
            ++itr;
    }

    QueryResult result = WorldDatabase.PQuery("SELECT entry, content_default, content_loc1, content_loc2, content_loc3, content_loc4, content_loc5, content_loc6, content_loc7, content_loc8, content_loc9, content_loc10 FROM %s", table);

    if (!result)
    {
        if (min_value == MIN_TRINITY_STRING_ID)              // error only in case internal strings
            TC_LOG_ERROR("server.loading", ">>  Loaded 0 trinity strings. DB table `%s` is empty. Cannot continue.", table);
        else
            TC_LOG_INFO("server.loading", ">> Loaded 0 string templates. DB table `%s` is empty.", table);

        return false;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        int32 entry = fields[0].GetInt32();

        if (entry == 0)
        {
            TC_LOG_ERROR("sql.sql", "Table `%s` contain reserved entry 0, ignored.", table);
            continue;
        }
        else if (entry < start_value || entry >= end_value)
        {
            TC_LOG_ERROR("sql.sql", "Table `%s` contain entry %i out of allowed range (%d - %d), ignored.", table, entry, min_value, max_value);
            continue;
        }

        TrinityStringLocale& data = _trinityStringLocaleStore[entry];

        if (!data.Content.empty())
        {
            TC_LOG_ERROR("sql.sql", "Table `%s` contain data for already loaded entry  %i (from another table?), ignored.", table, entry);
            continue;
        }

        data.Content.resize(1);
        ++count;

        for (uint8 i = 0; i < TOTAL_LOCALES; ++i)
            AddLocaleString(fields[i + 1].GetString(), LocaleConstant(i), data.Content);
    }
    while (result->NextRow());

    if (min_value == MIN_TRINITY_STRING_ID)
        TC_LOG_INFO("server.loading", ">> Loaded %u Trinity strings from table %s in %u ms", count, table, GetMSTimeDiffToNow(oldMSTime));
    else
        TC_LOG_INFO("server.loading", ">> Loaded %u string templates from %s in %u ms", count, table, GetMSTimeDiffToNow(oldMSTime));

    return true;
}

char const * ObjectMgr::GetTrinityString(int32 entry, LocaleConstant locale_idx) const
{
    if (TrinityStringLocale const* msl = GetTrinityStringLocale(entry))
    {
        if (msl->Content.size() > size_t(locale_idx) && !msl->Content[locale_idx].empty())
            return msl->Content[locale_idx].c_str();
        return msl->Content[DEFAULT_LOCALE].c_str();
    }

    if (entry > 0)
        TC_LOG_ERROR("sql.sql", "Entry %i not found in `trinity_string` table.", entry);
    else
        TC_LOG_ERROR("sql.sql", "Trinity string entry %i not found in DB.", entry);

    return "<error>";
}

void ObjectMgr::LoadFishingBaseSkillLevel()
{
    uint32 oldMSTime = getMSTime();

    _fishingBaseForAreaStore.clear();                            // for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, skill FROM skill_fishing_base_level");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 areas for fishing base skill level. DB table `skill_fishing_base_level` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        uint32 entry  = fields[0].GetUInt32();
        int32 skill   = fields[1].GetInt16();

        AreaTableEntry const* fArea = GetAreaEntryByAreaID(entry);
        if (!fArea)
        {
            TC_LOG_ERROR("sql.sql", "AreaId %u defined in `skill_fishing_base_level` does not exist", entry);
            continue;
        }

        _fishingBaseForAreaStore[entry] = skill;
        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u areas for fishing base skill level in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

bool ObjectMgr::CheckDeclinedNames(std::wstring w_ownname, DeclinedName const& names)
{
    // get main part of the name
    std::wstring mainpart = GetMainPartOfName(w_ownname, 0);
    // prepare flags
    bool x = true;
    bool y = true;

    // check declined names
    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
    {
        std::wstring wname;
        if (!Utf8toWStr(names.name[i], wname))
            return false;

        if (mainpart != GetMainPartOfName(wname, i+1))
            x = false;

        if (w_ownname != wname)
            y = false;
    }
    return (x || y);
}

uint32 ObjectMgr::GetAreaTriggerScriptId(uint32 trigger_id)
{
    AreaTriggerScriptContainer::const_iterator i = _areaTriggerScriptStore.find(trigger_id);
    if (i!= _areaTriggerScriptStore.end())
        return i->second;
    return 0;
}

SpellScriptsBounds ObjectMgr::GetSpellScriptsBounds(uint32 spell_id)
{
    return SpellScriptsBounds(_spellScriptsStore.lower_bound(spell_id), _spellScriptsStore.upper_bound(spell_id));
}

SkillRangeType GetSkillRangeType(SkillLineEntry const* pSkill, bool racial)
{
    switch (pSkill->categoryId)
    {
        case SKILL_CATEGORY_LANGUAGES:
            return SKILL_RANGE_LANGUAGE;
        case SKILL_CATEGORY_WEAPON:
            return SKILL_RANGE_LEVEL;
        case SKILL_CATEGORY_ARMOR:
        case SKILL_CATEGORY_CLASS:
            if (pSkill->id != SKILL_LOCKPICKING)
                return SKILL_RANGE_MONO;
            else
                return SKILL_RANGE_LEVEL;
        case SKILL_CATEGORY_SECONDARY:
        case SKILL_CATEGORY_PROFESSION:
            // not set skills for professions and racial abilities
            if (IsProfessionSkill(pSkill->id))
                return SKILL_RANGE_RANK;
            else if (racial)
                return SKILL_RANGE_NONE;
            else
                return SKILL_RANGE_MONO;
        default:
        case SKILL_CATEGORY_ATTRIBUTES:                     //not found in dbc
        case SKILL_CATEGORY_GENERIC:                        //only GENERIC(DND)
            return SKILL_RANGE_NONE;
    }
}

void ObjectMgr::LoadGameTele()
{
    uint32 oldMSTime = getMSTime();

    _gameTeleStore.clear();                                  // for reload case

    //                                                0       1           2           3           4        5     6
    QueryResult result = WorldDatabase.Query("SELECT id, position_x, position_y, position_z, orientation, map, name FROM game_tele");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">>  Loaded 0 GameTeleports. DB table `game_tele` is empty!");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 id         = fields[0].GetUInt32();

        GameTele gt;

        gt.position_x     = fields[1].GetFloat();
        gt.position_y     = fields[2].GetFloat();
        gt.position_z     = fields[3].GetFloat();
        gt.orientation    = fields[4].GetFloat();
        gt.mapId          = fields[5].GetUInt16();
        gt.name           = fields[6].GetString();

        if (!MapManager::IsValidMAP(gt.mapId, true) || !Trinity::IsValidMapCoord(gt.position_x, gt.position_y, gt.position_z, gt.orientation))
        {
            TC_LOG_ERROR("sql.sql", "Wrong position for id %u (name: %s) in `game_tele` table, ignoring.", id, gt.name.c_str());
            continue;
        }

        if (!Utf8toWStr(gt.name, gt.wnameLow))
        {
            TC_LOG_ERROR("sql.sql", "Wrong UTF8 name for id %u in `game_tele` table, ignoring.", id);
            continue;
        }

        wstrToLower(gt.wnameLow);

        _gameTeleStore[id] = gt;

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u GameTeleports in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

GameTele const* ObjectMgr::GetGameTele(const std::string& name) const
{
    // explicit name case
    std::wstring wname;
    if (!Utf8toWStr(name, wname))
        return NULL;

    // converting string that we try to find to lower case
    wstrToLower(wname);

    // Alternative first GameTele what contains wnameLow as substring in case no GameTele location found
    const GameTele* alt = NULL;
    for (GameTeleContainer::const_iterator itr = _gameTeleStore.begin(); itr != _gameTeleStore.end(); ++itr)
    {
        if (itr->second.wnameLow == wname)
            return &itr->second;
        else if (alt == NULL && itr->second.wnameLow.find(wname) != std::wstring::npos)
            alt = &itr->second;
    }

    return alt;
}

bool ObjectMgr::AddGameTele(GameTele& tele)
{
    // find max id
    uint32 new_id = 0;
    for (GameTeleContainer::const_iterator itr = _gameTeleStore.begin(); itr != _gameTeleStore.end(); ++itr)
        if (itr->first > new_id)
            new_id = itr->first;

    // use next
    ++new_id;

    if (!Utf8toWStr(tele.name, tele.wnameLow))
        return false;

    wstrToLower(tele.wnameLow);

    _gameTeleStore[new_id] = tele;

    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_INS_GAME_TELE);

    stmt->setUInt32(0, new_id);
    stmt->setFloat(1, tele.position_x);
    stmt->setFloat(2, tele.position_y);
    stmt->setFloat(3, tele.position_z);
    stmt->setFloat(4, tele.orientation);
    stmt->setUInt16(5, uint16(tele.mapId));
    stmt->setString(6, tele.name);

    WorldDatabase.Execute(stmt);

    return true;
}

bool ObjectMgr::DeleteGameTele(const std::string& name)
{
    // explicit name case
    std::wstring wname;
    if (!Utf8toWStr(name, wname))
        return false;

    // converting string that we try to find to lower case
    wstrToLower(wname);

    for (GameTeleContainer::iterator itr = _gameTeleStore.begin(); itr != _gameTeleStore.end(); ++itr)
    {
        if (itr->second.wnameLow == wname)
        {
            PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_DEL_GAME_TELE);

            stmt->setString(0, itr->second.name);

            WorldDatabase.Execute(stmt);

            _gameTeleStore.erase(itr);
            return true;
        }
    }

    return false;
}

void ObjectMgr::LoadMailLevelRewards()
{
    uint32 oldMSTime = getMSTime();

    _mailLevelRewardStore.clear();                           // for reload case

    //                                                 0        1             2            3
    QueryResult result = WorldDatabase.Query("SELECT level, raceMask, mailTemplateId, senderEntry FROM mail_level_reward");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">>  Loaded 0 level dependent mail rewards. DB table `mail_level_reward` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint8 level           = fields[0].GetUInt8();
        uint32 raceMask       = fields[1].GetUInt32();
        uint32 mailTemplateId = fields[2].GetUInt32();
        uint32 senderEntry    = fields[3].GetUInt32();

        if (level > MAX_LEVEL)
        {
            TC_LOG_ERROR("sql.sql", "Table `mail_level_reward` have data for level %u that more supported by client (%u), ignoring.", level, MAX_LEVEL);
            continue;
        }

        if (!(raceMask & RACEMASK_ALL_PLAYABLE))
        {
            TC_LOG_ERROR("sql.sql", "Table `mail_level_reward` have raceMask (%u) for level %u that not include any player races, ignoring.", raceMask, level);
            continue;
        }

        if (!sMailTemplateStore.LookupEntry(mailTemplateId))
        {
            TC_LOG_ERROR("sql.sql", "Table `mail_level_reward` have invalid mailTemplateId (%u) for level %u that invalid not include any player races, ignoring.", mailTemplateId, level);
            continue;
        }

        if (!GetCreatureTemplate(senderEntry))
        {
            TC_LOG_ERROR("sql.sql", "Table `mail_level_reward` have not existed sender creature entry (%u) for level %u that invalid not include any player races, ignoring.", senderEntry, level);
            continue;
        }

        _mailLevelRewardStore[level].push_back(MailLevelReward(raceMask, mailTemplateId, senderEntry));

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u level dependent mail rewards in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::AddSpellToTrainer(uint32 entry, uint32 spell, uint32 spellCost, uint32 reqSkill, uint32 reqSkillValue, uint32 reqLevel)
{
    if (entry >= TRINITY_TRAINER_START_REF)
        return;

    CreatureTemplate const* cInfo = GetCreatureTemplate(entry);
    if (!cInfo)
    {
        TC_LOG_ERROR("sql.sql", "Table `npc_trainer` contains an entry for a non-existing creature template (Entry: %u), ignoring", entry);
        return;
    }

    if (!(cInfo->npcflag & UNIT_NPC_FLAG_TRAINER))
    {
        TC_LOG_ERROR("sql.sql", "Table `npc_trainer` contains an entry for a creature template (Entry: %u) without trainer flag, ignoring", entry);
        return;
    }

    SpellInfo const* spellinfo = sSpellMgr->GetSpellInfo(spell);
    if (!spellinfo)
    {
        TC_LOG_ERROR("sql.sql", "Table `npc_trainer` contains an entry (Entry: %u) for a non-existing spell (Spell: %u), ignoring", entry, spell);
        return;
    }

    if (!sSpellMgr->IsSpellValid(spellinfo))
    {
        TC_LOG_ERROR("sql.sql", "Table `npc_trainer` contains an entry (Entry: %u) for a broken spell (Spell: %u), ignoring", entry, spell);
        return;
    }

    if (sSpellMgr->IsTalent(spellinfo->Id))
    {
        TC_LOG_ERROR("sql.sql", "Table `npc_trainer` contains an entry (Entry: %u) for a non-existing spell (Spell: %u) which is a talent, ignoring", entry, spell);
        return;
    }

    TrainerSpellData& data = _cacheTrainerSpellStore[entry];

    TrainerSpell& trainerSpell = data.spellList[spell];
    trainerSpell.spell         = spell;
    trainerSpell.spellCost     = spellCost;
    trainerSpell.reqSkill      = reqSkill;
    trainerSpell.reqSkillValue = reqSkillValue;
    trainerSpell.reqLevel      = reqLevel;

    if (!trainerSpell.reqLevel)
        trainerSpell.reqLevel = spellinfo->SpellLevel;

    // calculate learned spell for profession case when stored cast-spell
    trainerSpell.learnedSpell[0] = spell;
    for (uint8 i = 0; i < spellinfo->Effects.size(); ++i)
    {
        auto const &spellEffect = spellinfo->Effects[i];
        if (spellEffect.Effect != SPELL_EFFECT_LEARN_SPELL)
            continue;

        if (trainerSpell.learnedSpell[0] == spell)
            trainerSpell.learnedSpell[0] = 0;

        // player must be able to cast spell on himself
        if (spellEffect.TargetA.GetTarget() != 0
                && spellEffect.TargetA.GetTarget() != TARGET_UNIT_TARGET_ALLY
                && spellEffect.TargetA.GetTarget() != TARGET_UNIT_TARGET_ANY
                && spellEffect.TargetA.GetTarget() != TARGET_UNIT_CASTER)
        {
            TC_LOG_ERROR("sql.sql", "Table `npc_trainer` has spell %u for trainer entry %u with learn effect which has incorrect target type, ignoring learn effect!", spell, entry);
            continue;
        }

        trainerSpell.learnedSpell[i] = spellEffect.TriggerSpell;

        if (auto const learnedSpell = trainerSpell.learnedSpell[i])
        {
            auto const learnedSpellInfo = sSpellMgr->GetSpellInfo(learnedSpell);
            if (learnedSpellInfo && learnedSpellInfo->IsProfession())
                data.trainerType = 2;
        }
    }

    return;
}

void ObjectMgr::LoadTrainerSpell()
{
    uint32 oldMSTime = getMSTime();

    // For reload case
    _cacheTrainerSpellStore.clear();

    QueryResult result = WorldDatabase.Query("SELECT b.entry, a.spell, a.spellcost, a.reqskill, a.reqskillvalue, a.reqlevel FROM npc_trainer AS a "
                                             "INNER JOIN npc_trainer AS b ON a.entry = -(b.spell) "
                                             "UNION SELECT * FROM npc_trainer WHERE spell > 0");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">>  Loaded 0 Trainers. DB table `npc_trainer` is empty!");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry         = fields[0].GetUInt32();
        uint32 spell         = fields[1].GetUInt32();
        uint32 spellCost     = fields[2].GetUInt32();
        uint32 reqSkill      = fields[3].GetUInt16();
        uint32 reqSkillValue = fields[4].GetUInt16();
        uint32 reqLevel      = fields[5].GetUInt8();

        AddSpellToTrainer(entry, spell, spellCost, reqSkill, reqSkillValue, reqLevel);

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %d Trainers in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

int ObjectMgr::LoadReferenceVendor(int32 vendor, int32 item, uint8 type, std::set<uint32> *skip_vendors)
{
    // find all items from the reference vendor
    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_NPC_VENDOR_REF);
    stmt->setUInt32(0, uint32(item));
    stmt->setUInt8(1, type);
    PreparedQueryResult result = WorldDatabase.Query(stmt);

    if (!result)
        return 0;

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        int32 item_id = fields[0].GetInt32();

        // if item is a negative, its a reference
        if (item_id < 0)
            count += LoadReferenceVendor(vendor, -item_id, type, skip_vendors);
        else
        {
            int32  maxcount     = fields[1].GetUInt32();
            uint32 incrtime     = fields[2].GetUInt32();
            uint32 ExtendedCost = fields[3].GetUInt32();
            uint8  type         = fields[4].GetUInt8();

            if (!IsVendorItemValid(vendor, item_id, maxcount, incrtime, ExtendedCost, type, NULL, skip_vendors))
                continue;

            VendorItemData& vList = _cacheVendorItemStore[vendor];

            vList.AddItem(item_id, maxcount, incrtime, ExtendedCost, type);
            ++count;
        }
    }
    while (result->NextRow());

    return count;
}

void ObjectMgr::LoadVendors()
{
    uint32 oldMSTime = getMSTime();

    // For reload case
    for (CacheVendorItemContainer::iterator itr = _cacheVendorItemStore.begin(); itr != _cacheVendorItemStore.end(); ++itr)
        itr->second.Clear();
    _cacheVendorItemStore.clear();

    std::set<uint32> skip_vendors;

    QueryResult result = WorldDatabase.Query("SELECT entry, item, maxcount, incrtime, ExtendedCost, type FROM npc_vendor ORDER BY entry, slot ASC");
    if (!result)
    {

        TC_LOG_ERROR("server.loading", ">>  Loaded 0 Vendors. DB table `npc_vendor` is empty!");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry        = fields[0].GetUInt32();
        int32 item_id      = fields[1].GetInt32();

        // if item is a negative, its a reference
        if (item_id < 0)
            count += LoadReferenceVendor(entry, -item_id, 0, &skip_vendors);
        else
        {
            uint32 maxcount     = fields[2].GetUInt32();
            uint32 incrtime     = fields[3].GetUInt32();
            uint32 ExtendedCost = fields[4].GetUInt32();
            uint8  type         = fields[5].GetUInt8();

            if (!IsVendorItemValid(entry, item_id, maxcount, incrtime, ExtendedCost, type, NULL, &skip_vendors))
                continue;

            VendorItemData& vList = _cacheVendorItemStore[entry];

            vList.AddItem(item_id, maxcount, incrtime, ExtendedCost, type);
            ++count;
        }
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %d Vendors in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadGossipMenu()
{
    uint32 oldMSTime = getMSTime();

    _gossipMenusStore.clear();

    QueryResult result = WorldDatabase.Query("SELECT entry, text_id FROM gossip_menu");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0  gossip_menu entries. DB table `gossip_menu` is empty!");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        GossipMenus gMenu;

        gMenu.entry             = fields[0].GetUInt32();
        gMenu.text_id           = fields[1].GetUInt32();

        if (!GetGossipText(gMenu.text_id))
        {
            TC_LOG_ERROR("sql.sql", "Table gossip_menu entry %u are using non-existing text_id %u", gMenu.entry, gMenu.text_id);
            continue;
        }

        _gossipMenusStore.insert(GossipMenusContainer::value_type(gMenu.entry, gMenu));

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u gossip_menu entries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadGossipMenuItems()
{
    uint32 oldMSTime = getMSTime();

    _gossipMenuItemsStore.clear();

    QueryResult result = WorldDatabase.Query(
        //          0              1            2           3              4
        "SELECT menu_id, id, option_icon, option_text, option_id, npc_option_npcflag, "
        //       5              6           7          8         9
        "action_menu_id, action_poi_id, box_coded, box_money, box_text "
        "FROM gossip_menu_option ORDER BY menu_id, id");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 gossip_menu_option entries. DB table `gossip_menu_option` is empty!");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        GossipMenuItems gMenuItem;

        gMenuItem.MenuId                = fields[0].GetUInt32();
        gMenuItem.OptionIndex           = fields[1].GetUInt16();
        gMenuItem.OptionIcon            = fields[2].GetUInt32();
        gMenuItem.OptionText            = fields[3].GetString();
        gMenuItem.OptionType            = fields[4].GetUInt8();
        gMenuItem.OptionNpcflag         = fields[5].GetUInt32();
        gMenuItem.ActionMenuId          = fields[6].GetUInt32();
        gMenuItem.ActionPoiId           = fields[7].GetUInt32();
        gMenuItem.BoxCoded              = fields[8].GetBool();
        gMenuItem.BoxMoney              = fields[9].GetUInt32();
        gMenuItem.BoxText               = fields[10].GetString();

        if (gMenuItem.OptionIcon >= GOSSIP_ICON_MAX)
        {
            TC_LOG_ERROR("sql.sql", "Table gossip_menu_option for menu %u, id %u has unknown icon id %u. Replacing with GOSSIP_ICON_CHAT", gMenuItem.MenuId, gMenuItem.OptionIndex, gMenuItem.OptionIcon);
            gMenuItem.OptionIcon = GOSSIP_ICON_CHAT;
        }

        if (gMenuItem.OptionType >= GOSSIP_OPTION_MAX)
            TC_LOG_ERROR("sql.sql", "Table gossip_menu_option for menu %u, id %u has unknown option id %u. Option will not be used", gMenuItem.MenuId, gMenuItem.OptionIndex, gMenuItem.OptionType);

        if (gMenuItem.ActionPoiId && !GetPointOfInterest(gMenuItem.ActionPoiId))
        {
            TC_LOG_ERROR("sql.sql", "Table gossip_menu_option for menu %u, id %u use non-existing action_poi_id %u, ignoring", gMenuItem.MenuId, gMenuItem.OptionIndex, gMenuItem.ActionPoiId);
            gMenuItem.ActionPoiId = 0;
        }

        _gossipMenuItemsStore.insert(GossipMenuItemsContainer::value_type(gMenuItem.MenuId, gMenuItem));
        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u gossip_menu_option entries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::AddVendorItem(uint32 entry, uint32 item, int32 maxcount, uint32 incrtime, uint32 extendedCost, uint8 type, bool persist /*= true*/)
{
    VendorItemData& vList = _cacheVendorItemStore[entry];
    vList.AddItem(item, maxcount, incrtime, extendedCost, type);

    if (persist)
    {
        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_INS_NPC_VENDOR);

        stmt->setUInt32(0, entry);
        stmt->setUInt32(1, item);
        stmt->setUInt8(2, maxcount);
        stmt->setUInt32(3, incrtime);
        stmt->setUInt32(4, extendedCost);
        stmt->setUInt8(5, type);

        WorldDatabase.Execute(stmt);
    }
}

bool ObjectMgr::RemoveVendorItem(uint32 entry, uint32 item, uint8 type, bool persist /*= true*/)
{
    CacheVendorItemContainer::iterator  iter = _cacheVendorItemStore.find(entry);
    if (iter == _cacheVendorItemStore.end())
        return false;

    if (!iter->second.RemoveItem(item, type))
        return false;

    if (persist)
    {
        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_DEL_NPC_VENDOR);

        stmt->setUInt32(0, entry);
        stmt->setUInt32(1, item);
        stmt->setUInt8(2, type);

        WorldDatabase.Execute(stmt);
    }

    return true;
}

bool ObjectMgr::IsVendorItemValid(uint32 vendor_entry, uint32 id, int32 maxcount, uint32 incrtime, uint32 ExtendedCost, uint8 type, Player* player, std::set<uint32>* skip_vendors, uint32 ORnpcflag) const
{
    CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(vendor_entry);
    if (!cInfo)
    {
        if (player)
            ChatHandler(player).SendSysMessage(LANG_COMMAND_VENDORSELECTION);
        else
            TC_LOG_ERROR("sql.sql", "Table `(game_event_)npc_vendor` have data for not existed creature template (Entry: %u), ignore", vendor_entry);
        return false;
    }

    if (!((cInfo->npcflag | ORnpcflag) & UNIT_NPC_FLAG_VENDOR))
    {
        if (!skip_vendors || skip_vendors->count(vendor_entry) == 0)
        {
            if (player)
                ChatHandler(player).SendSysMessage(LANG_COMMAND_VENDORSELECTION);
            else
                TC_LOG_ERROR("sql.sql", "Table `(game_event_)npc_vendor` have data for not creature template (Entry: %u) without vendor flag, ignore", vendor_entry);

            if (skip_vendors)
                skip_vendors->insert(vendor_entry);
        }
        return false;
    }

    if ((type == ITEM_VENDOR_TYPE_ITEM && !sObjectMgr->GetItemTemplate(id)) ||
        (type == ITEM_VENDOR_TYPE_CURRENCY && !sCurrencyTypesStore.LookupEntry(id)))
    {
        if (player)
            ChatHandler(player).PSendSysMessage(LANG_ITEM_NOT_FOUND, id, type);
        else
            TC_LOG_ERROR("sql.sql", "Table `(game_event_)npc_vendor` for Vendor (Entry: %u) have in item list non-existed item (%u, type %u), ignore", vendor_entry, id, type);
        return false;
    }

    if (ExtendedCost && !sItemExtendedCostStore.LookupEntry(ExtendedCost))
    {
        if (player)
            ChatHandler(player).PSendSysMessage(LANG_EXTENDED_COST_NOT_EXIST, ExtendedCost);
        else
            TC_LOG_ERROR("sql.sql", "Table `(game_event_)npc_vendor` have Item (Entry: %u) with wrong ExtendedCost (%u) for vendor (%u), ignore", id, ExtendedCost, vendor_entry);
        return false;
    }

    if (type == ITEM_VENDOR_TYPE_ITEM) // not applicable to currencies
    {
        if (maxcount > 0 && incrtime == 0)
        {
            if (player)
                ChatHandler(player).PSendSysMessage("MaxCount != 0 (%u) but IncrTime == 0", maxcount);
            else
                TC_LOG_ERROR("sql.sql", "Table `(game_event_)npc_vendor` has `maxcount` (%u) for item %u of vendor (Entry: %u) but `incrtime`=0, ignore", maxcount, id, vendor_entry);
            return false;
        }
        else if (maxcount == 0 && incrtime > 0)
        {
            if (player)
                ChatHandler(player).PSendSysMessage("MaxCount == 0 but IncrTime<>= 0");
            else
                TC_LOG_ERROR("sql.sql", "Table `(game_event_)npc_vendor` has `maxcount`=0 for item %u of vendor (Entry: %u) but `incrtime`<>0, ignore", id, vendor_entry);
            return false;
        }
    }

    VendorItemData const* vItems = GetNpcVendorItemList(vendor_entry);
    if (!vItems)
        return true;                                        // later checks for non-empty lists

    if (vItems->FindItemCostPair(id, ExtendedCost, type))
    {
        if (player)
            ChatHandler(player).PSendSysMessage(LANG_ITEM_ALREADY_IN_LIST, id, ExtendedCost, type);
        else
            TC_LOG_ERROR("sql.sql", "Table `npc_vendor` has duplicate items %u (with extended cost %u, type %u) for vendor (Entry: %u), ignoring", id, ExtendedCost, type, vendor_entry);
        return false;
    }

    if (vItems->GetItemCount() >= MAX_VENDOR_ITEMS) // FIXME: GetItemCount range 0...255 MAX_VENDOR_ITEMS = 300
    {
        if (player)
            ChatHandler(player).SendSysMessage(LANG_COMMAND_ADDVENDORITEMITEMS);
        else
            TC_LOG_ERROR("sql.sql", "Table `npc_vendor` has too many items (%u >= %i) for vendor (Entry: %u), ignore", vItems->GetItemCount(), MAX_VENDOR_ITEMS, vendor_entry);
        return false;
    }

    if (type == ITEM_VENDOR_TYPE_CURRENCY && maxcount == 0)
    {
        TC_LOG_ERROR("sql.sql", "Table `(game_event_)npc_vendor` have Item (Entry: %u, type: %u) with missing maxcount for vendor (%u), ignore", id, type, vendor_entry);
        return false;
    }

    return true;
}

void ObjectMgr::LoadScriptNames()
{
    uint32 oldMSTime = getMSTime();

    _scriptNamesStore.push_back("");
    QueryResult result = WorldDatabase.Query(
      "SELECT DISTINCT(ScriptName) FROM achievement_criteria_data WHERE ScriptName <> '' AND type = 11 "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM battleground_template WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM creature_template WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM gameobject_template WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM item_script_names WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM areatrigger_scripts WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM spell_script_names WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM transports WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM game_weather WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM conditions WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM outdoorpvp_template WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(script) FROM instance_template WHERE script <> ''"
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM creature_script_names WHERE ScriptName <> ''"
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM spell_areatrigger_template WHERE ScriptName <> ''");

    if (!result)
    {

        TC_LOG_ERROR("sql.sql", ">> Loaded empty set of Script Names!");
        return;
    }

    uint32 count = 1;

    do
    {
        _scriptNamesStore.push_back((*result)[0].GetString());
        ++count;
    }
    while (result->NextRow());

    std::sort(_scriptNamesStore.begin(), _scriptNamesStore.end());
    TC_LOG_INFO("server.loading", ">> Loaded %d Script Names in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

uint32 ObjectMgr::GetScriptId(const char *name)
{
    // use binary search to find the script name in the sorted vector
    // assume "" is the first element
    if (!name)
        return 0;

    ScriptNameContainer::const_iterator itr = std::lower_bound(_scriptNamesStore.begin(), _scriptNamesStore.end(), name);
    if (itr == _scriptNamesStore.end() || *itr != name)
        return 0;

    return uint32(itr - _scriptNamesStore.begin());
}

void ObjectMgr::CheckScripts(ScriptsType type, std::set<int32>& ids)
{
    ScriptMapMap* scripts = GetScriptsMapByType(type);
    if (!scripts)
        return;

    for (ScriptMapMap::const_iterator itrMM = scripts->begin(); itrMM != scripts->end(); ++itrMM)
    {
        for (ScriptMap::const_iterator itrM = itrMM->second.begin(); itrM != itrMM->second.end(); ++itrM)
        {
            switch (itrM->second.command)
            {
                case SCRIPT_COMMAND_TALK:
                {
                    if (!GetTrinityStringLocale(itrM->second.Talk.TextID))
                    {
                        TC_LOG_ERROR("sql.sql", "Table `%s` references invalid text id %u from `db_script_string`, script id: %u.",
                                     GetScriptsTableNameByType(type), itrM->second.Talk.TextID, itrMM->first);
                    }

                    if (ids.find(itrM->second.Talk.TextID) != ids.end())
                        ids.erase(itrM->second.Talk.TextID);
                }
                default:
                    break;
            }
        }
    }
}

void ObjectMgr::LoadDbScriptStrings()
{
    LoadTrinityStrings("db_script_string", MIN_DB_SCRIPT_STRING_ID, MAX_DB_SCRIPT_STRING_ID);

    std::set<int32> ids;

    for (int32 i = MIN_DB_SCRIPT_STRING_ID; i < MAX_DB_SCRIPT_STRING_ID; ++i)
        if (GetTrinityStringLocale(i))
            ids.insert(i);

    for (int type = SCRIPTS_FIRST; type <= SCRIPTS_LAST; ++type)
        CheckScripts(ScriptsType(type), ids);

    for (std::set<int32>::const_iterator itr = ids.begin(); itr != ids.end(); ++itr)
        TC_LOG_ERROR("sql.sql", "Table `db_script_string` has unused string id  %u", *itr);
}

bool LoadTrinityStrings(const char* table, int32 start_value, int32 end_value)
{
    // MAX_DB_SCRIPT_STRING_ID is max allowed negative value for scripts (scrpts can use only more deep negative values
    // start/end reversed for negative values
    if (start_value > MAX_DB_SCRIPT_STRING_ID || end_value >= start_value)
    {
        TC_LOG_ERROR("sql.sql", "Table '%s' load attempted with range (%d - %d) reserved by Trinity, strings not loaded.", table, start_value, end_value+1);
        return false;
    }

    return sObjectMgr->LoadTrinityStrings(table, start_value, end_value);
}

CreatureBaseStats const* ObjectMgr::GetCreatureBaseStats(uint8 level, uint8 unitClass)
{
    CreatureBaseStatsContainer::const_iterator it = _creatureBaseStatsStore.find(MAKE_PAIR16(level, unitClass));

    if (it != _creatureBaseStatsStore.end())
        return &(it->second);

    struct DefaultCreatureBaseStats : public CreatureBaseStats
    {
        DefaultCreatureBaseStats()
        {
            BaseArmor = 1;
            for (uint8 j = 0; j < MAX_CREATURE_BASE_HP; ++j)
                BaseHealth[j] = 1;
            BaseMana = 0;
        }
    };
    static const DefaultCreatureBaseStats def_stats;
    return &def_stats;
}

void ObjectMgr::LoadCreatureClassLevelStats()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT level, class, basehp0, basehp1, basehp2, basehp3, basehp4, basemana, basearmor FROM creature_classlevelstats");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 creature base stats. DB table `creature_classlevelstats` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint8 index = 0;

        uint8 Level = fields[index++].GetInt8();
        uint8 Class = fields[index++].GetInt8();

        CreatureBaseStats stats;

        for (uint8 i = 0; i < MAX_CREATURE_BASE_HP; ++i)
            stats.BaseHealth[i] = fields[index++].GetUInt32();

        stats.BaseMana = fields[index++].GetUInt32();
        stats.BaseArmor = fields[index++].GetUInt32();

        if (!Class || ((1 << (Class - 1)) & CLASSMASK_ALL_CREATURES) == 0)
            TC_LOG_ERROR("sql.sql", "Creature base stats for level %u has invalid class %u", Level, Class);

        for (uint8 i = 0; i < MAX_CREATURE_BASE_HP; ++i)
        {
            if (stats.BaseHealth[i] < 1)
            {
                TC_LOG_ERROR("sql.sql", "Creature base stats for class %u, level %u has invalid zero base HP[%u] - set to 1", Class, Level, i);
                stats.BaseHealth[i] = 1;
            }
        }

        _creatureBaseStatsStore[MAKE_PAIR16(Level, Class)] = stats;

        ++count;
    }
    while (result->NextRow());

    CreatureTemplateContainer const* ctc = sObjectMgr->GetCreatureTemplates();
    for (CreatureTemplateContainer::const_iterator itr = ctc->begin(); itr != ctc->end(); ++itr)
    {
        for (uint16 lvl = itr->second.minlevel; lvl <= itr->second.maxlevel; ++lvl)
        {
            if (_creatureBaseStatsStore.find(MAKE_PAIR16(lvl, itr->second.unit_class)) == _creatureBaseStatsStore.end())
                TC_LOG_ERROR("sql.sql", "Missing base stats for creature class %u level %u", itr->second.unit_class, lvl);
        }
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u creature base stats in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadFactionChangeAchievements()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT alliance_id, horde_id FROM player_factionchange_achievement");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 faction change achievement pairs. DB table `player_factionchange_achievement` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 alliance = fields[0].GetUInt32();
        uint32 horde = fields[1].GetUInt32();

        if (!sAchievementStore.LookupEntry(alliance))
            TC_LOG_ERROR("sql.sql", "Achievement %u referenced in `player_factionchange_achievement` does not exist, pair skipped!", alliance);
        else if (!sAchievementStore.LookupEntry(horde))
            TC_LOG_ERROR("sql.sql", "Achievement %u referenced in `player_factionchange_achievement` does not exist, pair skipped!", horde);
        else
            FactionChangeAchievements[alliance] = horde;

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u faction change achievement pairs in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadFactionChangeItems()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT alliance_id, horde_id FROM player_factionchange_items");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 faction change item pairs. DB table `player_factionchange_items` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 alliance = fields[0].GetUInt32();
        uint32 horde = fields[1].GetUInt32();

        if (!GetItemTemplate(alliance))
            TC_LOG_ERROR("sql.sql", "Item %u referenced in `player_factionchange_items` does not exist, pair skipped!", alliance);
        else if (!GetItemTemplate(horde))
            TC_LOG_ERROR("sql.sql", "Item %u referenced in `player_factionchange_items` does not exist, pair skipped!", horde);
        else
            FactionChangeItems[alliance] = horde;

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u faction change item pairs in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadFactionChangeSpells()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT alliance_id, horde_id FROM player_factionchange_spells");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 faction change spell pairs. DB table `player_factionchange_spells` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 alliance = fields[0].GetUInt32();
        uint32 horde = fields[1].GetUInt32();

        if (!sSpellMgr->GetSpellInfo(alliance))
            TC_LOG_ERROR("sql.sql", "Spell %u referenced in `player_factionchange_spells` does not exist, pair skipped!", alliance);
        else if (!sSpellMgr->GetSpellInfo(horde))
            TC_LOG_ERROR("sql.sql", "Spell %u referenced in `player_factionchange_spells` does not exist, pair skipped!", horde);
        else
            FactionChangeSpells[alliance] = horde;

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u faction change spell pairs in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadFactionChangeQuests()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT alliance_id, horde_id FROM player_factionchange_quests");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 faction change quest pairs. DB table `player_factionchange_quests` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 alliance = fields[0].GetUInt32();
        uint32 horde = fields[1].GetUInt32();

        if (!GetQuestTemplate(alliance))
            TC_LOG_ERROR("sql.sql", "Quest %u (alliance_id) referenced in `player_factionchange_quests` does not exist, pair skipped!", alliance);
        else if (!GetQuestTemplate(horde))
            TC_LOG_ERROR("sql.sql", "Quest %u (horde_id) referenced in `player_factionchange_quests` does not exist, pair skipped!", horde);
        else
            FactionChangeQuests[alliance] = horde;

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u faction change quest pairs in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadFactionChangeReputations()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT alliance_id, horde_id FROM player_factionchange_reputations");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 faction change reputation pairs. DB table `player_factionchange_reputations` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 alliance = fields[0].GetUInt32();
        uint32 horde = fields[1].GetUInt32();

        if (!sFactionStore.LookupEntry(alliance))
            TC_LOG_ERROR("sql.sql", "Reputation %u referenced in `player_factionchange_reputations` does not exist, pair skipped!", alliance);
        else if (!sFactionStore.LookupEntry(horde))
            TC_LOG_ERROR("sql.sql", "Reputation %u referenced in `player_factionchange_reputations` does not exist, pair skipped!", horde);
        else
            FactionChangeReputation[alliance] = horde;

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u faction change reputation pairs in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadFactionChangeTitles()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT alliance_id, horde_id FROM player_factionchange_titles");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 faction change title pairs. DB table `player_factionchange_title` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 alliance = fields[0].GetUInt32();
        uint32 horde = fields[1].GetUInt32();

        if (!sCharTitlesStore.LookupEntry(alliance))
            TC_LOG_ERROR("sql.sql", "Title %u referenced in `player_factionchange_title` does not exist, pair skipped!", alliance);
        else if (!sCharTitlesStore.LookupEntry(horde))
            TC_LOG_ERROR("sql.sql", "Title %u referenced in `player_factionchange_title` does not exist, pair skipped!", horde);
        else
            FactionChangeTitles[alliance] = horde;

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u faction change title pairs in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadHotfixData()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT entry, type, UNIX_TIMESTAMP(hotfixDate) FROM hotfix_data");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 hotfix info entries. DB table `hotfix_data` is empty.");
        return;
    }

    uint32 count = 0;

    _hotfixData.reserve(result->GetRowCount());

    do
    {
        Field* fields = result->Fetch();

        HotfixInfo info;
        info.Entry = fields[0].GetUInt32();
        info.Type = fields[1].GetUInt32();
        info.Timestamp = fields[2].GetUInt64();
        _hotfixData.push_back(info);
        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u hotfix info entries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadFactionChangeRewardedRacials()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT race, spell FROM player_factionchange_rewarded_racials");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 faction change rewarded racials. DB table `player_factionchange_rewarded_racials` is empty.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint8 race = fields[0].GetUInt8();
        uint32 spell = fields[1].GetUInt32();

        FactionChangeRewardedRacials.insert(RewardedRacialMap::value_type(race, spell));
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded " SIZEFMTD " faction change rewarded racials in %u ms",
                FactionChangeRewardedRacials.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadPhaseDefinitions()
{
    _PhaseDefinitionStore.clear();

    uint32 oldMSTime = getMSTime();

    //                                                 0       1       2         3            4         5               6
    QueryResult result = WorldDatabase.Query("SELECT zoneId, entry, phasemask, phaseId, terrainswapmap, worldMapAreaId, flags FROM `phase_definitions` ORDER BY `entry` ASC");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 phasing definitions. DB table `phase_definitions` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field *fields = result->Fetch();

        PhaseDefinition pd;

        pd.zoneId                = fields[0].GetUInt32();
        pd.entry                 = fields[1].GetUInt16();
        pd.phasemask             = fields[2].GetUInt32();
        pd.phaseId               = fields[3].GetUInt16();
        pd.terrainswapmap        = fields[4].GetUInt16();
        pd.worldMapAreaId        = fields[5].GetUInt16();
        pd.flags                 = fields[6].GetUInt8();

        // Checks
        if ((pd.flags & PHASE_FLAG_OVERWRITE_EXISTING) && (pd.flags & PHASE_FLAG_NEGATE_PHASE))
        {
            TC_LOG_ERROR("sql.sql", "Flags defined in phase_definitions in zoneId %d and entry %u does contain PHASE_FLAG_OVERWRITE_EXISTING and PHASE_FLAG_NEGATE_PHASE. Setting flags to PHASE_FLAG_OVERWRITE_EXISTING", pd.zoneId, pd.entry);
            pd.flags &= ~PHASE_FLAG_NEGATE_PHASE;
        }

        _PhaseDefinitionStore[pd.zoneId].push_back(pd);

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u phasing definitions in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadSpellPhaseInfo()
{
    _SpellPhaseStore.clear();

    uint32 oldMSTime = getMSTime();

    //                                               0       1            2
    QueryResult result = WorldDatabase.Query("SELECT id, phasemask, terrainswapmap FROM `spell_phase`");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 spell dbc infos. DB table `spell_phase` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field *fields = result->Fetch();

        SpellPhaseInfo spellPhaseInfo;
        spellPhaseInfo.spellId                = fields[0].GetUInt32();

        SpellInfo const* spell = sSpellMgr->GetSpellInfo(spellPhaseInfo.spellId);
        if (!spell)
        {
            TC_LOG_ERROR("sql.sql", "Spell %u defined in `spell_phase` does not exists, skipped.", spellPhaseInfo.spellId);
            continue;
        }

        if (!spell->HasAura(SPELL_AURA_PHASE))
        {
            TC_LOG_ERROR("sql.sql", "Spell %u defined in `spell_phase` does not have aura effect type SPELL_AURA_PHASE, useless value.", spellPhaseInfo.spellId);
            continue;
        }

        spellPhaseInfo.phasemask              = fields[1].GetUInt32();
        spellPhaseInfo.terrainswapmap         = fields[2].GetUInt32();

        _SpellPhaseStore[spellPhaseInfo.spellId] = spellPhaseInfo;

        ++count;
    }
    while (result->NextRow());
    TC_LOG_INFO("server.loading", ">> Loaded %u spell dbc infos in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}


GameObjectTemplate const* ObjectMgr::GetGameObjectTemplate(uint32 entry)
{
    GameObjectTemplateContainer::const_iterator itr = _gameObjectTemplateStore.find(entry);
    if (itr != _gameObjectTemplateStore.end())
        return &(itr->second);

    return NULL;
}

CreatureTemplate const * ObjectMgr::GetCreatureTemplate(uint32 entry) const
{
    CreatureTemplateContainer::const_iterator itr = _creatureTemplateStore.find(entry);
    if (itr != _creatureTemplateStore.end())
        return &itr->second;

    return NULL;
}

AreaTriggerTemplate const* ObjectMgr::GetAreaTriggerTemplate(uint32 entry) const
{
    AreaTriggerTemplateContainer::const_iterator iter = _areaTriggerTemplateStore.find(entry);
    return (iter != _areaTriggerTemplateStore.end()) ? &iter->second : NULL;
}

CreatureTemplate * ObjectMgr::GetMutableCreatureTemplate(uint32 entry)
{
    CreatureTemplateContainer::iterator itr = _creatureTemplateStore.find(entry);
    if (itr != _creatureTemplateStore.end())
        return &itr->second;

    return NULL;
}

VehicleAccessoryList const* ObjectMgr::GetVehicleAccessoryList(Vehicle* veh) const
{
    if (Creature* cre = veh->GetBase()->ToCreature())
    {
        // Give preference to GUID-based accessories
        VehicleAccessoryContainer::const_iterator itr = _vehicleAccessoryStore.find(cre->GetDBTableGUIDLow());
        if (itr != _vehicleAccessoryStore.end())
            return &itr->second;
    }

    // Otherwise return entry-based
    VehicleAccessoryContainer::const_iterator itr = _vehicleTemplateAccessoryStore.find(veh->GetCreatureEntry());
    if (itr != _vehicleTemplateAccessoryStore.end())
        return &itr->second;
    return NULL;
}

void ObjectMgr::LoadResearchSiteZones()
{
    QueryResult result = WorldDatabase.Query("SELECT id, position_x, position_y, zone FROM research_site");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 research site zones. DB table `research_site` is empty.");
        return;
    }

    uint32 counter = 0;

    do
    {
        Field *fields = result->Fetch();

        uint32 siteId = 0;
        uint32 mapId = 0;
        uint32 POIid = fields[0].GetUInt32();
        uint32 zoneId = fields[3].GetUInt16();

        bool bFound = false;
        for (std::set<ResearchSiteEntry const*>::const_iterator itr = sResearchSiteSet.begin(); itr != sResearchSiteSet.end(); ++itr)
            if ((*itr)->POIid == POIid)
            {
                bFound = true;
                siteId = (*itr)->ID;
                mapId = (*itr)->mapId;
                break;
            }
        if (!bFound)
            continue;

        ResearchZoneEntry &ptr = _researchZoneMap[siteId];
        ptr.coords.push_back(ResearchPOIPoint(fields[1].GetInt32(), fields[2].GetInt32()));
        ptr.map = mapId;
        ptr.zone = zoneId;
        ptr.level = 0;
        for (uint32 i = 0; i < sAreaStore.GetNumRows(); ++i)
        {
            AreaTableEntry const* area = sAreaStore.LookupEntry(i);
            if (!area)
                continue;

            if (area->mapid == ptr.map && area->zone == ptr.zone)
            {
                ptr.level = area->area_level;
                break;
            }
        }
        ++counter;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u research site zones.", counter);
}

void ObjectMgr::LoadResearchSiteLoot()
{
    QueryResult result = WorldDatabase.Query("SELECT site_id, x, y, z, race FROM research_loot");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 research loot. DB table `research_loot` is empty.");
        return;
    }

    uint32 counter = 0;

    do
    {
        ResearchLootEntry dg;
        {
            Field *fields = result->Fetch();

            dg.id = uint16(fields[0].GetUInt32());
            dg.x = fields[1].GetFloat();
            dg.y = fields[2].GetFloat();
            dg.z = fields[3].GetFloat();
            dg.race = fields[4].GetUInt8();
        }

        _researchLoot.push_back(dg);

        ++counter;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u research site loot.", counter);
}

void ObjectMgr::LoadItemExtendedCost()
{
    QueryResult result = WorldDatabase.PQuery("SELECT ID, RequiredArenaSlot, RequiredItem1, RequiredItem2, RequiredItem3, RequiredItem4, RequiredItem5, RequiredItemCount1, RequiredItemCount2, RequiredItemCount3, RequiredItemCount4, RequiredItemCount5,RequiredPersonalArenaRating, RequiredCurrency1, RequiredCurrency2, RequiredCurrency3, RequiredCurrency4, RequiredCurrency5, RequiredCurrencyCount1, RequiredCurrencyCount2, RequiredCurrencyCount3, RequiredCurrencyCount4, RequiredCurrencyCount5 FROM item_extended_cost");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 item extended cost info. DB table `item_extended_cost` is empty.");
        return;
    }

    uint32 counter = 0;

    do
    {
        Field* field = result->Fetch();
        int index = 0;
        counter++;

        ItemExtendedCostEntry* extendedCost = new ItemExtendedCostEntry();
        extendedCost->ID = field[index++].GetUInt32();
        extendedCost->RequiredArenaSlot = field[index++].GetUInt32();

        for (uint32 i = 0; i < MAX_ITEM_EXT_COST_ITEMS; i++)
            extendedCost->RequiredItem[i] = field[index++].GetUInt32();

        for (uint32 i = 0; i < MAX_ITEM_EXT_COST_ITEMS; i++)
            extendedCost->RequiredItemCount[i] = field[index++].GetUInt32();

        extendedCost->RequiredPersonalArenaRating = field[index++].GetUInt32();

        for (uint32 i = 0; i < MAX_ITEM_EXT_COST_CURRENCIES; i++)
            extendedCost->RequiredCurrency[i] = field[index++].GetUInt32();

        for (uint32 i = 0; i < MAX_ITEM_EXT_COST_CURRENCIES; i++)
            extendedCost->RequiredCurrencyCount[i] = field[index++].GetUInt32();

        sItemExtendedCostStore.EraseEntry(extendedCost->ID);
        sItemExtendedCostStore.AddEntry(extendedCost->ID, (const ItemExtendedCostEntry*)extendedCost);
        _overwriteExtendedCosts.insert(extendedCost->ID);

    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u item extended cost info.", counter);
}

void ObjectMgr::LoadGuildChallengeRewardInfo()
{
    uint32 oldMSTime = getMSTime();
    QueryResult result = WorldDatabase.Query("SELECT Type, Experience, Gold, Gold2, Count FROM guild_challenge_reward");
    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 guild challenge reward data.");
        return;
    }

    _challengeRewardData.reserve(result->GetRowCount());

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 type = fields[0].GetUInt32();
        if (type >= CHALLENGE_MAX)
        {
            TC_LOG_INFO("server.loading", ">> guild_challenge_reward has unknown challenge type %u, skip.", type);
            continue;
        }

        GuildChallengeReward reward;
        {
            reward.Experience = fields[1].GetUInt32();
            reward.Gold = fields[2].GetUInt32();
            reward.Gold2 = fields[3].GetUInt32();
            reward.ChallengeCount = fields[4].GetUInt32();
        }

        _challengeRewardData.push_back(reward);
        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u guild challenge reward data in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::ReplaceCreatureTemplate(uint32 entry, CreatureTemplate const &newTemplate)
{
    CreatureTemplate &currTemplate = _creatureTemplateStore[entry];

    currTemplate = newTemplate;
    FixCreatureTemplate(currTemplate);
}

void ObjectMgr::LoadCreatureAddonPaths(char const *keyName, char const *tableName, CreatureAddonContainer &addonStorage)
{
    QueryResult result = WorldDatabase.PQuery("SELECT `%s`, `path` FROM `%s`", keyName, tableName);
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();
        addonStorage[entry].path_id = fields[1].GetUInt32();
    } while (result->NextRow());
}

void ObjectMgr::LoadCreatureAddonMounts(char const *keyName, char const *tableName, CreatureAddonContainer &addonStorage)
{
    QueryResult result = WorldDatabase.PQuery("SELECT `%s`, `mount` FROM `%s`", keyName, tableName);
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();
        addonStorage[entry].mount = fields[1].GetUInt32();
    } while (result->NextRow());
}

void ObjectMgr::LoadCreatureAddonBytes(char const *keyName, char const *tableName, CreatureAddonContainer &addonStorage)
{
    QueryResult result = WorldDatabase.PQuery("SELECT `%s`, `index`, `bytes` FROM `%s`", keyName, tableName);
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();
        uint8 index = fields[1].GetUInt8();

        addonStorage[entry].bytes[index] = fields[2].GetUInt32();
    } while (result->NextRow());
}

void ObjectMgr::LoadCreatureAddonEmotes(char const *keyName, char const *tableName, CreatureAddonContainer &addonStorage)
{
    QueryResult result = WorldDatabase.PQuery("SELECT `%s`, `emote` FROM `%s`", keyName, tableName);
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();
        addonStorage[entry].emote = fields[1].GetUInt32();
    } while (result->NextRow());
}

void ObjectMgr::LoadCreatureAddonAuras(char const *keyName, char const *tableName, CreatureAddonContainer &addonStorage)
{
    QueryResult result = WorldDatabase.PQuery("SELECT `%s`, `aura` FROM `%s`", keyName, tableName);
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();
        addonStorage[entry].auras.push_back(fields[1].GetUInt32());
    } while (result->NextRow());
}

void ObjectMgr::LoadCreatureTemplateCurrency()
{
    QueryResult result = WorldDatabase.Query("SELECT entry, currencyId, currencyCount FROM creature_template_currency");

    if (!result)
    {
        TC_LOG_INFO("server.loading", "Loaded 0 creature currencies, creature_template_currency is empty");
        return;
    }

    do
    {
        Field const * const fields = result->Fetch();

        auto const entry = fields[0].GetUInt32();
        auto const currencyId = fields[1].GetUInt32();
        auto const currencyCount = fields[2].GetUInt32();

        auto const currencyEntry = sCurrencyTypesStore.LookupEntry(currencyId);
        if (!currencyEntry)
        {
            TC_LOG_ERROR("server.loading", "creature_template_currency contains nonexistent currency %u", entry);
            continue;
        }

        CreatureCurrency currency;
        currency.currency = currencyEntry;
        currency.currencyCount = currencyCount;

        _creatureCurrencyStore.insert(std::make_pair(entry, currency));
    }
    while (result->NextRow());

    TC_LOG_INFO("misc", ">> Loaded " SIZEFMTD " Creature Currencies", _creatureCurrencyStore.size());
}

void ObjectMgr::LoadParryToPercentValues()
{
    QueryResult result = WorldDatabase.Query("SELECT class_level, perc_cap FROM parry_to_percent");

    if (!result)
    {
        TC_LOG_INFO("server.loading", "Loaded 0 parry conversions, parry_to_percent is empty");
        return;
    }

    do
    {
        Field const * const fields = result->Fetch();

        auto const classLevel = fields[0].GetUInt32();
        auto const percCap = fields[1].GetUInt32();

        _parryToPercentStore.insert(std::make_pair(classLevel, percCap));
    } while (result->NextRow());
}

float ObjectMgr::GetParryCapForClassLevel(uint32 classLevel)
{
    if (_parryToPercentStore.empty() || _parryToPercentStore.size() < classLevel)
        return 0.0f;

    return _parryToPercentStore[classLevel];
}

void ObjectMgr::LoadDodgeToPercentValues()
{
    QueryResult result = WorldDatabase.Query("SELECT class_level, perc_cap FROM dodge_to_percent");

    if (!result)
    {
        TC_LOG_INFO("server.loading", "Loaded 0 parry conversions, dodge_to_percent is empty");
        return;
    }

    do
    {
        Field const * const fields = result->Fetch();

        auto const classLevel = fields[0].GetUInt32();
        auto const percCap = fields[1].GetUInt32();

        _dodgeToPercentStore.insert(std::make_pair(classLevel, percCap));
    } while (result->NextRow());
}

float ObjectMgr::GetDodgeCapForClassLevel(uint32 classLevel)
{
    if (_dodgeToPercentStore.empty() || _dodgeToPercentStore.size() < classLevel)
        return 0.0f;

    return _dodgeToPercentStore[classLevel];
}

// this allows calculating base reputations to offline players, just by race and class
int32 ObjectMgr::GetBaseReputationOf(FactionEntry const* factionEntry, uint8 race, uint8 playerClass) const
{
    if (!factionEntry)
        return 0;

    uint32 raceMask = (1 << (race - 1));
    uint32 classMask = (1 << (playerClass-1));

    for (int i = 0; i < 4; i++)
    {
        if ((!factionEntry->BaseRepClassMask[i] ||
            factionEntry->BaseRepClassMask[i] & classMask) &&
            (!factionEntry->BaseRepRaceMask[i] ||
            factionEntry->BaseRepRaceMask[i] & raceMask))
            return factionEntry->BaseRepValue[i];
    }

    return 0;
}

QuestObjective const* ObjectMgr::GetQuestObjective(uint32 objectiveId) const
{
    auto citr = m_questObjectives.find(objectiveId);
    if (citr == m_questObjectives.end())
        return nullptr;

    return citr->second;
}

LFRLootBind const* ObjectMgr::GetLFRLootBind(uint32 id) const
{
    for (auto const &lockEntry : m_lfrLootBinds)
        if (lockEntry->Id == id)
            return lockEntry;

    return nullptr;
}

LFRLootBind const* ObjectMgr::GetLFRLootBind(uint32 entry, uint8 type) const
{
    for (auto const &lockEntry : m_lfrLootBinds)
        if (lockEntry->Entry == entry && lockEntry->Type == type)
            return lockEntry;

    return nullptr;
}

LFRLootBindSet const& ObjectMgr::GetLFRLootBindLinked(uint8 linkGroup)
{
    return m_lfrLootBindsLinked[linkGroup];
}

void ObjectMgr::LoadBattlePetBreedData()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT species, breed FROM `battle_pet_breed`");
    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 Battle Pets breed definitions. DB table `battle_pet_breed` is empty!");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        uint16 speciesId = fields[0].GetUInt16();
        uint8 breedId    = fields[1].GetUInt8();

        if (!sBattlePetSpeciesStore.LookupEntry(speciesId))
        {
            TC_LOG_ERROR("sql.sql", "Battle Pet species %u defined in `battle_pet_breed` does not exists, skipped!", speciesId);
            continue;
        }

        if (breedId > BATTLE_PET_MAX_BREED)
        {
            TC_LOG_ERROR("sql.sql", "Battle Pet breed %u defined in `battle_pet_breed` is not valid, skipped!", breedId);
        }

        m_battlePetBreedXSpeciesStore[speciesId].insert(breedId);
        count++;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u Battle Pet breed definitions in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadBattlePetQualityData()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT species, quality FROM `battle_pet_quality`");
    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 Battle Pets quality definitions. DB table `battle_pet_quality` is empty!");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        uint16 speciesId = fields[0].GetUInt16();
        uint8 quality    = fields[1].GetUInt8();

        if (!sBattlePetSpeciesStore.LookupEntry(speciesId))
        {
            TC_LOG_ERROR("sql.sql", "Battle Pet species %u defined in `battle_pet_quality` does not exists, skipped!", speciesId);
            continue;
        }

        if (quality > ITEM_QUALITY_LEGENDARY)
        {
            TC_LOG_ERROR("sql.sql", "Battle Pet quality %u defined in `battle_pet_quality` is invalid, skipped!", quality);
            continue;
        }

        m_battlePetQualityXSpeciesStore[speciesId].insert(quality);
        count++;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u Battle Pet quality definitions in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

uint8 ObjectMgr::BattlePetGetRandomBreed(uint16 speciesId) const
{
    auto breedMap = m_battlePetBreedXSpeciesStore.find(speciesId);
    if (breedMap != m_battlePetBreedXSpeciesStore.end())
    {
        auto breedEntry(breedMap->second.begin());
        std::advance(breedEntry, urand(0, breedMap->second.size() - 1));
        return *breedEntry;
    }

    return 0;
}

uint8 ObjectMgr::BattlePetGetRandomQuality(uint16 speciesId) const
{
    auto qualityMap = m_battlePetQualityXSpeciesStore.find(speciesId);
    if (qualityMap != m_battlePetQualityXSpeciesStore.end())
    {
        auto qualityEntry(qualityMap->second.begin());
        std::advance(qualityEntry, urand(0, qualityMap->second.size() - 1));
        return *qualityEntry;
    }

    return ITEM_QUALITY_NORMAL;
}

void ObjectMgr::LoadBattlePetItemToSpeciesData()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT itemId, species FROM `battle_pet_item_to_species`");
    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 Battle Pets item to species definitions. DB table `battle_pet_item_to_species` is empty!");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        uint32 itemId    = fields[0].GetUInt32();
        uint16 speciesId = fields[1].GetUInt16();

        if (!GetItemTemplate(itemId))
        {
            TC_LOG_ERROR("sql.sql", "Item id %u defined in `battle_pet_item_to_species` does not exists, skipped!", itemId);
            continue;
        }

        if (!sBattlePetSpeciesStore.LookupEntry(speciesId))
        {
            TC_LOG_ERROR("sql.sql", "Battle Pet species %u defined in `battle_pet_item_to_species` does not exists, skipped!", speciesId);
            continue;
        }

        m_battlePetItemToSpeciesStore[itemId] = speciesId;
        count++;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u Battle Pet quality definitions in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

uint16 ObjectMgr::BattlePetGetSpeciesFromItem(uint32 itemId) const
{
    auto itemSpeciesMap = m_battlePetItemToSpeciesStore.find(itemId);
    if (itemSpeciesMap == m_battlePetItemToSpeciesStore.end())
        return 0;

    return itemSpeciesMap->second;
}
