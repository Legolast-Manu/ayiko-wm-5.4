/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
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

#ifndef __BattleGroundKT_H
#define __BattleGroundKT_H

#include "Battleground.h"

enum BG_KT_NPC
{
    BG_SM_NPC_POWERBALL         = 29265
};

#define BG_KT_MAX_TEAM_SCORE        1600
#define BG_KT_ORB_POINTS_MAX        1600
#define BG_KT_POINTS_UPDATE_TIME    (5*IN_MILLISECONDS)
#define BG_KT_TIME_LIMIT            (25*MINUTE*IN_MILLISECONDS)
#define BG_KT_REMOVE_COL_TIME       (6.5*IN_MILLISECONDS)
#define BG_KT_EVENT_START_BATTLE    8563

enum BG_KT_Objects
{
    BG_KT_OBJECT_A_DOOR         = 0,
    BG_KT_OBJECT_H_DOOR         = 1,
    BG_KT_OBJECT_ORB_1          = 2,
    BG_KT_OBJECT_ORB_2          = 3,
    BG_KT_OBJECT_ORB_3          = 4,
    BG_KT_OBJECT_ORB_4          = 5,
    BG_KT_OBJECT_BERSERK_1      = 6,
    BG_KT_OBJECT_BERSERK_2      = 7,
    BG_KT_OBJECT_A_DOOR_COL     = 7,
    BG_KT_OBJECT_H_DOOR_COL     = 8,
    BG_KT_OBJECT_MAX            = 9
};

enum BG_KT_Creatures
{
    BG_KT_CREATURE_ORB_AURA_1   = 0,
    BG_KT_CREATURE_ORB_AURA_2   = 1,
    BG_KT_CREATURE_ORB_AURA_3   = 2,
    BG_KT_CREATURE_ORB_AURA_4   = 3,

    BG_KT_CREATURE_SPIRIT_1     = 4,
    BG_KT_CREATURE_SPIRIT_2     = 5,

    BG_KT_CREATURE_MAX          = 6
};

enum BG_KT_Objets_Entry
{
    BG_KT_OBJECT_DOOR_ENTRY     = 213172,

    BG_KT_OBJECT_ORB_1_ENTRY    = 212091,
    BG_KT_OBJECT_ORB_2_ENTRY    = 212092,
    BG_KT_OBJECT_ORB_3_ENTRY    = 212093,
    BG_KT_OBJECT_ORB_4_ENTRY    = 212094,
    BG_KT_OBJECT_BERSERK_1_ENTRY= 179905,
    BG_KT_OBJECT_BERSERK_2_ENTRY= 179907,

    BG_KT_OBJECT_DOOR_COL       = 180322
};

enum BG_KT_Sound
{
    BG_KT_SOUND_ORB_PLACED      = 8232,
    BG_KT_SOUND_A_ORB_PICKED_UP = 8174,
    BG_KT_SOUND_H_ORB_PICKED_UP = 8174,
    BG_KT_SOUND_ORB_RESPAWNED   = 8232
};

enum BG_KT_SpellId
{
    BG_KT_SPELL_ORB_PICKED_UP_1 = 121176,   
    BG_KT_SPELL_ORB_PICKED_UP_2 = 121175,   
    BG_KT_SPELL_ORB_PICKED_UP_3 = 121177,   
    BG_KT_SPELL_ORB_PICKED_UP_4 = 121164,   

    BG_KT_SPELL_ORB_AURA_1      = 121220,
    BG_KT_SPELL_ORB_AURA_2      = 121219,
    BG_KT_SPELL_ORB_AURA_3      = 121221,
    BG_KT_SPELL_ORB_AURA_4      = 121217,

    BG_KT_ALLIANCE_INSIGNIA     = 131527,
    BG_KT_HORDE_INSIGNIA        = 131528
};

enum BG_KT_WorldStates
{
    BG_KT_ICON_A                = 6308,
    BG_KT_ICON_H                = 6307,
    BG_KT_ORB_POINTS_A          = 6303,
    BG_KT_ORB_POINTS_H          = 6304,
    BG_KT_ORB_STATE             = 6309,
    BG_KT_TIME_ENABLED          = 4247,
    BG_KT_TIME_REMAINING        = 4248
};

enum BG_KT_Graveyards
{
    KT_GRAVEYARD_RECTANGLEA1    = 3552,
    KT_GRAVEYARD_RECTANGLEA2    = 4058,
    KT_GRAVEYARD_RECTANGLEH1    = 3553,
    KT_GRAVEYARD_RECTANGLEH2    = 4057
};

enum BG_KT_ZONE
{
    KT_ZONE_OUT                 = 0,
    KT_ZONE_IN                  = 1,
    KT_ZONE_MIDDLE              = 2,
    KT_KILLING_BLOW             = 3,
    KT_ZONE_MAX                 = 4
};


class BattleGroundKTScore : public BattlegroundScore
{
    public:
        BattleGroundKTScore() : OrbHandles(0), Score(0) {}
        virtual ~BattleGroundKTScore() {}
        uint32 OrbHandles;
        uint32 Score;
};

enum BG_TK_Events
{
    KT_EVENT_ORB                  = 0,
    // spiritguides will spawn (same moment, like TP_EVENT_DOOR_OPEN)
    KT_EVENT_SPIRITGUIDES_SPAWN   = 2
};

#define MAX_ORBS                    4

const float BG_KT_DoorPositions[2][4] =
{
    { 1783.84f, 1100.66f, 20.60f, 1.625020f },
    { 1780.15f, 1570.22f, 24.59f, 4.711630f }
};

const float BG_KT_DoorColPositions[2][4] =
{
    { 1783.84f, 1096.66f, 21.60f, 1.625020f },
    { 1780.15f, 1573.22f, 22.59f, 4.711630f }
};

const float BG_KT_OrbPositions[MAX_ORBS][4] =
{
    { 1716.78f, 1416.64f, 13.5709f, 1.57239f }, // GREEN
    { 1850.26f, 1416.77f, 13.5709f, 1.56061f }, // PURPLE
    { 1850.29f, 1250.31f, 13.5708f, 4.70848f }, // ORANGE
    { 1716.83f, 1249.93f, 13.5706f, 4.71397f }  // BLUE
};

const float BG_KT_SpiritPositions[MAX_ORBS][4] =
{
    { 1892.61f, 1151.69f, 14.7160f, 2.523528f },
    { 1672.40f, 1524.10f, 16.7387f, 6.032206f }
};

const uint32 BG_KT_ORBS_SPELLS[MAX_ORBS] =
{
    BG_KT_SPELL_ORB_PICKED_UP_1,
    BG_KT_SPELL_ORB_PICKED_UP_2,
    BG_KT_SPELL_ORB_PICKED_UP_3,
    BG_KT_SPELL_ORB_PICKED_UP_4
};

const uint32 BG_KT_ORBS_AURA[MAX_ORBS] =
{
    BG_KT_SPELL_ORB_AURA_1,
    BG_KT_SPELL_ORB_AURA_2,
    BG_KT_SPELL_ORB_AURA_3,
    BG_KT_SPELL_ORB_AURA_4
};

const uint32 BG_KT_ORBS_NAMES[MAX_ORBS] =
{
    LANG_BG_KT_ORB_GREEN,
    LANG_BG_KT_ORB_PURPLE,
    LANG_BG_KT_ORB_ORANGE,
    LANG_BG_KT_ORB_BLUE
};

//tick point according to which zone, last is for HK
const uint32 BG_KT_TickPoints[4] = { 3, 4, 5, 10};

class BattlegroundKT : public Battleground
{
    friend class BattleGroundMgr;

    public:
        /* Construction */
        BattlegroundKT();
        ~BattlegroundKT();
        void PostUpdateImpl(uint32 diff);

        /* inherited from BattlegroundClass */
        virtual void AddPlayer(Player* plr);
        virtual void StartingEventCloseDoors();
        virtual void StartingEventOpenDoors();

        /* Battleground Events */
        virtual void EventPlayerDroppedFlag(Player* player) { EventPlayerDroppedOrb(player, player->GetGUID(), player->GetTeam()); };
        void EventPlayerDroppedOrb(Player* source, uint64 guid, uint32 team);

        virtual void EventPlayerClickedOnFlag(Player* source, GameObject* target_obj) { EventPlayerClickedOnOrb(source, target_obj); }
        void EventPlayerClickedOnOrb(Player* source, GameObject* target_obj);

        void RemovePlayer(Player* plr, uint64 guid, uint32);
        void HandleAreaTrigger(Player* source, uint32 trigger);
        void HandleKillPlayer(Player* player, Player* killer);
        bool SetupBattleground();
        void Reset();
        virtual void EndBattleground(uint32 winner);
        WorldSafeLocsEntry const* GetClosestGraveYard(Player* player);
        uint32 GetRemainingTimeInMinutes() { return m_EndTimer ? (m_EndTimer-1) / (MINUTE * IN_MILLISECONDS) + 1 : 0; }

        void UpdateOrbState(Team team, uint32 value);
        void UpdateTeamScore(Team team);
        void UpdatePlayerScore(Player* Source, uint32 type, uint32 value, bool doAddHonor = true);
        virtual void FillInitialWorldStates(WorldPacket& data);

        /* Scorekeeping */
        uint32 GetTeamScore(Team team) const            { return m_TeamScores[GetTeamIndexByTeamId(team)]; }
        void AddPoint(Team team, uint32 Points = 1)     { m_TeamScores[GetTeamIndexByTeamId(team)] += Points; }
        void SetTeamPoint(Team team, uint32 Points = 0) { m_TeamScores[GetTeamIndexByTeamId(team)] = Points; }
        void RemovePoint(Team team, uint32 Points = 1)  { m_TeamScores[GetTeamIndexByTeamId(team)] -= Points; }

        void AccumulateScore(uint32 team, BG_KT_ZONE zone);
    private:

        uint64 m_OrbKeepers[MAX_ORBS];
        std::map<uint64, BG_KT_ZONE> m_playersZone;

        uint32 m_ReputationCapture;
        uint32 m_HonorWinKills;
        uint32 m_HonorEndKills;
        uint32 m_EndTimer;
        uint32 m_UpdatePointsTimer;
        uint32 m_RemoveColTimer;
        Team   m_LastCapturedOrbTeam;
};

#endif
