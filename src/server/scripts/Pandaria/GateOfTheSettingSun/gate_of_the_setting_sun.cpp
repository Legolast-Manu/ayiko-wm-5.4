/*
    Dungeon : Gate of the Setting Sun 90 Heroic
    Instance General Script
    */

#include "gate_of_the_setting_sun.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Vehicle.h"
#include "MoveSplineInit.h"
#include "GameObjectAI.h"

enum spells
{
    SPELL_EXPLOSE_GATE              = 115456,
    SPELL_BOMB_CAST_VISUAL          = 106729,
    SPELL_BOMB_AURA                 = 106875
};

class AggroEvent final : public BasicEvent
{
public:
    AggroEvent(Creature* creature) : _creature(creature) { }
private:
    bool Execute(uint64 /*time*/, uint32 /*diff*/) final
    {
        if(!_creature->IsInCombat())
            _creature->CastCustomSpell(43263, SPELLVALUE_MAX_TARGETS, 1);

        return true;
    }

    Creature* _creature;
};

struct gss_trash_generic_baseAI : public ScriptedAI
{
    gss_trash_generic_baseAI(Creature* creature) : ScriptedAI(creature), summons(me) {}

    float randHealth;
    EventMap events;
    SummonList summons;

    void InitializeAI() override
    {
        randHealth = frand(92.f, 96.f);
        me->SetReactState(REACT_AGGRESSIVE);
        me->setActive(true);

        Reset();
    }

    void Reset() override
    {
        me->m_Events.AddEvent(new AggroEvent(me), me->m_Events.CalculateTime(500));
    }

    void MoveInLineOfSight(Unit* who) override
    {
        if (who->GetTypeId() == TYPEID_PLAYER && me->IsWithinDistInMap(who, 15.0f) && !who->IsFriendlyTo(me))
        {
            if ((me->GetVictim() && me->GetVictim()->GetTypeId() != TYPEID_PLAYER) || !me->IsInCombat())
            {
                me->DeleteThreatList();
                me->getThreatManager().addThreat(who, 100.0f);
                AttackStart(who);
            }
        }
    }

    void DamageTaken(Unit* attacker, uint32 &damage) override
    {
        if (attacker->GetTypeId() == TYPEID_UNIT && !attacker->ToCreature()->isPet())
        {
            if (me->GetHealthPct() <= randHealth || me->GetHealth() <= damage)
            {
                damage = 0;
                me->getThreatManager().addThreat(attacker, 0.f);
            }
        }
    }

    void EnterEvadeMode() override
    {
        ScriptedAI::EnterEvadeMode();
        me->getThreatManager().resetAllAggro();
        me->DeleteThreatList();
    }
};

class mob_kirkthik_demolisher : public CreatureScript
{
public:
    mob_kirkthik_demolisher() : CreatureScript("mob_kirkthik_demolisher") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_kirkthik_demolisherAI(creature);
    }

    enum eEvents
    {
        EVENT_THROW_EXPLOSIVES = 1
    };

    enum eSpells
    {
        SPELL_THROW_EXPLOSIVES   = 107130,
        SPELL_VOLATILE_MUNITIONS = 109986
    };

    enum eNpc
    {
        NPC_VOLATILE_MUNITIONS = 56896
    };

    struct mob_kirkthik_demolisherAI : public gss_trash_generic_baseAI
    {
        mob_kirkthik_demolisherAI(Creature* creature) : gss_trash_generic_baseAI(creature) {}

        void Reset() override
        {
            events.Reset();
            events.ScheduleEvent(EVENT_THROW_EXPLOSIVES, urand(4, 8) * IN_MILLISECONDS);
        }

        void JustSummoned(Creature* summoned) override
        {
            if (summoned->GetEntry() == NPC_VOLATILE_MUNITIONS)
                summoned->CastSpell(summoned, SPELL_VOLATILE_MUNITIONS, false);

            summons.Summon(summoned);
        }

        void UpdateAI(const uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (events.ExecuteEvent() == EVENT_THROW_EXPLOSIVES)
            {
                me->CastSpell((Unit*)NULL, SPELL_THROW_EXPLOSIVES, false);
                events.ScheduleEvent(EVENT_THROW_EXPLOSIVES, urand(7, 10) * IN_MILLISECONDS);
            }

            DoMeleeAttackIfReady();
        }     
    };   
};

class mob_kirkthik_wind_shaper : public CreatureScript
{
public:
    mob_kirkthik_wind_shaper() : CreatureScript("mob_kirkthik_wind_shaper") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_kirkthik_wind_shaperAI(creature);
    }

    enum eEvents
    {
        EVENT_WIND_BLAST = 1,
        EVENT_GALE_FORCE = 2
    };

    enum eSpells
    {
        SPELL_WIND_BLAST = 115418,
        SPELL_GALE_FORCE = 115421
    };

    struct mob_kirkthik_wind_shaperAI : public gss_trash_generic_baseAI
    {
        mob_kirkthik_wind_shaperAI(Creature* creature) : gss_trash_generic_baseAI(creature) {}

        void Reset() override
        {
            events.Reset();
            events.ScheduleEvent(EVENT_WIND_BLAST, 2 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_GALE_FORCE, urand(8, 12) * IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_WIND_BLAST:
                        me->CastSpell((Unit*)NULL, SPELL_WIND_BLAST, false);
                        events.ScheduleEvent(EVENT_WIND_BLAST, 2 * IN_MILLISECONDS);
                        break;
                    case EVENT_GALE_FORCE:
                        me->CastSpell(me, SPELL_GALE_FORCE, false);
                        events.ScheduleEvent(EVENT_GALE_FORCE, urand(8, 12) * IN_MILLISECONDS);
                        break;
                }
            }
        }
    };
};

class mob_kirkthik_conscript : public CreatureScript
{
public:
    mob_kirkthik_conscript() : CreatureScript("mob_kirkthik_conscript") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_kirkthik_conscriptAI(creature);
    }

    enum eEvents
    {
        EVENT_RESIN_BURST   = 1,
        EVENT_RESIN_RESIDUE = 2
    };

    enum eSpells
    {
        SPELL_RESIN_BURST   = 118787,
        SPELL_RESIN_RESIDUE = 118795,
        SPELL_FENZY         = 118777
    };

    struct mob_kirkthik_conscriptAI : public gss_trash_generic_baseAI
    {
        mob_kirkthik_conscriptAI(Creature* creature) : gss_trash_generic_baseAI(creature) {}  

        void Reset() override
        {
            events.Reset();
            me->SetHealth(me->CountPctFromMaxHealth(30));
            events.ScheduleEvent(EVENT_RESIN_BURST, urand(7, 9) * IN_MILLISECONDS);
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (attacker->GetTypeId() == TYPEID_UNIT && !attacker->ToCreature()->isPet())
            {
                if (me->GetHealth() < me->GetMaxHealth() || me->GetHealth() <= damage)
                {
                    damage = 0;
                    me->getThreatManager().addThreat(attacker, 0.f);
                }
            }

            if (me->HealthBelowPctDamaged(15, damage) && !me->HasAura(SPELL_FENZY))
            {
                events.ScheduleEvent(EVENT_RESIN_RESIDUE, urand(30, 50)* IN_MILLISECONDS);
                me->CastSpell(me, SPELL_FENZY, false);
            }
        }

        void EnterEvadeMode() override
        {
            ScriptedAI::EnterEvadeMode();
            me->RemoveAllAuras();
        }

        void UpdateAI(const uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_RESIN_BURST:
                        me->CastSpell(me, SPELL_RESIN_BURST, false);
                        events.ScheduleEvent(EVENT_RESIN_BURST, urand(9, 14) * IN_MILLISECONDS);
                        break;
                    case EVENT_RESIN_RESIDUE:
                        me->CastSpell(me, SPELL_RESIN_RESIDUE, false);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

class mob_kirkthik_infiltrator : public CreatureScript
{
public:
    mob_kirkthik_infiltrator() : CreatureScript("mob_kirkthik_infiltrator") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_kirkthik_infiltratortAI(creature);
    }

    enum eEvents
    {
        EVENT_SMOKE_BOMB = 1
    };

    enum eSpells
    {
        SPELL_SMOKE_BOMB = 128829
    };

    struct mob_kirkthik_infiltratortAI : public gss_trash_generic_baseAI
    {
        mob_kirkthik_infiltratortAI(Creature* creature) : gss_trash_generic_baseAI(creature) {}

        void Reset() override
        {
           events.Reset();
           events.ScheduleEvent(EVENT_SMOKE_BOMB, urand(7, 13) * IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (events.ExecuteEvent() == EVENT_SMOKE_BOMB)
            {
                me->CastSpell(me, SPELL_SMOKE_BOMB, false);
                events.ScheduleEvent(EVENT_SMOKE_BOMB, urand(7, 13) * IN_MILLISECONDS);
            }

            DoMeleeAttackIfReady();
        }
    };
};

class mob_kirkthik_sapper : public CreatureScript
{
public:
    mob_kirkthik_sapper() : CreatureScript("mob_kirkthik_sapper") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_kirkthik_sapperAI(creature);
    }

    enum eEvents
    {
        EVENT_PICK_A_PLAYER = 1
    };

    enum eSpells
    {
        SPELL_PICK_A_PLAYER = 107097
    };

    struct mob_kirkthik_sapperAI : public gss_trash_generic_baseAI
    {
        mob_kirkthik_sapperAI(Creature* creature) : gss_trash_generic_baseAI(creature) {}

        void Reset() override
        {
            events.Reset();
            events.ScheduleEvent(EVENT_PICK_A_PLAYER, urand(7, 13) * IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (events.ExecuteEvent() == EVENT_PICK_A_PLAYER)
            {
                me->CastSpell(me, SPELL_PICK_A_PLAYER, false);
                events.ScheduleEvent(EVENT_PICK_A_PLAYER, urand(11, 17) * IN_MILLISECONDS);
            }

            DoMeleeAttackIfReady();
        }
    };
};

// All script in gss_trash_generic_baseAI
class mob_gss_trash_base : public CreatureScript
{
public:
    mob_gss_trash_base() : CreatureScript("mob_gss_trash_base") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_gss_trash_baseAI(creature);
    }

    struct mob_gss_trash_baseAI : public gss_trash_generic_baseAI
    {
        mob_gss_trash_baseAI(Creature* creature) : gss_trash_generic_baseAI(creature) {}
    };
};

class npc_krikthik_bombarder : public CreatureScript
{
public:
    npc_krikthik_bombarder() : CreatureScript("npc_krikthik_bombarder") { }

    struct npc_krikthik_bombarderAI : public ScriptedAI
    {
        npc_krikthik_bombarderAI(Creature* creature) : ScriptedAI(creature)
        {
            pInstance = creature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        uint32 bombTimer;

        void Reset() override
        {
            me->GetMotionMaster()->MoveRandom(5.0f);
            bombTimer = urand(1000, 7500);
        }

        // Called when spell hits a target
        void SpellHitTarget(Unit* target, SpellInfo const* /*spell*/) override
        {
            if (target->GetEntry() == NPC_BOMB_STALKER)
                me->AddAura(SPELL_BOMB_AURA, target);
        }

        void UpdateAI(const uint32 diff) override
        {
            if (bombTimer <= diff)
            {
                if (Unit* stalker = pInstance->instance->GetCreature(pInstance->GetData64(DATA_RANDOM_BOMB_STALKER)))
                    if (!stalker->HasAura(SPELL_BOMB_AURA))
                        me->CastSpell(stalker, SPELL_BOMB_CAST_VISUAL, true);

                bombTimer = urand(1000, 5000);
            }
            else bombTimer -= diff;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_krikthik_bombarderAI(creature);
    }
};

class go_setting_sun_brasier : public GameObjectScript
{
public:
    go_setting_sun_brasier() : GameObjectScript("go_setting_sun_brasier") { }

    enum eData
    {
        QUEST_LIGHTNING_THE_WAY = 31363,
        NPC_LTW_CREDIT          = 64553
    };

    bool OnGossipHello(Player* player, GameObject* go) override
    {
        if(InstanceScript* instance = player->GetInstanceScript())
        {
            if(instance->GetData(DATA_BRASIER_CLICKED) != DONE && instance->GetBossState(DATA_GADOK) == DONE)
            {
                instance->SetData(DATA_BRASIER_CLICKED, DONE);
                go->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE);

                Map::PlayerList const& playerList = go->GetMap()->GetPlayers();
                for(Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
                {
                    if(Player* player = i->GetSource())
                    {
                        if(player->GetQuestStatus(QUEST_LIGHTNING_THE_WAY) == QUEST_STATUS_INCOMPLETE)
                            player->KilledMonsterCredit(NPC_LTW_CREDIT);

                        player->SendCinematicStart(CINEMATIC_SETTING_SUN);
                        player->NearTeleportTo(1370.0f, 2283.6f, 402.328f, 2.70f);
                    }
                }

                if(Creature* rimok = Unit::GetCreature(*go, instance->GetData64(DATA_RIMOK)))
                {
                    rimok->SetPhaseMask(1, true);
                    rimok->AI()->Talk(2);
                }

                if(GameObject* signalFire = ObjectAccessor::GetGameObject(*go, instance->GetData64(DATA_SIGNAL_FIRE)))
                    signalFire->SetGoState(GO_STATE_ACTIVE);

                for(int i = 0; i < 4; ++i)
                {
                    if(Creature* krithik = instance->instance->SummonCreature(NPC_KRITHIK_INFILTRATOR, SummonPositionsRimok[i]))
                        krithik->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2H);
                }

                for(int i = 4; i < 6; ++i)
                {
                    if(Creature* krithik = instance->instance->SummonCreature(NPC_KRITHIK_WND_SHAPER, SummonPositionsRimok[i]))
                        krithik->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2H);
                }

                for(int i = 6; i < 8; ++i)
                {
                    if(Creature* krithik = instance->instance->SummonCreature(NPC_KRITHIK_SHAPER, SummonPositionsRimok[i]))
                        krithik->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2H);
                }

                return false;
            }
        }

        return true;
    }

    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_setting_sun_brasierAI(go);
    }

    struct go_setting_sun_brasierAI : public GameObjectAI
    {
        go_setting_sun_brasierAI(GameObject* go) : GameObjectAI(go)
        {
            go->setActive(true);

            if(InstanceScript* instance = go->GetInstanceScript())
            {
                if(instance->GetBossState(DATA_GADOK) == DONE && instance->GetData(DATA_BRASIER_CLICKED) == DONE)
                {
                    go->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE);

                    if(GameObject* signalFire = ObjectAccessor::GetGameObject(*go, instance->GetData64(DATA_SIGNAL_FIRE)))
                        signalFire->SetGoState(GO_STATE_ACTIVE);

                    if(Creature* rimok = Unit::GetCreature(*go, instance->GetData64(DATA_RIMOK)))
                        rimok->SetPhaseMask(1, true);

                    if(instance->GetBossState(DATA_RIMOK) != DONE)
                    {
                        for(int i = 0; i < 4; ++i)
                        {
                            if(Creature* krithik = instance->instance->SummonCreature(NPC_KRITHIK_INFILTRATOR, SummonPositionsRimok[i]))
                                krithik->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2H);
                        }

                        for(int i = 4; i < 6; ++i)
                        {
                            if(Creature* krithik = instance->instance->SummonCreature(NPC_KRITHIK_WND_SHAPER, SummonPositionsRimok[i]))
                                krithik->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2H);
                        }

                        for(int i = 6; i < 8; ++i)
                        {
                            if(Creature* krithik = instance->instance->SummonCreature(NPC_KRITHIK_SHAPER, SummonPositionsRimok[i]))
                                krithik->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2H);
                        }
                    }
                }
            }
        }
    };
};

class vehicle_artillery_to_wall : public VehicleScript
{
public:
    vehicle_artillery_to_wall() : VehicleScript("vehicle_artillery_to_wall") {}

    enum eSpells
    {
        SPELL_JUMP_TO_WALL = 131497
    };

    enum eEvents
    {
        EVENT_JUMP_TO_WALL = 1
    };

    struct vehicle_artillery_to_wallAI : public ScriptedAI
    {
        vehicle_artillery_to_wallAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;

        void PassengerBoarded(Unit* who, int8 seatId, bool apply) override
        {
            if (apply)
                events.ScheduleEvent(EVENT_JUMP_TO_WALL, 1 * IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 diff) override
        {
            events.Update(diff);

            if (events.ExecuteEvent() == EVENT_JUMP_TO_WALL)
            {
                if (Vehicle* vehicle = me->GetVehicleKit())
                    if (Unit* passenger = vehicle->GetPassenger(0))
                        if (Player* player = passenger->ToPlayer())
                        {
                            player->ExitVehicle();
                            player->CastSpell(player, SPELL_JUMP_TO_WALL, false);
                        }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new vehicle_artillery_to_wallAI(creature);
    }
};

class npc_serpents_spine_marksman : public CreatureScript
{
public:
    npc_serpents_spine_marksman() : CreatureScript("npc_serpents_spine_marksman") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_serpents_spine_marksmanAI(creature);
    }

    enum eEvents
    {
        EVENT_MISSILE     = 1,
        EVENT_KILL_MANTID = 2
    };

    enum eSpells
    {
        SPELL_MISSILE     = 106202,
        SPELL_KILL_MANTID = 115146
    };

    struct npc_serpents_spine_marksmanAI : public ScriptedAI
    {
        npc_serpents_spine_marksmanAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;

        void InitializeAI() override
        {
            me->setActive(true);
            events.ScheduleEvent(EVENT_MISSILE, 1 * IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 diff) override
        {
            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_MISSILE:
                        me->CastSpell(me, SPELL_MISSILE, false);
                        events.ScheduleEvent(urand(EVENT_MISSILE, EVENT_KILL_MANTID), 2 * IN_MILLISECONDS);
                        break;
                    case EVENT_KILL_MANTID:
                    {
                        std::list<Creature*> spitters;
                        me->GetCreatureListWithEntryInGrid(spitters, NPC_KRITHIK_GLIDER, 105.0f);
                        if(!spitters.empty())
                            if(auto creature = Trinity::Containers::SelectRandomContainerElement(spitters))
                                me->CastSpell(creature, SPELL_KILL_MANTID, false);

                        events.ScheduleEvent(urand(EVENT_MISSILE, EVENT_KILL_MANTID), 2 * IN_MILLISECONDS);
                    }
                    break;

                }
            }
        }
    };    
};

class npc_kirthik_glider : public CreatureScript
{
public:
    npc_kirthik_glider() : CreatureScript("npc_kirthik_glider") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_kirthik_gliderAI(creature);
    }

    struct npc_kirthik_gliderAI : public ScriptedAI
    {
        npc_kirthik_gliderAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;

        void InitializeAI() override
        {
            me->setActive(true);
            float x, y;
            GetPositionWithDistInOrientation(me, 130.0f, 4.743f, x, y);
            Movement::MoveSplineInit init(me);
            init.MoveTo(x, y, me->GetPositionZ());
            init.SetFly();
            init.SetVelocity(10.0f);
            init.Launch();

            me->DespawnOrUnsummon(me->GetSplineDuration());
        }

        void JustDied(Unit* killer) override
        {
            me->StopMoving();
            me->DespawnOrUnsummon();
        }
    };
};

class npc_great_wall_explosion_target_stalker : public CreatureScript
{
public:
    npc_great_wall_explosion_target_stalker() : CreatureScript("npc_great_wall_explosion_target_stalker") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_great_wall_explosion_target_stalkerAI(creature);
    }

    enum eSpells
    {
        SPELL_DESTROY_WALL_CORNER_A = 124993,
        SPELL_DESTROY_WALL_CORNER_B = 114906,
        SPELL_BOULDER               = 114971
    };

    enum eActions
    {
        ACTION_DESTROY_WALL_CORNER_A = 1,
        ACTION_DESTROY_WALL_CORNER_B = 2
    };

    enum eEvents
    {
        EVENT_DESTROY = 1
    };

    struct npc_great_wall_explosion_target_stalkerAI : public ScriptedAI
    {
        npc_great_wall_explosion_target_stalkerAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        bool cornerAdestroyed, cornerBdestroyed;

        void InitializeAI() override
        {
            cornerAdestroyed = false;
            cornerBdestroyed = false;
        }

        void DoAction(int32 const action) override
        {
            switch (action)
            {
                case ACTION_DESTROY_WALL_CORNER_A:
                    {
                        if(!cornerAdestroyed)
                        {
                            cornerAdestroyed = true;
                            if (Creature* caster = me->FindNearestCreature(NPC_EXPLOSION_CASTER, 100.0f))
                            {
                                caster->CastSpell(me, SPELL_DESTROY_WALL_CORNER_A, false);
                                caster->CastSpell(me, SPELL_BOULDER, false);
                            }
                        }
                    }
                    break;
                case ACTION_DESTROY_WALL_CORNER_B:
                    {
                        if(!cornerBdestroyed)
                        {
                            cornerBdestroyed = true;
                            if (Creature* caster = me->FindNearestCreature(NPC_EXPLOSION_CASTER, 100.0f))
                                caster->CastSpell(me, SPELL_DESTROY_WALL_CORNER_B, false);

                            events.ScheduleEvent(EVENT_DESTROY, 2.6 * IN_MILLISECONDS);
                        }
                    }
                    break;
            }
        }

        void UpdateAI(const uint32 diff) override
        {
            events.Update(diff);

            if(events.ExecuteEvent() == EVENT_DESTROY)
            {
                if(instance)
                    instance->SetData(DATA_CORNER_B, DONE);
            }
        }
    };
};

const float TraineeWPPath[7][3] =
{
    { 1233.126f, 2304.802f, 388.1548f },
    { 1240.626f, 2304.552f, 388.1548f },
    { 1248.126f, 2304.552f, 383.6548f },
    { 1253.126f, 2304.302f, 381.6548f },
    { 1259.126f, 2299.802f, 381.9048f },
    { 1264.626f, 2309.052f, 381.9048f },
    { 1271.626f, 2298.802f, 382.1548f }
};

class npc_great_wall_before_gadok_intro : public CreatureScript
{
public:
    npc_great_wall_before_gadok_intro() : CreatureScript("npc_great_wall_before_gadok_intro") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_great_wall_before_gadok_introAI(creature);
    }

    enum eActions
    {
        ACTION_TALK_INTRO = 1,
        ACTION_CANNONEER  = 2
    };

    enum eTalks
    {
        TALK_INTRO = 0
    };

    enum eEvents
    {
        EVENT_TALK_INTRO = 1,
        EVENT_MOVE       = 2,
        EVENT_CANN_1     = 3, //left
        EVENT_CANN_2     = 4, //left
        EVENT_CANN_3     = 5,
        EVENT_CANN_4     = 6
    };

    struct npc_great_wall_before_gadok_introAI : public ScriptedAI
    {
        npc_great_wall_before_gadok_introAI(Creature* creature) : ScriptedAI(creature) {}

        bool introTalked;

        EventMap events;

        void InitializeAI() override
        {
            introTalked = false;
            if (me->GetEntry() == NPC_SERPENTS_SPINE_DEF_A && me->GetInstanceScript() && me->GetInstanceScript()->GetBossState(DATA_RIMOK) == DONE)
            {
                events.ScheduleEvent(EVENT_CANN_1, urand(2, 8) * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_CANN_3, urand(2, 8) * IN_MILLISECONDS);
            }
        }

        void DoAction(int32 const action) override
        {
            if (action == ACTION_TALK_INTRO)
            {
                if (!introTalked)
                {
                    introTalked = true;
                    switch (me->GetEntry())
                    {
                        case NPC_SERPENTS_SPINE_DEF_A:
                            events.ScheduleEvent(EVENT_TALK_INTRO, 3 * IN_MILLISECONDS);
                            break;
                        case NPC_SERPENTS_SPINE_DEF_B:
                            events.ScheduleEvent(EVENT_TALK_INTRO, 8 * IN_MILLISECONDS);
                            break;
                        case NPC_SERPENTS_SPINE_TRAINEE:
                            events.ScheduleEvent(EVENT_TALK_INTRO, 12 * IN_MILLISECONDS);
                            events.ScheduleEvent(EVENT_MOVE, 14 * IN_MILLISECONDS);
                            break;
                    }
                }
            }
            else if (action == ACTION_CANNONEER)
            {
                events.ScheduleEvent(EVENT_CANN_1, urand(2, 8) * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_CANN_3, urand(2, 8) * IN_MILLISECONDS);
            }
        }

        void UpdateAI(const uint32 diff) override
        {
            events.Update(diff);

            switch (events.ExecuteEvent())
            {
                case EVENT_TALK_INTRO:
                    Talk(TALK_INTRO);
                    break;
                case EVENT_MOVE:
                   {
                       Movement::MoveSplineInit init(me);
                       for (uint8 i = 0; i < 7; ++i)
                       {
                           G3D::Vector3 path(TraineeWPPath[i][0], TraineeWPPath[i][1], TraineeWPPath[i][2]);
                           init.Path().push_back(path);
                       }
                       init.SetSmooth();
                       init.SetVelocity(10.0f);
                       init.Launch();

                       me->DespawnOrUnsummon(me->GetSplineDuration());
                    }
                    break;
                case EVENT_CANN_1:
                    if (Creature* cann = me->SummonCreature(NPC_CANNONEER, PandarenCannoneerPos[0]))
                        if(cann->IsAIEnabled)
                            cann->AI()->DoAction(1);

                        switch (urand(0,1))
                        {
                            case 0:
                                events.ScheduleEvent(EVENT_CANN_1, urand(3, 6) * IN_MILLISECONDS);
                                break;
                            case 1:
                                events.ScheduleEvent(EVENT_CANN_2, urand(3, 6) * IN_MILLISECONDS);
                                break;
                        }
                        break;
                case EVENT_CANN_2:
                    if (Creature* cann = me->SummonCreature(NPC_CANNONEER, PandarenCannoneerPos[1]))
                        if(cann->IsAIEnabled)
                            cann->AI()->DoAction(1);
                        
                        switch (urand(0, 1))
                        {
                            case 0:
                                events.ScheduleEvent(EVENT_CANN_1, urand(3, 6) * IN_MILLISECONDS);
                                break;
                            case 1:
                                events.ScheduleEvent(EVENT_CANN_2, urand(3, 6) * IN_MILLISECONDS);
                                break;
                        }
                    break;
                case EVENT_CANN_3:
                    if (Creature* cann = me->SummonCreature(NPC_CANNONEER, PandarenCannoneerPos[2]))
                        if(cann->IsAIEnabled)
                            cann->AI()->DoAction(2);
                        
                        switch (urand(0, 1))
                        {
                            case 0:
                                events.ScheduleEvent(EVENT_CANN_3, urand(3, 6) * IN_MILLISECONDS);
                                break;
                            case 1:
                                events.ScheduleEvent(EVENT_CANN_4, urand(3, 6) * IN_MILLISECONDS);
                                break;
                        }
                    break;
                case EVENT_CANN_4:
                    if (Creature* cann = me->SummonCreature(NPC_CANNONEER, PandarenCannoneerPos[3]))
                        if(cann->IsAIEnabled)
                            cann->AI()->DoAction(2);
                        
                        switch (urand(0, 1))
                        {
                            case 0:
                                events.ScheduleEvent(EVENT_CANN_3, urand(3, 6) * IN_MILLISECONDS);
                                break;
                            case 1:
                                events.ScheduleEvent(EVENT_CANN_4, urand(3, 6) * IN_MILLISECONDS);
                                break;
                        }
                    break;
            }
        }
    };
};

class npc_gss_pandaren_cannoneer : public CreatureScript
{
public:
    npc_gss_pandaren_cannoneer() : CreatureScript("npc_gss_pandaren_cannoneer") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_gss_pandaren_cannoneerAI(creature);
    }

    enum eActions
    {
        ACTION_START_MOVEMTNT_LEFT = 1
    };

    enum eEvents
    {
        EVENT_LEFT_MOVE_BEFORE_JUMP  = 1,
        EVENT_LEFT_MOVE_AFTER_JUMP   = 2,
        EVENT_LEFT_JUMP_1            = 3,
        EVENT_LEFT_JUMP_2            = 4,
        EVENT_RIGHT_MOVE_BEFORE_JUMP = 5,
        EVENT_RIGHT_MOVE_AFTER_JUMP  = 6,
        EVENT_RIGHT_JUMP_1           = 7,
        EVENT_RIGHT_JUMP_2           = 8,
        EVENT_RIGHT_BEFORE_JUMP      = 9
    };

    struct npc_gss_pandaren_cannoneerAI : public ScriptedAI
    {
        npc_gss_pandaren_cannoneerAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;

        void InitializeAI() override
        {
            me->setActive(true);
        }

        void DoAction(int32 const action) override
        {
            if (action == ACTION_START_MOVEMTNT_LEFT)
                events.ScheduleEvent(EVENT_LEFT_MOVE_BEFORE_JUMP, 2 * IN_MILLISECONDS);
            else
                events.ScheduleEvent(EVENT_RIGHT_MOVE_BEFORE_JUMP, 2 * IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 diff) override
        {
            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_LEFT_MOVE_BEFORE_JUMP:
                        {
                          Movement::MoveSplineInit init(me);
                          for (uint8 i = 0; i < 10; ++i)
                          {
                              G3D::Vector3 path(LeftCannoneerWPPath[i][0], LeftCannoneerWPPath[i][1], LeftCannoneerWPPath[i][2]);
                              init.Path().push_back(path);
                          }
                          init.SetSmooth();
                          init.SetVelocity(10.0f);
                          init.Launch();
                          events.ScheduleEvent(EVENT_LEFT_JUMP_1, me->GetSplineDuration());
                        }
                        break;
                    case EVENT_LEFT_JUMP_1:
                        me->GetMotionMaster()->MoveJump(1085.26f, 2304.05f, 361.189f, 15.0f, 15.f);
                        events.ScheduleEvent(EVENT_LEFT_JUMP_2, me->GetSplineDuration());
                        break;
                    case EVENT_LEFT_JUMP_2:
                        me->GetMotionMaster()->MoveJump(1090.68f, 2315.01f, 330.808f, 15.0f, 21.f);
                        events.ScheduleEvent(EVENT_LEFT_MOVE_AFTER_JUMP, me->GetSplineDuration());
                        break;
                    case EVENT_LEFT_MOVE_AFTER_JUMP:
                        {
                          Movement::MoveSplineInit init(me);
                          for (uint8 i = 11; i < 24; ++i)
                          {
                              G3D::Vector3 path(LeftCannoneerWPPath[i][0], LeftCannoneerWPPath[i][1], LeftCannoneerWPPath[i][2]);
                              init.Path().push_back(path);
                          }
                          init.SetSmooth();
                          init.SetVelocity(10.0f);
                          init.Launch();
                          me->DespawnOrUnsummon(me->GetSplineDuration());
                        }
                        break;
                    case EVENT_RIGHT_MOVE_BEFORE_JUMP:
                        {
                          Movement::MoveSplineInit init(me);
                          for (uint8 i = 0; i < 9; ++i)
                          {
                              G3D::Vector3 path(RightCannoneerWPPath[i][0], RightCannoneerWPPath[i][1], RightCannoneerWPPath[i][2]);
                              init.Path().push_back(path);
                          }
                          init.SetSmooth();
                          init.SetVelocity(10.0f);
                          init.Launch();
                          events.ScheduleEvent(EVENT_RIGHT_JUMP_1, me->GetSplineDuration());
                        }
                        break;
                    case EVENT_RIGHT_JUMP_1:
                        me->GetMotionMaster()->MoveJump(1079.03f, 2298.7f, 364.219f, 15.0f, 17.f);
                        events.ScheduleEvent(EVENT_RIGHT_BEFORE_JUMP, me->GetSplineDuration());
                        break;
                    case EVENT_RIGHT_BEFORE_JUMP:
                        {
                          Movement::MoveSplineInit init(me);
                          init.MoveTo(1084.88f, 2307.34f, 359.1696f);
                          init.SetVelocity(10.0f);
                          init.Launch();
                          events.ScheduleEvent(EVENT_RIGHT_JUMP_2, me->GetSplineDuration());
                        }
                        break;
                    case EVENT_RIGHT_JUMP_2:
                        me->GetMotionMaster()->MoveJump(1093.57f, 2310.65f, 330.807f, 15.0f, 21.f);
                        events.ScheduleEvent(EVENT_RIGHT_MOVE_AFTER_JUMP, me->GetSplineDuration());
                        break;
                    case EVENT_RIGHT_MOVE_AFTER_JUMP:
                        {
                          Movement::MoveSplineInit init(me);
                          for (uint8 i = 10; i < 27; ++i)
                          {
                              G3D::Vector3 path(RightCannoneerWPPath[i][0], RightCannoneerWPPath[i][1], RightCannoneerWPPath[i][2]);
                              init.Path().push_back(path);
                          }
                          init.SetSmooth();
                          init.SetVelocity(10.0f);
                          init.Launch();
                          me->DespawnOrUnsummon(me->GetSplineDuration());
                        }
                        break;
                }
            }
        }
    };
};

class npc_gss_rope_seat : public CreatureScript
{
public:
    npc_gss_rope_seat() : CreatureScript("npc_gss_rope_seat") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_gss_rope_seatAI(creature);
    }

    enum eEvents
    {
        EVENT_EJECT_PASSENGERS = 1
    };

    enum eSpells
    {
        SPELL_EJECT_PASSENGERS = 68576
    };

    struct npc_gss_rope_seatAI : public ScriptedAI
    {
        npc_gss_rope_seatAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;

        void PassengerBoarded(Unit* who, int8 seatId, bool apply) override
        {
            if (apply)
            {
                float x, y, z;
                me->GetPosition(x, y, z);
                Movement::MoveSplineInit init(me);
                init.MoveTo(x, y, z - 68.f);
                init.SetVelocity(10.0f);
                init.Launch();
                events.ScheduleEvent(EVENT_EJECT_PASSENGERS, me->GetSplineDuration());
            }

        }

        void UpdateAI(const uint32 diff) override
        {
            events.Update(diff);

            if (events.ExecuteEvent() == EVENT_EJECT_PASSENGERS)
            {
                if (Vehicle* vehicle = me->GetVehicleKit())
                    if (Unit* passenger = vehicle->GetPassenger(0))
                        if (Player* player = passenger->ToPlayer())
                        {
                            float x, y;
                            player->ExitVehicle();
                            GetPositionWithDistInOrientation(me, 10.f, me->GetOrientation(), x, y);
                            player->GetMotionMaster()->MoveJump(x, y, me->GetMap()->GetHeight(me->GetPhaseMask(), me->GetPositionX(), me->GetPositionY(), MAX_HEIGHT), 20.0f, 25.0f);
                        }

                float x, y, z, o;
                me->GetHomePosition(x, y, z, o);
                me->NearTeleportTo(x, y, z, o);
            }
        }
    };
};

class npc_gss_rope: public CreatureScript
{
public:
    npc_gss_rope() : CreatureScript("npc_gss_rope") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_gss_ropeAI(creature);
    }

    enum eSpells
    {
        SPELL_ROPE_VISUAL = 126493
    };

    enum eActions
    {
        ACTION_ACTIVATE_ROPE = 1
    };

    struct npc_gss_ropeAI : public ScriptedAI
    {
        npc_gss_ropeAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() override
        {
            me->SetCanFly(true);
            me->SetDisableGravity(true);
            if (me->GetInstanceScript())
            {
                if (me->GetInstanceScript()->GetBossState(DATA_RIMOK) == DONE)
                    me->CastSpell(me, SPELL_ROPE_VISUAL, false);
            }
            else
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }

        void DoAction(int32 const action) override
        {
            if (action == ACTION_ACTIVATE_ROPE)
            {
                me->CastSpell(me, SPELL_ROPE_VISUAL, false);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }
        }
    };
};

// 7332
class AreaTrigger_at_destroy_corner_a : public AreaTriggerScript
{
public:
    AreaTrigger_at_destroy_corner_a() : AreaTriggerScript("at_destroy_corner_a") {}

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
    {
            if (InstanceScript* instance = player->GetInstanceScript())
            {
                if (instance->GetData(DATA_CORNER_A) != DONE)
                {
                    if (Creature* cornerA = Unit::GetCreature(*player, instance->GetData64(DATA_CORNER_A)))
                        if (cornerA->IsAIEnabled)
                            cornerA->AI()->DoAction(1);

                    instance->SetData(DATA_FALL_DEFENDERS, DONE);
                    instance->SetData(DATA_CORNER_A, DONE);
                }
            }

        return false;
    }
};

// 7665
class AreaTrigger_at_destroy_corner_b: public AreaTriggerScript
{
public:
    AreaTrigger_at_destroy_corner_b() : AreaTriggerScript("at_destroy_corner_b") {}

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
    {
        if (InstanceScript* instance = player->GetInstanceScript())
        {
            if (instance->GetData(DATA_CORNER_B) != DONE)
            {
                if (Creature* cornerB = Unit::GetCreature(*player, instance->GetData64(DATA_CORNER_B)))
                    if (cornerB->IsAIEnabled)
                        cornerB->AI()->DoAction(2);
            }
        }

        return false;
    }
};

// 7104
class AreaTrigger_at_before_gadok : public AreaTriggerScript
{
public:
    AreaTrigger_at_before_gadok() : AreaTriggerScript("at_before_gadok") {}

    bool OnTrigger(Player* player, AreaTriggerEntry const* trigger)
    {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (Creature* defenderA = Unit::GetCreature(*player, instance->GetData64(DATA_DEFENDER_A)))
                    if(defenderA->IsAIEnabled)
                        defenderA->AI()->DoAction(1);

            if (InstanceScript* instance = player->GetInstanceScript())
                if (Creature* defenderB = Unit::GetCreature(*player, instance->GetData64(DATA_DEFENDER_B)))
                    if(defenderB->IsAIEnabled)
                        defenderB->AI()->DoAction(1);

            if (InstanceScript* instance = player->GetInstanceScript())
                if (Creature* trainee = Unit::GetCreature(*player, instance->GetData64(DATA_TRAINEE)))
                    if(trainee->IsAIEnabled)
                        trainee->AI()->DoAction(1);

        return false;
     }
};

// 8395
class AreaTrigger_at_intro_gadok : public AreaTriggerScript
{
public:
    AreaTrigger_at_intro_gadok() : AreaTriggerScript("at_intro_gadok") {}

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
    {
        if (player->GetInstanceScript())
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (Creature* gadok = Unit::GetCreature(*player, instance->GetData64(DATA_GADOK)))
                    if (gadok->IsAIEnabled)
                        gadok->AI()->DoAction(1);
        }

        return false;
    }
};

class go_gss_generic : public GameObjectScript
{
public:
    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_gss_genericAI(go);
    }

    go_gss_generic() : GameObjectScript("go_gss_generic") { }

    struct go_gss_genericAI : public GameObjectAI
    {
        go_gss_genericAI(GameObject* go) : GameObjectAI(go)
        {
            go->setActive(true);
            if(go->GetInstanceScript())
            {
                switch (go->GetEntry())
                {
                    case GO_KIPTILAK_ENTRANCE_DOOR:
                        if (go->GetInstanceScript()->GetData(DATA_OPEN_FIRST_DOOR) == DONE)
                            go->SetGoState(GO_STATE_ACTIVE);
                        break;
                    case GO_WALL_A:
                        if (go->GetInstanceScript()->GetData(DATA_CORNER_A) == DONE)
                            go->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);
                        break;
                    case GO_WALL_B:
                        if (go->GetInstanceScript()->GetData(DATA_CORNER_B) == DONE)
                            go->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);
                        break;
                    case GO_WALL_C:
                        if (go->GetInstanceScript()->GetData(DATA_CORNER_C) == DONE)
                            go->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);
                        break;
                    case GO_GREAT_DOOR_PHASE_1:
                        if (go->GetInstanceScript()->GetBossState(DATA_RIMOK) == DONE)
                            go->SetPhaseMask(2, true);
                        break;
                    case GO_GREAT_DOOR_PHASE_2:
                        if (go->GetInstanceScript()->GetBossState(DATA_RIMOK) == DONE)
                            go->SetPhaseMask(1, true);
                        break;
                    case GO_KIPTILAK_MANTID_BOMBS:
                        if(go->GetInstanceScript()->GetBossState(DATA_KIPTILAK) == DONE)
                            go->SetPhaseMask(32768, true);
                        break;
                }
            }
        }
    };  
};

// 109986 
class spell_gss_volatile_explosion : public SpellScriptLoader
{
public:
    spell_gss_volatile_explosion() : SpellScriptLoader("spell_gss_volatile_explosion") { }

    AuraScript* GetAuraScript() const
    {
        return new spell_gss_volatile_explosion_AuraScript();
    }

    enum eSpells
    {
        SPELL_MANTID_MUNITION_EXPLOSION = 107153
    };

    class spell_gss_volatile_explosion_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gss_volatile_explosion_AuraScript);

        void OnRemove(AuraEffect const * /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                caster->CastSpell(caster, SPELL_MANTID_MUNITION_EXPLOSION, false);
        }

        void Register()
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_gss_volatile_explosion_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };
};

class PickAPlayerTargetSelector
{
public:
    PickAPlayerTargetSelector() { }

    bool operator()(WorldObject* object)
    {
        if(object->GetTypeId() == TYPEID_PLAYER && !object->ToPlayer()->IsVehicle())
            return false;

        return true;
    }
};

class spell_gss_pick_a_player : public SpellScriptLoader
{
public:
    spell_gss_pick_a_player() : SpellScriptLoader("spell_gss_pick_a_player") { }

    SpellScript *GetSpellScript() const
    {
        return new spell_gss_pick_a_player_SpellScript();
    }

    AuraScript* GetAuraScript() const
    {
        return new spell_gss_pick_a_player_AuraScript();
    }

    class spell_gss_pick_a_player_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gss_pick_a_player_SpellScript)

        void FilterTargets(std::list<WorldObject*>& targetList)
        {
            targetList.remove_if(PickAPlayerTargetSelector());
            targetList.sort(Trinity::ObjectDistanceOrderPred(GetCaster()));
            targetList.resize(1);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_gss_pick_a_player_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_gss_pick_a_player_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    class spell_gss_pick_a_player_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gss_pick_a_player_AuraScript);

        void OnApplyVehicle(AuraEffect const * aurEff, AuraEffectHandleModes /*mode*/)
        {
            Player* owner = GetOwner()->ToPlayer();
            Unit* caster = GetCaster();

            if (!owner || !caster)
                return;

            caster->CastSpell(owner, aurEff->GetSpellInfo()->Effects[0].BasePoints, false);
            caster->CastSpell(owner, aurEff->GetSpellInfo()->Effects[1].BasePoints, false);
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_gss_pick_a_player_AuraScript::OnApplyVehicle, EFFECT_0, SPELL_AURA_SET_VEHICLE_ID, AURA_EFFECT_HANDLE_REAL);
        }
    };
};

class spell_gss_sabotage_vehicle : public SpellScriptLoader
{
public:
    spell_gss_sabotage_vehicle() : SpellScriptLoader("spell_gss_sabotage_vehicle") { }

    AuraScript* GetAuraScript() const
    {
        return new spell_gss_sabotage_vehicle_AuraScript();
    }

    enum eSpells
    {
        SPELL_SABOTAGE = 107094
    };

    class spell_gss_sabotage_vehicle_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gss_sabotage_vehicle_AuraScript);

        void OnApply(AuraEffect const * /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* owner = GetOwner()->ToPlayer();
            Unit* caster = GetCaster();

            if (!owner || !caster)
                return;

            caster->CastSpell(owner, SPELL_SABOTAGE, false);
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_gss_sabotage_vehicle_AuraScript::OnApply, EFFECT_0, SPELL_AURA_CONTROL_VEHICLE, AURA_EFFECT_HANDLE_REAL);
        }
    };
};

class spell_gss_sabotage_explosion : public SpellScriptLoader
{
public:
    spell_gss_sabotage_explosion() : SpellScriptLoader("spell_gss_sabotage_explosion") { }

    AuraScript* GetAuraScript() const
    {
        return new spell_gss_sabotage_explosion_AuraScript();
    }

    class spell_gss_sabotage_explosion_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gss_sabotage_explosion_AuraScript);

        void AfterRemove(AuraEffect const * aurEff, AuraEffectHandleModes /*mode*/)
        {
            Player* owner = GetOwner()->ToPlayer();
            Unit* caster = GetCaster();

            if (!owner || !caster)
                return;

            caster->CastSpell(owner, aurEff->GetSpellInfo()->Effects[0].BasePoints, false);

            if (caster->IsOnVehicle())
                caster->ExitVehicle();
        }

        void Register()
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_gss_sabotage_explosion_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };
};

void AddSC_gate_of_the_setting_sun()
{
    new mob_kirkthik_demolisher();
    new mob_kirkthik_wind_shaper();
    new mob_kirkthik_conscript();
    new mob_kirkthik_infiltrator();
    new mob_kirkthik_sapper();
    new mob_gss_trash_base();
    new npc_krikthik_bombarder();
    new npc_serpents_spine_marksman();
    new npc_great_wall_explosion_target_stalker();
    new npc_kirthik_glider();
    new npc_great_wall_before_gadok_intro();
    new npc_gss_rope();
    new npc_gss_rope_seat();
    new npc_gss_pandaren_cannoneer();

    new go_setting_sun_brasier();
    new go_gss_generic();
    new vehicle_artillery_to_wall();
    new AreaTrigger_at_destroy_corner_a();
    new AreaTrigger_at_destroy_corner_b();
    new AreaTrigger_at_before_gadok();
    new AreaTrigger_at_intro_gadok();

    new spell_gss_volatile_explosion();
    new spell_gss_pick_a_player();
    new spell_gss_sabotage_vehicle();
    new spell_gss_sabotage_explosion();
}
