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

#ifndef TRINITY_GRIDNOTIFIERS_H
#define TRINITY_GRIDNOTIFIERS_H

#include "UpdateData.h"

#include "Corpse.h"
#include "AreaTrigger.h"
#include "DynamicObject.h"
#include "GameObject.h"
#include "Player.h"
#include "CreatureAI.h"
#include "Spell.h"

namespace Trinity
{
    struct VisibleNotifier
    {
        Player &i_player;
        UpdateData i_data;
        std::set<Unit*> i_visibleNow;
        Player::ClientGUIDs vis_guids;

        VisibleNotifier(Player &player) : i_player(player), i_data(player.GetMapId()), vis_guids(player.m_clientGUIDs) {}

        void SendToSelf();

        template <typename AnyMapType>
        void Visit(AnyMapType &m);
    };

    struct VisibleChangesNotifier
    {
        WorldObject &i_object;

        explicit VisibleChangesNotifier(WorldObject &object) : i_object(object) {}

        void Visit(PlayerMapType &);
        void Visit(CreatureMapType &);
        void Visit(DynamicObjectMapType &);

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    class AIRelocationNotifier
    {
    public:
        explicit AIRelocationNotifier(Unit &unit)
            : unit_(&unit)
        { }

        void Visit(CreatureMapType &);

        template <typename NotInterested>
        void Visit(NotInterested &) { }

    private:
        Unit *unit_;
        std::vector<std::pair<Creature *, bool>> movedInLos_;
    };

    struct MessageDistDeliverer
    {
        WorldObject* i_source;
        WorldPacket* i_message;
        uint32 i_phaseMask;
        float i_distSq;
        uint32 team;
        Player const* skipped_receiver;
        MessageDistDeliverer(WorldObject* src, WorldPacket* msg, float dist, bool own_team_only = false, Player const* skipped = NULL)
            : i_source(src), i_message(msg), i_phaseMask(src->GetPhaseMask()), i_distSq(dist * dist)
            , team((own_team_only && src->GetTypeId() == TYPEID_PLAYER) ? ((Player*)src)->GetTeam() : 0)
            , skipped_receiver(skipped)
        { }

        void Visit(PlayerMapType &m);
        void Visit(CreatureMapType &m);
        void Visit(DynamicObjectMapType &m);

        template <typename NotInterested>
        void Visit(NotInterested &) {}

        void SendPacket(Player* player)
        {
            // never send packet to self
            if (player == i_source || (team && player->GetTeam() != team) || skipped_receiver == player)
                return;

            if (!player->HaveAtClient(i_source))
                return;

            if (WorldSession* session = player->GetSession())
                session->SendPacket(i_message);
        }
    };

    struct UnfriendlyMessageDistDeliverer
    {
        Unit const *i_source;
        WorldPacket* i_message;
        uint32 i_phaseMask;
        float i_distSq;

        UnfriendlyMessageDistDeliverer(Unit const *src, WorldPacket* msg, float dist)
            : i_source(src), i_message(msg), i_phaseMask(src->GetPhaseMask()), i_distSq(dist * dist)
        { }

        void Visit(PlayerMapType &m);

        template <typename NotInterested>
        void Visit(NotInterested &) {}

        void SendPacket(Player* player)
        {
            // never send packet to self
            if (player == i_source || (player->IsFriendlyTo(i_source)))
                return;

            if (!player->HaveAtClient(i_source))
                return;

            if (WorldSession* session = player->GetSession())
                session->SendPacket(i_message);

            if (i_message->GetOpcode() == SMSG_CLEAR_TARGET)
            {
                for (uint32 i = CURRENT_MELEE_SPELL; i < CURRENT_AUTOREPEAT_SPELL; ++i)
                {
                    Spell * spell = player->GetCurrentSpell(i);
                    if (spell && spell->GetUnitTarget() == i_source && spell->getState() == SPELL_STATE_CASTING)
                        spell->cancel();
                }
            }
        }
    };

    // SEARCHERS & LIST SEARCHERS & WORKERS

    // WorldObject searchers & workers

    template<class Check>
    struct WorldObjectSearcher
    {
        uint32 i_mapTypeMask;
        uint32 i_phaseMask;
        WorldObject* &i_object;
        Check &i_check;

        WorldObjectSearcher(WorldObject const* searcher, WorldObject* & result, Check& check, uint32 mapTypeMask = GRID_MAP_TYPE_MASK_ALL)
            : i_mapTypeMask(mapTypeMask), i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check) {}

        void Visit(GameObjectMapType &m);
        void Visit(PlayerMapType &m);
        void Visit(CreatureMapType &m);
        void Visit(CorpseMapType &m);
        void Visit(DynamicObjectMapType &m);
        void Visit(AreaTriggerMapType &m);

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    template<class Check>
    struct WorldObjectLastSearcher
    {
        uint32 i_mapTypeMask;
        uint32 i_phaseMask;
        WorldObject* &i_object;
        Check &i_check;

        WorldObjectLastSearcher(WorldObject const* searcher, WorldObject* & result, Check& check, uint32 mapTypeMask = GRID_MAP_TYPE_MASK_ALL)
            :  i_mapTypeMask(mapTypeMask), i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check) {}

        void Visit(GameObjectMapType &m);
        void Visit(PlayerMapType &m);
        void Visit(CreatureMapType &m);
        void Visit(CorpseMapType &m);
        void Visit(DynamicObjectMapType &m);
        void Visit(AreaTriggerMapType &m);

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    template<class Check>
    struct WorldObjectListSearcher
    {
        uint32 i_mapTypeMask;
        uint32 i_phaseMask;
        std::list<WorldObject*> &i_objects;
        Check& i_check;

        WorldObjectListSearcher(WorldObject const* searcher, std::list<WorldObject*> &objects, Check & check, uint32 mapTypeMask = GRID_MAP_TYPE_MASK_ALL)
            : i_mapTypeMask(mapTypeMask), i_phaseMask(searcher->GetPhaseMask()), i_objects(objects), i_check(check) {}

        void Visit(PlayerMapType &m);
        void Visit(CreatureMapType &m);
        void Visit(CorpseMapType &m);
        void Visit(GameObjectMapType &m);
        void Visit(DynamicObjectMapType &m);
        void Visit(AreaTriggerMapType &m);

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    template<class Do>
    struct WorldObjectWorker
    {
        uint32 i_mapTypeMask;
        uint32 i_phaseMask;
        Do const& i_do;

        WorldObjectWorker(WorldObject const* searcher, Do const& _do, uint32 mapTypeMask = GRID_MAP_TYPE_MASK_ALL)
            : i_mapTypeMask(mapTypeMask), i_phaseMask(searcher->GetPhaseMask()), i_do(_do) {}

        void Visit(GameObjectMapType &m)
        {
            if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_GAMEOBJECT))
                return;

            for (auto &obj : m)
                if (obj->InSamePhase(i_phaseMask))
                    i_do(obj);
        }

        void Visit(PlayerMapType &m)
        {
            if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_PLAYER))
                return;

            for (auto &player : m)
                if (player->InSamePhase(i_phaseMask))
                    i_do(player);
        }

        void Visit(CreatureMapType &m)
        {
            if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_CREATURE))
                return;

            for (auto &creature : m)
                if (creature->InSamePhase(i_phaseMask))
                    i_do(creature);
        }

        void Visit(CorpseMapType &m)
        {
            if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_CORPSE))
                return;

            for (auto &corpse : m)
                if (corpse->InSamePhase(i_phaseMask))
                    i_do(corpse);
        }

        void Visit(DynamicObjectMapType &m)
        {
            if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_DYNAMICOBJECT))
                return;

            for (auto &obj : m)
                if (obj->InSamePhase(i_phaseMask))
                    i_do(obj);
        }

        void Visit(AreaTriggerMapType &m)
        {
            if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_AREATRIGGER))
                return;

            for (auto &trigger : m)
                if (trigger->InSamePhase(i_phaseMask))
                    i_do(trigger);
        }

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    // Gameobject searchers

    template<class Check>
    struct GameObjectSearcher
    {
        uint32 i_phaseMask;
        GameObject* &i_object;
        Check &i_check;

        GameObjectSearcher(WorldObject const* searcher, GameObject* & result, Check& check)
            : i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check) {}

        void Visit(GameObjectMapType &m);

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    // Last accepted by Check GO if any (Check can change requirements at each call)
    template<class Check>
    struct GameObjectLastSearcher
    {
        uint32 i_phaseMask;
        GameObject* &i_object;
        Check& i_check;

        GameObjectLastSearcher(WorldObject const* searcher, GameObject* & result, Check& check)
            : i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check) {}

        void Visit(GameObjectMapType &m);

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    template<class Check>
    struct GameObjectListSearcher
    {
        uint32 i_phaseMask;
        std::list<GameObject*> &i_objects;
        Check& i_check;

        GameObjectListSearcher(WorldObject const* searcher, std::list<GameObject*> &objects, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_objects(objects), i_check(check) {}

        void Visit(GameObjectMapType &m);

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    template<class Functor>
    struct GameObjectWorker
    {
        GameObjectWorker(WorldObject const* searcher, Functor& func)
            : _func(func), _phaseMask(searcher->GetPhaseMask()) {}

        void Visit(GameObjectMapType& m)
        {
            for (auto &obj : m)
                if (obj->InSamePhase(_phaseMask))
                    _func(obj);
        }

        template <typename NotInterested>
        void Visit(NotInterested &) {}

    private:
        Functor& _func;
        uint32 _phaseMask;
    };

    // Unit searchers

    // First accepted by Check Unit if any
    template<class Check>
    struct UnitSearcher
    {
        uint32 i_phaseMask;
        Unit* &i_object;
        Check & i_check;

        UnitSearcher(WorldObject const* searcher, Unit* & result, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check) {}

        void Visit(CreatureMapType &m);
        void Visit(PlayerMapType &m);

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    // Last accepted by Check Unit if any (Check can change requirements at each call)
    template<class Check>
    struct UnitLastSearcher
    {
        uint32 i_phaseMask;
        Unit* &i_object;
        Check & i_check;

        UnitLastSearcher(WorldObject const* searcher, Unit* & result, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check) {}

        void Visit(CreatureMapType &m);
        void Visit(PlayerMapType &m);

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    // All accepted by Check units if any
    template<class Check>
    struct UnitListSearcher
    {
        uint32 i_phaseMask;
        std::list<Unit*> &i_objects;
        Check& i_check;

        UnitListSearcher(WorldObject const* searcher, std::list<Unit*> &objects, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_objects(objects), i_check(check) {}

        void Visit(PlayerMapType &m);
        void Visit(CreatureMapType &m);

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    // Creature searchers

    template<class Check>
    struct CreatureSearcher
    {
        uint32 i_phaseMask;
        Creature* &i_object;
        Check & i_check;

        CreatureSearcher(WorldObject const* searcher, Creature* & result, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check) {}

        void Visit(CreatureMapType &m);

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    // Last accepted by Check Creature if any (Check can change requirements at each call)
    template<class Check>
    struct CreatureLastSearcher
    {
        uint32 i_phaseMask;
        Creature* &i_object;
        Check & i_check;

        CreatureLastSearcher(WorldObject const* searcher, Creature* & result, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check) {}

        void Visit(CreatureMapType &m);

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    template<class Check>
    struct CreatureListSearcher
    {
        uint32 i_phaseMask;
        std::list<Creature*> &i_objects;
        Check& i_check;

        CreatureListSearcher(WorldObject const* searcher, std::list<Creature*> &objects, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_objects(objects), i_check(check) {}

        void Visit(CreatureMapType &m);

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    template<class Do>
    struct CreatureWorker
    {
        uint32 i_phaseMask;
        Do& i_do;

        CreatureWorker(WorldObject const* searcher, Do& _do)
            : i_phaseMask(searcher->GetPhaseMask()), i_do(_do) {}

        void Visit(CreatureMapType &m)
        {
            for (auto &creature : m)
                if (creature->InSamePhase(i_phaseMask))
                    i_do(creature);
        }

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    // Player searchers

    template<class Check>
    struct PlayerSearcher
    {
        uint32 i_phaseMask;
        Player* &i_object;
        Check & i_check;

        PlayerSearcher(WorldObject const* searcher, Player* & result, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check) {}

        void Visit(PlayerMapType &m);

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    template<class Check>
    struct PlayerListSearcher
    {
        uint32 i_phaseMask;
        std::list<Player*> &i_objects;
        Check& i_check;

        PlayerListSearcher(WorldObject const* searcher, std::list<Player*> &objects, Check & check)
            : i_phaseMask(searcher->GetPhaseMask()), i_objects(objects), i_check(check) {}

        void Visit(PlayerMapType &m);

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    template<class Check>
    struct PlayerLastSearcher
    {
        uint32 i_phaseMask;
        Player* &i_object;
        Check& i_check;

        PlayerLastSearcher(WorldObject const* searcher, Player*& result, Check& check)
            : i_phaseMask(searcher->GetPhaseMask()), i_object(result), i_check(check)
        { }

        void Visit(PlayerMapType &m);

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    template<class Do>
    struct PlayerWorker
    {
        uint32 i_phaseMask;
        Do& i_do;

        PlayerWorker(WorldObject const* searcher, Do& _do)
            : i_phaseMask(searcher->GetPhaseMask()), i_do(_do) {}

        void Visit(PlayerMapType &m)
        {
            for (auto &player : m)
                if (player->InSamePhase(i_phaseMask))
                    i_do(player);
        }

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    template<class Do>
    struct PlayerDistWorker
    {
        WorldObject const* i_searcher;
        float i_dist;
        Do& i_do;

        PlayerDistWorker(WorldObject const* searcher, float _dist, Do& _do)
            : i_searcher(searcher), i_dist(_dist), i_do(_do) {}

        void Visit(PlayerMapType &m)
        {
            for (auto &player : m)
                if (player->InSamePhase(i_searcher) && player->IsWithinDist(i_searcher, i_dist))
                    i_do(player);
        }

        template <typename NotInterested>
        void Visit(NotInterested &) {}
    };

    // CHECKS && DO classes

    // WorldObject check classes

    class AnyDeadUnitObjectInRangeCheck
    {
        public:
            AnyDeadUnitObjectInRangeCheck(Unit* searchObj, float range) : i_searchObj(searchObj), i_range(range) {}
            bool operator()(Player* u);
            bool operator()(Corpse* u);
            bool operator()(Creature* u);
            template<class NOT_INTERESTED> bool operator()(NOT_INTERESTED*) { return false; }
        protected:
            Unit const* const i_searchObj;
            float i_range;
    };

    class AnyDeadUnitSpellTargetInRangeCheck : public AnyDeadUnitObjectInRangeCheck
    {
        public:
            AnyDeadUnitSpellTargetInRangeCheck(Unit* searchObj, float range, SpellInfo const* spellInfo, SpellTargetCheckTypes check)
                : AnyDeadUnitObjectInRangeCheck(searchObj, range), i_spellInfo(spellInfo), i_check(searchObj, searchObj, spellInfo, check, NULL)
            {}
            bool operator()(Player* u);
            bool operator()(Corpse* u);
            bool operator()(Creature* u);
            template<class NOT_INTERESTED> bool operator()(NOT_INTERESTED*) { return false; }
        protected:
            SpellInfo const* i_spellInfo;
            WorldObjectSpellTargetCheck i_check;
    };

    // WorldObject do classes

    class RespawnDo
    {
        public:
            RespawnDo() {}
            void operator()(Creature* u) const { u->Respawn(); }
            void operator()(GameObject* u) const { u->Respawn(); }
            void operator()(WorldObject*) const {}
            void operator()(Corpse*) const {}
    };

    // GameObject checks

    class GameObjectFocusCheck
    {
        public:
            GameObjectFocusCheck(Unit const* unit, uint32 focusId) : i_unit(unit), i_focusId(focusId) {}
            bool operator()(GameObject* go) const
            {
                if (go->GetGOInfo()->type != GAMEOBJECT_TYPE_SPELL_FOCUS)
                    return false;

                if (go->GetGOInfo()->spellFocus.focusId != i_focusId)
                    return false;

                float dist = (float)((go->GetGOInfo()->spellFocus.dist)/2);

                return go->IsWithinDistInMap(i_unit, dist);
            }
        private:
            Unit const* i_unit;
            uint32 i_focusId;
    };

    // Find the nearest Fishing hole and return true only if source object is in range of hole
    class NearestGameObjectFishingHole
    {
        public:
            NearestGameObjectFishingHole(WorldObject const& obj, float range) : i_obj(obj), i_range(range) {}
            bool operator()(GameObject* go)
            {
                if (go->GetGOInfo()->type == GAMEOBJECT_TYPE_FISHINGHOLE && go->isSpawned() && i_obj.IsWithinDistInMap(go, i_range) && i_obj.IsWithinDistInMap(go, (float)go->GetGOInfo()->fishinghole.radius))
                {
                    i_range = i_obj.GetDistance(go);
                    return true;
                }
                return false;
            }
            float GetLastRange() const { return i_range; }
        private:
            WorldObject const& i_obj;
            float  i_range;

            // prevent clone
            NearestGameObjectFishingHole(NearestGameObjectFishingHole const&);
    };

    class NearestGameObjectCheck
    {
        public:
            NearestGameObjectCheck(WorldObject const& obj) : i_obj(obj), i_range(999) {}
            bool operator()(GameObject* go)
            {
                if (i_obj.IsWithinDistInMap(go, i_range))
                {
                    i_range = i_obj.GetDistance(go);        // use found GO range as new range limit for next check
                    return true;
                }
                return false;
            }
            float GetLastRange() const { return i_range; }
        private:
            WorldObject const& i_obj;
            float i_range;

            // prevent clone this object
            NearestGameObjectCheck(NearestGameObjectCheck const&);
    };

    // Success at unit in range, range update for next check (this can be use with GameobjectLastSearcher to find nearest GO)
    class NearestGameObjectEntryInObjectRangeCheck
    {
        public:
            NearestGameObjectEntryInObjectRangeCheck(WorldObject const& obj, uint32 entry, float range) : i_obj(obj), i_entry(entry), i_range(range) {}
            bool operator()(GameObject* go)
            {
                if (go->GetEntry() == i_entry && i_obj.IsWithinDistInMap(go, i_range))
                {
                    i_range = i_obj.GetDistance(go);        // use found GO range as new range limit for next check
                    return true;
                }
                return false;
            }
            float GetLastRange() const { return i_range; }
        private:
            WorldObject const& i_obj;
            uint32 i_entry;
            float  i_range;

            // prevent clone this object
            NearestGameObjectEntryInObjectRangeCheck(NearestGameObjectEntryInObjectRangeCheck const&);
    };

    // Success at unit in range, range update for next check (this can be use with GameobjectLastSearcher to find nearest GO with a certain type)
    class NearestGameObjectTypeInObjectRangeCheck
    {
    public:
        NearestGameObjectTypeInObjectRangeCheck(WorldObject const& obj, GameobjectTypes type, float range) : i_obj(obj), i_type(type), i_range(range) {}
        bool operator()(GameObject* go)
        {
            if (go->GetGoType() == i_type && i_obj.IsWithinDistInMap(go, i_range))
            {
                i_range = i_obj.GetDistance(go);        // use found GO range as new range limit for next check
                return true;
            }
            return false;
        }
        float GetLastRange() const { return i_range; }
    private:
        WorldObject const& i_obj;
        GameobjectTypes i_type;
        float  i_range;

        // prevent clone this object
        NearestGameObjectTypeInObjectRangeCheck(NearestGameObjectTypeInObjectRangeCheck const&);

    };

    class GameObjectWithDbGUIDCheck
    {
        public:
            GameObjectWithDbGUIDCheck(uint32 db_guid) : i_db_guid(db_guid) {}
            bool operator()(GameObject const* go) const
            {
                return go->GetDBTableGUIDLow() == i_db_guid;
            }
        private:
            uint32 i_db_guid;
    };

    // Unit checks

    class MostHPMissingInRange
    {
        public:
            MostHPMissingInRange(Unit const* obj, float range, uint32 hp) : i_obj(obj), i_range(range), i_hp(hp) {}
            bool operator()(Unit* u)
            {
                if (u->IsAlive() && u->IsInCombat() && !i_obj->IsHostileTo(u) && i_obj->IsWithinDistInMap(u, i_range) && u->GetMaxHealth() - u->GetHealth() > i_hp)
                {
                    i_hp = u->GetMaxHealth() - u->GetHealth();
                    return true;
                }
                return false;
            }
        private:
            Unit const* i_obj;
            float i_range;
            uint32 i_hp;
    };

    class FriendlyCCedInRange
    {
        public:
            FriendlyCCedInRange(Unit const* obj, float range) : i_obj(obj), i_range(range) {}
            bool operator()(Unit* u)
            {
                if (u->IsAlive() && u->IsInCombat() && !i_obj->IsHostileTo(u) && i_obj->IsWithinDistInMap(u, i_range) &&
                    (u->isFeared() || u->isCharmed() || u->isFrozen() || u->HasUnitState(UNIT_STATE_STUNNED) || u->HasUnitState(UNIT_STATE_CONFUSED)))
                {
                    return true;
                }
                return false;
            }
        private:
            Unit const* i_obj;
            float i_range;
    };

    class FriendlyMissingBuffInRange
    {
        public:
            FriendlyMissingBuffInRange(Unit const* obj, float range, uint32 spellid) : i_obj(obj), i_range(range), i_spell(spellid) {}
            bool operator()(Unit* u)
            {
                if (u->IsAlive() && u->IsInCombat() && !i_obj->IsHostileTo(u) && i_obj->IsWithinDistInMap(u, i_range) &&
                    !(u->HasAura(i_spell)))
                {
                    return true;
                }
                return false;
            }
        private:
            Unit const* i_obj;
            float i_range;
            uint32 i_spell;
    };

    class AnyUnfriendlyUnitInObjectRangeCheck
    {
        public:
            AnyUnfriendlyUnitInObjectRangeCheck(WorldObject const* obj, Unit const* funit, float range) : i_obj(obj), i_funit(funit), i_range(range) {}
            bool operator()(Unit* u)
            {
                if (u->IsAlive() && i_obj->IsWithinDistInMap(u, i_range) && !i_funit->IsFriendlyTo(u))
                    return true;
                else
                    return false;
            }
        private:
            WorldObject const* i_obj;
            Unit const* i_funit;
            float i_range;
    };

    class AnyUnfriendlyNoTotemUnitInObjectRangeCheck
    {
        public:
            AnyUnfriendlyNoTotemUnitInObjectRangeCheck(WorldObject const* obj, Unit const* funit, float range) : i_obj(obj), i_funit(funit), i_range(range) {}
            bool operator()(Unit* u)
            {
                if (!u->IsAlive())
                    return false;

                if (u->GetCreatureType() == CREATURE_TYPE_NON_COMBAT_PET)
                    return false;

                if (u->GetTypeId() == TYPEID_UNIT && ((Creature*)u)->isTotem())
                    return false;

                if (!u->isTargetableForAttack(false))
                    return false;

                return i_obj->IsWithinDistInMap(u, i_range) && !i_funit->IsFriendlyTo(u);
            }
        private:
            WorldObject const* i_obj;
            Unit const* i_funit;
            float i_range;
    };

    class AnyUnfriendlyAttackableVisibleUnitInObjectRangeCheck
    {
        public:
            AnyUnfriendlyAttackableVisibleUnitInObjectRangeCheck(Unit const* funit, float range)
                : i_funit(funit), i_range(range) {}

            bool operator()(const Unit* u)
            {
                return u->IsAlive()
                    && i_funit->IsWithinDistInMap(u, i_range)
                    && !i_funit->IsFriendlyTo(u)
                    && i_funit->IsValidAttackTarget(u)
                    && u->GetCreatureType() != CREATURE_TYPE_CRITTER
                    && i_funit->canSeeOrDetect(u);
            }
        private:
            Unit const* i_funit;
            float i_range;
    };

    class CreatureWithDbGUIDCheck
    {
        public:
            CreatureWithDbGUIDCheck(uint32 lowguid) : i_lowguid(lowguid) {}
            bool operator()(Creature* u)
            {
                return u->GetDBTableGUIDLow() == i_lowguid;
            }
        private:
            uint32 i_lowguid;
    };

    class AnyFriendlyUnitInObjectRangeCheck
    {
        public:
            AnyFriendlyUnitInObjectRangeCheck(WorldObject const* obj, Unit const* funit, float range) : i_obj(obj), i_funit(funit), i_range(range) {}
            bool operator()(Unit* u)
            {
                if (u->IsAlive() && i_obj->IsWithinDistInMap(u, i_range) && i_funit->IsFriendlyTo(u))
                    return true;
                else
                    return false;
            }
        private:
            WorldObject const* i_obj;
            Unit const* i_funit;
            float i_range;
    };

    class AnyUnitHavingBuffInObjectRangeCheck
    {
        public:
            AnyUnitHavingBuffInObjectRangeCheck(WorldObject const* obj,
                                                Unit const* funit, float range,
                                                uint32 spellid, bool isfriendly)
                : i_obj(obj)
                , i_funit(funit)
                , i_range(range)
                , i_spellid(spellid)
                , i_friendly(isfriendly)
            { }

            bool operator()(Unit* u)
            {
                if (u->IsAlive() && i_obj->IsWithinDistInMap(u, i_range) && i_funit->IsFriendlyTo(u) == i_friendly && u->HasAura(i_spellid, i_obj->GetGUID()))
                    return true;
                else
                    return false;
            }

        private:
            WorldObject const* i_obj;
            Unit const* i_funit;
            float i_range;
            uint32 i_spellid;
            bool i_friendly;
    };

    class AnyGroupedUnitInObjectRangeCheck
    {
        public:
            AnyGroupedUnitInObjectRangeCheck(WorldObject const* obj, Unit const* funit, float range, bool raid) : _source(obj), _refUnit(funit), _range(range), _raid(raid) {}
            bool operator()(Unit* u)
            {
                if (_raid)
                {
                    if (!_refUnit->IsInRaidWith(u))
                        return false;
                }
                else if (!_refUnit->IsInPartyWith(u))
                    return false;

                return !_refUnit->IsHostileTo(u) && u->IsAlive() && _source->IsWithinDistInMap(u, _range);
            }

        private:
            WorldObject const* _source;
            Unit const* _refUnit;
            float _range;
            bool _raid;
    };

    class AnyUnitInObjectRangeCheck
    {
        public:
            AnyUnitInObjectRangeCheck(WorldObject const* obj, float range) : i_obj(obj), i_range(range) {}
            bool operator()(Unit* u)
            {
                if (u->IsAlive() && i_obj->IsWithinDistInMap(u, i_range))
                    return true;

                return false;
            }
        private:
            WorldObject const* i_obj;
            float i_range;
    };

    // Success at unit in range, range update for next check (this can be use with UnitLastSearcher to find nearest unit)
    class NearestAttackableUnitInObjectRangeCheck
    {
        public:
            NearestAttackableUnitInObjectRangeCheck(WorldObject const* obj, Unit const* funit, float range) : i_obj(obj), i_funit(funit), i_range(range) {}
            bool operator()(Unit* u)
            {
                if (u->isTargetableForAttack() && i_obj->IsWithinDistInMap(u, i_range) &&
                    !i_funit->IsFriendlyTo(u))
                {
                    i_range = i_obj->GetDistance(u);        // use found unit range as new range limit for next check
                    return true;
                }

                return false;
            }
        private:
            WorldObject const* i_obj;
            Unit const* i_funit;
            float i_range;

            // prevent clone this object
            NearestAttackableUnitInObjectRangeCheck(NearestAttackableUnitInObjectRangeCheck const&);
    };

    // Success at unit in range, range update for next check (this can be use with UnitLastSearcher to find nearest unit)
    class NearestAttackableNoCCUnitInObjectRangeCheck
    {
        public:
            NearestAttackableNoCCUnitInObjectRangeCheck(WorldObject const* obj, Unit const* funit, float range) : i_obj(obj), i_funit(funit), i_range(range) {}
            bool operator()(Unit* u)
            {
                if (u->isTargetableForAttack() && i_obj->IsWithinDistInMap(u, i_range) &&
                    !i_funit->IsFriendlyTo(u) && !u->HasCrowdControlAura() && !u->HasAuraType(SPELL_AURA_MOD_CONFUSE))
                {
                    i_range = i_obj->GetDistance(u);        // use found unit range as new range limit for next check
                    return true;
                }

                return false;
            }
        private:
            WorldObject const* i_obj;
            Unit const* i_funit;
            float i_range;

            // prevent clone this object
            NearestAttackableNoCCUnitInObjectRangeCheck(NearestAttackableNoCCUnitInObjectRangeCheck const&);
    };

    class AnyAoETargetUnitInObjectRangeCheck
    {
        public:
            AnyAoETargetUnitInObjectRangeCheck(WorldObject const* obj, Unit const* funit, float range)
                : i_obj(obj), i_funit(funit), _spellInfo(NULL), i_range(range)
            {
                Unit const* check = i_funit;
                Unit const* owner = i_funit->GetOwner();
                if (owner)
                    check = owner;
                i_targetForPlayer = (check->GetTypeId() == TYPEID_PLAYER);
                if (i_obj->GetTypeId() == TYPEID_DYNAMICOBJECT)
                    _spellInfo = sSpellMgr->GetSpellInfo(((DynamicObject*)i_obj)->GetSpellId());
            }
            bool operator()(Unit* u)
            {
                // Check contains checks for: live, non-selectable, non-attackable flags, flight check and GM check, ignore totems
                if (u->GetTypeId() == TYPEID_UNIT && ((Creature*)u)->isTotem())
                    return false;

                if (i_funit->_IsValidAttackTarget(u, _spellInfo,i_obj->GetTypeId() == TYPEID_DYNAMICOBJECT ? i_obj : NULL) && i_obj->IsWithinDistInMap(u, i_range))
                    return true;

                return false;
            }
        private:
            bool i_targetForPlayer;
            WorldObject const* i_obj;
            Unit const* i_funit;
            SpellInfo const* _spellInfo;
            float i_range;
    };

    // do attack at call of help to friendly crearture
    class CallOfHelpCreatureInRangeDo
    {
        public:
            CallOfHelpCreatureInRangeDo(Unit* funit, Unit* enemy, float range)
                : i_funit(funit), i_enemy(enemy), i_range(range)
            {}
            void operator()(Creature* u)
            {
                if (u == i_funit)
                    return;

                if (!u->CanAssistTo(i_funit, i_enemy, false))
                    return;

                // too far
                if (!u->IsWithinDistInMap(i_funit, i_range))
                    return;

                // only if see assisted creature's enemy
                if (!u->IsWithinLOSInMap(i_enemy))
                    return;

                if (u->AI())
                    u->AI()->AttackStart(i_enemy);
            }
        private:
            Unit* const i_funit;
            Unit* const i_enemy;
            float i_range;
    };

    struct AnyDeadUnitCheck
    {
        bool operator()(Unit* u) { return !u->IsAlive(); }
    };

    /*
    struct AnyStealthedCheck
    {
        bool operator()(Unit* u) { return u->GetVisibility() == VISIBILITY_GROUP_STEALTH; }
    };
    */

    // Creature checks

    class NearestHostileUnitCheck
    {
        public:
            explicit NearestHostileUnitCheck(Unit const* creature, float dist = 0) : me(creature)
            {
                m_range = (dist == 0 ? 9999 : dist);
            }
            bool operator()(Unit* u)
            {
                if (!me->IsWithinDistInMap(u, m_range))
                    return false;

                if (!me->IsValidAttackTarget(u))
                    return false;

                m_range = me->GetDistance(u);   // use found unit range as new range limit for next check
                return true;
            }

    private:
            Unit const* me;
            float m_range;
            NearestHostileUnitCheck(NearestHostileUnitCheck const&);
    };

    class NearestHostileNoCCUnitCheck
    {
    public:
        explicit NearestHostileNoCCUnitCheck(Creature const* creature, float dist = 0) : me(creature)
        {
            m_range = (dist == 0 ? 9999 : dist);
        }
        bool operator()(Unit* u)
        {
            if (!me->IsWithinDistInMap(u, m_range))
                return false;

            if (!me->IsValidAttackTarget(u))
                return false;

            if (u->HasCrowdControlAura())
                return false;

            if (u->HasAuraType(SPELL_AURA_MOD_CONFUSE))
                return false;

            m_range = me->GetDistance(u);   // use found unit range as new range limit for next check
            return true;
        }

    private:
        Creature const *me;
        float m_range;
        NearestHostileNoCCUnitCheck(NearestHostileNoCCUnitCheck const&);
    };

    class NearestHostileUnitInAttackDistanceCheck
    {
        public:
            explicit NearestHostileUnitInAttackDistanceCheck(Creature const* creature, float dist = 0) : me(creature)
            {
                m_range = (dist == 0 ? 9999 : dist);
                m_force = (dist == 0 ? false : true);
            }
            bool operator()(Unit* u)
            {
                if (!me->IsWithinDistInMap(u, m_range))
                    return false;

                if (!me->canSeeOrDetect(u))
                    return false;

                if (m_force)
                {
                    if (!me->IsValidAttackTarget(u))
                        return false;
                }
                else if (!me->canStartAttack(u, false))
                    return false;

                m_range = me->GetDistance(u);   // use found unit range as new range limit for next check
                return true;
            }
            float GetLastRange() const { return m_range; }
        private:
            Creature const* me;
            float m_range;
            bool m_force;
            NearestHostileUnitInAttackDistanceCheck(NearestHostileUnitInAttackDistanceCheck const&);
    };

    class NearestHostileUnitInAggroRangeCheck
    {
        public:
            explicit NearestHostileUnitInAggroRangeCheck(Creature const* creature, bool useLOS = false) : _me(creature), _useLOS(useLOS)
            {
            }
            bool operator()(Unit* u)
            {
                if (!u->IsHostileTo(_me))
                    return false;

                if (!u->IsWithinDistInMap(_me, _me->GetAggroRange(u)))
                    return false;

                if (!_me->IsValidAttackTarget(u))
                    return false;

                if (_useLOS && !u->IsWithinLOSInMap(_me))
                    return false;

                return true;
            }

    private:
            Creature const* _me;
            bool _useLOS;
            NearestHostileUnitInAggroRangeCheck(NearestHostileUnitInAggroRangeCheck const&);
    };

    class AnyAssistCreatureInRangeCheck
    {
        public:
            AnyAssistCreatureInRangeCheck(Unit* funit, Unit* enemy, float range)
                : i_funit(funit), i_enemy(enemy), i_range(range)
            {
            }
            bool operator()(Creature* u)
            {
                if (u == i_funit)
                    return false;

                if (!u->CanAssistTo(i_funit, i_enemy))
                    return false;

                // too far
                if (!i_funit->IsWithinDistInMap(u, i_range))
                    return false;

                // only if see assisted creature
                if (!i_funit->IsWithinLOSInMap(u))
                    return false;

                return true;
            }
        private:
            Unit* const i_funit;
            Unit* const i_enemy;
            float i_range;
    };

    class NearestAssistCreatureInCreatureRangeCheck
    {
        public:
            NearestAssistCreatureInCreatureRangeCheck(Creature* obj, Unit* enemy, float range)
                : i_obj(obj), i_enemy(enemy), i_range(range) {}

            bool operator()(Creature* u)
            {
                if (u == i_obj)
                    return false;
                if (!u->CanAssistTo(i_obj, i_enemy))
                    return false;

                if (!i_obj->IsWithinDistInMap(u, i_range))
                    return false;

                if (!i_obj->IsWithinLOSInMap(u))
                    return false;

                i_range = i_obj->GetDistance(u);            // use found unit range as new range limit for next check
                return true;
            }
            float GetLastRange() const { return i_range; }
        private:
            Creature* const i_obj;
            Unit* const i_enemy;
            float  i_range;

            // prevent clone this object
            NearestAssistCreatureInCreatureRangeCheck(NearestAssistCreatureInCreatureRangeCheck const&);
    };

    // Success at unit in range, range update for next check (this can be use with CreatureLastSearcher to find nearest creature)
    class NearestCreatureEntryWithLiveStateInObjectRangeCheck
    {
        public:
            NearestCreatureEntryWithLiveStateInObjectRangeCheck(WorldObject const& obj, uint32 entry, bool alive, float range)
                : i_obj(obj), i_entry(entry), i_alive(alive), i_range(range) {}

            bool operator()(Creature* u)
            {
                if (u->GetEntry() == i_entry && u->IsAlive() == i_alive && i_obj.IsWithinDistInMap(u, i_range))
                {
                    i_range = i_obj.GetDistance(u);         // use found unit range as new range limit for next check
                    return true;
                }
                return false;
            }
            float GetLastRange() const { return i_range; }
        private:
            WorldObject const& i_obj;
            uint32 i_entry;
            bool   i_alive;
            float  i_range;

            // prevent clone this object
            NearestCreatureEntryWithLiveStateInObjectRangeCheck(NearestCreatureEntryWithLiveStateInObjectRangeCheck const&);
    };

    class AnyPlayerInObjectRangeCheck
    {
        public:
            AnyPlayerInObjectRangeCheck(WorldObject const* obj, float range, bool reqAlive = true) : _obj(obj), _range(range), _reqAlive(reqAlive) {}
            bool operator()(Player* u)
            {
                if (_reqAlive && !u->IsAlive())
                    return false;

                if (!_obj->IsWithinDistInMap(u, _range))
                    return false;

                return true;
            }

        private:
            WorldObject const* _obj;
            float _range;
            bool _reqAlive;
    };

    class NearestPlayerInObjectRangeCheck
    {
        public:
            NearestPlayerInObjectRangeCheck(WorldObject const* obj, float range) : i_obj(obj), i_range(range)
            {
            }

            bool operator()(Player* u)
            {
                if (u->IsAlive() && i_obj->IsWithinDistInMap(u, i_range))
                {
                    i_range = i_obj->GetDistance(u);
                    return true;
                }

                return false;
            }
        private:
            WorldObject const* i_obj;
            float i_range;

            NearestPlayerInObjectRangeCheck(NearestPlayerInObjectRangeCheck const&);
    };

    class NearestPlayerNotGMInObjectRangeCheck
    {
        public:
            NearestPlayerNotGMInObjectRangeCheck(WorldObject const* obj, float range) : i_obj(obj), i_range(range)
            {
            }

            bool operator()(Player* u)
            {
                if (!u->isGameMaster() && u->IsAlive() && i_obj->IsWithinDistInMap(u, i_range))
                {
                    i_range = i_obj->GetDistance(u);
                    return true;
                }

                return false;
            }
        private:
            WorldObject const* i_obj;
            float i_range;

            NearestPlayerNotGMInObjectRangeCheck(NearestPlayerNotGMInObjectRangeCheck const&);
    };

    class AllFriendlyCreaturesInGrid
    {
    public:
        AllFriendlyCreaturesInGrid(Unit const* obj) : unit(obj) {}
        bool operator() (Unit* u)
        {
            if (u->IsAlive() && u->IsVisible() && u->IsFriendlyTo(unit))
                return true;

            return false;
        }
    private:
        Unit const* unit;
    };

    class AllGameObjectsWithEntryInRange
    {
    public:
        AllGameObjectsWithEntryInRange(const WorldObject* object, uint32 entry, float maxRange) : m_pObject(object), m_uiEntry(entry), m_fRange(maxRange) {}
        bool operator() (GameObject* go)
        {
            if (go->GetEntry() == m_uiEntry && m_pObject->IsWithinDist(go, m_fRange, false))
                return true;

            return false;
        }
    private:
        const WorldObject* m_pObject;
        uint32 m_uiEntry;
        float m_fRange;
    };

    class AllCreaturesOfEntryInRange
    {
        public:
            AllCreaturesOfEntryInRange(const WorldObject* object, uint32 entry, float maxRange) : m_pObject(object), m_uiEntry(entry), m_fRange(maxRange) {}
            bool operator() (Unit* unit)
            {
                if (unit->GetEntry() == m_uiEntry && m_pObject->IsWithinDist(unit, m_fRange, false))
                    return true;

                return false;
            }

        private:
            const WorldObject* m_pObject;
            uint32 m_uiEntry;
            float m_fRange;
    };


    class AllDeadCreaturesInRange
    {
        public:
            AllDeadCreaturesInRange(const WorldObject* object, float maxRange, uint64 excludeGUID)
                : m_pObject(object)
                , m_fRange(maxRange)
                , m_excludeGUID(excludeGUID)
            { }

            bool operator()(Unit const *unit) const
            {
                return unit->GetTypeId() == TYPEID_UNIT
                        && unit->GetGUID() != m_excludeGUID
                        && !unit->IsAlive()
                        && m_pObject->IsWithinDist(unit, m_fRange, false);
            }

        private:
            const WorldObject* m_pObject;
            float m_fRange;
            uint64 m_excludeGUID;
    };

    class PlayerAtMinimumRangeAway
    {
    public:
        PlayerAtMinimumRangeAway(Unit const* unit, float fMinRange) : unit(unit), fRange(fMinRange) {}
        bool operator() (Player* player)
        {
            //No threat list check, must be done explicit if expected to be in combat with creature
            if (!player->isGameMaster() && player->IsAlive() && !unit->IsWithinDist(player, fRange, false))
                return true;

            return false;
        }

    private:
        Unit const* unit;
        float fRange;
    };

    class GameObjectInRangeCheck
    {
    public:
        GameObjectInRangeCheck(float _x, float _y, float _z, float _range, uint32 _entry = 0) :
          x(_x), y(_y), z(_z), range(_range), entry(_entry) {}
        bool operator() (GameObject* go)
        {
            if (!entry || (go->GetGOInfo() && go->GetGOInfo()->entry == entry))
                return go->IsInRange(x, y, z, range);
            else return false;
        }
    private:
        float x, y, z, range;
        uint32 entry;
    };

    class AllWorldObjectsInRange
    {
    public:
        AllWorldObjectsInRange(const WorldObject* object, float maxRange) : m_pObject(object), m_fRange(maxRange) {}
        bool operator() (WorldObject* go)
        {
            return m_pObject->IsWithinDist(go, m_fRange, false) && m_pObject->InSamePhase(go);
        }
    private:
        const WorldObject* m_pObject;
        float m_fRange;
    };

    class ObjectTypeIdCheck
    {
        public:
            ObjectTypeIdCheck(TypeID typeId, bool equals) : _typeId(typeId), _equals(equals) {}
            bool operator()(WorldObject* object)
            {
                return (object->GetTypeId() == _typeId) == _equals;
            }

        private:
            TypeID _typeId;
            bool _equals;
    };

    class ObjectGUIDCheck
    {
        public:
            ObjectGUIDCheck(uint64 GUID) : _GUID(GUID) {}
            bool operator()(WorldObject* object)
            {
                return object->GetGUID() == _GUID;
            }

        private:
            uint64 _GUID;
    };

    class UnitAuraCheck
    {
        public:
            UnitAuraCheck(bool present, uint32 spellId, uint64 casterGUID = 0) : _present(present), _spellId(spellId), _casterGUID(casterGUID) {}
            bool operator()(Unit* unit) const
            {
                return unit->HasAura(_spellId, _casterGUID) == _present;
            }

            bool operator()(WorldObject* object) const
            {
                return object->ToUnit() && object->ToUnit()->HasAura(_spellId, _casterGUID) == _present;
            }

        private:
            bool _present;
            uint32 _spellId;
            uint64 _casterGUID;
    };

    class UnitAuraTypeCheck
    {
        public:
            UnitAuraTypeCheck(bool present, AuraType type) : _present(present), _type(type) {}
            bool operator()(Unit* unit) const
            {
                return unit->HasAuraType(_type) == _present;
            }

            bool operator()(WorldObject* object) const
            {
                return object->ToUnit() && object->ToUnit()->HasAuraType(_type) == _present;
            }

        private:
            bool _present;
            AuraType _type;
    };

    // Player checks and do

    // Prepare using Builder localized packets with caching and send to player
    template<class Builder>
    class LocalizedPacketDo
    {
        public:
            explicit LocalizedPacketDo(Builder& builder) : i_builder(builder) {}

            ~LocalizedPacketDo()
            {
                for (size_t i = 0; i < i_data_cache.size(); ++i)
                    delete i_data_cache[i];
            }
            void operator()(Player* p);

        private:
            Builder& i_builder;
            std::vector<WorldPacket*> i_data_cache;         // 0 = default, i => i-1 locale index
    };

    // Prepare using Builder localized packets with caching and send to player
    template<class Builder>
    class LocalizedPacketListDo
    {
        public:
            typedef std::vector<WorldPacket*> WorldPacketList;
            explicit LocalizedPacketListDo(Builder& builder) : i_builder(builder) {}

            ~LocalizedPacketListDo()
            {
                for (size_t i = 0; i < i_data_cache.size(); ++i)
                    for (size_t j = 0; j < i_data_cache[i].size(); ++j)
                        delete i_data_cache[i][j];
            }
            void operator()(Player* p);

        private:
            Builder& i_builder;
            std::vector<WorldPacketList> i_data_cache;
                                                            // 0 = default, i => i-1 locale index
    };

    class SummonTimerOrderPred
    {
        public:
            SummonTimerOrderPred(bool ascending = true) : m_ascending(ascending) {}
            bool operator() (const Unit* a, const Unit* b) const
            {
                if (!a->IsSummon() || !b->IsSummon())
                    return (urand(0, 1) ? false : true);

                uint32 rA = ((TempSummon*)a)->GetTimer();
                uint32 rB = ((TempSummon*)b)->GetTimer();
                return m_ascending ? rA < rB : rA > rB;
            }
        private:
            const bool m_ascending;
    };
}
#endif
