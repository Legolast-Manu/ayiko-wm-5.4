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

#include "Common.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Opcodes.h"
#include "Log.h"
#include "UpdateMask.h"
#include "World.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "Player.h"
#include "Pet.h"
#include "Unit.h"
#include "Totem.h"
#include "Spell.h"
#include "DynamicObject.h"
#include "Group.h"
#include "UpdateData.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "CellImpl.h"
#include "SharedDefines.h"
#include "LootMgr.h"
#include "VMapFactory.h"
#include "Battleground.h"
#include "Util.h"
#include "TemporarySummon.h"
#include "Vehicle.h"
#include "SpellAuraEffects.h"
#include "ScriptMgr.h"
#include "ConditionMgr.h"
#include "DisableMgr.h"
#include "SpellScript.h"
#include "InstanceScript.h"
#include "SpellInfo.h"
#include "DB2Stores.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "GuildMgr.h"

extern pEffect SpellEffects[TOTAL_SPELL_EFFECTS];

#define MAX_SPELL_QUEUE_GCD 400

SpellDestination::SpellDestination()
{
    _position.Relocate(0, 0, 0, 0);
    _transportGUID = 0;
    _transportOffset.Relocate(0, 0, 0, 0);
}

SpellDestination::SpellDestination(float x, float y, float z, float orientation, uint32 mapId)
{
    _position.Relocate(x, y, z, orientation);
    _transportGUID = 0;
    _position.m_mapId = mapId;
}

SpellDestination::SpellDestination(Position const& pos)
{
    _position.Relocate(pos);
    _transportGUID = 0;
}

SpellDestination::SpellDestination(WorldObject const& wObj)
{
    _transportGUID = wObj.GetTransGUID();
    _transportOffset.Relocate(wObj.GetTransOffsetX(), wObj.GetTransOffsetY(), wObj.GetTransOffsetZ(), wObj.GetTransOffsetO());
    _position.Relocate(wObj);
    _position.SetOrientation(wObj.GetOrientation());
}


SpellCastTargets::SpellCastTargets() : m_elevation(0), m_speed(0), m_strTarget()
{
    m_objectTarget = NULL;
    m_itemTarget = NULL;

    m_objectTargetGUID   = 0;
    m_itemTargetGUID   = 0;
    m_itemTargetEntry  = 0;

    m_targetMask = 0;
}

SpellCastTargets::~SpellCastTargets()
{
}

void SpellCastTargets::Read(ByteBuffer& data, Unit* caster)
{
    data >> m_targetMask;

    if (m_targetMask == TARGET_FLAG_NONE)
        return;

    if (m_targetMask & (TARGET_FLAG_UNIT | TARGET_FLAG_UNIT_MINIPET | TARGET_FLAG_GAMEOBJECT | TARGET_FLAG_CORPSE_ENEMY | TARGET_FLAG_CORPSE_ALLY))
        data.readPackGUID(m_objectTargetGUID);

    if (m_targetMask & (TARGET_FLAG_ITEM | TARGET_FLAG_TRADE_ITEM))
        data.readPackGUID(m_itemTargetGUID);

    if (m_targetMask & TARGET_FLAG_SOURCE_LOCATION)
    {
        data.readPackGUID(m_src._transportGUID);
        if (m_src._transportGUID)
            data >> m_src._transportOffset.PositionXYZStream();
        else
            data >> m_src._position.PositionXYZStream();
    }
    else
    {
        m_src._transportGUID = caster->GetTransGUID();
        if (m_src._transportGUID)
            m_src._transportOffset.Relocate(caster->GetTransOffsetX(), caster->GetTransOffsetY(), caster->GetTransOffsetZ(), caster->GetTransOffsetO());
        else
            m_src._position.Relocate(caster);
    }

    if (m_targetMask & TARGET_FLAG_DEST_LOCATION)
    {
        data.readPackGUID(m_dst._transportGUID);
        if (m_dst._transportGUID)
            data >> m_dst._transportOffset.PositionXYZStream();
        else
            data >> m_dst._position.PositionXYZStream();
    }
    else
    {
        m_dst._transportGUID = caster->GetTransGUID();
        if (m_dst._transportGUID)
            m_dst._transportOffset.Relocate(caster->GetTransOffsetX(), caster->GetTransOffsetY(), caster->GetTransOffsetZ(), caster->GetTransOffsetO());
        else
            m_dst._position.Relocate(caster);
    }

    if (m_targetMask & TARGET_FLAG_STRING)
        data >> m_strTarget;

    Update(caster);
}

void SpellCastTargets::Write(ByteBuffer& data)
{
    data << uint32(m_targetMask);

    if (m_targetMask & (TARGET_FLAG_UNIT | TARGET_FLAG_CORPSE_ALLY | TARGET_FLAG_GAMEOBJECT | TARGET_FLAG_CORPSE_ENEMY | TARGET_FLAG_UNIT_MINIPET))
        data.appendPackGUID(m_objectTargetGUID);

    if (m_targetMask & (TARGET_FLAG_ITEM | TARGET_FLAG_TRADE_ITEM))
    {
        if (m_itemTarget)// 5.0.5 GetWoWGUID
            data.appendPackGUID(m_itemTarget->GetGUID());
        else
            data << uint64(0);
    }

    if (m_targetMask & TARGET_FLAG_SOURCE_LOCATION)
    {
        data.appendPackGUID(m_src._transportGUID); // relative position guid here - transport for example
        if (m_src._transportGUID)
            data << m_src._transportOffset.PositionXYZStream();
        else
            data << m_src._position.PositionXYZStream();
    }

    if (m_targetMask & TARGET_FLAG_DEST_LOCATION)
    {
        data.appendPackGUID(m_dst._transportGUID); // relative position guid here - transport for example
        if (m_dst._transportGUID)
            data << m_dst._transportOffset.PositionXYZStream();
        else
            data << m_dst._position.PositionXYZStream();
    }

    if (m_targetMask & TARGET_FLAG_STRING)
        data << m_strTarget;
}

uint64 SpellCastTargets::GetUnitTargetGUID() const
{
    switch (GUID_HIPART(m_objectTargetGUID))
    {
        case HIGHGUID_PLAYER:
        case HIGHGUID_VEHICLE:
        case HIGHGUID_UNIT:
        case HIGHGUID_PET:
            return m_objectTargetGUID;
        default:
            return 0LL;
    }
}

Unit* SpellCastTargets::GetUnitTarget() const
{
    if (m_objectTarget)
        return m_objectTarget->ToUnit();
    return NULL;
}

void SpellCastTargets::SetUnitTarget(Unit* target)
{
    if (!target)
        return;

    m_objectTarget = target;
    m_objectTargetGUID = target->GetGUID();
    m_targetMask |= TARGET_FLAG_UNIT;
}

uint64 SpellCastTargets::GetGOTargetGUID() const
{
    switch (GUID_HIPART(m_objectTargetGUID))
    {
        case HIGHGUID_TRANSPORT:
        case HIGHGUID_MO_TRANSPORT:
        case HIGHGUID_GAMEOBJECT:
            return m_objectTargetGUID;
        default:
            return 0LL;
    }
}

GameObject* SpellCastTargets::GetGOTarget() const
{
    if (m_objectTarget)
        return m_objectTarget->ToGameObject();
    return NULL;
}


void SpellCastTargets::SetGOTarget(GameObject* target)
{
    if (!target)
        return;

    m_objectTarget = target;
    m_objectTargetGUID = target->GetGUID();
    m_targetMask |= TARGET_FLAG_GAMEOBJECT;
}

uint64 SpellCastTargets::GetCorpseTargetGUID() const
{
    switch (GUID_HIPART(m_objectTargetGUID))
    {
        case HIGHGUID_CORPSE:
            return m_objectTargetGUID;
        default:
            return 0LL;
    }
}

Corpse* SpellCastTargets::GetCorpseTarget() const
{
    if (m_objectTarget)
        return m_objectTarget->ToCorpse();
    return NULL;
}

WorldObject* SpellCastTargets::GetObjectTarget() const
{
    return m_objectTarget;
}

uint64 SpellCastTargets::GetObjectTargetGUID() const
{
    return m_objectTargetGUID;
}

void SpellCastTargets::RemoveObjectTarget()
{
    m_objectTarget = NULL;
    m_objectTargetGUID = 0LL;
    m_targetMask &= ~(TARGET_FLAG_UNIT_MASK | TARGET_FLAG_CORPSE_MASK | TARGET_FLAG_GAMEOBJECT_MASK);
}

void SpellCastTargets::SetItemTarget(Item* item)
{
    if (!item)
        return;

    m_itemTarget = item;
    m_itemTargetGUID = item->GetGUID();
    m_itemTargetEntry = item->GetEntry();
    m_targetMask |= TARGET_FLAG_ITEM;
}

void SpellCastTargets::SetTradeItemTarget(Player* caster)
{
    m_itemTargetGUID = uint64(TRADE_SLOT_NONTRADED);
    m_itemTargetEntry = 0;
    m_targetMask |= TARGET_FLAG_TRADE_ITEM;

    Update(caster);
}

void SpellCastTargets::UpdateTradeSlotItem()
{
    if (m_itemTarget && (m_targetMask & TARGET_FLAG_TRADE_ITEM))
    {
        m_itemTargetGUID = m_itemTarget->GetGUID();
        m_itemTargetEntry = m_itemTarget->GetEntry();
    }
}

SpellDestination const* SpellCastTargets::GetSrc() const
{
    return &m_src;
}

Position const* SpellCastTargets::GetSrcPos() const
{
    return &m_src._position;
}

void SpellCastTargets::SetSrc(float x, float y, float z)
{
    m_src = SpellDestination(x, y, z);
    m_targetMask |= TARGET_FLAG_SOURCE_LOCATION;
}

void SpellCastTargets::SetSrc(Position const& pos)
{
    m_src = SpellDestination(pos);
    m_targetMask |= TARGET_FLAG_SOURCE_LOCATION;
}

void SpellCastTargets::SetSrc(WorldObject const& wObj)
{
    m_src = SpellDestination(wObj);
    m_targetMask |= TARGET_FLAG_SOURCE_LOCATION;
}

void SpellCastTargets::ModSrc(Position const& pos)
{
    ASSERT(m_targetMask & TARGET_FLAG_SOURCE_LOCATION);

    if (m_src._transportGUID)
    {
        Position offset;
        m_src._position.GetPositionOffsetTo(pos, offset);
        m_src._transportOffset.RelocateOffset(offset);
    }
    m_src._position.Relocate(pos);
}

void SpellCastTargets::RemoveSrc()
{
    m_targetMask &= ~(TARGET_FLAG_SOURCE_LOCATION);
}

SpellDestination const* SpellCastTargets::GetDst() const
{
    return &m_dst;
}

WorldLocation const* SpellCastTargets::GetDstPos() const
{
    return &m_dst._position;
}

void SpellCastTargets::SetDst(float x, float y, float z, float orientation, uint32 mapId)
{
    m_dst = SpellDestination(x, y, z, orientation, mapId);
    m_targetMask |= TARGET_FLAG_DEST_LOCATION;
}

void SpellCastTargets::SetDst(Position const& pos)
{
    m_dst = SpellDestination(pos);
    m_targetMask |= TARGET_FLAG_DEST_LOCATION;
}

void SpellCastTargets::SetDst(WorldObject const& wObj)
{
    m_dst = SpellDestination(wObj);
    m_targetMask |= TARGET_FLAG_DEST_LOCATION;
}

void SpellCastTargets::SetDst(SpellCastTargets const& spellTargets)
{
    m_dst = spellTargets.m_dst;
    m_targetMask |= TARGET_FLAG_DEST_LOCATION;
}

void SpellCastTargets::ModDst(Position const& pos)
{
    ASSERT(m_targetMask & TARGET_FLAG_DEST_LOCATION);

    if (m_dst._transportGUID)
    {
        Position offset;
        m_dst._position.GetPositionOffsetTo(pos, offset);
        m_dst._transportOffset.RelocateOffset(offset);
    }
    m_dst._position.Relocate(pos);
}

void SpellCastTargets::RemoveDst()
{
    m_targetMask &= ~(TARGET_FLAG_DEST_LOCATION);
}

void SpellCastTargets::Update(Unit* caster)
{
    m_objectTarget = m_objectTargetGUID ? ((m_objectTargetGUID == caster->GetGUID()) ? caster : ObjectAccessor::GetWorldObject(*caster, m_objectTargetGUID)) : NULL;

    m_itemTarget = NULL;
    if (caster->GetTypeId() == TYPEID_PLAYER)
    {
        Player* player = caster->ToPlayer();
        if (m_targetMask & TARGET_FLAG_ITEM)
            m_itemTarget = player->GetItemByGuid(m_itemTargetGUID);
        else if (m_targetMask & TARGET_FLAG_TRADE_ITEM)
            if (m_itemTargetGUID == TRADE_SLOT_NONTRADED) // here it is not guid but slot. Also prevents hacking slots
                if (TradeData* pTrade = player->GetTradeData())
                    m_itemTarget = pTrade->GetTraderData()->GetItem(TRADE_SLOT_NONTRADED);

        if (m_itemTarget)
            m_itemTargetEntry = m_itemTarget->GetEntry();
    }

    // update positions by transport move
    if (HasSrc() && m_src._transportGUID)
    {
        if (WorldObject* transport = ObjectAccessor::GetWorldObject(*caster, m_src._transportGUID))
        {
            m_src._position.Relocate(transport);
            m_src._position.RelocateOffset(m_src._transportOffset);
        }
    }

    if (HasDst() && m_dst._transportGUID)
    {
        if (WorldObject* transport = ObjectAccessor::GetWorldObject(*caster, m_dst._transportGUID))
        {
            m_dst._position.Relocate(transport);
            m_dst._position.RelocateOffset(m_dst._transportOffset);
        }
    }
}

void SpellCastTargets::OutDebug() const
{
    if (!m_targetMask)
        TC_LOG_INFO("spells", "No targets");

    TC_LOG_INFO("spells", "target mask: %u", m_targetMask);
    if (m_targetMask & (TARGET_FLAG_UNIT_MASK | TARGET_FLAG_CORPSE_MASK | TARGET_FLAG_GAMEOBJECT_MASK))
        TC_LOG_INFO("spells", "Object target: " UI64FMTD, m_objectTargetGUID);
    if (m_targetMask & TARGET_FLAG_ITEM)
        TC_LOG_INFO("spells", "Item target: " UI64FMTD, m_itemTargetGUID);
    if (m_targetMask & TARGET_FLAG_TRADE_ITEM)
        TC_LOG_INFO("spells", "Trade item target: " UI64FMTD, m_itemTargetGUID);
    if (m_targetMask & TARGET_FLAG_SOURCE_LOCATION)
        TC_LOG_INFO("spells", "Source location: transport guid:" UI64FMTD " trans offset: %s position: %s", m_src._transportGUID, m_src._transportOffset.ToString().c_str(), m_src._position.ToString().c_str());
    if (m_targetMask & TARGET_FLAG_DEST_LOCATION)
        TC_LOG_INFO("spells", "Destination location: transport guid:" UI64FMTD " trans offset: %s position: %s", m_dst._transportGUID, m_dst._transportOffset.ToString().c_str(), m_dst._position.ToString().c_str());
    if (m_targetMask & TARGET_FLAG_STRING)
        TC_LOG_INFO("spells", "String: %s", m_strTarget.c_str());
    TC_LOG_INFO("spells", "speed: %f", m_speed);
    TC_LOG_INFO("spells", "elevation: %f", m_elevation);
}

SpellValue::SpellValue(SpellInfo const* proto)
{
    for (uint32 i = 0; i < proto->Effects.size(); ++i)
        EffectBasePoints[i] = proto->Effects[i].BasePoints;
    MaxAffectedTargets = proto->MaxAffectedTargets;
    RadiusMod = 1.0f;
    AuraStackAmount = 1;
}

Spell::Spell(Unit* caster, SpellInfo const* info, TriggerCastFlags triggerFlags,
             uint64 originalCasterGUID, bool skipCheck, bool ignoreTriggeredAttribute) :
    m_spellInfo(sSpellMgr->GetSpellForDifficultyFromSpell(info, caster)),
    m_caster((info->AttributesEx6 & SPELL_ATTR6_CAST_BY_CHARMER && caster->GetCharmerOrOwner()) ? caster->GetCharmerOrOwner() : caster),
    m_spellValue(m_spellInfo)
{
    m_spellPowerData = m_caster->GetSpellPowerEntryBySpell(m_spellInfo);
    m_customError = SPELL_CUSTOM_ERROR_NONE;
    m_skipCheck = skipCheck;
    m_selfContainer = NULL;
    m_referencedFromCurrentSpell = false;
    m_executedCurrently = false;
    m_needComboPoints = m_spellInfo->NeedsComboPoints();
    m_comboPointGain = 0;
    m_delayStart = 0;
    m_delayAtDamageCount = 0;

    m_applyMultiplierMask = 0;
    m_auraScaleMask = 0;

    // Get data for type of attack
    switch (m_spellInfo->DmgClass)
    {
        case SPELL_DAMAGE_CLASS_MELEE:
            if (m_spellInfo->AttributesEx3 & SPELL_ATTR3_REQ_OFFHAND)
                m_attackType = OFF_ATTACK;
            else
                m_attackType = BASE_ATTACK;
            break;
        case SPELL_DAMAGE_CLASS_RANGED:
            m_attackType = m_spellInfo->IsRangedWeaponSpell() ? RANGED_ATTACK : BASE_ATTACK;
            break;
        default:
                                                            // Wands
            if (m_spellInfo->AttributesEx2 & SPELL_ATTR2_AUTOREPEAT_FLAG)
                m_attackType = RANGED_ATTACK;
            else
                m_attackType = BASE_ATTACK;
            break;
    }

    m_spellSchoolMask = info->GetSchoolMask();           // Can be override for some spell (wand shoot for example)

    if (m_attackType == RANGED_ATTACK)
        // wand case
        if ((m_caster->getClassMask() & CLASSMASK_WAND_USERS) != 0 && m_caster->GetTypeId() == TYPEID_PLAYER)
            if (Item* pItem = m_caster->ToPlayer()->GetWeaponForAttack(RANGED_ATTACK))
                m_spellSchoolMask = SpellSchoolMask(1 << pItem->GetTemplate()->DamageType);

    if (originalCasterGUID)
        m_originalCasterGUID = originalCasterGUID;
    else
        m_originalCasterGUID = m_caster->GetGUID();

    if (m_originalCasterGUID == m_caster->GetGUID())
        m_originalCaster = m_caster;
    else
    {
        m_originalCaster = ObjectAccessor::GetUnit(*m_caster, m_originalCasterGUID);
        if (m_originalCaster && !m_originalCaster->IsInWorld())
            m_originalCaster = NULL;
    }

    m_spellState = SPELL_STATE_NULL;
    _triggeredCastFlags = triggerFlags;
    if (info->AttributesEx4 & SPELL_ATTR4_TRIGGERED)
        _triggeredCastFlags = TRIGGERED_FULL_MASK;

    m_CastItem = NULL;
    m_castItemGUID = 0;

    unitTarget = NULL;
    itemTarget = NULL;
    gameObjTarget = NULL;
    focusObject = NULL;
    m_cast_count = 0;
    m_glyphIndex = 0;
    m_preCastSpell = 0;
    m_triggeredByAuraSpell  = NULL;
    m_spellAura = NULL;
    isStolen = false;

    //Auto Shot & Shoot (wand)
    m_autoRepeat = m_spellInfo->IsAutoRepeatRangedSpell();

    m_runesState = 0;
    m_powerCost = 0;                                        // setup to correct value in Spell::prepare, must not be used before.
    m_casttime = 0;                                         // setup to correct value in Spell::prepare, must not be used before.
    m_timer = 0;                                            // will set to castime in prepare
    m_periodicDamageModifier = 0.0f;

    m_channelTargetEffectMask = 0;
    m_hasDispelled = false;

    m_redirected = false;

    // Determine if spell can be reflected back to the caster
    // Patch 1.2 notes: Spell Reflection no longer reflects abilities
    m_canReflect = m_spellInfo->DmgClass == SPELL_DAMAGE_CLASS_MAGIC && !(m_spellInfo->Attributes & SPELL_ATTR0_ABILITY)
        && !(m_spellInfo->AttributesEx & SPELL_ATTR1_CANT_BE_REFLECTED) && !(m_spellInfo->Attributes & SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY)
        && !m_spellInfo->IsPassive() && !m_spellInfo->IsPositive() && !m_spellInfo->IsTargetingArea();

    CleanupTargetList();
    m_effectExecuteData.clear();

    for (uint8 i = 0; i < GetSpellInfo()->Effects.size(); ++i)
        m_destTargets[i] = SpellDestination(*m_caster);
}

Spell::~Spell()
{
    // unload scripts
    for (auto &script : m_loadedScripts)
    {
        script->_Unload();
        delete script;
    }

    if (m_referencedFromCurrentSpell && m_selfContainer && *m_selfContainer == this)
    {
        // Clean the reference to avoid later crash.
        // If this error is repeating, we may have to add an ASSERT to better track down how we get into this case.
        *m_selfContainer = NULL;
    }

    if (m_caster && m_caster->GetTypeId() == TYPEID_PLAYER)
        ASSERT(m_caster->ToPlayer()->m_spellModTakingSpell != this);

    CheckEffectExecuteData();
}

void Spell::InitExplicitTargets(SpellCastTargets const& targets)
{
    m_targets = targets;
    // this function tries to correct spell explicit targets for spell
    // client doesn't send explicit targets correctly sometimes - we need to fix such spells serverside
    // this also makes sure that we correctly send explicit targets to client (removes redundant data)
    uint32 neededTargets = m_spellInfo->GetExplicitTargetMask();

    if (WorldObject* target = m_targets.GetObjectTarget())
    {
        // check if object target is valid with needed target flags
        // for unit case allow corpse target mask because player with not released corpse is a unit target
        if ((target->ToUnit() && !(neededTargets & (TARGET_FLAG_UNIT_MASK | TARGET_FLAG_CORPSE_MASK)))
            || (target->ToGameObject() && !(neededTargets & TARGET_FLAG_GAMEOBJECT_MASK))
            || (target->ToCorpse() && !(neededTargets & TARGET_FLAG_CORPSE_MASK)))
            if (!m_spellInfo->HasPersistenAura() || (m_spellInfo->HasPersistenAura() && !(m_targets.GetTargetMask() & TARGET_FLAG_UNIT_MASK)))
                m_targets.RemoveObjectTarget();
    }
    else
    {
        // try to select correct unit target if not provided by client or by serverside cast
        if (neededTargets & (TARGET_FLAG_UNIT_MASK))
        {
            Unit* unit = NULL;
            // try to use player selection as a target
            if (Player* playerCaster = m_caster->ToPlayer())
            {
                // selection has to be found and to be valid target for the spell
                if (Unit* selectedUnit = ObjectAccessor::GetUnit(*m_caster, playerCaster->GetSelection()))
                    if (m_spellInfo->CheckExplicitTarget(m_caster, selectedUnit) == SPELL_CAST_OK)
                        unit = selectedUnit;
            }
            // try to use attacked unit as a target
            else if ((m_caster->GetTypeId() == TYPEID_UNIT) && neededTargets & (TARGET_FLAG_UNIT_ENEMY | TARGET_FLAG_UNIT))
                unit = m_caster->GetVictim();

            // didn't find anything - let's use self as target
            if (!unit && neededTargets & (TARGET_FLAG_UNIT_RAID | TARGET_FLAG_UNIT_PARTY | TARGET_FLAG_UNIT_ALLY))
                unit = m_caster;

            m_targets.SetUnitTarget(unit);
        }
    }

    // check if spell needs dst target
    if (neededTargets & TARGET_FLAG_DEST_LOCATION)
    {
        // and target isn't set
        if (!m_targets.HasDst())
        {
            // try to use unit target if provided
            if (WorldObject* target = targets.GetObjectTarget())
                m_targets.SetDst(*target);
            // or use self if not available
            else
                m_targets.SetDst(*m_caster);
        }
    }
    else
        m_targets.RemoveDst();

    if (neededTargets & TARGET_FLAG_SOURCE_LOCATION)
    {
        if (!targets.HasSrc())
            m_targets.SetSrc(*m_caster);
    }
    else
        m_targets.RemoveSrc();
}

void Spell::SelectExplicitTargets()
{
    // here go all explicit target changes made to explicit targets after spell prepare phase is finished
    if (Unit* target = m_targets.GetUnitTarget())
    {
        // check for explicit target redirection, for Grounding Totem for example
        if (m_spellInfo->GetExplicitTargetMask() & TARGET_FLAG_UNIT_ENEMY
            || (m_spellInfo->GetExplicitTargetMask() & TARGET_FLAG_UNIT && !m_spellInfo->IsPositive()))
        {
            Unit* redirect;
            switch (m_spellInfo->DmgClass)
            {
                case SPELL_DAMAGE_CLASS_MAGIC:
                    redirect = m_caster->GetMagicHitRedirectTarget(target, m_spellInfo);
                    break;
                case SPELL_DAMAGE_CLASS_MELEE:
                case SPELL_DAMAGE_CLASS_RANGED:
                    redirect = m_caster->GetMeleeHitRedirectTarget(target, m_spellInfo);
                    break;
                default:
                    redirect = NULL;
                    break;
            }
            if (redirect && (redirect != target))
            {
                m_targets.SetUnitTarget(redirect);
                m_redirected = true;
            }
        }
    }
}

void Spell::SelectSpellTargets()
{
    // select targets for cast phase
    SelectExplicitTargets();

    uint32 processedAreaEffectsMask = 0;
    for (uint32 i = 0; i < m_spellInfo->Effects.size(); ++i)
    {
        // not call for empty effect.
        // Also some spells use not used effect targets for store targets for dummy effect in triggered spells
        if (!m_spellInfo->Effects[i].IsEffect())
            continue;

        // set expected type of implicit targets to be sent to client
        uint32 implicitTargetMask = GetTargetFlagMask(m_spellInfo->Effects[i].TargetA.GetObjectType()) | GetTargetFlagMask(m_spellInfo->Effects[i].TargetB.GetObjectType());
        if (implicitTargetMask & TARGET_FLAG_UNIT)
            m_targets.SetTargetFlag(TARGET_FLAG_UNIT);
        if (implicitTargetMask & (TARGET_FLAG_GAMEOBJECT | TARGET_FLAG_GAMEOBJECT_ITEM))
            m_targets.SetTargetFlag(TARGET_FLAG_GAMEOBJECT);

        SelectEffectImplicitTargets(SpellEffIndex(i), m_spellInfo->Effects[i].TargetA, processedAreaEffectsMask);
        SelectEffectImplicitTargets(SpellEffIndex(i), m_spellInfo->Effects[i].TargetB, processedAreaEffectsMask);

        // Select targets of effect based on effect type
        // those are used when no valid target could be added for spell effect based on spell target type
        // some spell effects use explicit target as a default target added to target map (like SPELL_EFFECT_LEARN_SPELL)
        // some spell effects add target to target map only when target type specified (like SPELL_EFFECT_WEAPON)
        // some spell effects don't add anything to target map (confirmed with sniffs) (like SPELL_EFFECT_DESTROY_ALL_TOTEMS)
        SelectEffectTypeImplicitTargets(i);

        if (m_targets.HasDst())
            AddDestTarget(*m_targets.GetDst(), i);

        if (m_spellInfo->IsChanneled())
        {
            uint8 mask = (1 << i);
            for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
            {
                if (ihit->effectMask & mask)
                {
                    m_channelTargetEffectMask |= mask;
                    break;
                }
            }
        }
        else if (m_auraScaleMask)
        {
            bool checkLvl = !m_UniqueTargetInfo.empty();
            for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end();)
            {
                // remove targets which did not pass min level check
                if (m_auraScaleMask && ihit->effectMask == m_auraScaleMask)
                {
                    // Do not check for selfcast
                    if (!ihit->scaleAura && ihit->targetGUID != m_caster->GetGUID())
                    {
                         m_UniqueTargetInfo.erase(ihit++);
                         continue;
                    }
                }
                ++ihit;
            }
            if (checkLvl && m_UniqueTargetInfo.empty())
            {
                SendCastResult(SPELL_FAILED_LOWLEVEL);
                finish(false);
            }
        }
    }

    if (m_spellInfo->AttributesEx9 & SPELL_ATTR9_SPECIAL_DELAY_CALCULATION)
    {
        m_delayMoment = uint64(m_spellInfo->Speed * 1000.0f);
    }
    else if (m_targets.HasDst())
    {
        if (m_targets.HasTraj())
        {
            float speed = m_targets.GetSpeedXY();
            if (speed > 0.0f)
                m_delayMoment = (uint64)floor(m_targets.GetDist2d() / speed * 1000.0f);
        }
        else if (m_spellInfo->Speed > 0.0f)
        {
            float dist = m_caster->GetExactDist(m_targets.GetDstPos());
            m_delayMoment = uint64(floor(dist / m_spellInfo->Speed * 1000.0f));
        }
    }
}

void Spell::SelectEffectImplicitTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType, uint32& processedEffectMask)
{
    if (!targetType.GetTarget())
        return;

    uint32 effectMask = 1 << effIndex;
    // set the same target list for all effects
    // some spells appear to need this, however this requires more research
    switch (targetType.GetSelectionCategory())
    {
        case TARGET_SELECT_CATEGORY_NEARBY:
        case TARGET_SELECT_CATEGORY_CONE:
        case TARGET_SELECT_CATEGORY_AREA:
            // targets for effect already selected
            if (effectMask & processedEffectMask)
                return;
            // choose which targets we can select at once
            for (uint32 j = effIndex + 1; j < GetSpellInfo()->Effects.size(); ++j)
            {
                auto const &effects = GetSpellInfo()->Effects;
                if (effects[j].IsEffect()
                        && effects[effIndex].TargetA.GetTarget() == effects[j].TargetA.GetTarget()
                        && effects[effIndex].TargetB.GetTarget() == effects[j].TargetB.GetTarget()
                        && effects[effIndex].ImplicitTargetConditions == effects[j].ImplicitTargetConditions
                        && effects[effIndex].CalcRadius(m_caster) == effects[j].CalcRadius(m_caster))
                {
                    effectMask |= 1 << j;
                }
            }
            processedEffectMask |= effectMask;
            break;
        default:
            break;
    }

    switch (targetType.GetSelectionCategory())
    {
        case TARGET_SELECT_CATEGORY_CHANNEL:
            SelectImplicitChannelTargets(effIndex, targetType);
            break;
        case TARGET_SELECT_CATEGORY_NEARBY:
            SelectImplicitNearbyTargets(effIndex, targetType, effectMask);
            break;
        case TARGET_SELECT_CATEGORY_CONE:
            SelectImplicitConeTargets(effIndex, targetType, effectMask);
            break;
        case TARGET_SELECT_CATEGORY_AREA:
            SelectImplicitAreaTargets(effIndex, targetType, effectMask);
            break;
        case TARGET_SELECT_CATEGORY_DEFAULT:
            switch (targetType.GetObjectType())
            {
                case TARGET_OBJECT_TYPE_SRC:
                    switch (targetType.GetReferenceType())
                    {
                        case TARGET_REFERENCE_TYPE_CASTER:
                            m_targets.SetSrc(*m_caster);
                            break;
                        default:
                            ASSERT(false && "Spell::SelectEffectImplicitTargets: received not implemented select target reference type for TARGET_TYPE_OBJECT_SRC");
                            break;
                    }
                    break;
                case TARGET_OBJECT_TYPE_DEST:
                    switch (targetType.GetReferenceType())
                    {
                        case TARGET_REFERENCE_TYPE_CASTER:
                            SelectImplicitCasterDestTargets(effIndex, targetType);
                            break;
                        case TARGET_REFERENCE_TYPE_TARGET:
                            SelectImplicitTargetDestTargets(effIndex, targetType);
                            break;
                        case TARGET_REFERENCE_TYPE_DEST:
                            SelectImplicitDestDestTargets(effIndex, targetType);
                            break;
                        default:
                            ASSERT(false && "Spell::SelectEffectImplicitTargets: received not implemented select target reference type for TARGET_TYPE_OBJECT_DEST");
                            break;
                    }
                    break;
                default:
                    switch (targetType.GetReferenceType())
                    {
                        case TARGET_REFERENCE_TYPE_CASTER:
                            SelectImplicitCasterObjectTargets(effIndex, targetType);
                            break;
                        case TARGET_REFERENCE_TYPE_TARGET:
                            SelectImplicitTargetObjectTargets(effIndex, targetType);
                            break;
                        default:
                            ASSERT(false && "Spell::SelectEffectImplicitTargets: received not implemented select target reference type for TARGET_TYPE_OBJECT");
                            break;
                    }
                    break;
            }
            break;
        case TARGET_SELECT_CATEGORY_NYI:
            break;
        default:
            printf("Spell::SelectEffectImplicitTargets: received not implemented select target category / Spell ID = %u and Effect = %u and target type = %u \n", m_spellInfo->Id, effIndex, targetType.GetTarget());
            //ASSERT(false && "Spell::SelectEffectImplicitTargets: received not implemented select target category");
            break;
    }
}

void Spell::SelectImplicitChannelTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType)
{
    if (targetType.GetReferenceType() != TARGET_REFERENCE_TYPE_CASTER)
    {
        ASSERT(false && "Spell::SelectImplicitChannelTargets: received not implemented target reference type");
        return;
    }

    Spell* channeledSpell = m_originalCaster->GetCurrentSpell(CURRENT_CHANNELED_SPELL);
    if (!channeledSpell)
        return;

    switch (targetType.GetTarget())
    {
        case TARGET_UNIT_CHANNEL_TARGET:
        {
            WorldObject* target = ObjectAccessor::GetUnit(*m_caster, m_originalCaster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT));
            CallScriptObjectTargetSelectHandlers(target, effIndex);
            // unit target may be no longer avalible - teleported out of map for example
            if (target && target->ToUnit())
                AddUnitTarget(target->ToUnit(), 1 << effIndex);
            break;
        }
        case TARGET_DEST_CHANNEL_TARGET:
            if (channeledSpell->m_targets.HasDst())
                m_targets.SetDst(channeledSpell->m_targets);
            else if (WorldObject* target = ObjectAccessor::GetWorldObject(*m_caster, m_originalCaster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT)))
            {
                CallScriptObjectTargetSelectHandlers(target, effIndex);
                if (target)
                    m_targets.SetDst(*target);
            }
            break;
        case TARGET_DEST_CHANNEL_CASTER:
            m_targets.SetDst(*channeledSpell->GetCaster());
            break;
        default:
            ASSERT(false && "Spell::SelectImplicitChannelTargets: received not implemented target type");
            break;
    }
}

void Spell::SelectImplicitNearbyTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType, uint32 effMask)
{
    if (targetType.GetReferenceType() != TARGET_REFERENCE_TYPE_CASTER)
    {
        ASSERT(false && "Spell::SelectImplicitNearbyTargets: received not implemented target reference type");
        return;
    }

    float range = 0.0f;
    switch (targetType.GetCheckType())
    {
        case TARGET_CHECK_ENEMY:
            range = m_spellInfo->GetMaxRange(false, m_caster, this);
            break;
        case TARGET_CHECK_ALLY:
        case TARGET_CHECK_PARTY:
        case TARGET_CHECK_RAID:
        case TARGET_CHECK_RAID_CLASS:
            range = m_spellInfo->GetMaxRange(true, m_caster, this);
            break;
        case TARGET_CHECK_ENTRY:
        case TARGET_CHECK_DEFAULT:
            range = m_spellInfo->GetMaxRange(m_spellInfo->IsPositive(), m_caster, this);
            break;
        default:
            ASSERT(false && "Spell::SelectImplicitNearbyTargets: received not implemented selection check type");
            break;
    }

    ConditionList* condList = m_spellInfo->Effects[effIndex].ImplicitTargetConditions;

    // handle emergency case - try to use other provided targets if no conditions provided
    if (targetType.GetCheckType() == TARGET_CHECK_ENTRY && (!condList || condList->empty()))
    {
        switch (targetType.GetObjectType())
        {
            case TARGET_OBJECT_TYPE_GOBJ:
                if (m_spellInfo->RequiresSpellFocus)
                {
                    if (focusObject)
                        AddGOTarget(focusObject, effMask);
                    return;
                }
                break;
            case TARGET_OBJECT_TYPE_DEST:
                if (m_spellInfo->RequiresSpellFocus)
                {
                    if (focusObject)
                        m_targets.SetDst(*focusObject);
                    return;
                }
                break;
            default:
                break;
        }
    }

    WorldObject* target = SearchNearbyTarget(range, targetType.GetObjectType(), targetType.GetCheckType(), condList);
    if (!target)
        return;

    CallScriptObjectTargetSelectHandlers(target, effIndex);

    switch (targetType.GetObjectType())
    {
        case TARGET_OBJECT_TYPE_UNIT:
            if (Unit* unitTarget = target->ToUnit())
                AddUnitTarget(unitTarget, effMask, true, false);
            break;
        case TARGET_OBJECT_TYPE_GOBJ:
            if (GameObject* gobjTarget = target->ToGameObject())
                AddGOTarget(gobjTarget, effMask);
            break;
        case TARGET_OBJECT_TYPE_DEST:
            m_targets.SetDst(*target);
            break;
        default:
            ASSERT(false && "Spell::SelectImplicitNearbyTargets: received not implemented target object type");
            break;
    }

    SelectImplicitChainTargets(effIndex, targetType, target, effMask);
}

void Spell::SelectImplicitConeTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType, uint32 effMask)
{
    if (targetType.GetReferenceType() != TARGET_REFERENCE_TYPE_CASTER)
    {
        ASSERT(false && "Spell::SelectImplicitConeTargets: received not implemented target reference type");
        return;
    }
    std::list<WorldObject*> targets;
    SpellTargetObjectTypes objectType = targetType.GetObjectType();
    SpellTargetCheckTypes selectionType = targetType.GetCheckType();
    ConditionList* condList = m_spellInfo->Effects[effIndex].ImplicitTargetConditions;
    float coneAngle = M_PI/2;

    if (m_spellInfo->Effects[effIndex].TargetA.GetTarget() == TARGET_UNIT_CONE_ENEMY_110 ||
        m_spellInfo->Effects[effIndex].TargetA.GetTarget() == TARGET_UNIT_CONE_ENEMY_129 ||
        m_spellInfo->Effects[effIndex].TargetA.GetTarget() == TARGET_UNIT_CONE_ALLY_136)
        coneAngle = M_PI/6;

    switch (m_spellInfo->Id)
    {
        case 118094:
            coneAngle = M_PI/2;
            break;
        case 118105:
            coneAngle = M_PI/4;
            break;
        case 118106:
            coneAngle = M_PI/6;
            break;
    }

    float radius = m_spellInfo->Effects[effIndex].CalcRadius(m_caster) * m_spellValue.RadiusMod;

    if (uint32 containerTypeMask = GetSearcherTypeMask(objectType, condList))
    {
        Trinity::WorldObjectSpellConeTargetCheck check(coneAngle, radius, m_caster, m_spellInfo, selectionType, condList);
        Trinity::WorldObjectListSearcher<Trinity::WorldObjectSpellConeTargetCheck> searcher(m_caster, targets, check, containerTypeMask);
        SearchTargets<Trinity::WorldObjectListSearcher<Trinity::WorldObjectSpellConeTargetCheck> >(searcher, containerTypeMask, m_caster, m_caster, radius);

        CallScriptObjectAreaTargetSelectHandlers(targets, effIndex);

        if (!targets.empty())
        {
            // Other special target selection goes here
            if (uint32 maxTargets = m_spellValue.MaxAffectedTargets)
                Trinity::Containers::RandomResizeList(targets, maxTargets);

            // for compability with older code - add only unit and go targets
            // TODO: remove this
            std::list<Unit*> unitTargets;
            std::list<GameObject*> gObjTargets;

            for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
            {
                if (Unit* unitTarget = (*itr)->ToUnit())
                    unitTargets.push_back(unitTarget);
                else if (GameObject* gObjTarget = (*itr)->ToGameObject())
                    gObjTargets.push_back(gObjTarget);
            }

            // Light of Dawn
            if (m_spellInfo->Id == 85222)
            {
                uint8 maxSize = m_caster->HasAura(54940) ? 4 : 6; // Glyph of Light of Dawn
                unitTargets.push_back(m_caster);
                if (unitTargets.size() > maxSize)
                {
                    unitTargets.sort(Trinity::HealthPctOrderPred());
                    unitTargets.resize(maxSize);
                }
            }

            for (std::list<Unit*>::iterator itr = unitTargets.begin(); itr != unitTargets.end(); ++itr)
                AddUnitTarget(*itr, effMask, false);

            for (std::list<GameObject*>::iterator itr = gObjTargets.begin(); itr != gObjTargets.end(); ++itr)
                AddGOTarget(*itr, effMask);
        }
    }
}

void Spell::SelectImplicitAreaTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType, uint32 effMask)
{
    Unit* referer = NULL;
    switch (targetType.GetReferenceType())
    {
        case TARGET_REFERENCE_TYPE_SRC:
        case TARGET_REFERENCE_TYPE_DEST:
        case TARGET_REFERENCE_TYPE_CASTER:
            referer = m_caster;
            break;
        case TARGET_REFERENCE_TYPE_TARGET:
            referer = m_targets.GetUnitTarget();
            break;
        case TARGET_REFERENCE_TYPE_LAST:
        {
            // find last added target for this effect
            for (std::list<TargetInfo>::reverse_iterator ihit = m_UniqueTargetInfo.rbegin(); ihit != m_UniqueTargetInfo.rend(); ++ihit)
            {
                if (ihit->effectMask & (1<<effIndex))
                {
                    referer = ObjectAccessor::GetUnit(*m_caster, ihit->targetGUID);
                    break;
                }
            }
            break;
        }
        default:
            ASSERT(false && "Spell::SelectImplicitAreaTargets: received not implemented target reference type");
            return;
    }
    if (!referer)
        return;

    Position const* center = NULL;
    switch (targetType.GetReferenceType())
    {
        case TARGET_REFERENCE_TYPE_SRC:
            center = m_targets.GetSrcPos();
            break;
        case TARGET_REFERENCE_TYPE_DEST:
            center = m_targets.GetDstPos();
            break;
        case TARGET_REFERENCE_TYPE_CASTER:
        case TARGET_REFERENCE_TYPE_TARGET:
        case TARGET_REFERENCE_TYPE_LAST:
            center = referer;
            break;
         default:
             ASSERT(false && "Spell::SelectImplicitAreaTargets: received not implemented target reference type");
             return;
    }
    std::list<WorldObject*> targets;
    float radius = m_spellInfo->Effects[effIndex].CalcRadius(m_caster) * m_spellValue.RadiusMod;
    SearchAreaTargets(targets, radius, center, referer, targetType.GetObjectType(), targetType.GetCheckType(), m_spellInfo->Effects[effIndex].ImplicitTargetConditions);

    // Custom entries
    // TODO: remove those
    switch (m_spellInfo->Id)
    {
        case 117002:// Tap to the Spirit World
        {
            // remove existing targets
            CleanupTargetList();

            if (!targets.empty())
                for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                    if ((*itr) && (*itr)->ToUnit())
                        if ((*itr)->GetEntry() == 60512)
                            AddUnitTarget((*itr)->ToUnit(), 1 << effIndex, false);

            return;
        }
        default:
            break;
    }

    CallScriptObjectAreaTargetSelectHandlers(targets, effIndex);

    std::list<Unit*> unitTargets;
    std::list<GameObject*> gObjTargets;
    // for compability with older code - add only unit and go targets
    // TODO: remove this
    for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
    {
        if (!(*itr))
            continue;

        if (Unit* unitTarget = (*itr)->ToUnit())
            unitTargets.push_back(unitTarget);
        else if (GameObject* gObjTarget = (*itr)->ToGameObject())
            gObjTargets.push_back(gObjTarget);
    }

    if (!unitTargets.empty())
    {
        // Special target selection for smart heals and energizes
        uint32 maxSize = 0;
        int32 power = -1;
        switch (m_spellInfo->SpellFamilyName)
        {
            case SPELLFAMILY_GENERIC:
                switch (m_spellInfo->Id)
                {
                    case 52759: // Ancestral Awakening
                    case 71610: // Echoes of Light (Althor's Abacus normal version)
                    case 71641: // Echoes of Light (Althor's Abacus heroic version)
                    case 99152: // Cauterizing Flame, Item - Priest Healer T12 4P Bonus
                    case 96966: // Blaze of Life, Eye of Blazing Power (Normal)
                    case 97136: // Blaze of Life, Eye of Blazing Power (Heroic)
                    case 109825: // Nick of Time, Windward Heart (heroic)
                    case 108000: // Nick of Time, Windward Heart (normal)
                    case 109822: // Nick of Time, Windward Heart (lfr)
                        maxSize = 1;
                        power = POWER_HEALTH;
                        break;
                    case 54968: // Glyph of Holy Light
                        maxSize = m_spellInfo->MaxAffectedTargets;
                        power = POWER_HEALTH;
                        break;
                    case 114911: // Ancestral Guidance
                        maxSize = 3;
                        power = POWER_HEALTH;

                        // Remove targets outside caster's raid
                        for (std::list<Unit*>::iterator itr = unitTargets.begin(); itr != unitTargets.end();)
                        {
                            if (!(*itr)->IsInRaidWith(m_caster))
                                itr = unitTargets.erase(itr);
                            else
                                ++itr;
                        }
                        break;
                    case 57669: // Replenishment
                        // In arenas Replenishment may only affect the caster
                        if (m_caster->GetTypeId() == TYPEID_PLAYER && m_caster->ToPlayer()->InArena())
                        {
                            unitTargets.clear();
                            unitTargets.push_back(m_caster);
                            break;
                        }
                        maxSize = 10;
                        power = POWER_MANA;
                        break;
                    case 107145: // Light Wall Active
                        m_targets.AddExtraTarget(0, WorldLocation(996, -989.4236f, -2821.757f, 38.25466f, 0.0f));
                        m_targets.AddExtraTarget(0, WorldLocation(996, -1045.602f, -2822.323f, 38.25466f, 0.0f));
                        break;
                    default:
                        break;
                }
                break;
            case SPELLFAMILY_PRIEST:
                if (m_spellInfo->SpellFamilyFlags[0] == 0x10000000) // Circle of Healing
                {
                    maxSize = m_caster->HasAura(55675) ? 6 : 5; // Glyph of Circle of Healing
                    power = POWER_HEALTH;
                }
                else if (m_spellInfo->Id == 64844) // Divine Hymn
                {
                    maxSize = m_caster->GetMap()->Is25ManRaid() ? 12 : 5;
                    power = POWER_HEALTH;
                }
                else if (m_spellInfo->Id == 64904) // Hymn of Hope
                {
                    maxSize = 3;
                    power = POWER_MANA;
                }
                else
                    break;

                // Remove targets outside caster's raid
                for (std::list<Unit*>::iterator itr = unitTargets.begin(); itr != unitTargets.end();)
                {
                    if (!(*itr)->IsInRaidWith(m_caster))
                        itr = unitTargets.erase(itr);
                    else
                        ++itr;
                }
                break;
            case SPELLFAMILY_PALADIN:
            {
                switch (m_spellInfo->Id)
                {
                    case 119072:// Holy Wrath
                    {
                        static const uint8 types_noglyph[] = {CREATURE_TYPE_DEMON, CREATURE_TYPE_UNDEAD, 0};
                        static const uint8 types_glyph[] = {CREATURE_TYPE_DEMON, CREATURE_TYPE_UNDEAD, CREATURE_TYPE_ELEMENTAL, CREATURE_TYPE_DRAGONKIN, 0};
                        const uint8 *types = m_caster->HasAura(54923) ? types_glyph: types_noglyph;

                        // Normal case
                        if (effIndex == 1 && !m_caster->HasAura(115738))
                        {
                            for (std::list<Unit*>::iterator itr = unitTargets.begin() ; itr != unitTargets.end();)
                            {
                                bool found = false;
                                uint8 types_i = 0;
                                do
                                {
                                    if ((*itr)->GetCreatureType() == types[types_i])
                                    {
                                        found = true;
                                        break;
                                    }
                                }
                                while (types[++types_i]);

                                if (found)
                                    itr++;
                                else
                                    itr = unitTargets.erase(itr);
                            }
                        }
                        // Glyph of Focused Wrath
                        else if (m_caster->HasAura(115738))
                        {
                            if (m_caster->GetTypeId() != TYPEID_PLAYER)
                                break;

                            Player* player = m_caster->ToPlayer();
                            if (!player)
                                break;

                            if (Unit* target = player->GetSelectedUnit())
                            {
                                if (effIndex == 1)
                                {
                                    bool found = false;
                                    uint8 types_i = 0;
                                    do
                                    {
                                        if (target->GetCreatureType() == types[types_i])
                                        {
                                            found = true;
                                            break;
                                        }
                                    }
                                    while (types[++types_i]);

                                    unitTargets.clear();
                                    if (found)
                                        unitTargets.push_back(target);
                                }
                                else
                                {
                                    unitTargets.clear();
                                    unitTargets.push_back(target);
                                }
                            }
                            else
                            {
                                unitTargets.sort(Trinity::ObjectDistanceOrderPred(m_caster));
                                Unit* victim = (*unitTargets.begin())->ToUnit();

                                if (victim)
                                {
                                    if (effIndex == 1)
                                    {
                                        bool found = false;
                                        uint8 types_i = 0;
                                        do
                                        {
                                            if (victim->GetCreatureType() == types[types_i])
                                            {
                                                found = true;
                                                break;
                                            }
                                        }
                                        while (types[++types_i]);

                                        unitTargets.clear();
                                        if (found)
                                            unitTargets.push_back(victim);
                                    }
                                    else
                                    {
                                        unitTargets.clear();
                                        unitTargets.push_back(victim);
                                    }
                                }
                            }
                        }

                        break;
                    }
                    default:
                        break;
                }

                break;
            }
            case SPELLFAMILY_DRUID:
            {
                switch(m_spellInfo->Id)
                {
                    // Firebloom, Item  Druid T12 Restoration 4P Bonus
                    case 99017:
                        maxSize = 1;
                        power = POWER_HEALTH;
                        break;
                    // Efflorescence
                    case 81269:
                        maxSize = 3;
                        power = POWER_HEALTH;
                        break;
                    // Tranquility
                    case 44203:
                        maxSize = 5;
                        power = POWER_HEALTH;
                        break;
                }
                if (m_spellInfo->SpellFamilyFlags[1] == 0x04000000) // Wild Growth
                {
                    maxSize = m_caster->HasAura(62970) ? 6 : 5; // Glyph of Wild Growth
                    power = POWER_HEALTH;
                }
                else
                    break;

                // Remove targets outside caster's raid
                for (std::list<Unit*>::iterator itr = unitTargets.begin(); itr != unitTargets.end();)
                    if (!(*itr)->IsInRaidWith(m_caster))
                        itr = unitTargets.erase(itr);
                    else
                        ++itr;
                break;
            }
            case SPELLFAMILY_MONK:
            {
                switch (m_spellInfo->Id)
                {
                    case 117895: // Eminence (Statue)
                    case 126890: // Eminence (Player)
                    {
                        maxSize = 1;
                        power = POWER_HEALTH;

                        // Remove targets outside caster's raid
                        for (std::list<Unit*>::iterator itr = unitTargets.begin(); itr != unitTargets.end();)
                        {
                            if (!(*itr)->IsInRaidWith(m_caster))
                                itr = unitTargets.erase(itr);
                            else
                                ++itr;
                        }
                        break;
                    }
                }
                break;
            }
            default:
                break;
        }

        if (maxSize && power != -1)
        {
            if (Powers(power) == POWER_HEALTH)
            {
                if (unitTargets.size() > maxSize)
                {
                    unitTargets.sort(Trinity::HealthPctOrderPred());
                    unitTargets.resize(maxSize);
                }
            }
            else
            {
                for (std::list<Unit*>::iterator itr = unitTargets.begin(); itr != unitTargets.end();)
                    if ((*itr)->getPowerType() != (Powers)power)
                        itr = unitTargets.erase(itr);
                    else
                        ++itr;

                if (unitTargets.size() > maxSize)
                {
                    unitTargets.sort(Trinity::PowerPctOrderPred((Powers)power));
                    unitTargets.resize(maxSize);
                }
            }
        }

        // todo: move to scripts, but we must call it before resize list by MaxAffectedTargets
        // Intimidating Shout
        if (m_spellInfo->Id == 5246 && effIndex != EFFECT_0)
            unitTargets.remove(m_targets.GetUnitTarget());

        // Custom MoP Script
        // 107270 / 117640 - Spinning Crane Kick : Give 1 Chi if the spell hits at least 3 targets
        if (auto player = m_caster->ToPlayer())
        {
            if ((m_spellInfo->Id == 107270 || m_spellInfo->Id == 117640 || m_spellInfo->Id == 148187) && unitTargets.size() >= 3 && !player->HasSpellCooldown(129881))
            {
                m_caster->CastSpell(m_caster, 129881, true);
                player->AddSpellCooldown(129881, 0, 3 * IN_MILLISECONDS);
                // Muscle Memory
                if (player->HasSpell(139598))
                    player->CastSpell(player, 139597, true);
            }
        }

        // Other special target selection goes here
        if (uint32 maxTargets = m_spellValue.MaxAffectedTargets)
            Trinity::Containers::RandomResizeList(unitTargets, maxTargets);

        for (std::list<Unit*>::iterator itr = unitTargets.begin(); itr != unitTargets.end(); ++itr)
            AddUnitTarget(*itr, effMask, false);
    }

    if (!gObjTargets.empty())
    {
        if (uint32 maxTargets = m_spellValue.MaxAffectedTargets)
            Trinity::Containers::RandomResizeList(gObjTargets, maxTargets);

        for (std::list<GameObject*>::iterator itr = gObjTargets.begin(); itr != gObjTargets.end(); ++itr)
            AddGOTarget(*itr, effMask);
    }
}

void Spell::SelectImplicitCasterDestTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType)
{
    switch (targetType.GetTarget())
    {
        case TARGET_DEST_CASTER:
            m_targets.SetDst(*m_caster);
            return;
        case TARGET_DEST_HOME:
            if (Player* playerCaster = m_caster->ToPlayer())
                m_targets.SetDst(playerCaster->m_homebindX, playerCaster->m_homebindY, playerCaster->m_homebindZ, playerCaster->GetOrientation(), playerCaster->m_homebindMapId);
            return;
        case TARGET_DEST_DB:
            if (SpellTargetPosition const* st = sSpellMgr->GetSpellTargetPosition(m_spellInfo->Id, effIndex))
            {
                // @TODO: fix this check
                if (m_spellInfo->HasEffect(SPELL_EFFECT_TELEPORT_UNITS) || m_spellInfo->HasEffect(SPELL_EFFECT_BIND))
                    m_targets.SetDst(st->target_X, st->target_Y, st->target_Z, st->target_Orientation, (int32)st->target_mapId);
                else if (st->target_mapId == m_caster->GetMapId())
                    m_targets.SetDst(st->target_X, st->target_Y, st->target_Z, st->target_Orientation);
            }
            else
            {
                TC_LOG_DEBUG("spells", "SPELL: unknown target coordinates for spell ID %u", m_spellInfo->Id);
                WorldObject* target = m_targets.GetObjectTarget();
                m_targets.SetDst(target ? *target : *m_caster);
            }
            return;
        case TARGET_DEST_CASTER_FISHING:
        {
            float min_dis = m_spellInfo->GetMinRange(true);
            float max_dis = m_spellInfo->GetMaxRange(true);
            float dis = (float)rand_norm() * (max_dis - min_dis) + min_dis;
            float x, y, z, angle;
            angle = (float)rand_norm() * static_cast<float>(M_PI * 35.0f / 180.0f) - static_cast<float>(M_PI * 17.5f / 180.0f);
            m_caster->GetClosePoint(x, y, z, DEFAULT_WORLD_OBJECT_SIZE, dis, angle);

            float ground = z;
            float liquidLevel = m_caster->GetMap()->GetWaterOrGroundLevel(x, y, z, &ground);
            if (liquidLevel <= ground) // When there is no liquid Map::GetWaterOrGroundLevel returns ground level
            {
                SendCastResult(SPELL_FAILED_NOT_HERE);
                SendChannelUpdate(0);
                finish(false);
                return;
            }

            if (ground + 0.75 > liquidLevel)
            {
                SendCastResult(SPELL_FAILED_TOO_SHALLOW);
                SendChannelUpdate(0);
                finish(false);
                return;
            }

            m_targets.SetDst(x, y, liquidLevel, m_caster->GetOrientation());
            return;
        }
        case TARGET_DEST_CASTER_GROUND:
        {
            if (Map * map = m_caster->GetMap())
            {
                float groundZ = map->GetHeight(m_caster->GetPhaseMask(), m_caster->GetPositionX(), m_caster->GetPositionY(), MAX_HEIGHT);
                m_targets.SetDst(m_caster->GetPositionX(), m_caster->GetPositionY(), groundZ, m_caster->GetOrientation(), map->GetId());
            }
            return;
        }
        default:
            break;
    }

    float dist;
    float angle = targetType.CalcDirectionAngle();
    float objSize = m_caster->GetObjectSize();
    if (targetType.GetTarget() == TARGET_DEST_CASTER_SUMMON)
        dist = PET_FOLLOW_DIST;
    else
        dist = m_spellInfo->Effects[effIndex].CalcRadius(m_caster);

    if (dist < objSize)
        dist = objSize;
    else if (targetType.GetTarget() == TARGET_DEST_CASTER_RANDOM)
        dist = objSize + (dist - objSize) * (float)rand_norm();

    Position pos;

    switch (targetType.GetTarget())
    {
        case TARGET_DEST_CASTER_FRONT_LEAP:
        case TARGET_DEST_CASTER_FRONT_LEFT:
        case TARGET_DEST_CASTER_BACK_LEFT:
        case TARGET_DEST_CASTER_BACK_RIGHT:
        case TARGET_DEST_CASTER_FRONT_RIGHT:
        case TARGET_DEST_CASTER_FRONT:
            if (m_caster->GetTypeId() == TYPEID_UNIT && !m_caster->ToCreature()->isLosDisabled())
            {
                m_caster->GetPosition(&pos);
                m_caster->MovePositionFixedXY(pos, dist, angle);
            }else
                m_caster->GetFirstCollisionPosition(pos, dist, angle);
            break;
        case TARGET_DEST_CASTER_SUMMON:
            m_caster->GetPosition(&pos);
            m_caster->MovePositionFixedXY(pos, dist, angle);
            break;
        default:
            m_caster->GetPosition(&pos);
            m_caster->MovePositionFixedXY(pos, dist, angle);
            pos.SetOrientation(m_caster->GetOrientation() + angle);
            break;
    }

    m_targets.SetDst(*m_caster);
    m_targets.ModDst(pos);
}

void Spell::SelectImplicitTargetDestTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType)
{
    WorldObject* target = m_targets.GetObjectTarget();
    switch (targetType.GetTarget())
    {
        case TARGET_DEST_TARGET_ENEMY:
        case TARGET_DEST_TARGET_ANY:
            m_targets.SetDst(*target);
            return;
        default:
            break;
    }

    float angle = targetType.CalcDirectionAngle();
    float objSize = target->GetObjectSize();
    float dist = m_spellInfo->Effects[effIndex].CalcRadius(m_caster);
    if (dist < objSize)
        dist = objSize;
    else if (targetType.GetTarget() == TARGET_DEST_TARGET_RANDOM)
        dist = objSize + (dist - objSize) * (float)rand_norm();

    Position pos;
    target->GetPosition(&pos);
    target->MovePositionFixedXY(pos, dist, angle);

    m_targets.SetDst(*target);
    m_targets.ModDst(pos);
}

void Spell::SelectImplicitDestDestTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType)
{
    // set destination to caster if no dest provided
    // can only happen if previous destination target could not be set for some reason
    // (not found nearby target, or channel target for example
    // maybe we should abort the spell in such case?
    if (!m_targets.HasDst())
        m_targets.SetDst(*m_caster);

    switch (targetType.GetTarget())
    {
        case TARGET_DEST_DYNOBJ_ENEMY:
        case TARGET_DEST_DYNOBJ_ALLY:
        case TARGET_DEST_DYNOBJ_ALL_UNITS:
        case TARGET_DEST_DEST:
            return;
        case TARGET_DEST_TRAJ:
            SelectImplicitTrajTargets();
            return;
        default:
            break;
    }

    float angle = targetType.CalcDirectionAngle();
    float dist = m_spellInfo->Effects[effIndex].CalcRadius(m_caster);
    if (targetType.GetTarget() == TARGET_DEST_DEST_RANDOM)
        dist *= (float)rand_norm();

    Position pos = *m_targets.GetDstPos();
    m_caster->MovePositionToFirstCollision(pos, dist, angle);
    pos.SetOrientation(m_caster->GetOrientation() + angle);
    m_targets.ModDst(pos);
}

void Spell::SelectImplicitCasterObjectTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType)
{
    WorldObject* target = NULL;
    bool checkIfValid = true;

    switch (targetType.GetTarget())
    {
        case TARGET_UNIT_CASTER:
            target = m_caster;
            checkIfValid = false;
            break;
        case TARGET_UNIT_MASTER:
            target = m_caster->GetCharmerOrOwner();
            break;
        case TARGET_UNIT_PET:
            target = m_caster->GetGuardianPet();
            break;
        case TARGET_UNIT_SUMMONER:
            if (m_caster->IsSummon())
                target = m_caster->ToTempSummon()->GetSummoner();
            break;
        case TARGET_UNIT_VEHICLE:
            target = m_caster->GetVehicleBase();
            break;
        case TARGET_UNIT_PASSENGER_0:
        case TARGET_UNIT_PASSENGER_1:
        case TARGET_UNIT_PASSENGER_2:
        case TARGET_UNIT_PASSENGER_3:
        case TARGET_UNIT_PASSENGER_4:
        case TARGET_UNIT_PASSENGER_5:
        case TARGET_UNIT_PASSENGER_6:
        case TARGET_UNIT_PASSENGER_7:
            if (m_caster->GetTypeId() == TYPEID_UNIT && m_caster->ToCreature()->IsVehicle())
                target = m_caster->GetVehicleKit()->GetPassenger(targetType.GetTarget() - TARGET_UNIT_PASSENGER_0);
            break;
        default:
            break;
    }

    CallScriptObjectTargetSelectHandlers(target, effIndex);

    if (target && target->ToUnit())
        AddUnitTarget(target->ToUnit(), 1 << effIndex, checkIfValid);
}

void Spell::SelectImplicitTargetObjectTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType)
{
    // Persistent auras should cast trigger spells when they is activated on target
    // Wtf is this
    if (m_spellInfo->HasPersistenAura() && m_spellInfo->Id != 78675)
        return;

    ASSERT((m_targets.GetObjectTarget() || m_targets.GetItemTarget() || m_targets.GetUnitTarget()) && "Spell::SelectImplicitTargetObjectTargets - no explicit object or item target available!");

    WorldObject* target = m_targets.GetObjectTarget();

    CallScriptObjectTargetSelectHandlers(target, effIndex);

    if (target)
    {
        if (Unit* unit = target->ToUnit())
            AddUnitTarget(unit, 1 << effIndex, m_redirected == true ? false : true, false, effIndex);
        else if (GameObject* gobj = target->ToGameObject())
            AddGOTarget(gobj, 1 << effIndex);

        SelectImplicitChainTargets(effIndex, targetType, target, 1 << effIndex);
    }
    // Script hook can remove object target and we would wrongly land here
    else if (Item* item = m_targets.GetItemTarget())
        AddItemTarget(item, 1 << effIndex);
}

void Spell::SelectImplicitChainTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType, WorldObject* target, uint32 effMask)
{
    uint32 maxTargets = m_spellInfo->Effects[effIndex].ChainTarget;
    if (Player* modOwner = m_caster->GetSpellModOwner())
        modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_JUMP_TARGETS, maxTargets, this);

    if (maxTargets > 1)
    {
        // mark damage multipliers as used
        for (uint32 k = effIndex; k < GetSpellInfo()->Effects.size(); ++k)
            if (effMask & (1 << k))
                m_damageMultipliers[k] = 1.0f;

        m_applyMultiplierMask |= effMask;

        std::list<WorldObject*> targets;
        SearchChainTargets(targets, maxTargets - 1, target, targetType.GetObjectType(), targetType.GetCheckType(),
                           m_spellInfo->Effects[effIndex].ImplicitTargetConditions,
                           targetType.GetTarget() == TARGET_UNIT_TARGET_CHAINHEAL_ALLY);

        // Chain primary target is added earlier
        CallScriptObjectAreaTargetSelectHandlers(targets, effIndex);

        // for backward compability
        std::list<Unit*> unitTargets;
        for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
            if (Unit* unitTarget = (*itr)->ToUnit())
                unitTargets.push_back(unitTarget);

        for (std::list<Unit*>::iterator itr = unitTargets.begin(); itr != unitTargets.end(); ++itr)
            AddUnitTarget(*itr, effMask, false);
    }
}

float tangent(float x)
{
    x = tan(x);
    //if (x < std::numeric_limits<float>::max() && x > -std::numeric_limits<float>::max()) return x;
    //if (x >= std::numeric_limits<float>::max()) return std::numeric_limits<float>::max();
    //if (x <= -std::numeric_limits<float>::max()) return -std::numeric_limits<float>::max();
    if (x < 100000.0f && x > -100000.0f) return x;
    if (x >= 100000.0f) return 100000.0f;
    if (x <= 100000.0f) return -100000.0f;
    return 0.0f;
}

void Spell::SelectImplicitTrajTargets()
{
    if (!m_targets.HasTraj())
        return;

    float dist2d = m_targets.GetDist2d();
    if (!dist2d)
        return;

    float srcToDestDelta = m_targets.GetDstPos()->m_positionZ - m_targets.GetSrcPos()->m_positionZ;

    std::list<WorldObject*> targets;
    Trinity::WorldObjectSpellTrajTargetCheck check(dist2d, m_targets.GetSrcPos(), m_caster, m_spellInfo);
    Trinity::WorldObjectListSearcher<Trinity::WorldObjectSpellTrajTargetCheck> searcher(m_caster, targets, check, GRID_MAP_TYPE_MASK_ALL);
    SearchTargets(searcher, GRID_MAP_TYPE_MASK_ALL, m_caster, m_targets.GetSrcPos(), dist2d);
    if (targets.empty())
        return;

    targets.sort(Trinity::ObjectDistanceOrderPred(m_caster));

    float b = tangent(m_targets.GetElevation());
    float a = (srcToDestDelta - dist2d * b) / (dist2d * dist2d);
    if (a > -0.0001f)
        a = 0;

    float bestDist = m_spellInfo->GetMaxRange(false);

    auto itr = targets.begin();
    for (; itr != targets.end(); ++itr)
    {
        if (Unit* unitTarget = (*itr)->ToUnit())
            if (m_caster == *itr || m_caster->IsOnVehicle(unitTarget) || (unitTarget)->GetVehicle())//(*itr)->IsOnVehicle(m_caster))
                continue;

        const float size = std::max((*itr)->GetObjectSize() * 0.7f, 1.0f); // 1/sqrt(3)
        /// @todo all calculation should be based on src instead of m_caster
        const float objDist2d = m_targets.GetSrcPos()->GetExactDist2d(*itr) * std::cos(m_targets.GetSrcPos()->GetRelativeAngle(*itr));
        const float dz = (*itr)->GetPositionZ() - m_targets.GetSrcPos()->m_positionZ;

        float dist = objDist2d - size;
        float height = dist * (a * dist + b);

        if (dist < bestDist && height < dz + size && height > dz - size)
        {
            bestDist = dist > 0 ? dist : 0;
            break;
        }

#define CHECK_DIST {\
            if (dist > bestDist)\
                continue;\
            if (dist < objDist2d + size && dist > objDist2d - size)\
            {\
                bestDist = dist;\
                break;\
            }\
        }

        if (!a)
        {
            height = dz - size;
            dist = height / b;
            CHECK_DIST;

            height = dz + size;
            dist = height / b;
            CHECK_DIST;

            continue;
        }

        height = dz - size;
        float sqrt1 = b * b + 4 * a * height;
        if (sqrt1 > 0)
        {
            sqrt1 = sqrt(sqrt1);
            dist = (sqrt1 - b) / (2 * a);
            CHECK_DIST;
        }

        height = dz + size;
        float sqrt2 = b * b + 4 * a * height;
        if (sqrt2 > 0)
        {
            sqrt2 = sqrt(sqrt2);
            dist = (sqrt2 - b) / (2 * a);
            CHECK_DIST;

            dist = (-sqrt2 - b) / (2 * a);
            CHECK_DIST;
        }

        if (sqrt1 > 0)
        {
            dist = (-sqrt1 - b) / (2 * a);
            CHECK_DIST;
        }
    }

    if (m_targets.GetSrcPos()->GetExactDist2d(m_targets.GetDstPos()) > bestDist)
    {
        float x = m_targets.GetSrcPos()->m_positionX + std::cos(m_caster->GetOrientation()) * bestDist;
        float y = m_targets.GetSrcPos()->m_positionY + std::sin(m_caster->GetOrientation()) * bestDist;
        float z = m_targets.GetSrcPos()->m_positionZ + bestDist * (a * bestDist + b);

        if (itr != targets.end())
        {
            float distSq = (*itr)->GetExactDistSq(x, y, z);
            float sizeSq = (*itr)->GetObjectSize();
            sizeSq *= sizeSq;

            if (distSq > sizeSq)
            {
                float factor = 1 - sqrt(sizeSq / distSq);
                x += factor * ((*itr)->GetPositionX() - x);
                y += factor * ((*itr)->GetPositionY() - y);
                z += factor * ((*itr)->GetPositionZ() - z);

                distSq = (*itr)->GetExactDistSq(x, y, z);
            }
        }

        Position trajDst;
        trajDst.Relocate(x, y, z, m_caster->GetOrientation());
        m_targets.ModDst(trajDst);
    }
}

void Spell::SelectEffectTypeImplicitTargets(uint8 effIndex)
{
    // special case for SPELL_EFFECT_SUMMON_RAF_FRIEND and SPELL_EFFECT_SUMMON_PLAYER
    // TODO: this is a workaround - target shouldn't be stored in target map for those spells
    switch (m_spellInfo->Effects[effIndex].Effect)
    {
        case SPELL_EFFECT_SUMMON_RAF_FRIEND:
        case SPELL_EFFECT_SUMMON_PLAYER:
            if (m_caster->GetTypeId() == TYPEID_PLAYER && m_caster->ToPlayer()->GetSelection())
            {
                WorldObject* target = ObjectAccessor::FindPlayer(m_caster->ToPlayer()->GetSelection());

                CallScriptObjectTargetSelectHandlers(target, SpellEffIndex(effIndex));

                if (target && target->ToPlayer())
                    AddUnitTarget(target->ToUnit(), 1 << effIndex, false);
            }
            return;
        default:
            break;
    }

    // select spell implicit targets based on effect type
    if (!m_spellInfo->Effects[effIndex].GetImplicitTargetType())
        return;

    uint32 targetMask = m_spellInfo->Effects[effIndex].GetMissingTargetMask();

    if (!targetMask)
        return;

    WorldObject* target = NULL;

    switch (m_spellInfo->Effects[effIndex].GetImplicitTargetType())
    {
        // add explicit object target or self to the target map
        case EFFECT_IMPLICIT_TARGET_EXPLICIT:
            // player which not released his spirit is Unit, but target flag for it is TARGET_FLAG_CORPSE_MASK
            if (targetMask & (TARGET_FLAG_UNIT_MASK | TARGET_FLAG_CORPSE_MASK))
            {
                if (Unit* unitTarget = m_targets.GetUnitTarget())
                    target = unitTarget;
                else if (targetMask & TARGET_FLAG_CORPSE_MASK)
                {
                    if (Corpse* corpseTarget = m_targets.GetCorpseTarget())
                    {
                        // TODO: this is a workaround - corpses should be added to spell target map too, but we can't do that so we add owner instead
                        if (Player* owner = ObjectAccessor::FindPlayer(corpseTarget->GetOwnerGUID()))
                            target = owner;
                    }
                }
                else //if (targetMask & TARGET_FLAG_UNIT_MASK)
                    target = m_caster;
            }
            if (targetMask & TARGET_FLAG_ITEM_MASK)
            {
                if (Item* itemTarget = m_targets.GetItemTarget())
                    AddItemTarget(itemTarget, 1 << effIndex);
                return;
            }
            if (targetMask & TARGET_FLAG_GAMEOBJECT_MASK)
                target = m_targets.GetGOTarget();
            break;
        // add self to the target map
        case EFFECT_IMPLICIT_TARGET_CASTER:
            if (targetMask & TARGET_FLAG_UNIT_MASK)
                target = m_caster;
            break;
        default:
            break;
    }

    CallScriptObjectTargetSelectHandlers(target, SpellEffIndex(effIndex));

    if (target)
    {
        if (target->ToUnit())
            AddUnitTarget(target->ToUnit(), 1 << effIndex, false);
        else if (target->ToGameObject())
            AddGOTarget(target->ToGameObject(), 1 << effIndex);
    }
}

uint32 Spell::GetSearcherTypeMask(SpellTargetObjectTypes objType, ConditionList* condList)
{
    // this function selects which containers need to be searched for spell target
    uint32 retMask = GRID_MAP_TYPE_MASK_ALL;

    // filter searchers based on searched object type
    switch (objType)
    {
        case TARGET_OBJECT_TYPE_UNIT:
        case TARGET_OBJECT_TYPE_UNIT_AND_DEST:
        case TARGET_OBJECT_TYPE_CORPSE:
        case TARGET_OBJECT_TYPE_CORPSE_ENEMY:
        case TARGET_OBJECT_TYPE_CORPSE_ALLY:
            retMask &= GRID_MAP_TYPE_MASK_PLAYER | GRID_MAP_TYPE_MASK_CORPSE | GRID_MAP_TYPE_MASK_CREATURE;
            break;
        case TARGET_OBJECT_TYPE_GOBJ:
        case TARGET_OBJECT_TYPE_GOBJ_ITEM:
            retMask &= GRID_MAP_TYPE_MASK_GAMEOBJECT;
            break;
        default:
            break;
    }
    if (!(m_spellInfo->AttributesEx2 & SPELL_ATTR2_CAN_TARGET_DEAD))
        retMask &= ~GRID_MAP_TYPE_MASK_CORPSE;
    if (m_spellInfo->AttributesEx3 & SPELL_ATTR3_ONLY_TARGET_PLAYERS)
        retMask &= GRID_MAP_TYPE_MASK_CORPSE | GRID_MAP_TYPE_MASK_PLAYER;
    if (m_spellInfo->AttributesEx3 & SPELL_ATTR3_ONLY_TARGET_GHOSTS)
        retMask &= GRID_MAP_TYPE_MASK_PLAYER;

    if (condList)
        retMask &= sConditionMgr->GetSearcherTypeMaskForConditionList(*condList);
    return retMask;
}

template <typename Searcher>
void Spell::SearchTargets(Searcher& searcher, uint32 containerMask, Unit* referer, Position const* pos, float radius)
{
    if (!containerMask)
        return;

    // search world and grid for possible targets
    bool searchInGrid = containerMask & (GRID_MAP_TYPE_MASK_CREATURE | GRID_MAP_TYPE_MASK_GAMEOBJECT);
    bool searchInWorld = containerMask & (GRID_MAP_TYPE_MASK_CREATURE | GRID_MAP_TYPE_MASK_PLAYER | GRID_MAP_TYPE_MASK_CORPSE);
    if (searchInGrid || searchInWorld)
    {
        float x,y;
        x = pos->GetPositionX();
        y = pos->GetPositionY();

        CellCoord p(Trinity::ComputeCellCoord(x, y));
        Cell cell(p);
        cell.SetNoCreate();

        Map& map = *(referer->GetMap());

        if (searchInWorld)
            cell.Visit(p, Trinity::makeWorldVisitor(searcher), map, radius, x, y);

        if (searchInGrid)
            cell.Visit(p, Trinity::makeGridVisitor(searcher), map, radius, x , y);
    }
}

WorldObject* Spell::SearchNearbyTarget(float range, SpellTargetObjectTypes objectType, SpellTargetCheckTypes selectionType, ConditionList* condList)
{
    WorldObject* target = NULL;
    uint32 containerTypeMask = GetSearcherTypeMask(objectType, condList);
    if (!containerTypeMask)
        return NULL;
    Trinity::WorldObjectSpellNearbyTargetCheck check(range, m_caster, m_spellInfo, selectionType, condList);
    Trinity::WorldObjectLastSearcher<Trinity::WorldObjectSpellNearbyTargetCheck> searcher(m_caster, target, check, containerTypeMask);
    SearchTargets<Trinity::WorldObjectLastSearcher<Trinity::WorldObjectSpellNearbyTargetCheck> > (searcher, containerTypeMask, m_caster, m_caster, range);
    return target;
}

void Spell::SearchAreaTargets(std::list<WorldObject*>& targets, float range, Position const* position, Unit* referer, SpellTargetObjectTypes objectType, SpellTargetCheckTypes selectionType, ConditionList* condList, bool checkAuraStates)
{
    uint32 containerTypeMask = GetSearcherTypeMask(objectType, condList);
    if (!containerTypeMask)
        return;
    Trinity::WorldObjectSpellAreaTargetCheck check(range, position, m_caster, referer, m_spellInfo, selectionType, condList, checkAuraStates);
    Trinity::WorldObjectListSearcher<Trinity::WorldObjectSpellAreaTargetCheck> searcher(m_caster, targets, check, containerTypeMask);
    SearchTargets<Trinity::WorldObjectListSearcher<Trinity::WorldObjectSpellAreaTargetCheck> > (searcher, containerTypeMask, m_caster, position, range);
}

void Spell::SearchChainTargets(std::list<WorldObject*>& targets, uint32 chainTargets, WorldObject* target, SpellTargetObjectTypes objectType, SpellTargetCheckTypes selectType, ConditionList* condList, bool isChainHeal)
{
    // max dist for jump target selection
    float jumpRadius = 0.0f;
    switch (m_spellInfo->DmgClass)
    {
        case SPELL_DAMAGE_CLASS_RANGED:
            // 7.5y for multi shot
            jumpRadius = 7.5f;
            break;
        case SPELL_DAMAGE_CLASS_MELEE:
            // 5y for swipe, cleave and similar
            jumpRadius = 5.0f;
            break;
        case SPELL_DAMAGE_CLASS_NONE:
        case SPELL_DAMAGE_CLASS_MAGIC:
            // 12.5y for chain heal spell since 3.2 patch
            if (isChainHeal)
                jumpRadius = 12.5f;
            // 10y as default for magic chain spells
            else
                jumpRadius = 10.0f;
            break;
    }

    if (auto const modOwner = m_caster->GetSpellModOwner())
        modOwner->ApplySpellMod(GetSpellInfo()->Id, SPELLMOD_JUMP_DISTANCE, jumpRadius, this);

    bool isHavoc = false;
    switch (m_spellInfo->SpellFamilyName)
    {
        case SPELLFAMILY_WARLOCK:
        {
            // Havoc
            if (AuraEffect* aurEff = m_caster->GetAuraEffect(80240, EFFECT_1))
                if (aurEff->GetSpellEffectInfo().SpellClassMask & m_spellInfo->SpellFamilyFlags)
                {
                    if (m_spellInfo->Id == 116858 && aurEff->GetBase()->GetStackAmount() < 3)
                        return;

                    jumpRadius = 50.0f;
                    chainTargets = 1;
                    isHavoc = true;
                }
        }
    }

    // chain lightning/heal spells and similar - allow to jump at larger distance and go out of los
    bool isBouncingFar = (m_spellInfo->AttributesEx4 & SPELL_ATTR4_AREA_TARGET_CHAIN
        || m_spellInfo->DmgClass == SPELL_DAMAGE_CLASS_NONE
        || m_spellInfo->DmgClass == SPELL_DAMAGE_CLASS_MAGIC);

    // max dist which spell can reach
    float searchRadius = jumpRadius;
    if (isBouncingFar)
        searchRadius *= chainTargets;

    std::list<WorldObject*> tempTargets;
    SearchAreaTargets(tempTargets, searchRadius, target, m_caster, objectType, selectType, condList, false);
    tempTargets.remove(target);

    // remove targets which are always invalid for chain spells
    // for some spells allow only chain targets in front of caster (swipe for example)
    if (!isBouncingFar)
    {
        for (std::list<WorldObject*>::iterator itr = tempTargets.begin(); itr != tempTargets.end();)
        {
            std::list<WorldObject*>::iterator checkItr = itr++;
            if (!m_caster->HasInArc(static_cast<float>(M_PI), *checkItr))
                tempTargets.erase(checkItr);
        }
    }

    while (chainTargets)
    {
        // try to get unit for next chain jump
        std::list<WorldObject*>::iterator foundItr = tempTargets.end();
        // get unit with highest hp deficit in dist
        if (isChainHeal)
        {
            uint32 maxHPDeficit = 0;
            for (std::list<WorldObject*>::iterator itr = tempTargets.begin(); itr != tempTargets.end(); ++itr)
            {
                if (Unit* unitTarget = (*itr)->ToUnit())
                {
                    uint32 deficit = unitTarget->GetMaxHealth() - unitTarget->GetHealth();
                    if ((deficit > maxHPDeficit || foundItr == tempTargets.end()) && target->IsWithinDist(unitTarget, jumpRadius) && target->IsWithinLOSInMap(unitTarget))
                    {
                        foundItr = itr;
                        maxHPDeficit = deficit;
                    }
                }
            }
        }
        // get closest object
        else
        {
            // @TODO add script hook
            if (isHavoc)
            {
                for (std::list<WorldObject*>::iterator itr = tempTargets.begin(); itr != tempTargets.end(); ++itr)
                {
                    if (foundItr == tempTargets.end())
                        if (Unit* found = (*itr)->ToUnit())
                            if (target->IsWithinDist(*itr, jumpRadius) && target->IsWithinLOSInMap(*itr) && found->HasAura(80240))
                            {
                                foundItr = itr;
                                break;
                            }
                }
            }
            else
            {
                for (std::list<WorldObject*>::iterator itr = tempTargets.begin(); itr != tempTargets.end(); ++itr)
                {
                    if (foundItr == tempTargets.end())
                    {
                        if ((!isBouncingFar || target->IsWithinDist(*itr, jumpRadius)) && target->IsWithinLOSInMap(*itr))
                            foundItr = itr;
                    }
                    else if (target->GetDistanceOrder(*itr, *foundItr) && target->IsWithinLOSInMap(*itr))
                        foundItr = itr;
                }
            }
        }
        // not found any valid target - chain ends
        if (foundItr == tempTargets.end())
            break;
        target = *foundItr;
        tempTargets.erase(foundItr);
        targets.push_back(target);
        --chainTargets;
    }
}

void Spell::prepareDataForTriggerSystem(AuraEffect const * /*triggeredByAura*/)
{
    //==========================================================================================
    // Now fill data for trigger system, need know:
    // can spell trigger another or not (m_canTrigger)
    // Create base triggers flags for Attacker and Victim (m_procAttacker, m_procVictim and m_procEx)
    //==========================================================================================

    m_procVictim = m_procAttacker = 0;
    // Get data for type of attack and fill base info for trigger
    switch (m_spellInfo->DmgClass)
    {
        case SPELL_DAMAGE_CLASS_MELEE:
            m_procAttacker = PROC_FLAG_DONE_SPELL_MELEE_DMG_CLASS;
            if (m_attackType == OFF_ATTACK)
                m_procAttacker |= PROC_FLAG_DONE_OFFHAND_ATTACK;
            else
                m_procAttacker |= PROC_FLAG_DONE_MAINHAND_ATTACK;
            m_procVictim   = PROC_FLAG_TAKEN_SPELL_MELEE_DMG_CLASS;
            break;
        case SPELL_DAMAGE_CLASS_RANGED:
            // Auto attack
            if (m_spellInfo->AttributesEx2 & SPELL_ATTR2_AUTOREPEAT_FLAG)
            {
                m_procAttacker = PROC_FLAG_DONE_RANGED_AUTO_ATTACK;
                m_procVictim   = PROC_FLAG_TAKEN_RANGED_AUTO_ATTACK;
            }
            else // Ranged spell attack
            {
                m_procAttacker = PROC_FLAG_DONE_SPELL_RANGED_DMG_CLASS;
                m_procVictim   = PROC_FLAG_TAKEN_SPELL_RANGED_DMG_CLASS;
            }
            break;
        default:
            if (m_spellInfo->EquippedItemClass == ITEM_CLASS_WEAPON &&
                m_spellInfo->EquippedItemSubClassMask & (1<<ITEM_SUBCLASS_WEAPON_WAND)
                && m_spellInfo->AttributesEx2 & SPELL_ATTR2_AUTOREPEAT_FLAG) // Wands auto attack
            {
                m_procAttacker = PROC_FLAG_DONE_RANGED_AUTO_ATTACK;
                m_procVictim   = PROC_FLAG_TAKEN_RANGED_AUTO_ATTACK;
            }
            // For other spells trigger procflags are set in Spell::DoAllEffectOnTarget
            // Because spell positivity is dependant on target
    }
    m_procEx = PROC_EX_NONE;

    // Hunter trap spells - activation proc for Lock and Load, Entrapment and Misdirection
    if (m_spellInfo->SpellFamilyName == SPELLFAMILY_HUNTER &&
        (m_spellInfo->SpellFamilyFlags[0] & 0x18 ||     // Freezing and Frost Trap, Freezing Arrow
        m_spellInfo->Id == 57879 ||                     // Snake Trap - done this way to avoid double proc
        m_spellInfo->SpellFamilyFlags[2] & 0x00024000)) // Explosive and Immolation Trap

        m_procAttacker |= PROC_FLAG_DONE_TRAP_ACTIVATION;

    /* Effects which are result of aura proc from triggered spell cannot proc
        to prevent chain proc of these spells */

    // Hellfire Effect - trigger as DOT
    if (m_spellInfo->SpellFamilyName == SPELLFAMILY_WARLOCK && m_spellInfo->SpellFamilyFlags[0] & 0x00000040)
    {
        m_procAttacker = PROC_FLAG_DONE_PERIODIC;
        m_procVictim   = PROC_FLAG_TAKEN_PERIODIC;
    }

    // Ranged autorepeat attack is set as triggered spell - ignore it
    if (!(m_procAttacker & PROC_FLAG_DONE_RANGED_AUTO_ATTACK))
    {
        if (_triggeredCastFlags & TRIGGERED_DISALLOW_PROC_EVENTS &&
            (m_spellInfo->AttributesEx2 & SPELL_ATTR2_TRIGGERED_CAN_TRIGGER_PROC ||
            m_spellInfo->AttributesEx3 & SPELL_ATTR3_TRIGGERED_CAN_TRIGGER_PROC_2))
            m_procEx |= PROC_EX_INTERNAL_CANT_PROC;
        else if (_triggeredCastFlags & TRIGGERED_DISALLOW_PROC_EVENTS)
            m_procEx |= PROC_EX_INTERNAL_TRIGGERED;
    }
    // Totem casts require spellfamilymask defined in spell_proc_event to proc
    if (m_originalCaster && m_caster != m_originalCaster && m_caster->GetTypeId() == TYPEID_UNIT && m_caster->ToCreature()->isTotem() && m_caster->IsControlledByPlayer())
        m_procEx |= PROC_EX_INTERNAL_REQ_FAMILY;
}

void Spell::CleanupTargetList()
{
    m_UniqueTargetInfo.clear();
    m_UniqueGOTargetInfo.clear();
    m_UniqueItemInfo.clear();
    m_delayMoment = 0;
}

void Spell::AddUnitTarget(Unit* target, uint32 effectMask, bool checkIfValid /*= true*/, bool implicit /*= true*/, uint8 effectIndex /* = EFFECT_0 */)
{
    for (uint32 i = 0; i < m_spellInfo->Effects.size(); ++i)
        if (!m_spellInfo->Effects[i].IsEffect() || !CheckEffectTarget(target, i))
            effectMask &= ~(1 << i);

    // no effects left
    if (!effectMask)
        return;

    if (checkIfValid)
        if (m_spellInfo->CheckTarget(m_caster, target, IsTriggered() ? true: implicit) != SPELL_CAST_OK)
            return;

    // Check for effect immune skip if immuned
    for (uint32 i = 0; i < m_spellInfo->Effects.size(); ++i)
        if (target->IsImmunedToSpellEffect(m_spellInfo, i))
            effectMask &= ~(1 << i);

    // Quest item spell 'Kill Golden Stonefish'
    if (m_spellInfo->Id == 80962)
    {
        if (!target->ToCreature() || target->ToCreature()->GetEntry() != 43331)
            return;
    }

    uint64 targetGUID = target->GetGUID();

    // Lookup target in already in list
    for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
    {
        if (targetGUID == ihit->targetGUID)             // Found in list
        {
            ihit->effectMask |= effectMask;             // Immune effects removed from mask
            ihit->scaleAura = false;
            if (m_auraScaleMask && ihit->effectMask == m_auraScaleMask && m_caster != target)
            {
                SpellInfo const* auraSpell = sSpellMgr->GetSpellInfo(sSpellMgr->GetFirstSpellInChain(m_spellInfo->Id));
                if (uint32(target->getLevel() + 10) >= auraSpell->SpellLevel)
                    ihit->scaleAura = true;
            }
            return;
        }
    }

    // This is new target calculate data for him

    // Get spell hit result on target
    TargetInfo targetInfo;
    targetInfo.targetGUID = targetGUID;                         // Store target GUID
    targetInfo.effectMask = effectMask;                         // Store all effects not immune
    targetInfo.processed  = false;                              // Effects not apply on target
    targetInfo.alive      = target->IsAlive();
    targetInfo.damage     = 0;
    targetInfo.crit       = false;
    targetInfo.scaleAura  = false;
    targetInfo.timeDelay  = 0;

    if (m_auraScaleMask && targetInfo.effectMask == m_auraScaleMask && m_caster != target)
    {
        SpellInfo const* auraSpell = sSpellMgr->GetSpellInfo(sSpellMgr->GetFirstSpellInChain(m_spellInfo->Id));
        if (uint32(target->getLevel() + 10) >= auraSpell->SpellLevel)
            targetInfo.scaleAura = true;
    }

    // Calculate hit result
    if (m_originalCaster)
    {
        targetInfo.missCondition = m_originalCaster->SpellHitResult(target, m_spellInfo, m_canReflect);
        if (m_skipCheck && targetInfo.missCondition != SPELL_MISS_IMMUNE)
            targetInfo.missCondition = SPELL_MISS_NONE;
    }
    else
        targetInfo.missCondition = SPELL_MISS_EVADE; //SPELL_MISS_NONE;

    // Spell have speed - need calculate incoming time
    // Incoming time is zero for self casts. At least I think so.
    if (m_spellInfo->Effects[0].Effect == SPELL_EFFECT_KNOCK_BACK)
    {
        m_delayMoment = 1;
        targetInfo.timeDelay = 0;
    }
    else if (m_spellInfo->Speed > 0.0f && m_caster != target)
    {
        // calculate spell incoming interval
        // TODO: this is a hack
        float dist = m_caster->GetDistance(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());

        if (dist < 5.0f)
            dist = 5.0f;

        if (!(m_spellInfo->AttributesEx9 & SPELL_ATTR9_SPECIAL_DELAY_CALCULATION))
            targetInfo.timeDelay = uint64(floor(dist / m_spellInfo->Speed * 1000.0f));
        else
            targetInfo.timeDelay = uint64(m_spellInfo->Speed * 1000.0f);

        // Calculate minimum incoming time
        if (m_delayMoment == 0 || m_delayMoment > targetInfo.timeDelay)
            m_delayMoment = targetInfo.timeDelay;
    }
    else if ((m_caster->GetTypeId() == TYPEID_PLAYER || m_caster->ToCreature()->isPet()) && m_caster != target)
    {
        if (!IsTriggered() && m_spellInfo->_IsCrowdControl(0, false))
        {
            targetInfo.timeDelay = 100LL;
            m_delayMoment = 100LL;
        }
        else if (m_spellInfo->Id == 24259) // Spell Lock - Silenced
        {
            targetInfo.timeDelay = 100LL;
            m_delayMoment = 100LL;
        }
    }
    // Removing Death Grip cooldown
    else if (m_spellInfo->Id == 90289)
    {
        targetInfo.timeDelay = 100LL;
        m_delayMoment = 100LL;
    }
    // Misdirection
    else if (m_spellInfo->Id == 35079 || m_spellInfo->Id == 57934)
    {
        targetInfo.timeDelay = 100LL;
        m_delayMoment = 100LL;
    }
    else
        targetInfo.timeDelay = 0LL;

    // If target reflect spell back to caster
    if (targetInfo.missCondition == SPELL_MISS_REFLECT)
    {
        // process reflect removal (not delayed)
        if (!targetInfo.timeDelay)
        {
            m_caster->ProcDamageAndSpell(target, PROC_FLAG_NONE, PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_NEG, PROC_EX_REFLECT, 1, 0, BASE_ATTACK, m_spellInfo);
            if (m_spellInfo->Id == 2136) // hack to trigger impact in reflect
            {
                m_caster->ProcDamageAndSpell(m_caster, PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_NEG,
                    PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_NEG, PROC_EX_NORMAL_HIT, 1, 0, BASE_ATTACK, m_spellInfo);
            }
        }
        // Calculate reflected spell result on caster
        targetInfo.reflectResult = m_caster->SpellHitResult(m_caster, m_spellInfo, m_canReflect);

        if (targetInfo.reflectResult == SPELL_MISS_REFLECT)     // Impossible reflect again, so simply deflect spell
            targetInfo.reflectResult = SPELL_MISS_PARRY;

        // Increase time interval for reflected spells by 1.5
        targetInfo.timeDelay += targetInfo.timeDelay >> 1;
    }
    else
        targetInfo.reflectResult = SPELL_MISS_NONE;

    // Add target to list
    m_UniqueTargetInfo.push_back(targetInfo);
}

void Spell::AddGOTarget(GameObject* go, uint32 effectMask)
{
    for (uint32 i = 0; i < m_spellInfo->Effects.size(); ++i)
    {
        if (!m_spellInfo->Effects[i].IsEffect())
        {
            effectMask &= ~(1 << i);
        }
        else
        {
            switch (m_spellInfo->Effects[i].Effect)
            {
                case SPELL_EFFECT_GAMEOBJECT_DAMAGE:
                case SPELL_EFFECT_GAMEOBJECT_REPAIR:
                case SPELL_EFFECT_GAMEOBJECT_SET_DESTRUCTION_STATE:
                    if (go->GetGoType() != GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING)
                        effectMask &= ~(1 << i);
                    break;
                default:
                    break;
            }
        }
    }

    if (!effectMask)
        return;

    uint64 targetGUID = go->GetGUID();

    // Lookup target in already in list
    for (auto ihit = m_UniqueGOTargetInfo.begin(); ihit != m_UniqueGOTargetInfo.end(); ++ihit)
    {
        if (targetGUID == ihit->targetGUID)                 // Found in list
        {
            ihit->effectMask |= effectMask;                 // Add only effect mask
            return;
        }
    }

    // This is new target calculate data for him

    GOTargetInfo target;
    target.targetGUID = targetGUID;
    target.effectMask = effectMask;
    target.processed  = false;                              // Effects not apply on target

    // Spell have speed - need calculate incoming time
    if (m_spellInfo->Speed > 0.0f)
    {
        // calculate spell incoming interval
        float dist = m_caster->GetDistance(go->GetPositionX(), go->GetPositionY(), go->GetPositionZ());
        if (dist < 5.0f)
            dist = 5.0f;

       if (!(m_spellInfo->AttributesEx9 & SPELL_ATTR9_SPECIAL_DELAY_CALCULATION))
           target.timeDelay = uint64(floor(dist / m_spellInfo->Speed * 1000.0f));
       else
           target.timeDelay = uint64(m_spellInfo->Speed * 1000.0f);

        if (m_delayMoment == 0 || m_delayMoment > target.timeDelay)
            m_delayMoment = target.timeDelay;
    }
    else
        target.timeDelay = 0LL;

    // Add target to list
    m_UniqueGOTargetInfo.push_back(target);
}

void Spell::AddItemTarget(Item* item, uint32 effectMask)
{
    for (uint32 i = 0; i < m_spellInfo->Effects.size(); ++i)
        if (!m_spellInfo->Effects[i].IsEffect())
            effectMask &= ~(1 << i);

    // no effects left
    if (!effectMask)
        return;

    // Lookup target in already in list
    for (auto ihit = m_UniqueItemInfo.begin(); ihit != m_UniqueItemInfo.end(); ++ihit)
    {
        if (item == ihit->item)                            // Found in list
        {
            ihit->effectMask |= effectMask;                 // Add only effect mask
            return;
        }
    }

    // This is new target add data

    ItemTargetInfo target;
    target.item       = item;
    target.effectMask = effectMask;

    m_UniqueItemInfo.push_back(target);
}

void Spell::AddDestTarget(SpellDestination const& dest, uint32 effIndex)
{
    m_destTargets[effIndex] = dest;
}

void Spell::DoAllEffectOnTarget(TargetInfo* target)
{
    if (!target || target->processed)
        return;

    target->processed = true;                               // Target checked in apply effects procedure

    // Get mask of effects for target
    uint32 mask = target->effectMask;

    Unit* unit = m_caster->GetGUID() == target->targetGUID ? m_caster : ObjectAccessor::GetUnit(*m_caster, target->targetGUID);
    if (!unit)
    {
        uint32 farMask = 0;
        // create far target mask
        for (uint8 i = 0; i < m_spellInfo->Effects.size(); ++i)
            if (m_spellInfo->Effects[i].IsFarUnitTargetEffect())
                if ((1 << i) & mask)
                    farMask |= (1 << i);

        if (!farMask)
            return;

        // find unit in world
        unit = ObjectAccessor::FindUnit(target->targetGUID);
        if (!unit)
            return;

        // do far effects on the unit
        // can't use default call because of threading, do stuff as fast as possible
        for (uint8 i = 0; i < m_spellInfo->Effects.size(); ++i)
            if (farMask & (1 << i))
                HandleEffects(unit, NULL, NULL, i, SPELL_EFFECT_HANDLE_HIT_TARGET);
        return;
    }

    if (unit->IsAlive() != target->alive)
        return;

    if (getState() == SPELL_STATE_DELAYED && !m_spellInfo->IsPositive() && (getMSTime() - target->timeDelay) <= unit->m_lastSanctuaryTime)
        return;                                             // No missinfo in that case

    // Some spells should remove Camouflage after hit (traps, some spells that have casting time)
    if (target->targetGUID != m_caster->GetGUID() && m_spellInfo && m_spellInfo->IsBreakingCamouflageAfterHit())
    {
        if (TempSummon* summon = m_caster->ToTempSummon())
        {
            if (Unit* owner = summon->GetSummoner())
                owner->RemoveAurasByType(SPELL_AURA_MOD_CAMOUFLAGE);
        }
        else
            m_caster->RemoveAurasByType(SPELL_AURA_MOD_CAMOUFLAGE);
    }

    // Get original caster (if exist) and calculate damage/healing from him data
    Unit* caster = m_originalCaster ? m_originalCaster : m_caster;

    // Skip if m_originalCaster not avaiable
    if (!caster)
        return;

    SpellMissInfo missInfo = target->missCondition;

    // Need init unitTarget by default unit (can changed in code on reflect)
    // Or on missInfo != SPELL_MISS_NONE unitTarget undefined (but need in trigger subsystem)
    unitTarget = unit;

    // Reset damage/healing counter
    m_damage = target->damage;
    m_healing = -target->damage;

    // Fill base trigger info
    uint32 procAttacker = m_procAttacker;
    uint32 procVictim   = m_procVictim;
    uint32 procEx = m_procEx;

    m_spellAura = NULL; // Set aura to null for every target-make sure that pointer is not used for unit without aura applied

                            //Spells with this flag cannot trigger if effect is casted on self
    bool canEffectTrigger = !(m_spellInfo->AttributesEx3 & SPELL_ATTR3_CANT_TRIGGER_PROC) && unitTarget->CanProc() && CanExecuteTriggersOnHit(mask);
    Unit* spellHitTarget = NULL;

    if (missInfo == SPELL_MISS_NONE)                          // In case spell hit target, do all effect on that target
        spellHitTarget = unit;
    else if (missInfo == SPELL_MISS_REFLECT)                // In case spell reflect from target, do all effect on caster (if hit)
    {
        if (target->reflectResult == SPELL_MISS_NONE)       // If reflected spell hit caster -> do all effect on him
        {
            spellHitTarget = m_caster;
            // Start triggers for remove charges if need (trigger only for victim, and mark as active spell)
            m_caster->ProcDamageAndSpell(unit, PROC_FLAG_NONE, PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_NEG, PROC_EX_REFLECT, 1, 0, BASE_ATTACK, m_spellInfo);
            if (m_caster->GetTypeId() == TYPEID_UNIT)
                m_caster->ToCreature()->LowerPlayerDamageReq(target->damage);
        }
    }

    if (spellHitTarget)
    {
        SpellMissInfo missInfo2 = DoSpellHitOnUnit(spellHitTarget, mask, target->scaleAura);
        if (missInfo2 != SPELL_MISS_NONE)
        {
            if (missInfo2 != SPELL_MISS_MISS)
                m_caster->SendSpellMiss(unit, m_spellInfo->Id, missInfo2);
            m_damage = 0;
            spellHitTarget = NULL;
        }
    }

    // Your finishing moves restore X Energy per combo
    if (m_needComboPoints || m_spellInfo->Id == 127538)
    {
        if (Player* plrCaster = m_caster->ToPlayer())
        {
            if (uint8 cp = plrCaster->GetComboPoints())
            {
                // Relentless Strikes
                if (plrCaster->HasAura(58423))
                {
                    if (roll_chance_i(cp * 20))
                    {
                        if (!plrCaster->HasSpellCooldown(98440))
                        {
                            plrCaster->CastSpell(plrCaster, 98440, true); // Restore 25 energy
                            plrCaster->AddSpellCooldown(98440, 0, 1 * IN_MILLISECONDS); // Prevent double cast
                        }
                    }
                }
                // Soul of the Forest - 4 Energy
                else if (plrCaster->HasAura(114107))
                {
                    if (plrCaster->GetSpecializationId(plrCaster->GetActiveSpec()) == SPEC_DRUID_FERAL)
                        plrCaster->EnergizeBySpell(plrCaster, 114107, 4 * cp, POWER_ENERGY);
                }
            }
        }
    }

    // Refresh procEx after effect-handlers
    procEx |= m_procEx;

    // Do not take combo points on dodge and miss
    if (missInfo != SPELL_MISS_NONE && m_needComboPoints &&
            m_targets.GetUnitTargetGUID() == target->targetGUID)
    {
        m_needComboPoints = false;
        // Restore spell mods for a miss/dodge/parry Cold Blood
        // TODO: check how broad this rule should be
        if (m_caster->GetTypeId() == TYPEID_PLAYER && (missInfo == SPELL_MISS_MISS ||
                missInfo == SPELL_MISS_DODGE || missInfo == SPELL_MISS_PARRY))
            m_caster->ToPlayer()->RestoreSpellMods(*this, 14177);
    }

    // Trigger info was not filled in spell::preparedatafortriggersystem - we do it now
    if (canEffectTrigger && !procAttacker && !procVictim)
    {
        bool positive = true;
        if (m_damage > 0)
            positive = false;
        else if (!m_healing)
        {
            for (uint8 i = 0; i < GetSpellInfo()->Effects.size(); ++i)
                // If at least one effect negative spell is negative hit
                if (mask & (1 << i) && !m_spellInfo->IsPositiveEffect(i))
                {
                    positive = false;
                    break;
                }
        }
        switch (m_spellInfo->DmgClass)
        {
            case SPELL_DAMAGE_CLASS_MAGIC:
                if (positive)
                {
                    procAttacker |= PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS;
                    procVictim   |= PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_POS;
                }
                else
                {
                    procAttacker |= PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_NEG;
                    procVictim   |= PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_NEG;
                }
            break;
            case SPELL_DAMAGE_CLASS_NONE:
                if (positive)
                {
                    procAttacker |= PROC_FLAG_DONE_SPELL_NONE_DMG_CLASS_POS;
                    procVictim   |= PROC_FLAG_TAKEN_SPELL_NONE_DMG_CLASS_POS;
                }
                else
                {
                    procAttacker |= PROC_FLAG_DONE_SPELL_NONE_DMG_CLASS_NEG;
                    procVictim   |= PROC_FLAG_TAKEN_SPELL_NONE_DMG_CLASS_NEG;
                }
            break;
        }
    }
    CallScriptOnHitHandlers();

    // All calculated do it!
    // Do healing and triggers
    if (m_healing > 0)
    {
        bool crit = target->crit;
        uint32 addhealth = m_healing;
        if (crit)
        {
            procEx |= PROC_EX_CRITICAL_HIT;
            addhealth = caster->SpellCriticalHealingBonus(m_spellInfo, addhealth, NULL);
        }
        else
            procEx |= PROC_EX_NORMAL_HIT;

        int32 gain = caster->HealBySpell(unitTarget, m_spellInfo, addhealth, crit);
        unitTarget->getHostileRefManager().threatAssist(caster, float(gain) * 0.5f, m_spellInfo);
        m_healing = gain;

        // Do triggers for unit (reflect triggers passed on hit phase for correct drop charge)
        if (canEffectTrigger && missInfo != SPELL_MISS_REFLECT)
            caster->ProcDamageAndSpell(unitTarget, procAttacker, procVictim, procEx, addhealth, 0, m_attackType, m_spellInfo, m_triggeredByAuraSpell, true);
    }
    // Do damage and triggers
    else if (m_damage > 0)
    {
        // Fill base damage struct (unitTarget - is real spell target)
        SpellNonMeleeDamage damageInfo(caster, unitTarget, m_spellInfo->Id, m_spellSchoolMask);

        // Add bonuses and fill damageInfo struct
        caster->CalculateSpellDamageTaken(&damageInfo, m_damage, m_spellInfo, m_attackType, target->crit);
        caster->DealDamageMods(damageInfo.target, damageInfo.damage, &damageInfo.absorb);

        // Send log damage message to client
        caster->SendSpellNonMeleeDamageLog(&damageInfo);

        procEx |= createProcExtendMask(&damageInfo, missInfo);
        procVictim |= PROC_FLAG_TAKEN_DAMAGE;

        // Do triggers for unit (reflect triggers passed on hit phase for correct drop charge)
        if (canEffectTrigger && missInfo != SPELL_MISS_REFLECT)
        {
            caster->ProcDamageAndSpell(unitTarget, procAttacker, procVictim, procEx, damageInfo.damage, damageInfo.absorb, m_attackType, m_spellInfo, m_triggeredByAuraSpell);
            if (caster->GetTypeId() == TYPEID_PLAYER && (m_spellInfo->Attributes & SPELL_ATTR0_STOP_ATTACK_TARGET) == 0 &&
               (m_spellInfo->DmgClass == SPELL_DAMAGE_CLASS_MELEE || m_spellInfo->DmgClass == SPELL_DAMAGE_CLASS_RANGED))
                caster->ToPlayer()->CastItemCombatSpell(unitTarget, m_attackType, procVictim, procEx);
        }

        m_damage = damageInfo.damage;

        caster->DealSpellDamage(&damageInfo, true);

        // Used in spell scripts
        m_final_damage = damageInfo.damage;
        m_absorbed_damage = damageInfo.absorb;
    }
    // Passive spell hits/misses or active spells only misses (only triggers)
    else
    {
        // Fill base damage struct (unitTarget - is real spell target)
        SpellNonMeleeDamage damageInfo(caster, unitTarget, m_spellInfo->Id, m_spellSchoolMask);
        procEx |= createProcExtendMask(&damageInfo, missInfo);
        // Do triggers for unit (reflect triggers passed on hit phase for correct drop charge)
        if (canEffectTrigger && missInfo != SPELL_MISS_REFLECT)
            caster->ProcDamageAndSpell(unit, procAttacker, procVictim, procEx, 0, 0, m_attackType, m_spellInfo, m_triggeredByAuraSpell);

        // Failed Pickpocket, reveal rogue
        if (missInfo == SPELL_MISS_RESIST && m_spellInfo->AttributesCu & SPELL_ATTR0_CU_PICKPOCKET && unitTarget->GetTypeId() == TYPEID_UNIT)
        {
            m_caster->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_TALK);
            if (unitTarget->ToCreature()->IsAIEnabled)
                unitTarget->ToCreature()->AI()->AttackStart(m_caster);
        }
    }

    if (!m_spellInfo->IsIgnoringCombat() && missInfo != SPELL_MISS_EVADE && !m_caster->IsFriendlyTo(unit) && (!m_spellInfo->IsPositive() || m_spellInfo->HasEffect(SPELL_EFFECT_DISPEL)))
    {
        m_caster->CombatStart(unit, !(m_spellInfo->AttributesEx3 & SPELL_ATTR3_NO_INITIAL_AGGRO));

        if (m_spellInfo->AttributesCu & SPELL_ATTR0_CU_AURA_CC)
            if (!unit->IsStandState())
                unit->SetStandState(UNIT_STAND_STATE_STAND);
    }

    switch (m_caster->getClass())
    {
        case CLASS_DRUID:
            if (AuraEffect* mastery = m_caster->GetAuraEffect(77495, EFFECT_0))
            {
                if (mastery->GetBase()->GetEffect(EFFECT_0)->GetSpellEffectInfo().SpellClassMask & m_spellInfo->SpellFamilyFlags)
                {
                    int32 bp = mastery->GetAmount();
                    m_caster->CastCustomSpell(m_caster, 100977, &bp, &bp, NULL, true);
                }
            }
            break;
    }

    if (spellHitTarget)
    {
        //AI functions
        if (spellHitTarget->GetTypeId() == TYPEID_UNIT)
            if (spellHitTarget->ToCreature()->IsAIEnabled)
                spellHitTarget->ToCreature()->AI()->SpellHit(m_caster, m_spellInfo);

        if (m_caster->GetTypeId() == TYPEID_UNIT && m_caster->ToCreature()->IsAIEnabled)
            m_caster->ToCreature()->AI()->SpellHitTarget(spellHitTarget, m_spellInfo);

        // Needs to be called after dealing damage/healing to not remove breaking on damage auras
        DoTriggersOnSpellHit(spellHitTarget, mask);

        // if target is fallged for pvp also flag caster if a player
        if (unit->IsPvP() && m_caster->GetTypeId() == TYPEID_PLAYER)
            m_caster->ToPlayer()->UpdatePvP(true);

        CallScriptAfterHitHandlers();
    }
}

SpellMissInfo Spell::DoSpellHitOnUnit(Unit* unit, uint32 effectMask, bool scaleAura)
{
    if (!unit || !effectMask)
        return SPELL_MISS_EVADE;

    // For delayed spells immunity may be applied between missile launch and hit - check immunity for that case
    if (m_spellInfo->Speed && (unit->IsImmunedToDamage(m_spellInfo) || unit->IsImmunedToSpell(m_spellInfo)))
        return SPELL_MISS_IMMUNE;

    // Hack fix for Deterrence and charge stun effect
    if ((m_spellInfo->Id == 105771 || m_spellInfo->Id == 7922) && unit->HasAura(19263))
        return SPELL_MISS_MISS;

    // disable effects to which unit is immune
    SpellMissInfo returnVal = SPELL_MISS_IMMUNE;
    for (uint32 i = 0; i < GetSpellInfo()->Effects.size(); ++i)
        if (effectMask & (1 << i))
            if (unit->IsImmunedToSpellEffect(m_spellInfo, i))
                effectMask &= ~(1 << i);

    if (!effectMask)
        return returnVal;

    PrepareScriptHitHandlers();
    CallScriptBeforeHitHandlers();

    if (unit->GetTypeId() == TYPEID_PLAYER)
    {
        unit->ToPlayer()->GetAchievementMgr().StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_SPELL_TARGET, m_spellInfo->Id);
        unit->ToPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, m_spellInfo->Id, 0, 0, m_caster);
        unit->ToPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2, m_spellInfo->Id, 0, 0, m_caster);
    }

    if (m_caster->GetTypeId() == TYPEID_PLAYER)
    {
        m_caster->ToPlayer()->GetAchievementMgr().StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_SPELL_CASTER, m_spellInfo->Id);
        m_caster->ToPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2, m_spellInfo->Id, 0, 0, unit);
    }

    if (m_caster != unit)
    {
        // Recheck  UNIT_FLAG_NON_ATTACKABLE for delayed spells
        if (m_spellInfo->Speed > 0.0f && unit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE) && unit->GetCharmerOrOwnerGUID() != m_caster->GetGUID())
            return SPELL_MISS_EVADE;

        if (m_caster->_IsValidAttackTarget(unit, m_spellInfo))
        {
            if (m_spellInfo->IsBreakingStealth(unit))
            {
                unit->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_HITBYSPELL);
                //TODO: This is a hack. But we do not know what types of stealth should be interrupted by CC
                if (m_spellInfo->HasCustomAttribute(SPELL_ATTR0_CU_AURA_CC) && unit->IsControlledByPlayer())
                    unit->RemoveAurasByType(SPELL_AURA_MOD_STEALTH, 0, NULL, 131369);
            }
            if (m_spellInfo->HasCustomAttribute(SPELL_ATTR0_CU_BINARY) && !m_spellInfo->IsChanneled())
                if (m_originalCaster->IsSpellResisted(unit, m_spellSchoolMask, m_spellInfo))
                    return SPELL_MISS_RESIST;
        }
        else if (m_caster->IsFriendlyTo(unit))
        {
            // for delayed spells ignore negative spells (after duel end) for friendly targets
            // TODO: this cause soul transfer bugged
            if (m_spellInfo->Speed > 0.0f && unit->GetTypeId() == TYPEID_PLAYER && !m_spellInfo->IsPositive())
                return SPELL_MISS_EVADE;

            // assisting case, healing and resurrection
            if (unit->HasUnitState(UNIT_STATE_ATTACK_PLAYER))
            {
                m_caster->SetContestedPvP();
                if (m_caster->GetTypeId() == TYPEID_PLAYER)
                    m_caster->ToPlayer()->UpdatePvP(true);
            }
            if (unit->IsInCombat() && !(m_spellInfo->AttributesEx3 & SPELL_ATTR3_NO_INITIAL_AGGRO) && !m_spellInfo->IsIgnoringCombat())
            {
                m_caster->SetInCombatState(unit->GetCombatTimer() > 0, unit);
                unit->getHostileRefManager().threatAssist(m_caster, 0.0f);
            }
        }
    }
    else if (!m_spellInfo->IsPositive())
    {
        if (m_spellInfo->HasCustomAttribute(SPELL_ATTR0_CU_BINARY) && !m_spellInfo->IsChanneled())
            if (m_originalCaster->IsSpellResisted(unit, m_spellSchoolMask, m_spellInfo))
                return SPELL_MISS_RESIST;
    }

    uint32 aura_effmask = 0;
    for (uint8 i = 0; i < m_spellInfo->Effects.size(); ++i)
        if (effectMask & (1 << i) && m_spellInfo->Effects[i].IsUnitOwnedAuraEffect())
            aura_effmask |= 1 << i;

    if (aura_effmask)
    {
        // Get Data Needed for Diminishing Returns, some effects may have multiple auras, so this must be done on spell hit, not aura add
        m_diminishGroup = GetDiminishingReturnsGroupForSpell(m_spellInfo, m_triggeredByAuraSpell);
        if (m_diminishGroup)
        {
            m_diminishLevel = DIMINISHING_LEVEL_1;
            m_diminishLevel = unit->GetDiminishing(m_diminishGroup);
            DiminishingReturnsType type = GetDiminishingReturnsGroupType(m_diminishGroup);
            // Increase Diminishing on unit, current informations for actually casts will use values above
            if ((type == DRTYPE_PLAYER && (unit->GetCharmerOrOwnerPlayerOrPlayerItself() || (unit->GetTypeId() == TYPEID_UNIT && unit->ToCreature()->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_ALL_DIMINISH))) || type == DRTYPE_ALL)
                unit->IncrDiminishing(m_diminishGroup);
        }

        // Select rank for aura with level requirements only in specific cases
        // Unit has to be target only of aura effect, both caster and target have to be players, target has to be other than unit target
        SpellInfo const* scaledSpellInfo = m_spellInfo;
        int32 basePoints[MAX_SPELL_EFFECTS];

        if (scaleAura)
        {
            scaledSpellInfo = m_spellInfo->GetAuraRankForLevel(unitTarget->getLevel());
            ASSERT(scaledSpellInfo);

            auto const scaledEffectsLast = std::end(scaledSpellInfo->Effects);
            auto const effectsLast = std::end(m_spellInfo->Effects);

            auto i = std::begin(scaledSpellInfo->Effects);
            auto j = std::begin(m_spellInfo->Effects);

            auto basePointsPtr = std::begin(basePoints);

            for (; i != scaledEffectsLast && j != effectsLast; ++i, ++j)
            {
                if (i->Effect != j->Effect)
                    break;
                *basePointsPtr++ = i->BasePoints;
            }

            if (i != scaledEffectsLast || j != effectsLast)
                scaledSpellInfo = m_spellInfo;
        }

        if (m_originalCaster)
        {
            bool refresh = false;
            m_spellAura = Aura::TryRefreshStackOrCreate(scaledSpellInfo, effectMask, unit,
                m_originalCaster, m_spellPowerData, (scaledSpellInfo == m_spellInfo) ? m_spellValue.EffectBasePoints : basePoints, m_CastItem, 0, &refresh);
            if (m_spellAura)
            {
                // Set aura stack amount to desired value
                if (m_spellValue.AuraStackAmount > 1)
                {
                    if (!refresh)
                        m_spellAura->SetStackAmount(m_spellValue.AuraStackAmount);
                    else
                        m_spellAura->ModStackAmount(m_spellValue.AuraStackAmount - 1);
                }

                if (float damageModifier = m_periodicDamageModifier)
                {
                    for (uint8 i = 0; i < m_spellAura->GetSpellInfo()->Effects.size(); ++i)
                    {
                        auto auraEffect = m_spellAura->GetEffect(i);
                        if (auraEffect && auraEffect->HasFixedDamageInfo())
                            if (int32 oldDamage = auraEffect->GetFixedDamageInfo().GetFixedDamage())
                                auraEffect->GetFixedDamageInfo().SetFixedDamage(int32(oldDamage * damageModifier));
                    }
                }

                // Now Reduce spell duration using data received at spell hit
                int32 duration = m_spellAura->GetMaxDuration();
                int32 limitduration = GetDiminishingReturnsLimitDuration(m_diminishGroup, scaledSpellInfo);
                float diminishMod = unit->ApplyDiminishingToDuration(m_diminishGroup, duration, m_originalCaster, m_diminishLevel, limitduration);

                // unit is immune to aura if it was diminished to 0 duration
                if (diminishMod == 0.0f)
                {
                    m_spellAura->Remove();
                    bool found = false;
                    for (uint8 i = 0; i < m_spellInfo->Effects.size(); ++i)
                        if (effectMask & (1 << i) && m_spellInfo->Effects[i].Effect != SPELL_EFFECT_APPLY_AURA)
                            found = true;
                    if (!found)
                        return SPELL_MISS_IMMUNE;
                }
                else
                {
                    ((UnitAura*)m_spellAura)->SetDiminishGroup(m_diminishGroup);

                    bool positive = m_spellAura->GetSpellInfo()->IsPositive();
                    if (AuraApplication* aurApp = m_spellAura->GetApplicationOfTarget(m_originalCaster->GetGUID()))
                        positive = aurApp->IsPositive();

                    duration = m_originalCaster->ModSpellDuration(scaledSpellInfo, unit, duration, positive, effectMask);

                    if (duration > 0)
                    {
                        // Haste modifies duration of channeled spells
                        if (m_spellInfo->IsChanneled())
                        {
                            if (m_spellInfo->AttributesEx5 & SPELL_ATTR5_HASTE_AFFECT_DURATION)
                                m_originalCaster->ModSpellCastTime(scaledSpellInfo, duration, this);
                        }
                        else if (m_spellInfo->AttributesEx5 & SPELL_ATTR5_HASTE_AFFECT_DURATION)
                        {
                            int32 origDuration = duration;
                            duration = 0;
                            for (uint8 i = 0; i < m_spellAura->GetSpellInfo()->Effects.size(); ++i)
                            {
                                auto const eff = m_spellAura->GetEffect(i);

                                // amplitude is hastened by UNIT_MOD_CAST_SPEED
                                if (eff && eff->GetAmplitude())
                                    duration = int32(origDuration * (2.0f - m_originalCaster->GetFloatValue(UNIT_MOD_CAST_SPEED)));
                            }

                            // if there is no periodic effect
                            if (!duration)
                                duration = int32(origDuration * (2.0f - m_originalCaster->GetFloatValue(UNIT_MOD_CAST_SPEED)));
                        }
                    }

                    if (duration != m_spellAura->GetMaxDuration())
                    {
                        m_spellAura->SetMaxDuration(duration);
                        m_spellAura->SetDuration(duration);
                    }

                    if (isStolen && (!duration || duration == -1 || duration > 60000))
                    {
                        duration = 60000;
                        m_spellAura->SetMaxDuration(duration);
                        m_spellAura->SetDuration(duration);
                    }

                    m_spellAura->_RegisterForTargets();

                    if (m_spellInfo->IsChanneled())
                        if (!m_spellInfo->IsPositive() && m_spellInfo->HasEffect(SPELL_EFFECT_APPLY_AURA))
                            SendChannelStart(duration);
                }
            }
        }
    }

    for (uint32 i = 0; i < GetSpellInfo()->Effects.size(); ++i)
        if (effectMask & (1 << i))
            HandleEffects(unit, NULL, NULL, i, SPELL_EFFECT_HANDLE_HIT_TARGET);

    return SPELL_MISS_NONE;
}

void Spell::DoTriggersOnSpellHit(Unit* unit, uint32 effMask)
{
    // Apply additional spell effects to target
    // TODO: move this code to scripts
    // Blizz seems to just apply aura without bothering to cast
    if (m_preCastSpell)
        if (!m_spellInfo->IsWrongPrecastSpell(sSpellMgr->GetSpellInfo(m_preCastSpell)))
            if (sSpellMgr->GetSpellInfo(m_preCastSpell))
                m_caster->AddAura(m_preCastSpell, unit);

    // handle SPELL_AURA_ADD_TARGET_TRIGGER auras
    // this is executed after spell proc spells on target hit
    // spells are triggered for each hit spell target
    // info confirmed with retail sniffs of permafrost and shadow weaving
    if (!m_hitTriggerSpells.empty())
    {
        int32 duration = 0;
        for (HitTriggerSpellList::const_iterator i = m_hitTriggerSpells.begin(); i != m_hitTriggerSpells.end(); ++i)
        {
            if (!CanExecuteTriggersOnHit(effMask, i->triggeredByAura) || !roll_chance_i(i->chance))
                continue;

            TC_LOG_DEBUG("spells", "Spell %u triggered spell %u by SPELL_AURA_ADD_TARGET_TRIGGER aura", m_spellInfo->Id, i->triggeredSpell->Id);

            auto triggeredAur = unit->GetAura(i->triggeredSpell->Id, m_caster->GetGUID());

            if (triggeredAur)
            {
                triggeredAur->RefreshDuration();
            }
            else
            {
                // Ruthlessness - self-casted effects handled in other place
                if (m_caster == unit && i->triggeredByAura->Id == 14161)
                    continue;

                m_caster->CastSpell(unit, i->triggeredSpell, true);
                if (!(triggeredAur = unit->GetAura(i->triggeredSpell->Id, m_caster->GetGUID())))
                    continue;
            }

            // SPELL_AURA_ADD_TARGET_TRIGGER auras shouldn't trigger auras without duration
            // set duration of current aura to the triggered spell
            if (!triggeredAur->IsPermanent())
                continue;

            // get duration from aura-only once
            if (duration == 0)
            {
                Aura const *aur = unit->GetAura(m_spellInfo->Id, m_caster->GetGUID());
                duration = aur ? aur->GetDuration() : -1;
            }

            triggeredAur->SetMaxDuration(duration);
            triggeredAur->RefreshDuration();
        }
    }

    // trigger linked auras remove/apply
    // TODO: remove/cleanup this, as this table is not documented and people are doing stupid things with it
    if (auto const spellTriggered = sSpellMgr->GetSpellLinked(m_spellInfo->Id + SPELL_LINK_HIT)) {
        for (auto id : *spellTriggered) {
            if (id < 0)
                unit->RemoveAurasDueToSpell(-id);
            else
                unit->CastSpell(unit, id, true, 0, NULL, m_caster->GetGUID());
        }
    }
}

void Spell::DoAllEffectOnTarget(GOTargetInfo* target)
{
    if (target->processed)                                  // Check target
        return;
    target->processed = true;                               // Target checked in apply effects procedure

    uint32 effectMask = target->effectMask;
    if (!effectMask)
        return;

    GameObject* go = m_caster->GetMap()->GetGameObject(target->targetGUID);
    if (!go)
        return;

    PrepareScriptHitHandlers();
    CallScriptBeforeHitHandlers();

    for (uint32 i = 0; i < GetSpellInfo()->Effects.size(); ++i)
        if (effectMask & (1 << i))
            HandleEffects(NULL, NULL, go, i, SPELL_EFFECT_HANDLE_HIT_TARGET);

    CallScriptOnHitHandlers();
    CallScriptAfterHitHandlers();
}

void Spell::DoAllEffectOnTarget(ItemTargetInfo* target)
{
    uint32 effectMask = target->effectMask;
    if (!target->item || !effectMask)
        return;

    PrepareScriptHitHandlers();
    CallScriptBeforeHitHandlers();

    for (uint32 i = 0; i < GetSpellInfo()->Effects.size(); ++i)
        if (effectMask & (1 << i))
            HandleEffects(NULL, target->item, NULL, i, SPELL_EFFECT_HANDLE_HIT_TARGET);

    CallScriptOnHitHandlers();

    CallScriptAfterHitHandlers();
}

bool Spell::UpdateChanneledTargetList()
{
    // Not need check return true
    if (m_channelTargetEffectMask == 0)
        return true;

    uint32 channelTargetEffectMask = m_channelTargetEffectMask;
    uint32 channelAuraMask = 0;
    for (uint8 i = 0; i < m_spellInfo->Effects.size(); ++i)
        if (m_spellInfo->Effects[i].Effect == SPELL_EFFECT_APPLY_AURA)
            channelAuraMask |= 1 << i;

    channelAuraMask &= channelTargetEffectMask;

    float range = 0;
    if (channelAuraMask)
    {
        range = m_spellInfo->GetMaxRange(m_spellInfo->IsPositive());
        if (Player* modOwner = m_caster->GetSpellModOwner())
            modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_RANGE, range, this);

        // Blizzard adds an extra ~ 10 yards to channel range
        range += 10.0f;
    }

    for (std::list<TargetInfo>::iterator ihit= m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
    {
        if (ihit->missCondition == SPELL_MISS_NONE && (channelTargetEffectMask & ihit->effectMask))
        {
            Unit* unit = m_caster->GetGUID() == ihit->targetGUID ? m_caster : ObjectAccessor::GetUnit(*m_caster, ihit->targetGUID);

            if (!unit)
                continue;

            if (IsValidDeadOrAliveTarget(unit))
            {
                if (channelAuraMask & ihit->effectMask)
                {
                    if (AuraApplication * aurApp = unit->GetAuraApplication(m_spellInfo->Id, m_originalCasterGUID))
                    {
                        if (m_caster != unit && !m_caster->IsWithinDistInMap(unit, range))
                        {
                            ihit->effectMask &= ~aurApp->GetEffectMask();
                            unit->RemoveAura(aurApp);
                            continue;
                        }
                    }
                    else // aura is dispelled
                        continue;
                }

                channelTargetEffectMask &= ~ihit->effectMask;   // remove from need alive mask effect that have alive target
            }
        }
    }

    // is all effects from m_needAliveTargetMask have alive targets
    return channelTargetEffectMask == 0;
}

void Spell::prepare(SpellCastTargets const* targets, AuraEffect const *triggeredByAura, uint32 gcdAtCast)
{
    if (m_CastItem)
        m_castItemGUID = m_CastItem->GetGUID();
    else
        m_castItemGUID = 0;

    InitExplicitTargets(*targets);

    // Fill aura scaling information
    if (m_caster->IsControlledByPlayer() && !m_spellInfo->IsPassive() && m_spellInfo->SpellLevel && !m_spellInfo->IsChanneled() && !(_triggeredCastFlags & TRIGGERED_IGNORE_AURA_SCALING))
    {
        for (uint8 i = 0; i < m_spellInfo->Effects.size(); ++i)
        {
            if (m_spellInfo->Effects[i].Effect == SPELL_EFFECT_APPLY_AURA)
            {
                // Change aura with ranks only if basepoints are taken from spellInfo and aura is positive
                if (m_spellInfo->IsPositiveEffect(i))
                {
                    m_auraScaleMask |= (1 << i);
                    if (m_spellValue.EffectBasePoints[i] != m_spellInfo->Effects[i].BasePoints)
                    {
                        m_auraScaleMask = 0;
                        break;
                    }
                }
            }
        }
    }

    m_spellState = SPELL_STATE_PREPARING;

    if (triggeredByAura)
        m_triggeredByAuraSpell  = triggeredByAura->GetSpellInfo();

    // create and add update event for this spell
    SpellEvent* Event = new SpellEvent(this);
    if (gcdAtCast && gcdAtCast <= MAX_SPELL_QUEUE_GCD)
    {
        if (m_caster->ToPlayer()->m_queuedSpell)
        {
            m_caster->ToPlayer()->m_queuedSpell->SendCastResult(SPELL_FAILED_SPELL_IN_PROGRESS);
            m_caster->ToPlayer()->m_queuedSpell->finish(false);
        }
        m_caster->m_Events.AddEvent(Event, m_caster->m_Events.CalculateTime(gcdAtCast));
        m_spellState = SPELL_STATE_QUEUED;
        m_caster->ToPlayer()->m_queuedSpell = this;
        return;
    }
    else
        m_caster->m_Events.AddEvent(Event, m_caster->m_Events.CalculateTime(1));

    //Prevent casting at cast another spell (ServerSide check) - Do not check for Autoshot as Steady Shot should be available to init cast of it
    if (m_spellInfo->Id != 108839 && m_spellInfo->Id != 75)
        if (!(_triggeredCastFlags & TRIGGERED_IGNORE_CAST_IN_PROGRESS) && m_caster->IsNonMeleeSpellCasted(false, true, true) && m_cast_count)
        {
            SendCastResult(SPELL_FAILED_SPELL_IN_PROGRESS);
            finish(false);
            return;
        }

    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_SPELL, m_spellInfo->Id, m_caster))
    {
        SendCastResult(SPELL_FAILED_SPELL_UNAVAILABLE);
        finish(false);
        return;
    }
    LoadScripts();

    if (IsDarkSimulacrum() || (m_triggeredByAuraSpell && m_triggeredByAuraSpell->Id == 101056))
        isStolen = true;

    // Fill cost data (not use power for item casts) but don't take mods for power calculation
    if (auto const player = m_caster->ToPlayer())
        player->SetSpellModTakingSpell(this, true);

    m_powerCost = m_CastItem ? 0 : m_spellInfo->CalcPowerCost(m_caster, m_spellSchoolMask, m_spellPowerData, false);

    if (auto const player = m_caster->ToPlayer())
        player->SetSpellModTakingSpell(this, false);

    // Set combo point requirement
    if ((_triggeredCastFlags & TRIGGERED_IGNORE_COMBO_POINTS) || m_CastItem || !m_caster->m_movedPlayer)
        m_needComboPoints = false;

    SpellCastResult result = CheckCast(true);
    if (result != SPELL_CAST_OK && !IsAutoRepeat())          //always cast autorepeat dummy for triggering
    {
        // Periodic auras should be interrupted when aura triggers a spell which can't be cast
        // for example bladestorm aura should be removed on disarm as of patch 3.3.5
        // channeled periodic spells should be affected by this (arcane missiles, penance, etc)
        // a possible alternative sollution for those would be validating aura target on unit state change
        if (triggeredByAura && triggeredByAura->IsPeriodic() && !triggeredByAura->GetBase()->IsPassive())
        {
            SendChannelUpdate(0);
            triggeredByAura->GetBase()->SetDuration(0);
        }

        SendCastResult(result);

        if (auto const player = m_caster->ToPlayer())
        {
            player->RestoreSpellMods(*this);
            player->SetSpellModTakingSpell(this, false);
        }

        finish(false);
        return;
    }

    // Prepare data for triggers
    prepareDataForTriggerSystem(triggeredByAura);

    if (auto const player = m_caster->ToPlayer())
    {
        if (!player->GetCommandStatus(CHEAT_CASTTIME))
        {
            player->SetSpellModTakingSpell(this, true);

            // Re-calculate spell power and take mod
            m_powerCost = m_CastItem ? 0 : m_spellInfo->CalcPowerCost(m_caster, m_spellSchoolMask, m_spellPowerData);

            // calculate cast time (calculated after first CheckCast check to prevent charge counting for first CheckCast fail)
            m_casttime = m_spellInfo->CalcCastTime(player, this);

            player->SetSpellModTakingSpell(this, false);
        }
        else
        {
            m_casttime = 0; // Set cast time to 0 if .cheat casttime is enabled.
        }
    }
    else
    {
        m_casttime = m_spellInfo->CalcCastTime(m_caster, this);
    }

    // If spell not channeled and was stolen it has no cast time
    if (isStolen && !m_spellInfo->IsChanneled() && m_spellInfo->Id != 605)
        m_casttime = 0;

    // don't allow channeled spells / spells with cast time to be casted while moving
    // (even if they are interrupted on moving, spells with almost immediate effect get to have their effect processed before movement interrupter kicks in)
    // don't cancel spells which are affected by a SPELL_AURA_CAST_WHILE_WALKING effect
    if (((m_spellInfo->IsChanneled() || m_casttime) && m_caster->GetTypeId() == TYPEID_PLAYER && m_caster->isMoving() &&
        m_spellInfo->InterruptFlags & SPELL_INTERRUPT_FLAG_MOVEMENT) && !m_caster->HasAuraTypeWithAffectMask(SPELL_AURA_CAST_WHILE_WALKING, m_spellInfo))
    {
        SendCastResult(SPELL_FAILED_MOVING);
        finish(false);
        return;
    }

    // set timer base at cast time
    ReSetTimer();

    //Containers for channeled spells have to be set
    //TODO:Apply this to all casted spells if needed
    // Why check duration? 29350: channeled triggers channeled
    if ((_triggeredCastFlags & TRIGGERED_CAST_DIRECTLY) && (!m_spellInfo->IsChanneled() || !m_spellInfo->GetMaxDuration()))
        cast(true);
    else
    {
        // stealth must be removed at cast starting (at show channel bar)
        // skip triggered spell (item equip spell casting and other not explicit character casts/item uses)
        if (!(_triggeredCastFlags & TRIGGERED_IGNORE_AURA_INTERRUPT_FLAGS) && m_spellInfo->IsBreakingStealth(m_caster) && (!m_caster->HasAuraType(SPELL_AURA_MOD_CAMOUFLAGE) || m_spellInfo->IsBreakingCamouflage()))
        {
            m_caster->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_CAST);
            for (auto const &spellEffect : m_spellInfo->Effects)
            {
                if (spellEffect.GetUsedTargetObjectType() == TARGET_OBJECT_TYPE_UNIT)
                {
                    m_caster->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_SPELL_ATTACK);
                    break;
                }
            }
        }

        m_caster->SetCurrentCastedSpell(this);
        SendSpellStart();

        // set target for proper facing
        if ((m_casttime || m_spellInfo->IsChanneled()) && !(_triggeredCastFlags & TRIGGERED_IGNORE_SET_FACING))
            if (m_caster->GetGUID() != m_targets.GetObjectTargetGUID() && m_caster->GetTypeId() == TYPEID_UNIT)
                m_caster->FocusTarget(this, m_targets.GetObjectTargetGUID());

        if (!(_triggeredCastFlags & TRIGGERED_IGNORE_GCD) && result == SPELL_CAST_OK)
            TriggerGlobalCooldown();

        if (!m_casttime && !m_castItemGUID && GetCurrentContainer() == CURRENT_GENERIC_SPELL)
            cast(true);
    }
}

void Spell::cancel()
{
    if (m_spellState == SPELL_STATE_FINISHED)
        return;

    uint32 oldState = m_spellState;
    m_spellState = SPELL_STATE_FINISHED;

    m_autoRepeat = false;
    switch (oldState)
    {
        case SPELL_STATE_PREPARING:
            CancelGlobalCooldown();
            if (m_caster->GetTypeId() == TYPEID_PLAYER)
                m_caster->ToPlayer()->RestoreSpellMods(*this);
        case SPELL_STATE_DELAYED:
            SendInterrupted(0);
            SendCastResult(SPELL_FAILED_INTERRUPTED);
            break;

        case SPELL_STATE_CASTING:
            for (std::list<TargetInfo>::const_iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
                if ((*ihit).missCondition == SPELL_MISS_NONE)
                    if (Unit* unit = m_caster->GetGUID() == ihit->targetGUID ? m_caster : ObjectAccessor::GetUnit(*m_caster, ihit->targetGUID))
                        unit->RemoveOwnedAura(m_spellInfo->Id, m_originalCasterGUID, 0, AURA_REMOVE_BY_CANCEL);

            SendChannelUpdate(0);
            SendInterrupted(0);
            SendCastResult(SPELL_FAILED_INTERRUPTED);

            // spell is canceled-take mods and clear list
            if (m_caster->GetTypeId() == TYPEID_PLAYER)
            {
                m_caster->ToPlayer()->RemoveSpellMods(*this);
                m_appliedMods.clear();
            }
            break;

        default:
            break;
    }

    SetReferencedFromCurrent(false);
    if (m_selfContainer && *m_selfContainer == this)
        *m_selfContainer = NULL;

    m_caster->RemoveDynObject(m_spellInfo->Id);
    if (m_spellInfo->IsChanneled()) // if not channeled then the object for the current cast wasn't summoned yet
        m_caster->RemoveGameObject(m_spellInfo->Id, true);

    //set state back so finish will be processed
    m_spellState = oldState;

    finish(false);
}

void Spell::cast(bool skipCheck)
{
    // update pointers base at GUIDs to prevent access to non-existed already object
    UpdatePointers();

    // cancel at lost explicit target during cast
    if (m_targets.GetObjectTargetGUID() && !m_targets.GetObjectTarget())
    {
        cancel();
        return;
    }

    // cancel if cast item is lost
    if (!IsTriggered() && m_castItemGUID && !m_CastItem)
    {
        cancel();
        return;
    }

    // Certain spells that trigger on equip, Cunning of the Cruel trinket's effect
    // for example, have nonzero speed. It must be ignored and spell must be
    // processed immediately to prevent exploit
    bool isOnEquipSpell = false;
    if (m_CastItem) {
        ItemTemplate const * const proto = m_CastItem->GetTemplate();
        for (std::size_t i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i) {
            auto const &data = proto->Spells[i];
            if (data.SpellId > 0
                    && static_cast<uint32>(data.SpellId) == m_spellInfo->Id
                    && data.SpellTrigger == ITEM_SPELLTRIGGER_ON_EQUIP) {
                isOnEquipSpell = true;
                break;
            }
        }
    }

    if (Player* playerCaster = m_caster->ToPlayer())
    {
        // now that we've done the basic check, now run the scripts
        // should be done before the spell is actually executed
        sScriptMgr->OnPlayerSpellCast(playerCaster, this, skipCheck);

        // As of 3.0.2 pets begin attacking their owner's target immediately
        // Let any pets know we've attacked something. Check DmgClass for harmful spells only
        // This prevents spells such as Hunter's Mark from triggering pet attack
        if (this->GetSpellInfo()->DmgClass != SPELL_DAMAGE_CLASS_NONE)
            if (Pet* playerPet = playerCaster->GetPet())
                if (playerPet->ToCreature()->IsAIEnabled && playerPet->IsAlive() && playerPet->isControlled() && (m_targets.GetTargetMask() & TARGET_FLAG_UNIT))
                    if (WorldObject* target = m_targets.GetObjectTarget())
                        playerPet->AI()->OwnerAttacked(target->ToUnit());
    }
    SetExecutedCurrently(true);

    if (m_caster->GetTypeId() != TYPEID_PLAYER && m_targets.GetUnitTarget() && m_targets.GetUnitTarget() != m_caster)
        m_caster->SetInFront(m_targets.GetUnitTarget());

    // Should this be done for original caster?
    if (m_caster->GetTypeId() == TYPEID_PLAYER)
    {
        // Set spell which will drop charges for triggered cast spells
        // if not successfully casted, will be remove in finish(false)
        m_caster->ToPlayer()->SetSpellModTakingSpell(this, true);
    }

    CallScriptBeforeCastHandlers();

    // skip check if done already (for instant cast spells for example)
    if (!skipCheck)
    {
        SpellCastResult castResult = CheckCast(false);
        if (castResult != SPELL_CAST_OK)
        {
            SendCastResult(castResult);
            SendInterrupted(0);
            //restore spell mods
            if (m_caster->GetTypeId() == TYPEID_PLAYER)
            {
                m_caster->ToPlayer()->RestoreSpellMods(*this);
                // cleanup after mod system
                // triggered spell pointer can be not removed in some cases
                m_caster->ToPlayer()->SetSpellModTakingSpell(this, false);
            }
            finish(false);
            SetExecutedCurrently(false);
            return;
        }

        // additional check after cast bar completes (must not be in CheckCast)
        // if trade not complete then remember it in trade data
        if (m_targets.GetTargetMask() & TARGET_FLAG_TRADE_ITEM)
        {
            if (m_caster->GetTypeId() == TYPEID_PLAYER)
            {
                if (TradeData* my_trade = m_caster->ToPlayer()->GetTradeData())
                {
                    if (!my_trade->IsInAcceptProcess())
                    {
                        // Spell will be casted at completing the trade. Silently ignore at this place
                        my_trade->SetSpell(m_spellInfo->Id, m_CastItem);
                        SendCastResult(SPELL_FAILED_DONT_REPORT);
                        SendInterrupted(0);
                        m_caster->ToPlayer()->RestoreSpellMods(*this);
                        // cleanup after mod system
                        // triggered spell pointer can be not removed in some cases
                        m_caster->ToPlayer()->SetSpellModTakingSpell(this, false);
                        finish(false);
                        SetExecutedCurrently(false);
                        return;
                    }
                }
            }
        }
    }

    SelectSpellTargets();

    // Spell may be finished after target map check
    if (m_spellState == SPELL_STATE_FINISHED)
    {
        SendInterrupted(0);
        //restore spell mods
        if (m_caster->GetTypeId() == TYPEID_PLAYER)
        {
            m_caster->ToPlayer()->RestoreSpellMods(*this);
            // cleanup after mod system
            // triggered spell pointer can be not removed in some cases
            m_caster->ToPlayer()->SetSpellModTakingSpell(this, false);
        }
        finish(false);
        SetExecutedCurrently(false);
        return;
    }

    PrepareTriggersExecutedOnHit();

    if (m_caster->getClass() == CLASS_DEATH_KNIGHT)
        if (IsDarkSimulacrum())
            m_caster->RemoveAurasDueToSpell(77616);

    CallScriptOnCastHandlers();

    // traded items have trade slot instead of guid in m_itemTargetGUID
    // set to real guid to be sent later to the client
    m_targets.UpdateTradeSlotItem();

    if (Player* player = m_caster->ToPlayer())
    {
        if (!(_triggeredCastFlags & TRIGGERED_IGNORE_CAST_ITEM) && m_CastItem)
        {
            player->GetAchievementMgr().StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_ITEM, m_CastItem->GetEntry());
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM, m_CastItem->GetEntry(), 0, 0, player);
        }

        player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL, m_spellInfo->Id);
    }

    if (!(_triggeredCastFlags & TRIGGERED_IGNORE_POWER_AND_REAGENT_COST))
    {
        // Powers have to be taken before SendSpellGo
        TakePower();
        TakeReagents();                                         // we must remove reagents before HandleEffects to allow place crafted item in same slot
    }
    else if (Item* targetItem = m_targets.GetItemTarget())
    {
        /// Not own traded item (in trader trade slot) req. reagents including triggered spell case
        if (targetItem->GetOwnerGUID() != m_caster->GetGUID())
            TakeReagents();
    }

    // CAST SPELL
    SendSpellCooldown();

    PrepareScriptHitHandlers();

    HandleLaunchPhase();

    // we must send smsg_spell_go packet before m_castItem delete in TakeCastItem()...
    SendSpellGo();

    // Okay, everything is prepared. Now we need to distinguish between immediate and evented delayed spells
    if (((m_spellInfo->Speed > 0.0f || (m_delayMoment && (m_caster->GetTypeId() == TYPEID_PLAYER || m_caster->ToCreature()->isPet())))
            && !isOnEquipSpell && !m_spellInfo->IsChanneled()) || m_spellInfo->_IsNeedDelay())
    {
        // Remove used for cast item if need (it can be already NULL after TakeReagents call
        // in case delayed spell remove item at cast delay start
        TakeCastItem();

        // Okay, maps created, now prepare flags
        m_immediateHandled = false;
        m_spellState = SPELL_STATE_DELAYED;
        SetDelayStart(0);

        if (m_caster->HasUnitState(UNIT_STATE_CASTING) && !m_caster->IsNonMeleeSpellCasted(false, false, true))
            m_caster->ClearUnitState(UNIT_STATE_CASTING);
    }
    else
    {
        // Immediate spell, no big deal
        handle_immediate();
    }

    CallScriptAfterCastHandlers();

    // Hack for Whiplash, TODO: Move to spell-script
    if (m_spellInfo->Id == 119909 && m_caster->GetTypeId() == TYPEID_PLAYER)
    {
        if (WorldLocation const * loc = m_targets.GetDstPos())
            if (Pet* pet = m_caster->ToPlayer()->GetPet())
            {
                pet->CastSpell(loc->GetPositionX(), loc->GetPositionY(), loc->GetPositionZ(), 6360, true);
                pet->AddCreatureSpellCooldown(6360);
                m_caster->ToPlayer()->AddSpellCooldown(m_spellInfo->Id, 0, pet->GetCreatureSpellCooldownDelay(6360) * 1000);
            }
    }

    // Kil'Jaeden's Cunning - 10% speed less for each cast while moving (up to 2 charges)
    if (m_caster->HasAuraType(SPELL_AURA_KIL_JAEDENS_CUNNING) && m_caster->isMoving() && !m_caster->HasAura(119048) && m_spellInfo->CalcCastTime(m_caster) > 0)
        m_caster->CastSpell(m_caster, 119050, true);

    if (const std::vector<int32> *spell_triggered = sSpellMgr->GetSpellLinked(m_spellInfo->Id))
    {
        for (std::vector<int32>::const_iterator i = spell_triggered->begin(); i != spell_triggered->end(); ++i)
            if (*i < 0)
                m_caster->RemoveAurasDueToSpell(-(*i));
            else
                m_caster->CastSpell(m_targets.GetUnitTarget() ? m_targets.GetUnitTarget() : m_caster, *i, true);
    }

    HandleGenericAfterCast();

    if (m_caster->GetTypeId() == TYPEID_PLAYER)
    {
        // Remove spell mods after cast
        if (m_spellInfo->Speed && !m_spellInfo->IsChanneled())
            m_caster->ToPlayer()->RemoveSpellMods(*this);

        m_caster->ToPlayer()->SetSpellModTakingSpell(this, false);
        //Clear spell cooldowns after every spell is cast if .cheat cooldown is enabled.
        if (m_caster->ToPlayer()->GetCommandStatus(CHEAT_COOLDOWN))
            m_caster->ToPlayer()->RemoveSpellCooldown(m_spellInfo->Id, true);
    }

    SetExecutedCurrently(false);
}

void Spell::HandleGenericAfterCast()
{
    Unit::AuraEffectList swaps = m_caster->GetAuraEffectsByType(SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS);
    Unit::AuraEffectList const& swaps2 = m_caster->GetAuraEffectsByType(SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS_2);
    if (!swaps2.empty())
        swaps.insert(swaps.end(), swaps2.begin(), swaps2.end());

    if (!swaps.empty())
        for (Unit::AuraEffectList::const_iterator itr = swaps.begin(); itr != swaps.end(); ++itr)
            if ((*itr)->GetAmount() == m_spellInfo->Id)
                if (!(*itr)->GetSpellInfo()->IsPassive())
                {
                    if ((*itr)->GetBase()->IsUsingCharges())
                        (*itr)->GetBase()->DropCharge();
                }

    Unit::AuraEffectList procFromSpell = m_caster->GetAuraEffectsByType(SPELL_AURA_PROC_FROM_SPELL);
    for (Unit::AuraEffectList::iterator j = procFromSpell.begin(); j != procFromSpell.end();)
    {
        Aura* base = (*j)->GetBase();

        if (!base || base->GetSpellInfo()->ProcFlags || base->GetDuration() == base->GetMaxDuration())
        {
            j++;
            continue;
        }

        switch ((*j)->GetSpellInfo()->Id)
        {
            case 125195: // Tigereye brew, handled in script
            case 114851: // Blood Charges, handled in script
                j++;
                break;
            default:
                // Still needs better handling
                int32 consumeStacks = 0;
                if (m_spellInfo->Id == (*j)->GetSpellInfo()->Effects[(*j)->GetEffIndex()].TriggerSpell)
                {
                    j++;
                    base->ModStackAmount(-1);
                }
                else
                    j++;
        }
    }

    Unit::AuraEffectList const& stateAuras = m_caster->GetAuraEffectsByType(SPELL_AURA_ABILITY_IGNORE_AURASTATE);
    for (Unit::AuraEffectList::const_iterator j = stateAuras.begin(); j != stateAuras.end();)
    {
        if ((*j)->IsAffectingSpell(m_spellInfo))
        {
            Aura* base = (*j)->GetBase();
            if (base->GetSpellInfo()->StackAmount)
            {
                j++;
                base->ModStackAmount(-1);
                continue;
            }
        }
        j++;
    }

    switch (m_spellInfo->SpellFamilyName)
    {
        case SPELLFAMILY_WARLOCK:
            // Havoc
            if (AuraEffect* aurEff = m_caster->GetAuraEffect(80240, EFFECT_1))
                if (aurEff->GetSpellEffectInfo().SpellClassMask & m_spellInfo->SpellFamilyFlags)
                    if (GetUnitTarget() && !GetUnitTarget()->HasAura(80240))
                        aurEff->GetBase()->ModStackAmount(m_spellInfo->Id == 116858 ? -3 : -1);
            break;
    }
}

void Spell::handle_immediate()
{
    // start channeling if applicable
    if (m_spellInfo->IsChanneled())
    {
        int32 duration = m_spellInfo->GetDuration();
        if (duration)
        {
            // First mod_duration then haste - see Missile Barrage
            // Apply duration mod
            if (Player* modOwner = m_caster->GetSpellModOwner())
                modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_DURATION, duration);
            // Apply haste mods
            if (m_spellInfo->AttributesEx5 & SPELL_ATTR5_HASTE_AFFECT_DURATION)
                m_caster->ModSpellCastTime(m_spellInfo, duration, this);

            m_spellState = SPELL_STATE_CASTING;
            m_caster->AddInterruptMask(m_spellInfo->ChannelInterruptFlags);
            if (m_spellInfo->IsPositive() || !m_spellInfo->HasEffect(SPELL_EFFECT_APPLY_AURA))
                SendChannelStart(duration);
        }
        else if (duration == -1)
        {
            m_spellState = SPELL_STATE_CASTING;
            m_caster->AddInterruptMask(m_spellInfo->ChannelInterruptFlags);
            SendChannelStart(duration);
        }
    }

    PrepareTargetProcessing();

    // process immediate effects (items, ground, etc.) also initialize some variables
    _handle_immediate_phase();

    for (std::list<TargetInfo>::iterator ihit= m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
        DoAllEffectOnTarget(&(*ihit));

    for (std::list<GOTargetInfo>::iterator ihit= m_UniqueGOTargetInfo.begin(); ihit != m_UniqueGOTargetInfo.end(); ++ihit)
        DoAllEffectOnTarget(&(*ihit));

    FinishTargetProcessing();

    // spell is finished, perform some last features of the spell here
    _handle_finish_phase();

    // Remove used for cast item if need (it can be already NULL after TakeReagents call
    TakeCastItem();

    // handle ammo consumption for thrown weapons
    if (m_spellInfo->IsRangedWeaponSpell() && m_spellInfo->IsChanneled())
        TakeAmmo();

    if (m_spellState != SPELL_STATE_CASTING)
        finish(true);                                       // successfully finish spell cast (not last in case autorepeat or channel spell)
}

uint64 Spell::handle_delayed(uint64 t_offset)
{
    UpdatePointers();

    if (m_caster->GetTypeId() == TYPEID_PLAYER)
        m_caster->ToPlayer()->SetSpellModTakingSpell(this, true);

    uint64 next_time = 0;

    PrepareTargetProcessing();

    if (!m_immediateHandled)
    {
        _handle_immediate_phase();
        m_immediateHandled = true;
    }

    bool single_missile = (m_targets.HasDst());

    // now recheck units targeting correctness (need before any effects apply to prevent adding immunity at first effect not allow apply second spell effect and similar cases)
    for (std::list<TargetInfo>::iterator ihit= m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
    {
        if (ihit->processed == false)
        {
            if (single_missile || ihit->timeDelay <= t_offset)
            {
                ihit->timeDelay = t_offset;
                DoAllEffectOnTarget(&(*ihit));
            }
            else if (next_time == 0 || ihit->timeDelay < next_time)
                next_time = ihit->timeDelay;
        }
    }

    // now recheck gameobject targeting correctness
    for (std::list<GOTargetInfo>::iterator ighit= m_UniqueGOTargetInfo.begin(); ighit != m_UniqueGOTargetInfo.end(); ++ighit)
    {
        if (ighit->processed == false)
        {
            if (single_missile || ighit->timeDelay <= t_offset)
                DoAllEffectOnTarget(&(*ighit));
            else if (next_time == 0 || ighit->timeDelay < next_time)
                next_time = ighit->timeDelay;
        }
    }

    FinishTargetProcessing();

    if (m_caster->GetTypeId() == TYPEID_PLAYER)
        m_caster->ToPlayer()->SetSpellModTakingSpell(this, false);

    // All targets passed - need finish phase
    if (next_time == 0)
    {
        // spell is finished, perform some last features of the spell here
        _handle_finish_phase();

        finish(true);                                       // successfully finish spell cast

        // return zero, spell is finished now
        return 0;
    }
    else
    {
        // spell is unfinished, return next execution time
        return next_time;
    }
}

void Spell::_handle_immediate_phase()
{
    m_spellAura = NULL;
    // initialize Diminishing Returns Data
    m_diminishLevel = DIMINISHING_LEVEL_1;
    m_diminishGroup = DIMINISHING_NONE;

    // handle some immediate features of the spell here
    HandleThreatSpells();

    PrepareScriptHitHandlers();

    // handle effects with SPELL_EFFECT_HANDLE_HIT mode
    for (uint32 j = 0; j < m_spellInfo->Effects.size(); ++j)
    {
        // don't do anything for empty effect
        if (m_spellInfo->Effects[j].IsEffect())
            HandleEffects(NULL, NULL, NULL, j, SPELL_EFFECT_HANDLE_HIT);
    }

    // process items
    for (std::list<ItemTargetInfo>::iterator ihit= m_UniqueItemInfo.begin(); ihit != m_UniqueItemInfo.end(); ++ihit)
        DoAllEffectOnTarget(&(*ihit));

    if (!m_originalCaster)
        return;
    // Handle procs on cast
    // TODO: finish new proc system:P
    if (m_UniqueTargetInfo.empty() && m_targets.HasDst())
    {
        uint32 procAttacker = m_procAttacker;
        if (!procAttacker)
            procAttacker |= PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS;

        // Proc the spells that have DEST target
        m_originalCaster->ProcDamageAndSpell(NULL, procAttacker, 0, m_procEx | PROC_EX_NORMAL_HIT, 0, 0, BASE_ATTACK, m_spellInfo, m_triggeredByAuraSpell);
    }
}

void Spell::_handle_finish_phase()
{
    if (m_caster->m_movedPlayer)
    {
        // Take for real after all targets are processed
        if (m_needComboPoints || m_spellInfo->Id == 127538)
        {
            // Save combo info for further use
            auto const comboTarget = Unit::GetUnit(*m_caster->m_movedPlayer, m_caster->m_movedPlayer->GetComboTarget());
            auto const comboPoints = m_caster->m_movedPlayer->GetComboPoints();
            // Clear combo points
            m_caster->m_movedPlayer->ClearComboPoints();

            if (Player* _player = m_caster->ToPlayer())
            {
                // Anticipation
                if (_player->HasAura(115189) && m_spellInfo->Id != 5171 && m_spellInfo->Id != 73651)
                {
                    int32 basepoints0 = _player->GetAura(115189) ? _player->GetAura(115189)->GetStackAmount() : 0;
                    _player->CastCustomSpell(comboTarget, 115190, &basepoints0, NULL, NULL, true);

                    if (basepoints0)
                        _player->RemoveAura(115189);
                }

                // Ruthlessness - need hack to prevent adding combo points to self
                if (_player->HasAura(14161) && GetUnitTarget() == _player && roll_chance_i(20 * comboPoints))
                    _player->CastSpell(comboTarget, 139569, true);
            }
        }

        // Real add combo points from effects
        if (m_comboPointGain)
            m_caster->m_movedPlayer->GainSpellComboPoints(m_comboPointGain);

        if (m_spellInfo->spellPower.PowerType == POWER_HOLY_POWER && m_caster->m_movedPlayer->getClass() == CLASS_PALADIN)
            HandleHolyPower(m_caster->m_movedPlayer);
    }

    if (m_caster->m_extraAttacks && GetSpellInfo()->HasEffect(SPELL_EFFECT_ADD_EXTRA_ATTACKS))
        m_caster->HandleProcExtraAttackFor(m_caster->GetVictim());

    // TODO: trigger proc phase finish here
}

void Spell::SendSpellCooldown()
{
    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* _player = (Player*)m_caster;

    // mana/health/etc potions, disabled by client (until combat out as declarate)
    if (m_CastItem && m_CastItem->IsPotion())
    {
        // need in some way provided data for Spell::finish SendCooldownEvent
        _player->SetLastPotionId(m_CastItem->GetEntry());
        return;
    }

    // have infinity cooldown but set at aura apply                  // do not set cooldown for triggered spells (needed by reincarnation)
    if (m_spellInfo->Attributes & (SPELL_ATTR0_DISABLED_WHILE_ACTIVE | SPELL_ATTR0_PASSIVE) 
        || (_triggeredCastFlags & TRIGGERED_IGNORE_SPELL_AND_CATEGORY_CD) && !(m_spellInfo->AttributesEx4 & SPELL_ATTR4_TRIGGERED))
        return;

    if (m_caster->HasAuraTypeWithAffectMask(SPELL_AURA_ALLOW_CAST_WHILE_IN_COOLDOWN, m_spellInfo))
        return;

    // Heroic Strike and Cleave share cooldowns, prevent cheat by using macro for bypass cooldown
    if (m_spellInfo->Id == 78)
        _player->AddSpellAndCategoryCooldowns(sSpellMgr->GetSpellInfo(845), 0, this);
    else if (m_spellInfo->Id == 845)
        _player->AddSpellAndCategoryCooldowns(sSpellMgr->GetSpellInfo(78), 0, this);

    _player->AddSpellAndCategoryCooldowns(m_spellInfo, m_CastItem ? m_CastItem->GetEntry() : 0, this);
}

void Spell::update(uint32 difftime)
{
    if (m_spellState != SPELL_STATE_QUEUED)
    {
        // update pointers based at it's GUIDs
        UpdatePointers();

        if (m_targets.GetUnitTargetGUID() && !m_targets.GetUnitTarget())
        {
            cancel();
            return;
        }

        // check if the player caster has moved before the spell finished
        // with the exception of spells affected with SPELL_AURA_CAST_WHILE_WALKING effect
        if ((m_caster->GetTypeId() == TYPEID_PLAYER && m_timer != 0) &&
            m_caster->isMoving() && (m_spellInfo->InterruptFlags & SPELL_INTERRUPT_FLAG_MOVEMENT) &&
            (m_spellInfo->Effects[0].Effect != SPELL_EFFECT_STUCK || !m_caster->HasUnitMovementFlag(MOVEMENTFLAG_FALLING_FAR)) &&
            !m_caster->HasAuraTypeWithAffectMask(SPELL_AURA_CAST_WHILE_WALKING, m_spellInfo))
        {
            // don't cancel for melee, autorepeat, triggered and instant spells
            if (!IsNextMeleeSwingSpell() && !IsAutoRepeat() && !IsTriggered())
                cancel();
        }
    }

    switch (m_spellState)
    {
        case SPELL_STATE_QUEUED:
        {
            m_caster->ToPlayer()->GetGlobalCooldownMgr().CancelGlobalCooldown(m_spellInfo);
            m_caster->ToPlayer()->m_queuedSpell = NULL;
            SpellCastTargets targets = m_targets;
            Spell* spell = new Spell(m_caster, m_spellInfo, TRIGGERED_NONE, 0, false);
            spell->m_cast_count = m_cast_count;                       // set count of casts
            spell->m_glyphIndex = m_glyphIndex;
            cancel();
            uint64 guid = targets.GetUnitTargetGUID();
            if (!guid || ObjectAccessor::FindUnit(guid))
                spell->prepare(&targets);
            break;
        }
        case SPELL_STATE_PREPARING:
        {
            if (m_timer > 0)
            {
                if (!IsTriggered() && m_castItemGUID && !m_CastItem)
                {
                    cancel();
                    return;
                }

                if (difftime >= (uint32)m_timer)
                    m_timer = 0;
                else
                    m_timer -= difftime;
            }

            if (m_timer == 0 && !IsNextMeleeSwingSpell() && !IsAutoRepeat())
                // don't CheckCast for instant spells - done in spell::prepare, skip duplicate checks, needed for range checks for example
                cast(!m_casttime);
            break;
        }
        case SPELL_STATE_CASTING:
        {
            if (m_timer)
            {
                // check if there are alive targets left
                if (!UpdateChanneledTargetList() || (!IsTriggered() && m_castItemGUID && !m_CastItem))
                {
                    SendChannelUpdate(0);
                    finish();
                    return;
                }

                if (m_timer > 0)
                {
                    if (difftime >= (uint32)m_timer)
                        m_timer = 0;
                    else
                        m_timer -= difftime;
                }
            }

            if (m_timer == 0)
            {
                SendChannelUpdate(0);
                finish();
            }
            break;
        }
        default:
            break;
    }
}

void Spell::finish(bool ok)
{
    if (!m_caster)
        return;

    if (m_spellState == SPELL_STATE_FINISHED)
        return;
    m_spellState = SPELL_STATE_FINISHED;

    if (m_spellInfo->IsChanneled())
        m_caster->UpdateInterruptMask();

    if (m_caster->HasUnitState(UNIT_STATE_CASTING) && !m_caster->IsNonMeleeSpellCasted(false, false, true))
        m_caster->ClearUnitState(UNIT_STATE_CASTING);

    // Unsummon summon as possessed creatures on spell cancel
    if (m_spellInfo->IsChanneled() && m_caster->GetTypeId() == TYPEID_PLAYER)
    {
        if (Unit* charm = m_caster->GetCharm())
            if (charm->GetTypeId() == TYPEID_UNIT
                && charm->ToCreature()->HasUnitTypeMask(UNIT_MASK_PUPPET)
                && charm->GetUInt32Value(UNIT_CREATED_BY_SPELL) == m_spellInfo->Id)
                ((Puppet*)charm)->UnSummon();
    }

    if (m_caster->GetTypeId() == TYPEID_UNIT)
        m_caster->ReleaseFocus(this);

    if (!ok)
        return;

    if (m_caster->GetTypeId() == TYPEID_UNIT && m_caster->ToCreature()->IsSummon())
    {
        // Unsummon statue
        uint32 spell = m_caster->GetUInt32Value(UNIT_CREATED_BY_SPELL);
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell);
        if (spellInfo && spellInfo->SpellIconID == 2056)
        {
            m_caster->setDeathState(JUST_DIED);
            return;
        }
    }

    if (IsAutoActionResetSpell())
    {
        bool found = false;
        Unit::AuraEffectList const& vIgnoreReset = m_caster->GetAuraEffectsByType(SPELL_AURA_IGNORE_MELEE_RESET);
        for (Unit::AuraEffectList::const_iterator i = vIgnoreReset.begin(); i != vIgnoreReset.end(); ++i)
        {
            if ((*i)->IsAffectingSpell(m_spellInfo))
            {
                found = true;
                break;
            }
        }
        if (!found && !(m_spellInfo->AttributesEx2 & SPELL_ATTR2_NOT_RESET_AUTO_ACTIONS))
        {
            m_caster->resetAttackTimer(BASE_ATTACK);
            if (m_caster->haveOffhandWeapon())
                m_caster->resetAttackTimer(OFF_ATTACK);
            m_caster->resetAttackTimer(RANGED_ATTACK);
        }
    }

    // potions disabled by client, send event "not in combat" if need
    if (auto const player = m_caster->ToPlayer())
    {
        if (!m_triggeredByAuraSpell)
            player->UpdatePotionCooldown(this);

        // triggered spell pointer can be not set in some cases
        // this is needed for proper apply of triggered spell mods
        player->SetSpellModTakingSpell(this, true);

        // Take mods after trigger spell (needed for 14177 to affect 48664)
        // mods are taken only on succesfull cast and independantly from targets of the spell
        if (!m_spellInfo->Speed || m_spellInfo->IsChanneled())
            player->RemoveSpellMods(*this);

        player->SetSpellModTakingSpell(this, false);
    }

    // Stop Attack for some spells
    if (m_spellInfo->Attributes & SPELL_ATTR0_STOP_ATTACK_TARGET)
    {
        m_caster->AttackStop();
        if (auto player = m_caster->ToPlayer())
            player->SendAttackSwingCancelAttack();
    }

    // Dispel cd reset if nothing was dispelled
    if (m_spellInfo->IsPositive() && !m_hasDispelled && m_caster->GetTypeId() == TYPEID_PLAYER && m_spellInfo->GetRecoveryTime() < 30 * IN_MILLISECONDS)
    {
        bool canResetCooldown = true;
        for (uint8 i = 0; i < m_spellInfo->Effects.size(); ++i)
            if (m_spellInfo->Effects[i].Effect != SPELL_EFFECT_DISPEL)
                canResetCooldown = false;

        if (canResetCooldown)
        {
            if (m_caster->GetTypeId() == TYPEID_PLAYER)
                m_caster->ToPlayer()->RemoveSpellCooldown(m_spellInfo->Id, true);
        }
    }

    if (m_castItemGUID && m_caster->GetTypeId() == TYPEID_PLAYER)
        if (Item* item = m_caster->ToPlayer()->GetItemByGuid(m_castItemGUID))
            if (item->IsEquipable() && !item->IsEquipped())
                m_caster->ToPlayer()->ApplyItemEquipSpell(item, false);

    switch (m_spellInfo->Id)
    {
        case 32379: // Shadow Word: Death
        {
            if (m_caster->GetTypeId() != TYPEID_PLAYER)
                break;

            if (m_caster->ToPlayer()->GetSpecializationId(m_caster->ToPlayer()->GetActiveSpec()) != SPEC_PRIEST_SHADOW)
                break;

            if (m_caster->HasAura(95652))
                break;

            if (!unitTarget || !unitTarget->IsAlive() || unitTarget->GetHealthPct() >= 20.0f)
            {
                m_caster->CastSpell(m_caster, 125927, true); // Shadow Orb energize
                break;
            }

            m_caster->CastSpell(m_caster, 125927, true); // Shadow Orb energize
            m_caster->CastSpell(m_caster, 95652, true); // Effect cooldown marker
            m_caster->ToPlayer()->RemoveSpellCooldown(m_spellInfo->Id, true);

            break;
        }
        case 49560: // Death Grip
        case 49576: // Death Grip dummy
        {
            if (m_caster->GetTypeId() != TYPEID_PLAYER)
                break;

            // Only for Death Grip triggered to prevent dysfunctionment of the set bonus
            if (m_spellInfo->Id == 49560)
            {
                bool first = !m_caster->HasAura(131547);

                m_caster->RemoveAurasDueToSpell(131547); // Relentless Grip

                if (m_caster->HasAura(131542) && first) // Item DK - PvP S12 4P Bonus
                    m_caster->CastSpell(m_caster, 131547, true);
            }

            if (m_UniqueTargetInfo.empty())
                break;

            if (m_UniqueTargetInfo.front().missCondition != SPELL_MISS_IMMUNE)
                break;

            if (m_caster->HasAura(59309)) // Glyph of Resilient Grip
                m_caster->ToPlayer()->RemoveSpellCooldown(m_spellInfo->Id, true);

            break;
        }
        case 53351: // Kill Shot
        {
            if (!unitTarget || !unitTarget->IsAlive() || unitTarget->GetHealthPct() >= 20.0f || m_caster->HasAura(90967))
                break;

            m_caster->CastSpell(m_caster, 90967, true); // Effect cooldown marker
            m_caster->ToPlayer()->RemoveSpellCooldown(m_spellInfo->Id, true);

            break;
        }
        default:
            break;
    }
}

void Spell::SendCastResult(SpellCastResult result)
{
    if (result == SPELL_CAST_OK)
        return;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    if (m_caster->ToPlayer()->GetSession()->PlayerLoading())  // don't send cast results at loading time
        return;

    SendCastResult(m_caster->ToPlayer(), m_spellInfo, m_spellPowerData, m_cast_count, result, m_customError);
}

void Spell::SendCastResult(Player* caster, SpellInfo const* spellInfo, SpellPowerEntry const* powerData, uint8 cast_count, SpellCastResult result, SpellCustomErrors customError /*= SPELL_CUSTOM_ERROR_NONE*/)
{
    if (result == SPELL_CAST_OK)
        return;

    WorldPacket data(SMSG_CAST_FAILED, 1+4+4);
    data << uint8(cast_count);                              // single cast or multi 2.3 (0/1)
    data << uint32(result);                                 // problem
    data << uint32(spellInfo->Id);                          // spellId

    switch (result)
    {
        case SPELL_FAILED_NO_POWER:
        {
            data.WriteBit(0);
            data.WriteBit(1);
            data << uint32(powerData->PowerType);
            break;
        }
        case SPELL_FAILED_NOT_READY:
        {
            data.WriteBit(0);
            data.WriteBit(1);
            data << uint32(0);                              // unknown (value 1 update cooldowns on client flag)
            break;
        }
        case SPELL_FAILED_REQUIRES_SPELL_FOCUS:
        {
            data.WriteBit(0);
            data.WriteBit(1);
            data << uint32(spellInfo->RequiresSpellFocus);  // SpellFocusObject.dbc id
            break;
        }
        case SPELL_FAILED_REQUIRES_AREA:                    // AreaTable.dbc id
        {
            data.WriteBit(0);
            data.WriteBit(1);
            data.FlushBits();

            // hardcode areas limitation case
            switch (spellInfo->Id)
            {
                case 41617:                                 // Cenarion Mana Salve
                case 41619:                                 // Cenarion Healing Salve
                    data << uint32(3905);
                    break;
                case 41618:                                 // Bottled Nethergon Energy
                case 41620:                                 // Bottled Nethergon Vapor
                    data << uint32(3842);
                    break;
                case 45373:                                 // Bloodberry Elixir
                    data << uint32(4075);
                    break;
                default:                                    // default case (don't must be)
                    data << uint32(0);
                    break;
            }

            break;
        }
        case SPELL_FAILED_TOTEMS:
        {
            if (spellInfo->Totem[0] && spellInfo->Totem[1])
            {
                data.WriteBit(0);
                data.WriteBit(0);
            }
            else
            {
                data.WriteBit(0);
                data.WriteBit(1);
            }

            data.FlushBits();

            if (spellInfo->Totem[0])
                data << uint32(spellInfo->Totem[0]);
            if (spellInfo->Totem[1])
                data << uint32(spellInfo->Totem[1]);

            break;
        }
        case SPELL_FAILED_TOTEM_CATEGORY:
        {
            if (spellInfo->TotemCategory[0] && spellInfo->TotemCategory[1])
            {
                data.WriteBit(0);
                data.WriteBit(0);
            }
            else
            {
                data.WriteBit(0);
                data.WriteBit(1);
            }

            data.FlushBits();

            if (spellInfo->TotemCategory[0])
                data << uint32(spellInfo->TotemCategory[0]);
            if (spellInfo->TotemCategory[1])
                data << uint32(spellInfo->TotemCategory[1]);

            break;
        }
        case SPELL_FAILED_EQUIPPED_ITEM_CLASS:
        case SPELL_FAILED_EQUIPPED_ITEM_CLASS_MAINHAND:
        case SPELL_FAILED_EQUIPPED_ITEM_CLASS_OFFHAND:
        {
            data.WriteBit(0);
            data.WriteBit(0);
            data << uint32(spellInfo->EquippedItemClass);
            data << uint32(spellInfo->EquippedItemSubClassMask);

            break;
        }
        case SPELL_FAILED_TOO_MANY_OF_ITEM:
        {
            uint32 item = 0;
            for (auto const &spellEffect : spellInfo->Effects)
                if (spellEffect.ItemType)
                    item = spellEffect.ItemType;
            ItemTemplate const* proto = sObjectMgr->GetItemTemplate(item);
            if (proto && proto->ItemLimitCategory)
            {
                data.WriteBit(0);
                data.WriteBit(1);
                data << uint32(proto->ItemLimitCategory);
            }
            else
            {
                data.WriteBit(1);
                data.WriteBit(1);
                data.FlushBits();
            }

            break;
        }
        case SPELL_FAILED_PREVENTED_BY_MECHANIC:
        {
            data.WriteBit(0);
            data.WriteBit(1);
            data << uint32(spellInfo->GetAllEffectsMechanicMask());  // SpellMechanic.dbc id
            break;
        }
        case SPELL_FAILED_NEED_EXOTIC_AMMO:
        {
            data.WriteBit(0);
            data.WriteBit(1);
            data << uint32(spellInfo->EquippedItemSubClassMask); // seems correct...
            break;
        }
        case SPELL_FAILED_NEED_MORE_ITEMS:
        {
            data.WriteBit(0);
            data.WriteBit(0);
            data << uint32(0);                              // Item id
            data << uint32(0);                              // Item count?
            break;
        }
        case SPELL_FAILED_MIN_SKILL:
        {
            data.WriteBit(0);
            data.WriteBit(0);
            data << uint32(0);                              // SkillLine.dbc id
            data << uint32(0);                              // required skill value
            break;
        }
        case SPELL_FAILED_FISHING_TOO_LOW:
        {
            data.WriteBit(0);
            data.WriteBit(1);
            data << uint32(0);                              // required fishing skill
            break;
        }
        case SPELL_FAILED_CUSTOM_ERROR:
        {
            data.WriteBit(0);
            data.WriteBit(1);
            data << uint32(customError);
            break;
        }
        case SPELL_FAILED_SILENCED:
        {
            data.WriteBit(0);
            data.WriteBit(1);
            data << uint32(0);                              // Unknown
            break;
        }
        case SPELL_FAILED_REAGENTS:
        {
            uint32 missingItem = 0;
            for (uint32 i = 0; i < MAX_SPELL_REAGENTS; i++)
            {
                if (spellInfo->Reagent[i] <= 0)
                    continue;

                uint32 itemid    = spellInfo->Reagent[i];
                uint32 itemcount = spellInfo->ReagentCount[i];

                if (!caster->HasItemCount(itemid, itemcount))
                {
                    missingItem = itemid;
                    break;
                }
            }

            data.WriteBit(0);
            data.WriteBit(1);
            data << uint32(missingItem);  // first missing item
            break;
        }
        // TODO: SPELL_FAILED_NOT_STANDING
        default:
            data.WriteBit(1);
            data.WriteBit(1);
            data.FlushBits();
            break;
    }

    caster->GetSession()->SendPacket(&data);
}

void Spell::SendSpellStart()
{
    if (!IsNeedSendToClient())
        return;

    //TC_LOG_DEBUG("spells", "Sending SMSG_SPELL_START id=%u", m_spellInfo->Id);

    uint32 castFlags = CAST_FLAG_HAS_TRAJECTORY;

    if ((m_caster->GetTypeId() == TYPEID_PLAYER || (m_caster->GetTypeId() == TYPEID_UNIT && m_caster->ToCreature()->isPet()))
            && m_spellPowerData->PowerType != POWER_HEALTH)
        castFlags |= CAST_FLAG_POWER_LEFT_SELF;

    if (m_spellInfo->RuneCostID && m_spellPowerData->PowerType == POWER_RUNES)
        castFlags |= CAST_FLAG_UNKNOWN_19;

    WorldPacket data(SMSG_SPELL_START);

    uint32 unkStringLength = 0;
    uint32 powerCount = 0;
    uint32 unkCounter1 = 0;
    uint32 unkCounter2 = 0;
    uint32 extraTargets = m_targets.GetExtraTargetsCount();
    uint32 unkCounter4 = 0;

    ObjectGuid predictedHealOverrideTarget;
    ObjectGuid caster = m_caster->GetGUID();
    ObjectGuid powerUnit = caster;
    ObjectGuid itemCaster = m_CastItem ? m_CastItem->GetGUID() : uint64(caster);
    ObjectGuid sourceTransport = m_targets.GetSrc()->_transportGUID;
    ObjectGuid destTransport = m_targets.GetDst()->_transportGUID;
    ObjectGuid unitTargetGUID = m_targets.GetObjectTargetGUID();
    ObjectGuid itemTargetGUID = m_targets.GetItemTargetGUID();

    bool unkBit = false;
    bool unkInt = false;
    uint32 predictedHealAmount = 0;
    bool unkInt3 = false;
    bool unkInt4 = false;
    bool unkInt5 = false;
    bool hasRuneStateBefore = false; // don't needed in spell_start
    bool unkByte2 = false;
    bool hasRuneStateAfter = false; // don't needed in spell_start
    uint8 predictedHealType = 0;
    bool unkByte5 = false;
    bool unkFloat = false;

    // Initialize predicated heal values
    if (m_spellInfo->HasEffect(SPELL_EFFECT_HEAL))
    {
        if (Unit* target = sObjectAccessor->FindUnit(m_targets.GetObjectTargetGUID()))
        {
            castFlags |= CAST_FLAG_HEAL_PREDICTION;
            predictedHealType = 2;
            predictedHealOverrideTarget = target->GetGUID();

            // Guess spell healing amount
            predictedHealAmount = m_caster->CalculateSpellDamage(target, m_spellInfo, EFFECT_0);
            predictedHealAmount = m_caster->SpellHealingBonusDone(target, m_spellInfo, EFFECT_0, predictedHealAmount, HEAL);
            predictedHealAmount = m_caster->SpellHealingBonusTaken(target, m_spellInfo, EFFECT_0, predictedHealAmount, HEAL);
        }
    }

    data.WriteBit(m_targets.HasDst());
    data.WriteBitSeq<6, 7>(itemCaster);
    data.WriteBit(!caster);                                   // !has unk guid 2

    data.WriteBitSeq<2, 6, 0, 3, 4, 1, 7, 5>(unitTargetGUID);

    data.WriteBitSeq<4>(caster);
    data.WriteBitSeq<1>(itemCaster);
    data.WriteBitSeq<2>(caster);
    data.WriteBits(unkCounter1, 24);                        // unk counter, related to hit target, dodge ...etc, useless for SMSG_SPELL_START
    data.WriteBitSeq<0>(itemCaster);
    data.WriteBits(unkCounter2, 24);                        // unk counter 2, related to hit target, dodge ...etc, useless for SMSG_SPELL_START
    data.WriteBitSeq<5>(caster);

    if (m_targets.HasDst())
        data.WriteBitSeq<4, 7, 5, 3, 6, 2, 1, 0>(destTransport);

    data.WriteBit(1);                                       // !fake bit guid, has type 2 target guid

    data.WriteBitSeq<5, 3, 4, 6, 7, 1, 2, 0>(predictedHealOverrideTarget);

    data.WriteBit(!unkBit);                                 // !unkBit
    data.WriteBit(!unkInt);                                 // !has unk int
    data.WriteBit(!hasRuneStateBefore);                     // !has rune before
    data.WriteBit(!predictedHealAmount);                    // !has unk int 2
    data.WriteBitSeq<7>(caster);

    std::vector<ObjectGuid> unkGuids1(unkCounter1);
    for (uint32 i = 0; i < unkCounter1; i++)
        data.WriteBitSeq<5, 3, 1, 4, 6, 0, 2, 7>(unkGuids1[i]);

    std::vector<ObjectGuid> unkGuids2(unkCounter2);
    for (uint32 i = 0; i < unkCounter2; i++)
        data.WriteBitSeq<0, 2, 7, 4, 6, 3, 5, 1>(unkGuids2[i]);

    data.WriteBit(!m_targets.GetTargetMask());              // !hasTargetFlags
    data.WriteBit(!unkInt3);                                // !has unk int 3
    data.WriteBits(powerCount, 21);                         // powerCount
    data.WriteBit(castFlags & CAST_FLAG_POWER_LEFT_SELF);   // hasPowerData
    data.WriteBit(m_targets.HasSrc());
    data.WriteBit(!unkByte2);                               // !has unk byte 2

    if (m_targets.HasSrc())
        data.WriteBitSeq<4, 7, 6, 3, 2, 0, 5, 1>(sourceTransport);

    data.WriteBit(!hasRuneStateAfter);                               // !has rune after
    data.WriteBit(!predictedHealType);                               // !has unk byte 4
    data.WriteBitSeq<4>(itemCaster);

    if (castFlags & CAST_FLAG_POWER_LEFT_SELF)
    {
        data.WriteBitSeq<4>(powerUnit);
        data.WriteBits(1, 21);                              // power type count, always 1 ?
        data.WriteBitSeq<2, 3, 7, 6, 5, 0, 1>(powerUnit);
    }

    data.WriteBit(!unkFloat);                               // !has unk float
    data.WriteBit(!unkInt4);                                // !has unk int 4

    if (m_targets.GetTargetMask())
        data.WriteBits(m_targets.GetTargetMask(), 20);

    data.WriteBits(0, 13);                                  // unk bits
    data.WriteBitSeq<3>(itemCaster);

    uint8 runeCooldownCount = 0;
    //if (m_caster->GetTypeId() == TYPEID_PLAYER && m_caster->getClass() == CLASS_DEATH_KNIGHT)
        //runeCooldownCount = 6;

    data.WriteBits(runeCooldownCount, 3);                   // runeCooldownCount
    data.WriteBit(!itemTargetGUID);                         // !itemTargetGUID
    data.WriteBitSeq<2>(itemCaster);
    data.WriteBit(!unkByte5);                               // !has unk byte 5
    data.WriteBitSeq<3>(caster);
    data.WriteBits(extraTargets, 20);

    for (auto itr : m_targets.GetExtraTargets())
    {
        ObjectGuid extraTargetGuid = itr._transportGUID;
        data.WriteBitSeq<5, 1, 4, 7, 3, 6, 0, 2>(extraTargetGuid);
    }

    data.WriteBitSeq<4, 1, 5, 2, 7, 6, 0, 3>(itemTargetGUID);

    data.WriteBitSeq<1>(caster);
    data.WriteBitSeq<5>(itemCaster);
    data.WriteBitSeq<6>(caster);
    data.WriteBits(unkCounter4, 25);
    data.WriteBitSeq<0>(caster);

    if (unkBit)
        data.WriteBits(unkStringLength, 7);                 // unk string length

    data.WriteBit(!unkInt5);                                // !has unk int 5
    data.WriteBit(castFlags & CAST_FLAG_PROJECTILE);        // hasProjectile

    for (uint32 i = 0; i < unkCounter4; i++)
    {
        uint32 unkVal = 0;

        data.WriteBits(unkVal, 4);                          // unk bits
        if (unkVal == 11)
            data.WriteBits(0, 4);                           // unk bits
    }

    data.WriteByteSeq<5, 3, 4, 2, 0, 1, 7, 6>(itemTargetGUID);

    data.WriteByteSeq<2>(itemCaster);
    data << uint32(m_casttime);

    for (uint32 i = 0; i < unkCounter1; i++)
        data.WriteByteSeq<3, 6, 7, 5, 0, 4, 2, 1>(unkGuids2[i]);

    data.WriteByteSeq<6>(itemCaster);

    for (uint32 i = 0; i < unkCounter2; i++)
        data.WriteByteSeq<6, 0, 3, 7, 2, 1, 5, 4>(unkGuids1[i]);

    data.WriteByteSeq<0>(caster);

    if (m_targets.HasSrc())
    {
        float x, y, z;

        if (m_targets.GetSrc()->_transportGUID)
        {
            x = m_targets.GetSrc()->_transportOffset.m_positionX;
            y = m_targets.GetSrc()->_transportOffset.m_positionY;
            z = m_targets.GetSrc()->_transportOffset.m_positionZ;
        }
        else
        {
            x = m_targets.GetSrc()->_position.m_positionX;
            y = m_targets.GetSrc()->_position.m_positionY;
            z = m_targets.GetSrc()->_position.m_positionZ;
        }

        data.WriteByteSeq<6, 7, 3, 0>(sourceTransport);
        data << float(y);
        data.WriteByteSeq<1>(sourceTransport);
        data << float(z);
        data.WriteByteSeq<4, 2>(sourceTransport);
        data << float(x);
        data.WriteByteSeq<5>(sourceTransport);
    }

    if (unkInt)
        data << uint32(0);                                  // unk uint32

    if (m_targets.HasDst())
    {
        float x, y, z;

        if (m_targets.GetDst()->_transportGUID)
        {
            x = m_targets.GetDst()->_transportOffset.m_positionX;
            y = m_targets.GetDst()->_transportOffset.m_positionY;
            z = m_targets.GetDst()->_transportOffset.m_positionZ;
        }
        else
        {
            x = m_targets.GetDst()->_position.m_positionX;
            y = m_targets.GetDst()->_position.m_positionY;
            z = m_targets.GetDst()->_position.m_positionZ;
        }

        data.WriteByteSeq<5, 1, 6, 0, 3, 4>(destTransport);
        data << float(x);
        data.WriteByteSeq<2>(destTransport);
        data << float(y);
        data << float(z);
        data.WriteByteSeq<7>(destTransport);
    }

    if (unkByte5)
        data << uint8(0);

    data.WriteByteSeq<5, 7>(itemCaster);

    data.WriteByteSeq<1, 7, 3, 0, 6, 2, 4, 5>(predictedHealOverrideTarget);

    if (castFlags & CAST_FLAG_PROJECTILE)
    {
        uint32 ammoDisplayId = 0;
        uint32 inventoryType = 0;

        auto const &spellAmmoMap = sSpellMgr->GetSpellAmmoMap();

        auto const itr = spellAmmoMap.find(m_spellInfo->Id);
        if (itr != spellAmmoMap.end())
        {
            ammoDisplayId = itr->second.ammoDisplayID;
            inventoryType = itr->second.inventoryType;
        }

        data << uint32(ammoDisplayId); // Ammo display ID
        data << uint32(inventoryType); // Inventory Type
    }

    data.WriteByteSeq<4>(itemCaster);

    for (auto itr : m_targets.GetExtraTargets())
    {
        ObjectGuid extraTargetGuid = itr._transportGUID;

        data.WriteByteSeq<4, 5>(extraTargetGuid);
        data << float(itr._position.GetPositionY());
        data.WriteByteSeq<0, 1, 2, 3>(extraTargetGuid);
        data << float(itr._position.GetPositionX());
        data << float(itr._position.GetPositionZ());
        data.WriteByteSeq<6, 7>(extraTargetGuid);
    }

    if (castFlags & CAST_FLAG_POWER_LEFT_SELF)
    {
        data << uint32(m_caster->GetTotalAttackPowerValue(BASE_ATTACK));
        data << uint32(m_caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_ALL));

        // Always 1 ?
        /*for (int i = 0; i < PowerTypeCount; i++)
        {
        }*/

        data << uint32((Powers)m_spellPowerData->PowerType);
        data << uint32(m_caster->GetPower((Powers)m_spellPowerData->PowerType));

        data.WriteByteSeq<4, 7, 0, 2, 3, 5, 6, 1>(powerUnit);

        data << uint32(m_caster->GetHealth());
    }

    data.WriteByteSeq<6>(caster);
    data.WriteByteSeq<3>(itemCaster);

    if (hasRuneStateBefore)
        data << uint8(m_runesState);

    data.WriteByteSeq<3, 2, 1, 4, 6, 0, 7, 5>(unitTargetGUID);
    data.WriteByteSeq<3>(caster);

    if (runeCooldownCount)
    {
        if (Player* player = m_caster->ToPlayer())
        {
            uint32 const baseCd = player->GetRuneBaseCooldown();
            for (uint8 i = 0; i < MAX_RUNES; ++i)
                data << uint8(float(baseCd - player->GetRuneCooldown(i)) / baseCd * 255); // rune cooldown passed
        }
        else
        {
            for (uint8 i = 0; i < MAX_RUNES; ++i)
                data << uint8(0);
        }
    }

    if (unkStringLength > 0)
    {
        std::string unkString = "";
        data << unkString;
    }

    data << uint8(m_cast_count);                            // pending spell cast?

    if (unkInt5)
        data << uint32(0);

    data.WriteByteSeq<7>(caster);

    if (predictedHealAmount)
        data << uint32(predictedHealAmount);

    if (unkFloat)
        data << float(0.0f);

    data << uint32(castFlags);                                      // unk 88
    data.WriteByteSeq<4, 1>(caster);
    data << uint32(m_spellInfo->Id);                        // spellId

    if (predictedHealType)
        data << uint8(predictedHealType);

    for (uint32 i = 0; i < powerCount; i++)
    {
        // not sure about this ...
        data << int32(m_spellPowerData->PowerType);
        data << int32(m_caster->GetPower((Powers)m_spellPowerData->PowerType));
    }

    data.WriteByteSeq<0>(itemCaster);

    if (unkByte2)
        data << uint8(0);

    if (hasRuneStateAfter)
        data << uint8(m_caster->ToPlayer()->GetRunesState());

    if (unkInt4)
        data << uint32(0);

    data.WriteByteSeq<1>(itemCaster);

    if (unkInt3)
        data << uint32(0);

    data.WriteByteSeq<5, 2>(caster);

    m_caster->SendMessageToSet(&data, true);
}

void Spell::SendSpellGo()
{
    // not send invisible spell casting
    if (!IsNeedSendToClient())
        return;

    //TC_LOG_DEBUG("spells", "Sending SMSG_SPELL_GO id=%u", m_spellInfo->Id);

    uint32 castFlags = CAST_FLAG_UNKNOWN_9;

    if ((m_caster->GetTypeId() == TYPEID_PLAYER || (m_caster->GetTypeId() == TYPEID_UNIT && m_caster->ToCreature()->isPet()))
            && m_spellPowerData->PowerType != POWER_HEALTH)
        castFlags |= CAST_FLAG_POWER_LEFT_SELF; // should only be sent to self, but the current messaging doesn't make that possible

    if ((m_caster->GetTypeId() == TYPEID_PLAYER)
        && (m_caster->getClass() == CLASS_DEATH_KNIGHT)
        && m_spellInfo->RuneCostID
        && m_spellPowerData->PowerType == POWER_RUNES)
    {
        castFlags |= CAST_FLAG_UNKNOWN_19;                   // same as in SMSG_SPELL_START
        castFlags |= CAST_FLAG_RUNE_LIST;                    // rune cooldowns list
    }

    if (m_spellInfo->HasEffect(SPELL_EFFECT_ACTIVATE_RUNE))
    {
        castFlags |= CAST_FLAG_RUNE_LIST;                    // rune cooldowns list
        castFlags |= CAST_FLAG_UNKNOWN_19;                   // same as in SMSG_SPELL_START
    }

    if (m_targets.HasTraj())
        castFlags |= CAST_FLAG_ADJUST_MISSILE;

    if (m_spellInfo->AttributesCu & SPELL_ATTR0_CU_USE_AMMO)
        castFlags |= CAST_FLAG_PROJECTILE;

    WorldPacket data(SMSG_SPELL_GO);

    ObjectGuid guid1, guid2, guid6;
    ObjectGuid caster = m_caster->GetGUID();
    ObjectGuid target = m_targets.GetUnitTarget() ? m_targets.GetUnitTarget()->GetGUID() : 0;
    ObjectGuid itemGuid = itemTarget ? itemTarget->GetGUID() : 0;
    ObjectGuid itemCaster = m_castItemGUID ? m_castItemGUID : uint64(caster);
    ObjectGuid powerUnit = caster;
    ObjectGuid transportSrc = m_targets.GetSrc()->_transportGUID;
    ObjectGuid transportDst = m_targets.GetDst()->_transportGUID;

    uint32 powerCount = (m_spellPowerData->PowerType != POWER_HEALTH) ? 1 : 0;
    uint32 powerTypeCount = (m_spellPowerData->PowerType != POWER_HEALTH) ? 1 : 0;
    uint32 runeCooldownCount = 0;
    uint32 unkStringLength = 0;
    uint32 extraTargets = m_targets.GetExtraTargetsCount();

    bool hasSrc = m_targets.HasSrc();
    bool hasDest = m_targets.HasDst();
    bool hasBit48 = false;
    bool hasBit90 = false;
    bool hasBit91 = false;
    bool hasBit101 = false;
    bool hasBit102 = false;
    bool hasBit106 = false;
    bool hasRuneStateBefore = m_runesState;
    bool hasRuneStateAfter = false;
    if (m_caster->GetTypeId() == TYPEID_PLAYER && m_caster->getClass() == CLASS_DEATH_KNIGHT &&  m_caster->ToPlayer()->GetRunesState())
        hasRuneStateAfter = true;
    bool hasDelayMoment = m_delayMoment;
    bool hasBit368 = false;
    bool hasBit384 = false;
    bool hasBit428 = false;

    uint32 missCount = 0;
    uint32 hitCount = 0;

    // Process targets data
    {
        // ---- Miss target ---- //
        for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
        {
            if ((*ihit).effectMask == 0)                  // No effect apply - all immuned add state
                ihit->missCondition = SPELL_MISS_IMMUNE2;
        }

        for (std::list<TargetInfo>::const_iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
            if (ihit->missCondition != SPELL_MISS_NONE)        // Add only miss
                ++missCount;

        // ---- Hit target ---- //
        for (std::list<TargetInfo>::const_iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
        {
            if ((*ihit).missCondition == SPELL_MISS_NONE)       // Add only hits
            {
                m_channelTargetEffectMask |=ihit->effectMask;
                ++hitCount;
            }
        }

        // Reset m_needAliveTargetMask for non channeled spell
        if (!m_spellInfo->IsChanneled())
            m_channelTargetEffectMask = 0;

        for (std::list<GOTargetInfo>::const_iterator ighit = m_UniqueGOTargetInfo.begin(); ighit != m_UniqueGOTargetInfo.end(); ++ighit)
            ++hitCount;
    }

    data.WriteBit(!target);                                       // !hasGuid1
    data.WriteBitSeq<4>(caster);
    data.WriteBit(!hasBit102);                              // !hasBit102
    data.WriteBit(!hasBit368);                              // !hasBit368
    data.WriteBit(!hasBit90);                               // !hasBit90
    data.WriteBit(!itemGuid);                               // !hasGuid2
    data.WriteBitSeq<7>(itemCaster);

    data.WriteBitSeq<2, 5, 6, 1, 0, 3, 7, 4>(target);

    data.WriteBitSeq<1>(caster);
    data.WriteBit(!hasRuneStateAfter);

    data.WriteBitSeq<7, 0, 1, 3, 4, 2, 5, 6>(itemGuid);

    data.WriteBitSeq<2>(itemCaster);
    data.WriteBit(!m_targets.GetTargetMask());              // !hasTargetFlags
    data.WriteBit(!hasBit384);                              // !hasBit384
    data.WriteBitSeq<5>(itemCaster);
    data.WriteBit(!hasBit428);                              // !hasBit428
    data.WriteBits(powerCount, 21);                         // powerCount
    data.WriteBit(!hasBit91);                               // !hasBit91
    data.WriteBits(0, 13);                                  // counter11 - sniffed value 0x10, look like flags, unk, maybe castFlags ?
    data.WriteBit(castFlags & CAST_FLAG_PROJECTILE);        // hasBit380
    data.WriteBit(hasSrc);                                  // hasGuid3
    data.WriteBitSeq<3>(itemCaster);
    data.WriteBits(extraTargets, 20);                       // extraTargets
    data.WriteBitSeq<1>(itemCaster);
    data.WriteBit(caster[0]);
    data.WriteBitSeq<6, 5>(itemCaster);
    data.WriteBit(!hasBit101);                              // !hasBit101
    data.WriteBits(missCount, 24);                          // missCount guid counter
    data.WriteBitSeq<3>(caster);
    data.WriteBit(!hasBit106);                              // !hasBit106
    data.WriteBit(!hasBit48);                               // !hasBit48

    if (m_targets.GetTargetMask())
        data.WriteBits(m_targets.GetTargetMask(), 20);

    data.WriteBitSeq<2>(caster);

    if (hasSrc)
        data.WriteBitSeq<2, 4, 6, 3, 7, 5, 1, 0>(transportSrc);

    for (std::list<TargetInfo>::const_iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
    {
        if (ihit->missCondition != SPELL_MISS_NONE)
        {
            ObjectGuid missGuid = ihit->targetGUID;
            data.WriteBitSeq<6, 7, 0, 2, 5, 4, 1, 3>(missGuid);
        }
    }

    data.WriteBit(hasDest);                                 // hasDest
    data.WriteBit(1);                                       // !hasBit416

    for (auto itr : m_targets.GetExtraTargets())
    {
        ObjectGuid extraTarget = itr._transportGUID;
        data.WriteBitSeq<0, 4, 3, 1, 6, 7, 2, 5>(extraTarget);
    }

    data.WriteBitSeq<4, 6, 7, 0, 1, 2, 3, 5>(guid6);

    data.WriteBit(!hasDelayMoment);                              // !hasDelayMoment

    if (hasDest)
        data.WriteBitSeq<4, 1, 7, 3, 0, 5, 6, 2>(transportDst);

    data.WriteBit(powerUnit != 0);                          // hasPowerUnitGuid
    data.WriteBits(hitCount, 24);                           // hit count

    if (powerUnit)
    {
        data.WriteBitSeq<7, 4, 0, 6>(powerUnit);
        data.WriteBits(powerTypeCount, 21);
        data.WriteBitSeq<5, 2, 3, 1>(powerUnit);
    }

    // Send hit guid bits
    {
        // First units ...
        for (std::list<TargetInfo>::const_iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
        {
            if ((*ihit).missCondition == SPELL_MISS_NONE)
            {
                ObjectGuid hitGuid = ihit->targetGUID;
                data.WriteBitSeq<4, 0, 2, 7, 6, 1, 3, 5>(hitGuid);
            }
        }

        // And GameObjects :)
        for (std::list<GOTargetInfo>::const_iterator ighit = m_UniqueGOTargetInfo.begin(); ighit != m_UniqueGOTargetInfo.end(); ++ighit)
        {
            ObjectGuid hitGuid = ighit->targetGUID;
            data.WriteBitSeq<4, 0, 2, 7, 6, 1, 3, 5>(hitGuid);
        }
    }

    if (m_caster->GetTypeId() == TYPEID_PLAYER && m_caster->getClass() ==  CLASS_DEATH_KNIGHT)
        runeCooldownCount = MAX_RUNES;

    data.WriteBitSeq<0, 4>(itemCaster);
    data.WriteBitSeq<6>(caster);
    data.WriteBits(runeCooldownCount, 3);
    data.WriteBit(!hasRuneStateBefore);
    data.WriteBitSeq<7>(caster);
    data.WriteBits(missCount, 25);

    if (hasBit48)
        data.WriteBits(unkStringLength, 7);

    for (std::list<TargetInfo>::const_iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
    {
        if (ihit->missCondition != SPELL_MISS_NONE)
        {
            data.WriteBits(ihit->missCondition, 4);
            if (ihit->missCondition == SPELL_MISS_REFLECT)
                data.WriteBits(ihit->reflectResult, 4);
        }
    }

    data.WriteByteSeq<7>(caster);

    if (powerUnit)
    {
        data.WriteByteSeq<3, 5, 1, 4>(powerUnit);

        for (uint32 i = 0; i < powerTypeCount; i++)
        {
            data << uint32((Powers)m_spellPowerData->PowerType); //Power
            data << uint32(m_caster->GetPower((Powers)m_spellPowerData->PowerType));
        }

        data.WriteByteSeq<6, 2>(powerUnit);

        // Health, attack power, spellpower
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);

        data.WriteByteSeq<7, 0>(powerUnit);
    }

    data.WriteByteSeq<5, 3, 2, 7, 4, 0, 6, 1>(guid6);

    for (std::list<TargetInfo>::const_iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
    {
        if (ihit->missCondition != SPELL_MISS_NONE)
        {
            ObjectGuid missGuid = ihit->targetGUID;
            data.WriteByteSeq<7, 1, 0, 4, 2, 5, 6, 3>(missGuid);
        }
    }

    if (hasSrc)
    {
        float x, y, z;

        if (m_targets.GetSrc()->_transportGUID)
        {
            x = m_targets.GetSrc()->_transportOffset.m_positionX;
            y = m_targets.GetSrc()->_transportOffset.m_positionY;
            z = m_targets.GetSrc()->_transportOffset.m_positionZ;
        }
        else
        {
            x = m_targets.GetSrc()->_position.m_positionX;
            y = m_targets.GetSrc()->_position.m_positionY;
            z = m_targets.GetSrc()->_position.m_positionZ;
        }

        data.WriteByteSeq<2, 6, 0, 3, 4>(transportSrc);
        data << float(y);
        data << float(x);
        data << float(z);
        data.WriteByteSeq<7, 5, 1>(transportSrc);
    }

    data.WriteByteSeq<0, 6, 5, 7, 3, 2, 4, 1>(itemGuid);
    data.WriteByteSeq<3>(itemCaster);

    // Send hit guid bytes
    {
        // First units ...
        for (std::list<TargetInfo>::const_iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
        {
            if ((*ihit).missCondition == SPELL_MISS_NONE)
            {
                ObjectGuid hitGuid = ihit->targetGUID;
                data.WriteByteSeq<6, 0, 7, 1, 2, 5, 3, 4>(hitGuid);
            }
        }

        // And GameObjects :)
        for (std::list<GOTargetInfo>::const_iterator ighit = m_UniqueGOTargetInfo.begin(); ighit != m_UniqueGOTargetInfo.end(); ++ighit)
        {
            ObjectGuid hitGuid = ighit->targetGUID;
            data.WriteByteSeq<6, 0, 7, 1, 2, 5, 3, 4>(hitGuid);
        }
    }


    if (hasBit90)
        data << float(0.0f); // may be data << m_targets.GetElevation(); ? need to try

    data.WriteByteSeq<3, 1, 5, 0, 7, 6, 4, 2>(target);

    data.WriteByteSeq<3>(caster);

    if (hasBit428)
        data << uint8(0);

    data.WriteByteSeq<4>(itemCaster);

    if (hasBit102)
        data << uint32(0);

    if (hasRuneStateAfter)
        data << uint8(m_caster->ToPlayer()->GetRunesState());

    data.WriteByteSeq<0>(itemCaster);
    data.WriteByteSeq<4>(caster);

    if (hasDest)
    {
        float x, y, z;

        if (m_targets.GetDst()->_transportGUID)
        {
            x = m_targets.GetDst()->_transportOffset.m_positionX;
            y = m_targets.GetDst()->_transportOffset.m_positionY;
            z = m_targets.GetDst()->_transportOffset.m_positionZ;
        }
        else
        {
            x = m_targets.GetDst()->_position.m_positionX;
            y = m_targets.GetDst()->_position.m_positionY;
            z = m_targets.GetDst()->_position.m_positionZ;
        }


        data << float(z);
        data.WriteByteSeq<4>(transportDst);
        data << float(y);
        data << float(x);
        data.WriteByteSeq<7, 3, 2, 1, 6, 0, 5>(transportDst);
    }

    data.WriteByteSeq<2>(itemCaster);
    data.WriteByteSeq<6>(caster);

    if (hasRuneStateBefore)
        data << uint8(m_runesState);

    data.WriteByteSeq<1>(caster);
    data << uint32(getMSTime());                            // timestamp

    for (auto itr : m_targets.GetExtraTargets())
    {
        ObjectGuid extraTarget = itr._transportGUID;

        data.WriteByteSeq<2>(extraTarget);
        data << float(itr._position.GetPositionY());
        data.WriteByteSeq<6>(extraTarget);
        data << float(itr._position.GetPositionZ());
        data << float(itr._position.GetPositionX());
        data.WriteByteSeq<4, 1, 3, 0, 7, 5>(extraTarget);
    }

    if (castFlags & CAST_FLAG_PROJECTILE)
    {
        uint32 ammoDisplayId = 0;
        uint32 inventoryType = 0;

        auto const &spellAmmoMap = sSpellMgr->GetSpellAmmoMap();

        auto const itr = spellAmmoMap.find(m_spellInfo->Id);
        if (itr != spellAmmoMap.end())
        {
            ammoDisplayId = itr->second.ammoDisplayID;
            inventoryType = itr->second.inventoryType;
        }

        data << uint32(ammoDisplayId); // Ammo display ID
        data << uint32(inventoryType); // Inventory Type
    }

    data << uint32(m_spellInfo->Id);                        // spellId

    if (hasBit91)
        data << uint32(0);

    data.WriteByteSeq<6>(itemCaster);

    if (hasBit106)
        data << uint32(0);

    for (uint32 i = 0; i < powerCount; i++)
    {
        data << uint32(m_caster->GetPower((Powers)m_spellPowerData->PowerType));
        data << uint8(m_spellPowerData->PowerType); //Power
    }

    if (runeCooldownCount)
    {
        if (Player* player = m_caster->ToPlayer())
        {
            uint32 const baseCd = player->GetRuneBaseCooldown();
            for (uint8 i = 0; i < MAX_RUNES; ++i)
                data << uint8(float(baseCd - player->GetRuneCooldown(i)) / baseCd * 255); // rune cooldown passed
        }
        else
        {
            for (uint8 i = 0; i < MAX_RUNES; ++i)
                data << uint8(0);
        }
    }

    if (unkStringLength > 0)
    {
        std::string unkString = "";
        data << unkString;
    }

    data.WriteByteSeq<1>(itemCaster);

    if (hasBit368)
        data << uint8(0);

    if (hasDelayMoment)
        data << uint32(m_delayMoment);

    if (hasBit384)
        data << uint8(0);

    data << uint32(castFlags); // unk flags, sniffed from retail
    data.WriteByteSeq<5>(caster);

    if (hasBit101)
        data << uint32(0);

    data.WriteByteSeq<0>(caster);
    data.WriteByteSeq<7, 5>(itemCaster);
    data.WriteByteSeq<2>(caster);

    data << uint8(m_cast_count);

#if 0
    // OLD DATAS 5.0.5

    if (m_CastItem)
        data.append(m_CastItem->GetPackGUID());
    else
        data.append(m_caster->GetPackGUID());

    data.append(m_caster->GetPackGUID());
    data << uint8(m_cast_count);                            // pending spell cast?
    data << uint32(castFlags);                              // cast flags
    data << uint32(m_timer);

    WriteSpellGoTargets(&data);

    m_targets.Write(data);

    if (castFlags & CAST_FLAG_POWER_LEFT_SELF) // done
    {
        data << uint32(1);//m_caster->GetPower((Powers)m_spellInfo->PowerType));


        for (int i = 0; i < m_caster->GetPower((Powers)m_spellInfo->PowerType); ++i)
        {
            data << uint32(0);
            data << uint32(0);
        }
    }
    if (castFlags & CAST_FLAG_RUNE_LIST)                   // rune cooldowns list, done
    {
        //TODO: There is a crash caused by a spell with CAST_FLAG_RUNE_LIST casted by a creature
        //The creature is the mover of a player, so HandleCastSpellOpcode uses it as the caster
        if (Player* player = m_caster->ToPlayer())
        {
            data << uint8(m_runesState);                     // runes state before
            data << uint8(player->GetRunesState());          // runes state after
            for (uint8 i = 0; i < MAX_RUNES; ++i)
            {
                // float casts ensure the division is performed on floats as we need float result
                float baseCd = float(player->GetRuneBaseCooldown(i));
                data << uint8((baseCd - float(player->GetRuneCooldown(i))) / baseCd * 255); // rune cooldown passed
            }
        }
        else
        {
            data << uint8(0);
            data << uint8(0);
            for (uint8 i = 0; i < MAX_RUNES; ++i)
                data << uint8(0);
        }
    }

    if (castFlags & CAST_FLAG_ADJUST_MISSILE) // TODO: find it in bits & bytes ... We need a sniff
    {
        data << m_targets.GetElevation();
        data << uint32(m_delayMoment);
    }

    if (castFlags & CAST_FLAG_PROJECTILE)
    {

    }

    if (castFlags & CAST_FLAG_VISUAL_CHAIN)
    {
        data << uint32(0);
        data << uint32(0);
    }

    if (m_targets.GetTargetMask() & TARGET_FLAG_DEST_LOCATION)
    {
        data << uint8(0);
    }

    if (m_targets.GetTargetMask() & TARGET_FLAG_EXTRA_TARGETS)
    {
        data << uint32(0); // Extra targets count

        for (uint8 i = 0; i < count; ++i)
        {
            data << float(0);   // Target Position X
            data << float(0);   // Target Position Y
            data << float(0);   // Target Position Z
            data << uint64(0);  // Target Guid
        }
    }
#endif

    m_caster->SendMessageToSet(&data, true);
}

/// Writes miss and hit targets for a SMSG_SPELL_GO packet
void Spell::WriteSpellGoTargets(WorldPacket* data)
{
    // This function also fill data for channeled spells:
    // m_needAliveTargetMask req for stop channelig if one target die
    for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
    {
        if ((*ihit).effectMask == 0)                  // No effect apply - all immuned add state

            // possibly SPELL_MISS_IMMUNE2 for this??
            ihit->missCondition = SPELL_MISS_IMMUNE2;
    }

    // Hit and miss target counts are both uint8, that limits us to 255 targets for each
    // sending more than 255 targets crashes the client (since count sent would be wrong)
    // Spells like 40647 (with a huge radius) can easily reach this limit (spell might need
    // target conditions but we still need to limit the number of targets sent and keeping
    // correct count for both hit and miss).

    uint32 hit = 0;
    size_t hitPos = data->wpos();
    *data << (uint8)0; // placeholder
    for (std::list<TargetInfo>::const_iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end() && hit <= 255; ++ihit)

    {
        if ((*ihit).missCondition == SPELL_MISS_NONE)       // Add only hits
        {
            *data << uint64(ihit->targetGUID);
            m_channelTargetEffectMask |=ihit->effectMask;
            ++hit;
        }
    }

    for (std::list<GOTargetInfo>::const_iterator ighit = m_UniqueGOTargetInfo.begin(); ighit != m_UniqueGOTargetInfo.end() && hit <= 255; ++ighit)
    {
        *data << uint64(ighit->targetGUID);                 // Always hits
        ++hit;
    }

    uint32 miss = 0;
    size_t missPos = data->wpos();
    *data << (uint8)0; // placeholder
    for (std::list<TargetInfo>::const_iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end() && miss <= 255; ++ihit)
    {
        if (ihit->missCondition != SPELL_MISS_NONE)        // Add only miss
        {
            *data << uint64(ihit->targetGUID);
            *data << uint8(ihit->missCondition);
            if (ihit->missCondition == SPELL_MISS_REFLECT)
                *data << uint8(ihit->reflectResult);
            ++miss;
        }
    }
    // Reset m_needAliveTargetMask for non channeled spell
    if (!m_spellInfo->IsChanneled())
        m_channelTargetEffectMask = 0;

    data->put<uint8>(hitPos, (uint8)hit);
    data->put<uint8>(missPos, (uint8)miss);
}

void Spell::SendLogExecute()
{
    if (m_effectExecuteData.empty())
        return;

    ObjectGuid guid = m_caster->GetGUID();
    WorldPacket data(SMSG_SPELLLOGEXECUTE, (8+4+4+4+4+8));

    data.WriteBitSeq<6, 5, 7, 3, 1, 4>(guid);

    data.WriteBits(m_effectExecuteData.size(), 19);

    for (auto const &itr : m_effectExecuteData)
    {
        auto const &helper = itr.second;

        data.WriteBits(helper.CreatedItems.size(), 22);
        data.WriteBit(false); // CasterData - temp disabled
        data.WriteBits(0, 21); // unk block Guids1 in parser
        data.WriteBits(0, 21); // unk block Guids2 in parser
        data.WriteBits(helper.Targets.size(), 24);
        for (auto const &target : helper.Targets)
            data.WriteBitSeq<7, 1, 2, 5, 0, 3, 6, 4>(target);

        data.WriteBits(0, 22); // unk counter84
        data.WriteBits(helper.Energizes.size(), 20);

        for (auto const &energize : helper.Energizes)
            data.WriteBitSeq<6, 4, 1, 0, 5, 7, 3, 2>(energize.Guid);
    }

    data.WriteBitSeq<2, 0>(guid);

    for (auto const &itr : m_effectExecuteData)
    {
        auto const &effIndex = itr.first;
        auto const &helper = itr.second;

        for (auto const &id : helper.CreatedItems)
            data << uint32(id);

        for (auto const &energize : helper.Energizes)
        {
            data.WriteByteSeq<3, 7>(energize.Guid);
            data << uint32(energize.PowerType);
            data.WriteByteSeq<5, 0>(energize.Guid);
            data << uint32(energize.Value);
            data.WriteByteSeq<2>(energize.Guid);
            data << float(energize.Multiplier);
            data.WriteByteSeq<1, 6, 4>(energize.Guid);
        }

        for (auto const &target : helper.Targets)
            data.WriteByteSeq<1, 0, 2, 3, 7, 4, 5, 6>(target);

        data << uint32(m_spellInfo->Effects[effIndex].Effect);
    }

    data.WriteByteSeq<1, 6>(guid);
    data << uint32(m_spellInfo->Id);
    data.WriteByteSeq<4, 5, 2, 3, 0, 7>(guid);

    m_caster->SendMessageToSet(&data, true);

    m_effectExecuteData.clear();
}

void Spell::ExecuteLogEffectTakeTargetPower(uint8 effIndex, Unit* target, uint32 powerType, uint32 powerTaken, float gainMultiplier)
{
    InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex].AddEnergize(target->GetGUID(), gainMultiplier, powerTaken, powerType);
}

void Spell::ExecuteLogEffectExtraAttacks(uint8 /*effIndex*/, Unit* /*victim*/, uint32 /*attCount*/)
{
    /*InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex]->append(victim->GetPackGUID());
    *m_effectExecuteData[effIndex] << uint32(attCount);*/
}

void Spell::ExecuteLogEffectInterruptCast(uint8 /*effIndex*/, Unit* victim, uint32 spellId)
{
    ObjectGuid casterGuid = m_caster->GetGUID();
    ObjectGuid targetGuid = victim->GetGUID();

    WorldPacket data(SMSG_SPELL_INTERRUPT_LOG, 8 + 8 + 4 + 4);
    data.WriteBitSeq<5>(casterGuid);
    data.WriteBitSeq<5, 4>(targetGuid);
    data.WriteBitSeq<2>(casterGuid);
    data.WriteBitSeq<0>(targetGuid);
    data.WriteBitSeq<6>(casterGuid);
    data.WriteBit(0);
    data.WriteBitSeq<6>(targetGuid);
    data.WriteBitSeq<4, 1>(casterGuid);
    data.WriteBitSeq<2>(targetGuid);
    data.WriteBitSeq<0>(casterGuid);
    data.WriteBitSeq<3>(targetGuid);
    data.WriteBitSeq<3>(casterGuid);
    data.WriteBitSeq<1>(targetGuid);
    data.WriteBitSeq<7>(casterGuid);
    data.WriteBitSeq<7>(targetGuid);

    data.WriteByteSeq<0, 4>(casterGuid);
    data.WriteByteSeq<3>(targetGuid);
    data.WriteByteSeq<3>(casterGuid);
    data << uint32(m_spellInfo->Id);
    data.WriteByteSeq<4>(targetGuid);
    data.WriteByteSeq<1>(casterGuid);
    data.WriteByteSeq<7, 0>(targetGuid);
    data.WriteByteSeq<5>(casterGuid);
    data << uint32(spellId);
    data.WriteByteSeq<1, 5>(targetGuid);
    data.WriteByteSeq<7>(casterGuid);
    data.WriteByteSeq<6>(targetGuid);
    data.WriteByteSeq<6, 2>(casterGuid);
    data.WriteByteSeq<2>(targetGuid);

    m_caster->SendMessageToSet(&data, true);
}

void Spell::ExecuteLogEffectDurabilityDamage(uint8 /*effIndex*/, Unit* /*victim*/, uint32 /*itemslot*/, uint32 /*damage*/)
{
    /*InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex]->append(victim->GetPackGUID());
    *m_effectExecuteData[effIndex] << uint32(m_spellInfo->Id);
    *m_effectExecuteData[effIndex] << uint32(damage);*/
}

void Spell::ExecuteLogEffectOpenLock(uint8 effIndex, Object* obj)
{
    InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex].AddTarget(obj->GetGUID());
}

void Spell::ExecuteLogEffectCreateItem(uint8 effIndex, uint32 entry)
{
    InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex].AddCreatedItem(entry);
}

void Spell::ExecuteLogEffectDestroyItem(uint8 effIndex, uint32 entry)
{
    InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex].AddCreatedItem(entry);
}

void Spell::ExecuteLogEffectSummonObject(uint8 effIndex, WorldObject* obj)
{
    InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex].AddTarget(obj->GetGUID());
}

void Spell::ExecuteLogEffectUnsummonObject(uint8 effIndex, WorldObject* obj)
{
    InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex].AddTarget(obj->GetGUID());
}

void Spell::ExecuteLogEffectResurrect(uint8 effIndex, Unit* target)
{
    InitEffectExecuteData(effIndex);
    m_effectExecuteData[effIndex].AddTarget(target->GetGUID());
}

void Spell::SendInterrupted(uint8 result)
{
    ObjectGuid casterGuid = m_caster->GetGUID();

    WorldPacket data(SMSG_SPELL_FAILURE, (8+4+1));
    data.WriteBitSeq<2, 1, 7, 0, 5, 3, 4, 6>(casterGuid);

    data << uint8(m_cast_count);
    data.WriteByteSeq<7, 4, 5>(casterGuid);
    data << uint8(result);
    data << uint32(m_spellInfo->Id);
    data.WriteByteSeq<3, 6, 0, 1, 2>(casterGuid);

    m_caster->SendMessageToSet(&data, true);

    data.Initialize(SMSG_SPELL_FAILED_OTHER, (8+4));

    data.WriteBitSeq<5, 6, 7, 0, 4, 3, 1, 2>(casterGuid);

    data.WriteByteSeq<4, 6>(casterGuid);
    data << uint8(result);
    data << uint8(m_cast_count);
    data.WriteByteSeq<0, 7, 2, 1, 3>(casterGuid);
    data << uint32(m_spellInfo->Id);
    data.WriteByteSeq<5>(casterGuid);

    m_caster->SendMessageToSet(&data, true);
}

void Spell::SendChannelUpdate(uint32 time)
{
    if (time == 0)
    {
        m_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
        m_caster->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
    }

    ObjectGuid guid = m_caster->GetGUID();

    WorldPacket data(SMSG_SPELL_CHANNEL_UPDATE, 8+4);
    data.WriteBitSeq<2, 7, 0, 6, 5, 3, 1, 4>(guid);
    data.WriteByteSeq<2, 0, 1, 7, 4, 5>(guid);
    data << uint32(time);
    data.WriteByteSeq<3, 6>(guid);

    m_caster->SendMessageToSet(&data, true);
}

void Spell::SendChannelStart(uint32 duration)
{
    uint64 channelTarget = m_targets.GetObjectTargetGUID();

    if (!channelTarget && !m_spellInfo->NeedsExplicitUnitTarget())
        if (m_UniqueTargetInfo.size() + m_UniqueGOTargetInfo.size() == 1)   // this is for TARGET_SELECT_CATEGORY_NEARBY
            if (!m_UniqueTargetInfo.empty())
                channelTarget = !m_UniqueTargetInfo.empty() ? m_UniqueTargetInfo.front().targetGUID : m_UniqueGOTargetInfo.front().targetGUID;

    WorldPacket data(SMSG_SPELL_CHANNEL_START, (8+4+4));
    ObjectGuid guid = m_caster->GetGUID();

    data.WriteBit(false); // hasHealPrediction
    data.WriteBit(false); // hasCastFlagImmunity
    data.WriteBitSeq<6, 0, 4, 1, 2, 3, 5, 7>(guid);
    data.WriteByteSeq<5, 6, 4>(guid);
    data << uint32(m_spellInfo->Id);
    data.WriteByteSeq<7, 1, 3>(guid);
    data << uint32(duration);
    data.WriteByteSeq<2, 0>(guid);

    m_caster->SendMessageToSet(&data, true);

    m_timer = duration;
    if (channelTarget)
        m_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, channelTarget);

    if (m_spellInfo->Id != 101546)
        m_caster->SetUInt32Value(UNIT_CHANNEL_SPELL, m_spellInfo->Id);
}

void Spell::SendResurrectRequest(Player* target)
{
    // get ressurector name for creature resurrections, otherwise packet will be not accepted
    // for player resurrections the name is looked up by guid
    auto const &resurrectorName = m_caster->GetTypeId() == TYPEID_PLAYER
            ? ""
            : m_caster->GetNameForLocaleIdx(target->GetSession()->GetSessionDbLocaleIndex());

    ObjectGuid guid = m_caster->GetGUID();

    WorldPacket data(SMSG_RESURRECT_REQUEST, (8+4+resurrectorName.length()+1+1+1+4));
    data.WriteBits(resurrectorName.length(), 6);

    data.WriteBitSeq<0, 5>(guid);
    data.WriteBit(m_caster->GetTypeId() == TYPEID_PLAYER ? 0 : 1);
    data.WriteBitSeq<7>(guid);
    data.WriteBit(false);
    data.WriteBitSeq<2, 1, 4, 6, 3>(guid);

    data.WriteByteSeq<2, 4, 1, 5>(guid);

    data << uint32(m_spellInfo->Id);

    data.WriteByteSeq<3, 0>(guid);
    data.WriteString(resurrectorName);

    data.WriteByteSeq<7>(guid);

    data << uint32(0);
    data << uint32(0);

    data.WriteByteSeq<6>(guid);

    target->GetSession()->SendPacket(&data);
}

void Spell::TakeCastItem()
{
    if (!m_CastItem || m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    // not remove cast item at triggered spell (equipping, weapon damage, etc)
    if (_triggeredCastFlags & TRIGGERED_IGNORE_CAST_ITEM)
        return;

    ItemTemplate const* proto = m_CastItem->GetTemplate();

    if (!proto)
    {
        // This code is to avoid a crash
        // I'm not sure, if this is really an error, but I guess every item needs a prototype
        return;
    }

    bool expendable = false;
    bool withoutCharges = false;

    for (int i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
    {
        if (proto->Spells[i].SpellId)
        {
            // item has limited charges
            if (proto->Spells[i].SpellCharges)
            {
                if (proto->Spells[i].SpellCharges < 0)
                    expendable = true;

                int32 charges = m_CastItem->GetSpellCharges(i);

                // item has charges left
                if (charges)
                {
                    (charges > 0) ? --charges : ++charges;  // abs(charges) less at 1 after use
                    if (proto->Stackable == 1)
                        m_CastItem->SetSpellCharges(i, charges);
                    m_CastItem->SetState(ITEM_CHANGED, (Player*)m_caster);
                }

                // all charges used
                withoutCharges = (charges == 0);
            }
        }
    }

    if (expendable && withoutCharges)
    {
        uint32 count = 1;
        m_caster->ToPlayer()->DestroyItemCount(m_CastItem, count, true);

        // prevent crash at access to deleted m_targets.GetItemTarget
        if (m_CastItem == m_targets.GetItemTarget())
            m_targets.SetItemTarget(NULL);

        m_CastItem = NULL;
    }
}

void Spell::TakePower()
{
    if (m_CastItem || m_triggeredByAuraSpell)
        return;

    // Don't take power if the spell is cast while .cheat power is enabled.
    if (m_caster->GetTypeId() == TYPEID_PLAYER)
    {
        if (m_caster->ToPlayer()->GetCommandStatus(CHEAT_POWER))
            return;
    }

    Powers powerType = Powers(m_spellPowerData->PowerType);
    bool hit = true;
    if (m_caster->GetTypeId() == TYPEID_PLAYER)
    {
        if (powerType == POWER_HOLY_POWER || powerType == POWER_ENERGY || powerType == POWER_RUNES)
        {
            if (uint64 targetGUID = m_targets.GetUnitTargetGUID())
            {
                for (std::list<TargetInfo>::iterator ihit= m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
                {
                    if (ihit->targetGUID == targetGUID)
                    {
                        if (ihit->missCondition != SPELL_MISS_NONE)
                        {
                            hit = false;
                            //lower spell cost on fail (by talent aura)
                            if (Player* modOwner = m_caster->ToPlayer()->GetSpellModOwner())
                                modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_SPELL_COST_REFUND_ON_FAIL, m_powerCost);
                        }
                        break;
                    }
                }
            }
        }
    }

    if (powerType == POWER_RUNES)
    {
        TakeRunePower(hit);
        return;
    }

    // In Spell::HandleHolyPower
    if (m_spellInfo->spellPower.PowerType == POWER_HOLY_POWER)
    {
        if (m_spellInfo->Id == 85222)
        {
            m_powerCost = m_caster->GetPower(POWER_HOLY_POWER);

            if (m_powerCost > 3)
                m_powerCost = 3;
        }
    }

    if (!m_powerCost)
        return;

    // health as power used
    if (powerType == POWER_HEALTH)
    {
        m_caster->ModifyHealth(-(int32)m_powerCost);
        return;
    }

    if (powerType >= MAX_POWERS)
        return;

    int32 pct = 20;
    if (powerType == POWER_RUNIC_POWER)
        pct = 10;

    if (hit)
        m_caster->ModifyPower(powerType, -m_powerCost);
    else
        m_caster->ModifyPower(powerType, -CalculatePct(m_powerCost, pct)); // Refund 80% of power on fail 4.x
}

void Spell::TakeAmmo()
{
    if (m_attackType == RANGED_ATTACK && m_caster->GetTypeId() == TYPEID_PLAYER)
    {
        Item* pItem = m_caster->ToPlayer()->GetWeaponForAttack(RANGED_ATTACK);

        // wands don't have ammo
        if (!pItem  || pItem->IsBroken() || pItem->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_WAND)
            return;

        if (pItem->GetTemplate()->InventoryType == INVTYPE_THROWN)
        {
            if (pItem->GetMaxStackCount() == 1)
            {
                // decrease durability for non-stackable throw weapon
                m_caster->ToPlayer()->DurabilityPointLossForEquipSlot(EQUIPMENT_SLOT_MAINHAND);
            }
            else
            {
                // decrease items amount for stackable throw weapon
                uint32 count = 1;
                m_caster->ToPlayer()->DestroyItemCount(pItem, count, true);
            }
        }
    }
}

SpellCastResult Spell::CheckRuneCost(uint32 runeCostID)
{
    if (m_spellPowerData->PowerType != POWER_RUNES || !runeCostID)
        return SPELL_CAST_OK;

    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return SPELL_CAST_OK;

    Player* player = (Player*)m_caster;

    if (player->getClass() != CLASS_DEATH_KNIGHT)
        return SPELL_CAST_OK;

    SpellRuneCostEntry const* src = sSpellRuneCostStore.LookupEntry(runeCostID);

    if (!src)
        return SPELL_CAST_OK;

    if (src->NoRuneCost())
        return SPELL_CAST_OK;

    int32 runeCost[NUM_RUNE_TYPES];                         // blood, frost, unholy, death

    for (uint32 i = 0; i < RUNE_DEATH; ++i)
    {
        runeCost[i] = src->RuneCost[i];
        if (Player* modOwner = m_caster->GetSpellModOwner())
            modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_COST, runeCost[i], this, false);
    }

    runeCost[RUNE_DEATH] = MAX_RUNES;                       // calculated later

    for (uint32 i = 0; i < MAX_RUNES; ++i)
    {
        RuneType rune = player->GetCurrentRune(i);
        if ((player->GetRuneCooldown(i) == 0) && (runeCost[rune] > 0))
            runeCost[rune]--;
    }

    for (uint32 i = 0; i < RUNE_DEATH; ++i)
        if (runeCost[i] > 0)
            runeCost[RUNE_DEATH] += runeCost[i];

    runeCost[RUNE_DEATH] += src->RuneCost[RUNE_DEATH];

    if (runeCost[RUNE_DEATH] > MAX_RUNES)
        return SPELL_FAILED_NO_POWER;                       // not sure if result code is correct

    return SPELL_CAST_OK;
}

void Spell::TakeRunePower(bool didHit)
{
    if (m_caster->GetTypeId() != TYPEID_PLAYER || m_caster->getClass() != CLASS_DEATH_KNIGHT)
        return;

    SpellRuneCostEntry const* runeCostData = sSpellRuneCostStore.LookupEntry(m_spellInfo->RuneCostID);
    if (!runeCostData || (runeCostData->NoRuneCost() && runeCostData->NoRunicPowerGain()))
        return;

    Player* player = m_caster->ToPlayer();
    m_runesState = player->GetRunesState();                 // store previous state

    int32 runeCost[NUM_RUNE_TYPES];                         // blood, frost, unholy, death
    player->ClearLastUsedRuneMask();
    SpellSchools school = GetFirstSchoolInMask(m_spellSchoolMask);

    for (uint32 i = 0; i < RUNE_DEATH; ++i)
    {
        runeCost[i] = runeCostData->RuneCost[i];
        if (Player* modOwner = m_caster->GetSpellModOwner())
        {
            modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_COST, runeCost[i], this);

            // PCT mod from user auras by school
            runeCost[i] = int32(runeCost[i] * (1.0f + m_caster->GetFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER + school)));
            if (runeCost[i] < 0)
                runeCost[i] = 0;
        }
    }

    runeCost[RUNE_DEATH] = 0;                               // calculated later

    bool gain_runic = runeCostData->NoRuneCost();           //  if spell doesn't have runecost - player can have some runic power, Horn of Winter for example

    uint32 const cooldown = ((m_spellInfo->SpellFamilyFlags[0] & SPELLFAMILYFLAG_DK_DEATH_STRIKE) > 0 || didHit)
            ? player->GetRuneBaseCooldown() : uint32(RUNE_MISS_COOLDOWN);

    for (uint32 i = 0; i < MAX_RUNES; ++i)
    {
        RuneType rune = player->GetCurrentRune(i);
        if (player->GetRuneCooldown(i) || !runeCost[rune])
            continue;

        player->SetRuneCooldown(i, cooldown);
        player->SetDeathRuneUsed(i, false);
        player->SetLastUsedRuneIndex(i);

        runeCost[rune]--;
        gain_runic = true;
    }

    runeCost[RUNE_DEATH] = runeCost[RUNE_BLOOD] + runeCost[RUNE_UNHOLY] + runeCost[RUNE_FROST];
    if (!runeCost[RUNE_DEATH])
        runeCost[RUNE_DEATH] = runeCostData->RuneCost[RUNE_DEATH];

    if (runeCost[RUNE_DEATH] > 0)
    {
        for (uint32 i = 0; i < MAX_RUNES; ++i)
        {
            RuneType rune = player->GetCurrentRune(i);
            if (!player->GetRuneCooldown(i) && rune == RUNE_DEATH)
            {
                player->SetRuneCooldown(i, cooldown);
                player->SetLastUsedRuneIndex(i);
                runeCost[rune]--;

                gain_runic = true;

                bool takePower = didHit;
                if (uint32 spell = player->GetRuneConvertSpell(i))
                    takePower = spell != 54637;

                if (player->IsRunePermanentlyConverted(i))
                    takePower = false;

                // keep Death Rune type if missed or player has Blood of the North or rune is permanently converted
                if (takePower)
                {
                    player->RestoreBaseRune(i);
                    player->SetDeathRuneUsed(i, true);
                }

                if (runeCost[RUNE_DEATH] == 0)
                    break;
            }
        }
    }

    // you can gain some runic power when use runes
    if (didHit && gain_runic)
    {
        if (int32 rp = int32(runeCostData->runePowerGain * sWorld->getRate(RATE_POWER_RUNICPOWER_INCOME)))
        {
            AddPct(rp, player->GetTotalAuraModifier(SPELL_AURA_MOD_RUNE_REGEN_SPEED));
            player->ModifyPower(POWER_RUNIC_POWER, int32(rp));
        }
    }
}

void Spell::TakeReagents()
{
    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return;

    ItemTemplate const* castItemTemplate = m_CastItem ? m_CastItem->GetTemplate() : NULL;

    // do not take reagents for these item casts
    if (castItemTemplate && castItemTemplate->Flags & ITEM_PROTO_FLAG_TRIGGERED_CAST)
        return;

    Player* p_caster = m_caster->ToPlayer();
    if (p_caster->CanNoReagentCast(m_spellInfo))
        return;

    for (uint32 x = 0; x < MAX_SPELL_REAGENTS; ++x)
    {
        if (m_spellInfo->Reagent[x] <= 0)
            continue;

        uint32 itemid = m_spellInfo->Reagent[x];
        uint32 itemcount = m_spellInfo->ReagentCount[x];

        // if CastItem is also spell reagent
        if (castItemTemplate && castItemTemplate->ItemId == itemid)
        {
            for (int s = 0; s < MAX_ITEM_PROTO_SPELLS; ++s)
            {
                // CastItem will be used up and does not count as reagent
                int32 charges = m_CastItem->GetSpellCharges(s);
                if (castItemTemplate->Spells[s].SpellCharges < 0 && abs(charges) < 2)
                {
                    ++itemcount;
                    break;
                }
            }

            m_CastItem = NULL;
        }

        // if GetItemTarget is also spell reagent
        if (m_targets.GetItemTargetEntry() == itemid)
            m_targets.SetItemTarget(NULL);

        p_caster->DestroyItemCount(itemid, itemcount, true);
    }
}

void Spell::HandleThreatSpells()
{
    if (m_UniqueTargetInfo.empty())
        return;

    if ((m_spellInfo->AttributesEx  & SPELL_ATTR1_NO_THREAT) ||
        (m_spellInfo->AttributesEx3 & SPELL_ATTR3_NO_INITIAL_AGGRO))
        return;

    float threat = 0.0f;
    if (SpellThreatEntry const* threatEntry = sSpellMgr->GetSpellThreatEntry(m_spellInfo->Id))
    {
        if (threatEntry->apPctMod != 0.0f)
            threat += threatEntry->apPctMod * m_caster->GetTotalAttackPowerValue(BASE_ATTACK);

        threat += threatEntry->flatMod;
    }
    else if ((m_spellInfo->AttributesCu & SPELL_ATTR0_CU_NO_INITIAL_THREAT) == 0)
        threat += m_spellInfo->SpellLevel;

    // past this point only multiplicative effects occur
    if (threat == 0.0f)
        return;

    // since 2.0.1 threat from positive effects also is distributed among all targets, so the overall caused threat is at most the defined bonus
    threat /= m_UniqueTargetInfo.size();

    for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
    {
        if (ihit->missCondition != SPELL_MISS_NONE)
            continue;

        Unit* target = ObjectAccessor::GetUnit(*m_caster, ihit->targetGUID);
        if (!target)
            continue;

        // positive spells distribute threat among all units that are in combat with target, like healing
        if (m_spellInfo->_IsPositiveSpell())
            target->getHostileRefManager().threatAssist(m_caster, threat, m_spellInfo);
        // for negative spells threat gets distributed among affected targets
        else
        {
            if (!target->CanHaveThreatList())
                continue;

            target->AddThreat(m_caster, threat, m_spellInfo->GetSchoolMask(), m_spellInfo);
        }
    }
}

void Spell::HandleHolyPower(Player* caster)
{
    if (!caster)
        return;

    // Templar's Verdict and Eternal Flame - Don't remove power twice
    if (m_spellInfo->Id == 85256 || m_spellInfo->Id == 114163)
        return;

    // Word of Glory - Don't remove power twice (idem for Light of Dawn)
    if (m_spellInfo->Id == 85673 || m_spellInfo->Id == 85222)
        return;

    // Shield of the Righteous - Don't remove power twice
    if (m_spellInfo->Id == 53600)
        return;

    bool hit = true;
    Player* modOwner = caster->GetSpellModOwner();
    m_powerCost = caster->GetPower(POWER_HOLY_POWER); // Always use all the holy power we have
    if (!m_powerCost || !modOwner)
        return;

    if (uint64 targetGUID = m_targets.GetUnitTargetGUID())
    {
        for (std::list<TargetInfo>::iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
        {
            if (ihit->targetGUID == targetGUID)
            {
                if (ihit->missCondition != SPELL_MISS_NONE && ihit->missCondition != SPELL_MISS_MISS)
                    hit = false;
                break;
            }
        }

        // The spell did hit the target, apply aura cost mods if there are any.
        if (hit)
        {
            modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_COST, m_powerCost);
            m_caster->ModifyPower(POWER_HOLY_POWER, -m_powerCost);
        }
    }
}

void Spell::HandleEffects(Unit* pUnitTarget, Item* pItemTarget, GameObject* pGOTarget, uint32 i, SpellEffectHandleMode mode)
{
    effectHandleMode = mode;
    unitTarget = pUnitTarget;
    itemTarget = pItemTarget;
    gameObjTarget = pGOTarget;
    destTarget = &m_destTargets[i]._position;

    damage = GetSpellInfo()->Effects[i].CalcValue(m_caster, &m_spellValue.EffectBasePoints[i], unitTarget, m_CastItem);

    bool const preventDefault = CallScriptEffectHandlers((SpellEffIndex)i, mode);

    auto const eff = GetSpellInfo()->Effects[i].Effect;
    if (!preventDefault && eff < TOTAL_SPELL_EFFECTS)
    {
        (this->*SpellEffects[eff])((SpellEffIndex)i);
    }
}

SpellCastResult Spell::CheckCast(bool strict)
{
    // Custom Spell_failed
    if (m_spellInfo->IsCustomCastCanceled(m_caster))
        return SPELL_FAILED_DONT_REPORT;

    // check death state
    if (!m_caster->IsAlive() && !(m_spellInfo->Attributes & SPELL_ATTR0_PASSIVE) && !((m_spellInfo->Attributes & SPELL_ATTR0_CASTABLE_WHILE_DEAD) || (IsTriggered() && !m_triggeredByAuraSpell)))
        return SPELL_FAILED_CASTER_DEAD;

    // check cooldowns to prevent cheating
    if (m_caster->GetTypeId() == TYPEID_PLAYER && !(m_spellInfo->Attributes & SPELL_ATTR0_PASSIVE))
    {
        auto const player = m_caster->ToPlayer();

        //can cast triggered (by aura only?) spells while have this flag
        if (!(_triggeredCastFlags & TRIGGERED_IGNORE_CASTER_AURASTATE) && player->HasFlag(PLAYER_FLAGS, PLAYER_ALLOW_ONLY_ABILITY))
        {
            Unit::AuraEffectList const& allowedAb = m_caster->GetAuraEffectsByType(SPELL_AURA_ALLOW_ONLY_ABILITY);
            for (Unit::AuraEffectList::const_iterator i = allowedAb.begin(); i != allowedAb.end(); ++i)
            {
                if (!(*i)->GetSpellEffectInfo().SpellClassMask)
                    return SPELL_FAILED_SPELL_IN_PROGRESS;

                if (!((*i)->GetSpellEffectInfo().SpellClassMask & m_spellInfo->SpellFamilyFlags))
                    return SPELL_FAILED_SPELL_IN_PROGRESS;
            }
        }

        if (player->HasSpellCooldown(m_spellInfo->Id) && !player->HasAuraTypeWithAffectMask(SPELL_AURA_ALLOW_CAST_WHILE_IN_COOLDOWN, m_spellInfo))
        {
            if (m_triggeredByAuraSpell)
                return SPELL_FAILED_DONT_REPORT;
            else
                return SPELL_FAILED_NOT_READY;
        }

        auto const categories = m_spellInfo->GetSpellCategories();
        if (categories && categories->ChargesCategory != 0) {
            auto const category = sSpellCategoryStore.LookupEntry(categories->ChargesCategory);
            if (category && !player->HasSpellCharge(m_spellInfo->Id, *category))
                return m_triggeredByAuraSpell ? SPELL_FAILED_DONT_REPORT : SPELL_FAILED_NOT_READY;
        }
    }

    if (m_spellInfo->AttributesEx7 & SPELL_ATTR7_IS_CHEAT_SPELL && !m_caster->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_ALLOW_CHEAT_SPELLS))
    {
        m_customError = SPELL_CUSTOM_ERROR_GM_ONLY;
        return SPELL_FAILED_CUSTOM_ERROR;
    }

    // Check global cooldown
    if (strict && !(_triggeredCastFlags & TRIGGERED_IGNORE_GCD) && HasGlobalCooldown() && !m_spellInfo->DoesIgnoreGlobalCooldown(m_caster))
        return SPELL_FAILED_NOT_READY;

    // only triggered spells can be processed an ended battleground
    if (!IsTriggered() && m_caster->GetTypeId() == TYPEID_PLAYER)
        if (Battleground* bg = m_caster->ToPlayer()->GetBattleground())
            if (bg->GetStatus() == STATUS_WAIT_LEAVE)
                return SPELL_FAILED_DONT_REPORT;

    if (m_caster->GetTypeId() == TYPEID_PLAYER && VMAP::VMapFactory::createOrGetVMapManager()->isLineOfSightCalcEnabled())
    {
        if (m_spellInfo->Attributes & SPELL_ATTR0_OUTDOORS_ONLY &&
                !m_caster->GetMap()->IsOutdoors(m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ()))
            return SPELL_FAILED_ONLY_OUTDOORS;

        if (m_spellInfo->Attributes & SPELL_ATTR0_INDOORS_ONLY &&
                m_caster->GetMap()->IsOutdoors(m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ()))
            return SPELL_FAILED_ONLY_INDOORS;
    }

    // only check at first call, Stealth auras are already removed at second call
    // for now, ignore triggered spells
    if (strict && !(_triggeredCastFlags & TRIGGERED_IGNORE_SHAPESHIFT))
    {
        bool checkForm = true;
        // Ignore form req aura
        Unit::AuraEffectList const& ignore = m_caster->GetAuraEffectsByType(SPELL_AURA_MOD_IGNORE_SHAPESHIFT);
        for (Unit::AuraEffectList::const_iterator i = ignore.begin(); i != ignore.end(); ++i)
        {
            if (!(*i)->IsAffectingSpell(m_spellInfo))
                continue;
            checkForm = false;
            break;
        }
        if (checkForm)
        {
            // Cannot be used in this stance/form
            SpellCastResult shapeError = m_spellInfo->CheckShapeshift(m_caster->GetShapeshiftForm());
            if (shapeError != SPELL_CAST_OK)
                return shapeError;

            if ((m_spellInfo->Attributes & SPELL_ATTR0_ONLY_STEALTHED) && !(m_caster->HasStealthAura()))
                return SPELL_FAILED_ONLY_STEALTHED;
        }
    }

    Unit::AuraEffectList const& blockSpells = m_caster->GetAuraEffectsByType(SPELL_AURA_BLOCK_SPELL_FAMILY);
    for (Unit::AuraEffectList::const_iterator blockItr = blockSpells.begin(); blockItr != blockSpells.end(); ++blockItr)
        if (uint32((*blockItr)->GetMiscValue()) == m_spellInfo->SpellFamilyName)
            return SPELL_FAILED_SPELL_UNAVAILABLE;

    bool reqCombat = true;
    Unit::AuraEffectList const& stateAuras = m_caster->GetAuraEffectsByType(SPELL_AURA_ABILITY_IGNORE_AURASTATE);
    for (Unit::AuraEffectList::const_iterator j = stateAuras.begin(); j != stateAuras.end(); ++j)
    {
        if ((*j)->IsAffectingSpell(m_spellInfo))
        {
            m_needComboPoints = false;
            if ((*j)->GetMiscValue() == 1)
            {
                reqCombat=false;
                break;
            }
        }
    }

    // caster state requirements
    // not for triggered spells (needed by execute)
    if (!(_triggeredCastFlags & TRIGGERED_IGNORE_CASTER_AURASTATE))
    {
        // Custom MoP Script
        // 76856 - Mastery : Unshackled Fury - Hack Fix fake check cast
        if (m_spellInfo->Id == 76856 && m_caster->ToPlayer() && m_caster->ToPlayer()->getLevel() > 80)
            return SPELL_CAST_OK;
        if (m_spellInfo->CasterAuraState && !m_caster->HasAuraState(AuraStateType(m_spellInfo->CasterAuraState), m_spellInfo, m_caster))
            return SPELL_FAILED_CASTER_AURASTATE;
        if (m_spellInfo->CasterAuraStateNot && m_caster->HasAuraState(AuraStateType(m_spellInfo->CasterAuraStateNot), m_spellInfo, m_caster))
            return SPELL_FAILED_CASTER_AURASTATE;

        // Note: spell 62473 requres casterAuraSpell = triggering spell
        if (!((m_spellInfo->Id == 48020 || m_spellInfo->Id == 114794) && m_spellInfo->CasterAuraSpell == 62388) && m_spellInfo->CasterAuraSpell && !m_caster->HasAura(m_spellInfo->CasterAuraSpell))
            return SPELL_FAILED_CASTER_AURASTATE;
        if (m_spellInfo->ExcludeCasterAuraSpell && m_caster->HasAura(m_spellInfo->ExcludeCasterAuraSpell))
            return SPELL_FAILED_CASTER_AURASTATE;

        if (reqCombat && m_caster->IsInCombat() && !m_spellInfo->CanBeUsedInCombat())
            return SPELL_FAILED_AFFECTING_COMBAT;
    }

    // cancel autorepeat spells if cast start when moving
    // (not wand currently autorepeat cast delayed to moving stop anyway in spell update code)
    // Do not cancel spells which are affected by a SPELL_AURA_CAST_WHILE_WALKING effect
    if (m_caster->GetTypeId() == TYPEID_PLAYER && m_caster->ToPlayer()->isMoving() && !m_caster->HasAuraTypeWithAffectMask(SPELL_AURA_CAST_WHILE_WALKING, m_spellInfo))
    {
        // skip stuck spell to allow use it in falling case and apply spell limitations at movement
        if ((!m_caster->HasUnitMovementFlag(MOVEMENTFLAG_FALLING_FAR) || m_spellInfo->Effects[0].Effect != SPELL_EFFECT_STUCK) &&
            (IsAutoRepeat() || (m_spellInfo->AuraInterruptFlags & AURA_INTERRUPT_FLAG_NOT_SEATED) != 0))
            return SPELL_FAILED_MOVING;
    }

    // Check vehicle flags
    if (!(_triggeredCastFlags & TRIGGERED_IGNORE_CASTER_MOUNTED_OR_ON_VEHICLE))
    {
        SpellCastResult vehicleCheck = m_spellInfo->CheckVehicle(m_caster);
        if (vehicleCheck != SPELL_CAST_OK)
            return vehicleCheck;
    }

    // check spell cast conditions from database
    {
        ConditionSourceInfo condInfo = ConditionSourceInfo(m_caster);
        condInfo.mConditionTargets[1] = m_targets.GetObjectTarget();
        ConditionList conditions = sConditionMgr->GetConditionsForNotGroupedEntry(CONDITION_SOURCE_TYPE_SPELL, m_spellInfo->Id);
        if (!conditions.empty() && !sConditionMgr->IsObjectMeetToConditions(condInfo, conditions))
        {
            // mLastFailedCondition can be NULL if there was an error processing the condition in Condition::Meets (i.e. wrong data for ConditionTarget or others)
            if (condInfo.mLastFailedCondition && condInfo.mLastFailedCondition->ErrorType)
            {
                if (condInfo.mLastFailedCondition->ErrorType == SPELL_FAILED_CUSTOM_ERROR)
                    m_customError = SpellCustomErrors(condInfo.mLastFailedCondition->ErrorTextId);
                return SpellCastResult(condInfo.mLastFailedCondition->ErrorType);
            }
            if (!condInfo.mLastFailedCondition || !condInfo.mLastFailedCondition->ConditionTarget)
                return SPELL_FAILED_CASTER_AURASTATE;
            return SPELL_FAILED_BAD_TARGETS;
        }
    }

    // Don't check explicit target for passive spells (workaround) (check should be skipped only for learn case)
    // those spells may have incorrect target entries or not filled at all (for example 15332)
    // such spells when learned are not targeting anyone using targeting system, they should apply directly to caster instead
    // also, such casts shouldn't be sent to client
    if (!IsTriggered() || !m_targets.GetUnitTarget())
    {
        if (!((m_spellInfo->Attributes & SPELL_ATTR0_PASSIVE) && (!m_targets.GetUnitTarget() || m_targets.GetUnitTarget() == m_caster)))
        {
            // Check explicit target for m_originalCaster - todo: get rid of such workarounds
            SpellCastResult castResult = m_spellInfo->CheckExplicitTarget(m_originalCaster ? m_originalCaster : m_caster, m_targets.GetObjectTarget(), m_targets.GetItemTarget());
            if (castResult != SPELL_CAST_OK)
                return castResult;
        }
    }

    if (Unit* target = m_targets.GetUnitTarget())
    {
        SpellCastResult castResult = m_spellInfo->CheckTarget(m_caster, target, IsTriggered() ? true: false);
        if (castResult != SPELL_CAST_OK)
            return castResult;

        if (strict && IsMorePowerfulAura(target))
            return IsTriggered() ? SPELL_FAILED_DONT_REPORT: SPELL_FAILED_AURA_BOUNCED;

        if (target != m_caster)
        {
            // Must be behind the target
            if ((m_spellInfo->AttributesCu & SPELL_ATTR0_CU_REQ_CASTER_BEHIND_TARGET) && target->HasInArc(static_cast<float>(M_PI), m_caster))
            {
                // Ambush with Cloak and Dagger shouln't have requirement as it teleports player behind
                if (m_spellInfo->Id != 8676 || !m_caster->HasAura(138106))
                    return SPELL_FAILED_NOT_BEHIND;
            }

            // Target must be facing you
            if ((m_spellInfo->AttributesCu & SPELL_ATTR0_CU_REQ_TARGET_FACING_CASTER) && !target->HasInArc(static_cast<float>(M_PI), m_caster))
                return SPELL_FAILED_NOT_INFRONT;

            // Gouge and Glyph of Gouge
            if (m_spellInfo->Id == 1776 && !m_caster->HasAura(56809) && !target->HasInArc(static_cast<float>(M_PI), m_caster))
                return SPELL_FAILED_NOT_INFRONT;

            if (!IsTriggered())
            {
                // Hackfix for Raigonn
                if (m_caster->GetEntry() != WORLD_TRIGGER && target->GetEntry() != 56895) // Ignore LOS for gameobjects casts (wrongly casted by a trigger)
                    if (!(m_spellInfo->AttributesEx2 & SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS) && !DisableMgr::IsDisabledFor(DISABLE_TYPE_SPELL, m_spellInfo->Id, NULL, SPELL_DISABLE_LOS) && !m_caster->IsWithinLOSInMap(target))
                        return SPELL_FAILED_LINE_OF_SIGHT;

                // Smoke Bomb, Camouflage
                if (m_caster->IsVisionObscured(target, m_spellInfo))
                    return SPELL_FAILED_VISION_OBSCURED;
            }
        }
    }

    // Check for line of sight for spells with dest
    if (m_targets.HasDst())
    {
        float x, y, z;
        m_targets.GetDstPos()->GetPosition(x, y, z);

        if (!(m_spellInfo->AttributesEx2 & SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS) && !DisableMgr::IsDisabledFor(DISABLE_TYPE_SPELL, m_spellInfo->Id, NULL, SPELL_DISABLE_LOS) && !m_caster->IsWithinLOS(x, y, z))
            return SPELL_FAILED_LINE_OF_SIGHT;
    }

    // check pet presence
    for (auto const &spellEffect : m_spellInfo->Effects)
    {
        if (spellEffect.TargetA.GetTarget() == TARGET_UNIT_PET)
        {
            if (!m_caster->GetGuardianPet())
            {
                if (m_triggeredByAuraSpell)              // not report pet not existence for triggered spells
                    return SPELL_FAILED_DONT_REPORT;
                else
                    return SPELL_FAILED_NO_PET;
            }
            break;
        }
    }

    // Spell casted only on battleground
    if ((m_spellInfo->AttributesEx3 & SPELL_ATTR3_BATTLEGROUND) &&  m_caster->GetTypeId() == TYPEID_PLAYER)
        if (!m_caster->ToPlayer()->InBattleground())
            return SPELL_FAILED_ONLY_BATTLEGROUNDS;

    Player* player = m_caster->ToPlayer();
    if (!player && m_caster->GetOwner())
        player = m_caster->GetOwner()->ToPlayer();
    // do not allow spells to be cast in arenas or rated battlegrounds
    if (player && player->InArena())
    {
        SpellCastResult castResult = CheckArenaAndRatedBattlegroundCastRules();
        if (castResult != SPELL_CAST_OK)
            return castResult;
    }

    // zone check
    if (m_caster->GetTypeId() == TYPEID_UNIT || !m_caster->ToPlayer()->isGameMaster())
    {
        uint32 zone, area;
        m_caster->GetZoneAndAreaId(zone, area);

        SpellCastResult locRes= m_spellInfo->CheckLocation(m_caster->GetMapId(), zone, area,
            m_caster->GetTypeId() == TYPEID_PLAYER ? m_caster->ToPlayer() : NULL);
        if (locRes != SPELL_CAST_OK)
            return locRes;
    }

    // not let players cast spells at mount (and let do it to creatures)
    if (m_caster->IsMounted() && m_caster->GetTypeId() == TYPEID_PLAYER && !(_triggeredCastFlags & TRIGGERED_IGNORE_CASTER_MOUNTED_OR_ON_VEHICLE) &&
        !m_spellInfo->IsPassive() && !(m_spellInfo->Attributes & SPELL_ATTR0_CASTABLE_WHILE_MOUNTED))
    {
        if (m_caster->isInFlight())
            return SPELL_FAILED_NOT_ON_TAXI;
        else
            return SPELL_FAILED_NOT_MOUNTED;
    }

    SpellCastResult castResult = SPELL_CAST_OK;

    // always (except passive spells) check items (focus object can be required for any type casts)
    if (!m_spellInfo->IsPassive())
    {
        castResult = CheckItems();
        if (castResult != SPELL_CAST_OK)
            return castResult;
    }

    // Triggered spells also have range check
    // TODO: determine if there is some flag to enable/disable the check
    castResult = CheckRange(strict);
    if (castResult != SPELL_CAST_OK)
        return castResult;

    if (!(_triggeredCastFlags & TRIGGERED_IGNORE_POWER_AND_REAGENT_COST))
    {
        castResult = CheckPower();
        if (castResult != SPELL_CAST_OK)
            return castResult;
    }

    if (!(_triggeredCastFlags & TRIGGERED_IGNORE_CASTER_AURAS))
    {
        castResult = CheckCasterAuras();
        if (castResult != SPELL_CAST_OK)
            return castResult;
    }

    // script hook
    castResult = CallScriptCheckCastHandlers();
    if (castResult != SPELL_CAST_OK)
        return castResult;

    for (uint8 i = 0; i < m_spellInfo->Effects.size(); ++i)
    {
        // for effects of spells that have only one target
        switch (m_spellInfo->Effects[i].Effect)
        {
            case SPELL_EFFECT_KNOCK_BACK_DEST:
            {
                switch (m_spellInfo->Id)
                {
                    case 68645: // Rocket Pack
                        if (!m_caster->GetTransport())
                            return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
                        break;
                }
                break;
            }
            case SPELL_EFFECT_DUMMY:
            {
                // Death Coil and Death Coil (Symbiosis)
                if (m_spellInfo->SpellFamilyName == SPELLFAMILY_DEATHKNIGHT
                        && (m_spellInfo->SpellFamilyFlags[0] == 0x2000 || m_spellInfo->Id == 122282))
                {
                    Unit* target = m_targets.GetUnitTarget();
                    if (!target || (target->IsFriendlyTo(m_caster) && target->GetCreatureType() != CREATURE_TYPE_UNDEAD))
                    {
                        // Glyph of Death Coils
                        if (!m_caster->HasAura(63333) || m_caster == target)
                            return SPELL_FAILED_BAD_TARGETS;
                    }
                    if (!target->IsFriendlyTo(m_caster) && !m_caster->HasInArc(static_cast<float>(M_PI), target))
                        return SPELL_FAILED_UNIT_NOT_INFRONT;
                }
                else if (m_spellInfo->Id == 19938)          // Awaken Peon
                {
                    Unit* unit = m_targets.GetUnitTarget();
                    if (!unit || !unit->HasAura(17743))
                        return SPELL_FAILED_BAD_TARGETS;
                }
                else if (m_spellInfo->Id == 31789)          // Righteous Defense
                {
                    if (m_caster->GetTypeId() != TYPEID_PLAYER)
                        return SPELL_FAILED_DONT_REPORT;

                    Unit* target = m_targets.GetUnitTarget();
                    if (!target || !target->IsFriendlyTo(m_caster) || target->getAttackers().empty())
                        return SPELL_FAILED_BAD_TARGETS;

                }
                break;
            }
            case SPELL_EFFECT_LEARN_SPELL:
            {
                if (m_caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_BAD_TARGETS;

                if (m_spellInfo->Effects[i].TargetA.GetTarget() != TARGET_UNIT_PET)
                    break;

                Pet* pet = m_caster->ToPlayer()->GetPet();

                if (!pet)
                    return SPELL_FAILED_NO_PET;

                SpellInfo const* learn_spellproto = sSpellMgr->GetSpellInfo(m_spellInfo->Effects[i].TriggerSpell);

                if (!learn_spellproto)
                    return SPELL_FAILED_NOT_KNOWN;

                if (m_spellInfo->SpellLevel > pet->getLevel())
                    return SPELL_FAILED_LOWLEVEL;

                break;
            }
            case SPELL_EFFECT_LEARN_PET_SPELL:
            {
                // check target only for unit target case
                if (Unit* unitTarget = m_targets.GetUnitTarget())
                {
                    if (m_caster->GetTypeId() != TYPEID_PLAYER)
                        return SPELL_FAILED_BAD_TARGETS;

                    Pet* pet = unitTarget->ToPet();
                    if (!pet || pet->GetOwner() != m_caster)
                        return SPELL_FAILED_BAD_TARGETS;

                    SpellInfo const* learn_spellproto = sSpellMgr->GetSpellInfo(m_spellInfo->Effects[i].TriggerSpell);

                    if (!learn_spellproto)
                        return SPELL_FAILED_NOT_KNOWN;

                    if (m_spellInfo->SpellLevel > pet->getLevel())
                        return SPELL_FAILED_LOWLEVEL;
                }
                break;
            }
            case SPELL_EFFECT_APPLY_GLYPH:
            {
                uint32 glyphId = m_spellInfo->Effects[i].MiscValue;
                if (GlyphPropertiesEntry const* gp = sGlyphPropertiesStore.LookupEntry(glyphId))
                    if (m_caster->HasAura(gp->SpellId))
                        return SPELL_FAILED_UNIQUE_GLYPH;
                break;
            }
            case SPELL_EFFECT_FEED_PET:
            {
                if (m_caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_BAD_TARGETS;

                Item* foodItem = m_targets.GetItemTarget();
                if (!foodItem)
                    return SPELL_FAILED_BAD_TARGETS;

                Pet* pet = m_caster->ToPlayer()->GetPet();

                if (!pet)
                    return SPELL_FAILED_NO_PET;

                if (!pet->HaveInDiet(foodItem->GetTemplate()))
                    return SPELL_FAILED_WRONG_PET_FOOD;

                if (!pet->GetCurrentFoodBenefitLevel(foodItem->GetTemplate()->ItemLevel))
                    return SPELL_FAILED_FOOD_LOWLEVEL;

                if (m_caster->IsInCombat() || pet->IsInCombat())
                    return SPELL_FAILED_AFFECTING_COMBAT;

                break;
            }
            case SPELL_EFFECT_POWER_BURN:
            case SPELL_EFFECT_POWER_DRAIN:
            {
                // Can be area effect, Check only for players and not check if target - caster (spell can have multiply drain/burn effects)
                if (m_caster->GetTypeId() == TYPEID_PLAYER)
                    if (Unit* target = m_targets.GetUnitTarget())
                        if (target != m_caster && target->getPowerType() != Powers(m_spellInfo->Effects[i].MiscValue))
                            return SPELL_FAILED_BAD_TARGETS;
                break;
            }
            case SPELL_EFFECT_CHARGE:
            {
                if (m_spellInfo->SpellFamilyName == SPELLFAMILY_WARRIOR)
                {
                    // Warbringer - can't be handled in proc system - should be done before checkcast root check and charge effect process
                    if (strict && m_caster->IsScriptOverriden(m_spellInfo, 6953))
                        m_caster->RemoveMovementImpairingAuras();
                    // Intervene and Safeguard can be casted in root effects, so we need to remove movement impairing auras before check cast result
                    if (m_spellInfo->Id == 3411 || m_spellInfo->Id == 114029)
                        m_caster->RemoveMovementImpairingAuras();
                }
                if (m_caster->HasUnitState(UNIT_STATE_ROOT))
                    return SPELL_FAILED_ROOTED;
                if (m_caster->GetTypeId() == TYPEID_PLAYER)
                    if (Unit* target = m_targets.GetUnitTarget())
                        if (!target->IsAlive())
                            return SPELL_FAILED_BAD_TARGETS;
                break;
            }
            case SPELL_EFFECT_SKINNING:
            {
                if (m_caster->GetTypeId() != TYPEID_PLAYER || !m_targets.GetUnitTarget() || m_targets.GetUnitTarget()->GetTypeId() != TYPEID_UNIT)
                    return SPELL_FAILED_BAD_TARGETS;

                if (!(m_targets.GetUnitTarget()->GetUInt32Value(UNIT_FIELD_FLAGS) & UNIT_FLAG_SKINNABLE))
                    return SPELL_FAILED_TARGET_UNSKINNABLE;

                Creature* creature = m_targets.GetUnitTarget()->ToCreature();
                if (creature->GetCreatureType() != CREATURE_TYPE_CRITTER && !creature->loot.isLooted())
                    return SPELL_FAILED_TARGET_NOT_LOOTED;

                uint32 skill = creature->GetCreatureTemplate()->GetRequiredLootSkill();

                int32 skillValue = m_caster->ToPlayer()->GetSkillValue(skill);
                int32 TargetLevel = m_targets.GetUnitTarget()->getLevel();
                int32 ReqValue = (skillValue < 100 ? (TargetLevel-10) * 10 : TargetLevel * 5);
                if (ReqValue > skillValue)
                    return SPELL_FAILED_LOW_CASTLEVEL;

                // chance for fail at orange skinning attempt
                if ((m_selfContainer && (*m_selfContainer) == this) &&
                    skillValue < sWorld->GetConfigMaxSkillValue() &&
                    (ReqValue < 0 ? 0 : ReqValue) > irand(skillValue - 25, skillValue + 37))
                    return SPELL_FAILED_TRY_AGAIN;

                break;
            }
            case SPELL_EFFECT_OPEN_LOCK:
            {
                if (m_spellInfo->Effects[i].TargetA.GetTarget() != TARGET_GAMEOBJECT_TARGET &&
                    m_spellInfo->Effects[i].TargetA.GetTarget() != TARGET_GAMEOBJECT_ITEM_TARGET)
                    break;

                if (m_caster->GetTypeId() != TYPEID_PLAYER  // only players can open locks, gather etc.
                    // we need a go target in case of TARGET_GAMEOBJECT_TARGET
                    || (m_spellInfo->Effects[i].TargetA.GetTarget() == TARGET_GAMEOBJECT_TARGET && !m_targets.GetGOTarget()))
                    return SPELL_FAILED_BAD_TARGETS;

                Item* pTempItem = NULL;
                if (m_targets.GetTargetMask() & TARGET_FLAG_TRADE_ITEM)
                {
                    if (TradeData* pTrade = m_caster->ToPlayer()->GetTradeData())
                        pTempItem = pTrade->GetTraderData()->GetItem(TradeSlots(m_targets.GetItemTargetGUID()));
                }
                else if (m_targets.GetTargetMask() & TARGET_FLAG_ITEM)
                    pTempItem = m_caster->ToPlayer()->GetItemByGuid(m_targets.GetItemTargetGUID());

                // we need a go target, or an openable item target in case of TARGET_GAMEOBJECT_ITEM_TARGET
                if (m_spellInfo->Effects[i].TargetA.GetTarget() == TARGET_GAMEOBJECT_ITEM_TARGET &&
                    !m_targets.GetGOTarget() &&
                    (!pTempItem || !pTempItem->GetTemplate()->LockID || !pTempItem->IsLocked()))
                    return SPELL_FAILED_BAD_TARGETS;

                if (m_spellInfo->Id != 1842 || (m_targets.GetGOTarget() &&
                    m_targets.GetGOTarget()->GetGOInfo()->type != GAMEOBJECT_TYPE_TRAP))
                    if (m_caster->ToPlayer()->InBattleground() && // In Battleground players can use only flags and banners
                        !m_caster->ToPlayer()->CanUseBattlegroundObject())
                        return SPELL_FAILED_TRY_AGAIN;

                // get the lock entry
                uint32 lockId = 0;
                if (GameObject* go = m_targets.GetGOTarget())
                {
                    lockId = go->GetGOInfo()->GetLockId();
                    if (!lockId)
                        return SPELL_FAILED_BAD_TARGETS;
                }
                else if (Item* itm = m_targets.GetItemTarget())
                    lockId = itm->GetTemplate()->LockID;

                SkillType skillId = SKILL_NONE;
                int32 reqSkillValue = 0;
                int32 skillValue = 0;

                // check lock compatibility
                SpellCastResult res = CanOpenLock(i, lockId, skillId, reqSkillValue, skillValue);
                if (res != SPELL_CAST_OK)
                    return res;

                // chance for fail at orange mining/herb/LockPicking gathering attempt
                // second check prevent fail at rechecks
                if (skillId != SKILL_NONE && (!m_selfContainer || ((*m_selfContainer) != this)))
                {
                    bool canFailAtMax = skillId != SKILL_HERBALISM && skillId != SKILL_MINING;

                    // chance for failure in orange gather / lockpick (gathering skill can't fail at maxskill)
                    if ((canFailAtMax || skillValue < sWorld->GetConfigMaxSkillValue()) && reqSkillValue > irand(skillValue - 25, skillValue + 37))
                        return SPELL_FAILED_TRY_AGAIN;
                }
                break;
            }
            case SPELL_EFFECT_RESURRECT_PET:
            {
                Creature* pet = m_caster->GetGuardianPet();
                if (!pet)
                    return SPELL_FAILED_NO_PET;

                if (pet->IsAlive())
                    return SPELL_FAILED_ALREADY_HAVE_SUMMON;

                break;
            }
            // This is generic summon effect
            case SPELL_EFFECT_SUMMON:
            {
                SummonPropertiesEntry const* SummonProperties = sSummonPropertiesStore.LookupEntry(m_spellInfo->Effects[i].MiscValueB);
                if (!SummonProperties)
                    break;
                switch (SummonProperties->Category)
                {
                    case SUMMON_CATEGORY_PET:
                        if (m_caster->GetPetGUID())
                            return SPELL_FAILED_ALREADY_HAVE_SUMMON;
                    case SUMMON_CATEGORY_PUPPET:
                        if (m_caster->GetCharmGUID())
                            return SPELL_FAILED_ALREADY_HAVE_CHARM;
                        break;
                }
                break;
            }
            case SPELL_EFFECT_CREATE_TAMED_PET:
            {
                if (m_targets.GetUnitTarget())
                {
                    if (m_targets.GetUnitTarget()->GetTypeId() != TYPEID_PLAYER)
                        return SPELL_FAILED_BAD_TARGETS;
                    if (m_targets.GetUnitTarget()->GetPetGUID())
                        return SPELL_FAILED_ALREADY_HAVE_SUMMON;
                }
                break;
            }
            case SPELL_EFFECT_SUMMON_PET:
            {
                if (m_caster->GetPetGUID())                  //let warlock do a replacement summon
                {
                    if (m_caster->GetTypeId() == TYPEID_PLAYER && m_caster->getClass() == CLASS_WARLOCK)
                    {
                        if (strict)                         //starting cast, trigger pet stun (cast by pet so it doesn't attack player)
                            if (Pet* pet = m_caster->ToPlayer()->GetPet())
                                pet->CastSpell(pet, 32752, true, NULL, NULL, pet->GetGUID());
                    }
                    else
                        return SPELL_FAILED_ALREADY_HAVE_SUMMON;
                }

                if (m_caster->GetCharmGUID())
                    return SPELL_FAILED_ALREADY_HAVE_CHARM;
                break;
            }
            case SPELL_EFFECT_SUMMON_PLAYER:
            {
                if (m_caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_BAD_TARGETS;
                if (!m_caster->ToPlayer()->GetSelection())
                    return SPELL_FAILED_BAD_TARGETS;

                Player* target = ObjectAccessor::FindPlayer(m_caster->ToPlayer()->GetSelection());
                if (!target || m_caster->ToPlayer() == target || (!target->IsInSameRaidWith(m_caster->ToPlayer()) && m_spellInfo->Id != 48955)) // refer-a-friend spell
                    return SPELL_FAILED_BAD_TARGETS;

                // check if our map is dungeon
                MapEntry const* map = sMapStore.LookupEntry(m_caster->GetMapId());
                if (map->IsDungeon())
                {
                    uint32 mapId = m_caster->GetMap()->GetId();
                    Difficulty difficulty = m_caster->GetMap()->GetDifficulty();
                    if (map->IsRaid())
                        if (InstancePlayerBind* targetBind = target->GetBoundInstance(mapId, difficulty))
                            if (InstancePlayerBind* casterBind = m_caster->ToPlayer()->GetBoundInstance(mapId, difficulty))
                                if (targetBind->perm && targetBind->save != casterBind->save)
                                    return SPELL_FAILED_TARGET_LOCKED_TO_RAID_INSTANCE;

                    InstanceTemplate const* instance = sObjectMgr->GetInstanceTemplate(mapId);
                    if (!instance)
                        return SPELL_FAILED_TARGET_NOT_IN_INSTANCE;
                    if (!target->Satisfy(sObjectMgr->GetAccessRequirement(mapId, difficulty), mapId))
                        return SPELL_FAILED_BAD_TARGETS;
                }
                break;
            }
            // RETURN HERE
            case SPELL_EFFECT_SUMMON_RAF_FRIEND:
            {
                if (m_caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_BAD_TARGETS;

                Player* playerCaster = m_caster->ToPlayer();
                    //
                if (!(playerCaster->GetSelection()))
                    return SPELL_FAILED_BAD_TARGETS;

                Player* target = ObjectAccessor::FindPlayer(playerCaster->GetSelection());

                if (!target ||
                    !(target->GetSession()->GetRecruiterId() == playerCaster->GetSession()->GetAccountId() || target->GetSession()->GetAccountId() == playerCaster->GetSession()->GetRecruiterId()))
                    return SPELL_FAILED_BAD_TARGETS;

                break;
            }
            case SPELL_EFFECT_LEAP:
            case SPELL_EFFECT_TELEPORT_UNITS_FACE_CASTER:
            {
              //Do not allow to cast it before BG starts.
                if (m_caster->GetTypeId() == TYPEID_PLAYER)
                    if (Battleground const* bg = m_caster->ToPlayer()->GetBattleground())
                        if (bg->GetStatus() != STATUS_IN_PROGRESS)
                            return SPELL_FAILED_TRY_AGAIN;
                break;
            }
            case SPELL_EFFECT_STEAL_BENEFICIAL_BUFF:
            {
                if (m_targets.GetUnitTarget() == m_caster)
                    return SPELL_FAILED_BAD_TARGETS;
                break;
            }
            case SPELL_EFFECT_LEAP_BACK:
            {
                if (m_caster->HasUnitState(UNIT_STATE_ROOT))
                {
                    if (m_caster->GetTypeId() == TYPEID_PLAYER)
                    {
                        switch (m_spellInfo->Id)
                        {
                            case 781:   // Disengage
                                if (m_caster->HasAura(109215)) // Posthaste
                                    break;
                                return SPELL_FAILED_ROOTED;
                            default:
                                return SPELL_FAILED_ROOTED;
                        }
                    }
                    else
                        return SPELL_FAILED_DONT_REPORT;
                }
                break;
            }
            case SPELL_EFFECT_TALENT_SPEC_SELECT:
                // can't change during already started arena/battleground
                if (m_caster->GetTypeId() == TYPEID_PLAYER)
                    if (Battleground const* bg = m_caster->ToPlayer()->GetBattleground())
                        if (bg->GetStatus() == STATUS_IN_PROGRESS)
                            return SPELL_FAILED_NOT_IN_BATTLEGROUND;
                break;
            case SPELL_EFFECT_UNLOCK_GUILD_VAULT_TAB:
            {
                if (m_caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_BAD_TARGETS;
                if (Guild* guild = m_caster->ToPlayer()->GetGuild())
                    if (guild->GetLeaderGUID() != m_caster->ToPlayer()->GetGUID())
                        return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
                break;
            }
            case SPELL_EFFECT_SEND_EVENT:
                if (m_caster->GetMap()->IsEventScriptActive(m_spellInfo->Effects[i].MiscValue))
                    return SPELL_FAILED_CHEST_IN_USE;
                break;
            default:
                break;
        }
    }

    for (uint8 i = 0; i < m_spellInfo->Effects.size(); ++i)
    {
        switch (m_spellInfo->Effects[i].ApplyAuraName)
        {
            case SPELL_AURA_MOD_POSSESS:
            case SPELL_AURA_MOD_CHARM:
            case SPELL_AURA_AOE_CHARM:
            {
                if (m_caster->GetCharmerGUID())
                    return SPELL_FAILED_CHARMED;

                if (m_spellInfo->Effects[i].ApplyAuraName == SPELL_AURA_MOD_CHARM
                    || m_spellInfo->Effects[i].ApplyAuraName == SPELL_AURA_MOD_POSSESS)
                {
                    if (m_caster->GetPetGUID())
                        return SPELL_FAILED_ALREADY_HAVE_SUMMON;

                    if (m_caster->GetCharmGUID() || (m_spellInfo->Id == 605 && m_caster->HasAura(605, m_caster->GetGUID())))
                        return SPELL_FAILED_ALREADY_HAVE_CHARM;
                }

                if (Unit* target = m_targets.GetUnitTarget())
                {
                    if (target->GetTypeId() == TYPEID_UNIT && target->ToCreature()->IsVehicle())
                        return SPELL_FAILED_BAD_IMPLICIT_TARGETS;

                    if (target->IsMounted())
                        return SPELL_FAILED_CANT_BE_CHARMED;

                    if (target->GetCharmerGUID())
                        return SPELL_FAILED_CHARMED;

                    int32 damage = CalculateDamage(i, target);
                    if (damage && int32(target->getLevel()) > damage)
                        return SPELL_FAILED_HIGHLEVEL;
                }

                break;
            }
            case SPELL_AURA_MOUNTED:
            {
                // Ignore map check if spell have AreaId. AreaId already checked and this prevent special mount spells
                bool allowMount = !m_caster->GetMap()->IsDungeon() || m_caster->GetMap()->IsBattlegroundOrArena();
                InstanceTemplate const* it = sObjectMgr->GetInstanceTemplate(m_caster->GetMapId());
                if (it)
                    allowMount = it->AllowMount;
                if (m_caster->GetTypeId() == TYPEID_PLAYER && !allowMount && !m_spellInfo->AreaGroupId)
                    return SPELL_FAILED_NO_MOUNTS_ALLOWED;

                if (m_caster->IsInDisallowedMountForm())
                    return SPELL_FAILED_NOT_SHAPESHIFT;

                break;
            }
            case SPELL_AURA_RANGED_ATTACK_POWER_ATTACKER_BONUS:
            {
                if (!m_targets.GetUnitTarget())
                    return SPELL_FAILED_BAD_IMPLICIT_TARGETS;

                // can be casted at non-friendly unit or own pet/charm
                if (m_caster->IsFriendlyTo(m_targets.GetUnitTarget()))
                    return SPELL_FAILED_TARGET_FRIENDLY;

                break;
            }
            case SPELL_AURA_FLY:
            case SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED:
            {
                // not allow cast fly spells if not have req. skills  (all spells is self target)
                // allow always ghost flight spells
                if (m_originalCaster && m_originalCaster->GetTypeId() == TYPEID_PLAYER && m_originalCaster->IsAlive())
                {
                    SpellCastResult const failReason = (_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR)
                            ? SPELL_FAILED_DONT_REPORT
                            : SPELL_FAILED_NOT_HERE;

                    AreaTableEntry const * const area = GetAreaEntryByAreaID(m_originalCaster->GetAreaId());
                    if (area && (area->flags & AREA_FLAG_NO_FLY_ZONE) != 0)
                        return failReason;

					if (sWorld->getBoolConfig(CONFIG_APRIL_FOOLS_NO_FLYING))
						return failReason;

                    Battlefield const * const battleField = sBattlefieldMgr->GetBattlefieldToZoneId(m_originalCaster->GetZoneId());
                    if (battleField && !battleField->CanFlyIn())
                        return failReason;
                }
                break;
            }
            case SPELL_AURA_PERIODIC_MANA_LEECH:
            {
                if (m_spellInfo->Effects[i].IsTargetingArea())
                    break;

                if (!m_targets.GetUnitTarget())
                    return SPELL_FAILED_BAD_IMPLICIT_TARGETS;

                if (m_caster->GetTypeId() != TYPEID_PLAYER || m_CastItem)
                    break;

                if (m_targets.GetUnitTarget()->getPowerType() != POWER_MANA)
                    return SPELL_FAILED_BAD_TARGETS;

                break;
            }
            case SPELL_AURA_MOD_HEALTH_REGEN_PERCENT:
            {
                // Health Funnel
                if (m_spellInfo->Id == 755)
                {
                    if (m_caster->GetTypeId() == TYPEID_PLAYER)
                        if (Pet * pet = m_caster->ToPlayer()->GetPet())
                            if (pet->IsFullHealth())
                                return SPELL_FAILED_ALREADY_AT_FULL_HEALTH;
                }
                break;
            }
            case SPELL_AURA_MOD_ATTACKER_MELEE_HIT_CHANCE:
            case SPELL_AURA_FEIGN_DEATH:
            {
                // Feign Death and Deterrence is disabled while carrying flag
                if (m_spellInfo->Id == 5384 || m_spellInfo->Id == 19263)
                    if (m_caster->HasAura(23335) || m_caster->HasAura(23333))
                    {
                        m_customError = SPELL_CUSTOM_ERROR_HOLDING_FLAG;
                        return SPELL_FAILED_CUSTOM_ERROR;
                    }
                break;
            }
            case SPELL_AURA_MOD_PACIFY:
            {
                // Hand of Protection is disabled if target is carrying flag
                if (m_spellInfo->Id == 1022)
                {
                    Unit *target = m_targets.GetUnitTarget();
                    if (!target)
                        break;
                    if (target->HasAura(23335) || target->HasAura(23333))
                    {
                        this->m_customError = SPELL_CUSTOM_ERROR_HOLDING_FLAG;
                        return SPELL_FAILED_CUSTOM_ERROR;
                    }
                    break;
                }
                break;
            }
            default:
                break;
        }
    }

    switch(m_spellInfo->Id)
    {
        case 50334: // Berserk
        case 61336: // Survival Instincts
        {
            if (m_caster->GetTypeId() != TYPEID_PLAYER || !m_caster->ToPlayer()->IsInFeralForm())
                return SPELL_FAILED_ONLY_SHAPESHIFT;

            break;
        }
        case 5217: // Tiger's Fury
        {
            if (m_caster->GetShapeshiftForm() != FORM_CAT)
            {
                m_customError = SPELL_CUSTOM_ERROR_MUST_BE_IN_CAT_FORM;
                return SPELL_FAILED_CUSTOM_ERROR;
            }
            break;
        }
        default: break;
    }

    // hex
    if (m_caster->HasAuraWithMechanic(1 << MECHANIC_POLYMORPH))
    {
        // Shapeshift
        if (m_spellInfo->HasAura(SPELL_AURA_MOD_SHAPESHIFT) && m_spellInfo->Id != 33891)    // Tree of Life is exception
            return SPELL_FAILED_CHARMED;

        // Teleport
        if (m_spellInfo->HasEffect(SPELL_EFFECT_TELEPORT_UNITS))
            return SPELL_FAILED_CHARMED;

        // Summon totem
        if (m_spellInfo->HasAttribute(SPELL_ATTR7_SUMMON_TOTEM) || m_spellInfo->HasEffect(SPELL_EFFECT_CAST_BUTTON))
            return SPELL_FAILED_CHARMED;

        // Fishing
        if (m_spellInfo->HasAttribute(SPELL_ATTR1_IS_FISHING))
            return SPELL_FAILED_CHARMED;

        // Death Knight - Outbreak
        if (m_spellInfo->Id == 77575)
            return SPELL_FAILED_CHARMED;

        // Drink
        switch (m_spellInfo->GetSpellSpecific())
        {
            case SPELL_SPECIFIC_DRINK:
            case SPELL_SPECIFIC_FOOD:
            case SPELL_SPECIFIC_FOOD_AND_DRINK:
                return SPELL_FAILED_CHARMED;
        default:
            break;
        }
    }

    // check trade slot case (last, for allow catch any another cast problems)
    if (m_targets.GetTargetMask() & TARGET_FLAG_TRADE_ITEM)
    {
        if (m_CastItem)
            return SPELL_FAILED_ITEM_ENCHANT_TRADE_WINDOW;

        if (m_caster->GetTypeId() != TYPEID_PLAYER)
            return SPELL_FAILED_NOT_TRADING;

        TradeData* my_trade = m_caster->ToPlayer()->GetTradeData();

        if (!my_trade)
            return SPELL_FAILED_NOT_TRADING;

        TradeSlots slot = TradeSlots(m_targets.GetItemTargetGUID());
        if (slot != TRADE_SLOT_NONTRADED)
            return SPELL_FAILED_BAD_TARGETS;

        if (!IsTriggered())
            if (my_trade->GetSpell())
                return SPELL_FAILED_ITEM_ALREADY_ENCHANTED;
    }

    // check if caster has at least 1 combo point for spells that require combo points
    if (m_needComboPoints)
        if (Player* plrCaster = m_caster->ToPlayer())
            if (!plrCaster->GetComboPoints())
                return SPELL_FAILED_NO_COMBO_POINTS;

    // all ok
    return SPELL_CAST_OK;
}

SpellCastResult Spell::CheckPetCast(Unit* target)
{
    if (m_caster->HasUnitState(UNIT_STATE_CASTING) && !(_triggeredCastFlags & TRIGGERED_IGNORE_CAST_IN_PROGRESS))              //prevent spellcast interruption by another spellcast
        return SPELL_FAILED_SPELL_IN_PROGRESS;

    // dead owner (pets still alive when owners ressed?)
    if (Unit* owner = m_caster->GetCharmerOrOwner())
        if (!owner->IsAlive())
            return SPELL_FAILED_CASTER_DEAD;

    if (!target && m_targets.GetUnitTarget())
        target = m_targets.GetUnitTarget();

    if (m_spellInfo->NeedsExplicitUnitTarget())
    {
        if (!target)
            return SPELL_FAILED_BAD_IMPLICIT_TARGETS;
        m_targets.SetUnitTarget(target);
    }

    // cooldown
    if (Creature const* creatureCaster = m_caster->ToCreature())
        if (creatureCaster->HasSpellCooldown(m_spellInfo->Id))
            return SPELL_FAILED_NOT_READY;

    return CheckCast(true);
}

SpellCastResult Spell::CheckCasterAuras() const
{
    // spells totally immuned to caster auras (wsg flag drop, give marks etc)
    if (m_spellInfo->AttributesEx6 & SPELL_ATTR6_IGNORE_CASTER_AURAS)
        return SPELL_CAST_OK;

    uint8 school_immune = 0;
    uint32 mechanic_immune = 0;
    uint32 dispel_immune = 0;

    // Check if the spell grants school or mechanic immunity.
    // We use bitmasks so the loop is done only once and not on every aura check below.
    if (m_spellInfo->AttributesEx & SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY)
    {
        for (auto const &spellEffect : GetSpellInfo()->Effects)
        {
            if (spellEffect.ApplyAuraName == SPELL_AURA_SCHOOL_IMMUNITY)
                school_immune |= uint32(spellEffect.MiscValue);
            else if (spellEffect.ApplyAuraName == SPELL_AURA_MECHANIC_IMMUNITY)
                mechanic_immune |= 1 << uint32(spellEffect.MiscValue);
            else if (spellEffect.ApplyAuraName == SPELL_AURA_DISPEL_IMMUNITY)
                dispel_immune |= SpellInfo::GetDispelMask(DispelType(spellEffect.MiscValue));
        }

        // immune movement impairment and loss of control
        if (m_spellInfo->IsRemoveLossControlEffects())
            mechanic_immune = IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK;

        // Custom mechanic immunitys - TODO: Find a better way to handle this
        // Tremor totem for example has USABLE_WHILE_FEARED - This should most likely include MECHANIC_FEAR, MECHANIC_SLEEP & MECHANIC_CHARM
        switch (m_spellInfo->Id)
        {
            // Tremor Totem
            case 8143:
                mechanic_immune = (1 << MECHANIC_FEAR) | (1 << MECHANIC_SLEEP) | (1 << MECHANIC_CHARM);
                break;
        }
    }

    bool usableInStun = m_spellInfo->AttributesEx5 & SPELL_ATTR5_USABLE_WHILE_STUNNED;

    // Glyph of Life Cocoon
    if (m_spellInfo->Id == 116849 && m_caster->HasAura(124989))
        usableInStun = true;

    // Check whether the cast should be prevented by any state you might have.
    SpellCastResult prevented_reason = SPELL_CAST_OK;
    // Have to check if there is a stun aura. Otherwise will have problems with ghost aura apply while logging out
    uint32 unitflag = m_caster->GetUInt32Value(UNIT_FIELD_FLAGS);     // Get unit state
    if (unitflag & UNIT_FLAG_STUNNED)
    {
        // spell is usable while stunned, check if caster has only mechanic stun auras, another stun types must prevent cast spell
        if (usableInStun)
        {
            bool foundNotStun = false;
            Unit::AuraEffectList const& stunAuras = m_caster->GetAuraEffectsByType(SPELL_AURA_MOD_STUN);
            for (Unit::AuraEffectList::const_iterator i = stunAuras.begin(); i != stunAuras.end(); ++i)
            {
                if ((*i)->GetSpellInfo()->GetAllEffectsMechanicMask() && !((*i)->GetSpellInfo()->GetAllEffectsMechanicMask() & (1<<MECHANIC_STUN)))
                {
                    // @TODO: Fix this mess
                    // Sap & Hand of Freedom hack
                    if ((*i)->GetSpellInfo()->Id == 6770 && m_spellInfo->Id == 1044)
                        continue;

                    // Freezing trap hack
                    if ((*i)->GetSpellInfo()->Id == 3355 && (m_spellInfo->Id == 33206 || m_spellInfo->Id == 47788))
                        continue;

                    // Cold snap should be usable while in Ice block
                    if ((*i)->GetSpellInfo()->Id == 45438 && m_spellInfo->Id == 11958)
                        continue;

                    // Unbound Will
                    if (m_spellInfo->Id == 108482)
                        continue;

                    foundNotStun = true;
                    break;
                }
            }
            if (foundNotStun && m_spellInfo->Id != 22812)
                prevented_reason = SPELL_FAILED_STUNNED;
        }
        else
            prevented_reason = SPELL_FAILED_STUNNED;
    }
    else if (unitflag & UNIT_FLAG_CONFUSED && !(m_spellInfo->AttributesEx5 & SPELL_ATTR5_USABLE_WHILE_CONFUSED))
        prevented_reason = SPELL_FAILED_CONFUSED;
    else if (unitflag & UNIT_FLAG_FLEEING && !(m_spellInfo->AttributesEx5 & SPELL_ATTR5_USABLE_WHILE_FEARED))
        prevented_reason = SPELL_FAILED_FLEEING;
    else if (unitflag & UNIT_FLAG_SILENCED && m_spellInfo->PreventionType == SPELL_PREVENTION_TYPE_SILENCE)
        prevented_reason = SPELL_FAILED_SILENCED;
    else if (unitflag & UNIT_FLAG_PACIFIED && m_spellInfo->PreventionType == SPELL_PREVENTION_TYPE_PACIFY)
        prevented_reason = SPELL_FAILED_PACIFIED;
    else if (unitflag & UNIT_FLAG_PACIFIED &&
        (m_spellInfo->PreventionType == SPELL_PREVENTION_TYPE_UNK2 ||
        m_spellInfo->Id == 1850) && m_spellInfo->Id != 781) // THIS ... IS ... HACKYYYY !
        prevented_reason = SPELL_FAILED_PACIFIED;

    // SPELL_PREVENTION_TYPE_UNK3 aka Prevention_type_silence_and_pacify?
    if (m_spellInfo->PreventionType == SPELL_PREVENTION_TYPE_UNK3)
    {
        if (unitflag & UNIT_FLAG_SILENCED)
            prevented_reason = SPELL_FAILED_SILENCED;
        else if (unitflag & UNIT_FLAG_PACIFIED)
            prevented_reason = SPELL_FAILED_PACIFIED;
    }

    // Barkskin & Hex hotfix 4.3 patch http://eu.battle.net/wow/ru/blog/10037151
    if (m_spellInfo->Id == 22812 && m_caster->HasAura(51514))
        prevented_reason = SPELL_FAILED_PACIFIED;

    // Attr must make flag drop spell totally immune from all effects
    if (prevented_reason != SPELL_CAST_OK)
    {
        if (school_immune || mechanic_immune || dispel_immune)
        {
            //Checking auras is needed now, because you are prevented by some state but the spell grants immunity.
            Unit::AuraApplicationMap const& auras = m_caster->GetAppliedAuras();
            for (Unit::AuraApplicationMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
            {
                Aura const *aura = itr->second->GetBase();
                SpellInfo const* auraInfo = aura->GetSpellInfo();
                if (auraInfo->GetAllEffectsMechanicMask() & mechanic_immune)
                    continue;
                if (auraInfo->GetSchoolMask() & school_immune && !(auraInfo->AttributesEx & SPELL_ATTR1_UNAFFECTED_BY_SCHOOL_IMMUNE))
                    continue;
                if (auraInfo->GetDispelMask() & dispel_immune)
                    continue;

                //Make a second check for spell failed so the right SPELL_FAILED message is returned.
                //That is needed when your casting is prevented by multiple states and you are only immune to some of them.
                for (uint8 i = 0; i < aura->GetSpellInfo()->Effects.size(); ++i)
                {
                    if (AuraEffect *part = aura->GetEffect(i))
                    {
                        switch (part->GetAuraType())
                        {
                            case SPELL_AURA_MOD_STUN:
                                if (!usableInStun || !(auraInfo->GetAllEffectsMechanicMask() & (1<<MECHANIC_STUN)))
                                    return SPELL_FAILED_STUNNED;
                                break;
                            case SPELL_AURA_MOD_CONFUSE:
                                if (!(m_spellInfo->AttributesEx5 & SPELL_ATTR5_USABLE_WHILE_CONFUSED))
                                    return SPELL_FAILED_CONFUSED;
                                break;
                            case SPELL_AURA_MOD_FEAR:
                            case SPELL_AURA_MOD_FEAR_2:
                                if (!(m_spellInfo->AttributesEx5 & SPELL_ATTR5_USABLE_WHILE_FEARED))
                                    return SPELL_FAILED_FLEEING;
                                break;
                            case SPELL_AURA_MOD_SILENCE:
                            case SPELL_AURA_MOD_PACIFY:
                            case SPELL_AURA_MOD_PACIFY_SILENCE:
                                if (m_spellInfo->PreventionType == SPELL_PREVENTION_TYPE_PACIFY)
                                    return SPELL_FAILED_PACIFIED;
                                else if (m_spellInfo->PreventionType == SPELL_PREVENTION_TYPE_SILENCE)
                                    return SPELL_FAILED_SILENCED;
                                break;
                            default: break;
                        }
                    }
                }
            }
        }
        // You are prevented from casting and the spell casted does not grant immunity. Return a failed error.
        else
            return prevented_reason;
    }
    return SPELL_CAST_OK;
}

SpellCastResult Spell::CheckArenaAndRatedBattlegroundCastRules()
{
    bool isRatedBattleground = false; // NYI
    bool isArena = !isRatedBattleground;

    // check USABLE attributes
    // USABLE takes precedence over NOT_USABLE
    if (isRatedBattleground && m_spellInfo->AttributesEx9 & SPELL_ATTR9_USABLE_IN_RATED_BATTLEGROUNDS)
        return SPELL_CAST_OK;

    if (isArena && m_spellInfo->AttributesEx4 & SPELL_ATTR4_USABLE_IN_ARENA)
        return SPELL_CAST_OK;

    // check NOT_USABLE attributes
    if (m_spellInfo->AttributesEx4 & SPELL_ATTR4_NOT_USABLE_IN_ARENA_OR_RATED_BG)
        return isArena ? SPELL_FAILED_NOT_IN_ARENA : SPELL_FAILED_NOT_IN_RATED_BATTLEGROUND;

    if (isArena && m_spellInfo->AttributesEx9 & SPELL_ATTR9_NOT_USABLE_IN_ARENA)
            return SPELL_FAILED_NOT_IN_ARENA;

    // check cooldowns
    uint32 spellCooldown = m_spellInfo->GetRecoveryTime();
    if (isArena && spellCooldown > 10 * MINUTE * IN_MILLISECONDS) // not sure if still needed
        return SPELL_FAILED_NOT_IN_ARENA;

    if (isRatedBattleground && spellCooldown > 15 * MINUTE * IN_MILLISECONDS)
        return SPELL_FAILED_NOT_IN_RATED_BATTLEGROUND;

    return SPELL_CAST_OK;
}

bool Spell::CanAutoCast(Unit* target)
{
    uint64 targetguid = target->GetGUID();

    for (uint32 j = 0; j < m_spellInfo->Effects.size(); ++j)
    {
        if (m_spellInfo->Effects[j].Effect == SPELL_EFFECT_APPLY_AURA)
        {
            if (m_spellInfo->StackAmount <= 1)
            {
                if (target->HasAuraEffect(m_spellInfo->Id, j))
                    return false;
            }
            else
            {
                if (AuraEffect *aureff = target->GetAuraEffect(m_spellInfo->Id, j))
                    if (aureff->GetBase()->GetStackAmount() >= m_spellInfo->StackAmount)
                        return false;
            }
        }
        else if (m_spellInfo->Effects[j].IsAreaAuraEffect())
        {
            if (target->HasAuraEffect(m_spellInfo->Id, j))
                return false;
        }
    }

    SpellCastResult result = CallScriptCheckAutoCastHandlers();
    if (result != SPELL_CAST_OK)
        return false;

    result = CheckPetCast(target);

    if (result == SPELL_CAST_OK || result == SPELL_FAILED_UNIT_NOT_INFRONT)
    {
        SelectSpellTargets();
        //check if among target units, our WANTED target is as well (->only self cast spells return false)
        for (std::list<TargetInfo>::iterator ihit= m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
            if (ihit->targetGUID == targetguid)
                return true;
    }
    return false;                                           //target invalid
}

SpellCastResult Spell::CheckRange(bool strict)
{
    // Don't check for instant cast spells
    if (!strict && m_casttime == 0)
        return SPELL_CAST_OK;

    uint32 range_type = 0;

    if (m_spellInfo->RangeEntry)
    {
        // check needed by 68766 51693 - both spells are cast on enemies and have 0 max range
        // these are triggered by other spells - possibly we should omit range check in that case?
        if (m_spellInfo->RangeEntry->ID == 1)
            return SPELL_CAST_OK;

        range_type = m_spellInfo->RangeEntry->type;
    }

    Unit* target = m_targets.GetUnitTarget();
    float max_range = m_caster->GetSpellMaxRangeForTarget(target, m_spellInfo);
    float min_range = m_caster->GetSpellMinRangeForTarget(target, m_spellInfo);

    if (Player* modOwner = m_caster->GetSpellModOwner())
    {
        // Pet basic abilities must not apply mod-range from Blink Strikes when its on cooldown
        if ((m_spellInfo->Id != 16827 && m_spellInfo->Id != 17253 && m_spellInfo->Id != 49966 && m_spellInfo->Id != 145625) || !modOwner->HasSpellCooldown(130393))
            modOwner->ApplySpellMod(m_spellInfo->Id, SPELLMOD_RANGE, max_range, this);
    }

    if (target && target != m_caster)
    {
        if (range_type == SPELL_RANGE_MELEE)
        {
            // Because of lag, we can not check too strictly here.
            if (!m_caster->IsWithinMeleeRange(target, max_range))
                return !(_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_OUT_OF_RANGE : SPELL_FAILED_DONT_REPORT;
        }
        else if (!m_caster->IsWithinCombatRange(target, max_range))
            return !(_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_OUT_OF_RANGE : SPELL_FAILED_DONT_REPORT; //0x5A;

        if (range_type == SPELL_RANGE_RANGED)
        {
            if (m_caster->IsWithinMeleeRange(target))
                return !(_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_TOO_CLOSE : SPELL_FAILED_DONT_REPORT;
        }
        else if (min_range && m_caster->IsWithinCombatRange(target, min_range)) // skip this check if min_range = 0
            return !(_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_TOO_CLOSE : SPELL_FAILED_DONT_REPORT;

        if (m_caster->GetTypeId() == TYPEID_PLAYER &&
            (m_spellInfo->FacingCasterFlags & SPELL_FACING_FLAG_INFRONT) && !m_caster->HasInArc(static_cast<float>(M_PI), target))
            return !(_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_UNIT_NOT_INFRONT : SPELL_FAILED_DONT_REPORT;
    }
    else if (m_spellInfo->Id == 119996)
    {
        if (auto player = GetCaster()->ToPlayer())
            if (auto pet = player->GetPet())
                if (!m_caster->IsWithinDist3d(pet->GetPositionX(), pet->GetPositionY(), pet->GetPositionZ(), max_range))
                    return SPELL_FAILED_OUT_OF_RANGE;
    }

    if (m_targets.HasDst() && !m_targets.HasTraj())
    {
        if (!m_caster->IsWithinDist3d(m_targets.GetDstPos(), max_range))
            return SPELL_FAILED_OUT_OF_RANGE;
        if (min_range && m_caster->IsWithinDist3d(m_targets.GetDstPos(), min_range))
            return SPELL_FAILED_TOO_CLOSE;
    }

    return SPELL_CAST_OK;
}

SpellCastResult Spell::CheckPower()
{
    // item cast not used power
    if (m_CastItem)
        return SPELL_CAST_OK;

    // Dark Simulacrum case
    if (isStolen)
        return SPELL_CAST_OK;

    // health as power used - need check health amount
    if (m_spellPowerData->PowerType == POWER_HEALTH)
    {
        if (int32(m_caster->GetHealth()) <= m_powerCost)
            return SPELL_FAILED_CASTER_AURASTATE;
        return SPELL_CAST_OK;
    }
    // Check valid power type
    if (m_spellPowerData->PowerType >= MAX_POWERS)
        return SPELL_FAILED_UNKNOWN;

    //check rune cost only if a spell has PowerType == POWER_RUNES
    if (m_spellPowerData->PowerType == POWER_RUNES)
    {
        SpellCastResult failReason = CheckRuneCost(m_spellInfo->RuneCostID);
        if (failReason != SPELL_CAST_OK)
            return failReason;
    }

    // Check power amount
    Powers powerType = Powers(m_spellPowerData->PowerType);
    if (int32(m_caster->GetPower(powerType)) < m_powerCost)
        return SPELL_FAILED_NO_POWER;
    else
        return SPELL_CAST_OK;
}

SpellCastResult Spell::CheckItems()
{
    if (m_caster->GetTypeId() != TYPEID_PLAYER)
        return SPELL_CAST_OK;

    Player* p_caster = (Player*)m_caster;

    if (!m_CastItem)
    {
        if (m_castItemGUID)
            return SPELL_FAILED_ITEM_NOT_READY;
    }
    else
    {
        uint32 itemid = m_CastItem->GetEntry();
        if (!p_caster->HasItemCount(itemid))
            return SPELL_FAILED_ITEM_NOT_READY;

        ItemTemplate const* proto = m_CastItem->GetTemplate();
        if (!proto)
            return SPELL_FAILED_ITEM_NOT_READY;

        for (int i = 0; i < MAX_ITEM_SPELLS; ++i)
            if (proto->Spells[i].SpellCharges)
                if (m_CastItem->GetSpellCharges(i) == 0)
                    return SPELL_FAILED_NO_CHARGES_REMAIN;

        // consumable cast item checks
        if (proto->Class == ITEM_CLASS_CONSUMABLE && m_targets.GetUnitTarget())
        {
            // such items should only fail if there is no suitable effect at all - see Rejuvenation Potions for example
            SpellCastResult failReason = SPELL_CAST_OK;
            for (auto const &spellEffect : m_spellInfo->Effects)
            {
                // skip check, pet not required like checks, and for TARGET_UNIT_PET m_targets.GetUnitTarget() is not the real target but the caster
                if (spellEffect.TargetA.GetTarget() == TARGET_UNIT_PET)
                    continue;

                if (spellEffect.Effect == SPELL_EFFECT_HEAL)
                {
                    if (m_targets.GetUnitTarget()->IsFullHealth())
                    {
                        failReason = SPELL_FAILED_ALREADY_AT_FULL_HEALTH;
                        continue;
                    }
                    else
                    {
                        failReason = SPELL_CAST_OK;
                        break;
                    }
                }

                // Mana Potion, Rage Potion, Thistle Tea(Rogue), ...
                if (spellEffect.Effect == SPELL_EFFECT_ENERGIZE)
                {
                    if (spellEffect.MiscValue < 0 || spellEffect.MiscValue >= int8(MAX_POWERS))
                    {
                        failReason = SPELL_FAILED_ALREADY_AT_FULL_POWER;
                        continue;
                    }

                    Powers power = Powers(spellEffect.MiscValue);
                    if (m_targets.GetUnitTarget()->GetPower(power) == m_targets.GetUnitTarget()->GetMaxPower(power))
                    {
                        failReason = SPELL_FAILED_ALREADY_AT_FULL_POWER;
                        continue;
                    }
                    else
                    {
                        failReason = SPELL_CAST_OK;
                        break;
                    }
                }
            }
            if (failReason != SPELL_CAST_OK)
                return failReason;
        }
    }

    // check target item
    if (m_targets.GetItemTargetGUID())
    {
        if (m_caster->GetTypeId() != TYPEID_PLAYER)
            return SPELL_FAILED_BAD_TARGETS;

        if (!m_targets.GetItemTarget())
            return SPELL_FAILED_ITEM_GONE;

        if (!m_targets.GetItemTarget()->IsFitToSpellRequirements(m_spellInfo))
            return SPELL_FAILED_EQUIPPED_ITEM_CLASS;
    }
    // if not item target then required item must be equipped
    else
    {
        if (m_caster->GetTypeId() == TYPEID_PLAYER && !m_caster->ToPlayer()->HasItemFitToSpellRequirements(m_spellInfo))
            return SPELL_FAILED_EQUIPPED_ITEM_CLASS;
    }

    // check spell focus object
    if (m_spellInfo->RequiresSpellFocus)
    {
        CellCoord p(Trinity::ComputeCellCoord(m_caster->GetPositionX(), m_caster->GetPositionY()));
        Cell cell(p);

        GameObject* ok = NULL;
        Trinity::GameObjectFocusCheck go_check(m_caster, m_spellInfo->RequiresSpellFocus);
        Trinity::GameObjectSearcher<Trinity::GameObjectFocusCheck> checker(m_caster, ok, go_check);

        cell.Visit(p, Trinity::makeGridVisitor(checker), *m_caster->GetMap(), *m_caster, m_caster->GetVisibilityRange());

        if (!ok)
            return SPELL_FAILED_REQUIRES_SPELL_FOCUS;

        focusObject = ok;                                   // game object found in range
    }

    // do not take reagents for these item casts
    if (!(m_CastItem && m_CastItem->GetTemplate()->Flags & ITEM_PROTO_FLAG_TRIGGERED_CAST))
    {
        bool checkReagents = !(_triggeredCastFlags & TRIGGERED_IGNORE_POWER_AND_REAGENT_COST) && !p_caster->CanNoReagentCast(m_spellInfo);
        // Not own traded item (in trader trade slot) requires reagents even if triggered spell
        if (!checkReagents)
            if (Item* targetItem = m_targets.GetItemTarget())
                if (targetItem->GetOwnerGUID() != m_caster->GetGUID())
                    checkReagents = true;

        // check reagents (ignore triggered spells with reagents processed by original spell) and special reagent ignore case.
        if (checkReagents)
        {
            for (uint32 i = 0; i < MAX_SPELL_REAGENTS; i++)
            {
                if (m_spellInfo->Reagent[i] <= 0)
                    continue;

                uint32 itemid    = m_spellInfo->Reagent[i];
                uint32 itemcount = m_spellInfo->ReagentCount[i];

                // if CastItem is also spell reagent
                if (m_CastItem && m_CastItem->GetEntry() == itemid)
                {
                    ItemTemplate const* proto = m_CastItem->GetTemplate();
                    if (!proto)
                        return SPELL_FAILED_ITEM_NOT_READY;
                    for (int s=0; s < MAX_ITEM_PROTO_SPELLS; ++s)
                    {
                        // CastItem will be used up and does not count as reagent
                        int32 charges = m_CastItem->GetSpellCharges(s);
                        if (proto->Spells[s].SpellCharges < 0 && abs(charges) < 2)
                        {
                            ++itemcount;
                            break;
                        }
                    }
                }
                if (!p_caster->HasItemCount(itemid, itemcount))
                    return SPELL_FAILED_REAGENTS;
            }
        }

        // check totem-item requirements (items presence in inventory)
        uint32 totems = 2;
        for (int i = 0; i < 2; ++i)
        {
            if (m_spellInfo->Totem[i] != 0)
            {
                if (p_caster->HasItemCount(m_spellInfo->Totem[i]))
                {
                    totems -= 1;
                    continue;
                }
            }
            else
            totems -= 1;
        }
        if (totems != 0)
            return SPELL_FAILED_TOTEMS;
    }

    // special checks for spell effects
    for (auto const &spellEffect : m_spellInfo->Effects)
    {
        switch (spellEffect.Effect)
        {
            case SPELL_EFFECT_CREATE_ITEM:
            case SPELL_EFFECT_CREATE_ITEM_2:
            {
                if (!IsTriggered() && spellEffect.ItemType)
                {
                    ItemPosCountVec dest;
                    InventoryResult msg = p_caster->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, spellEffect.ItemType, 1);
                    if (msg != EQUIP_ERR_OK)
                    {
                        ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(spellEffect.ItemType);
                        // TODO: Needs review
                        if (pProto && !pProto->ItemLimitCategory)
                        {
                            p_caster->SendEquipError(msg, NULL, NULL, spellEffect.ItemType);
                            return SPELL_FAILED_DONT_REPORT;
                        }
                        else
                        {
                            if (m_spellInfo->Id != 759) // Conjure Mana Gem
                                return SPELL_FAILED_TOO_MANY_OF_ITEM;
                            else if (!p_caster->HasItemCount(spellEffect.ItemType))
                                return SPELL_FAILED_TOO_MANY_OF_ITEM;
                            else
                                p_caster->CastSpell(m_caster, m_spellInfo->Effects[EFFECT_1].CalcValue(), false); // move this to anywhere
                            return SPELL_FAILED_DONT_REPORT;
                        }
                    }
                }
                break;
            }
            case SPELL_EFFECT_ENCHANT_ITEM:
                if (spellEffect.ItemType && m_targets.GetItemTarget()
                    && (m_targets.GetItemTarget()->IsVellum()))
                {
                    // cannot enchant vellum for other player
                    if (m_targets.GetItemTarget()->GetOwner() != m_caster)
                        return SPELL_FAILED_NOT_TRADEABLE;
                    // do not allow to enchant vellum from scroll made by vellum-prevent exploit
                    if (m_CastItem && m_CastItem->GetTemplate()->Flags & ITEM_PROTO_FLAG_TRIGGERED_CAST)
                        return SPELL_FAILED_TOTEM_CATEGORY;
                    ItemPosCountVec dest;
                    InventoryResult msg = p_caster->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, spellEffect.ItemType, 1);
                    if (msg != EQUIP_ERR_OK)
                    {
                        p_caster->SendEquipError(msg, NULL, NULL, spellEffect.ItemType);
                        return SPELL_FAILED_DONT_REPORT;
                    }
                }
            case SPELL_EFFECT_ENCHANT_ITEM_PRISMATIC:
            {
                Item* targetItem = m_targets.GetItemTarget();
                if (!targetItem)
                    return SPELL_FAILED_ITEM_NOT_FOUND;

                if (targetItem->GetTemplate()->ItemLevel < m_spellInfo->BaseLevel)
                    return SPELL_FAILED_LOWLEVEL;

                bool isItemUsable = false;
                for (uint8 e = 0; e < MAX_ITEM_PROTO_SPELLS; ++e)
                {
                    ItemTemplate const* proto = targetItem->GetTemplate();
                    if (proto->Spells[e].SpellId && (
                        proto->Spells[e].SpellTrigger == ITEM_SPELLTRIGGER_ON_USE ||
                        proto->Spells[e].SpellTrigger == ITEM_SPELLTRIGGER_ON_NO_DELAY_USE))
                    {
                        isItemUsable = true;
                        break;
                    }
                }

                SpellItemEnchantmentEntry const* pEnchant = sSpellItemEnchantmentStore.LookupEntry(spellEffect.MiscValue);
                // do not allow adding usable enchantments to items that have use effect already
                if (pEnchant && isItemUsable)
                    for (uint8 s = 0; s < MAX_ITEM_ENCHANTMENT_EFFECTS; ++s)
                        if (pEnchant->type[s] == ITEM_ENCHANTMENT_TYPE_USE_SPELL)
                            return SPELL_FAILED_ON_USE_ENCHANT;

                // Not allow enchant in trade slot for some enchant type
                if (targetItem->GetOwner() != m_caster)
                {
                    if (!pEnchant)
                        return SPELL_FAILED_ERROR;
                    if (pEnchant->slot & ENCHANTMENT_CAN_SOULBOUND)
                        return SPELL_FAILED_NOT_TRADEABLE;
                }
                break;
            }
            case SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY:
            {
                Item* item = m_targets.GetItemTarget();
                if (!item)
                    return SPELL_FAILED_ITEM_NOT_FOUND;
                // Not allow enchant in trade slot for some enchant type
                if (item->GetOwner() != m_caster)
                {
                    uint32 enchant_id = spellEffect.MiscValue;
                    SpellItemEnchantmentEntry const* pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
                    if (!pEnchant)
                        return SPELL_FAILED_ERROR;
                    if (pEnchant->slot & ENCHANTMENT_CAN_SOULBOUND)
                        return SPELL_FAILED_NOT_TRADEABLE;
                }
                break;
            }
            case SPELL_EFFECT_ENCHANT_HELD_ITEM:
                // check item existence in effect code (not output errors at offhand hold item effect to main hand for example
                break;
            case SPELL_EFFECT_DISENCHANT:
            {
                if (!m_targets.GetItemTarget())
                    return SPELL_FAILED_CANT_BE_DISENCHANTED;

                // prevent disenchanting in trade slot
                if (m_targets.GetItemTarget()->GetOwnerGUID() != m_caster->GetGUID())
                    return SPELL_FAILED_CANT_BE_DISENCHANTED;

                ItemTemplate const* itemProto = m_targets.GetItemTarget()->GetTemplate();
                if (!itemProto)
                    return SPELL_FAILED_CANT_BE_DISENCHANTED;

                uint32 item_quality = itemProto->Quality;
                // 2.0.x addon: Check player enchanting level against the item disenchanting requirements
                uint32 item_disenchantskilllevel = itemProto->RequiredDisenchantSkill;
                if (item_disenchantskilllevel == uint32(-1))
                    return SPELL_FAILED_CANT_BE_DISENCHANTED;
                if (item_disenchantskilllevel > p_caster->GetSkillValue(SKILL_ENCHANTING))
                    return SPELL_FAILED_LOW_CASTLEVEL;
                if (item_quality > 4 || item_quality < 2)
                    return SPELL_FAILED_CANT_BE_DISENCHANTED;
                if (itemProto->Class != ITEM_CLASS_WEAPON && itemProto->Class != ITEM_CLASS_ARMOR)
                    return SPELL_FAILED_CANT_BE_DISENCHANTED;
                if (!itemProto->DisenchantID)
                    return SPELL_FAILED_CANT_BE_DISENCHANTED;
                break;
            }
            case SPELL_EFFECT_PROSPECTING:
            {
                if (!m_targets.GetItemTarget())
                    return SPELL_FAILED_CANT_BE_PROSPECTED;
                //ensure item is a prospectable ore
                if (!(m_targets.GetItemTarget()->GetTemplate()->Flags & ITEM_PROTO_FLAG_PROSPECTABLE))
                    return SPELL_FAILED_CANT_BE_PROSPECTED;
                //prevent prospecting in trade slot
                if (m_targets.GetItemTarget()->GetOwnerGUID() != m_caster->GetGUID())
                    return SPELL_FAILED_CANT_BE_PROSPECTED;
                //Check for enough skill in jewelcrafting
                uint32 item_prospectingskilllevel = m_targets.GetItemTarget()->GetTemplate()->RequiredSkillRank;
                if (item_prospectingskilllevel >p_caster->GetSkillValue(SKILL_JEWELCRAFTING))
                    return SPELL_FAILED_LOW_CASTLEVEL;
                //make sure the player has the required ores in inventory
                if (m_targets.GetItemTarget()->GetCount() < 5)
                    return SPELL_FAILED_NEED_MORE_ITEMS;

                if (!LootTemplates_Prospecting.HaveLootFor(m_targets.GetItemTargetEntry()))
                    return SPELL_FAILED_CANT_BE_PROSPECTED;

                break;
            }
            case SPELL_EFFECT_MILLING:
            {
                if (!m_targets.GetItemTarget())
                    return SPELL_FAILED_CANT_BE_MILLED;
                //ensure item is a millable herb
                if (!(m_targets.GetItemTarget()->GetTemplate()->Flags & ITEM_PROTO_FLAG_MILLABLE))
                    return SPELL_FAILED_CANT_BE_MILLED;
                //prevent milling in trade slot
                if (m_targets.GetItemTarget()->GetOwnerGUID() != m_caster->GetGUID())
                    return SPELL_FAILED_CANT_BE_MILLED;
                //Check for enough skill in inscription
                uint32 item_millingskilllevel = m_targets.GetItemTarget()->GetTemplate()->RequiredSkillRank;
                if (item_millingskilllevel >p_caster->GetSkillValue(SKILL_INSCRIPTION))
                    return SPELL_FAILED_LOW_CASTLEVEL;
                //make sure the player has the required herbs in inventory
                if (m_targets.GetItemTarget()->GetCount() < 5)
                    return SPELL_FAILED_NEED_MORE_ITEMS;

                if (!LootTemplates_Milling.HaveLootFor(m_targets.GetItemTargetEntry()))
                    return SPELL_FAILED_CANT_BE_MILLED;

                break;
            }
            case SPELL_EFFECT_WEAPON_DAMAGE:
            case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
            {
                if (m_caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_TARGET_NOT_PLAYER;

                if (m_attackType != RANGED_ATTACK)
                    break;

                Item* pItem = m_caster->ToPlayer()->GetWeaponForAttack(m_attackType);
                if (!pItem || pItem->IsBroken())
                    return SPELL_FAILED_EQUIPPED_ITEM;

                switch (pItem->GetTemplate()->SubClass)
                {
                    case ITEM_SUBCLASS_WEAPON_THROWN:
                    {
                        uint32 ammo = pItem->GetEntry();
                        if (!m_caster->ToPlayer()->HasItemCount(ammo))
                            return SPELL_FAILED_NO_AMMO;
                    };
                    break;
                    case ITEM_SUBCLASS_WEAPON_GUN:
                    case ITEM_SUBCLASS_WEAPON_BOW:
                    case ITEM_SUBCLASS_WEAPON_CROSSBOW:
                    case ITEM_SUBCLASS_WEAPON_WAND:
                        break;
                    default:
                        break;
                }
                break;
            }
            case SPELL_EFFECT_CREATE_MANA_GEM:
            {
                uint32 item_id = spellEffect.ItemType;
                ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(item_id);

                if (!pProto)
                    return SPELL_FAILED_ITEM_AT_MAX_CHARGES;

                if (Item* pitem = p_caster->GetItemByEntry(item_id))
                {
                    for (int x = 0; x < MAX_ITEM_PROTO_SPELLS; ++x)
                        if (pProto->Spells[x].SpellCharges != 0 && pitem->GetSpellCharges(x) == pProto->Spells[x].SpellCharges)
                            return SPELL_FAILED_ITEM_AT_MAX_CHARGES;
                }
                break;
            }
            default:
                break;
        }
    }

    // check weapon presence in slots for main/offhand weapons
    if (m_spellInfo->EquippedItemClass >= 0)
    {
        // main hand weapon required
        if (m_spellInfo->AttributesEx3 & SPELL_ATTR3_MAIN_HAND)
        {
            Item* item = m_caster->ToPlayer()->GetWeaponForAttack(BASE_ATTACK);

            // skip spell if no weapon in slot or broken
            if (!item || item->IsBroken())
                return (_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_DONT_REPORT : SPELL_FAILED_EQUIPPED_ITEM_CLASS;

            // skip spell if weapon not fit to triggered spell
            if (!item->IsFitToSpellRequirements(m_spellInfo))
                return (_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_DONT_REPORT : SPELL_FAILED_EQUIPPED_ITEM_CLASS;
        }

        // offhand hand weapon required
        if (m_spellInfo->AttributesEx3 & SPELL_ATTR3_REQ_OFFHAND)
        {
            Item* item = m_caster->ToPlayer()->GetWeaponForAttack(OFF_ATTACK);

            // skip spell if no weapon in slot or broken
            if (!item || item->IsBroken())
                return (_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_DONT_REPORT : SPELL_FAILED_EQUIPPED_ITEM_CLASS;

            // skip spell if weapon not fit to triggered spell
            if (!item->IsFitToSpellRequirements(m_spellInfo))
                return (_triggeredCastFlags & TRIGGERED_DONT_REPORT_CAST_ERROR) ? SPELL_FAILED_DONT_REPORT : SPELL_FAILED_EQUIPPED_ITEM_CLASS;
        }
    }

    return SPELL_CAST_OK;
}

// Called only in Unit::DealDamage
void Spell::Delayed()
{
    // Spell is active and can't be time-backed
    if (!m_caster)
        return;

    // Spells may only be delayed twice
    if (isDelayableNoMore())
        return;

    //check pushback reduce
    int32 delaytime = 500;                                  // spellcasting delay is normally 500ms
    int32 delayReduce = 100;                                // must be initialized to 100 for percent modifiers
    m_caster->ToPlayer()->ApplySpellMod(m_spellInfo->Id, SPELLMOD_NOT_LOSE_CASTING_TIME, delayReduce, this);
    delayReduce += m_caster->GetTotalAuraModifier(SPELL_AURA_REDUCE_PUSHBACK) - 100;
    if (delayReduce >= 100)
        return;

    AddPct(delaytime, -delayReduce);

    if (m_timer + delaytime > m_casttime)
    {
        delaytime = m_casttime - m_timer;
        m_timer = m_casttime;
    }
    else
        m_timer += delaytime;

    ObjectGuid casterGuid = m_caster->GetGUID();

    WorldPacket data(SMSG_SPELL_DELAYED, 8+4);
    data.WriteBitSeq<4, 6, 3, 7, 2, 5, 0, 1>(casterGuid);
    data.WriteByteSeq<7, 0, 1, 4, 6, 2, 5>(casterGuid);
    data << uint32(delaytime);
    data.WriteByteSeq<3>(casterGuid);

    m_caster->SendMessageToSet(&data, true);
}

void Spell::DelayedChannel()
{
    if (!m_caster || m_caster->GetTypeId() != TYPEID_PLAYER || getState() != SPELL_STATE_CASTING)
        return;

    if (isDelayableNoMore())                                    // Spells may only be delayed twice
        return;

    //check pushback reduce
    int32 delaytime = CalculatePct(m_spellInfo->GetDuration(), 25); // channeling delay is normally 25% of its time per hit
    int32 delayReduce = 100;                                    // must be initialized to 100 for percent modifiers
    m_caster->ToPlayer()->ApplySpellMod(m_spellInfo->Id, SPELLMOD_NOT_LOSE_CASTING_TIME, delayReduce, this);
    delayReduce += m_caster->GetTotalAuraModifier(SPELL_AURA_REDUCE_PUSHBACK) - 100;
    if (delayReduce >= 100)
        return;

    AddPct(delaytime, -delayReduce);

    if (m_timer <= delaytime)
    {
        delaytime = m_timer;
        m_timer = 0;
    }
    else
        m_timer -= delaytime;

    for (std::list<TargetInfo>::const_iterator ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
        if ((*ihit).missCondition == SPELL_MISS_NONE)
            if (Unit* unit = (m_caster->GetGUID() == ihit->targetGUID) ? m_caster : ObjectAccessor::GetUnit(*m_caster, ihit->targetGUID))
                unit->DelayOwnedAuras(m_spellInfo->Id, m_originalCasterGUID, delaytime);

    // partially interrupt persistent area auras
    if (DynamicObject* dynObj = m_caster->GetDynObject(m_spellInfo->Id))
        dynObj->Delay(delaytime);

    SendChannelUpdate(m_timer);
}

void Spell::UpdatePointers()
{
    if (m_originalCasterGUID == m_caster->GetGUID())
        m_originalCaster = m_caster;
    else
    {
        m_originalCaster = ObjectAccessor::GetUnit(*m_caster, m_originalCasterGUID);
        if (m_originalCaster && !m_originalCaster->IsInWorld())
            m_originalCaster = NULL;
    }

    if (m_castItemGUID && m_caster->GetTypeId() == TYPEID_PLAYER)
        m_CastItem = m_caster->ToPlayer()->GetItemByGuid(m_castItemGUID);

    m_targets.Update(m_caster);

    // further actions done only for dest targets
    if (!m_targets.HasDst())
        return;

    // cache last transport
    WorldObject* transport = NULL;

    // update effect destinations (in case of moved transport dest target)
    for (uint8 effIndex = 0; effIndex < GetSpellInfo()->Effects.size(); ++effIndex)
    {
        SpellDestination& dest = m_destTargets[effIndex];
        if (!dest._transportGUID)
            continue;

        if (!transport || transport->GetGUID() != dest._transportGUID)
            transport = ObjectAccessor::GetWorldObject(*m_caster, dest._transportGUID);

        if (transport)
        {
            dest._position.Relocate(transport);
            dest._position.RelocateOffset(dest._transportOffset);
        }
    }
}

CurrentSpellTypes Spell::GetCurrentContainer() const
{
    if (IsNextMeleeSwingSpell())
        return(CURRENT_MELEE_SPELL);
    else if (IsAutoRepeat())
        return(CURRENT_AUTOREPEAT_SPELL);
    else if (m_spellInfo->IsChanneled())
        return(CURRENT_CHANNELED_SPELL);
    else
        return(CURRENT_GENERIC_SPELL);
}

bool Spell::CheckEffectTarget(Unit const* target, uint32 eff) const
{
    switch (m_spellInfo->Effects[eff].ApplyAuraName)
    {
        case SPELL_AURA_MOD_POSSESS:
        case SPELL_AURA_MOD_CHARM:
        case SPELL_AURA_AOE_CHARM:
            if (target->GetTypeId() == TYPEID_UNIT && target->IsVehicle())
                return false;
            if (target->IsMounted())
                return false;
            if (target->GetCharmerGUID())
                return false;
            if (int32 damage = CalculateDamage(eff, target))
                if ((int32)target->getLevel() > damage)
                    return false;
            break;
        default:
            break;
    }

    // Hour of Twilight
    if (m_spellInfo->Id == 103327)
    {
        // Deterrence
        if (target->HasAura(19263))
            return false;
    }

    if (!m_spellInfo->IsNeedAdditionalLosChecks() && (IsTriggered() || m_spellInfo->AttributesEx2 & SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS) || DisableMgr::IsDisabledFor(DISABLE_TYPE_SPELL, m_spellInfo->Id, NULL, SPELL_DISABLE_LOS))
        return true;

    // Hack fix for Ice Tombs (Sindragosa encounter)
    if (target->GetTypeId() == TYPEID_UNIT)
        if (target->GetEntry() == 36980 || target->GetEntry() == 38320 || target->GetEntry() == 38321 || target->GetEntry() == 38322)
            return true;

    // todo: shit below shouldn't be here, but it's temporary
    //Check targets for LOS visibility (except spells without range limitations)
    switch (m_spellInfo->Effects[eff].Effect)
    {
        case SPELL_EFFECT_RESURRECT_NEW:
            // player far away, maybe his corpse near?
            if (target != m_caster && !target->IsWithinLOSInMap(m_caster))
            {
                if (!m_targets.GetCorpseTargetGUID())
                    return false;

                Corpse* corpse = ObjectAccessor::GetCorpse(*m_caster, m_targets.GetCorpseTargetGUID());
                if (!corpse)
                    return false;

                if (target->GetGUID() != corpse->GetOwnerGUID())
                    return false;

                if (!corpse->IsWithinLOSInMap(m_caster))
                    return false;
            }

            // all ok by some way or another, skip normal check
            break;
        default:                                            // normal case
            // Get GO cast coordinates if original caster -> GO
            WorldObject* caster = NULL;
            if (IS_GAMEOBJECT_GUID(m_originalCasterGUID))
                caster = m_caster->GetMap()->GetGameObject(m_originalCasterGUID);
            if (!caster)
                caster = m_caster;

            // Sin and Punishment from duel bug
            if (m_spellInfo->Id == 87204)
            {
                if (caster != target && caster->GetTypeId() == TYPEID_PLAYER && caster->ToPlayer()->duel && target->GetTypeId() == TYPEID_PLAYER)
                    return false;
            }
            // Glyph of Concussive Shot
            else if (m_spellInfo->Id == 5116)
            {
                if (eff == EFFECT_1 && !m_caster->HasAura(56851))
                    return false;
            }

            if (target->GetEntry() == 5925)
                return true;
            if (LOSAdditionalRules(target))
                return true;

            if (m_targets.HasDst())
            {
                float x, y, z;
                m_targets.GetDstPos()->GetPosition(x, y, z);

                if (!target->IsWithinLOS(x, y, z))
                    return false;
            }
            else if (target != m_caster && !target->IsWithinLOSInMap(caster))
                return false;
            break;
    }

    return true;
}

bool Spell::IsNextMeleeSwingSpell() const
{
    return m_spellInfo->Attributes & SPELL_ATTR0_ON_NEXT_SWING;
}

bool Spell::IsAutoActionResetSpell() const
{
    // TODO: changed SPELL_INTERRUPT_FLAG_AUTOATTACK -> SPELL_INTERRUPT_FLAG_INTERRUPT to fix compile - is this check correct at all?
    return !IsTriggered() && (m_spellInfo->InterruptFlags & SPELL_INTERRUPT_FLAG_INTERRUPT);
}

bool Spell::IsNeedSendToClient() const
{
    // This switch is here because the spell included has SPELL_ATTR4_TRIGGERED which causes it not to be sent to client
    // It should however be sent, since changing which spells are sent to client could have client crash like consequences I added this exception instead
    switch (m_spellInfo->Id)
    {
        case 120465: // Trigger Summon Demon Cooldown
            return true;
        default:
            break;
    }

    return m_spellInfo->SpellVisual[0] || m_spellInfo->SpellVisual[1] || m_spellInfo->IsChanneled() ||
        (m_spellInfo->AttributesEx8 & SPELL_ATTR8_AURA_SEND_AMOUNT) || m_spellInfo->Speed > 0.0f || (!m_triggeredByAuraSpell && !IsTriggered());
}

bool Spell::HaveTargetsForEffect(uint8 effect) const
{
    for (std::list<TargetInfo>::const_iterator itr = m_UniqueTargetInfo.begin(); itr != m_UniqueTargetInfo.end(); ++itr)
        if (itr->effectMask & (1 << effect))
            return true;

    for (std::list<GOTargetInfo>::const_iterator itr = m_UniqueGOTargetInfo.begin(); itr != m_UniqueGOTargetInfo.end(); ++itr)
        if (itr->effectMask & (1 << effect))
            return true;

    for (std::list<ItemTargetInfo>::const_iterator itr = m_UniqueItemInfo.begin(); itr != m_UniqueItemInfo.end(); ++itr)
        if (itr->effectMask & (1 << effect))
            return true;

    return false;
}

SpellEvent::SpellEvent(Spell* spell) : BasicEvent()
{
    m_Spell = spell;
}

SpellEvent::~SpellEvent()
{
    if (m_Spell->getState() != SPELL_STATE_FINISHED)
        m_Spell->cancel();

    if (m_Spell->IsDeletable())
    {
        delete m_Spell;
    }
    else
    {
        TC_LOG_ERROR("spells", "~SpellEvent: %s %u tried to delete non-deletable spell %u. Was not deleted, causes memory leak.",
            (m_Spell->GetCaster()->GetTypeId() == TYPEID_PLAYER ? "Player" : "Creature"), m_Spell->GetCaster()->GetGUIDLow(), m_Spell->m_spellInfo->Id);
        ASSERT(false);
    }
}

bool SpellEvent::Execute(uint64 e_time, uint32 p_time)
{
    // update spell if it is not finished
    if (m_Spell->getState() != SPELL_STATE_FINISHED)
        m_Spell->update(p_time);

    // check spell state to process
    switch (m_Spell->getState())
    {
        case SPELL_STATE_FINISHED:
        {
            // spell was finished, check deletable state
            if (m_Spell->IsDeletable())
            {
                // check, if we do have unfinished triggered spells
                return true;                                // spell is deletable, finish event
            }
            // event will be re-added automatically at the end of routine)
        } break;

        case SPELL_STATE_DELAYED:
        {
            // first, check, if we have just started
            if (m_Spell->GetDelayStart() != 0)
            {
                // no, we aren't, do the typical update
                // check, if we have channeled spell on our hands
                /*
                if (m_Spell->m_spellInfo->IsChanneled())
                {
                    // evented channeled spell is processed separately, casted once after delay, and not destroyed till finish
                    // check, if we have casting anything else except this channeled spell and autorepeat
                    if (m_Spell->GetCaster()->IsNonMeleeSpellCasted(false, true, true))
                    {
                        // another non-melee non-delayed spell is casted now, abort
                        m_Spell->cancel();
                    }
                    else
                    {
                        // Set last not triggered spell for apply spellmods
                        ((Player*)m_Spell->GetCaster())->SetSpellModTakingSpell(m_Spell, true);
                        // do the action (pass spell to channeling state)
                        m_Spell->handle_immediate();

                        // And remove after effect handling
                        ((Player*)m_Spell->GetCaster())->SetSpellModTakingSpell(m_Spell, false);
                    }
                    // event will be re-added automatically at the end of routine)
                }
                else
                */
                {
                    // run the spell handler and think about what we can do next
                    uint64 t_offset = e_time - m_Spell->GetDelayStart();
                    uint64 n_offset = m_Spell->handle_delayed(t_offset);
                    if (n_offset)
                    {
                        // re-add us to the queue
                        m_Spell->GetCaster()->m_Events.AddEvent(this, m_Spell->GetDelayStart() + n_offset, false);
                        return false;                       // event not complete
                    }
                    // event complete
                    // finish update event will be re-added automatically at the end of routine)
                }
            }
            else
            {
                // delaying had just started, record the moment
                m_Spell->SetDelayStart(e_time);
                // re-plan the event for the delay moment
                m_Spell->GetCaster()->m_Events.AddEvent(this, e_time + m_Spell->GetDelayMoment(), false);
                return false;                               // event not complete
            }
        } break;

        default:
        {
            // all other states
            // event will be re-added automatically at the end of routine)
        } break;
    }

    // spell processing not complete, plan event on the next update interval
    m_Spell->GetCaster()->m_Events.AddEvent(this, e_time + 1, false);
    return false;                                           // event not complete
}

void SpellEvent::Abort(uint64 /*e_time*/)
{
    // oops, the spell we try to do is aborted
    if (m_Spell->getState() != SPELL_STATE_FINISHED)
        m_Spell->cancel();
}

bool SpellEvent::IsDeletable() const
{
    return m_Spell->IsDeletable();
}

bool Spell::IsValidDeadOrAliveTarget(Unit const* target) const
{
    if (target->IsAlive())
        return !m_spellInfo->IsRequiringDeadTarget();
    if (m_spellInfo->IsAllowingDeadTarget())
        return true;
    return false;
}

void Spell::HandleLaunchPhase()
{
    // handle effects with SPELL_EFFECT_HANDLE_LAUNCH mode
    for (uint32 i = 0; i < m_spellInfo->Effects.size(); ++i)
    {
        // don't do anything for empty effect
        if (m_spellInfo->Effects[i].IsEffect())
            HandleEffects(NULL, NULL, NULL, i, SPELL_EFFECT_HANDLE_LAUNCH);
    }

    float multiplier[MAX_SPELL_EFFECTS];
    for (uint8 i = 0; i < m_spellInfo->Effects.size(); ++i)
        if (m_applyMultiplierMask & (1 << i))
            multiplier[i] = m_spellInfo->Effects[i].CalcDamageMultiplier(m_originalCaster, this);

    bool usesAmmo = m_spellInfo->AttributesCu & SPELL_ATTR0_CU_DIRECT_DAMAGE;

    for (auto ihit = m_UniqueTargetInfo.begin(); ihit != m_UniqueTargetInfo.end(); ++ihit)
    {
        TargetInfo& target = *ihit;

        uint32 mask = target.effectMask;
        if (!mask)
            continue;

        // do not consume ammo anymore for Hunter's volley spell
        if (IsTriggered() && m_spellInfo->SpellFamilyName == SPELLFAMILY_HUNTER && m_spellInfo->IsTargetingArea())
            usesAmmo = false;

        if (usesAmmo)
        {
            bool ammoTaken = false;
            for (uint8 i = 0; i < m_spellInfo->Effects.size(); i++)
            {
                if (!(mask & 1 << i))
                    continue;
                switch (m_spellInfo->Effects[i].Effect)
                {
                    case SPELL_EFFECT_SCHOOL_DAMAGE:
                    case SPELL_EFFECT_WEAPON_DAMAGE:
                    case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
                    case SPELL_EFFECT_NORMALIZED_WEAPON_DMG:
                    case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
                        ammoTaken = true;
                        TakeAmmo();
                        break;
                }
                if (ammoTaken)
                    break;
            }
        }
        DoAllEffectOnLaunchTarget(target, multiplier);
    }
}

void Spell::DoAllEffectOnLaunchTarget(TargetInfo& targetInfo, float* multiplier)
{
    Unit* unit = NULL;
    // In case spell hit target, do all effect on that target
    if (targetInfo.missCondition == SPELL_MISS_NONE)
        unit = m_caster->GetGUID() == targetInfo.targetGUID ? m_caster : ObjectAccessor::GetUnit(*m_caster, targetInfo.targetGUID);
    // In case spell reflect from target, do all effect on caster (if hit)
    else if (targetInfo.missCondition == SPELL_MISS_REFLECT && targetInfo.reflectResult == SPELL_MISS_NONE)
        unit = m_caster;
    if (!unit)
        return;

    for (uint32 i = 0; i < m_spellInfo->Effects.size(); ++i)
    {
        if (targetInfo.effectMask & (1<<i))
        {
            m_damage = 0;
            m_healing = 0;

            HandleEffects(unit, NULL, NULL, i, SPELL_EFFECT_HANDLE_LAUNCH_TARGET);

            if (m_damage > 0)
            {
                if (m_spellInfo->Effects[i].IsTargetingArea())
                {
                    if (m_caster->GetTypeId() == TYPEID_PLAYER)
                    {
                        uint32 targetAmount = m_UniqueTargetInfo.size();
                        if (targetAmount > 10)
                            m_damage = m_damage * 10/targetAmount;

                        // Hack Fix Frost Bomb : Doesn't add AoE damage to main target
                        if (m_spellInfo->Id == 113092)
                            if (targetInfo.targetGUID == (*m_UniqueTargetInfo.begin()).targetGUID)
                                continue;
                    }
                }
            }
            else if (m_damage < 0)
            {
                switch (m_spellInfo->Id)
                {
                    case 73921: // Healing Rain
                    case 81280: // Blood Burst
                    case 88686: // Holy Word : Sanctuary
                    {
                        if (m_caster->GetTypeId() == TYPEID_PLAYER)
                        {
                            uint32 targetAmount = m_UniqueTargetInfo.size();
                            if (targetAmount > 6)
                                m_damage = (m_damage * 6) / int32(targetAmount);
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            if (m_applyMultiplierMask & (1 << i))
            {
                m_damage = int32(m_damage * m_damageMultipliers[i]);
                m_damageMultipliers[i] *= multiplier[i];
            }
            targetInfo.damage += m_damage;
        }
    }

    targetInfo.crit = m_caster->isSpellCrit(unit, m_spellInfo, m_spellSchoolMask, m_attackType);
}

SpellCastResult Spell::CanOpenLock(uint32 effIndex, uint32 lockId, SkillType& skillId, int32& reqSkillValue, int32& skillValue)
{
    if (!lockId)                                             // possible case for GO and maybe for items.
        return SPELL_CAST_OK;

    // Get LockInfo
    LockEntry const* lockInfo = sLockStore.LookupEntry(lockId);

    if (!lockInfo)
        return SPELL_FAILED_BAD_TARGETS;

    bool reqKey = false;                                    // some locks not have reqs

    for (int j = 0; j < MAX_LOCK_CASE; ++j)
    {
        switch (lockInfo->Type[j])
        {
            // check key item (many fit cases can be)
            case LOCK_KEY_ITEM:
                if (lockInfo->Index[j] && m_CastItem && m_CastItem->GetEntry() == lockInfo->Index[j])
                    return SPELL_CAST_OK;
                reqKey = true;
                break;
                // check key skill (only single first fit case can be)
            case LOCK_KEY_SKILL:
            {
                reqKey = true;

                // wrong locktype, skip
                if (uint32(m_spellInfo->Effects[effIndex].MiscValue) != lockInfo->Index[j])
                    continue;

                skillId = SkillByLockType(LockType(lockInfo->Index[j]));

                if (skillId != SKILL_NONE)
                {
                    reqSkillValue = lockInfo->Skill[j];

                    // castitem check: rogue using skeleton keys. the skill values should not be added in this case.
                    skillValue = m_CastItem || m_caster->GetTypeId()!= TYPEID_PLAYER ?
                        0 : m_caster->ToPlayer()->GetSkillValue(skillId);

                    // skill bonus provided by casting spell (mostly item spells)
                    // add the effect base points modifier from the spell casted (cheat lock / skeleton key etc.)
                    if (m_spellInfo->Effects[effIndex].TargetA.GetTarget() == TARGET_GAMEOBJECT_ITEM_TARGET || m_spellInfo->Effects[effIndex].TargetB.GetTarget() == TARGET_GAMEOBJECT_ITEM_TARGET)
                        skillValue += m_spellInfo->Effects[effIndex].CalcValue(m_caster, 0);

                    if (skillValue < reqSkillValue)
                        return SPELL_FAILED_LOW_CASTLEVEL;
                }

                return SPELL_CAST_OK;
            }
        }
    }

    if (reqKey)
        return SPELL_FAILED_BAD_TARGETS;

    return SPELL_CAST_OK;
}

void Spell::SetSpellValue(SpellValueMod mod, int32 value)
{
    switch (mod)
    {
        case SPELLVALUE_BASE_POINT0:
            m_spellValue.EffectBasePoints[0] = m_spellInfo->Effects[EFFECT_0].CalcBaseValue(value);
            break;
        case SPELLVALUE_BASE_POINT1:
            if (m_spellInfo->Effects.size() > EFFECT_1)
                m_spellValue.EffectBasePoints[1] = m_spellInfo->Effects[EFFECT_1].CalcBaseValue(value);
            break;
        case SPELLVALUE_BASE_POINT2:
            if (m_spellInfo->Effects.size() > EFFECT_2)
                m_spellValue.EffectBasePoints[2] = m_spellInfo->Effects[EFFECT_2].CalcBaseValue(value);
            break;
        case SPELLVALUE_BASE_POINT3:
            if (m_spellInfo->Effects.size() > EFFECT_3)
                m_spellValue.EffectBasePoints[3] = m_spellInfo->Effects[EFFECT_3].CalcBaseValue(value);
            break;
        case SPELLVALUE_BASE_POINT4:
            if (m_spellInfo->Effects.size() > EFFECT_4)
                m_spellValue.EffectBasePoints[4] = m_spellInfo->Effects[EFFECT_4].CalcBaseValue(value);
            break;
        case SPELLVALUE_BASE_POINT5:
            if (m_spellInfo->Effects.size() > EFFECT_5)
                m_spellValue.EffectBasePoints[5] = m_spellInfo->Effects[EFFECT_5].CalcBaseValue(value);
            break;
        case SPELLVALUE_RADIUS_MOD:
            m_spellValue.RadiusMod = (float)value / 10000;
            break;
        case SPELLVALUE_MAX_TARGETS:
            m_spellValue.MaxAffectedTargets = (uint32)value;
            break;
        case SPELLVALUE_AURA_STACK:
            m_spellValue.AuraStackAmount = uint8(value);
            break;
    }
}

void Spell::PrepareTargetProcessing()
{
    CheckEffectExecuteData();
}

void Spell::FinishTargetProcessing()
{
    SendLogExecute();
}

void Spell::InitEffectExecuteData(uint8 /*effIndex*/)
{
}

void Spell::CheckEffectExecuteData()
{
}

void Spell::LoadScripts()
{
    // Scripts can already be loaded in pet auto cast AI to access scripthooks
    if (!m_loadedScripts.empty())
        return;

    sScriptMgr->CreateSpellScripts(m_spellInfo->Id, m_loadedScripts);
    for (std::list<SpellScript*>::iterator itr = m_loadedScripts.begin(); itr != m_loadedScripts.end();)
    {
        if (!(*itr)->_Load(this))
        {
            std::list<SpellScript*>::iterator bitr = itr;
            ++itr;
            delete (*bitr);
            m_loadedScripts.erase(bitr);
            continue;
        }
        TC_LOG_DEBUG("spells", "Spell::LoadScripts: Script `%s` for spell `%u` is loaded now", (*itr)->_GetScriptName()->c_str(), m_spellInfo->Id);
        (*itr)->Register();
        ++itr;
    }
}

void Spell::CallScriptBeforeCastHandlers()
{
    for (auto &script : m_loadedScripts)
    {
        auto const g = Trinity::makeScriptCallGuard(script, SPELL_SCRIPT_HOOK_BEFORE_CAST);
        for (auto &hook : script->BeforeCast)
            hook.Call(script);
    }
}

void Spell::CallScriptOnCastHandlers()
{
    for (auto &script : m_loadedScripts)
    {
        auto const g = Trinity::makeScriptCallGuard(script, SPELL_SCRIPT_HOOK_ON_CAST);
        for (auto &hook : script->OnCast)
            hook.Call(script);
    }
}

void Spell::CallScriptAfterCastHandlers()
{
    for (auto &script : m_loadedScripts)
    {
        auto const g = Trinity::makeScriptCallGuard(script, SPELL_SCRIPT_HOOK_AFTER_CAST);
        for (auto &hook : script->AfterCast)
            hook.Call(script);
    }
}

SpellCastResult Spell::CallScriptCheckCastHandlers()
{
    for (auto &script : m_loadedScripts)
    {
        auto const g = Trinity::makeScriptCallGuard(script, SPELL_SCRIPT_HOOK_CHECK_CAST);
        for (auto &hook : script->OnCheckCast)
        {
            auto const tempResult = hook.Call(script);
            if (tempResult != SPELL_CAST_OK)
                return tempResult;
        }
    }

    return SPELL_CAST_OK;
}

SpellCastResult Spell::CallScriptCheckAutoCastHandlers()
{
    for (auto &script : m_loadedScripts)
    {
        auto const g = Trinity::makeScriptCallGuard(script, SPELL_SCRIPT_HOOK_CHECK_CAST);
        for (auto &hook : script->OnCheckAutoCast)
        {
            auto const tempResult = hook.Call(script);
            if (tempResult != SPELL_CAST_OK)
                return tempResult;
        }
    }

    return SPELL_CAST_OK;
}


void Spell::PrepareScriptHitHandlers()
{
    for (auto &script : m_loadedScripts)
        script->_InitHit();
}

bool Spell::CallScriptEffectHandlers(SpellEffIndex effIndex, SpellEffectHandleMode mode)
{
    // execute script effect handler hooks and check if effects was prevented
    bool preventDefault = false;

    for (auto &script : m_loadedScripts)
    {
        HookList<SpellScript::EffectHandler> *hookList;
        SpellScriptHookType hookType;

        switch (mode)
        {
            case SPELL_EFFECT_HANDLE_LAUNCH:
                hookList = &script->OnEffectLaunch;
                hookType = SPELL_SCRIPT_HOOK_EFFECT_LAUNCH;
                break;
            case SPELL_EFFECT_HANDLE_LAUNCH_TARGET:
                hookList = &script->OnEffectLaunchTarget;
                hookType = SPELL_SCRIPT_HOOK_EFFECT_LAUNCH_TARGET;
                break;
            case SPELL_EFFECT_HANDLE_HIT:
                hookList = &script->OnEffectHit;
                hookType = SPELL_SCRIPT_HOOK_EFFECT_HIT;
                break;
            case SPELL_EFFECT_HANDLE_HIT_TARGET:
                hookList = &script->OnEffectHitTarget;
                hookType = SPELL_SCRIPT_HOOK_EFFECT_HIT_TARGET;
                break;
            default:
                ASSERT(false);
                return false;
        }

        auto const g = Trinity::makeScriptCallGuard(script, hookType);

        for (auto &hook : *hookList)
            // effect execution can be prevented
            if (!script->_IsEffectPrevented(effIndex) && hook.IsEffectAffected(m_spellInfo, effIndex))
                hook.Call(script, effIndex);

        if (!preventDefault)
            preventDefault = script->_IsDefaultEffectPrevented(effIndex);
    }

    return preventDefault;
}

void Spell::CallScriptBeforeHitHandlers()
{
    for (auto &script : m_loadedScripts)
    {
        auto const g = Trinity::makeScriptCallGuard(script, SPELL_SCRIPT_HOOK_BEFORE_HIT);
        for (auto &hook : script->BeforeHit)
            hook.Call(script);
    }
}

void Spell::CallScriptOnHitHandlers()
{
    for (auto &script : m_loadedScripts)
    {
        auto const g = Trinity::makeScriptCallGuard(script, SPELL_SCRIPT_HOOK_HIT);
        for (auto &hook : script->OnHit)
            hook.Call(script);
    }
}

void Spell::CallScriptAfterHitHandlers()
{
    for (auto &script : m_loadedScripts)
    {
        auto const g = Trinity::makeScriptCallGuard(script, SPELL_SCRIPT_HOOK_AFTER_HIT);
        for (auto &hook : script->AfterHit)
            hook.Call(script);
    }
}

void Spell::CallScriptObjectAreaTargetSelectHandlers(std::list<WorldObject*>& targets, SpellEffIndex effIndex)
{
    for (auto &script : m_loadedScripts)
    {
        auto const g = Trinity::makeScriptCallGuard(script, SPELL_SCRIPT_HOOK_OBJECT_AREA_TARGET_SELECT);
        for (auto &hook : script->OnObjectAreaTargetSelect)
            if (hook.IsEffectAffected(m_spellInfo, effIndex))
                hook.Call(script, targets);
    }
}

void Spell::CallScriptObjectTargetSelectHandlers(WorldObject*& target, SpellEffIndex effIndex)
{
    for (auto &script : m_loadedScripts)
    {
        auto const g = Trinity::makeScriptCallGuard(script, SPELL_SCRIPT_HOOK_OBJECT_TARGET_SELECT);
        for (auto &hook : script->OnObjectTargetSelect)
            if (hook.IsEffectAffected(m_spellInfo, effIndex))
                hook.Call(script, target);
    }
}

bool Spell::CanExecuteTriggersOnHit(uint32 effMask, SpellInfo const* triggeredByAura) const
{
    bool only_on_caster = (triggeredByAura && (triggeredByAura->AttributesEx4 & SPELL_ATTR4_PROC_ONLY_ON_CASTER));
    // If triggeredByAura has SPELL_ATTR4_PROC_ONLY_ON_CASTER then it can only proc on a casted spell with TARGET_UNIT_CASTER
    for (uint8 i = 0; i < m_spellInfo->Effects.size(); ++i)
    {
        if ((effMask & (1 << i)) && (!only_on_caster || (m_spellInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_CASTER)))
            return true;
    }
    return false;
}

void Spell::PrepareTriggersExecutedOnHit()
{
    // todo: move this to scripts
    if (m_spellInfo->SpellFamilyName)
    {
        SpellInfo const* excludeCasterSpellInfo = sSpellMgr->GetSpellInfo(m_spellInfo->ExcludeCasterAuraSpell);
        if (excludeCasterSpellInfo && !excludeCasterSpellInfo->IsPositive())
            m_preCastSpell = m_spellInfo->ExcludeCasterAuraSpell;
        SpellInfo const* excludeTargetSpellInfo = sSpellMgr->GetSpellInfo(m_spellInfo->ExcludeTargetAuraSpell);
        if (excludeTargetSpellInfo && !excludeTargetSpellInfo->IsPositive())
            m_preCastSpell = m_spellInfo->ExcludeTargetAuraSpell;
    }

    // todo: move this to scripts
    switch (m_spellInfo->SpellFamilyName)
    {
        case SPELLFAMILY_MAGE:
        {
             // Permafrost
             if (m_spellInfo->SpellFamilyFlags[1] & 0x00001000 ||  m_spellInfo->SpellFamilyFlags[0] & 0x00100220)
                 m_preCastSpell = 68391;
             break;
        }
    }

    if (m_spellInfo->IsPassive())
        return;

    // handle SPELL_AURA_ADD_TARGET_TRIGGER auras:
    // save auras which were present on spell caster on cast, to prevent triggered auras from affecting caster
    // and to correctly calculate proc chance when combopoints are present
    Unit::AuraEffectList const& targetTriggers = m_caster->GetAuraEffectsByType(SPELL_AURA_ADD_TARGET_TRIGGER);
    for (Unit::AuraEffectList::const_iterator i = targetTriggers.begin(); i != targetTriggers.end(); ++i)
    {
        if (!(*i)->IsAffectingSpell(m_spellInfo))
            continue;
        SpellInfo const* auraSpellInfo = (*i)->GetSpellInfo();
        uint32 auraSpellIdx = (*i)->GetEffIndex();
        if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(auraSpellInfo->Effects[auraSpellIdx].TriggerSpell))
        {
            // calculate the chance using spell base amount, because aura amount is not updated on combo-points change
            // this possibly needs fixing
            int32 auraBaseAmount = (*i)->GetBaseAmount();
            // proc chance is stored in effect amount
            int32 chance = m_caster->CalculateSpellDamage(NULL, auraSpellInfo, auraSpellIdx, &auraBaseAmount);
            // build trigger and add to the list
            HitTriggerSpell spellTriggerInfo;
            spellTriggerInfo.triggeredSpell = spellInfo;
            spellTriggerInfo.triggeredByAura = auraSpellInfo;
            spellTriggerInfo.chance = chance * (*i)->GetBase()->GetStackAmount();
            m_hitTriggerSpells.push_back(spellTriggerInfo);
        }
    }
}

// Global cooldowns management
enum GCDLimits
{
    MIN_GCD = 1000,
    MAX_GCD = 1500
};

bool Spell::HasGlobalCooldown() const
{
    // Only player or controlled units have global cooldown
    if (m_caster->GetCharmInfo())
        return m_caster->GetCharmInfo()->GetGlobalCooldownMgr().HasGlobalCooldown(m_caster, m_spellInfo);
    else if (m_caster->GetTypeId() == TYPEID_PLAYER)
        return m_caster->ToPlayer()->GetGlobalCooldownMgr().HasGlobalCooldown(m_caster, m_spellInfo);
    else
        return false;
}

void Spell::TriggerGlobalCooldown()
{
    int32 gcd = m_spellInfo->StartRecoveryTime;

    if (gcd == 0)
    {
        if (!m_caster->GetCharmInfo())
            return;
        if (m_spellInfo->CategoryRecoveryTime == 0 && m_spellInfo->RecoveryTime == 0)
            gcd = MIN_GCD;
    }

     if (m_caster->GetTypeId() == TYPEID_PLAYER)
          if (m_caster->ToPlayer()->GetCommandStatus(CHEAT_COOLDOWN))
               return;

    // Global cooldown can't leave range 1..1.5 secs
    // There are some spells (mostly not casted directly by player) that have < 1 sec and > 1.5 sec global cooldowns
    // but as tests show are not affected by any spell mods.
    if (m_spellInfo->StartRecoveryTime >= MIN_GCD && m_spellInfo->StartRecoveryTime <= MAX_GCD)
    {
        // gcd modifier auras are applied only to own spells and only players have such mods
        if (m_caster->GetTypeId() == TYPEID_PLAYER)
            m_caster->ToPlayer()->ApplySpellMod(m_spellInfo->Id, SPELLMOD_GLOBAL_COOLDOWN, gcd, this);

        if (gcd == 0)
            return;

        // Apply haste rating
        gcd = int32(float(gcd) * m_caster->GetFloatValue(UNIT_MOD_CAST_SPEED));
        if (gcd < MIN_GCD)
            gcd = MIN_GCD;
        else if (gcd > MAX_GCD)
            gcd = MAX_GCD;
    }

    // Only players or controlled units have global cooldown
    if (m_caster->GetCharmInfo())
        m_caster->GetCharmInfo()->GetGlobalCooldownMgr().AddGlobalCooldown(m_spellInfo, gcd);
    else if (m_caster->GetTypeId() == TYPEID_PLAYER)
        m_caster->ToPlayer()->GetGlobalCooldownMgr().AddGlobalCooldown(m_spellInfo, gcd);
}

void Spell::CancelGlobalCooldown()
{
    if (!m_spellInfo->StartRecoveryTime)
        return;

    // Cancel global cooldown when interrupting current cast
    if (m_caster->GetCurrentSpell(CURRENT_GENERIC_SPELL) != this)
        return;

    // Only players or controlled units have global cooldown
    if (m_caster->GetCharmInfo())
        m_caster->GetCharmInfo()->GetGlobalCooldownMgr().CancelGlobalCooldown(m_spellInfo);
    else if (m_caster->GetTypeId() == TYPEID_PLAYER)
        m_caster->ToPlayer()->GetGlobalCooldownMgr().CancelGlobalCooldown(m_spellInfo);
}

bool Spell::IsCritForTarget(Unit* target) const
{
    if (!target)
        return false;

    for (auto itr : m_UniqueTargetInfo)
        if (itr.targetGUID == target->GetGUID() && itr.crit)
            return true;

    return false;
}

bool Spell::IsDarkSimulacrum() const
{
    // Dark Simulacrum
    if (AuraEffect *darkSimulacrum = m_caster->GetAuraEffect(77616, 0))
    {
        if (m_spellInfo->Id == static_cast<uint32>(darkSimulacrum->GetAmount()))
        {
            return true;
        }
        else
        {
            SpellInfo const* amountSpell = sSpellMgr->GetSpellInfo(darkSimulacrum->GetAmount());
            if (!amountSpell)
                return false;

            SpellInfo const* triggerSpell = sSpellMgr->GetSpellInfo(amountSpell->Effects[0].BasePoints);
            if (!triggerSpell)
                return false;

            if (m_spellInfo->Id == triggerSpell->Id)
                return true;
        }
    }

    return false;
}

bool Spell::LOSAdditionalRules(Unit const* target, int8 eff) const
{
    uint8 const first = eff == -1 ? 0 : eff;
    uint8 const last = eff == -1 ? m_spellInfo->Effects.size() : eff + 1;

    // Okay, custom rules for LoS
    for (uint8 i = first; i < last; ++i)
    {
        // like paladin auras
        if (m_spellInfo->Effects[i].Effect == SPELL_EFFECT_APPLY_AREA_AURA_RAID)
            return true;

        // like bloodlust / prayers
        if (m_spellInfo->Effects[i].ApplyAuraName && (m_spellInfo->Effects[i].TargetB.GetTarget() == TARGET_UNIT_SRC_AREA_ALLY ||
            m_spellInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_CASTER_AREA_RAID))
            return !IsMorePowerfulAura(target);

        if (m_spellInfo->IsChanneled())
            continue;

        switch (m_spellInfo->Effects[i].TargetA.GetTarget())
        {
            case TARGET_UNIT_PET:
            case TARGET_UNIT_MASTER:
                return true;
            default:
                break;
        }
    }

    return false;
}

bool Spell::IsMorePowerfulAura(Unit const* target) const
{
    if (m_spellInfo->GetDuration() >= 2 * MINUTE * IN_MILLISECONDS)
    {
        switch (m_spellInfo->Effects[0].ApplyAuraName)
        {
            case SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE:
            case SPELL_AURA_MOD_STAT:
            case SPELL_AURA_MOD_RANGED_ATTACK_POWER:
            {
                Unit::VisibleAuraMap const *visibleAuras = target->GetVisibleAuras();
                for (Unit::VisibleAuraMap::const_iterator itr = visibleAuras->begin(); itr != visibleAuras->end(); ++itr)
                    if (AuraEffect *auraeff = itr->second->GetBase()->GetEffect(0))
                    {
                        if (auraeff->GetBase()->GetDuration() <= 2*MINUTE*IN_MILLISECONDS)
                            continue;

                        if (auraeff->GetSpellInfo()->SpellFamilyName == SPELLFAMILY_POTION)
                            continue;

                        if (auraeff->GetAuraType() == m_spellInfo->Effects[0].ApplyAuraName &&
                            (m_spellInfo->Effects[0].ApplyAuraName == SPELL_AURA_MOD_RANGED_ATTACK_POWER || m_spellInfo->Effects[0].MiscValue == auraeff->GetMiscValue()))
                        {
                            uint32 dmg = abs(CalculateDamage(0, target));
                            uint32 amount = abs(auraeff->GetAmount());
                            if (amount < dmg)
                                continue;

                            else if (amount == dmg && m_spellInfo->GetDuration() > auraeff->GetBase()->GetDuration())
                                continue;

                            return true;
                        }
                    }
            }
            default:
                break;
        }
    }
    return false;
}

namespace Trinity
{

WorldObjectSpellTargetCheck::WorldObjectSpellTargetCheck(Unit* caster, Unit* referer, SpellInfo const* spellInfo,
            SpellTargetCheckTypes selectionType, ConditionList* condList) : _caster(caster), _referer(referer), _spellInfo(spellInfo),
    _targetSelectionType(selectionType), _condList(condList)
{
    if (condList)
        _condSrcInfo = new ConditionSourceInfo(NULL, caster);
    else
        _condSrcInfo = NULL;
}

WorldObjectSpellTargetCheck::~WorldObjectSpellTargetCheck()
{
    if (_condSrcInfo)
        delete _condSrcInfo;
}

bool WorldObjectSpellTargetCheck::operator()(WorldObject* target, bool checkAuraStates)
{
    if (_spellInfo->CheckTarget(_caster, target, true, checkAuraStates) != SPELL_CAST_OK)
        return false;

    Unit* unitTarget = target->ToUnit();
    if (Corpse* corpseTarget = target->ToCorpse())
    {
        // use ofter for party/assistance checks
        if (Player* owner = ObjectAccessor::FindPlayer(corpseTarget->GetOwnerGUID()))
            unitTarget = owner;
        else
            return false;
    }
    if (unitTarget)
    {
        switch (_targetSelectionType)
        {
            case TARGET_CHECK_ENEMY:
                if (unitTarget->isTotem())
                    return false;
                if (!_caster->_IsValidAttackTarget(unitTarget, _spellInfo))
                    return false;
                break;
            case TARGET_CHECK_ALLY:
                if (unitTarget->isTotem())
                    return false;
                if (!_caster->_IsValidAssistTarget(unitTarget, _spellInfo))
                    return false;
                break;
            case TARGET_CHECK_PARTY:
                if (unitTarget->isTotem())
                    return false;
                if (!_caster->_IsValidAssistTarget(unitTarget, _spellInfo))
                    return false;
                if (!_referer->IsInPartyWith(unitTarget))
                    return false;
                break;
            case TARGET_CHECK_RAID_CLASS:
                if (_referer->getClass() != unitTarget->getClass())
                    return false;
                // nobreak;
            case TARGET_CHECK_RAID:
                if (unitTarget->isTotem())
                    return false;
                if (!_caster->_IsValidAssistTarget(unitTarget, _spellInfo))
                    return false;
                if (!_referer->IsInRaidWith(unitTarget))
                    return false;
                break;
            default:
                break;
        }
    }
    if (!_condSrcInfo)
        return true;
    _condSrcInfo->mConditionTargets[0] = target;
    return sConditionMgr->IsObjectMeetToConditions(*_condSrcInfo, *_condList);
}

WorldObjectSpellNearbyTargetCheck::WorldObjectSpellNearbyTargetCheck(float range, Unit* caster, SpellInfo const* spellInfo,
    SpellTargetCheckTypes selectionType, ConditionList* condList)
    : WorldObjectSpellTargetCheck(caster, caster, spellInfo, selectionType, condList), _range(range), _position(caster)
{
}

bool WorldObjectSpellNearbyTargetCheck::operator()(WorldObject* target)
{
    float dist = target->GetDistance(*_position);
    if (dist < _range && WorldObjectSpellTargetCheck::operator ()(target) && _caster->GetGUID() != target->GetGUID())
    {
        _range = dist;
        return true;
    }
    return false;
}

WorldObjectSpellAreaTargetCheck::WorldObjectSpellAreaTargetCheck(float range, Position const* position, Unit* caster,
    Unit* referer, SpellInfo const* spellInfo, SpellTargetCheckTypes selectionType, ConditionList* condList, bool checkAuraStates)
    : WorldObjectSpellTargetCheck(caster, referer, spellInfo, selectionType, condList), _range(range), _position(position), _checkAuraStates(checkAuraStates)
{
}

bool WorldObjectSpellAreaTargetCheck::operator()(WorldObject* target)
{
    if (!target->IsWithinDist3d(_position, _range))
        return false;
    return WorldObjectSpellTargetCheck::operator ()(target, _checkAuraStates);
}

WorldObjectSpellConeTargetCheck::WorldObjectSpellConeTargetCheck(float coneAngle, float range, Unit* caster,
    SpellInfo const* spellInfo, SpellTargetCheckTypes selectionType, ConditionList* condList)
    : WorldObjectSpellAreaTargetCheck(range, caster, caster, caster, spellInfo, selectionType, condList), _coneAngle(coneAngle)
{
}

bool WorldObjectSpellConeTargetCheck::operator()(WorldObject* target)
{
    if (_spellInfo->AttributesCu & SPELL_ATTR0_CU_CONE_BACK)
    {
        if (!_caster->isInBack(target, _coneAngle))
            return false;
    }
    else if (_spellInfo->AttributesCu & SPELL_ATTR0_CU_CONE_LINE)
    {
        if (!_caster->HasInLine(target, _caster->GetObjectSize()))
            return false;
    }
    else
    {
        if (!_caster->isInFront(target, _coneAngle))
            return false;
    }
    return WorldObjectSpellAreaTargetCheck::operator ()(target);
}

WorldObjectSpellTrajTargetCheck::WorldObjectSpellTrajTargetCheck(float range, Position const* position, Unit* caster, SpellInfo const* spellInfo)
    : WorldObjectSpellAreaTargetCheck(range, position, caster, caster, spellInfo, TARGET_CHECK_DEFAULT, NULL)
{
}

bool WorldObjectSpellTrajTargetCheck::operator()(WorldObject* target)
{
    // return all targets on missile trajectory (0 - size of a missile)
    if (!_caster->HasInLine(target, 0))
        return false;
    return WorldObjectSpellAreaTargetCheck::operator ()(target);
}

} //namespace Trinity
