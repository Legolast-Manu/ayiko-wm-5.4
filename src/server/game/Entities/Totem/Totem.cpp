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

#include "Totem.h"
#include "WorldPacket.h"
#include "Log.h"
#include "Group.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "SpellInfo.h"

#define STONECLAW_TOTEM_ENTRY 59712

Totem::Totem(SummonPropertiesEntry const* properties, Unit* owner) : Minion(properties, owner, false)
{
    m_unitTypeMask |= UNIT_MASK_TOTEM;
    m_duration = 0;
    m_type = TOTEM_PASSIVE;
}

void Totem::Update(uint32 time)
{
    if (!m_owner->IsAlive() || !IsAlive())
    {
        UnSummon();                                         // remove self
        return;
    }

    if (m_duration <= time)
    {
        UnSummon();                                         // remove self
        return;
    }
    else
        m_duration -= time;

    Creature::Update(time);
}

void Totem::InitStats(uint32 duration)
{
    uint32 spellId1 = 0;
    uint32 spellId2 = 0;
    uint32 spellId3 = 0;
    uint32 spellId4 = 0;

    // client requires SMSG_TOTEM_CREATED to be sent before adding to world and before removing old totem
    if (m_owner->GetTypeId() == TYPEID_PLAYER
            && m_Properties->Slot >= SUMMON_SLOT_TOTEM
            && m_Properties->Slot < MAX_TOTEM_SLOT)
    {
        WorldPacket data(SMSG_TOTEM_CREATED, 1 + 8 + 4 + 4);
        ObjectGuid totemGuid = GetGUID();

        data.WriteBitSeq<2, 6, 4, 0, 7, 3, 5, 1>(totemGuid);

        data << uint32(duration);
        data.WriteByteSeq<1, 3>(totemGuid);
        data << uint32(GetUInt32Value(UNIT_CREATED_BY_SPELL));
        data.WriteByteSeq<7, 4, 0>(totemGuid);
        data << uint8(m_Properties->Slot - 1);
        data.WriteByteSeq<6, 2, 5>(totemGuid);

        m_owner->ToPlayer()->SendDirectMessage(&data);

        // set display id depending on caster's race (not for statues)
        if (m_Properties->Type != SUMMON_TYPE_STATUE && m_Properties->Type != SUMMON_TYPE_WAR_BANNER)
            SetDisplayId(m_owner->GetModelForTotem(PlayerTotemType(m_Properties->Id)));

        // Light's Hammer
        if (GetUInt32Value(UNIT_CREATED_BY_SPELL) == 122773)
            SetDisplayId(11686);

        // Totemic Encirclement
        if (m_owner->HasAura(58057)
            && GetUInt32Value(UNIT_CREATED_BY_SPELL) != 120214
            && GetUInt32Value(UNIT_CREATED_BY_SPELL) != 120217
            && GetUInt32Value(UNIT_CREATED_BY_SPELL) != 120218
            && GetUInt32Value(UNIT_CREATED_BY_SPELL) != 120219)
        {
            for (size_t i = SUMMON_SLOT_TOTEM; i < MAX_TOTEM_SLOT; ++i)
            {
                if (i != m_Properties->Slot)
                {
                    if (Creature* totem = m_owner->GetMap()->GetCreature(m_owner->m_SummonSlot[i]))
                    {
                        uint32 spell_id = totem->GetUInt32Value(UNIT_CREATED_BY_SPELL);
                        if (spell_id != 120214 && spell_id != 120217 && spell_id != 120218 && spell_id != 120219)
                            continue;
                    }

                    switch (i)
                    {
                        case 1:// Fire
                            spellId1 = 120217;
                            break;
                        case 2:// Earth
                            spellId2 = 120218;
                            break;
                        case 3:// Water
                            spellId3 = 120214;
                            break;
                        case 4:// Wind
                            spellId4 = 120219;
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }

    Minion::InitStats(duration);

    // Get spell cast by totem
    if (SpellInfo const* totemSpell = sSpellMgr->GetSpellInfo(GetSpell()))
        if (totemSpell->CalcCastTime())   // If spell has cast time -> its an active totem
            m_type = TOTEM_ACTIVE;

    if (GetEntry() == SENTRY_TOTEM_ENTRY)
        SetReactState(REACT_AGGRESSIVE);

    m_duration = duration;

    SetLevel(m_owner->getLevel());

    // Totems must receive stamina from owner
    if (GetEntry() == STONECLAW_TOTEM_ENTRY)
        SetModifierValue(UNIT_MOD_STAT_STAMINA, BASE_VALUE, float(m_owner->GetStat(STAT_STAMINA)) * 0.1f);

    if (spellId1)
        m_owner->CastSpell(m_owner, spellId1, true); // Fake Fire Totem
    if (spellId2)
        m_owner->CastSpell(m_owner, spellId2, true); // Fake Earth Totem
    if (spellId3)
        m_owner->CastSpell(m_owner, spellId3, true); // Fake Water Totem
    if (spellId4)
        m_owner->CastSpell(m_owner, spellId4, true); // Fake Wind Totem
}

void Totem::InitSummon()
{
    if (m_type == TOTEM_PASSIVE && GetSpell())
    {
        switch (GetSpell())
        {
            case 8178: // Grounding Totem
                if (m_owner->HasAura(55441))
                {
                    m_spells[0] = 89523;
                    CastSpell(this, 89523, true);
                    return;
                }
                break;
        }
        CastSpell(this, GetSpell(), true);
    }

    // Some totems can have both instant effect and passive spell
    if (GetSpell(1))
        CastSpell(this, GetSpell(1), true);
}

void Totem::UnSummon(uint32 msTime)
{
    if (msTime)
    {
        m_Events.AddEvent(new ForcedUnsummonDelayEvent(*this), m_Events.CalculateTime(msTime));
        return;
    }

    CombatStop();
    RemoveAurasDueToSpell(GetSpell(), GetGUID());

    // clear owner's totem slot
    for (int i = SUMMON_SLOT_TOTEM; i < MAX_TOTEM_SLOT; ++i)
    {
        if (m_owner->m_SummonSlot[i] == GetGUID())
        {
            m_owner->m_SummonSlot[i] = 0;
            break;
        }
    }

    // Stone Bulwark totem does not remove aura from owner
    if (GetEntry() != 59712)
        m_owner->RemoveAurasDueToSpell(GetSpell(), GetGUID());

    // Remove Sentry Totem Aura
    if (GetEntry() == SENTRY_TOTEM_ENTRY)
        m_owner->RemoveAurasDueToSpell(SENTRY_TOTEM_SPELLID);

    //remove aura all party members too
    if (Player* owner = m_owner->ToPlayer())
    {
        owner->SendAutoRepeatCancel(this);

        if (SpellInfo const* spell = sSpellMgr->GetSpellInfo(GetUInt32Value(UNIT_CREATED_BY_SPELL)))
            owner->SendCooldownEvent(spell, 0, NULL, false);

        if (Group* group = owner->GetGroup())
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* target = itr->GetSource();
                if (target && group->SameSubGroup(owner, target))
                    target->RemoveAurasDueToSpell(GetSpell(), GetGUID());
            }
        }
    }

    AddObjectToRemoveList();
}

bool Totem::IsImmunedToSpellEffect(SpellInfo const* spellInfo, uint32 index) const
{
    // TODO: possibly all negative auras immune?
    if (GetEntry() == 5925)
        return false;

    switch (spellInfo->Effects[index].ApplyAuraName)
    {
        case SPELL_AURA_PERIODIC_DAMAGE:
        case SPELL_AURA_PERIODIC_LEECH:
        case SPELL_AURA_MOD_FEAR:
        case SPELL_AURA_MOD_FEAR_2:
        case SPELL_AURA_TRANSFORM:
            return true;
        default:
            break;
    }

    return Creature::IsImmunedToSpellEffect(spellInfo, index);
}

uint32 Totem::GetSpell(uint8 slot)
{
    // Glyph of Grounding Totem
    if (m_spells[slot] == 8178 && this->GetOwner()->HasAura(55441))
        return 89523;

    return m_spells[slot];
}
