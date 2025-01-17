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
#include "Log.h"
#include "ObjectMgr.h"
#include "Vehicle.h"
#include "Unit.h"
#include "Util.h"
#include "WorldPacket.h"
#include "ScriptMgr.h"
#include "CreatureAI.h"
#include "ZoneScript.h"
#include "SpellMgr.h"
#include "SpellInfo.h"
#include "MoveSplineInit.h"

Vehicle::Vehicle(Unit* unit, VehicleEntry const* vehInfo, uint32 creatureEntry)
    : _me(unit)
    , _vehicleInfo(vehInfo)
    , _usableSeatNum(0)
    , _creatureEntry(creatureEntry)
    , _status(STATUS_NONE)
{
    for (uint32 i = 0; i < MAX_VEHICLE_SEATS; ++i)
    {
        if (uint32 seatId = _vehicleInfo->m_seatID[i])
            if (VehicleSeatEntry const* veSeat = sVehicleSeatStore.LookupEntry(seatId))
            {
                Seats.insert(std::make_pair(i, VehicleSeat(veSeat)));
                if (veSeat->CanEnterOrExit())
                    ++_usableSeatNum;
            }
    }

    InitMovementInfoForBase();
}

Vehicle::~Vehicle()
{
    /// @Uninstall must be called before this.
    ASSERT(_status == STATUS_UNINSTALLING);
    for (SeatMap::const_iterator itr = Seats.begin(); itr != Seats.end(); ++itr)
        ASSERT(!itr->second.Passenger);
}

void Vehicle::Install()
{
    if (Creature* creature = _me->ToCreature())
    {
        switch (_vehicleInfo->m_powerType)
        {
            case POWER_STEAM:
            case POWER_HEAT:
            case POWER_BLOOD:
            case POWER_OOZE:
            case POWER_WRATH:
                _me->setPowerType(POWER_ENERGY);
                _me->SetMaxPower(POWER_ENERGY, 100);
                break;
            case POWER_PYRITE:
                _me->setPowerType(POWER_ENERGY);
                _me->SetMaxPower(POWER_ENERGY, 50);
                break;
            default:
                for (uint32 i = 0; i < MAX_SPELL_VEHICLE; ++i)
                {
                    if (!creature->m_spells[i])
                        continue;

                    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(creature->m_spells[i]);
                    if (!spellInfo)
                        continue;

                    if (spellInfo->spellPower.PowerType == POWER_ENERGY)
                    {
                        _me->setPowerType(POWER_ENERGY);
                        _me->SetMaxPower(POWER_ENERGY, 100);
                        break;
                    }
                }
                break;
        }
    }

    _status = STATUS_INSTALLED;
    if (GetBase()->GetTypeId() == TYPEID_UNIT)
        sScriptMgr->OnInstall(this);
}

void Vehicle::InstallAllAccessories(bool evading)
{
    if (GetBase()->GetTypeId() == TYPEID_PLAYER || !evading)
        RemoveAllPassengers();   // We might have aura's saved in the DB with now invalid casters - remove

    VehicleAccessoryList const* accessories = sObjectMgr->GetVehicleAccessoryList(this);
    if (!accessories)
        return;

    for (VehicleAccessoryList::const_iterator itr = accessories->begin(); itr != accessories->end(); ++itr)
        if (!evading || itr->IsMinion)  // only install minions on evade mode
            InstallAccessory(itr->AccessoryEntry, itr->SeatId, itr->IsMinion, itr->SummonedType, itr->SummonTime);
}

void Vehicle::Uninstall(bool dismount/* = false*/)
{
    /// @Prevent recursive uninstall call. (Bad script in OnUninstall/OnRemovePassenger/PassengerBoarded hook.)
    if (_status == STATUS_UNINSTALLING)
    {
        TC_LOG_DEBUG("entities.vehicle", "Vehicle GuidLow: %u, Entry: %u attempts to uninstall, but already has STATUS_UNINSTALLING! "
            "Check Uninstall/PassengerBoarded script hooks for errors.", _me->GetGUIDLow(), _me->GetEntry());
        return;
    }
    _status = STATUS_UNINSTALLING;
    TC_LOG_DEBUG("entities.vehicle", "Vehicle::Uninstall Entry: %u, GuidLow: %u", _creatureEntry, _me->GetGUIDLow());
    RemoveAllPassengers(dismount);

    if (GetBase() && GetBase()->GetTypeId() == TYPEID_UNIT)
        sScriptMgr->OnUninstall(this);
}

void Vehicle::Reset(bool evading /*= false*/)
{
    TC_LOG_DEBUG("entities.vehicle", "Vehicle::Reset Entry: %u, GuidLow: %u", _creatureEntry, _me->GetGUIDLow());
    if (_me->GetTypeId() == TYPEID_PLAYER)
    {
        if (_usableSeatNum)
            _me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_PLAYER_VEHICLE);
    }
    else
    {
        ApplyAllImmunities();
        InstallAllAccessories(evading);
        if (_usableSeatNum)
            _me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
    }

    if (GetBase()->GetTypeId() == TYPEID_UNIT)
        sScriptMgr->OnReset(this);
}

void Vehicle::ApplyAllImmunities()
{
    // This couldn't be done in DB, because some spells have MECHANIC_NONE

    // Vehicles should be immune on Knockback ...
    _me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
    _me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);

    // Mechanical units & vehicles ( which are not Bosses, they have own immunities in DB ) should be also immune on healing ( exceptions in switch below )
    if (_me->ToCreature() && _me->ToCreature()->GetCreatureTemplate()->type == CREATURE_TYPE_MECHANICAL && !_me->ToCreature()->isWorldBoss())
    {
        // Heal & dispel ...
        _me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_HEAL, true);
        _me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_HEAL_PCT, true);
        _me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_DISPEL, true);
        _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_PERIODIC_HEAL, true);

        // ... Shield & Immunity grant spells ...
        _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_SCHOOL_IMMUNITY, true);
        _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_UNATTACKABLE, true);
        _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_SCHOOL_ABSORB, true);
        _me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SHIELD, true);
        _me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_IMMUNE_SHIELD, true);
        _me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);

        // ... Resistance, Split damage, Change stats ...
        _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_DAMAGE_SHIELD, true);
        _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_SPLIT_DAMAGE_PCT, true);
        _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_RESISTANCE, true);
        _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_STAT, true);
        _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, true);
    }

    // Different immunities for vehicles goes below
    switch (GetVehicleInfo()->m_ID)
    {
        // code below prevents a bug with movable cannons
        case 160: // Strand of the Ancients
        case 244: // Wintergrasp
        case 321: // Pilgrim's Bounty chairs
        case 510: // Isle of Conquest
            _me->SetControlled(true, UNIT_STATE_ROOT);
            // why we need to apply this? we can simple add immunities to slow mechanic in DB
            _me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_DECREASE_SPEED, true);
            break;
        default:
            break;
    }
}

void Vehicle::RemoveAllPassengers(bool dismount/* = false*/)
{
    TC_LOG_DEBUG("entities.vehicle", "Vehicle::RemoveAllPassengers. Entry: %u, GuidLow: %u", _creatureEntry, _me->GetGUIDLow());

    // Passengers always cast an aura with SPELL_AURA_CONTROL_VEHICLE on the vehicle
    // We just remove the aura and the unapply handler will make the target leave the vehicle.
    // We don't need to iterate over Seats
    _me->RemoveAurasByType(SPELL_AURA_CONTROL_VEHICLE);

    if (dismount)
    {
        for (SeatMap::iterator itr = Seats.begin(); itr != Seats.end(); ++itr)
        {
            if (itr->second.Passenger)
            {
                if (Unit* passenger = ObjectAccessor::GetUnit(*GetBase(), itr->second.Passenger))
                    passenger->ExitVehicle();
                else
                    itr->second.Passenger = 0;
            }
        }
    }
    // Following the above logic, this assertion should NEVER fail.
    // Even in 'hacky' cases, there should at least be VEHICLE_SPELL_RIDE_HARDCODED on us.
    // SeatMap::const_iterator itr;
    // for (itr = Seats.begin(); itr != Seats.end(); ++itr)
    //    ASSERT(!itr->second.passenger);
}

bool Vehicle::HasEmptySeat(int8 seatId) const
{
    SeatMap::const_iterator seat = Seats.find(seatId);
    if (seat == Seats.end())
        return false;
    return !seat->second.Passenger;
}

Unit* Vehicle::GetPassenger(int8 seatId) const
{
    SeatMap::const_iterator seat = Seats.find(seatId);
    if (seat == Seats.end())
        return NULL;

    return ObjectAccessor::GetUnit(*GetBase(), seat->second.Passenger);
}

int8 Vehicle::GetNextEmptySeat(int8 seatId, bool next) const
{
    SeatMap::const_iterator seat = Seats.find(seatId);
    if (seat == Seats.end())
        return -1;

    while (seat->second.Passenger || (!seat->second.SeatInfo->CanEnterOrExit() && !seat->second.SeatInfo->IsUsableByOverride()))
    {
        if (next)
        {
            ++seat;
            if (seat == Seats.end())
                seat = Seats.begin();
        }
        else
        {
            if (seat == Seats.begin())
                seat = Seats.end();
            --seat;
        }

        if (seat->first == seatId)
            return -1; // no available seat
    }

    return seat->first;
}

void Vehicle::InstallAccessory(uint32 entry, int8 seatId, bool minion, uint8 type, uint32 summonTime)
{
    /// @Prevent adding accessories when vehicle is uninstalling. (Bad script in OnUninstall/OnRemovePassenger/PassengerBoarded hook.)
    if (_status == STATUS_UNINSTALLING)
    {
        TC_LOG_DEBUG("entities.vehicle", "Vehicle GuidLow: %u, Entry: %u attempts to install accessory Entry: %u on seat %d with STATUS_UNINSTALLING! "
            "Check Uninstall/PassengerBoarded script hooks for errors.", _me->GetGUIDLow(), _me->GetEntry(), entry, (int32)seatId);
        return;
    }

    TC_LOG_DEBUG("entities.vehicle", "Vehicle: Installing accessory entry %u on vehicle entry %u (seat:%i)", entry, GetCreatureEntry(), seatId);
    if (Unit* passenger = GetPassenger(seatId))
    {
        // already installed
        if (passenger->GetEntry() == entry)
        {
            ASSERT(passenger->GetTypeId() == TYPEID_UNIT);
            if (_me->GetTypeId() == TYPEID_UNIT)
            {
                if (_me->ToCreature()->IsInEvadeMode() && passenger->ToCreature()->IsAIEnabled)
                    passenger->ToCreature()->AI()->EnterEvadeMode();
                return;
            }
        }
        else
            passenger->ExitVehicle(); // this should not happen
    }

    if (TempSummon* accessory = _me->SummonCreature(entry, *_me, TempSummonType(type), summonTime))
    {
        if (minion)
            accessory->AddUnitTypeMask(UNIT_MASK_ACCESSORY);

        if (!_me->HandleSpellClick(accessory, seatId))
        {
            accessory->UnSummon();
            return;
        }

        if (GetBase()->GetTypeId() == TYPEID_UNIT)
            sScriptMgr->OnInstallAccessory(this, accessory);
    }
}

bool Vehicle::CheckCustomCanEnter()
{
    switch (GetCreatureEntry())
    {
        case 65522:  // Bubble shield, Stormstout Brewery
        case 56682: // Keg in Stormstout Brewery
        case 46185: // Sanitron
            return true;
    }

    return false;
}

bool Vehicle::AddPassenger(Unit* unit, int8 seatId)
{
    /// @Prevent adding passengers when vehicle is uninstalling. (Bad script in OnUninstall/OnRemovePassenger/PassengerBoarded hook.)
    if (_status == STATUS_UNINSTALLING)
    {
        TC_LOG_DEBUG("entities.vehicle", "Passenger GuidLow: %u, Entry: %u, attempting to board vehicle GuidLow: %u, Entry: %u during uninstall! SeatId: %i",
            unit->GetGUIDLow(), unit->GetEntry(), _me->GetGUIDLow(), _me->GetEntry(), (int32)seatId);
        return false;
    }

    if (unit->GetVehicle() != this)
        return false;

    if (unit->GetTypeId() == TYPEID_PLAYER && unit->GetMap()->IsBattleArena())
        return false;

    SeatMap::iterator seat;
    if (seatId < 0) // no specific seat requirement
    {
        for (seat = Seats.begin(); seat != Seats.end(); ++seat)
            if (!seat->second.Passenger && (seat->second.SeatInfo->CanEnterOrExit() || seat->second.SeatInfo->IsUsableByOverride() || CheckCustomCanEnter()))
                break;

        if (seat == Seats.end()) // no available seat
            return false;
    }
    else
    {
        seat = Seats.find(seatId);
        if (seat == Seats.end())
            return false;

        if (seat->second.Passenger)
        {
            if (Unit* passenger = ObjectAccessor::GetUnit(*GetBase(), seat->second.Passenger))
                passenger->ExitVehicle();
            else
                seat->second.Passenger = 0;
        }

        ASSERT(!seat->second.Passenger);
    }

    TC_LOG_DEBUG("entities.vehicle", "Unit %s enter vehicle entry %u id %u dbguid %u seat %d",
                 unit->GetName().c_str(), _me->GetEntry(), _vehicleInfo->m_ID, _me->GetGUIDLow(), (int32)seat->first);

    seat->second.Passenger = unit->GetGUID();
    if (seat->second.SeatInfo->CanEnterOrExit())
    {
        ASSERT(_usableSeatNum);
        --_usableSeatNum;
        if (!_usableSeatNum)
        {
            if (_me->GetTypeId() == TYPEID_PLAYER)
                _me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_PLAYER_VEHICLE);
            else
                _me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        }
    }

    if (seat->second.SeatInfo->m_flags & VEHICLE_SEAT_FLAG_PASSENGER_NOT_SELECTABLE)
        unit->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

    VehicleSeatEntry const* veSeat = seat->second.SeatInfo;

    Position enterPosition;

    // Hackfix
    switch (veSeat->m_ID)
    {
        case 10882:
            enterPosition.Relocate(15.0f, 0.0f, 30.0f, 0.0f);
            break;
        default:
            enterPosition.Relocate(veSeat->m_attachmentOffsetX, veSeat->m_attachmentOffsetY, veSeat->m_attachmentOffsetZ, 0);
            break;
    }
    unit->m_movementInfo.t_pos.Relocate(enterPosition);
    unit->m_movementInfo.t_time = 0; // 1 for player
    unit->m_movementInfo.t_seat = seat->first;
    unit->m_movementInfo.t_guid = _me->GetGUID();

    if (Creature* creature = _me->ToCreature())
        if (creature->IsAIEnabled)
            creature->AI()->PassengerWillBoard(unit, enterPosition, seatId);

    if (_me->GetTypeId() == TYPEID_UNIT
            && unit->GetTypeId() == TYPEID_PLAYER
            && (seat->second.SeatInfo->m_flags & VEHICLE_SEAT_FLAG_CAN_CONTROL))
    {
        bool result = _me->SetCharmedBy(unit, CHARM_TYPE_VEHICLE);
        ASSERT(result);  // SMSG_CLIENT_CONTROL
    }

    unit->SendClearTarget();                                // SMSG_BREAK_TARGET
    if (unit->GetTypeId() == TYPEID_PLAYER && _me->IsFlying())
        unit->SetDisableGravity(true);
    unit->SetControlled(true, UNIT_STATE_ROOT);             // SMSG_FORCE_ROOT - In some cases we send SMSG_SPLINE_MOVE_ROOT here (for creatures)
                                                            // also adds MOVEMENTFLAG_ROOT
    Movement::MoveSplineInit init(unit);
    init.DisableTransportPathTransformations();
    init.MoveTo(enterPosition.GetPositionX(), enterPosition.GetPositionY(), enterPosition.GetPositionZ());
    init.SetFacing(enterPosition.GetOrientation());
    init.SetTransportEnter();
    init.Launch();

    if (Creature* creature = _me->ToCreature())
        if (creature->IsAIEnabled)
            creature->AI()->PassengerBoarded(unit, seat->first, true);

    if (Creature* creature = unit->ToCreature())
        if (creature->IsAIEnabled)
            creature->AI()->OnControlVehicle(_me, seat->first, true);

    if (GetBase()->GetTypeId() == TYPEID_UNIT)
        sScriptMgr->OnAddPassenger(this, unit, seatId);

    return true;
}

void Vehicle::RemovePassenger(Unit* unit)
{
    if (unit->GetVehicle() != this)
        return;

    SeatMap::iterator seat = GetSeatIteratorForPassenger(unit);
    if (seat == Seats.end())
        return;

    TC_LOG_DEBUG("entities.vehicle", "Unit %s exit vehicle entry %u id %u dbguid %u seat %d",
                 unit->GetName().c_str(), _me->GetEntry(), _vehicleInfo->m_ID, _me->GetGUIDLow(), (int32)seat->first);

    if(seat->second.SeatInfo->CanEnterOrExit() && ++_usableSeatNum)
        _me->SetFlag(UNIT_NPC_FLAGS, (_me->GetTypeId() == TYPEID_PLAYER ? UNIT_NPC_FLAG_PLAYER_VEHICLE : UNIT_NPC_FLAG_SPELLCLICK));

    // Remove UNIT_FLAG_NOT_SELECTABLE if passenger did not have it before entering vehicle
    if (seat->second.SeatInfo->m_flags & VEHICLE_SEAT_FLAG_PASSENGER_NOT_SELECTABLE /*&& !seat->second.Passenger.IsUnselectable*/)
        unit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

    seat->second.Passenger = 0;

    if (_me->GetTypeId() == TYPEID_UNIT && unit->GetTypeId() == TYPEID_PLAYER && seat->second.SeatInfo->m_flags & VEHICLE_SEAT_FLAG_CAN_CONTROL)
        _me->RemoveCharmedBy(unit);

    if (_me->IsInWorld())
    {
        unit->m_movementInfo.t_pos.Relocate(0, 0, 0, 0);
        unit->m_movementInfo.t_seat = -1;
        unit->m_movementInfo.t_time = 0;
    }

    // only for flyable vehicles
    if (unit->IsFlying() && unit->GetTypeId() == TYPEID_PLAYER && !unit->GetMap()->IsDungeon())
        _me->CastSpell(unit, VEHICLE_SPELL_PARACHUTE, true);

    if (_me->GetTypeId() == TYPEID_UNIT && _me->ToCreature()->IsAIEnabled)
        _me->ToCreature()->AI()->PassengerBoarded(unit, seat->first, false);

    if (Creature* creature = unit->ToCreature())
        if (creature->IsAIEnabled)
            creature->AI()->OnControlVehicle(_me, seat->first, false);

    if (GetBase()->GetTypeId() == TYPEID_UNIT)
        sScriptMgr->OnRemovePassenger(this, unit);
}

//! Must be called after m_base::Relocate
void Vehicle::RelocatePassengers()
{
    ASSERT(_me->GetMap());

    // not sure that absolute position calculation is correct, it must depend on vehicle pitch angle
    for (SeatMap::const_iterator itr = Seats.begin(); itr != Seats.end(); ++itr)
    {
        if (Unit* passenger = ObjectAccessor::GetUnit(*GetBase(), itr->second.Passenger))
        {
            ASSERT(passenger->IsInWorld());

            float px, py, pz, po;
            passenger->m_movementInfo.t_pos.GetPosition(px, py, pz, po);
            CalculatePassengerPosition(px, py, pz, po);
            passenger->UpdatePosition(px, py, pz, po);
        }
    }
}

void Vehicle::Dismiss()
{
    if (GetBase()->GetTypeId() != TYPEID_UNIT)
        return;

    TC_LOG_DEBUG("entities.vehicle", "Vehicle::Dismiss Entry: %u, GuidLow %u", _creatureEntry, _me->GetGUIDLow());
    Uninstall();
    GetBase()->ToCreature()->DespawnOrUnsummon();
}

bool Vehicle::IsVehicleInUse() const
{
    for (SeatMap::const_iterator itr = Seats.begin(); itr != Seats.end(); ++itr)
        if (itr->second.Passenger)
            return true;

    return false;
}

void Vehicle::InitMovementInfoForBase()
{
    uint32 vehicleFlags = GetVehicleInfo()->m_flags;

    if (vehicleFlags & VEHICLE_FLAG_NO_STRAFE)
        _me->AddExtraUnitMovementFlag(MOVEMENTFLAG2_NO_STRAFE);
    if (vehicleFlags & VEHICLE_FLAG_NO_JUMPING)
        _me->AddExtraUnitMovementFlag(MOVEMENTFLAG2_NO_JUMPING);
    if (vehicleFlags & VEHICLE_FLAG_FULLSPEEDTURNING)
        _me->AddExtraUnitMovementFlag(MOVEMENTFLAG2_FULL_SPEED_TURNING);
    if (vehicleFlags & VEHICLE_FLAG_ALLOW_PITCHING)
        _me->AddExtraUnitMovementFlag(MOVEMENTFLAG2_ALWAYS_ALLOW_PITCHING);
    if (vehicleFlags & VEHICLE_FLAG_FULLSPEEDPITCHING)
        _me->AddExtraUnitMovementFlag(MOVEMENTFLAG2_FULL_SPEED_PITCHING);
}

VehicleSeatEntry const* Vehicle::GetSeatForPassenger(Unit const* passenger)
{
    if (!passenger)
        return NULL;

    SeatMap::iterator itr;
    for (itr = Seats.begin(); itr != Seats.end(); ++itr)
        if (itr->second.Passenger == passenger->GetGUID())
            return itr->second.SeatInfo;

    return NULL;
}

SeatMap::iterator Vehicle::GetSeatIteratorForPassenger(Unit* passenger)
{
    SeatMap::iterator itr;
    for (itr = Seats.begin(); itr != Seats.end(); ++itr)
        if (itr->second.Passenger == passenger->GetGUID())
            return itr;

    return Seats.end();
}

uint8 Vehicle::GetAvailableSeatCount() const
{
    uint8 ret = 0;
    SeatMap::const_iterator itr;
    for (itr = Seats.begin(); itr != Seats.end(); ++itr)
        if (!itr->second.Passenger && (itr->second.SeatInfo->CanEnterOrExit() || itr->second.SeatInfo->IsUsableByOverride()))
            ++ret;

    return ret;
}

void Vehicle::CalculatePassengerPosition(float& x, float& y, float& z, float& o) const
{
    float inx = x, iny = y, inz = z, ino = o;
    o = GetBase()->GetOrientation() + ino;
    x = GetBase()->GetPositionX() + inx * std::cos(GetBase()->GetOrientation()) - iny * std::sin(GetBase()->GetOrientation());
    y = GetBase()->GetPositionY() + iny * std::cos(GetBase()->GetOrientation()) + inx * std::sin(GetBase()->GetOrientation());
    z = GetBase()->GetPositionZ() + inz;
}

void Vehicle::CalculatePassengerOffset(float& x, float& y, float& z, float& o) const
{
    o -= GetBase()->GetOrientation();
    z -= GetBase()->GetPositionZ();
    y -= GetBase()->GetPositionY();    // y = searchedY * std::cos(o) + searchedX * std::sin(o)
    x -= GetBase()->GetPositionX();    // x = searchedX * std::cos(o) + searchedY * std::sin(o + pi)
    float inx = x, iny = y;
    y = (iny - inx * tan(GetBase()->GetOrientation())) / (cos(GetBase()->GetOrientation()) + std::sin(GetBase()->GetOrientation()) * tan(GetBase()->GetOrientation()));
    x = (inx + iny * tan(GetBase()->GetOrientation())) / (cos(GetBase()->GetOrientation()) + std::sin(GetBase()->GetOrientation()) * tan(GetBase()->GetOrientation()));
}
