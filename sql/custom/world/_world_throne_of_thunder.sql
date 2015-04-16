/* Throne of Thunder */
SET FOREIGN_KEY_CHECKS = 0;

DELETE FROM `creature` WHERE `map` = 1098;
DELETE FROM `gameobject` WHERE `map` = 1098;
DELETE FROM `gameobject` WHERE `id` = 214539;

-- GUID RANGES
-- Gameobjects
SET @OGUID := (SELECT MAX(guid) FROM gameobject) + 1;
-- Creatures 
SET @CGUID := (SELECT MAX(guid) FROM creature) + 1;


-- Raid Portal
REPLACE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `AIName`, `ScriptName`, `WDBVerified`) VALUES 
(214539, 31, 11469, 'Instance Portal (Raid 4 Difficulties)', '', '', '', 35, 0, 2.5, 0, 0, 0, 0, 0, 0, 2, 216, 217, 216, 217, 11471, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(214611, 31, 11469, 'Instance Portal (Raid 4 Difficulties)', '', '', '', 35, 0, 5, 0, 0, 0, 0, 0, 0, 2, 216, 217, 216, 217, 11471, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658);

INSERT INTO `gameobject` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
(@OGUID+26, 214539, 1064, 0, 0, 1, 1, 7260.45, 5019.11, 80.2229, 2.38534, 0, 0, 0.929358, 0.36918, 86400, 0, 1, 0, NULL);

DELETE FROM `instance_encounters` WHERE `map` = 1098;
INSERT INTO `instance_encounters` (`map`, `index`, `difficulty`, `name`, `type`, `entry`, `dungeon`) VALUES
(1098, 9, 3, 'Jin''rokh the Breaker', 0, 69465, 0),
(1098, 9, 4, 'Jin''rokh the Breaker', 0, 69465, 0),
(1098, 9, 5, 'Jin''rokh the Breaker', 0, 69465, 0),
(1098, 9, 6, 'Jin''rokh the Breaker', 0, 69465, 0),

(1098, 7, 3, 'Horridon', 0, 68476, 0),
(1098, 7, 4, 'Horridon', 0, 68476, 0),
(1098, 7, 5, 'Horridon', 0, 68476, 0),
(1098, 7, 6, 'Horridon', 0, 68476, 0),

(1098, 3, 3, 'Council of Elders', 0, 69135, 0),
(1098, 3, 4, 'Council of Elders', 0, 69135, 0),
(1098, 3, 5, 'Council of Elders', 0, 69135, 0),
(1098, 3, 6, 'Council of Elders', 0, 69135, 0),

(1098, 2, 3, 'Tortos', 0, 67977, 0),
(1098, 2, 4, 'Tortos', 0, 67977, 0),
(1098, 2, 5, 'Tortos', 0, 67977, 0),
(1098, 2, 6, 'Tortos', 0, 67977, 0),

(1098, 10, 3, 'Megaera', 0, 68065, 0),
(1098, 10, 4, 'Megaera', 0, 68065, 0),
(1098, 10, 5, 'Megaera', 0, 68065, 0),
(1098, 10, 6, 'Megaera', 0, 68065, 0),

(1098, 5, 3, 'Ji-Kun', 0, 69712, 0),
(1098, 5, 4, 'Ji-Kun', 0, 69712, 0),
(1098, 5, 5, 'Ji-Kun', 0, 69712, 0),
(1098, 5, 6, 'Ji-Kun', 0, 69712, 0),

(1098, 4, 3, 'Durumu the Forgotten', 0, 68036, 0),
(1098, 4, 4, 'Durumu the Forgotten', 0, 68036, 0),
(1098, 4, 5, 'Durumu the Forgotten', 0, 68036, 0),
(1098, 4, 6, 'Durumu the Forgotten', 0, 68036, 0),

(1098, 6, 3, 'Primordius', 0, 69017, 0),
(1098, 6, 4, 'Primordius', 0, 69017, 0),
(1098, 6, 5, 'Primordius', 0, 69017, 0),
(1098, 6, 6, 'Primordius', 0, 69017, 0),

(1098, 8, 3, 'Dark Animus', 0, 69427, 0),
(1098, 8, 4, 'Dark Animus', 0, 69427, 0),
(1098, 8, 5, 'Dark Animus', 0, 69427, 0),
(1098, 8, 6, 'Dark Animus', 0, 69427, 0),

(1098, 0, 3, 'Iron Qon', 0, 68078, 0),
(1098, 0, 4, 'Iron Qon', 0, 68078, 0),
(1098, 0, 5, 'Iron Qon', 0, 68078, 0),
(1098, 0, 6, 'Iron Qon', 0, 68078, 0),

(1098, 1, 3, 'Twin Consorts', 0, 68905, 0),
(1098, 1, 4, 'Twin Consorts', 0, 68905, 0),
(1098, 1, 5, 'Twin Consorts', 0, 68905, 0),
(1098, 1, 6, 'Twin Consorts', 0, 68905, 0),

(1098, 11, 3, 'Lei Shen', 0, 68397, 0),
(1098, 11, 4, 'Lei Shen', 0, 68397, 0),
(1098, 11, 5, 'Lei Shen', 0, 68397, 0),
(1098, 11, 6, 'Lei Shen', 0, 68397, 0),

(1098, 12, 5, 'Ra-Den', 0, 69473, 0),
(1098, 12, 6, 'Ra-Den', 0, 69473, 0);

-- Isle of Thunder available
DELETE FROM `spell_area` WHERE `area` = 6507;

-- Throne of Thunder Ghost Zone fix
DELETE FROM `game_graveyard_zone` WHERE `id` = 4490;
INSERT INTO `game_graveyard_zone` (`id`, `ghost_zone`, `faction`) VALUES
(4490,6622,0);

DELETE FROM `spell_area` WHERE `spell` IN (55164,55173) AND `area` = 6507;
INSERT INTO `spell_area` VALUES 
(55164,6507,0,0,8326,10487799,2,1,0,0), -- (Pandaria) Isle of Thunder
(55173,6507,0,0,8326,8,2,1,0,0);    -- (Pandaria-NightElf) Isle of Thunder

-- Instance Script
DELETE FROM `instance_template` WHERE `map` = 1098;
INSERT INTO `instance_template` (map, parent, script, allowMount) VALUES
(1098, 1064, 'instance_throne_of_thunder', 0);

-- Difficulty Data
SET @RAID_DIFF_10N := 3100000;
SET @RAID_DIFF_10H := 3200000;
SET @RAID_DIFF_25N := 3300000;
SET @RAID_DIFF_25H := 3400000;
SET @RAID_DIFF_25R := 3500000;

SET @HP_MOD_93 := (SELECT basehp4 FROM creature_classlevelstats WHERE class = 1 AND LEVEL = 93);
SET @HP_MOD_92 := (SELECT basehp4 FROM creature_classlevelstats WHERE class = 1 AND LEVEL = 92);
SET @HP_MOD_91 := (SELECT basehp4 FROM creature_classlevelstats WHERE class = 1 AND LEVEL = 91);
SET @HP_MOD_90 := (SELECT basehp4 FROM creature_classlevelstats WHERE class = 1 AND LEVEL = 90);
SET @HP_MOD_1  := (SELECT basehp0 FROM creature_classlevelstats WHERE class = 1 AND LEVEL = 0);

SET @HP_MOD_93_M := (SELECT basehp4 FROM creature_classlevelstats WHERE class = 2 AND LEVEL = 93);
SET @HP_MOD_92_M := (SELECT basehp4 FROM creature_classlevelstats WHERE class = 2 AND LEVEL = 92);
SET @HP_MOD_91_M := (SELECT basehp4 FROM creature_classlevelstats WHERE class = 2 AND LEVEL = 91);
SET @HP_MOD_90_M := (SELECT basehp4 FROM creature_classlevelstats WHERE class = 2 AND LEVEL = 90);
SET @HP_MOD_1_M  := (SELECT basehp0 FROM creature_classlevelstats WHERE class = 2 AND LEVEL = 0);

SET @HP_MOD_93_4 := (SELECT basehp4 FROM creature_classlevelstats WHERE class = 4 AND LEVEL = 93);
SET @HP_MOD_92_4 := (SELECT basehp4 FROM creature_classlevelstats WHERE class = 4 AND LEVEL = 92);
SET @HP_MOD_91_4 := (SELECT basehp4 FROM creature_classlevelstats WHERE class = 4 AND LEVEL = 91);
SET @HP_MOD_90_4 := (SELECT basehp4 FROM creature_classlevelstats WHERE class = 4 AND LEVEL = 90);
SET @HP_MOD_1_4  := (SELECT basehp0 FROM creature_classlevelstats WHERE class = 4 AND LEVEL = 0);

SET @HP_MOD_93_8 := (SELECT basehp4 FROM creature_classlevelstats WHERE class = 8 AND LEVEL = 93);
SET @HP_MOD_92_8 := (SELECT basehp4 FROM creature_classlevelstats WHERE class = 8 AND LEVEL = 92);
SET @HP_MOD_91_8 := (SELECT basehp4 FROM creature_classlevelstats WHERE class = 8 AND LEVEL = 91);
SET @HP_MOD_90_8 := (SELECT basehp4 FROM creature_classlevelstats WHERE class = 8 AND LEVEL = 90);
SET @HP_MOD_1_8  := (SELECT basehp0 FROM creature_classlevelstats WHERE class = 8 AND LEVEL = 0);

/* BEGIN Jin'rokh the Breaker */
/*---------------------------------------------------------------------------------------------------------------*/
-- Spell Scripts
DELETE FROM `spell_script_names` WHERE `spell_id` IN (137370,139218,139319,139559);
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(137161, 'spell_thundering_throw_silence'),
(137162, 'spell_static_burst'),
(137167, 'spell_thundering_throw_damage'),
(137168, 'spell_conductive_water_dummy'),
(137180, 'spell_thundering_throw'),
(137194, 'spell_focused_lightning_targeting'),
(137374, 'spell_focused_lightning_detonation'),
(137389, 'spell_focused_lightning_speed'),
(137399, 'spell_focused_lightning'),
(137429, 'spell_focused_lightning_aoe'),
(137507, 'spell_implosion'),
(137530, 'spell_focused_lightning_conduction'),
(138002, 'spell_water_auras'),
(138006, 'spell_water_auras'),
(138133, 'spell_lightning_fissure_conduction'),
(138349, 'spell_static_wound'),
(138389, 'spell_static_wound_damage'),
(138568, 'spell_lightning_storm_visual'),
(138990, 'spell_focused_lightning_detonation'),
(139203, 'spell_focused_lightning_targeting'),
(139209, 'spell_focused_lightning_aoe_trash'),
(139218, 'spell_storm_weapon_aura'),
(139319, 'spell_storm_weapon'),
(139559, 'spell_storm_energy');

/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Speeches
DELETE FROM `creature_text` WHERE `entry` IN (69465, 69593);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(69465, 0,  0, 'Da Thunder King give me power! Come, I show you!', 14, 0, 100, 0, 0, 35550,'Jin''rokh the Breaker - Intro'),
(69465, 1,  0, 'I must break you.', 14, 0, 100, 0, 0, 35547,  'Jin''rokh the Breaker - Aggro'),
(69465, 2,  0, 'Smash!', 14, 0, 100, 0, 0, 35553,  'Jin''rokh the Breaker - Static Burst'),
(69465, 3,  0, 'Crush your bones!', 14, 0, 100, 0, 0, 35554,  'Jin''rokh the Breaker - Thundering Throw'),
(69465, 4,  0, 'I call da storm!', 14, 0, 100, 0, 0, 35555,  'Jin''rokh the Breaker - Lightning Storm'),
(69465, 5,  0, 'Zap!', 14, 0, 100, 0, 0, 35556,  'Jin''rokh the Breaker - Focused Lightning'),
(69465, 6,  0, '|TInterface\\Icons\\ability_warrior_throwdown:20|t%s grabs $n and readies his |cFFFF0000|Hspell:137175|h[Thundering Throw]|h|r!', 41, 0, 100, 0, 0, 0,  'Jin''rokh the Breaker'),
(69465, 7,  0, '|TInterface\\Icons\\spell_shaman_thunderstorm:20|t%s begins to channel an intense |cFFFF0000|Hspell:137313|h[Lightning Storm]|h|r!', 41, 0, 100, 0, 0, 0,  'Jin''rokh the Breaker'),
(69465, 8,  0, '|TInterface\\Icons\\spell_shaman_thunderstorm:20|t%s begins to channel an intense |cFFFF0000|Hspell:137313|h[Lightning Storm]|h|r!', 41, 0, 100, 0, 0, 0,  'Jin''rokh the Breaker'),
(69465, 9,  0, 'Weak.', 14, 0, 100, 0, 0, 35551,  'Jin''rokh the Breaker - Kill01'),
(69465, 9,  1, 'So easy to break.', 14, 0, 100, 0, 0, 35552,  'Jin''rokh the Breaker - Kill02'),
(69465, 10, 0, 'Bored. You die now.', 14, 0, 100, 0, 0, 35552,  'Jin''rokh the Breaker - Berserk'),
(69465, 11, 0, 'How you beat me?', 14, 0, 100, 0, 0, 35549,  'Jin''rokh the Breaker Death'),

(69593, 0, 0, 'Jin''rokh the Breaker''s |cFFFF0000|Hspell:137422|h[Focused Lightning]|h|r fixates on you. Run!', 41, 0, 100, 0, 0, 0, 'Focused Lightning emote');

/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Templates 
REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`,`equipment_id`) VALUES 
(70245, 0, 0, 0, 0, 0, 27510, 0, 0, 0, 'Training Dummy', '', '', 0, 1, 1, 4, 14, 14, 0, 1, 1.14286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 1, 131076, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 65000000, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 'npc_training_dummy', 17614,0);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`,`equipment_id`) VALUES
(69388, 0, 0, 0, 0, 0, 47507, 0, 0, 0, 'Zandalari Spear-Shaper', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 184, 1, 0, 0, 'npc_zandalari_spearshaper', 17614,69388),
(69390, 0, 0, 0, 0, 0, 47528, 0, 0, 0, 'Zandalari Storm-Caller', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 11000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 0, '', 17614,69390),
(69395, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Drawn Water', '', '', 0, 93, 93, 4, 16, 16, 0, 0.01, 0.01, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 33685508, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 17826816, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, '', 17614,0),
(69438, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Thrown Spear', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 33685572, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 17826816, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1, 0),
(69455, 0, 0, 0, 0, 0, 47536, 0, 0, 0, 'Zandalari Water-Binder', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 11000, '', 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 0, '', 17614,69455),
(69459, 0, 0, 0, 0, 0, 525, 0, 0, 0, 'Bound Water Elemental', '', '', 0, 90, 90, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 200, 1, 0, 0, '', 17614,0),
(69465, 0, 0, 0, 0, 0, 47552, 0, 0, 0, 'Jin\'rokh the Breaker', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2900000, 3000000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 667893759, 1, 'boss_jinrokh', 17614,0),
(69467, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Statue', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 32768, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 15, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 128, 'npc_jinrokh_statue', 17614,0),
(69469, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Conductive Water', '', '', 0, 1, 1, 4, 35, 35, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 33554438, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 17826816, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 130, 'npc_conductive_water', 17614,0),
(69593, 0, 0, 0, 0, 0, 47698, 0, 0, 0, 'Focused Lightning', '', '', 0, 93, 93, 4, 14, 14, 0, 0.65, 0.65, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 33554434, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 17826816, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 94, 1, 0, 0, 'npc_focused_lightning', 17614,0),
(69609, 0, 0, 0, 0, 0, 39633, 0, 0, 0, 'Lightning Fissure', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 33554438, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 17826816, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 130, 'npc_lightning_fissure', 17614,0),
(69676, 0, 0, 0, 0, 0, 39633, 0, 0, 0, 'Call Da Storm Stalker', '', '', 0, 1, 1, 4, 35, 35, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 17826816, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,0),
(69753, 0, 0, 0, 0, 0, 15294, 0, 0, 0, 'Lightning Strike', '', '', 0, 1, 1, 4, 35, 35, 0, 1, 1.14286, 1, 0, 9839, 14339, 0, 42299, 1, 2000, 2000, 1, 33554434, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 17826816, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 100, 1, 0, 130, '', 17614,0),
(70174, 0, 0, 0, 0, 0, 47698, 0, 0, 0, 'Focused Lightning', '', '', 0, 1, 1, 4, 35, 35, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 33554434, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 17826816, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 94, 1, 0, 130, 'npc_focused_lightning_trash', 17614,0),
(70230, 0, 0, 0, 0, 0, 47941, 47942, 47943, 47944, 'Zandalari Blade Initiate', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 184, 1, 0, 0, '', 17614,70230),
(70236, 0, 0, 0, 0, 0, 47528, 0, 0, 0, 'Zandalari Storm-Caller', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 0, 'npc_zandalari_stormcaller', 17614,70236),
(70491, 0, 0, 0, 0, 0, 47528, 0, 0, 0, 'Crazed Storm-Caller', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 650854399, 0, 'npc_zandalari_stormcaller', 17614,70236),
(69548, 0, 0, 0, 0, 0, 47568, 25501, 11032, 45754, 'Shadowed Loa Spirit', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 118, 1, 0, 0, 'mob_shadowed_loa_spirit', 17614, 0),

(@RAID_DIFF_10N + 69388, 0, 0, 0, 0, 0, 47507, 0, 0, 0, 'Zandalari Spear-Shaper', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 184, 1, 0, 0, '', 17614,69388),
(@RAID_DIFF_10N + 69390, 0, 0, 0, 0, 0, 47528, 0, 0, 0, 'Zandalari Storm-Caller', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 11000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 0, '', 17614,69390),
(@RAID_DIFF_10N + 69455, 0, 0, 0, 0, 0, 47536, 0, 0, 0, 'Zandalari Water-Binder', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 11000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_10N + 69465, 0, 0, 0, 0, 0, 47552, 0, 0, 0, 'Jin\'rokh the Breaker', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2900000, 3000000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 667893759, 1, 'boss_jinrokh', 17614,0),
(@RAID_DIFF_10N + 70230, 0, 0, 0, 0, 0, 47941, 47942, 47943, 47944, 'Zandalari Blade Initiate', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 184, 1, 0, 0, '', 17614,70230),
(@RAID_DIFF_10N + 70236, 0, 0, 0, 0, 0, 47528, 0, 0, 0, 'Zandalari Storm-Caller', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 0, '', 17614,70236),
(@RAID_DIFF_10N + 70491, 0, 0, 0, 0, 0, 47528, 0, 0, 0, 'Crazed Storm-Caller', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 650854399, 0, '', 17614,70236),
(@RAID_DIFF_10N + 69548, 0, 0, 0, 0, 0, 47568, 25501, 11032, 45754, 'Shadowed Loa Spirit', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 118, 1, 0, 0, 'mob_shadowed_loa_spirit', 17614, 0),

(@RAID_DIFF_25N + 69388, 0, 0, 0, 0, 0, 47507, 0, 0, 0, 'Zandalari Spear-Shaper', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 184, 1, 0, 0, '', 17614,69388),
(@RAID_DIFF_25N + 69390, 0, 0, 0, 0, 0, 47528, 0, 0, 0, 'Zandalari Storm-Caller', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 11000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 0, '', 17614,69390),
(@RAID_DIFF_25N + 69455, 0, 0, 0, 0, 0, 47536, 0, 0, 0, 'Zandalari Water-Binder', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 11000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_25N + 69465, 0, 0, 0, 0, 0, 47552, 0, 0, 0, 'Jin\'rokh the Breaker', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2900000, 3000000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 667893759, 1, 'boss_jinrokh', 17614,0),
(@RAID_DIFF_25N + 70230, 0, 0, 0, 0, 0, 47941, 47942, 47943, 47944, 'Zandalari Blade Initiate', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 184, 1, 0, 0, '', 17614,70230),
(@RAID_DIFF_25N + 70236, 0, 0, 0, 0, 0, 47528, 0, 0, 0, 'Zandalari Storm-Caller', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 0, '', 17614,70236),
(@RAID_DIFF_25N + 70491, 0, 0, 0, 0, 0, 47528, 0, 0, 0, 'Crazed Storm-Caller', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 650854399, 0, '', 17614,70236),
(@RAID_DIFF_25N + 69548, 0, 0, 0, 0, 0, 47568, 25501, 11032, 45754, 'Shadowed Loa Spirit', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 118, 1, 0, 0, 'mob_shadowed_loa_spirit', 17614, 0),

(@RAID_DIFF_10H + 69388, 0, 0, 0, 0, 0, 47507, 0, 0, 0, 'Zandalari Spear-Shaper', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 184, 1, 0, 0, '', 17614,69388),
(@RAID_DIFF_10H + 69390, 0, 0, 0, 0, 0, 47528, 0, 0, 0, 'Zandalari Storm-Caller', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 11000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 0, '', 17614,69390),
(@RAID_DIFF_10H + 69455, 0, 0, 0, 0, 0, 47536, 0, 0, 0, 'Zandalari Water-Binder', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 11000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_10H + 69465, 0, 0, 0, 0, 0, 47552, 0, 0, 0, 'Jin\'rokh the Breaker', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2900000, 3000000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 667893759, 1, 'boss_jinrokh', 17614,0),
(@RAID_DIFF_10H + 70230, 0, 0, 0, 0, 0, 47941, 47942, 47943, 47944, 'Zandalari Blade Initiate', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 184, 1, 0, 0, '', 17614,70230),
(@RAID_DIFF_10H + 70236, 0, 0, 0, 0, 0, 47528, 0, 0, 0, 'Zandalari Storm-Caller', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 0, '', 17614,70236),
(@RAID_DIFF_10H + 70491, 0, 0, 0, 0, 0, 47528, 0, 0, 0, 'Crazed Storm-Caller', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 650854399, 0, '', 17614,70236),
(@RAID_DIFF_10H + 69548, 0, 0, 0, 0, 0, 47568, 25501, 11032, 45754, 'Shadowed Loa Spirit', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 118, 1, 0, 0, 'mob_shadowed_loa_spirit', 17614, 0),

(@RAID_DIFF_25H + 69388, 0, 0, 0, 0, 0, 47507, 0, 0, 0, 'Zandalari Spear-Shaper', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 184, 1, 0, 0, '', 17614,69388),
(@RAID_DIFF_25H + 69390, 0, 0, 0, 0, 0, 47528, 0, 0, 0, 'Zandalari Storm-Caller', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 11000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 0, '', 17614,69390),
(@RAID_DIFF_25H + 69455, 0, 0, 0, 0, 0, 47536, 0, 0, 0, 'Zandalari Water-Binder', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 11000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_25H + 69465, 0, 0, 0, 0, 0, 47552, 0, 0, 0, 'Jin\'rokh the Breaker', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2900000, 3000000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 667893759, 1, 'boss_jinrokh', 17614,0),
(@RAID_DIFF_25H + 70230, 0, 0, 0, 0, 0, 47941, 47942, 47943, 47944, 'Zandalari Blade Initiate', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 184, 1, 0, 0, '', 17614,70230),
(@RAID_DIFF_25H + 70236, 0, 0, 0, 0, 0, 47528, 0, 0, 0, 'Zandalari Storm-Caller', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 0, '', 17614,70236),
(@RAID_DIFF_25H + 70491, 0, 0, 0, 0, 0, 47528, 0, 0, 0, 'Crazed Storm-Caller', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 650854399, 0, '', 17614,70236),
(@RAID_DIFF_25H + 69548, 0, 0, 0, 0, 0, 47568, 25501, 11032, 45754, 'Shadowed Loa Spirit', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 118, 1, 0, 0, 'mob_shadowed_loa_spirit', 17614, 0),

(@RAID_DIFF_25R + 69388, 0, 0, 0, 0, 0, 47507, 0, 0, 0, 'Zandalari Spear-Shaper', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 184, 1, 0, 0, '', 17614,69388),
(@RAID_DIFF_25R + 69390, 0, 0, 0, 0, 0, 47528, 0, 0, 0, 'Zandalari Storm-Caller', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 11000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 0, '', 17614,69390),
(@RAID_DIFF_25R + 69455, 0, 0, 0, 0, 0, 47536, 0, 0, 0, 'Zandalari Water-Binder', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 11000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_25R + 69465, 0, 0, 0, 0, 0, 47552, 0, 0, 0, 'Jin\'rokh the Breaker', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2900000, 3000000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 667893759, 1, 'boss_jinrokh', 17614,0),
(@RAID_DIFF_25R + 70230, 0, 0, 0, 0, 0, 47941, 47942, 47943, 47944, 'Zandalari Blade Initiate', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 184, 1, 0, 0, '', 17614,70230),
(@RAID_DIFF_25R + 70236, 0, 0, 0, 0, 0, 47528, 0, 0, 0, 'Zandalari Storm-Caller', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 0, 0, '', 17614,70236),
(@RAID_DIFF_25R + 70491, 0, 0, 0, 0, 0, 47528, 0, 0, 0, 'Crazed Storm-Caller', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 650854399, 0, '', 17614,70236),
(@RAID_DIFF_25R + 69548, 0, 0, 0, 0, 0, 47568, 25501, 11032, 45754, 'Shadowed Loa Spirit', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 118, 1, 0, 0, 'mob_shadowed_loa_spirit', 17614, 0);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Updates
-- Creature Trash Mobs Damage, HP, LootId
-- Base HP Gamepedia Raid 25N
-- 25R -10%
-- 25H +25%
-- 10N (BASE HP: 25 = X : 10)
-- 10H=(10N+25%)

-- Script Names
UPDATE `creature_template` SET `ScriptName` = 'npc_zandalari_spearshaper' WHERE `entry` = 69388;
UPDATE `creature_template` SET `ScriptName` = 'npc_focused_lightning', `HoverHeight` = 5 WHERE `entry` = 69593;
UPDATE `creature_template` SET `ScriptName` = 'npc_lightning_fissure' WHERE `entry` = 69609;
UPDATE `creature_template` SET `ScriptName` = 'npc_zandalari_stormcaller' WHERE `entry` = 70236;

-- Difficulty entries bosses and Trash Mobs
UPDATE creature_template SET difficulty_entry_3 = @RAID_DIFF_10N + entry, difficulty_entry_4 = @RAID_DIFF_25N + entry, difficulty_entry_5 = @RAID_DIFF_10H + entry, difficulty_entry_6 = @RAID_DIFF_25H + entry, difficulty_entry_7 = @RAID_DIFF_25R + entry WHERE entry IN 
(69388,69390,69455,69465,70230,70236,70491);

-- Immunity and bind bosses
UPDATE creature_template SET flags_extra = flags_extra | 1, mechanic_immune_mask = 667893759 WHERE entry IN (69465,3169465,3269465,3369465,3469465,3569465);

-- Zandalari Spear-Shaper
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 4, Health_Mod = 14165200 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 69388;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 5, Health_Mod = 17706500 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 69388;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 7, Health_Mod = 35413000 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 69388;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 8, Health_Mod = 44266250 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 69388;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 6, Health_Mod = 31871700 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 69388;

-- Zandalari Storm-Caller
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 4, Health_Mod = 9308400  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 69390;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 5, Health_Mod = 11635500 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 69390;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 7, Health_Mod = 23271000 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 69390;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 8, Health_Mod = 29088750 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 69390;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 6, Health_Mod = 20943900 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 69390;

-- Zandalari Water-Binder
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 4, Health_Mod = 7446958  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 69455;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 5, Health_Mod = 9308698  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 69455;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 7, Health_Mod = 18617396 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 69455;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 8, Health_Mod = 23271745 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 69455;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 6, Health_Mod = 16755656 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 69455;

-- Zandalari Blade Initiate
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 3, Health_Mod = 6070800  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 70230;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 4, Health_Mod = 7588500  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 70230;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 6, Health_Mod = 15177000 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 70230;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 7, Health_Mod = 18971250 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 70230;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 5, Health_Mod = 13659300 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 70230;

-- Zandalari Storm-Caller (Door's Guard)
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 5, Health_Mod = 11196000 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 70236;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 6, Health_Mod = 13995000 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 70236;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 8, Health_Mod = 27990000 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 70236;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 9, Health_Mod = 34987500 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 70236;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 7, Health_Mod = 25191000 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 70236;

-- Crazed Storm-Caller (Door's Guard)
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 5, Health_Mod = 11196000 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 70491;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 6, Health_Mod = 13995000 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 70491;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 8, Health_Mod = 27990000 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 70491;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 9, Health_Mod = 34987500 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 70491;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 7, Health_Mod = 25191000 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 70491;

-- Jin'rokh the Breaker
UPDATE creature_template SET LootId = @RAID_DIFF_10N + 69465, dmg_multiplier = 13, Health_Mod = 207601216 / @HP_MOD_93 WHERE entry = @RAID_DIFF_10N + 69465;
UPDATE creature_template SET LootId = @RAID_DIFF_10H + 69465, dmg_multiplier = 14, Health_Mod = 317507744 / @HP_MOD_93 WHERE entry = @RAID_DIFF_10H + 69465;
UPDATE creature_template SET LootId = @RAID_DIFF_25N + 69465, dmg_multiplier = 15, Health_Mod = 622803648 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25N + 69465;
UPDATE creature_template SET LootId = @RAID_DIFF_25H + 69465, dmg_multiplier = 16, Health_Mod = 952523200 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25H + 69465;
UPDATE creature_template SET LootId = @RAID_DIFF_25R + 69465, dmg_multiplier = 12, Health_Mod = 545171264 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25R + 69465;
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Trash Mobs SAI
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (69390,69455,69459,70230);
DELETE FROM `smart_scripts` WHERE `entryorguid`IN (69390,69455,69459,70230) AND `source_type`= 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(69390, 0, 0, 0, 0, 0, 100, 0, 3000, 10000, 12000, 18000, 11, 139206, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, "Zandalari Storm-Caller - Cast Focused Lightning on a random player"),
(69390, 0, 1, 0, 0, 0, 100, 0, 6000, 12000, 20000, 25000, 11, 139218, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Zandalari Storm-Caller - Cast Storm Weapon"),
(69390, 0, 2, 0, 0, 0, 100, 0, 9000, 12000, 20000, 30000, 11, 139251, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Zandalari Storm-Caller - Cast Water Bolt"),
(69455, 0, 0, 0, 0, 0, 100, 0, 3000, 4000, 7000, 10000, 11, 136952, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Zandalari Water-Binder - Cast Frostbolt"),
(69455, 0, 1, 0, 0, 0, 70, 0, 5000, 10000, 13000, 18000, 11, 137099, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Zandalari Water-Binder - Cast Bind Water"),
(69455, 0, 2, 0, 0, 0, 100, 0, 9000, 15000, 17000, 26000, 11, 136881, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, "Zandalari Water-Binder - Cast Deluge"),
(69459, 0, 0, 0, 1, 0, 100, 0, 1, 1, 1000, 1000, 49, 0, 0, 0, 0, 0, 0, 21, 50, 0, 0, 0, 0, 0, 0, "Bound Water Elemental - Attack on spawn"),
(70230, 0, 0, 0, 0, 0, 100, 0, 12000, 13000, 15000, 16000, 11, 140049, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Zandalari Blade Initiate - Cast Wounding Strike");
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Trash Mobs Spell fix
DELETE FROM spell_linked_spell WHERE spell_trigger IN (136986, -137077);
INSERT INTO spell_linked_spell VALUES
(136986, 137066, 0, 'Spear Throw - Trigger Disarm'),
(-137077, -137066, 0, 'Spear Spin - Remove Disarm');
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Equip
REPLACE INTO `creature_equip_template` (`entry`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES 
(69388, 82006, 0, 0),
(69390, 95867, 0, 0),
(69455, 94118, 0, 0),
(70230, 95803, 0, 0),
(70236, 95867, 0, 0);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Jin'rokh the Breaker Loots & Currency
DELETE FROM `creature_loot_template` WHERE `entry`IN(3169465,3269465,3369465,3469465,3569465);
INSERT INTO `creature_loot_template` VALUES 
-- 10 Normal Loot
(3169465, 94723, 0, 1, 1, 1, 1),
(3169465, 94724, 0, 1, 1, 1, 1),
(3169465, 94725, 0, 1, 1, 1, 1),
(3169465, 94726, 0, 1, 1, 1, 1),
(3169465, 94727, 0, 1, 1, 1, 1),
(3169465, 94728, 0, 1, 1, 1, 1),
(3169465, 94731, 0, 1, 1, 1, 1),
(3169465, 94732, 0, 1, 1, 1, 1),
(3169465, 94733, 0, 1, 1, 1, 1),
(3169465, 94734, 0, 1, 1, 1, 1),
(3169465, 94736, 0, 1, 1, 1, 1),
(3169465, 94737, 0, 1, 1, 1, 1),
(3169465, 94739, 0, 1, 1, 1, 1),
(3169465, 94512, 0, 1, 2, 1, 1),
(3169465, 94722, 0, 1, 2, 1, 1),
(3169465, 94729, 0, 1, 2, 1, 1),
(3169465, 94730, 0, 1, 2, 1, 1),
(3169465, 94735, 0, 1, 2, 1, 1),
(3169465, 94738, 0, 1, 2, 1, 1),
(3169465, 95510, 0, 1, 2, 1, 1),
(3169465, 94593, 1, 1, 3, 1, 1),
(3169465, 94594, 1, 1, 3, 1, 1),
(3169465, 87208, 1, 1, 3, 1, 1),
(3169465, 87209, 1, 1, 3, 1, 1),
(3169465, 95498, 5, 1, 10, -95498, 1),
 
-- 10 Heroic Loot
(3269465, 96370, 0, 1, 1, 1, 1),
(3269465, 96380, 0, 1, 1, 1, 1),
(3269465, 96381, 0, 1, 1, 1, 1),
(3269465, 96386, 0, 1, 1, 1, 1),
(3269465, 96378, 0, 1, 1, 1, 1),
(3269465, 96379, 0, 1, 1, 1, 1),
(3269465, 96372, 0, 1, 2, 1, 1),
(3269465, 96374, 0, 1, 2, 1, 1),
(3269465, 96371, 0, 1, 2, 1, 1),
(3269465, 96388, 0, 1, 2, 1, 1),
(3269465, 96375, 0, 1, 2, 1, 1),
(3269465, 96387, 0, 1, 2, 1, 1),
(3269465, 96373, 0, 1, 2, 1, 1),
(3269465, 96383, 0, 1, 3, 1, 1),
(3269465, 96368, 0, 1, 3, 1, 1),
(3269465, 96369, 0, 1, 3, 1, 1),
(3269465, 96376, 0, 1, 3, 1, 1),
(3269465, 96377, 0, 1, 3, 1, 1),
(3269465, 96382, 0, 1, 3, 1, 1),
(3269465, 96384, 0, 1, 3, 1, 1),
(3269465, 94593, 1, 1, 4, 1, 1),
(3269465, 94594, 1, 1, 4, 1, 1),
(3269465, 87208, 1, 1, 4, 1, 1),
(3269465, 87209, 1, 1, 4, 1, 1),
(3269465, 96621, 5, 1, 10, -96621, 1),
 
-- 25 Normal Loot
(3369465, 94723, 0, 1, 1, 1, 1),
(3369465, 94724, 0, 1, 1, 1, 1),
(3369465, 94725, 0, 1, 1, 1, 1),
(3369465, 94733, 0, 1, 1, 1, 1),
(3369465, 94728, 0, 1, 2, 1, 1),
(3369465, 94731, 0, 1, 2, 1, 1),
(3369465, 94732, 0, 1, 2, 1, 1),
(3369465, 94734, 0, 1, 2, 1, 1),
(3369465, 94727, 0, 1, 3, 1, 1),
(3369465, 94736, 0, 1, 3, 1, 1),
(3369465, 94737, 0, 1, 3, 1, 1),
(3369465, 94726, 0, 1, 4, 1, 1),
(3369465, 94735, 0, 1, 4, 1, 1),
(3369465, 94739, 0, 1, 4, 1, 1),
(3369465, 94512, 0, 1, 5, 1, 1),
(3369465, 94722, 0, 1, 5, 1, 1),
(3369465, 94729, 0, 1, 5, 1, 1),
(3369465, 94730, 0, 1, 5, 1, 1),
(3369465, 94738, 0, 1, 5, 1, 1),
(3369465, 95510, 0, 1, 5, 1, 1),
(3369465, 94593, 1, 1, 6, 1, 1),
(3369465, 94594, 1, 1, 6, 1, 1),
(3369465, 87208, 1, 1, 6, 1, 1),
(3369465, 87209, 1, 1, 6, 1, 1),
(3369465, 95498, 5, 1, 10, -95498, 1),
 
-- 25 Heroic Loot
(3469465, 96374, 0, 1, 1, 1, 1),
(3469465, 96370, 0, 1, 1, 1, 1),
(3469465, 96372, 0, 1, 1, 1, 1),
(3469465, 96380, 0, 1, 1, 1, 1),
(3469465, 96371, 0, 1, 2, 1, 1),
(3469465, 96381, 0, 1, 2, 1, 1),
(3469465, 96386, 0, 1, 2, 1, 1),
(3469465, 96388, 0, 1, 2, 1, 1),
(3469465, 96375, 0, 1, 3, 1, 1),
(3469465, 96387, 0, 1, 3, 1, 1),
(3469465, 96378, 0, 1, 3, 1, 1),
(3469465, 96373, 0, 1, 4, 1, 1),
(3469465, 96383, 0, 1, 4, 1, 1),
(3469465, 96379, 0, 1, 4, 1, 1),
(3469465, 96369, 0, 1, 5, 1, 1),
(3469465, 96376, 0, 1, 5, 1, 1),
(3469465, 96377, 0, 1, 5, 1, 1),
(3469465, 96384, 0, 1, 5, 1, 1),
(3469465, 96382, 0, 1, 5, 1, 1),
(3469465, 96368, 0, 1, 5, 1, 1),
(3469465, 94593, 1, 1, 6, 1, 1),
(3469465, 94594, 1, 1, 6, 1, 1),
(3469465, 87208, 1, 1, 6, 1, 1),
(3469465, 87209, 1, 1, 6, 1, 1),
(3469465, 96621, 5, 1, 10, -96621, 1),
 
-- 25 LFR Loot
(3569465, 95626, 0, 1, 1, 1, 1),
(3569465, 95628, 0, 1, 1, 1, 1),
(3569465, 95630, 0, 1, 1, 1, 1),
(3569465, 95636, 0, 1, 1, 1, 1),
(3569465, 95627, 0, 1, 2, 1, 1),
(3569465, 95637, 0, 1, 2, 1, 1),
(3569465, 95642, 0, 1, 2, 1, 1),
(3569465, 95644, 0, 1, 2, 1, 1),
(3569465, 95631, 0, 1, 3, 1, 1),
(3569465, 95634, 0, 1, 3, 1, 1),
(3569465, 95643, 0, 1, 3, 1, 1),
(3569465, 95629, 0, 1, 4, 1, 1),
(3569465, 95635, 0, 1, 4, 1, 1),
(3569465, 95639, 0, 1, 4, 1, 1),
(3569465, 95624, 0, 1, 5, 1, 1),
(3569465, 95625, 0, 1, 5, 1, 1),
(3569465, 95632, 0, 1, 5, 1, 1),
(3569465, 95633, 0, 1, 5, 1, 1),
(3569465, 95638, 0, 1, 5, 1, 1),
(3569465, 95640, 0, 1, 5, 1, 1),
(3569465, 95877, 5, 1, 10, -95877, 1);
 
 
 DELETE FROM `creature_template_currency` WHERE `entry` IN(3169465,3269465,3369465,3469465);
 INSERT INTO `creature_template_currency` VALUES
(3169465, 396, 40),
(3269465, 396, 40),
(3369465, 396, 40),
(3469465, 396, 40);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Spawns
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`npcflag2`,`unit_flags`,`unit_flags2`,`dynamicflags`,`isActive`,`protec_anti_doublet`) VALUES
(@CGUID+1,  69388, 1098, 0, 0, 248, 1, 0, 69388, 5917.69, 6525.49, 112.228, 5.97571, 86400, 0, 0, 16863680, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+2,  69388, 1098, 0, 0, 248, 1, 0, 69388, 5860.51, 6522.21, 112.261, 4.64341, 86400, 0, 0, 16863680, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+3,  69388, 1098, 0, 0, 248, 1, 0, 69388, 5873.17, 6494.15, 118.106, 1.42956, 86400, 0, 0, 16863680, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+4,  69388, 1098, 0, 0, 248, 1, 0, 69388, 5840.67, 6482.09, 117.996, 3.21006, 86400, 0, 0, 16863680, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+5,  69388, 1098, 0, 0, 248, 1, 0, 69388, 5946.51, 6475.25, 118.107, 0.0849757, 86400, 0, 0, 16863680, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),

(@CGUID+6,  69390, 1098, 0, 0, 248, 1, 0, 69390, 5909.45, 6555.49, 112.261, 2.07639, 86400, 0, 0, 13912536, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+7,  69390, 1098, 0, 0, 248, 1, 0, 69390, 5882.01, 6554.81, 112.261, 3.69621, 86400, 0, 0, 13912536, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+8,  69390, 1098, 0, 0, 248, 1, 0, 69390, 5943.90, 6491.32, 118.107, 6.25192, 86400, 0, 0, 13912536, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),

(@CGUID+9,  69455, 1098, 0, 0, 248, 1, 0, 69455, 5905.52, 6556.86, 112.260, 0.51738, 86400, 0, 0, 9696616, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+10, 69455, 1098, 0, 0, 248, 1, 0, 69455, 5875.67, 6550.89, 112.261, 1.04235, 86400, 0, 0, 9696616, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+11, 69455, 1098, 0, 0, 248, 1, 0, 69455, 5867.4, 6490.76, 118.106, 0.868784, 86400, 0, 0, 9696616, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+12, 69455, 1098, 0, 0, 248, 1, 0, 69455, 5947.27, 6494.46, 118.107, 5.41233, 86400, 0, 0, 9696616, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),

(@CGUID+13, 69465, 1098, 0, 0, 248, 1, 0, 0, 5891.82, 6263.38, 124.034, 1.56886, 604800, 0, 0, 207601216, 0, 0, 0, 0, 0, 0, 0, 1, NULL),

(@CGUID+14, 69467, 1098, 0, 0, 248, 1, 0, 0, 5839.32, 6316.74, 156.822, 5.49425, 7200, 0, 0, 84, 0, 0, 0, 0, 0, 0, 0, 1, NULL),
(@CGUID+15, 69467, 1098, 0, 0, 248, 1, 0, 0, 5945.87, 6318.07, 156.822, 3.93524, 7201, 0, 0, 84, 0, 0, 0, 0, 0, 0, 0, 1, NULL),
(@CGUID+16, 69467, 1098, 0, 0, 248, 1, 0, 0, 5943.92, 6210.56, 156.822, 2.43905, 7202, 0, 0, 84, 0, 0, 0, 0, 0, 0, 0, 1, NULL),
(@CGUID+17, 69467, 1098, 0, 0, 248, 1, 0, 0, 5839.16, 6210.36, 156.822, 0.821134, 7203, 0, 0, 84, 0, 0, 0, 0, 0, 0, 0, 1, NULL),

(@CGUID+18, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5915.01, 6555.70, 112.262, 2.41254, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+19, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5929.73, 6543.16, 112.261, 3.72292, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+20, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5931.22, 6538.27, 112.261, 3.38913, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+21, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5930.63, 6529.92, 112.253, 3.03963, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+22, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5918.55, 6518.21, 112.259, 2.18511, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+23, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5925.01, 6518.61, 112.259, 2.50555, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+24, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5926.01, 6523.48, 112.258, 2.75531, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+25, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5873.91, 6558.49, 112.737, 1.24184, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+26, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5853.16, 6520.82, 112.261, 4.89002, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+27, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5878.06, 6489.66, 118.107, 1.66440, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+28, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5830.94, 6476.99, 118.107, 3.21948, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+29, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5830.65, 6493.49, 118.107, 3.17707, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+30, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5831.48, 6469.27, 118.107, 3.11220, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+31, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5830.66, 6486.52, 118.107, 3.22608, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+32, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5955.64, 6492.72, 118.107, 3.26741, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+33, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5953.43, 6489.91, 118.107, 2.62966, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+34, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5953.10, 6485.33, 118.107, 2.42625, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+35, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5948.62, 6483.84, 118.107, 2.19141, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+36, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5955.83, 6479.02, 118.107, 3.51874, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+37, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5955.42, 6472.66, 118.107, 2.79224, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+38, 70230, 1098, 0, 0, 248, 1, 0, 70230, 5951.30, 6468.36, 118.107, 2.33750, 86400, 0, 0, 5059104, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),

(@CGUID+39, 70236, 1098, 0, 0, 248, 1, 0, 70236, 5892.01, 6436.98, 118.106, 1.61915, 86400, 0, 0, 21079600, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),

(@CGUID+40, 70245, 1098, 0, 0, 248, 1, 0, 0, 5859.92, 6515.6, 112.261, 1.54454, 86400, 0, 0, 144628, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+41, 70245, 1098, 0, 0, 248, 1, 0, 0, 5852.87, 6515.65, 112.261, 1.58539, 86400, 0, 0, 144628, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+42, 70245, 1098, 0, 0, 248, 1, 0, 0, 5824.56, 6493.54, 118.107, 0.0295164, 86400, 0, 0, 144628, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+43, 70245, 1098, 0, 0, 248, 1, 0, 0, 5824.91, 6486.64, 118.107, 0.0295164, 86400, 0, 0, 144628, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+44, 70245, 1098, 0, 0, 248, 1, 0, 0, 5824.89, 6477.12, 118.107, 6.26951, 86400, 0, 0, 144628, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+45, 70245, 1098, 0, 0, 248, 1, 0, 0, 5825.11, 6469.8, 118.107, 6.26558, 86400, 0, 0, 144628, 0, 0, 0, 0, 0, 2048, 0, 0, NULL);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Auras
DELETE FROM creature_template_aura WHERE entry IN (69395,69438);
INSERT INTO creature_template_aura VALUES
(69395, 137087),
(69438, 137058);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Emote
DELETE FROM `creature_emote` WHERE `guid` IN (@CGUID+2,@CGUID+19,@CGUID+20,@CGUID+21,@CGUID+22,@CGUID+23,@CGUID+24,@CGUID+26,@CGUID+28,@CGUID+29,@CGUID+30,@CGUID+31,@CGUID+32,@CGUID+33,@CGUID+34,@CGUID+35,@CGUID+36,@CGUID+37,@CGUID+38);
INSERT INTO `creature_emote` VALUES
(@CGUID+2,333),
(@CGUID+19,333),
(@CGUID+20,333),
(@CGUID+21,333),
(@CGUID+22,333),
(@CGUID+23,333),
(@CGUID+24,333),
(@CGUID+26,333),
(@CGUID+28,333),
(@CGUID+29,333),
(@CGUID+30,333),
(@CGUID+31,333),
(@CGUID+32,333),
(@CGUID+33,333),
(@CGUID+34,333),
(@CGUID+35,333),
(@CGUID+36,333),
(@CGUID+37,333),
(@CGUID+38,333);

DELETE FROM `creature_bytes` WHERE `guid` IN (@CGUID+18);
INSERT INTO `creature_bytes` VALUES
(@CGUID+18,0,8);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Gameobjects templates
REPLACE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `AIName`, `ScriptName`, `WDBVerified`) VALUES
(214539, 31, 11469, 'Instance Portal (Raid 4 Difficulties)', '', '', '', 0, 16, 2.5, 0, 0, 0, 0, 0, 0, 2, 216, 217, 216, 217, 11471, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218664, 0, 13679, 'Wrought Iron Door', '', '', '', 1375, 48, 1.1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218665, 0, 13679, 'Wrought Iron Door', '', '', '', 1375, 32, 1.1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218667, 0, 13679, 'Wrought Iron Door', '', '', '', 1375, 48, 1.1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218675, 0, 13615, 'Mogu Fountain', '', '', '', 1375, 48, 12.5537, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218676, 0, 13615, 'Mogu Fountain', '', '', '', 1375, 48, 12.5537, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218677, 0, 13615, 'Mogu Fountain', '', '', '', 1375, 48, 12.5537, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218678, 0, 13615, 'Mogu Fountain', '', '', '', 1375, 48, 12.5537, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Gameobjects spawns
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(@OGUID+1, 214539, 1098, 760, 1, 5892.22, 6630.41, 106.427, 4.64664, 0, 0, 0, 1, 86400, 255, 1),
(@OGUID+2, 218664, 1098, 760, 1, 5891.77, 6349, 124.068, 4.71239, 0, 0, 1, -4.37114E-8, 86400, 255, 0),
(@OGUID+3, 218665, 1098, 760, 1, 5891.77, 6410.01, 124.068, 4.71239, 0, 0, 1, -0.0000000437114, 86400, 255, 1),
(@OGUID+4, 218667, 1098, 760, 1, 5806.350, 6263.49, 124.021, 3.13417, 0, 0, 0.999993, 0.00371344, 86400, 255, 1),
(@OGUID+5, 218675, 1098, 760, 1, 5840.52, 6315.62, 125.112, 5.49779, 0, 0, 1, -4.37114E-8, 7200, 255, 1),
(@OGUID+6, 218676, 1098, 760, 1, 5839.67, 6212.19, 125.112, 0.785397, 0, 0, 1, -4.37114E-8, 7203, 255, 1),
(@OGUID+7, 218677, 1098, 760, 1, 5943.1, 6211.33, 125.112, 2.35619, 0, 0, 1, -4.37114E-8, 7202, 255, 1),
(@OGUID+8, 218678, 1098, 760, 1, 5945.27, 6316.69, 125.194, 3.92699, 0, 0, 1, -4.37114E-8, 7201, 255, 1);
/*---------------------------------------------------------------------------------------------------------------*/
/* END Jin'rokh the Breaker */





/* BEGIN Horridon */
/*---------------------------------------------------------------------------------------------------------------*/
-- Spell Scripts
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(136723,'spell_horridon_sand_trap'),
(136739,'spell_horridon_double_swipe'),
(136740,'spell_horridon_double_swipe'),
(137433,'spell_control_horridon'),
(137442,'spell_control_horridon'),
(137443,'spell_control_horridon'),
(137444,'spell_control_horridon'),
(136480,'spell_horridon_chain_lightning'),
(137294,'spell_headache'),
(136741,'spell_double_swipe_aura'),
(136770,'spell_double_swipe_aura');
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Templates
REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`,`equipment_id`) VALUES 
(54020, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'General Purpose Bunny JMF (Look 2 - Flying, Huge AOI)', '', '', 0, 1, 1, 4, 35, 35, 0, 1, 1.14286, 1, 0, 1, 2, 0, 0, 4, 2000, 2000, 1, 0, 37750784, 0, 0, 0, 0, 0, 0, 1, 2, 0, 10, 1024, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 128, '', 16048,0),
(69164, 0, 0, 0, 0, 0, 47378, 47263, 47379, 47264, 'Gurubashi Venom Priest', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 9.75, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69164),
(69167, 0, 0, 0, 0, 0, 47442, 47443, 47444, 47445, 'Gurubashi Bloodlord', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69167),
(69168, 0, 0, 0, 0, 0, 22307, 22308, 47335, 47336, 'Amani\'shi Flame Caster', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69168),
(69169, 0, 0, 0, 0, 0, 22263, 22264, 47337, 47338, 'Amani\'shi Protector', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69169),
(69172, 0, 0, 0, 0, 0, 47469, 45463, 0, 0, 'Sul\'lithuz Stonegazer', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2099200, 0, 130, 0, 0, 0, 0, 0, 0, 0, 1, 2097248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,0),
(69173, 0, 0, 0, 0, 0, 47559, 47560, 47561, 47562, 'Farraki Skirmisher', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69173),
(69175, 0, 0, 0, 0, 0, 47475, 47477, 47478, 47479, 'Farraki Wastewalker', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69175),
(69176, 0, 0, 0, 0, 0, 47331, 47332, 47333, 47334, 'Amani\'shi Beast Shaman', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69176),
(69177, 0, 0, 0, 0, 0, 47341, 0, 0, 0, 'Amani Warbear', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 0.992063, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 1, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2666, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,0),
(69178, 0, 0, 0, 0, 0, 47447, 47448, 47449, 47450, 'Drakkari Frozen Warlord', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 16.2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69178),
(69184, 0, 0, 0, 0, 0, 47446, 27056, 47558, 0, 'Risen Drakkari Warrior', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.42857, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, '', 17614,69184),
(69185, 0, 0, 0, 0, 0, 47556, 47557, 0, 0, 'Risen Drakkari Champion', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.42857, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, '', 17614,69185),
(69215, 0, 0, 0, 0, 0, 47354, 0, 0, 0, 'Lightning Nova Totem', '', '', 0, 93, 93, 4, 16, 16, 0, 0.01, 0.01, 1, 0, 9839, 14339, 0, 42299, 1, 2000, 2000, 1, 131076, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 5, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,0),
(69221, 0, 0, 0, 0, 0, 47494, 47495, 47496, 47497, 'Zandalari Dinomancer', '', '', 0, 92, 92, 4, 16, 16, 0, 0.666668, 2, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 0, 0, '', 17614,69221),
(69268, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Frozen Orb', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 0, 9839, 14339, 0, 42299, 1, 2000, 2000, 1, 33554434, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1074791424, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'mob_horridon_summons', 17614,0),
(69313, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Living Poison', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 0, 9839, 14339, 0, 42299, 1, 2000, 2000, 1, 33554434, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1074791424, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 94, 1, 0, 0, 'mob_horridon_summons', 17614,0),
(69314, 0, 0, 0, 0, 0, 47378, 47263, 47379, 47264, 'Venomous Effusion', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'mob_horridon_summons', 17614,0),
(69346, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Sand Trap', '', '', 0, 93, 93, 4, 14, 14, 0, 0.01, 0.01, 1, 0, 9839, 14339, 0, 42299, 1, 2000, 2000, 1, 33554438, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1091568640, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 128, 'mob_horridon_summons', 17614,0),
(69374, 0, 0, 0, 0, 0, 47502, 0, 0, 0, 'War-God Jalak', 'Ward of Beasts', '', 0, 93, 93, 4, 16, 16, 0, 1, 2, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 40, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 0, 0, '', 17614,69374),
(68476, 0, 0, 0, 0, 0, 47325, 0, 0, 0, 'Horridon', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 2, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 1, 0, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000000, 3100000, '', 0, 3, 1, 820, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 667893759, 1, '', 17614,0),
(70246, 0, 0, 0, 0, 0, 47951, 0, 0, 0, 'Spirit Flayer', '', '', 0, 90, 90, 4, 14, 14, 0, 0.5, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 131074, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 2, 4, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,70246),
(70308, 0, 0, 0, 0, 0, 47998, 0, 0, 0, 'Soul-Fed Construct', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 80, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 650854271, 0, '', 17614,0),
(70341, 0, 0, 0, 0, 0, 48048, 0, 0, 0, 'Tormented Spirit', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 0.992063, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8388624, 0, '', 1,0),
(70441, 0, 0, 0, 0, 0, 47693, 0, 0, 0, 'Lost Soul', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 0.992063, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8388624, 0, '', 1,0),
(70445, 0, 0, 0, 0, 0, 48049, 0, 0, 0, 'Stormbringer Draz\'kil', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 70, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 650854271, 0, '', 17614,70445),
(70448, 0, 0, 0, 0, 0, 48052, 48051, 48050, 0, 'Ancient Python', '', '', 0, 90, 90, 4, 16, 16, 0, 1, 1.42857, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2048, 0, 35, 0, 0, 0, 0, 0, 0, 0, 1, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 5, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, '', 17614,0),
(662202, 0, 0, 0, 0, 0, 1287, 0, 0, 0, 'stalker_horridon_event', '', '', 0, 1, 1, 0, 35, 35, 0, 1, 1.14286, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 128, 'npc_horridon_event_helper', 1,0),
(662206, 0, 0, 0, 0, 0, 1287, 0, 0, 0, 'stalker_horridon_intro', '', '', 0, 93, 93, 0, 35, 35, 0, 1, 1.14286, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 128, 'npc_horridon_rp_event_helper', 1,0),

(@RAID_DIFF_10N + 69164, 0, 0, 0, 0, 0, 47378, 47263, 47379, 47264, 'Gurubashi Venom Priest', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 9.75, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69164),
(@RAID_DIFF_10N + 69167, 0, 0, 0, 0, 0, 47442, 47443, 47444, 47445, 'Gurubashi Bloodlord', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69167),
(@RAID_DIFF_10N + 69168, 0, 0, 0, 0, 0, 22307, 22308, 47335, 47336, 'Amani\'shi Flame Caster', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69168),
(@RAID_DIFF_10N + 69169, 0, 0, 0, 0, 0, 22263, 22264, 47337, 47338, 'Amani\'shi Protector', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69169),
(@RAID_DIFF_10N + 69172, 0, 0, 0, 0, 0, 47469, 45463, 0, 0, 'Sul\'lithuz Stonegazer', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2099200, 0, 130, 0, 0, 0, 0, 0, 0, 0, 1, 2097248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_10N + 69173, 0, 0, 0, 0, 0, 47559, 47560, 47561, 47562, 'Farraki Skirmisher', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69173),
(@RAID_DIFF_10N + 69175, 0, 0, 0, 0, 0, 47475, 47477, 47478, 47479, 'Farraki Wastewalker', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69175),
(@RAID_DIFF_10N + 69176, 0, 0, 0, 0, 0, 47331, 47332, 47333, 47334, 'Amani\'shi Beast Shaman', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69176),
(@RAID_DIFF_10N + 69177, 0, 0, 0, 0, 0, 47341, 0, 0, 0, 'Amani Warbear', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 0.992063, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 1, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2666, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_10N + 69178, 0, 0, 0, 0, 0, 47447, 47448, 47449, 47450, 'Drakkari Frozen Warlord', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 16.2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69178),
(@RAID_DIFF_10N + 69184, 0, 0, 0, 0, 0, 47446, 27056, 47558, 0, 'Risen Drakkari Warrior', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.42857, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, '', 17614,69184),
(@RAID_DIFF_10N + 69185, 0, 0, 0, 0, 0, 47556, 47557, 0, 0, 'Risen Drakkari Champion', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.42857, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, '', 17614,69185),
(@RAID_DIFF_10N + 69221, 0, 0, 0, 0, 0, 47494, 47495, 47496, 47497, 'Zandalari Dinomancer', '', '', 0, 92, 92, 4, 16, 16, 0, 0.666668, 2, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 0, 0, '', 17614,69221),
(@RAID_DIFF_10N + 69314, 0, 0, 0, 0, 0, 47378, 47263, 47379, 47264, 'Venomous Effusion', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'mob_horridon_summons', 17614,0),
(@RAID_DIFF_10N + 69374, 0, 0, 0, 0, 0, 47502, 0, 0, 0, 'War-God Jalak', 'Ward of Beasts', '', 0, 93, 93, 4, 16, 16, 0, 1, 2, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 40, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 0, 0, '', 17614,69374),
(@RAID_DIFF_10N + 68476, 0, 0, 0, 0, 0, 47325, 0, 0, 0, 'Horridon', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 2, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 1, 0, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000000, 3100000, '', 0, 3, 1, 820, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 667893759, 1, '', 17614,0),
(@RAID_DIFF_10N + 70308, 0, 0, 0, 0, 0, 47998, 0, 0, 0, 'Soul-Fed Construct', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 80, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 650854271, 0, '', 17614,0),
(@RAID_DIFF_10N + 70341, 0, 0, 0, 0, 0, 48048, 0, 0, 0, 'Tormented Spirit', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 0.992063, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8388624, 0, '', 1,0),
(@RAID_DIFF_10N + 70445, 0, 0, 0, 0, 0, 48049, 0, 0, 0, 'Stormbringer Draz\'kil', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 70, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 650854271, 0, '', 17614,70445),
(@RAID_DIFF_10N + 70448, 0, 0, 0, 0, 0, 48052, 48051, 48050, 0, 'Ancient Python', '', '', 0, 90, 90, 4, 16, 16, 0, 1, 1.42857, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2048, 0, 35, 0, 0, 0, 0, 0, 0, 0, 1, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 5, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, '', 17614,0),

(@RAID_DIFF_25N + 69164, 0, 0, 0, 0, 0, 47378, 47263, 47379, 47264, 'Gurubashi Venom Priest', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 9.75, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69164),
(@RAID_DIFF_25N + 69167, 0, 0, 0, 0, 0, 47442, 47443, 47444, 47445, 'Gurubashi Bloodlord', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69167),
(@RAID_DIFF_25N + 69168, 0, 0, 0, 0, 0, 22307, 22308, 47335, 47336, 'Amani\'shi Flame Caster', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69168),
(@RAID_DIFF_25N + 69169, 0, 0, 0, 0, 0, 22263, 22264, 47337, 47338, 'Amani\'shi Protector', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69169),
(@RAID_DIFF_25N + 69172, 0, 0, 0, 0, 0, 47469, 45463, 0, 0, 'Sul\'lithuz Stonegazer', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2099200, 0, 130, 0, 0, 0, 0, 0, 0, 0, 1, 2097248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_25N + 69173, 0, 0, 0, 0, 0, 47559, 47560, 47561, 47562, 'Farraki Skirmisher', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69173),
(@RAID_DIFF_25N + 69175, 0, 0, 0, 0, 0, 47475, 47477, 47478, 47479, 'Farraki Wastewalker', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69175),
(@RAID_DIFF_25N + 69176, 0, 0, 0, 0, 0, 47331, 47332, 47333, 47334, 'Amani\'shi Beast Shaman', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69176),
(@RAID_DIFF_25N + 69177, 0, 0, 0, 0, 0, 47341, 0, 0, 0, 'Amani Warbear', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 0.992063, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 1, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2666, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_25N + 69178, 0, 0, 0, 0, 0, 47447, 47448, 47449, 47450, 'Drakkari Frozen Warlord', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 16.2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69178),
(@RAID_DIFF_25N + 69184, 0, 0, 0, 0, 0, 47446, 27056, 47558, 0, 'Risen Drakkari Warrior', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.42857, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, '', 17614,69184),
(@RAID_DIFF_25N + 69185, 0, 0, 0, 0, 0, 47556, 47557, 0, 0, 'Risen Drakkari Champion', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.42857, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, '', 17614,69185),
(@RAID_DIFF_25N + 69221, 0, 0, 0, 0, 0, 47494, 47495, 47496, 47497, 'Zandalari Dinomancer', '', '', 0, 92, 92, 4, 16, 16, 0, 0.666668, 2, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 0, 0, '', 17614,69221),
(@RAID_DIFF_25N + 69314, 0, 0, 0, 0, 0, 47378, 47263, 47379, 47264, 'Venomous Effusion', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'mob_horridon_summons', 17614,0),
(@RAID_DIFF_25N + 69374, 0, 0, 0, 0, 0, 47502, 0, 0, 0, 'War-God Jalak', 'Ward of Beasts', '', 0, 93, 93, 4, 16, 16, 0, 1, 2, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 40, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 0, 0, '', 17614,69374),
(@RAID_DIFF_25N + 68476, 0, 0, 0, 0, 0, 47325, 0, 0, 0, 'Horridon', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 2, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 1, 0, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000000, 3100000, '', 0, 3, 1, 820, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 667893759, 1, '', 17614,0),
(@RAID_DIFF_25N + 70308, 0, 0, 0, 0, 0, 47998, 0, 0, 0, 'Soul-Fed Construct', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 80, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 650854271, 0, '', 17614,0),
(@RAID_DIFF_25N + 70341, 0, 0, 0, 0, 0, 48048, 0, 0, 0, 'Tormented Spirit', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 0.992063, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8388624, 0, '', 1,0),
(@RAID_DIFF_25N + 70445, 0, 0, 0, 0, 0, 48049, 0, 0, 0, 'Stormbringer Draz\'kil', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 70, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 650854271, 0, '', 17614,70445),
(@RAID_DIFF_25N + 70448, 0, 0, 0, 0, 0, 48052, 48051, 48050, 0, 'Ancient Python', '', '', 0, 90, 90, 4, 16, 16, 0, 1, 1.42857, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2048, 0, 35, 0, 0, 0, 0, 0, 0, 0, 1, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 5, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, '', 17614,0),

(@RAID_DIFF_10H + 69164, 0, 0, 0, 0, 0, 47378, 47263, 47379, 47264, 'Gurubashi Venom Priest', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 9.75, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69164),
(@RAID_DIFF_10H + 69167, 0, 0, 0, 0, 0, 47442, 47443, 47444, 47445, 'Gurubashi Bloodlord', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69167),
(@RAID_DIFF_10H + 69168, 0, 0, 0, 0, 0, 22307, 22308, 47335, 47336, 'Amani\'shi Flame Caster', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69168),
(@RAID_DIFF_10H + 69169, 0, 0, 0, 0, 0, 22263, 22264, 47337, 47338, 'Amani\'shi Protector', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69169),
(@RAID_DIFF_10H + 69172, 0, 0, 0, 0, 0, 47469, 45463, 0, 0, 'Sul\'lithuz Stonegazer', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2099200, 0, 130, 0, 0, 0, 0, 0, 0, 0, 1, 2097248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_10H + 69173, 0, 0, 0, 0, 0, 47559, 47560, 47561, 47562, 'Farraki Skirmisher', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69173),
(@RAID_DIFF_10H + 69175, 0, 0, 0, 0, 0, 47475, 47477, 47478, 47479, 'Farraki Wastewalker', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69175),
(@RAID_DIFF_10H + 69176, 0, 0, 0, 0, 0, 47331, 47332, 47333, 47334, 'Amani\'shi Beast Shaman', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69176),
(@RAID_DIFF_10H + 69177, 0, 0, 0, 0, 0, 47341, 0, 0, 0, 'Amani Warbear', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 0.992063, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 1, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2666, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_10H + 69178, 0, 0, 0, 0, 0, 47447, 47448, 47449, 47450, 'Drakkari Frozen Warlord', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 16.2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69178),
(@RAID_DIFF_10H + 69184, 0, 0, 0, 0, 0, 47446, 27056, 47558, 0, 'Risen Drakkari Warrior', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.42857, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, '', 17614,69184),
(@RAID_DIFF_10H + 69185, 0, 0, 0, 0, 0, 47556, 47557, 0, 0, 'Risen Drakkari Champion', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.42857, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, '', 17614,69185),
(@RAID_DIFF_10H + 69221, 0, 0, 0, 0, 0, 47494, 47495, 47496, 47497, 'Zandalari Dinomancer', '', '', 0, 92, 92, 4, 16, 16, 0, 0.666668, 2, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 0, 0, '', 17614,69221),
(@RAID_DIFF_10H + 69314, 0, 0, 0, 0, 0, 47378, 47263, 47379, 47264, 'Venomous Effusion', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'mob_horridon_summons', 17614,0),
(@RAID_DIFF_10H + 69374, 0, 0, 0, 0, 0, 47502, 0, 0, 0, 'War-God Jalak', 'Ward of Beasts', '', 0, 93, 93, 4, 16, 16, 0, 1, 2, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 40, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 0, 0, '', 17614,69374),
(@RAID_DIFF_10H + 68476, 0, 0, 0, 0, 0, 47325, 0, 0, 0, 'Horridon', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 2, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 1, 0, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000000, 3100000, '', 0, 3, 1, 820, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 667893759, 1, '', 17614,0),
(@RAID_DIFF_10H + 70308, 0, 0, 0, 0, 0, 47998, 0, 0, 0, 'Soul-Fed Construct', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 80, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 650854271, 0, '', 17614,0),
(@RAID_DIFF_10H + 70341, 0, 0, 0, 0, 0, 48048, 0, 0, 0, 'Tormented Spirit', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 0.992063, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8388624, 0, '', 1,0),
(@RAID_DIFF_10H + 70445, 0, 0, 0, 0, 0, 48049, 0, 0, 0, 'Stormbringer Draz\'kil', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 70, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 650854271, 0, '', 17614,70445),
(@RAID_DIFF_10H + 70448, 0, 0, 0, 0, 0, 48052, 48051, 48050, 0, 'Ancient Python', '', '', 0, 90, 90, 4, 16, 16, 0, 1, 1.42857, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2048, 0, 35, 0, 0, 0, 0, 0, 0, 0, 1, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 5, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, '', 17614,0),

(@RAID_DIFF_25H + 69164, 0, 0, 0, 0, 0, 47378, 47263, 47379, 47264, 'Gurubashi Venom Priest', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 9.75, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69164),
(@RAID_DIFF_25H + 69167, 0, 0, 0, 0, 0, 47442, 47443, 47444, 47445, 'Gurubashi Bloodlord', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69167),
(@RAID_DIFF_25H + 69168, 0, 0, 0, 0, 0, 22307, 22308, 47335, 47336, 'Amani\'shi Flame Caster', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69168),
(@RAID_DIFF_25H + 69169, 0, 0, 0, 0, 0, 22263, 22264, 47337, 47338, 'Amani\'shi Protector', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69169),
(@RAID_DIFF_25H + 69172, 0, 0, 0, 0, 0, 47469, 45463, 0, 0, 'Sul\'lithuz Stonegazer', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2099200, 0, 130, 0, 0, 0, 0, 0, 0, 0, 1, 2097248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_25H + 69173, 0, 0, 0, 0, 0, 47559, 47560, 47561, 47562, 'Farraki Skirmisher', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69173),
(@RAID_DIFF_25H + 69175, 0, 0, 0, 0, 0, 47475, 47477, 47478, 47479, 'Farraki Wastewalker', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69175),
(@RAID_DIFF_25H + 69176, 0, 0, 0, 0, 0, 47331, 47332, 47333, 47334, 'Amani\'shi Beast Shaman', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69176),
(@RAID_DIFF_25H + 69177, 0, 0, 0, 0, 0, 47341, 0, 0, 0, 'Amani Warbear', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 0.992063, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 1, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2666, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_25H + 69178, 0, 0, 0, 0, 0, 47447, 47448, 47449, 47450, 'Drakkari Frozen Warlord', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 16.2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69178),
(@RAID_DIFF_25H + 69184, 0, 0, 0, 0, 0, 47446, 27056, 47558, 0, 'Risen Drakkari Warrior', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.42857, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, '', 17614,69184),
(@RAID_DIFF_25H + 69185, 0, 0, 0, 0, 0, 47556, 47557, 0, 0, 'Risen Drakkari Champion', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.42857, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, '', 17614,69185),
(@RAID_DIFF_25H + 69221, 0, 0, 0, 0, 0, 47494, 47495, 47496, 47497, 'Zandalari Dinomancer', '', '', 0, 92, 92, 4, 16, 16, 0, 0.666668, 2, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 0, 0, '', 17614,69221),
(@RAID_DIFF_25H + 69314, 0, 0, 0, 0, 0, 47378, 47263, 47379, 47264, 'Venomous Effusion', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'mob_horridon_summons', 17614,0),
(@RAID_DIFF_25H + 69374, 0, 0, 0, 0, 0, 47502, 0, 0, 0, 'War-God Jalak', 'Ward of Beasts', '', 0, 93, 93, 4, 16, 16, 0, 1, 2, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 40, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 0, 0, '', 17614,69374),
(@RAID_DIFF_25H + 68476, 0, 0, 0, 0, 0, 47325, 0, 0, 0, 'Horridon', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 2, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 1, 0, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000000, 3100000, '', 0, 3, 1, 820, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 667893759, 1, '', 17614,0),
(@RAID_DIFF_25H + 70308, 0, 0, 0, 0, 0, 47998, 0, 0, 0, 'Soul-Fed Construct', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 80, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 650854271, 0, '', 17614,0),
(@RAID_DIFF_25H + 70341, 0, 0, 0, 0, 0, 48048, 0, 0, 0, 'Tormented Spirit', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 0.992063, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8388624, 0, '', 1,0),
(@RAID_DIFF_25H + 70445, 0, 0, 0, 0, 0, 48049, 0, 0, 0, 'Stormbringer Draz\'kil', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 70, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 650854271, 0, '', 17614,70445),
(@RAID_DIFF_25H + 70448, 0, 0, 0, 0, 0, 48052, 48051, 48050, 0, 'Ancient Python', '', '', 0, 90, 90, 4, 16, 16, 0, 1, 1.42857, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2048, 0, 35, 0, 0, 0, 0, 0, 0, 0, 1, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 5, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, '', 17614,0),

(@RAID_DIFF_25R + 69164, 0, 0, 0, 0, 0, 47378, 47263, 47379, 47264, 'Gurubashi Venom Priest', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 9.75, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69164),
(@RAID_DIFF_25R + 69167, 0, 0, 0, 0, 0, 47442, 47443, 47444, 47445, 'Gurubashi Bloodlord', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69167),
(@RAID_DIFF_25R + 69168, 0, 0, 0, 0, 0, 22307, 22308, 47335, 47336, 'Amani\'shi Flame Caster', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69168),
(@RAID_DIFF_25R + 69169, 0, 0, 0, 0, 0, 22263, 22264, 47337, 47338, 'Amani\'shi Protector', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69169),
(@RAID_DIFF_25R + 69172, 0, 0, 0, 0, 0, 47469, 45463, 0, 0, 'Sul\'lithuz Stonegazer', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2099200, 0, 130, 0, 0, 0, 0, 0, 0, 0, 1, 2097248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_25R + 69173, 0, 0, 0, 0, 0, 47559, 47560, 47561, 47562, 'Farraki Skirmisher', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69173),
(@RAID_DIFF_25R + 69175, 0, 0, 0, 0, 0, 47475, 47477, 47478, 47479, 'Farraki Wastewalker', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69175),
(@RAID_DIFF_25R + 69176, 0, 0, 0, 0, 0, 47331, 47332, 47333, 47334, 'Amani\'shi Beast Shaman', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69176),
(@RAID_DIFF_25R + 69177, 0, 0, 0, 0, 0, 47341, 0, 0, 0, 'Amani Warbear', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 0.992063, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 1, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2666, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_25R + 69178, 0, 0, 0, 0, 0, 47447, 47448, 47449, 47450, 'Drakkari Frozen Warlord', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 16.2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,69178),
(@RAID_DIFF_25R + 69184, 0, 0, 0, 0, 0, 47446, 27056, 47558, 0, 'Risen Drakkari Warrior', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.42857, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, '', 17614,69184),
(@RAID_DIFF_25R + 69185, 0, 0, 0, 0, 0, 47556, 47557, 0, 0, 'Risen Drakkari Champion', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.42857, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, '', 17614,69185),
(@RAID_DIFF_25R + 69221, 0, 0, 0, 0, 0, 47494, 47495, 47496, 47497, 'Zandalari Dinomancer', '', '', 0, 92, 92, 4, 16, 16, 0, 0.666668, 2, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 0, 0, '', 17614,69221),
(@RAID_DIFF_25R + 69314, 0, 0, 0, 0, 0, 47378, 47263, 47379, 47264, 'Venomous Effusion', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'mob_horridon_summons', 17614,0),
(@RAID_DIFF_25R + 69374, 0, 0, 0, 0, 0, 47502, 0, 0, 0, 'War-God Jalak', 'Ward of Beasts', '', 0, 93, 93, 4, 16, 16, 0, 1, 2, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 40, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 0, 0, '', 17614,69374),
(@RAID_DIFF_25R + 68476, 0, 0, 0, 0, 0, 47325, 0, 0, 0, 'Horridon', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 2, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 1, 0, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000000, 3100000, '', 0, 3, 1, 820, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 667893759, 1, '', 17614,0),
(@RAID_DIFF_25R + 70308, 0, 0, 0, 0, 0, 47998, 0, 0, 0, 'Soul-Fed Construct', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 80, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 650854271, 0, '', 17614,0),
(@RAID_DIFF_25R + 70341, 0, 0, 0, 0, 0, 48048, 0, 0, 0, 'Tormented Spirit', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 0.992063, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8388624, 0, '', 1,0),
(@RAID_DIFF_25R + 70445, 0, 0, 0, 0, 0, 48049, 0, 0, 0, 'Stormbringer Draz\'kil', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 70, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 650854271, 0, '', 17614,70445),
(@RAID_DIFF_25R + 70448, 0, 0, 0, 0, 0, 48052, 48051, 48050, 0, 'Ancient Python', '', '', 0, 90, 90, 4, 16, 16, 0, 1, 1.42857, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 2048, 0, 35, 0, 0, 0, 0, 0, 0, 0, 1, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 5, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, '', 17614,0);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Updates
-- Creature Trash Mobs Damage, HP, LootId
-- Base HP Gamepedia Raid 25N
-- 25R -10%
-- 25H +25%
-- 10N (BASE HP: 25 = X : 10)
-- 10H=(10N+25%)

UPDATE `creature_template` SET `ScriptName` = 'mob_horridon_trashs' WHERE `entry` IN 
(69164,69167,69168,69169,69172,69173,69175,69177,69178,69184,69185);

UPDATE `creature_template` SET `ScriptName` = 'mob_horridon_summons' WHERE `entry` IN 
(69268,69313,69314,69346);
UPDATE `creature_template` SET `ScriptName` = 'npc_living_poison' WHERE `entry` = 69313;
UPDATE `creature_template` SET `ScriptName` = 'npc_venomous_effusion' WHERE `entry` = 69314;
UPDATE `creature_template` SET `ScriptName` = 'npc_amani_shi_warbear' WHERE `entry` = 69177;

UPDATE `creature_template` SET `ScriptName` = 'mob_direhorn_spirit' WHERE `entry` = 70688;
UPDATE `creature_template` SET `ScriptName` = 'mob_zandalari_dinomancer' WHERE `entry` = 69221;

UPDATE `creature_template` SET `ScriptName` = 'boss_horridon' WHERE `entry` = 68476;
UPDATE `creature_template` SET `ScriptName` = 'mob_war_god_jalak' WHERE `entry` = 69374;

-- Difficulty entries bosses and Trash Mobs
UPDATE creature_template SET difficulty_entry_3 = @RAID_DIFF_10N + entry, difficulty_entry_4 = @RAID_DIFF_25N + entry, difficulty_entry_5 = @RAID_DIFF_10H + entry, difficulty_entry_6 = @RAID_DIFF_25H + entry, difficulty_entry_7 = @RAID_DIFF_25R + entry WHERE entry IN 
(68476,69164,69167,69168,69169,69172,69173,69175,69176,69177,69178,69184,69185,69221,69314,69374,70308,70341,70445,70448);

-- Immunity and bind bosses
UPDATE creature_template SET flags_extra = flags_extra | 1, mechanic_immune_mask = 667893759 WHERE entry IN 
(68476,3168476,3268476,3368476,3468476,3568476,69374,3169374,3269374,3369374,3469374,3569374);

-- Gurubashi Venom Priest
UPDATE creature_template SET LootId = 0, dmg_multiplier = 4, Health_Mod = 5059104  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 69164;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 6323880  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 69164;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 7, Health_Mod = 12647760 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 69164;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 8, Health_Mod = 15809700 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 69164;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 11382984 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 69164;

-- Gurubashi Bloodlord
UPDATE creature_template SET LootId = 0, dmg_multiplier = 4, Health_Mod = 5059104  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 69167;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 6323880  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 69167;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 7, Health_Mod = 12647760 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 69167;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 8, Health_Mod = 15809700 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 69167;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 11382984 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 69167;

-- Amani'shi Flame Caster
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 1956154  / @HP_MOD_91 WHERE entry = @RAID_DIFF_10N + 69168;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 2445193  / @HP_MOD_91 WHERE entry = @RAID_DIFF_10H + 69168;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 7, Health_Mod = 4890384  / @HP_MOD_91 WHERE entry = @RAID_DIFF_25N + 69168;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 8, Health_Mod = 6112980  / @HP_MOD_91 WHERE entry = @RAID_DIFF_25H + 69168;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 7, Health_Mod = 4401346  / @HP_MOD_91 WHERE entry = @RAID_DIFF_25R + 69168;

-- Amani'shi Protector
UPDATE creature_template SET LootId = 0, dmg_multiplier = 4, Health_Mod = 2500000  / @HP_MOD_91 WHERE entry = @RAID_DIFF_10N + 69169;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 3125000  / @HP_MOD_91 WHERE entry = @RAID_DIFF_10H + 69169;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 7, Health_Mod = 6250000  / @HP_MOD_91 WHERE entry = @RAID_DIFF_25N + 69169;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 8, Health_Mod = 7812500  / @HP_MOD_91 WHERE entry = @RAID_DIFF_25H + 69169;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 5625000  / @HP_MOD_91 WHERE entry = @RAID_DIFF_25R + 69169;

-- Sul'lithuz Stonegazer
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 1304000  / @HP_MOD_91 WHERE entry = @RAID_DIFF_10N + 69172;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 1630000  / @HP_MOD_91 WHERE entry = @RAID_DIFF_10H + 69172;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 8, Health_Mod = 3260000  / @HP_MOD_91 WHERE entry = @RAID_DIFF_25N + 69172;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 9, Health_Mod = 4075000  / @HP_MOD_91 WHERE entry = @RAID_DIFF_25H + 69172;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 7, Health_Mod = 2934000  / @HP_MOD_91 WHERE entry = @RAID_DIFF_25R + 69172;

-- Farraki Skirmisher
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 1630000  / @HP_MOD_91 WHERE entry = @RAID_DIFF_10N + 69173;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 2037500  / @HP_MOD_91 WHERE entry = @RAID_DIFF_10H + 69173;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 8, Health_Mod = 4075000  / @HP_MOD_91 WHERE entry = @RAID_DIFF_25N + 69173;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 9, Health_Mod = 5093750  / @HP_MOD_91 WHERE entry = @RAID_DIFF_25H + 69173;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 7, Health_Mod = 3667500  / @HP_MOD_91 WHERE entry = @RAID_DIFF_25R + 69173;

-- Farraki Wastewalker
UPDATE creature_template SET LootId = 0, dmg_multiplier = 4, Health_Mod = 6745200  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 69175;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 8431500  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 69175;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 7, Health_Mod = 16863000 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 69175;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 8, Health_Mod = 21078750 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 69175;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 15176700 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 69175;

-- Amani'shi Beast Shaman
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 2529200  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 69176;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 3161500  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 69176;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 8, Health_Mod = 6323000  / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 69176;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 9, Health_Mod = 7903750  / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 69176;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 7, Health_Mod = 5690700  / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 69176;

-- Amani Warbear
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 2529200  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 69177;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 3161500  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 69177;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 8, Health_Mod = 6323000  / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 69177;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 9, Health_Mod = 7903750  / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 69177;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 7, Health_Mod = 5690700  / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 69177;

-- Drakkari Frozen Warlord
UPDATE creature_template SET LootId = 0, dmg_multiplier = 4, Health_Mod = 8431600  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 69178;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 10539500 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 69178;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 7, Health_Mod = 21079000 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 69178;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 8, Health_Mod = 26348750 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 69178;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 18971100 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 69178;

-- Risen Drakkari Warrior
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 652000   / @HP_MOD_91 WHERE entry = @RAID_DIFF_10N + 69184;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 815000   / @HP_MOD_91 WHERE entry = @RAID_DIFF_10H + 69184;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 8, Health_Mod = 1630000  / @HP_MOD_91 WHERE entry = @RAID_DIFF_25N + 69184;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 9, Health_Mod = 2037500  / @HP_MOD_91 WHERE entry = @RAID_DIFF_25H + 69184;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 7, Health_Mod = 1467000  / @HP_MOD_91 WHERE entry = @RAID_DIFF_25R + 69184;

-- Risen Drakkari Champion
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 5059104  / @HP_MOD_91 WHERE entry = @RAID_DIFF_10N + 69185;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 6323880  / @HP_MOD_91 WHERE entry = @RAID_DIFF_10H + 69185;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 8, Health_Mod = 12647760 / @HP_MOD_91 WHERE entry = @RAID_DIFF_25N + 69185;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 9, Health_Mod = 15809700 / @HP_MOD_91 WHERE entry = @RAID_DIFF_25H + 69185;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 7, Health_Mod = 11382984 / @HP_MOD_91 WHERE entry = @RAID_DIFF_25R + 69185;

-- Zandalari Dinomancer
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 5059104  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 69221;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 6323880  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 69221;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 8, Health_Mod = 12647760 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 69221;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 9, Health_Mod = 15809700 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 69221;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 7, Health_Mod = 11382984 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 69221;

-- Venomous Effusion
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 1550000  / @HP_MOD_93 WHERE entry = @RAID_DIFF_10N + 69314;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 1937500  / @HP_MOD_93 WHERE entry = @RAID_DIFF_10H + 69314;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 8, Health_Mod = 2842500  / @HP_MOD_93 WHERE entry = @RAID_DIFF_25N + 69314;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 9, Health_Mod = 3790000  / @HP_MOD_93 WHERE entry = @RAID_DIFF_25H + 69314;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 7, Health_Mod = 2558250  / @HP_MOD_93 WHERE entry = @RAID_DIFF_25R + 69314;

-- Soul-Fed Construct
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 6,  Health_Mod = 40472832  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 70308;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 7,  Health_Mod = 50591040  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 70308;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 9,  Health_Mod = 101182080 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 70308;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 10, Health_Mod = 126477600 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 70308;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 8,  Health_Mod = 91063872  / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 70308;

-- Tormented Spirit
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 5, Health_Mod = 24451920 / @HP_MOD_91 WHERE entry = @RAID_DIFF_10N + 70341;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 6, Health_Mod = 30564900 / @HP_MOD_91 WHERE entry = @RAID_DIFF_10H + 70341;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 8, Health_Mod = 61129800 / @HP_MOD_91 WHERE entry = @RAID_DIFF_25N + 70341;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 9, Health_Mod = 76412250 / @HP_MOD_91 WHERE entry = @RAID_DIFF_25H + 70341;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 7, Health_Mod = 55016820 / @HP_MOD_91 WHERE entry = @RAID_DIFF_25R + 70341;

-- Stormbringer Draz'kil
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 6,  Health_Mod = 35413728  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 70445;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 7,  Health_Mod = 44267160  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 70445;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 9,  Health_Mod = 88534320  / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 70445;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 10, Health_Mod = 110667900 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 70445;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 8,  Health_Mod = 79680888  / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 70445;

-- Ancient Python
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 4, Health_Mod = 2363646 / @HP_MOD_90 WHERE entry = @RAID_DIFF_10N + 70448;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 5, Health_Mod = 2954557 / @HP_MOD_90 WHERE entry = @RAID_DIFF_10H + 70448;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 7, Health_Mod = 5909115 / @HP_MOD_90 WHERE entry = @RAID_DIFF_25N + 70448;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 8, Health_Mod = 7386393 / @HP_MOD_90 WHERE entry = @RAID_DIFF_25H + 70448;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 6, Health_Mod = 5318203 / @HP_MOD_90 WHERE entry = @RAID_DIFF_25R + 70448;

-- War-God Jalak
UPDATE creature_template SET LootId = 0, dmg_multiplier = 13, Health_Mod = 20934576 / @HP_MOD_93 WHERE entry = @RAID_DIFF_10N + 69374;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 14, Health_Mod = 26168220 / @HP_MOD_93 WHERE entry = @RAID_DIFF_10H + 69374;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 15, Health_Mod = 52336440 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25N + 69374;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 16, Health_Mod = 65420550 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25H + 69374;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 12, Health_Mod = 47102796 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25R + 69374;

-- Horridon
UPDATE creature_template SET LootId = @RAID_DIFF_10N + 68476, dmg_multiplier = 13, Health_Mod = 392523296  / @HP_MOD_93 WHERE entry = @RAID_DIFF_10N + 68476;
UPDATE creature_template SET LootId = @RAID_DIFF_10H + 68476, dmg_multiplier = 14, Health_Mod = 654205504  / @HP_MOD_93 WHERE entry = @RAID_DIFF_10H + 68476;
UPDATE creature_template SET LootId = @RAID_DIFF_25N + 68476, dmg_multiplier = 15, Health_Mod = 1177569920 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25N + 68476;
UPDATE creature_template SET LootId = @RAID_DIFF_25H + 68476, dmg_multiplier = 16, Health_Mod = 1962616448 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25H + 68476;
UPDATE creature_template SET LootId = @RAID_DIFF_25R + 68476, dmg_multiplier = 12, Health_Mod = 1059812928 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25R + 68476;
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Trash Mobs SAI
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (70308,70341,70445,70448);
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` IN (69176,69215);
DELETE FROM `smart_scripts` WHERE `entryorguid`IN (69176,69215,70308,70341,70445,70448) AND `source_type`= 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(69176, 0, 0, 0, 9, 0, 100, 0, 7, 40, 2000, 3000, 11, 136480, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Amani'shi Beast Shaman - Cast Chain Lightning"),
(69176, 0, 1, 0, 0, 0, 100, 0, 8000, 9000, 22000, 25000, 11, 136512, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, "Amani'shi Beast Shaman - Cast Hex of Confusion"),
(69176, 0, 2, 0, 0, 0, 100, 0, 15000, 17000, 30000, 35000, 11, 136487, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Amani'shi Beast Shaman - Cast Lightning Nova Totem"),
(69215, 0, 0, 0, 10, 0, 100, 0, 1, 50, 1000, 1000, 38, 0, 0, 0, 0, 0, 0, 21, 50, 0, 0, 0, 0, 0, 0, "Lightning Nova Totem - Cast enter in combat on spawn"),
(69215, 0, 1, 2, 0, 0, 100, 1, 1, 1, 0, 0, 11, 136489, 0, 0, 0, 0, 0, 21, 50, 0, 0, 0, 0, 0, 0, "Lightning Nova Totem - Cast Lightning Nova"),
(69215, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 67, 1, 25000, 25000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Lightning Nova Totem - Force Despawn after the cast"),
(69215, 0, 3, 0, 59, 0, 100, 1, 1, 0, 0, 0, 41, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lightning Nova Totem - Force Despawn after the cast"),
(70308, 0, 0, 0, 0, 0, 100, 0, 10000, 12000, 15000, 25000, 11, 139895, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Soul-Fed Construct - Cast Spiritfire Beam"),
(70308, 0, 1, 0, 0, 0, 100, 0, 8000, 15000, 15000, 18000, 11, 33661, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Soul-Fed Construct - Cast Crush Armor"),
(70341, 0, 0, 0, 0, 0, 100, 0, 3000, 10000, 7000, 10000, 11, 139550, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Tormented Spirit - Cast Torment"),
(70445, 0, 0, 0, 1, 0, 100, 1, 1, 1, 0, 0, 11, 139871, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Stormbringer Draz'kil - Cast Stormbringing OOC"),
(70445, 0, 1, 0, 0, 0, 100, 0, 7000, 8000, 8000, 9000, 11, 139900, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, "Stormbringer Draz'kil - Cast Stormcloud on random players"),
(70445, 0, 2, 0, 0, 0, 100, 0, 4000, 5000, 10000, 15000, 11, 139903, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Stormbringer Draz'kil - Cast Chain Lightning"),
(70448, 0, 0, 0, 1, 0, 100, 0, 1, 1, 1, 1, 11, 139885, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Ancient Python - Cast Stealth OOC"),
(70448, 0, 1, 0, 0, 0, 100, 0, 3000, 13000, 6000, 20000, 11, 139888, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Ancient Python - Cast Ancient Venom");
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Auras
DELETE FROM creature_template_aura WHERE entry = 54020 AND aura = 137126;
INSERT INTO creature_template_aura VALUES
(54020, 137126);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Equip
REPLACE INTO `creature_equip_template` (`entry`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES 
(69164, 95647, 0, 0),
(69167, 19874, 0, 0),
(69168, 33983, 0, 0),
(69169, 33984, 33789, 0),
(69173, 9511, 9511, 0),
(69175, 95686, 95686, 0),
(69176, 93667, 0, 0),
(69178, 95507, 0, 0),
(69184, 37721, 0, 0),
(69185, 49839, 0, 0),
(69221, 92358, 0, 0),
(69374, 94758, 0, 0),
(70246, 0, 93755, 0),
(70445, 95815, 0, 0);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Horridon Loots & Currency
DELETE FROM `creature_loot_template` WHERE entry IN(3168476, 3268476, 3368476, 3468476, 3568476);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
-- Horridon 10 NM
(3168476, 94752, 0, 1, 1, 1, 1),
(3168476, 94750, 0, 1, 1, 1, 1),
(3168476, 94753, 0, 1, 1, 1, 1),
(3168476, 94743, 0, 1, 1, 1, 1),
(3168476, 94756, 0, 1, 1, 1, 1),
(3168476, 94741, 0, 1, 1, 1, 1),
(3168476, 94975, 0, 1, 1, 1, 1),
(3168476, 94744, 0, 1, 1, 1, 1),
(3168476, 94751, 0, 1, 1, 1, 1),
(3168476, 94742, 0, 1, 1, 1, 1),
(3168476, 94747, 0, 1, 1, 1, 1),
(3168476, 94755, 0, 1, 2, 1, 1),
(3168476, 94740, 0, 1, 2, 1, 1),
(3168476, 94749, 0, 1, 2, 1, 1),
(3168476, 94526, 0, 1, 2, 1, 1),
(3168476, 94514, 0, 1, 2, 1, 1),
(3168476, 95514, 0, 1, 2, 1, 1),
(3168476, 94746, 0, 1, 2, 1, 1),
(3168476, 94754, 0, 1, 2, 1, 1),
(3168476, 94745, 0, 1, 2, 1, 1),
(3168476, 94748, 0, 1, 2, 1, 1),
(3168476, 94593, 1, 1, 3, 1, 1),
(3168476, 94594, 1, 1, 3, 1, 1),
(3168476, 87208, 1, 1, 3, 1, 1),
(3168476, 87209, 1, 1, 3, 1, 1),
(3168476, 93666, 1, 1, 4, 1, 1),
(3168476, 95498, 5, 1, 10, -95498, 1),

-- Horridon 10 HM
(3268476, 96404, 0, 1, 1, 1, 1),
(3268476, 96391, 0, 1, 1, 1, 1),
(3268476, 96401, 0, 1, 1, 1, 1),
(3268476, 96398, 0, 1, 1, 1, 1),
(3268476, 96385, 0, 1, 1, 1, 1),
(3268476, 96403, 0, 1, 1, 1, 1),
(3268476, 96390, 0, 1, 1, 1, 1),
(3268476, 96402, 0, 1, 1, 1, 1),
(3268476, 96397, 0, 1, 2, 1, 1),
(3268476, 96389, 0, 1, 2, 1, 1),
(3268476, 96399, 0, 1, 2, 1, 1),
(3268476, 96400, 0, 1, 2, 1, 1),
(3268476, 96406, 0, 1, 2, 1, 1),
(3268476, 96392, 0, 1, 2, 1, 1),
(3268476, 96405, 0, 1, 2, 1, 1),
(3268476, 96393, 0, 1, 3, 1, 1),
(3268476, 96407, 0, 1, 3, 1, 1),
(3268476, 96396, 0, 1, 3, 1, 1),
(3268476, 96408, 0, 1, 3, 1, 1),
(3268476, 96394, 0, 1, 3, 1, 1),
(3268476, 96395, 0, 1, 3, 1, 1),
(3268476, 94593, 1, 1, 4, 1, 1),
(3268476, 94594, 1, 1, 4, 1, 1),
(3268476, 87208, 1, 1, 4, 1, 1),
(3268476, 87209, 1, 1, 4, 1, 1),
(3268476, 93666, 1, 1, 5, 1, 1),
(3268476, 96621, 5, 1, 10, -96621, 1),

-- Horridon 25 NM
(3368476, 94752, 0, 1, 1, 1, 1),
(3368476, 94750, 0, 1, 1, 1, 1),
(3368476, 94753, 0, 1, 1, 1, 1),
(3368476, 94743, 0, 1, 1, 1, 1),
(3368476, 94756, 0, 1, 1, 1, 1),
(3368476, 94741, 0, 1, 2, 1, 1),
(3368476, 94975, 0, 1, 2, 1, 1),
(3368476, 94744, 0, 1, 2, 1, 1),
(3368476, 94751, 0, 1, 2, 1, 1),
(3368476, 94742, 0, 1, 2, 1, 1),
(3368476, 94747, 0, 1, 2, 1, 1),
(3368476, 94746, 0, 1, 3, 1, 1),
(3368476, 94754, 0, 1, 3, 1, 1),
(3368476, 94745, 0, 1, 3, 1, 1),
(3368476, 94748, 0, 1, 3, 1, 1),
(3368476, 94526, 0, 1, 4, 1, 1),
(3368476, 94514, 0, 1, 4, 1, 1),
(3368476, 95514, 0, 1, 4, 1, 1),
(3368476, 94755, 0, 1, 5, 1, 1),
(3368476, 94740, 0, 1, 5, 1, 1),
(3368476, 94749, 0, 1, 5, 1, 1),
(3368476, 94593, 1, 1, 6, 1, 1),
(3368476, 94594, 1, 1, 6, 1, 1),
(3368476, 87208, 1, 1, 6, 1, 1),
(3368476, 87209, 1, 1, 6, 1, 1),
(3368476, 93666, 1, 1, 7, 1, 1),
(3368476, 95498, 5, 1, 10, -95498, 1),

-- Horridon 25 HM
(3468476, 96399, 0, 1, 1, 1, 1),
(3468476, 96400, 0, 1, 1, 1, 1),
(3468476, 96406, 0, 1, 1, 1, 1),
(3468476, 96392, 0, 1, 1, 1, 1),
(3468476, 96405, 0, 1, 1, 1, 1),
(3468476, 96393, 0, 1, 2, 1, 1),
(3468476, 96407, 0, 1, 2, 1, 1),
(3468476, 96396, 0, 1, 2, 1, 1),
(3468476, 96408, 0, 1, 2, 1, 1),
(3468476, 96394, 0, 1, 2, 1, 1),
(3468476, 96395, 0, 1, 2, 1, 1),
(3468476, 96390, 0, 1, 3, 1, 1),
(3468476, 96402, 0, 1, 3, 1, 1),
(3468476, 96397, 0, 1, 3, 1, 1),
(3468476, 96389, 0, 1, 3, 1, 1),
(3468476, 96398, 0, 1, 4, 1, 1),
(3468476, 96385, 0, 1, 4, 1, 1),
(3468476, 96403, 0, 1, 4, 1, 1),
(3468476, 96404, 0, 1, 5, 1, 1),
(3468476, 96391, 0, 1, 5, 1, 1),
(3468476, 96401, 0, 1, 5, 1, 1),
(3468476, 94593, 1, 1, 6, 1, 1),
(3468476, 94594, 1, 1, 6, 1, 1),
(3468476, 87208, 1, 1, 6, 1, 1),
(3468476, 87209, 1, 1, 6, 1, 1),
(3468476, 93666, 1, 1, 7, 1, 1),
(3468476, 96621, 5, 1, 10, -96621, 1),

-- Horridon 25 LFR
(3568476, 95655, 0, 1, 1, 1, 1),
(3568476, 95656, 0, 1, 1, 1, 1),
(3568476, 95662, 0, 1, 1, 1, 1),
(3568476, 95648, 0, 1, 1, 1, 1),
(3568476, 95661, 0, 1, 1, 1, 1),
(3568476, 95649, 0, 1, 2, 1, 1),
(3568476, 95663, 0, 1, 2, 1, 1),
(3568476, 95652, 0, 1, 2, 1, 1),
(3568476, 95664, 0, 1, 2, 1, 1),
(3568476, 95650, 0, 1, 2, 1, 1),
(3568476, 95651, 0, 1, 2, 1, 1),
(3568476, 95645, 0, 1, 3, 1, 1),
(3568476, 95653, 0, 1, 3, 1, 1),
(3568476, 95658, 0, 1, 3, 1, 1),
(3568476, 95646, 0, 1, 3, 1, 1),
(3568476, 95659, 0, 1, 4, 1, 1),
(3568476, 95641, 0, 1, 4, 1, 1),
(3568476, 95654, 0, 1, 4, 1, 1),
(3568476, 95660, 0, 1, 5, 1, 1),
(3568476, 95647, 0, 1, 5, 1, 1),
(3568476, 95657, 0, 1, 5, 1, 1),
(3568476, 95877, 5, 1, 10, -95877, 1);

 DELETE FROM `creature_template_currency` WHERE `entry` IN(3168476,3268476,3368476,3468476);
 INSERT INTO `creature_template_currency` VALUES
(3168476, 396, 40),
(3268476, 396, 40),
(3368476, 396, 40),
(3468476, 396, 40);
/*----------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Spawns
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`npcflag2`,`unit_flags`,`unit_flags2`,`dynamicflags`,`isActive`,`protec_anti_doublet`) VALUES
(@CGUID+92, 70448, 1098, 0, 0, 248, 1, 0, 0, 5684.72, 6234.49, 117.991, 0.241424, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+93, 70448, 1098, 0, 0, 248, 1, 0, 0, 5689.06, 6227.88, 117.996, 0.888938, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+94, 70448, 1098, 0, 0, 248, 1, 0, 0, 5675.26, 6231.98, 117.991, 0.736093, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+95, 70448, 1098, 0, 0, 248, 1, 0, 0, 5674.91, 6238.48, 117.987, 6.2789, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+96, 70448, 1098, 0, 0, 248, 1, 0, 0, 5677.87, 6225.76, 117.998, 0.553147, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+97, 70448, 1098, 0, 0, 248, 1, 0, 0, 5668.63, 6283.99, 117.988, 4.43054, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+98, 70448, 1098, 0, 0, 248, 1, 0, 0, 5664.49, 6283.8, 117.991, 4.73351, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+99, 70448, 1098, 0, 0, 248, 1, 0, 0, 5675.04, 6289.6, 117.989, 4.21592, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+100, 70448, 1098, 0, 0, 248, 1, 0, 0, 5668.76, 6298.77, 117.995, 4.57136, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+101, 70448, 1098, 0, 0, 248, 1, 0, 0, 5672.47, 6299.35, 117.996, 4.45019, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+102, 70448, 1098, 0, 0, 248, 1, 0, 0, 5404.42, 6035.12, 118.062, 6.00756, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+103, 70448, 1098, 0, 0, 248, 1, 0, 0, 5450.11, 6031.95, 117.985, 3.8329, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+104, 70448, 1098, 0, 0, 248, 1, 0, 0, 5455, 6016.71, 117.98, 2.32112, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+105, 70448, 1098, 0, 0, 248, 1, 0, 0, 5469.07, 6017.77, 117.997, 2.64303, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+106, 70448, 1098, 0, 0, 248, 1, 0, 0, 5452.18, 6023.21, 117.977, 2.45537, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+107, 70448, 1098, 0, 0, 248, 1, 0, 0, 5464.58, 6023.73, 117.985, 2.75258, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+108, 70341, 1098, 0, 0, 248, 1, 0, 0, 5609.3, 6270.26, 110.873, 3.14095, 86400, 25, 0, 407532, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+109, 70341, 1098, 0, 0, 248, 1, 0, 0, 5609.68, 6256.56, 110.873, 3.12677, 86400, 25, 0, 407532, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+110, 70341, 1098, 0, 0, 248, 1, 0, 0, 5492.98, 6254.91, 112.052, 3.15447, 86400, 25, 0, 407532, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+111, 70341, 1098, 0, 0, 248, 1, 0, 0, 5506.65, 6270.73, 112.054, 3.1535, 86400, 25, 0, 407532, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+112, 70341, 1098, 0, 0, 248, 1, 0, 0, 5438.34, 6211.03, 112.049, 4.70172, 86400, 25, 0, 407532, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+113, 70341, 1098, 0, 0, 248, 1, 0, 0, 5423.62, 6198.15, 112.052, 4.71771, 86400, 25, 0, 407532, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+114, 70341, 1098, 0, 0, 248, 1, 0, 0, 5423.66, 6072.44, 112.056, 1.56091, 86400, 25, 0, 407532, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+115, 70341, 1098, 0, 0, 248, 1, 0, 0, 5439.58, 6072.41, 112.056, 1.55699, 86400, 25, 0, 407532, 0, 1, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+116, 70441, 1098, 0, 0, 248, 1, 0, 0, 5478.67, 6055.58, 85.8571, 2.75688, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+117, 70441, 1098, 0, 0, 248, 1, 0, 0, 5635.23, 6326.59, 85.883, 2.52624, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+118, 70441, 1098, 0, 0, 248, 1, 0, 0, 5604.48, 6321.95, 85.9547, 4.22331, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+119, 70441, 1098, 0, 0, 248, 1, 0, 0, 5644.38, 6235.42, 85.8583, 0.48365, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+120, 70441, 1098, 0, 0, 248, 1, 0, 0, 5614.52, 6215.82, 85.9481, 3.67264, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+121, 70441, 1098, 0, 0, 248, 1, 0, 0, 5594.75, 6212.97, 86.0418, 5.19807, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+122, 70441, 1098, 0, 0, 248, 1, 0, 0, 5588.33, 6334.56, 85.9843, 0.326071, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+123, 70441, 1098, 0, 0, 248, 1, 0, 0, 5567.31, 6298.98, 86.0272, 3.93508, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+124, 70441, 1098, 0, 0, 248, 1, 0, 0, 5549.42, 6238.75, 86.0938, 6.08496, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+125, 70441, 1098, 0, 0, 248, 1, 0, 0, 5542.32, 6206.97, 86.1275, 2.02617, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+126, 70441, 1098, 0, 0, 248, 1, 0, 0, 5537.19, 6339.14, 86.0001, 4.96901, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+127, 70441, 1098, 0, 0, 248, 1, 0, 0, 5512.01, 6215.39, 86.1995, 5.77462, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+128, 70441, 1098, 0, 0, 248, 1, 0, 0, 5508.19, 6225.3, 86.1402, 3.08307, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+129, 70441, 1098, 0, 0, 248, 1, 0, 0, 5486.92, 6306.69, 86.037, 6.17205, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+130, 70441, 1098, 0, 0, 248, 1, 0, 0, 5518.81, 6294.52, 86.0441, 3.80188, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+131, 70441, 1098, 0, 0, 248, 1, 0, 0, 5477.87, 6319.41, 86.0318, 4.39338, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+132, 70441, 1098, 0, 0, 248, 1, 0, 0, 5507.55, 6191.39, 86.2938, 5.12455, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+133, 70441, 1098, 0, 0, 248, 1, 0, 0, 5462.64, 6225.08, 86.2135, 5.12723, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+134, 70441, 1098, 0, 0, 248, 1, 0, 0, 5455.42, 6146.26, 86.2181, 5.945, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+135, 70441, 1098, 0, 0, 248, 1, 0, 0, 5477.23, 6121.48, 86.1348, 3.36737, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+136, 70441, 1098, 0, 0, 248, 1, 0, 0, 5471.92, 6167.69, 86.219, 5.48889, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+137, 70441, 1098, 0, 0, 248, 1, 0, 0, 5454.42, 6332.53, 86.0332, 2.25653, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+138, 70441, 1098, 0, 0, 248, 1, 0, 0, 5430.5, 6325.06, 86.039, 4.82098, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+139, 70441, 1098, 0, 0, 248, 1, 0, 0, 5476.98, 6086.94, 85.9786, 3.07306, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+140, 70441, 1098, 0, 0, 248, 1, 0, 0, 5403.58, 6209.89, 86.0999, 4.18501, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+141, 70441, 1098, 0, 0, 248, 1, 0, 0, 5483.87, 6069.14, 85.8605, 4.03088, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+142, 70441, 1098, 0, 0, 248, 1, 0, 0, 5404.77, 6278.24, 86.1055, 0.901, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+143, 70441, 1098, 0, 0, 248, 1, 0, 0, 5415.64, 6139.62, 86.0897, 2.77581, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+144, 70441, 1098, 0, 0, 248, 1, 0, 0, 5399.38, 6294.11, 86.1144, 2.80013, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+145, 70441, 1098, 0, 0, 248, 1, 0, 0, 5399.33, 6190.48, 86.0826, 6.09881, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+146, 70441, 1098, 0, 0, 248, 1, 0, 0, 5380.23, 6281.77, 86.0773, 0.218534, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+147, 70441, 1098, 0, 0, 248, 1, 0, 0, 5408.54, 6334.2, 86.0514, 1.32694, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+148, 70441, 1098, 0, 0, 248, 1, 0, 0, 5379.66, 6186.5, 86.0422, 4.15724, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+149, 70441, 1098, 0, 0, 248, 1, 0, 0, 5367.7, 6245.9, 86.0387, 5.77835, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+150, 70441, 1098, 0, 0, 248, 1, 0, 0, 5362.03, 6215.32, 86.0273, 1.15532, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+151, 70441, 1098, 0, 0, 248, 1, 0, 0, 5398.76, 6123.88, 86.0394, 4.01576, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+152, 70441, 1098, 0, 0, 248, 1, 0, 0, 5360.91, 6163.52, 86.0143, 0.804472, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+153, 70441, 1098, 0, 0, 248, 1, 0, 0, 5390.87, 6070.06, 85.8669, 1.14741, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+154, 70441, 1098, 0, 0, 248, 1, 0, 0, 5379.32, 6082.79, 85.9345, 5.39872, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+155, 70308, 1098, 0, 0, 248, 1, 0, 0, 5430.48, 6262.07, 117.91, 6.21155, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+156, 70445, 1098, 0, 0, 248, 1, 0, 0, 5431.68, 5984.71, 118.029, 1.57414, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+157, 68476, 1098, 0, 0, 248, 1, 0, 0, 5431.38, 5621.59, 130.12, 1.58353, 604800, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+158, 69374, 1098, 0, 0, 248, 1, 0, 69374, 5432.82, 5671.34, 192.263, 1.60374, 604800, 0, 0, 536137, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+159, 69221, 1098, 0, 0, 248, 1, 0, 69221, 5447.01, 5667.53, 192.352, 1.64919, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+160, 69221, 1098, 0, 0, 248, 1, 0, 0, 5326.83, 5787.72, 169.237, 6.10253, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+161, 69221, 1098, 0, 0, 248, 1, 0, 0, 5536.14, 5721.06, 169.238, 2.95279, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+162, 69221, 1098, 0, 0, 248, 1, 0, 0, 5325.51, 5720.81, 169.237, 0.32948, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+163, 69221, 1098, 0, 0, 248, 1, 0, 0, 5418.5, 5666.55, 192.349, 1.45472, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+164, 69221, 1098, 0, 0, 248, 1, 0, 0, 5340.08, 5648.4, 190.758, 0.876165, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+165, 69221, 1098, 0, 0, 248, 1, 0, 0, 5521.05, 5649.76, 190.653, 2.39193, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+166, 69221, 1098, 0, 0, 248, 1, 0, 0, 5537.03, 5784.27, 169.238, 3.67703, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+167, 69221, 1098, 0, 0, 248, 1, 0, 0, 5534.14, 5838.86, 190.416, 3.98585, 86400, 0, 0, 2866826, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+168, 69221, 1098, 0, 0, 248, 1, 0, 0, 5377.32, 5884.95, 190.356, 5.23621, 86400, 0, 0, 2866826, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+169, 69167, 1098, 0, 0, 248, 1, 0, 69167, 5476.53, 5630.99, 183.657, 1.9283, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+170, 69167, 1098, 0, 0, 248, 1, 0, 0, 5547.27, 5730.71, 177.174, 2.93249, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+171, 69167, 1098, 0, 0, 248, 1, 0, 0, 5557.87, 5711.52, 185.965, 2.80968, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+172, 69167, 1098, 0, 0, 248, 1, 0, 0, 5534.98, 5683.5, 183.657, 2.53953, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+173, 69167, 1098, 0, 0, 248, 1, 0, 0, 5500.83, 5650.06, 182.449, 2.16197, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+174, 69167, 1098, 0, 0, 248, 1, 0, 0, 5502.72, 5660.06, 177.365, 2.22015, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+175, 69164, 1098, 0, 0, 248, 1, 0, 0, 5528.67, 5696.59, 170.645, 2.32984, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+176, 69164, 1098, 0, 0, 248, 1, 0, 0, 5485.29, 5659.4, 169.238, 2.39773, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+177, 69164, 1098, 0, 0, 248, 1, 0, 0, 5519.45, 5677.57, 177.174, 2.35716, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+178, 69173, 1098, 0, 0, 248, 1, 0, 69173, 5494.01, 5864.54, 182.619, 4.211, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+179, 69173, 1098, 0, 0, 248, 1, 0, 69173, 5467.78, 5866.9, 175.33, 4.41469, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+180, 69173, 1098, 0, 0, 248, 1, 0, 69173, 5565.99, 5783.72, 190.567, 3.36932, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+181, 69173, 1098, 0, 0, 248, 1, 0, 0, 5477.77, 5878.04, 185.366, 4.36746, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+182, 69173, 1098, 0, 0, 248, 1, 0, 0, 5456.27, 5874.6, 182.159, 4.52191, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+183, 69173, 1098, 0, 0, 248, 1, 0, 0, 5497.9, 5860.65, 182.619, 4.16907, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+184, 69173, 1098, 0, 0, 248, 1, 0, 0, 5503.86, 5842.62, 175.542, 4.04276, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+185, 69173, 1098, 0, 0, 248, 1, 0, 0, 5511.75, 5850.01, 185.48, 4.02993, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+186, 69173, 1098, 0, 0, 248, 1, 0, 0, 5516.96, 5845.16, 185.567, 3.97312, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+187, 69173, 1098, 0, 0, 248, 1, 0, 0, 5530.83, 5827.71, 182.619, 3.79368, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+188, 69173, 1098, 0, 0, 248, 1, 0, 0, 5552.83, 5808.35, 184.292, 3.57466, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+189, 69173, 1098, 0, 0, 248, 1, 0, 0, 5538.96, 5806.53, 175.294, 3.60968, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+190, 69173, 1098, 0, 0, 248, 1, 0, 0, 5533.25, 5817.55, 178.663, 3.71377, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+191, 69173, 1098, 0, 0, 248, 1, 0, 0, 5553.69, 5793.66, 182.454, 3.46711, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+192, 69175, 1098, 0, 0, 248, 1, 0, 0, 5486.25, 5848.07, 169.238, 3.88623, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+193, 69175, 1098, 0, 0, 248, 1, 0, 0, 5517.81, 5828.87, 175.592, 3.91906, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+194, 69175, 1098, 0, 0, 248, 1, 0, 0, 5530.56, 5805.38, 169.727, 3.97107, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+195, 69168, 1098, 0, 0, 248, 1, 0, 69168, 5340.4, 5822.71, 175.343, 5.55286, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+196, 69168, 1098, 0, 0, 248, 1, 0, 69168, 5368.42, 5866.23, 184.291, 5.1909, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+197, 69168, 1098, 0, 0, 248, 1, 0, 0, 5399.75, 5868.7, 176.997, 4.97512, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+198, 69168, 1098, 0, 0, 248, 1, 0, 0, 5307.27, 5799.17, 184.426, 5.83688, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+199, 69177, 1098, 0, 0, 248, 1, 0, 0, 5378.38, 5852.85, 170.322, 5.38272, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+200, 69177, 1098, 0, 0, 248, 1, 0, 0, 5341.32, 5834.04, 182.022, 5.45862, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+201, 69177, 1098, 0, 0, 248, 1, 0, 0, 5334.85, 5809.79, 170.437, 5.49383, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+202, 69169, 1098, 0, 0, 248, 1, 0, 69169, 5361.96, 5859.76, 185.329, 5.25234, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+203, 69169, 1098, 0, 0, 248, 1, 0, 0, 5355.71, 5844.13, 178.902, 5.35377, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+204, 69169, 1098, 0, 0, 248, 1, 0, 0, 5317.12, 5809.7, 181.983, 5.73693, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+205, 69184, 1098, 0, 0, 248, 1, 0, 69184, 5361.01, 5661.38, 175.928, 0.933913, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+206, 69184, 1098, 0, 0, 248, 1, 0, 0, 5319, 5776.17, 174.042, 6.07716, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+207, 69184, 1098, 0, 0, 248, 1, 0, 0, 5305.42, 5784.01, 187.001, 6.03866, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+208, 69184, 1098, 0, 0, 248, 1, 0, 0, 5307.92, 5729.69, 184.07, 0.191863, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+209, 69184, 1098, 0, 0, 248, 1, 0, 0, 5326.88, 5695.01, 177.083, 0.518202, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+210, 69184, 1098, 0, 0, 248, 1, 0, 0, 5328.94, 5680.24, 185.359, 0.630327, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+211, 69185, 1098, 0, 0, 248, 1, 0, 69185, 5305.97, 5714.2, 185.775, 0.307118, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+212, 69185, 1098, 0, 0, 248, 1, 0, 69185, 5349.96, 5659.78, 184.187, 0.869069, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+213, 69185, 1098, 0, 0, 248, 1, 0, 0, 5333.17, 5686.35, 178.697, 0.609445, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+214, 69185, 1098, 0, 0, 248, 1, 0, 0, 5364.33, 5647.59, 183.656, 1.02185, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+215, 69178, 1098, 0, 0, 248, 1, 0, 0, 5337.02, 5699.22, 169.237, 0.782265, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+216, 69178, 1098, 0, 0, 248, 1, 0, 0, 5345.19, 5676.25, 177.197, 0.900182, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+217, 69178, 1098, 0, 0, 248, 1, 0, 0, 5376.79, 5659.66, 169.237, 0.760764, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+218, 69176, 1098, 0, 0, 248, 1, 0, 69176, 5324.67, 5819.76, 183.477, 5.68392, 86400, 0, 0, 421592, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+219, 69176, 1098, 0, 0, 248, 1, 0, 69176, 5381.81, 5871.46, 179.917, 5.17106, 86400, 0, 0, 421592, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+220, 69176, 1098, 0, 0, 248, 1, 0, 69176, 5408.79, 5866.17, 173.993, 4.81291, 86400, 0, 0, 421592, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+221, 69176, 1098, 0, 0, 248, 1, 0, 69176, 5318.5, 5798.08, 174.958, 6.00986, 86400, 0, 0, 421592, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),

(@CGUID+222, 69172, 1098, 0, 0, 248, 1, 0, 0, 5568.93, 5801.68, 190.602, 3.38506, 86400, 0, 0, 1222596, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+223, 69172, 1098, 0, 0, 248, 1, 0, 0, 5514.82, 5858.67, 190.429, 4.00709, 86400, 0, 0, 1222596, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+224, 69172, 1098, 0, 0, 248, 1, 0, 0, 5462.34, 5885.87, 190.401, 4.70845, 86400, 0, 0, 1222596, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+225, 70246, 1098, 0, 0, 248, 1, 0, 70246, 5712.82, 6263.74, 128.01, 4.73874, 1, 0, @CGUID+225, 393941, 0, 2, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+226, 70246, 1098, 0, 0, 248, 1, 0, 70246, 5483.29, 6268.58, 120.01, 6.26399, 1, 0, @CGUID+226, 393941, 0, 2, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+227, 70246, 1098, 0, 0, 248, 1, 0, 70246, 5483.29, 6268.58, 120.01, 6.26399, 1, 0, @CGUID+227, 393941, 0, 2, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+228, 70246, 1098, 0, 0, 248, 1, 0, 70246, 5431.33, 6281.24, 123.61, 6.26399, 1, 0, @CGUID+228, 393941, 0, 2, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+229, 70246, 1098, 0, 0, 248, 1, 0, 70246, 5405.78, 6263.1,  123.61, 1.433,   1, 0, @CGUID+229, 393941, 0, 2, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+230, 70246, 1098, 0, 0, 248, 1, 0, 70246, 5426.55, 6061.12, 117.79, 2.63834, 1, 0, @CGUID+230, 393941, 0, 2, 0, 0, 0, 2048, 0, 0, NULL),

(@CGUID+233, 662202, 1098, 0, 0, 248, 1, 0, 0, 5432.21, 5866.93, 235.962, 4.64324, 86400, 0, 0, 84, 0, 0, 0, 0, 33554432, 0, 0, 1, NULL),
(@CGUID+234, 662206, 1098, 0, 0, 248, 1, 0, 0, 5428.32, 5742.16, 129.604, 1.56204, 86400, 0, 0, 436137, 0, 0, 0, 0, 33554432, 0, 0, 0, NULL),

(@CGUID+236, 54020, 1098, 0, 0, 248, 1, 0, 0, 5515.29, 5646.11, 130.089, 2.39389, 86400, 0, 0, 84, 0, 0, 0, 0, 33554432, 0, 0, 0, NULL),
(@CGUID+237, 54020, 1098, 0, 0, 248, 1, 0, 0, 5539.28, 5671.98, 130.085, 2.39389, 86400, 0, 0, 84, 0, 0, 0, 0, 33554432, 0, 0, 0, NULL),
(@CGUID+238, 54020, 1098, 0, 0, 248, 1, 0, 0, 5523.58, 5661.83, 147.39, 2.48814, 86400, 0, 0, 84, 0, 0, 0, 0, 33554432, 0, 0, 0, NULL),
(@CGUID+239, 54020, 1098, 0, 0, 248, 1, 0, 0, 5519.11, 5843.02, 130.098, 3.95449, 86400, 0, 0, 84, 0, 0, 0, 0, 33554432, 0, 0, 0, NULL),
(@CGUID+240, 54020, 1098, 0, 0, 248, 1, 0, 0, 5534.8, 5830.16, 130.078, 3.95449, 86400, 0, 0, 84, 0, 0, 0, 0, 33554432, 0, 0, 0, NULL),
(@CGUID+241, 54020, 1098, 0, 0, 248, 1, 0, 0, 5505.88, 5861.82, 130.064, 3.95449, 86400, 0, 0, 84, 0, 0, 0, 0, 33554432, 0, 0, 0, NULL),
(@CGUID+242, 54020, 1098, 0, 0, 248, 1, 0, 0, 5523.31, 5845.21, 147.404, 3.95449, 86400, 0, 0, 84, 0, 0, 0, 0, 33554432, 0, 0, 0, NULL),
(@CGUID+243, 54020, 1098, 0, 0, 248, 1, 0, 0, 5345.6, 5841.22, 130.092, 5.5025, 86400, 0, 0, 84, 0, 0, 0, 0, 33554432, 0, 0, 0, NULL),
(@CGUID+244, 54020, 1098, 0, 0, 248, 1, 0, 0, 5347.09, 5857.35, 130.098, 5.68078, 86400, 0, 0, 84, 0, 0, 0, 0, 33554432, 0, 0, 0, NULL),
(@CGUID+245, 54020, 1098, 0, 0, 248, 1, 0, 0, 5324.4, 5835.66, 130.086, 5.18127, 86400, 0, 0, 84, 0, 0, 0, 0, 33554432, 0, 0, 0, NULL),
(@CGUID+246, 54020, 1098, 0, 0, 248, 1, 0, 0, 5337.1, 5847.97, 147.401, 5.29437, 86400, 0, 0, 84, 0, 0, 0, 0, 33554432, 0, 0, 0, NULL),
(@CGUID+247, 54020, 1098, 0, 0, 248, 1, 0, 0, 5343.59, 5664.15, 130.096, 0.606325, 86400, 0, 0, 84, 0, 0, 0, 0, 33554432, 0, 0, 0, NULL),
(@CGUID+248, 54020, 1098, 0, 0, 248, 1, 0, 0, 5323.5, 5670.26, 130.085, 0.757122, 86400, 0, 0, 84, 0, 0, 0, 0, 33554432, 0, 0, 0, NULL),
(@CGUID+249, 54020, 1098, 0, 0, 248, 1, 0, 0, 5349.94, 5642.27, 130.075, 0.757122, 86400, 0, 0, 84, 0, 0, 0, 0, 33554432, 0, 0, 0, NULL),
(@CGUID+250, 54020, 1098, 0, 0, 248, 1, 0, 0, 5334.87, 5656.93, 147.402, 0.757122, 86400, 0, 0, 84, 0, 0, 0, 0, 33554432, 0, 0, 0, NULL),
(@CGUID+251, 54020, 1098, 0, 0, 248, 1, 0, 0, 5520.22, 5665.73, 130.096, 2.33106, 86400, 0, 0, 84, 0, 0, 0, 0, 33554432, 0, 0, 0, NULL);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Aura, Path & Waypoints
DELETE FROM `creature_aura` WHERE `guid` IN (@CGUID+225,@CGUID+226,@CGUID+227,@CGUID+228,@CGUID+229,@CGUID+230);
INSERT INTO `creature_aura` VALUES
(@CGUID+225,139364),
(@CGUID+226,139364),
(@CGUID+227,139364),
(@CGUID+228,139364),
(@CGUID+229,139364),
(@CGUID+230,139364);

DELETE FROM `creature_path` WHERE `guid` IN (@CGUID+225,@CGUID+226,@CGUID+227,@CGUID+228,@CGUID+229,@CGUID+230);
INSERT INTO `creature_path` VALUES
(@CGUID+225,@CGUID+225),
(@CGUID+226,@CGUID+226),
(@CGUID+227,@CGUID+227),
(@CGUID+228,@CGUID+228),
(@CGUID+229,@CGUID+229),
(@CGUID+230,@CGUID+230);

DELETE FROM `waypoint_data` WHERE `id`IN (@CGUID+225,@CGUID+226,@CGUID+227,@CGUID+228,@CGUID+229,@CGUID+230);
INSERT INTO `waypoint_data` VALUES
(@CGUID+225,  1,  5712.82, 6263.74, 128.01, 0,0,0,0,100,0),
(@CGUID+225,  2,  5710.92, 6254.46, 128.01, 0,0,0,0,100,0),
(@CGUID+225,  3,  5705.85, 6246.20, 128.01, 0,0,0,0,100,0),
(@CGUID+225,  4,  5698.16, 6240.37, 128.01, 0,0,0,0,100,0),
(@CGUID+225,  5,  5688.06, 6238.43, 128.01, 0,0,0,0,100,0),
(@CGUID+225,  6,  5678.57, 6240.31, 128.01, 0,0,0,0,100,0),
(@CGUID+225,  7,  5670.94, 6245.49, 128.01, 0,0,0,0,100,0),
(@CGUID+225,  8,  5665.21, 6253.35, 128.01, 0,0,0,0,100,0),
(@CGUID+225,  9,  5663.45, 6263.16, 128.01, 0,0,0,0,100,0),
(@CGUID+225, 10,  5665.32, 6272.36, 128.01, 0,0,0,0,100,0),
(@CGUID+225, 11,  5669.91, 6280.21, 128.01, 0,0,0,0,100,0),
(@CGUID+225, 12,  5677.81, 6286.70, 128.01, 0,0,0,0,100,0),
(@CGUID+225, 13,  5687.25, 6288.59, 128.01, 0,0,0,0,100,0),
(@CGUID+225, 14,  5697.04, 6287.02, 128.01, 0,0,0,0,100,0),
(@CGUID+225, 15,  5705.74, 6281.65, 128.01, 0,0,0,0,100,0),
(@CGUID+225, 16,  5710.72, 6274.26, 128.01, 0,0,0,0,100,0),

(@CGUID+226,  1,  5483.29, 6268.58, 120.01, 0,0,0,0,100,0),
(@CGUID+226,  2,  5634.89, 6268.75, 120.01, 0,0,0,0,100,0),

(@CGUID+227,  1,  5498.06, 6258.07, 120.01, 0,0,0,0,100,0),
(@CGUID+227,  2,  5634.67, 6258.01, 120.01, 0,0,0,0,100,0),

(@CGUID+228,  1,  5431.33, 6281.24, 123.61, 0,0,0,0,100,0),
(@CGUID+228,  2,  5444.12, 6276.02, 123.61, 0,0,0,0,100,0),
(@CGUID+228,  3,  5449.73, 6263.38, 123.61, 0,0,0,0,100,0),
(@CGUID+228,  4,  5443.76, 6250.64, 123.61, 0,0,0,0,100,0),
(@CGUID+228,  5,  5431.52, 6245.38, 123.61, 0,0,0,0,100,0),
(@CGUID+228,  6,  5419.42, 6250.7,  123.61, 0,0,0,0,100,0),
(@CGUID+228,  7,  5413.72, 6263.36, 123.61, 0,0,0,0,100,0),
(@CGUID+228,  8,  5419.48, 6276.24, 123.61, 0,0,0,0,100,0),

(@CGUID+229,  1,  5405.78, 6263.1,  123.61, 0,0,0,0,100,0),
(@CGUID+229,  2,  5407.53, 6273.24, 123.61, 0,0,0,0,100,0),
(@CGUID+229,  3,  5413.7,  6281.91, 123.61, 0,0,0,0,100,0),
(@CGUID+229,  4,  5421.59, 6287.52, 123.61, 0,0,0,0,100,0),
(@CGUID+229,  5,  5432.38, 6289.52, 123.61, 0,0,0,0,100,0),
(@CGUID+229,  6,  5441.25, 6287.47, 123.61, 0,0,0,0,100,0),
(@CGUID+229,  7,  5449.78, 6282.71, 123.61, 0,0,0,0,100,0),
(@CGUID+229,  8,  5455.37, 6273.53, 123.61, 0,0,0,0,100,0),
(@CGUID+229,  9,  5457.35, 6263.59, 123.61, 0,0,0,0,100,0),
(@CGUID+229, 10,  5455.3,  6253.98, 123.61, 0,0,0,0,100,0),
(@CGUID+229, 11,  5449.12, 6245.61, 123.61, 0,0,0,0,100,0),
(@CGUID+229, 12,  5443.34, 6241.66, 123.61, 0,0,0,0,100,0),
(@CGUID+229, 13,  5434.4,  6239.26, 123.61, 0,0,0,0,100,0),
(@CGUID+229, 14,  5422.44, 6240.44, 123.61, 0,0,0,0,100,0),
(@CGUID+229, 15,  5415.18, 6244.78, 123.61, 0,0,0,0,100,0),
(@CGUID+229, 16,  5408.9,  6253.54, 123.61, 0,0,0,0,100,0),

(@CGUID+230,  1,  5426.55, 6061.12, 117.79, 0,0,0,0,100,0),
(@CGUID+230,  2,  5424.2,  6074.81, 117.79, 0,0,0,0,100,0),
(@CGUID+230,  3,  5418.94, 6087.06, 117.79, 0,0,0,0,100,0),
(@CGUID+230,  4,  5427.73, 6097.43, 117.79, 0,0,0,0,100,0),
(@CGUID+230,  5,  5439.41, 6105.21, 117.79, 0,0,0,0,100,0),
(@CGUID+230,  6,  5440.37, 6114.75, 117.79, 0,0,0,0,100,0),
(@CGUID+230,  7,  5436.75, 6125.44, 117.79, 0,0,0,0,100,0),
(@CGUID+230,  8,  5432.8,  6132.33, 117.79, 0,0,0,0,100,0),
(@CGUID+230,  9,  5425.01, 6145.61, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 10,  5421.45, 6160.31, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 11,  5420.15, 6170.3,  117.79, 0,0,0,0,100,0),
(@CGUID+230, 12,  5416.44, 6178.06, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 13,  5420.04, 6190.41, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 14,  5423.48, 6198.89, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 15,  5423.94, 6206.79, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 16,  5428.47, 6212.31, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 17,  5436.76, 6211.06, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 18,  5440.88, 6202.55, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 19,  5443.26, 6192.27, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 20,  5446.73, 6182.11, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 21,  5442.98, 6167.72, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 22,  5437.48, 6154.72, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 23,  5432.28, 6145.01, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 24,  5428.29, 6133.1,  117.79, 0,0,0,0,100,0),
(@CGUID+230, 25,  5425.12, 6125.4,  117.79, 0,0,0,0,100,0),
(@CGUID+230, 26,  5422.68, 6114.69, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 27,  5424.73, 6106.6,  117.79, 0,0,0,0,100,0),
(@CGUID+230, 28,  5432.61, 6098.73, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 29,  5438,    6092.98, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 30,  5444.21, 6080.96, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 31,  5441.16, 6071.47, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 32,  5436.52, 6060.34, 117.79, 0,0,0,0,100,0),
(@CGUID+230, 33,  5430.02, 6057.15, 117.79, 0,0,0,0,100,0);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Speeches
DELETE FROM `creature_text` WHERE `entry` IN (68476, 69221, 69374);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(68476, 0, 0, 'Farraki forces pour from the Farraki Tribal Door!', 41, 0, 100, 0, 0, 0, 'War-God Jalak'),
(68476, 1, 0, 'Gurubashi forces pour from the Gurubashi Tribal Door!', 41, 0, 100, 0, 0, 0, 'War-God Jalak'),
(68476, 2, 0, 'Drakkari forces pour from the Drakkari Tribal Door!', 41, 0, 100, 0, 0, 0, 'War-God Jalak'),
(68476, 3, 0, 'Amani forces pour from the Amani Tribal Door!', 41, 0, 100, 0, 0, 0, 'War-God Jalak'),
(68476, 4, 0, 'Horridon sets his eyes on $n and stamps his tail!', 41, 0, 100, 0, 0, 0, 'Horridon to Player'),
(68476, 5, 0, '$n forces Horridon to charge the Farakki door!', 41, 0, 100, 0, 0, 0, 'Horridon to Player'),
(68476, 6, 0, '$n forces Horridon to charge the Gurubashi door!', 41, 0, 100, 0, 0, 0, 'Horridon to Player'),
(68476, 7, 0, '$n forces Horridon to charge the Drakkari door!', 41, 0, 100, 0, 0, 0, 'Horridon to Player'),
(68476, 8, 0, '$n forces Horridon to charge the Amani door!', 41, 0, 100, 0, 0, 0, 'Horridon to Player'),

(69374, 0, 0, 'Welcome, weaklings, to the rebirth of the Zandalari Empire!', 14, 0, 100, 0, 0, 36009, 'War-God Jalak INTRO 1'),
(69374, 1, 0, 'The tribes have assembled - ye face not one force, but the combined might of all of the troll empire! The hand of Zul will span all the continents of Azeroth once again! An ye lesser races will know pain!', 14, 0, 100, 0, 0, 36010, 'War-God Jalak INTRO 2'),
(69374, 2, 0, 'Now, witness the true might of the Beast Ward. D\'akala di\'chuka HORRIDON! Kalimaste!', 14, 0, 100, 0, 0, 36011, 'War-God Jalak INTRO 3'),
(69374, 3, 0, 'Hahaha! Now it be my turn! Ye gonna see what it means to be a War-God!', 14, 0, 100, 0, 0, 36007, 'War-God Jalak aggro'),
(69374, 4, 0, 'Drakkari tribe, show them the might of the frozen North!', 14, 0, 100, 0, 0, 36013, 'War-God Jalak SPELL 1'),
(69374, 5, 0, 'Gurubashi tribe, send their bloated corpses down the river with ya jungle poisons.', 14, 0, 100, 0, 0, 36014, 'War-God Jalak SPELL 2'),
(69374, 6, 0, 'Farraki tribe, flay their flesh wit the fury of the sands!', 14, 0, 100, 0, 0, 36015, 'War-God Jalak SPELL 3'),
(69374, 7, 0, 'Amani tribe, avenge ye fallen warlords, in the name of Zul\'jin!', 14, 0, 100, 0, 0, 36016, 'War-God Jalak SPELL 4'),
(69374, 8, 0, 'Ya skull gonna make a fine ornament for my tusks.', 14, 0, 100, 0, 0, 36012, 'War-God Jalak Kill'),
(69374, 9, 0, 'Da\'kala koraste...Horridon...destroy them...', 14, 0, 100, 0, 0, 36008, 'War-God Jalak Death'),

(69221, 0, 0, 'The Zandalari Dinomancer drops an Orb of Control!', 41, 0, 100, 0, 0, 0, 'Zandalari Dinomancer');
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Gameobjects templates
REPLACE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `AIName`, `ScriptName`, `WDBVerified`) VALUES 
(218193, 10, 7951, 'Orb of Control', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 86, 0, 0, 3000, 0, 1, 0, 0, 0, 0, 137433, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, '', 'gob_horridon_orb_of_control', 17658),
(218374, 10, 7951, 'Orb of Control', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 86, 0, 0, 3000, 0, 1, 0, 0, 0, 0, 137442, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, '', 'gob_horridon_orb_of_control', 17658),
(218375, 10, 7951, 'Orb of Control', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 86, 0, 0, 3000, 0, 1, 0, 0, 0, 0, 137443, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, '', 'gob_horridon_orb_of_control', 17658),
(218376, 10, 7951, 'Orb of Control', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 86, 0, 0, 3000, 0, 1, 0, 0, 0, 0, 137444, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, '', 'gob_horridon_orb_of_control', 17658),
(218663, 0, 13679, 'Wrought Iron Door', '', '', '', 1375, 32, 1.1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218666, 0, 13679, 'Wrought Iron Door', '', '', '', 1375, 32, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218669, 0, 13679, 'Wrought Iron Door', '', '', '', 1375, 48, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218670, 0, 13589, 'Gurubashi Tribal Door', '', '', '', 1375, 48, 1, 0, 0, 0, 0, 0, 0, 0, 77, 3000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218671, 0, 13590, 'Drakkari Tribal Door', '', '', '', 1375, 48, 1, 0, 0, 0, 0, 0, 0, 0, 77, 3000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218672, 0, 13591, 'Farraki Tribal Door', '', '', '', 1375, 48, 1, 0, 0, 0, 0, 0, 0, 0, 77, 3000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218673, 0, 13592, 'Amani Tribal Door', '', '', '', 1375, 48, 1, 0, 0, 0, 0, 0, 0, 0, 77, 3000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218674, 0, 13593, 'Big Gate', '', '', '', 114, 48, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Gameobjects spawns
INSERT INTO `gameobject` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
(@OGUID+15, 218663, 1098, 0, 0, 760, 1, 5745.38, 6263.5, 124.021, 3.12867, 0, 0, 0.999979, 0.00646237, 86400, 255, 0, 0, NULL),
(@OGUID+16, 218663, 1098, 0, 0, 760, 1, 5431.65, 5863.2, 129.996, 4.71239, 0, 0, 1, -4.37114E-8, 86400, 255, 1, 0, NULL),

(@OGUID+22, 218666, 1098, 0, 0, 760, 1, 5541.31, 5753.58, 129.996, 3.14159, 0, 0, 1, -4.37114E-8, 86400, 255, 1, 0, NULL),
(@OGUID+23, 218669, 1098, 0, 0, 760, 1, 5431.65, 5949.82, 124.068, 4.71239, 0, 0, 1, -4.37114E-8, 86400, 255, 0, 0, NULL),

(@OGUID+17, 218670, 1098, 0, 0, 760, 1, 5523.46, 5662.12, 127.363, 2.35619, 0, 0, 1, -4.37114E-8, 86400, 255, 1, 0, NULL),
(@OGUID+18, 218671, 1098, 0, 0, 760, 1, 5340.14, 5661.82, 127.362, 0.785397, 0, 0, 1, -4.37114E-8, 86400, 255, 1, 0, NULL),
(@OGUID+19, 218672, 1098, 0, 0, 760, 1, 5523.25, 5845.22, 127.363, 3.92699, 0, 0, 1, -4.37114E-8, 86400, 255, 1, 0, NULL),
(@OGUID+20, 218673, 1098, 0, 0, 760, 1, 5339.94, 5845.09, 127.362, 5.49779, 0, 0, 1, -4.37114E-8, 86400, 255, 1, 0, NULL),
(@OGUID+21, 218674, 1098, 0, 0, 760, 1, 5431.71, 5652.19, 126.755, 1.57079, 0, 0, 1, -4.37114E-8, 86400, 255, 1, 0, NULL);
/*---------------------------------------------------------------------------------------------------------------*/
/* END Horridon */





/* BEGIN Council of Elders */
/*---------------------------------------------------------------------------------------------------------------*/
-- Spell Scripts
REPLACE INTO spell_script_names VALUES
  (136442, "spell_garajal_possessed"),
  (136507, "spell_dark_power"),
  (136857, "spell_quicksand_entrapped"),
  (136860, "spell_quicksand_periodic"),
  (136894, "spell_sul_sandstorm"),
  (136903, "spell_malakk_frigid_assault"),
  (136917, "spell_malakk_biting_cold"),
  (136922, "spell_malakk_frostbite_periodic"),
  (136937, "spell_malakk_frostbite_allies"),
  (136990, "spell_malakk_frostbite"),
  (137084, "spell_malakk_body_heat"),
  (137107, "spell_kazrajin_reckless_charge_targeting"),
  (137117, "spell_kazrajin_reckless_charge"),
  (137149, "spell_kazrajin_overload"),
  (137166, "spell_kazrajin_discharge"),
  (137203, "spell_marli_summon_blessed_loa_spirit"),
  (137350, "spell_marli_summon_shadowed_loa_spirit"),
  (137359, "spell_marked_soul"),
  (137390, "spell_shadowed_gift"),
  (137891, "spell_marli_twisted_fate_first"),
  (137962, "spell_marli_twisted_fate_second"),
  (137986, "spell_marli_twisted_fate_damages"),
  (137645, "spell_soul_fragment_target_selector"),
  (137643, "spell_soul_fragment_switcher"),
  (137359, "spell_marked_soul"),
  (138652, "spell_eruption");
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Templates
DELETE FROM creature_template_aura WHERE entry IN (69135,70029,70060,3170060,3270060,3370060,3470060,3570060);
INSERT INTO creature_template_aura VALUES
(69135, 57764), -- Gara'jal Hover
(69135, 91218), -- Gara'jal Ghost Visual
(70029, 138655), -- Eruption
(70060, 138719), -- Shadowed Voodoo Spirit
(3170060, 138719), -- Shadowed Voodoo Spirit 10N
(3270060, 138719), -- Shadowed Voodoo Spirit 10H
(3370060, 138719), -- Shadowed Voodoo Spirit 25N
(3470060, 138719), -- Shadowed Voodoo Spirit 25H
(3570060, 138719); -- Shadowed Voodoo Spirit 25LFR

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES 
(69740, 0, 0, 0, 0, 0, 23767, 0, 0, 0, 'Twisted Fate', '', '', 0, 92, 92, 4, 35, 35, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 5, 1, 1, 0, 0, 0, 0, 0, 0, 0, 68, 1, 0, 0, 'mob_twisted_fate', 17614);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`,`equipment_id`) VALUES

(69078, 0, 0, 0, 0, 0, 47505, 0, 0, 0, 'Sul the Sandcrawler', '', '', 0, 93, 93, 4, 14, 14, 0, 2, 1.714286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 4, 0, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2670, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 0, '', 17614,69078),
(69131, 0, 0, 0, 0, 0, 47506, 0, 0, 0, 'Frost King Malakk', '', '', 0, 93, 93, 4, 14, 14, 0, 2, 1.714286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 4, 0, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2665, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 0, '', 17614,69131),
(69132, 0, 0, 0, 0, 0, 47730, 0, 0, 0, 'High Priestess Mar\'li', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 3, 17839, 24339, 0, 51299, 1, 1800, 1800, 4, 0, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000000, 3100000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 0, '', 17614,69132),
(69134, 0, 0, 0, 0, 0, 47229, 0, 0, 0, 'Kazra\'jin', '', '', 0, 93, 93, 4, 14, 14, 0, 5.6, 2, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 4, 0, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2669, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 912, 1, 0, 0, '', 17614,0),
(69135, 0, 0, 0, 0, 0, 47230, 0, 0, 0, 'Gara\'jal the Spiritbinder', '', '', 0, 90, 90, 4, 16, 16, 0, 5.6, 2, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 33587456, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 912, 1, 0, 0, '', 17614,0),
(69944, 0, 0, 0, 0, 0, 47677, 0, 0, 0, 'Sand Elemental', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 165, 1, 0, 0, '', 17614,0),
(69899, 0, 0, 0, 0, 0, 47775, 47782, 47783, 47784, 'Farraki Sand Conjurer', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69899),
(69905, 0, 0, 0, 0, 0, 47766, 47767, 0, 0, 'Gurubashi Berserker', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69905),
(69906, 0, 0, 0, 0, 0, 47761, 47762, 0, 0, 'Zandalari High Priest', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69906),
(69909, 0, 0, 0, 0, 0, 22307, 22308, 47335, 47336, 'Amani\'shi Flame Chanter', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69909),
(69910, 0, 0, 0, 0, 0, 47769, 47780, 47781, 0, 'Drakkari Frost Warden', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69910),
(69911, 0, 0, 0, 0, 0, 47760, 0, 0, 0, 'Zandalari Warlord', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 42000, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 69911, 0, 0, 0, 0, 0, 0, 0, 0, 140414, 140422, 134856, 0, 0, 0, 0, 0, 0, 0, 11401, 11401, 'SmartAI', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 109125471, 0, '', 1,69911),
(69916, 0, 0, 0, 0, 0, 48181, 0, 0, 0, 'Gurubashi Berserker', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 42000, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 69916, 0, 0, 0, 0, 0, 0, 0, 0, 138540, 138427, 138693, 0, 0, 0, 0, 0, 0, 0, 11408, 11408, 'SmartAI', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 109133791, 0, '', 1,0),
(69927, 0, 0, 0, 0, 0, 47885, 0, 0, 0, 'Zandalari Prelate', '', '', 0, 92, 92, 4, 16, 16, 0, 2.8, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 1000, 2000, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 171, 1, 0, 0, '', 17614,69927),
(70029, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Eruption', '', '', 0, 93, 93, 4, 14, 14, 0, 0.01, 0.01, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 33554436, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 17826816, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, '', 17614,0),
(70047, 0, 0, 0, 0, 0, 31720, 0, 0, 0, 'Glacial Freeze Totem', '', '', 0, 92, 92, 4, 35, 35, 0, 0.01, 0.01, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,0),
(70056, 0, 0, 0, 0, 0, 47230, 0, 0, 0, 'Gara\'jal the Spiritbinder', '', '', 0, 93, 93, 4, 16, 16, 0, 5.6, 2, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 33587456, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 912, 1, 0, 0, '', 17614,0),
(70060, 0, 0, 0, 0, 0, 47853, 0, 0, 0, 'Shadowed Voodoo Spirit', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 2, 32768, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 8388624, 0, '', 17614,0),
(70137, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Gara\'jal\'s Trash Soul', '', '', 0, 90, 90, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 17826816, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2664, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614,0),
(70557, 0, 0, 0, 0, 0, 48121, 0, 0, 0, 'Zandalari Prophet', '', '', 0, 92, 92, 4, 16, 16, 0, 2.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 171, 1, 0, 0, '', 17614,70557),
(662204, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Twisted Fate Helper', '', '', 0, 1, 1, 4, 35, 35, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 33554438, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 17826816, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 130, 'mob_twisted_fate_helper', 17614,0),
(69480, 0, 0, 0, 0, 0, 47568, 25501, 11032, 45754, 'Blessed Loa Spirit', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 118, 1, 0, 0, 'mob_blessed_loa_spirit', 17614, 0),
(69153, 0, 0, 0, 0, 0, 47677, 0, 0, 0, 'Living Sand', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 912, 1, 0, 0, 'mob_living_sand', 17614, 0),

(@RAID_DIFF_10N + 69078, 0, 0, 0, 0, 0, 47505, 0, 0, 0, 'Sul the Sandcrawler', '', '', 0, 93, 93, 4, 16, 16, 0, 2, 1.714286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2670, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 0, '', 17614,69078),
(@RAID_DIFF_10N + 69131, 0, 0, 0, 0, 0, 47506, 0, 0, 0, 'Frost King Malakk', '', '', 0, 93, 93, 4, 16, 16, 0, 2, 1.714286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2665, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 0, '', 17614,69131),
(@RAID_DIFF_10N + 69132, 0, 0, 0, 0, 0, 47730, 0, 0, 0, 'High Priestess Mar\'li', '', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 17839, 24339, 0, 51299, 1, 1800, 1800, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000000, 3100000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 0, '', 17614,69132),
(@RAID_DIFF_10N + 69134, 0, 0, 0, 0, 0, 47229, 0, 0, 0, 'Kazra\'jin', '', '', 0, 93, 93, 4, 16, 16, 0, 5.6, 2, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2669, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 912, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_10N + 69899, 0, 0, 0, 0, 0, 47775, 47782, 47783, 47784, 'Farraki Sand Conjurer', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69899),
(@RAID_DIFF_10N + 69905, 0, 0, 0, 0, 0, 47766, 47767, 0, 0, 'Gurubashi Berserker', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69905),
(@RAID_DIFF_10N + 69906, 0, 0, 0, 0, 0, 47761, 47762, 0, 0, 'Zandalari High Priest', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69906),
(@RAID_DIFF_10N + 69909, 0, 0, 0, 0, 0, 22307, 22308, 47335, 47336, 'Amani\'shi Flame Chanter', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69909),
(@RAID_DIFF_10N + 69910, 0, 0, 0, 0, 0, 47769, 47780, 47781, 0, 'Drakkari Frost Warden', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69910),
(@RAID_DIFF_10N + 69911, 0, 0, 0, 0, 0, 47760, 0, 0, 0, 'Zandalari Warlord', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 42000, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 69911, 0, 0, 0, 0, 0, 0, 0, 0, 140414, 140422, 134856, 0, 0, 0, 0, 0, 0, 0, 11401, 11401, 'SmartAI', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 109125471, 0, '', 1,69911),
(@RAID_DIFF_10N + 69916, 0, 0, 0, 0, 0, 48181, 0, 0, 0, 'Gurubashi Berserker', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 42000, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 69916, 0, 0, 0, 0, 0, 0, 0, 0, 138540, 138427, 138693, 0, 0, 0, 0, 0, 0, 0, 11408, 11408, 'SmartAI', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 109133791, 0, '', 1,0),
(@RAID_DIFF_10N + 69927, 0, 0, 0, 0, 0, 47885, 0, 0, 0, 'Zandalari Prelate', '', '', 0, 92, 92, 4, 16, 16, 0, 2.8, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 1000, 2000, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 171, 1, 0, 0, '', 17614,69927),
(@RAID_DIFF_10N + 70060, 0, 0, 0, 0, 0, 47853, 0, 0, 0, 'Shadowed Voodoo Spirit', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 2, 32768, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 8388624, 0, '', 17614,0),
(@RAID_DIFF_10N + 70557, 0, 0, 0, 0, 0, 48121, 0, 0, 0, 'Zandalari Prophet', '', '', 0, 92, 92, 4, 16, 16, 0, 2.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 171, 1, 0, 0, '', 17614,70557),
(@RAID_DIFF_10N + 69480, 0, 0, 0, 0, 0, 47568, 25501, 11032, 45754, 'Blessed Loa Spirit', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 118, 1, 0, 0, 'mob_blessed_loa_spirit', 17614, 0),
(@RAID_DIFF_10N + 69153, 0, 0, 0, 0, 0, 47677, 0, 0, 0, 'Living Sand', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 912, 1, 0, 0, 'mob_living_sand', 17614, 0),

(@RAID_DIFF_25N + 69078, 0, 0, 0, 0, 0, 47505, 0, 0, 0, 'Sul the Sandcrawler', '', '', 0, 93, 93, 4, 16, 16, 0, 2, 1.714286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2670, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 0, '', 17614,69078),
(@RAID_DIFF_25N + 69131, 0, 0, 0, 0, 0, 47506, 0, 0, 0, 'Frost King Malakk', '', '', 0, 93, 93, 4, 16, 16, 0, 2, 1.714286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2665, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 0, '', 17614,69131),
(@RAID_DIFF_25N + 69132, 0, 0, 0, 0, 0, 47730, 0, 0, 0, 'High Priestess Mar\'li', '', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 17839, 24339, 0, 51299, 1, 1800, 1800, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000000, 3100000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 0, '', 17614,69132),
(@RAID_DIFF_25N + 69134, 0, 0, 0, 0, 0, 47229, 0, 0, 0, 'Kazra\'jin', '', '', 0, 93, 93, 4, 16, 16, 0, 5.6, 2, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2669, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 912, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_25N + 69899, 0, 0, 0, 0, 0, 47775, 47782, 47783, 47784, 'Farraki Sand Conjurer', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69899),
(@RAID_DIFF_25N + 69905, 0, 0, 0, 0, 0, 47766, 47767, 0, 0, 'Gurubashi Berserker', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69905),
(@RAID_DIFF_25N + 69906, 0, 0, 0, 0, 0, 47761, 47762, 0, 0, 'Zandalari High Priest', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69906),
(@RAID_DIFF_25N + 69909, 0, 0, 0, 0, 0, 22307, 22308, 47335, 47336, 'Amani\'shi Flame Chanter', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69909),
(@RAID_DIFF_25N + 69910, 0, 0, 0, 0, 0, 47769, 47780, 47781, 0, 'Drakkari Frost Warden', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69910),
(@RAID_DIFF_25N + 69911, 0, 0, 0, 0, 0, 47760, 0, 0, 0, 'Zandalari Warlord', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 42000, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 69911, 0, 0, 0, 0, 0, 0, 0, 0, 140414, 140422, 134856, 0, 0, 0, 0, 0, 0, 0, 11401, 11401, 'SmartAI', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 109125471, 0, '', 1,69911),
(@RAID_DIFF_25N + 69916, 0, 0, 0, 0, 0, 48181, 0, 0, 0, 'Gurubashi Berserker', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 42000, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 69916, 0, 0, 0, 0, 0, 0, 0, 0, 138540, 138427, 138693, 0, 0, 0, 0, 0, 0, 0, 11408, 11408, 'SmartAI', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 109133791, 0, '', 1,0),
(@RAID_DIFF_25N + 69927, 0, 0, 0, 0, 0, 47885, 0, 0, 0, 'Zandalari Prelate', '', '', 0, 92, 92, 4, 16, 16, 0, 2.8, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 1000, 2000, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 171, 1, 0, 0, '', 17614,69927),
(@RAID_DIFF_25N + 70060, 0, 0, 0, 0, 0, 47853, 0, 0, 0, 'Shadowed Voodoo Spirit', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 2, 32768, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 8388624, 0, '', 17614,0),
(@RAID_DIFF_25N + 70557, 0, 0, 0, 0, 0, 48121, 0, 0, 0, 'Zandalari Prophet', '', '', 0, 92, 92, 4, 16, 16, 0, 2.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 171, 1, 0, 0, '', 17614,70557),
(@RAID_DIFF_25N + 69480, 0, 0, 0, 0, 0, 47568, 25501, 11032, 45754, 'Blessed Loa Spirit', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 118, 1, 0, 0, 'mob_blessed_loa_spirit', 17614, 0),
(@RAID_DIFF_25N + 69153, 0, 0, 0, 0, 0, 47677, 0, 0, 0, 'Living Sand', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 912, 1, 0, 0, 'mob_living_sand', 17614, 0),

(@RAID_DIFF_10H + 69078, 0, 0, 0, 0, 0, 47505, 0, 0, 0, 'Sul the Sandcrawler', '', '', 0, 93, 93, 4, 16, 16, 0, 2, 1.714286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2670, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 0, '', 17614,69078),
(@RAID_DIFF_10H + 69131, 0, 0, 0, 0, 0, 47506, 0, 0, 0, 'Frost King Malakk', '', '', 0, 93, 93, 4, 16, 16, 0, 2, 1.714286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2665, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 0, '', 17614,69131),
(@RAID_DIFF_10H + 69132, 0, 0, 0, 0, 0, 47730, 0, 0, 0, 'High Priestess Mar\'li', '', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 17839, 24339, 0, 51299, 1, 1800, 1800, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000000, 3100000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 0, '', 17614,69132),
(@RAID_DIFF_10H + 69134, 0, 0, 0, 0, 0, 47229, 0, 0, 0, 'Kazra\'jin', '', '', 0, 93, 93, 4, 16, 16, 0, 5.6, 2, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2669, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 912, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_10H + 69899, 0, 0, 0, 0, 0, 47775, 47782, 47783, 47784, 'Farraki Sand Conjurer', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69899),
(@RAID_DIFF_10H + 69905, 0, 0, 0, 0, 0, 47766, 47767, 0, 0, 'Gurubashi Berserker', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69905),
(@RAID_DIFF_10H + 69906, 0, 0, 0, 0, 0, 47761, 47762, 0, 0, 'Zandalari High Priest', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69906),
(@RAID_DIFF_10H + 69909, 0, 0, 0, 0, 0, 22307, 22308, 47335, 47336, 'Amani\'shi Flame Chanter', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69909),
(@RAID_DIFF_10H + 69910, 0, 0, 0, 0, 0, 47769, 47780, 47781, 0, 'Drakkari Frost Warden', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69910),
(@RAID_DIFF_10H + 69911, 0, 0, 0, 0, 0, 47760, 0, 0, 0, 'Zandalari Warlord', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 42000, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 69911, 0, 0, 0, 0, 0, 0, 0, 0, 140414, 140422, 134856, 0, 0, 0, 0, 0, 0, 0, 11401, 11401, 'SmartAI', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 109125471, 0, '', 1,69911),
(@RAID_DIFF_10H + 69916, 0, 0, 0, 0, 0, 48181, 0, 0, 0, 'Gurubashi Berserker', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 42000, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 69916, 0, 0, 0, 0, 0, 0, 0, 0, 138540, 138427, 138693, 0, 0, 0, 0, 0, 0, 0, 11408, 11408, 'SmartAI', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 109133791, 0, '', 1,0),
(@RAID_DIFF_10H + 69927, 0, 0, 0, 0, 0, 47885, 0, 0, 0, 'Zandalari Prelate', '', '', 0, 92, 92, 4, 16, 16, 0, 2.8, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 1000, 2000, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 171, 1, 0, 0, '', 17614,69927),
(@RAID_DIFF_10H + 70060, 0, 0, 0, 0, 0, 47853, 0, 0, 0, 'Shadowed Voodoo Spirit', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 2, 32768, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 8388624, 0, '', 17614,0),
(@RAID_DIFF_10H + 70557, 0, 0, 0, 0, 0, 48121, 0, 0, 0, 'Zandalari Prophet', '', '', 0, 92, 92, 4, 16, 16, 0, 2.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 171, 1, 0, 0, '', 17614,70557),
(@RAID_DIFF_10H + 69480, 0, 0, 0, 0, 0, 47568, 25501, 11032, 45754, 'Blessed Loa Spirit', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 118, 1, 0, 0, 'mob_blessed_loa_spirit', 17614, 0),
(@RAID_DIFF_10H + 69153, 0, 0, 0, 0, 0, 47677, 0, 0, 0, 'Living Sand', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 912, 1, 0, 0, 'mob_living_sand', 17614, 0),

(@RAID_DIFF_25H + 69078, 0, 0, 0, 0, 0, 47505, 0, 0, 0, 'Sul the Sandcrawler', '', '', 0, 93, 93, 4, 16, 16, 0, 2, 1.714286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2670, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 0, '', 17614,69078),
(@RAID_DIFF_25H + 69131, 0, 0, 0, 0, 0, 47506, 0, 0, 0, 'Frost King Malakk', '', '', 0, 93, 93, 4, 16, 16, 0, 2, 1.714286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2665, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 0, '', 17614,69131),
(@RAID_DIFF_25H + 69132, 0, 0, 0, 0, 0, 47730, 0, 0, 0, 'High Priestess Mar\'li', '', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 17839, 24339, 0, 51299, 1, 1800, 1800, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000000, 3100000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 0, '', 17614,69132),
(@RAID_DIFF_25H + 69134, 0, 0, 0, 0, 0, 47229, 0, 0, 0, 'Kazra\'jin', '', '', 0, 93, 93, 4, 16, 16, 0, 5.6, 2, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2669, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 912, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_25H + 69899, 0, 0, 0, 0, 0, 47775, 47782, 47783, 47784, 'Farraki Sand Conjurer', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69899),
(@RAID_DIFF_25H + 69905, 0, 0, 0, 0, 0, 47766, 47767, 0, 0, 'Gurubashi Berserker', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69905),
(@RAID_DIFF_25H + 69906, 0, 0, 0, 0, 0, 47761, 47762, 0, 0, 'Zandalari High Priest', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69906),
(@RAID_DIFF_25H + 69909, 0, 0, 0, 0, 0, 22307, 22308, 47335, 47336, 'Amani\'shi Flame Chanter', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69909),
(@RAID_DIFF_25H + 69910, 0, 0, 0, 0, 0, 47769, 47780, 47781, 0, 'Drakkari Frost Warden', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69910),
(@RAID_DIFF_25H + 69911, 0, 0, 0, 0, 0, 47760, 0, 0, 0, 'Zandalari Warlord', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 42000, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 69911, 0, 0, 0, 0, 0, 0, 0, 0, 140414, 140422, 134856, 0, 0, 0, 0, 0, 0, 0, 11401, 11401, 'SmartAI', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 109125471, 0, '', 1,69911),
(@RAID_DIFF_25H + 69916, 0, 0, 0, 0, 0, 48181, 0, 0, 0, 'Gurubashi Berserker', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 42000, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 69916, 0, 0, 0, 0, 0, 0, 0, 0, 138540, 138427, 138693, 0, 0, 0, 0, 0, 0, 0, 11408, 11408, 'SmartAI', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 109133791, 0, '', 1,0),
(@RAID_DIFF_25H + 69927, 0, 0, 0, 0, 0, 47885, 0, 0, 0, 'Zandalari Prelate', '', '', 0, 92, 92, 4, 16, 16, 0, 2.8, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 1000, 2000, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 171, 1, 0, 0, '', 17614,69927),
(@RAID_DIFF_25H + 70060, 0, 0, 0, 0, 0, 47853, 0, 0, 0, 'Shadowed Voodoo Spirit', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 2, 32768, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 8388624, 0, '', 17614,0),
(@RAID_DIFF_25H + 70557, 0, 0, 0, 0, 0, 48121, 0, 0, 0, 'Zandalari Prophet', '', '', 0, 92, 92, 4, 16, 16, 0, 2.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 171, 1, 0, 0, '', 17614,70557),
(@RAID_DIFF_25H + 69480, 0, 0, 0, 0, 0, 47568, 25501, 11032, 45754, 'Blessed Loa Spirit', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 118, 1, 0, 0, 'mob_blessed_loa_spirit', 17614, 0),
(@RAID_DIFF_25N + 69153, 0, 0, 0, 0, 0, 47677, 0, 0, 0, 'Living Sand', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 912, 1, 0, 0, 'mob_living_sand', 17614, 0),

(@RAID_DIFF_25R + 69078, 0, 0, 0, 0, 0, 47505, 0, 0, 0, 'Sul the Sandcrawler', '', '', 0, 93, 93, 4, 16, 16, 0, 2, 1.714286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2670, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 0, '', 17614,69078),
(@RAID_DIFF_25R + 69131, 0, 0, 0, 0, 0, 47506, 0, 0, 0, 'Frost King Malakk', '', '', 0, 93, 93, 4, 16, 16, 0, 2, 1.714286, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2665, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 0, '', 17614,69131),
(@RAID_DIFF_25R + 69132, 0, 0, 0, 0, 0, 47730, 0, 0, 0, 'High Priestess Mar\'li', '', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 17839, 24339, 0, 51299, 1, 1800, 1800, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000000, 3100000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 0, '', 17614,69132),
(@RAID_DIFF_25R + 69134, 0, 0, 0, 0, 0, 47229, 0, 0, 0, 'Kazra\'jin', '', '', 0, 93, 93, 4, 16, 16, 0, 5.6, 2, 1, 3, 17839, 24339, 0, 51299, 1, 2000, 2000, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2669, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 912, 1, 0, 0, '', 17614,0),
(@RAID_DIFF_25R + 69899, 0, 0, 0, 0, 0, 47775, 47782, 47783, 47784, 'Farraki Sand Conjurer', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69899),
(@RAID_DIFF_25R + 69905, 0, 0, 0, 0, 0, 47766, 47767, 0, 0, 'Gurubashi Berserker', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69905),
(@RAID_DIFF_25R + 69906, 0, 0, 0, 0, 0, 47761, 47762, 0, 0, 'Zandalari High Priest', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69906),
(@RAID_DIFF_25R + 69909, 0, 0, 0, 0, 0, 22307, 22308, 47335, 47336, 'Amani\'shi Flame Chanter', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69909),
(@RAID_DIFF_25R + 69910, 0, 0, 0, 0, 0, 47769, 47780, 47781, 0, 'Drakkari Frost Warden', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614,69910),
(@RAID_DIFF_25R + 69911, 0, 0, 0, 0, 0, 47760, 0, 0, 0, 'Zandalari Warlord', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 42000, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 69911, 0, 0, 0, 0, 0, 0, 0, 0, 140414, 140422, 134856, 0, 0, 0, 0, 0, 0, 0, 11401, 11401, 'SmartAI', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 109125471, 0, '', 1,69911),
(@RAID_DIFF_25R + 69916, 0, 0, 0, 0, 0, 48181, 0, 0, 0, 'Gurubashi Berserker', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 42000, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 69916, 0, 0, 0, 0, 0, 0, 0, 0, 138540, 138427, 138693, 0, 0, 0, 0, 0, 0, 0, 11408, 11408, 'SmartAI', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 109133791, 0, '', 1,0),
(@RAID_DIFF_25R + 69927, 0, 0, 0, 0, 0, 47885, 0, 0, 0, 'Zandalari Prelate', '', '', 0, 92, 92, 4, 16, 16, 0, 2.8, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 1000, 2000, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 171, 1, 0, 0, '', 17614,69927),
(@RAID_DIFF_25R + 70060, 0, 0, 0, 0, 0, 47853, 0, 0, 0, 'Shadowed Voodoo Spirit', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 2, 32768, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 8388624, 0, '', 17614,0),
(@RAID_DIFF_25R + 70557, 0, 0, 0, 0, 0, 48121, 0, 0, 0, 'Zandalari Prophet', '', '', 0, 92, 92, 4, 16, 16, 0, 2.8, 1.71429, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 8, 32768, 2099200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11000, 12000, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 171, 1, 0, 0, '', 17614,70557),
(@RAID_DIFF_25R + 69480, 0, 0, 0, 0, 0, 47568, 25501, 11032, 45754, 'Blessed Loa Spirit', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 118, 1, 0, 0, 'mob_blessed_loa_spirit', 17614, 0),
(@RAID_DIFF_25R + 69153, 0, 0, 0, 0, 0, 47677, 0, 0, 0, 'Living Sand', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 912, 1, 0, 0, 'mob_living_sand', 17614, 0);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Updates
-- Creature Trash Mobs Damage, HP, LootId
-- Base HP Gamepedia Raid 25N
-- 25R -10%
-- 25H +25%
-- 10N (BASE HP: 25 = X : 10)
-- 10H=(10N+25%)

-- Script Names
UPDATE creature_template SET ScriptName = "boss_sul_the_sandcrawler", VehicleId = 2670, movementId = 180 WHERE entry = 69078;
UPDATE creature_template SET ScriptName = "boss_frost_king_malakk", VehicleId = 2665, movementId = 180 WHERE entry = 69131;
UPDATE creature_template SET ScriptName = "boss_high_priestess_marli", VehicleId = 2668, movementId = 180 WHERE entry = 69132;
UPDATE creature_template SET ScriptName = "boss_kazrajin", VehicleId = 2669, movementId = 180 WHERE entry = 69134;
UPDATE creature_template SET ScriptName = "mob_living_sand" WHERE entry = 69153;
UPDATE creature_template SET ScriptName = "mob_garajals_soul", VehicleId = 2664, modelid1 = 11686, modelid2 = 0, speed_walk = 5, speed_run = 5,
speed_fly = 5, difficulty_entry_3 = 0, difficulty_entry_4 = 0, difficulty_entry_5 = 0, difficulty_entry_6 = 0, difficulty_entry_7 = 0 WHERE entry = 69182;
UPDATE creature_template SET ScriptName = "mob_blessed_loa_spirit" WHERE entry = 69480;
UPDATE creature_template SET ScriptName = "mob_shadowed_loa_spirit" WHERE entry = 69548;
UPDATE creature_template SET ScriptName = "mob_twisted_fate" WHERE entry IN (69740, 69746);
UPDATE creature_template SET ScriptName = "npc_council_event_helper" WHERE entry = 662203;
UPDATE creature_template SET ScriptName = "mob_twisted_fate_helper" WHERE entry = 662204;
UPDATE creature_template SET ScriptName = "mob_quicksand_stalker" WHERE entry = 662205;
UPDATE creature_template SET ScriptName = 'mob_garajal', HoverHeight = 2 WHERE entry = 69135;

UPDATE creature_template SET movementId = 180, unit_flags2 = 2099200, faction_A = 14, faction_H = 14 WHERE entry IN (@RAID_DIFF_10N + 69078, @RAID_DIFF_25H + 69078, @RAID_DIFF_25N + 69078, @RAID_DIFF_10H + 69078,
@RAID_DIFF_10N + 69131, @RAID_DIFF_25H + 69131, @RAID_DIFF_25N + 69131, @RAID_DIFF_10H + 69131,
@RAID_DIFF_10N + 69132, @RAID_DIFF_25H + 69132, @RAID_DIFF_25N + 69132, @RAID_DIFF_10H + 69132,
@RAID_DIFF_10N + 69134, @RAID_DIFF_25H + 69134, @RAID_DIFF_25N + 69134, @RAID_DIFF_10H + 69134);

-- Difficulty entries bosses and Trash Mobs
UPDATE creature_template SET difficulty_entry_3 = @RAID_DIFF_10N + entry, difficulty_entry_4 = @RAID_DIFF_25N + entry, difficulty_entry_5 = @RAID_DIFF_10H + entry, difficulty_entry_6 = @RAID_DIFF_25H + entry, difficulty_entry_7 = @RAID_DIFF_25R + entry WHERE entry IN 
(69078,69131,69132,69134,69153,69480,69548,69899,69905,69906,69909,69910,69911,69916,69927,70060,70557);

-- Immunity and bind bosses
UPDATE creature_template SET flags_extra = flags_extra | 1, mechanic_immune_mask = 667893759 WHERE entry IN 
(69131,3169131,3269131,3369131,3469131,3569131,69134,3169134,3269134,3369134,3469134,3569134);

UPDATE creature_template SET flags_extra = flags_extra | 1, mechanic_immune_mask = 634339327 WHERE entry IN 
(69078,3169078,3269078,3369078,3469078,3569078,69132,3169132,3269132,3369132,3469132,3569132);

-- Living Sand
UPDATE creature_template SET LootId = 0, dmg_multiplier = 2, Health_Mod = 1290071 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 69153;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 3, Health_Mod = 1612589 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 69153;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 3225179 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 69153;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 4031474 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 69153;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 4, Health_Mod = 2902661 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 69153;

-- Blessed Loa Spirit
UPDATE creature_template SET LootId = 0, dmg_multiplier = 2, Health_Mod = 1335603 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 69480;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 3, Health_Mod = 1669504 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 69480;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 3339008 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 69480;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 4173760 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 69480;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 4, Health_Mod = 3005107 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 69480;

-- Shadowed Loa Spirit
UPDATE creature_template SET LootId = 0, dmg_multiplier = 2, Health_Mod = 2226005 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 69548;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 3, Health_Mod = 2782507 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 69548;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 5565014 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 69548;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 6956267 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 69548;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 4, Health_Mod = 5008512 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 69548;

-- Farraki Sand Conjurer
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 5, Health_Mod = 3237830  / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_10N + 69899;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 6, Health_Mod = 4047287  / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_10H + 69899;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 8, Health_Mod = 8094576  / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_25N + 69899;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 9, Health_Mod = 10118220 / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_25H + 69899;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 7, Health_Mod = 7285118  / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_25R + 69899;

-- Gurubashi Berserker
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 5, Health_Mod = 5059103  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 69905;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 6, Health_Mod = 6323879  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 69905;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 8, Health_Mod = 12647759 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 69905;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 9, Health_Mod = 15809698 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 69905;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 7, Health_Mod = 11382983 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 69905;

-- Zandalari High Priest
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 5, Health_Mod = 5059110  / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_10N + 69906;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 6, Health_Mod = 6323887  / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_10H + 69906;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 8, Health_Mod = 12647775 / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_25N + 69906;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 9, Health_Mod = 15809718 / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_25H + 69906;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 7, Health_Mod = 11382997 / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_25R + 69906;

-- Amani'shi Flame Chanter
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 3, Health_Mod = 4047288  / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_10N + 69909;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 4, Health_Mod = 5059110  / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_10H + 69909;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 6, Health_Mod = 10118220 / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_25N + 69909;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 7, Health_Mod = 12647775 / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_25H + 69909;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 5, Health_Mod = 9106398  / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_25R + 69909;

-- Drakkari Frost Warden
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 5, Health_Mod = 3237830  / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_10N + 69910;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 6, Health_Mod = 4047288  / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_10H + 69910;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 8, Health_Mod = 8094576  / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_25N + 69910;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 9, Health_Mod = 10118220 / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_25H + 69910;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 7, Health_Mod = 7285118  / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_25R + 69910;

-- Zandalari Warlord
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 5, Health_Mod = 11804400 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 69911;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 6, Health_Mod = 14755500 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 69911;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 8, Health_Mod = 29511000 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 69911;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 9, Health_Mod = 36888750 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 69911;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 7, Health_Mod = 26559900 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 69911;

-- Gurubashi Berserker
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 5, Health_Mod = 6268939  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 69916;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 6, Health_Mod = 7836174  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 69916;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 8, Health_Mod = 15672349 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 69916;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 9, Health_Mod = 19590436 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 69916;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 7, Health_Mod = 14105114 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 69916;

-- Zandalari Prelate
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 5, Health_Mod = 6745471  / @HP_MOD_92_M WHERE entry = @RAID_DIFF_10N + 69927;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 6, Health_Mod = 8431839  / @HP_MOD_92_M WHERE entry = @RAID_DIFF_10H + 69927;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 8, Health_Mod = 16863678 / @HP_MOD_92_M WHERE entry = @RAID_DIFF_25N + 69927;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 9, Health_Mod = 21079597 / @HP_MOD_92_M WHERE entry = @RAID_DIFF_25H + 69927;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 7, Health_Mod = 15177310 / @HP_MOD_92_M WHERE entry = @RAID_DIFF_25R + 69927;

-- Shadowed Voodoo Spirit
UPDATE creature_template SET LootId = 0, dmg_multiplier = 4, Health_Mod = 708000   / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_10N + 70060;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 885000   / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_10H + 70060;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 7, Health_Mod = 1770000  / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_25N + 70060;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 8, Health_Mod = 2212500  / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_25H + 70060;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 1593000  / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_25R + 70060;

-- Zandalari Prophet
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 5, Health_Mod = 4856745  / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_10N + 70557;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 6, Health_Mod = 6070931  / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_10H + 70557;
UPDATE creature_template SET LootId = 69388, dmg_multiplier = 8, Health_Mod = 12141863 / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_25N + 70557;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 9, Health_Mod = 15177328 / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_25H + 70557;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 7, Health_Mod = 10927676 / @HP_MOD_92_8 WHERE entry = @RAID_DIFF_25R + 70557;

-- Sul the Sandcrawler
UPDATE creature_template SET LootId = @RAID_DIFF_10N + 69078, dmg_multiplier = 12, Health_Mod = 89844224  / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_10N + 69078;
UPDATE creature_template SET LootId = @RAID_DIFF_10H + 69078, dmg_multiplier = 13, Health_Mod = 112305280 / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_10H + 69078;
UPDATE creature_template SET LootId = @RAID_DIFF_25N + 69078, dmg_multiplier = 14, Health_Mod = 224610560 / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_25N + 69078;
UPDATE creature_template SET LootId = @RAID_DIFF_25H + 69078, dmg_multiplier = 15, Health_Mod = 280763200 / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_25H + 69078;
UPDATE creature_template SET LootId = @RAID_DIFF_25R + 69078, dmg_multiplier = 11, Health_Mod = 202149504 / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_25R + 69078;

-- Frost King Malakk
UPDATE creature_template SET LootId = @RAID_DIFF_10N + 69131, dmg_multiplier = 12, Health_Mod = 89844224  / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_10N + 69131;
UPDATE creature_template SET LootId = @RAID_DIFF_10H + 69131, dmg_multiplier = 13, Health_Mod = 112305280 / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_10H + 69131;
UPDATE creature_template SET LootId = @RAID_DIFF_25N + 69131, dmg_multiplier = 14, Health_Mod = 224610560 / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_25N + 69131;
UPDATE creature_template SET LootId = @RAID_DIFF_25H + 69131, dmg_multiplier = 15, Health_Mod = 280763200 / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_25H + 69131;
UPDATE creature_template SET LootId = @RAID_DIFF_25R + 69131, dmg_multiplier = 11, Health_Mod = 202149504 / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_25R + 69131;

-- High Priestess Mar'li
UPDATE creature_template SET LootId = @RAID_DIFF_10N + 69132, dmg_multiplier = 12, Health_Mod = 89844224  / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_10N + 69132;
UPDATE creature_template SET LootId = @RAID_DIFF_10H + 69132, dmg_multiplier = 13, Health_Mod = 112305280 / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_10H + 69132;
UPDATE creature_template SET LootId = @RAID_DIFF_25N + 69132, dmg_multiplier = 14, Health_Mod = 224610560 / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_25N + 69132;
UPDATE creature_template SET LootId = @RAID_DIFF_25H + 69132, dmg_multiplier = 15, Health_Mod = 280763200 / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_25H + 69132;
UPDATE creature_template SET LootId = @RAID_DIFF_25R + 69132, dmg_multiplier = 11, Health_Mod = 202149504 / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_25R + 69132;

-- Kazra'jin
UPDATE creature_template SET LootId = @RAID_DIFF_10N + 69134, dmg_multiplier = 12, Health_Mod = 89844224  / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_10N + 69134;
UPDATE creature_template SET LootId = @RAID_DIFF_10H + 69134, dmg_multiplier = 13, Health_Mod = 112305280 / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_10H + 69134;
UPDATE creature_template SET LootId = @RAID_DIFF_25N + 69134, dmg_multiplier = 14, Health_Mod = 224610560 / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_25N + 69134;
UPDATE creature_template SET LootId = @RAID_DIFF_25H + 69134, dmg_multiplier = 15, Health_Mod = 280763200 / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_25H + 69134;
UPDATE creature_template SET LootId = @RAID_DIFF_25R + 69134, dmg_multiplier = 11, Health_Mod = 202149504 / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_25R + 69134;
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Trash Mobs SAI
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (69899,69905,69906,69909,69910,69911,69916,69927,69944,70047,70557);
DELETE FROM `smart_scripts` WHERE `entryorguid`IN (69899,69905,69906,69909,69910,69911,69916,69927,69944,70047,70557) AND `source_type`= 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(69899, 0, 0, 0, 0, 0, 100, 0, 3000, 4000, 6000, 10000, 11, 138739, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Farraki Sand Conjurer - Cast Sand Bolt"),
(69899, 0, 1, 3, 0, 0, 100, 0, 2000, 3000, 30000, 32000, 11, 140636, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Farraki Sand Conjurer - Cast Conjure Elementals"),
(69899, 0, 2, 0, 0, 0, 100, 0, 8000, 10000, 20000, 25000, 11, 138742, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Farraki Sand Conjurer - Cast Choking Sands"),
(69899, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 12, 69944, 1, 30000, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, "Farraki Sand Conjurer - Summon Event Sand Elemental1"),
(69899, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 12, 69944, 1, 30000, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, "Farraki Sand Conjurer - Summon Event Sand Elemental2"),

(69905, 0, 0, 0, 1, 0, 100, 1, 1, 1, 0, 0, 28, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Gurubashi Berserker - Remove all auras when OOC"),
(69905, 0, 1, 0, 0, 0, 100, 0, 10000, 12000, 15000, 20000, 11, 138693, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Gurubashi Berserker - Cast Bloodletting"),
(69905, 0, 2, 0, 2, 0, 100, 1, 0, 50, 0, 0, 11, 138427, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Gurubashi Berserker - Cast Berserker Frenzy"),

(69906, 0, 0, 0, 14, 0, 100, 0, 500000, 40, 12500, 18000, 11, 139228, 0, 0, 0, 0, 0, 11, 0, 40, 0, 0, 0, 0, 0, "Zandalari High Priest - Cast Light of the Loa"),

(69909, 0, 0, 0, 1, 0, 100, 1, 1, 1, 0, 0, 28, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Amani'shi Flame Chanter - Remove all aura OOC"),
(69909, 0, 1, 0, 1, 0, 100, 1, 2, 2, 0, 0, 11, 138432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Amani'shi Flame Chanter - Aura"),
(69909, 0, 2, 0, 0, 0, 100, 0, 4000, 8000, 13000, 24000, 11, 138607, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Amani'shi Flame Chanter - Cast Call Flames"),
(69909, 0, 3, 0, 0, 0, 100, 0, 6000, 10000, 10000, 20000, 11, 138652, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, "Amani'shi Flame Chanter - Cast Eruption"),
(69909, 0, 4, 0, 31, 0, 100, 0, 138607, 0, 1000, 1000, 11, 138610, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Amani'shi Flame Chanter - Cast Fiery Core after Call Flames"),
(69909, 0, 5, 0, 31, 0, 100, 0, 138652, 0, 1000, 1000, 11, 138610, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Amani'shi Flame Chanter - Cast Fiery Core after Eruption"),
(69909, 0, 6, 7, 23, 0, 100, 0, 138610, 5, 1000, 1000, 11, 138651, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Amani'shi Flame Chanter - Cast Molten Barrage on Fiery Core's stacks x5"),
(69909, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 28, 138610, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Amani'shi Flame Chanter - Remove Fiery Core's stacks after the Molten Barrage cast"),

(69910, 0, 0, 0, 0, 0, 100, 0, 5000, 8000, 13000, 15000, 11, 138668, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Drakkari Frost Warden - Cast Frost Bulwark"),
(69910, 0, 1, 0, 0, 0, 100, 0, 15000, 20000, 15000, 20000, 11, 138690, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, "Drakkari Frost Warden - Cast Glacial Freeze Totem"),

(69911, 0, 0, 0, 0, 0, 100, 0, 10000, 12000, 30000, 35000, 11, 140422, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Zandalari Warlord - Cast Strength of the Loa"),
(69911, 0, 1, 0, 0, 0, 100, 0, 3000, 4000, 8000, 12000, 11, 140414, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Zandalari Warlord - Cast Cleave"),
(69911, 0, 2, 0, 0, 0, 100, 0, 16000, 18000, 45000, 60000, 11, 134856, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Zandalari Warlord - Cast Zandalari Warcry"),

(69916, 0, 0, 0, 1, 0, 100, 1, 1, 1, 0, 0, 28, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Gurubashi Berserker - Remove all auras when OOC"),
(69916, 0, 1, 0, 1, 0, 100, 1, 2, 2, 0, 0, 11, 138427, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Gurubashi Berserker - Cast Berserker Frenzy"),
(69916, 0, 2, 0, 0, 0, 100, 0, 2000, 4000, 15000, 16000, 11, 138693, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Gurubashi Berserker - Cast Bloodletting"),

(69927, 0, 0, 0, 0, 0, 100, 0, 2500, 3000, 3000, 4000, 11, 139213, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Zandalari Prelate - Mark of the Loa"),
(69927, 0, 1, 0, 0, 0, 100, 0, 10000, 12000, 15000, 18000, 11, 139223, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Zandalari Prelate - Judgment of the Loa"),
(69927, 0, 2, 0, 14, 0, 100, 0, 500000, 40, 35000, 40000, 11, 139228, 0, 0, 0, 0, 0, 11, 0, 40, 0, 0, 0, 0, 0, "Zandalari Prelate - Light of the Loa"),

(69944, 0, 0, 0, 1, 0, 100, 0, 1, 1, 1000, 1000, 49, 0, 0, 0, 0, 0, 0, 21, 50, 0, 0, 0, 0, 0, 0, "Sand Elemental - Attack on spawn"),

(70047, 0, 0, 0, 1, 0, 100, 0, 1, 1, 1000, 1000, 49, 0, 0, 0, 0, 0, 0, 21, 50, 0, 0, 0, 0, 0, 0, "Glacial Freeze Totem - Cast Glacial Freeze"),
(70047, 0, 1, 0, 0, 0, 100, 1, 1, 1, 0, 0, 11, 138678, 0, 0, 0, 0, 0, 21, 50, 0, 0, 0, 0, 0, 0, "Glacial Freeze Totem - Cast Glacial Freeze"),
(70047, 0, 2, 0, 0, 0, 100, 1, 6000, 6000, 0, 0, 41, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Glacial Freeze Totem - Force Despawn after the cast"),

(70557, 0, 0, 0, 0, 0, 100, 0, 3000, 6000, 25000, 30000, 11, 140400, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Zandalari Prophet - Cast Mark of the Prophet"),
(70557, 0, 1, 0, 0, 0, 100, 0, 10000, 12000, 35000, 40000, 11, 140115, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Zandalari Prophet - Cast Visions of Demise"),
(70557, 0, 2, 0, 0, 0, 100, 0, 1000, 2000, 55000, 60000, 11, 139205, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Zandalari Prophet - Cast Visions of Grandeur");

/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Equip
REPLACE INTO `creature_equip_template` (`entry`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES 
(69078, 93238, 0, 0),
(69131, 93798, 93798, 0),
(69132, 94313, 0, 0),
(69899, 93238, 0, 0),
(69905, 90001, 90007, 0),
(69906, 94134, 0, 0),
(69909, 93241, 0, 0),
(69910, 94118, 94194, 0),
(69911, 94758, 0, 0),
(69927, 94122, 94193, 0),
(70557, 94248, 0, 0);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Council Loots
DELETE FROM `creature_loot_template` WHERE entry IN(3169078, 3269078, 3369078, 3469078, 3569078,3169131,3269131,3369131,3469131,3569131,3169132,3269132,3369132,3469132,3569132,3169134,3269134,3369134,3469134,3569134);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
-- Sul the Sandcrawler 10 NM
(3169078, 94761, 0, 1, 1, 1, 1),
(3169078, 94762, 0, 1, 1, 1, 1),
(3169078, 94763, 0, 1, 1, 1, 1),
(3169078, 94764, 0, 1, 1, 1, 1),
(3169078, 94765, 0, 1, 1, 1, 1),
(3169078, 94766, 0, 1, 1, 1, 1),
(3169078, 94767, 0, 1, 1, 1, 1),
(3169078, 94513, 0, 1, 2, 1, 1),
(3169078, 94516, 0, 1, 2, 1, 1),
(3169078, 94523, 0, 1, 2, 1, 1),
(3169078, 94758, 0, 1, 2, 1, 1),
(3169078, 94759, 0, 1, 2, 1, 1),
(3169078, 94760, 0, 1, 2, 1, 1),
(3169078, 95570, 0, 1, 3, 1, 1),
(3169078, 95575, 0, 1, 3, 1, 1),
(3169078, 95580, 0, 1, 3, 1, 1),
(3169078, 94593, 1, 1, 4, 1, 1),
(3169078, 94594, 1, 1, 4, 1, 1),
(3169078, 87208, 1, 1, 4, 1, 1),
(3169078, 87209, 1, 1, 4, 1, 1),
(3169078, 95498, 5, 1, 10, -95498, 1),

-- Sul the Sandcrawler 10 HM
(3269078, 96412, 0, 1, 1, 1, 1),
(3269078, 96411, 0, 1, 1, 1, 1),
(3269078, 96415, 0, 1, 1, 1, 1),
(3269078, 96416, 0, 1, 1, 1, 1),
(3269078, 96417, 0, 1, 1, 1, 1),
(3269078, 96418, 0, 1, 1, 1, 1),
(3269078, 96420, 0, 1, 1, 1, 1),
(3269078, 96409, 0, 1, 2, 1, 1),
(3269078, 96421, 0, 1, 2, 1, 1),
(3269078, 96413, 0, 1, 2, 1, 1),
(3269078, 96414, 0, 1, 2, 1, 1),
(3269078, 96410, 0, 1, 2, 1, 1),
(3269078, 96419, 0, 1, 2, 1, 1),
(3269078, 96599, 0, 1, 3, 1, 1),
(3269078, 96600, 0, 1, 3, 1, 1),
(3269078, 96601, 0, 1, 3, 1, 1),
(3269078, 94593, 1, 1, 4, 1, 1),
(3269078, 94594, 1, 1, 4, 1, 1),
(3269078, 87208, 1, 1, 4, 1, 1),
(3269078, 87209, 1, 1, 4, 1, 1),
(3269078, 96621, 5, 1, 10, -96621, 1),

-- Sul the Sandcrawler 25 NM
(3369078, 94761, 0, 1, 1, 1, 1),
(3369078, 94762, 0, 1, 1, 1, 1),
(3369078, 94763, 0, 1, 1, 1, 1),
(3369078, 94764, 0, 1, 2, 1, 1),
(3369078, 94765, 0, 1, 2, 1, 1),
(3369078, 94767, 0, 1, 2, 1, 1),
(3369078, 94766, 0, 1, 3, 1, 1),
(3369078, 94513, 0, 1, 3, 1, 1),
(3369078, 94516, 0, 1, 3, 1, 1),
(3369078, 94523, 0, 1, 3, 1, 1),
(3369078, 94758, 0, 1, 4, 1, 1),
(3369078, 94759, 0, 1, 4, 1, 1),
(3369078, 94760, 0, 1, 4, 1, 1),
(3369078, 95570, 0, 1, 5, 1, 1),
(3369078, 95575, 0, 1, 5, 1, 1),
(3369078, 95580, 0, 1, 5, 1, 1),
(3369078, 94593, 1, 1, 6, 1, 1),
(3369078, 94594, 1, 1, 6, 1, 1),
(3369078, 87208, 1, 1, 6, 1, 1),
(3369078, 87209, 1, 1, 6, 1, 1),
(3369078, 95498, 5, 1, 10, -95498, 1),

-- Sul the Sandcrawler 25 HM
(3469078, 96412, 0, 1, 1, 1, 1),
(3469078, 96411, 0, 1, 1, 1, 1),
(3469078, 96415, 0, 1, 1, 1, 1),
(3469078, 96416, 0, 1, 2, 1, 1),
(3469078, 96417, 0, 1, 2, 1, 1),
(3469078, 96418, 0, 1, 2, 1, 1),
(3469078, 96414, 0, 1, 3, 1, 1),
(3469078, 96410, 0, 1, 3, 1, 1),
(3469078, 96419, 0, 1, 3, 1, 1),
(3469078, 96409, 0, 1, 4, 1, 1),
(3469078, 96413, 0, 1, 4, 1, 1),
(3469078, 96420, 0, 1, 4, 1, 1),
(3469078, 96421, 0, 1, 4, 1, 1),
(3469078, 96599, 0, 1, 5, 1, 1),
(3469078, 96600, 0, 1, 5, 1, 1),
(3469078, 96601, 0, 1, 5, 1, 1),
(3469078, 94593, 1, 1, 6, 1, 1),
(3469078, 94594, 1, 1, 6, 1, 1),
(3469078, 87208, 1, 1, 6, 1, 1),
(3469078, 87209, 1, 1, 6, 1, 1),
(3469078, 96621, 5, 1, 10, -96621, 1),

-- Sul the Sandcrawler 25 LFR
(3569078, 95667, 0, 1, 1, 1, 1),
(3569078, 95668, 0, 1, 1, 1, 1),
(3569078, 95671, 0, 1, 1, 1, 1),
(3569078, 95672, 0, 1, 2, 1, 1),
(3569078, 95673, 0, 1, 2, 1, 1),
(3569078, 95674, 0, 1, 2, 1, 1),
(3569078, 95665, 0, 1, 3, 1, 1),
(3569078, 95669, 0, 1, 3, 1, 1),
(3569078, 95676, 0, 1, 3, 1, 1),
(3569078, 95677, 0, 1, 3, 1, 1),
(3569078, 95666, 0, 1, 4, 1, 1),
(3569078, 95670, 0, 1, 4, 1, 1),
(3569078, 95675, 0, 1, 4, 1, 1),
(3569078, 95855, 0, 1, 5, 1, 1),
(3569078, 95856, 0, 1, 5, 1, 1),
(3569078, 95857, 0, 1, 5, 1, 1),
(3569078, 95877, 5, 1, 10, -95877, 1),

-- Frost King Malakk 10 NM
(3169131, 94761, 0, 1, 1, 1, 1),
(3169131, 94762, 0, 1, 1, 1, 1),
(3169131, 94763, 0, 1, 1, 1, 1),
(3169131, 94764, 0, 1, 1, 1, 1),
(3169131, 94765, 0, 1, 1, 1, 1),
(3169131, 94766, 0, 1, 1, 1, 1),
(3169131, 94767, 0, 1, 1, 1, 1),
(3169131, 94513, 0, 1, 2, 1, 1),
(3169131, 94516, 0, 1, 2, 1, 1),
(3169131, 94523, 0, 1, 2, 1, 1),
(3169131, 94758, 0, 1, 2, 1, 1),
(3169131, 94759, 0, 1, 2, 1, 1),
(3169131, 94760, 0, 1, 2, 1, 1),
(3169131, 95570, 0, 1, 3, 1, 1),
(3169131, 95575, 0, 1, 3, 1, 1),
(3169131, 95580, 0, 1, 3, 1, 1),
(3169131, 94593, 1, 1, 4, 1, 1),
(3169131, 94594, 1, 1, 4, 1, 1),
(3169131, 87208, 1, 1, 4, 1, 1),
(3169131, 87209, 1, 1, 4, 1, 1),
(3169131, 95498, 5, 1, 10, -95498, 1),

-- Frost King Malakk 10 HM
(3269131, 96412, 0, 1, 1, 1, 1),
(3269131, 96411, 0, 1, 1, 1, 1),
(3269131, 96415, 0, 1, 1, 1, 1),
(3269131, 96416, 0, 1, 1, 1, 1),
(3269131, 96417, 0, 1, 1, 1, 1),
(3269131, 96418, 0, 1, 1, 1, 1),
(3269131, 96420, 0, 1, 1, 1, 1),
(3269131, 96409, 0, 1, 2, 1, 1),
(3269131, 96421, 0, 1, 2, 1, 1),
(3269131, 96413, 0, 1, 2, 1, 1),
(3269131, 96414, 0, 1, 2, 1, 1),
(3269131, 96410, 0, 1, 2, 1, 1),
(3269131, 96419, 0, 1, 2, 1, 1),
(3269131, 96599, 0, 1, 3, 1, 1),
(3269131, 96600, 0, 1, 3, 1, 1),
(3269131, 96601, 0, 1, 3, 1, 1),
(3269131, 94593, 1, 1, 4, 1, 1),
(3269131, 94594, 1, 1, 4, 1, 1),
(3269131, 87208, 1, 1, 4, 1, 1),
(3269131, 87209, 1, 1, 4, 1, 1),
(3269131, 96621, 5, 1, 10, -96621, 1),

-- Frost King Malakk 25 NM
(3369131, 94761, 0, 1, 1, 1, 1),
(3369131, 94762, 0, 1, 1, 1, 1),
(3369131, 94763, 0, 1, 1, 1, 1),
(3369131, 94764, 0, 1, 2, 1, 1),
(3369131, 94765, 0, 1, 2, 1, 1),
(3369131, 94767, 0, 1, 2, 1, 1),
(3369131, 94766, 0, 1, 3, 1, 1),
(3369131, 94513, 0, 1, 3, 1, 1),
(3369131, 94516, 0, 1, 3, 1, 1),
(3369131, 94523, 0, 1, 3, 1, 1),
(3369131, 94758, 0, 1, 4, 1, 1),
(3369131, 94759, 0, 1, 4, 1, 1),
(3369131, 94760, 0, 1, 4, 1, 1),
(3369131, 95570, 0, 1, 5, 1, 1),
(3369131, 95575, 0, 1, 5, 1, 1),
(3369131, 95580, 0, 1, 5, 1, 1),
(3369131, 94593, 1, 1, 6, 1, 1),
(3369131, 94594, 1, 1, 6, 1, 1),
(3369131, 87208, 1, 1, 6, 1, 1),
(3369131, 87209, 1, 1, 6, 1, 1),
(3369131, 95498, 5, 1, 10, -95498, 1),

-- Frost King Malakk 25 HM
(3469131, 96412, 0, 1, 1, 1, 1),
(3469131, 96411, 0, 1, 1, 1, 1),
(3469131, 96415, 0, 1, 1, 1, 1),
(3469131, 96416, 0, 1, 2, 1, 1),
(3469131, 96417, 0, 1, 2, 1, 1),
(3469131, 96418, 0, 1, 2, 1, 1),
(3469131, 96414, 0, 1, 3, 1, 1),
(3469131, 96410, 0, 1, 3, 1, 1),
(3469131, 96419, 0, 1, 3, 1, 1),
(3469131, 96409, 0, 1, 4, 1, 1),
(3469131, 96413, 0, 1, 4, 1, 1),
(3469131, 96420, 0, 1, 4, 1, 1),
(3469131, 96421, 0, 1, 4, 1, 1),
(3469131, 96599, 0, 1, 5, 1, 1),
(3469131, 96600, 0, 1, 5, 1, 1),
(3469131, 96601, 0, 1, 5, 1, 1),
(3469131, 94593, 1, 1, 6, 1, 1),
(3469131, 94594, 1, 1, 6, 1, 1),
(3469131, 87208, 1, 1, 6, 1, 1),
(3469131, 87209, 1, 1, 6, 1, 1),
(3469131, 96621, 5, 1, 10, -96621, 1),

-- Frost King Malakk 25 LFR
(3569131, 95667, 0, 1, 1, 1, 1),
(3569131, 95668, 0, 1, 1, 1, 1),
(3569131, 95671, 0, 1, 1, 1, 1),
(3569131, 95672, 0, 1, 2, 1, 1),
(3569131, 95673, 0, 1, 2, 1, 1),
(3569131, 95674, 0, 1, 2, 1, 1),
(3569131, 95665, 0, 1, 3, 1, 1),
(3569131, 95669, 0, 1, 3, 1, 1),
(3569131, 95676, 0, 1, 3, 1, 1),
(3569131, 95677, 0, 1, 3, 1, 1),
(3569131, 95666, 0, 1, 4, 1, 1),
(3569131, 95670, 0, 1, 4, 1, 1),
(3569131, 95675, 0, 1, 4, 1, 1),
(3569131, 95855, 0, 1, 5, 1, 1),
(3569131, 95856, 0, 1, 5, 1, 1),
(3569131, 95857, 0, 1, 5, 1, 1),
(3569131, 95877, 5, 1, 10, -95877, 1),

-- High Priestess Mar'li 10 NM
(3169132, 94761, 0, 1, 1, 1, 1),
(3169132, 94762, 0, 1, 1, 1, 1),
(3169132, 94763, 0, 1, 1, 1, 1),
(3169132, 94764, 0, 1, 1, 1, 1),
(3169132, 94765, 0, 1, 1, 1, 1),
(3169132, 94766, 0, 1, 1, 1, 1),
(3169132, 94767, 0, 1, 1, 1, 1),
(3169132, 94513, 0, 1, 2, 1, 1),
(3169132, 94516, 0, 1, 2, 1, 1),
(3169132, 94523, 0, 1, 2, 1, 1),
(3169132, 94758, 0, 1, 2, 1, 1),
(3169132, 94759, 0, 1, 2, 1, 1),
(3169132, 94760, 0, 1, 2, 1, 1),
(3169132, 95570, 0, 1, 3, 1, 1),
(3169132, 95575, 0, 1, 3, 1, 1),
(3169132, 95580, 0, 1, 3, 1, 1),
(3169132, 94593, 1, 1, 4, 1, 1),
(3169132, 94594, 1, 1, 4, 1, 1),
(3169132, 87208, 1, 1, 4, 1, 1),
(3169132, 87209, 1, 1, 4, 1, 1),
(3169132, 95498, 5, 1, 10, -95498, 1),

-- High Priestess Mar'li 10 HM
(3269132, 96412, 0, 1, 1, 1, 1),
(3269132, 96411, 0, 1, 1, 1, 1),
(3269132, 96415, 0, 1, 1, 1, 1),
(3269132, 96416, 0, 1, 1, 1, 1),
(3269132, 96417, 0, 1, 1, 1, 1),
(3269132, 96418, 0, 1, 1, 1, 1),
(3269132, 96420, 0, 1, 1, 1, 1),
(3269132, 96409, 0, 1, 2, 1, 1),
(3269132, 96421, 0, 1, 2, 1, 1),
(3269132, 96413, 0, 1, 2, 1, 1),
(3269132, 96414, 0, 1, 2, 1, 1),
(3269132, 96410, 0, 1, 2, 1, 1),
(3269132, 96419, 0, 1, 2, 1, 1),
(3269132, 96599, 0, 1, 3, 1, 1),
(3269132, 96600, 0, 1, 3, 1, 1),
(3269132, 96601, 0, 1, 3, 1, 1),
(3269132, 94593, 1, 1, 4, 1, 1),
(3269132, 94594, 1, 1, 4, 1, 1),
(3269132, 87208, 1, 1, 4, 1, 1),
(3269132, 87209, 1, 1, 4, 1, 1),
(3269132, 96621, 5, 1, 10, -96621, 1),

-- High Priestess Mar'li 25 NM
(3369132, 94761, 0, 1, 1, 1, 1),
(3369132, 94762, 0, 1, 1, 1, 1),
(3369132, 94763, 0, 1, 1, 1, 1),
(3369132, 94764, 0, 1, 2, 1, 1),
(3369132, 94765, 0, 1, 2, 1, 1),
(3369132, 94767, 0, 1, 2, 1, 1),
(3369132, 94766, 0, 1, 3, 1, 1),
(3369132, 94513, 0, 1, 3, 1, 1),
(3369132, 94516, 0, 1, 3, 1, 1),
(3369132, 94523, 0, 1, 3, 1, 1),
(3369132, 94758, 0, 1, 4, 1, 1),
(3369132, 94759, 0, 1, 4, 1, 1),
(3369132, 94760, 0, 1, 4, 1, 1),
(3369132, 95570, 0, 1, 5, 1, 1),
(3369132, 95575, 0, 1, 5, 1, 1),
(3369132, 95580, 0, 1, 5, 1, 1),
(3369132, 94593, 1, 1, 6, 1, 1),
(3369132, 94594, 1, 1, 6, 1, 1),
(3369132, 87208, 1, 1, 6, 1, 1),
(3369132, 87209, 1, 1, 6, 1, 1),
(3369132, 95498, 5, 1, 10, -95498, 1),

-- High Priestess Mar'li 25 HM
(3469132, 96412, 0, 1, 1, 1, 1),
(3469132, 96411, 0, 1, 1, 1, 1),
(3469132, 96415, 0, 1, 1, 1, 1),
(3469132, 96416, 0, 1, 2, 1, 1),
(3469132, 96417, 0, 1, 2, 1, 1),
(3469132, 96418, 0, 1, 2, 1, 1),
(3469132, 96414, 0, 1, 3, 1, 1),
(3469132, 96410, 0, 1, 3, 1, 1),
(3469132, 96419, 0, 1, 3, 1, 1),
(3469132, 96409, 0, 1, 4, 1, 1),
(3469132, 96413, 0, 1, 4, 1, 1),
(3469132, 96420, 0, 1, 4, 1, 1),
(3469132, 96421, 0, 1, 4, 1, 1),
(3469132, 96599, 0, 1, 5, 1, 1),
(3469132, 96600, 0, 1, 5, 1, 1),
(3469132, 96601, 0, 1, 5, 1, 1),
(3469132, 94593, 1, 1, 6, 1, 1),
(3469132, 94594, 1, 1, 6, 1, 1),
(3469132, 87208, 1, 1, 6, 1, 1),
(3469132, 87209, 1, 1, 6, 1, 1),
(3469132, 96621, 5, 1, 10, -96621, 1),

-- High Priestess Mar'li 25 LFR
(3569132, 95667, 0, 1, 1, 1, 1),
(3569132, 95668, 0, 1, 1, 1, 1),
(3569132, 95671, 0, 1, 1, 1, 1),
(3569132, 95672, 0, 1, 2, 1, 1),
(3569132, 95673, 0, 1, 2, 1, 1),
(3569132, 95674, 0, 1, 2, 1, 1),
(3569132, 95665, 0, 1, 3, 1, 1),
(3569132, 95669, 0, 1, 3, 1, 1),
(3569132, 95676, 0, 1, 3, 1, 1),
(3569132, 95677, 0, 1, 3, 1, 1),
(3569132, 95666, 0, 1, 4, 1, 1),
(3569132, 95670, 0, 1, 4, 1, 1),
(3569132, 95675, 0, 1, 4, 1, 1),
(3569132, 95855, 0, 1, 5, 1, 1),
(3569132, 95856, 0, 1, 5, 1, 1),
(3569132, 95857, 0, 1, 5, 1, 1),
(3569132, 95877, 5, 1, 10, -95877, 1),

-- Kazra'jin 10 NM
(3169134, 94761, 0, 1, 1, 1, 1),
(3169134, 94762, 0, 1, 1, 1, 1),
(3169134, 94763, 0, 1, 1, 1, 1),
(3169134, 94764, 0, 1, 1, 1, 1),
(3169134, 94765, 0, 1, 1, 1, 1),
(3169134, 94766, 0, 1, 1, 1, 1),
(3169134, 94767, 0, 1, 1, 1, 1),
(3169134, 94513, 0, 1, 2, 1, 1),
(3169134, 94516, 0, 1, 2, 1, 1),
(3169134, 94523, 0, 1, 2, 1, 1),
(3169134, 94758, 0, 1, 2, 1, 1),
(3169134, 94759, 0, 1, 2, 1, 1),
(3169134, 94760, 0, 1, 2, 1, 1),
(3169134, 95570, 0, 1, 3, 1, 1),
(3169134, 95575, 0, 1, 3, 1, 1),
(3169134, 95580, 0, 1, 3, 1, 1),
(3169134, 94593, 1, 1, 4, 1, 1),
(3169134, 94594, 1, 1, 4, 1, 1),
(3169134, 87208, 1, 1, 4, 1, 1),
(3169134, 87209, 1, 1, 4, 1, 1),
(3169134, 95498, 5, 1, 10, -95498, 1),

-- Kazra'jin 10 HM
(3269134, 96412, 0, 1, 1, 1, 1),
(3269134, 96411, 0, 1, 1, 1, 1),
(3269134, 96415, 0, 1, 1, 1, 1),
(3269134, 96416, 0, 1, 1, 1, 1),
(3269134, 96417, 0, 1, 1, 1, 1),
(3269134, 96418, 0, 1, 1, 1, 1),
(3269134, 96420, 0, 1, 1, 1, 1),
(3269134, 96409, 0, 1, 2, 1, 1),
(3269134, 96421, 0, 1, 2, 1, 1),
(3269134, 96413, 0, 1, 2, 1, 1),
(3269134, 96414, 0, 1, 2, 1, 1),
(3269134, 96410, 0, 1, 2, 1, 1),
(3269134, 96419, 0, 1, 2, 1, 1),
(3269134, 96599, 0, 1, 3, 1, 1),
(3269134, 96600, 0, 1, 3, 1, 1),
(3269134, 96601, 0, 1, 3, 1, 1),
(3269134, 94593, 1, 1, 4, 1, 1),
(3269134, 94594, 1, 1, 4, 1, 1),
(3269134, 87208, 1, 1, 4, 1, 1),
(3269134, 87209, 1, 1, 4, 1, 1),
(3269134, 96621, 5, 1, 10, -96621, 1),

-- Kazra'jin 25 NM
(3369134, 94761, 0, 1, 1, 1, 1),
(3369134, 94762, 0, 1, 1, 1, 1),
(3369134, 94763, 0, 1, 1, 1, 1),
(3369134, 94764, 0, 1, 2, 1, 1),
(3369134, 94765, 0, 1, 2, 1, 1),
(3369134, 94767, 0, 1, 2, 1, 1),
(3369134, 94766, 0, 1, 3, 1, 1),
(3369134, 94513, 0, 1, 3, 1, 1),
(3369134, 94516, 0, 1, 3, 1, 1),
(3369134, 94523, 0, 1, 3, 1, 1),
(3369134, 94758, 0, 1, 4, 1, 1),
(3369134, 94759, 0, 1, 4, 1, 1),
(3369134, 94760, 0, 1, 4, 1, 1),
(3369134, 95570, 0, 1, 5, 1, 1),
(3369134, 95575, 0, 1, 5, 1, 1),
(3369134, 95580, 0, 1, 5, 1, 1),
(3369134, 94593, 1, 1, 6, 1, 1),
(3369134, 94594, 1, 1, 6, 1, 1),
(3369134, 87208, 1, 1, 6, 1, 1),
(3369134, 87209, 1, 1, 6, 1, 1),
(3369134, 95498, 5, 1, 10, -95498, 1),

-- Kazra'jin 25 HM
(3469134, 96412, 0, 1, 1, 1, 1),
(3469134, 96411, 0, 1, 1, 1, 1),
(3469134, 96415, 0, 1, 1, 1, 1),
(3469134, 96416, 0, 1, 2, 1, 1),
(3469134, 96417, 0, 1, 2, 1, 1),
(3469134, 96418, 0, 1, 2, 1, 1),
(3469134, 96414, 0, 1, 3, 1, 1),
(3469134, 96410, 0, 1, 3, 1, 1),
(3469134, 96419, 0, 1, 3, 1, 1),
(3469134, 96409, 0, 1, 4, 1, 1),
(3469134, 96413, 0, 1, 4, 1, 1),
(3469134, 96420, 0, 1, 4, 1, 1),
(3469134, 96421, 0, 1, 4, 1, 1),
(3469134, 96599, 0, 1, 5, 1, 1),
(3469134, 96600, 0, 1, 5, 1, 1),
(3469134, 96601, 0, 1, 5, 1, 1),
(3469134, 94593, 1, 1, 6, 1, 1),
(3469134, 94594, 1, 1, 6, 1, 1),
(3469134, 87208, 1, 1, 6, 1, 1),
(3469134, 87209, 1, 1, 6, 1, 1),
(3469134, 96621, 5, 1, 10, -96621, 1),

-- Kazra'jin 25 LFR
(3569134, 95667, 0, 1, 1, 1, 1),
(3569134, 95668, 0, 1, 1, 1, 1),
(3569134, 95671, 0, 1, 1, 1, 1),
(3569134, 95672, 0, 1, 2, 1, 1),
(3569134, 95673, 0, 1, 2, 1, 1),
(3569134, 95674, 0, 1, 2, 1, 1),
(3569134, 95665, 0, 1, 3, 1, 1),
(3569134, 95669, 0, 1, 3, 1, 1),
(3569134, 95676, 0, 1, 3, 1, 1),
(3569134, 95677, 0, 1, 3, 1, 1),
(3569134, 95666, 0, 1, 4, 1, 1),
(3569134, 95670, 0, 1, 4, 1, 1),
(3569134, 95675, 0, 1, 4, 1, 1),
(3569134, 95855, 0, 1, 5, 1, 1),
(3569134, 95856, 0, 1, 5, 1, 1),
(3569134, 95857, 0, 1, 5, 1, 1),
(3569134, 95877, 5, 1, 10, -95877, 1);
/*----------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Spawn
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`npcflag2`,`unit_flags`,`unit_flags2`,`dynamicflags`,`isActive`,`protec_anti_doublet`) VALUES
(@CGUID+46, 69078, 1098, 0, 0, 248, 1, 0, 69078, 6068.75, 5396.01, 136.171, 2.32446, 604800, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, NULL),
(@CGUID+47, 69131, 1098, 0, 0, 248, 1, 0, 69131, 6025.24, 5393.14, 136.171, 0.699817, 604800, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, NULL),
(@CGUID+48, 69132, 1098, 0, 0, 248, 1, 0, 69132, 6035.9, 5385.33, 136.171, 1.17586, 604800, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, NULL),
(@CGUID+49, 69134, 1098, 0, 0, 248, 1, 0, 0, 6056.48, 5386.44, 136.171, 2.11191, 604800, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, NULL),

(@CGUID+50, 69899, 1098, 0, 0, 248, 1, 0, 69899, 6017.28, 5743.1, 123.242, 0.152315, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+51, 69899, 1098, 0, 0, 248, 1, 0, 69899, 5873.9, 5754.04, 129.03, 3.14486, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+52, 69899, 1098, 0, 0, 248, 1, 0, 69899, 5762.38, 5738.08, 124.047, 2.81813, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+53, 69899, 1098, 0, 0, 248, 1, 0, 0, 5663.36, 5768.88, 124.073, 3.64997, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+54, 69899, 1098, 0, 0, 248, 1, 0, 0, 5663.44, 5738.31, 124.072, 2.81813, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+55, 69899, 1098, 0, 0, 248, 1, 0, 0, 5762.89, 5769.07, 124.047, 3.53284, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+56, 69905, 1098, 0, 0, 248, 1, 0, 0, 5584.47, 5753.58, 124.096, 3.13424, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+57, 69905, 1098, 0, 0, 248, 1, 0, 0, 5651.97, 5753.4, 124.096, 3.1587, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+58, 69906, 1098, 0, 0, 248, 1, 0, 69906, 5857.67, 5741.87, 124.302, 3.20745, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+59, 69906, 1098, 0, 0, 248, 1, 0, 69906, 5857.61, 5765.46, 124.046, 3.20745, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+60, 69906, 1098, 0, 0, 248, 1, 0, 69906, 6039.14, 5765.86, 123.242, 4.64115, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+61, 69906, 1098, 0, 0, 248, 1, 0, 0, 5753.83, 5753.79, 123.953, 3.10666, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+62, 69909, 1098, 0, 0, 248, 1, 0, 69909, 5869.17, 5760.24, 126.903, 3.20745, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+63, 69909, 1098, 0, 0, 248, 1, 0, 69909, 6057.98, 5745.58, 123.242, 3.13719, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+64, 69909, 1098, 0, 0, 248, 1, 0, 0, 5592.1, 5763.32, 124.096, 3.30104, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+65, 69909, 1098, 0, 0, 248, 1, 0, 0, 5655.54, 5761.32, 124.096, 3.53003, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+66, 69909, 1098, 0, 0, 248, 1, 0, 0, 5753.12, 5763.39, 123.953, 3.53003, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+67, 69910, 1098, 0, 0, 248, 1, 0, 69910, 5869.4, 5746.92, 126.927, 3.12549, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+68, 69910, 1098, 0, 0, 248, 1, 0, 69910, 6034.67, 5724.74, 123.242, 1.44532, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+69, 69910, 1098, 0, 0, 248, 1, 0, 0, 5592.06, 5743.87, 124.096, 3.05921, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+70, 69910, 1098, 0, 0, 248, 1, 0, 0, 5655.31, 5745.27, 124.096, 2.84562, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+71, 69910, 1098, 0, 0, 248, 1, 0, 0, 5752.58, 5744.31, 123.953, 2.84562, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+72, 69911, 1098, 0, 0, 248, 1, 0, 69911, 6037.76, 5744.68, 123.84, 3.94453, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+73, 69916, 1098, 0, 0, 248, 1, 0, 0, 5849.38, 5753.12, 123.385, 3.14292, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+74, 69916, 1098, 0, 0, 248, 1, 0, 0, 5716.64, 5754.24, 123.099, 3.12866, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+75, 69927, 1098, 0, 0, 248, 1, 0, 69927, 6049.02, 5549.85, 123.907, 1.57388, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+76, 69927, 1098, 0, 0, 248, 1, 0, 69927, 5990.56, 5492.3, 131.652, 1.58347, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+77, 69927, 1098, 0, 0, 248, 1, 0, 69927, 6076.57, 5539.53, 123.908, 3.11403, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+78, 69927, 1098, 0, 0, 248, 1, 0, 0, 5990.88, 5532.83, 123.907, 4.69265, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+79, 69927, 1098, 0, 0, 248, 1, 0, 0, 6102.31, 5506.21, 124.686, 1.5339, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+80, 70557, 1098, 0, 0, 248, 1, 0, 70557, 5971.24, 5763.1, 123.997, 6.27124, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+81, 70557, 1098, 0, 0, 248, 1, 0, 70557, 5971.68, 5744.31, 123.993, 0.0670589, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+82, 70557, 1098, 0, 0, 248, 1, 0, 70557, 5985, 5481.58, 136.092, 1.58054, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+83, 70557, 1098, 0, 0, 248, 1, 0, 70557, 6033.63, 5677.82, 124.087, 1.55757, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+84, 70557, 1098, 0, 0, 248, 1, 0, 70557, 6042.95, 5557.03, 123.907, 1.55951, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+85, 70557, 1098, 0, 0, 248, 1, 0, 70557, 6025.1, 5536.17, 123.907, 0.069091, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+86, 70557, 1098, 0, 0, 248, 1, 0, 70557, 6058.24, 5678.03, 124.074, 1.58991, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+87, 70557, 1098, 0, 0, 248, 1, 0, 70557, 6079.19, 5533.8, 123.908, 3.11403, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+88, 70557, 1098, 0, 0, 248, 1, 0, 0, 5987.32, 5538.04, 123.907, 4.02757, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+89, 70557, 1098, 0, 0, 248, 1, 0, 0, 5996.28, 5486.42, 134.6, 1.58102, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+90, 70557, 1098, 0, 0, 248, 1, 0, 0, 6096.42, 5500.65, 127.673, 1.54294, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+91, 70557, 1098, 0, 0, 248, 1, 0, 0, 6103.13, 5539.81, 123.907, 2.33145, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+231, 662203, 1098, 0, 0, 248, 1, 0, 0, 6056.48, 5386.44, 136.171, 2.11191, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, NULL),
(@CGUID+232, 662204, 1098, 0, 0, 248, 1, 0, 0, 6056.48, 5386.44, 136.171, 2.11191, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, NULL),
(@CGUID+235, 69135, 1098, 0, 0, 248, 1, 0, 0, 6046.09, 5430.82, 137.088, 4.69324, 86400, 0, 0, 315153, 21262, 0, 0, 0, 0, 0, 0, 1, NULL);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Speeches
DELETE FROM `creature_text` WHERE `entry` IN (69078,69131,69132,69134,69135,70056);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(69078, 0,  0, 'Da sands will consume everyting!', 14, 0, 100, 0, 0, 35812, 'Sul the Sandcrawler - Aggro'),
(69078, 1,  0, 'I will bury ya all!', 14, 0, 100, 0, 0, 35813, 'Sul the Sandcrawler - On Possess'),
(69078, 2,  0, 'Da storm approaches!', 14, 0, 100, 0, 0, 35818, 'Sul the Sandcrawler - Sandstorm'),
(69078, 3,  0, 'Watch yer step!', 14, 0, 100, 0, 0, 35817, 'Sul the Sandcrawler - Quicksand'),
(69078, 4,  0, 'Da sands are endless.', 14, 0, 100, 0, 0, 35815, 'Sul the Sandcrawler - Kill01'),
(69078, 4,  1, 'Da first of many!', 14, 0, 100, 0, 0, 35816, 'Sul the Sandcrawler - Kill02'),
(69078, 5,  0, 'I return... to... da... sands...', 14, 0, 100, 0, 0, 36454, 'Sul the Sandcrawler - On Death'),
(69078, 6,  0, '|TInterface\\Icons\\achievement_moguraid_03:20|t%s is |cFFF00000|Hspell:136442|h[Possessed]|h|r by the spirit of Gara''jal!', 41, 0, 100, 0, 0, 0, 'Possess emote'),
(69078, 7,  0, '|TInterface\\Icons\\spell_monk_envelopingmist:20|t%s summons a deadly |cFFF00000|Hspell:136894|h[Sandstorm]|h|r!', 41, 0, 100, 0, 0, 0, 'Possess emote'),

(69131, 0,  0, 'Ya have met your match, fools!', 14, 0, 100, 0, 0, 35387, 'Frost King Malakk - Aggro'),
(69131, 1,  0, 'Winter is coming...', 14, 0, 100, 0, 0, 35389, 'Frost King Malakk - On Possess'),
(69131, 2,  0, 'Getting cold?', 14, 0, 100, 0, 0, 35393, 'Frost King Malakk - Frostbite'),
(69131, 2,  1, 'Freeze!', 14, 0, 100, 0, 0, 35394, 'Frost King Malakk - Frostbite'),
(69131, 3,  0, 'Death\'s cold embrace.', 14, 0, 100, 0, 0, 35391, 'Frost King Malakk - Kill01'),
(69131, 3,  1, 'Witness da Drakkari\'s might.', 14, 0, 100, 0, 0, 35392, 'Frost King Malakk - Kill02'),
(69131, 4,  0, 'Da... empire... can\'t... fall...', 14, 0, 100, 0, 0, 35388, 'Frost King Malakk - On Death'),
(69131, 5,  0, '|TInterface\\Icons\\spell_mage_frostbomb:20|t$n is afflicted with |cFFF00000|Hspell:136922|h[Frostbite]|h|r!', 41, 0, 100, 0, 0, 0, 'Frost King Malakk to Player'),
(69131, 6,  0, '|TInterface\\Icons\\achievement_moguraid_03:20|t%s is |cFFF00000|Hspell:136442|h[Possessed]|h|r by the spirit of Gara''jal!', 41, 0, 100, 0, 0, 0, 'Possess emote'),

(69132, 0,  0, 'Death ta all who appose da empire!', 14, 0, 100, 0, 0, 35432, 'High Priestess Marli - On Aggro'),
(69132, 1,  0, 'Da spiritbinder reveals yer soul ta me!', 14, 0, 100, 0, 0, 35434, 'High Priestess Marli - On Possess'),
(69132, 2,  0, 'Succumb ta her venom!', 14, 0, 100, 0, 0, 35438, 'High Priestess Marli - Shadowed Loa Spirit'),
(69132, 2,  1, 'Yer soul belongs ta me!', 14, 0, 100, 0, 0, 35439, 'High Priestess Marli - Shadowed Loa Spirit'),
(69132, 2,  2, 'Embrace yer demise!', 14, 0, 100, 0, 0, 35440, 'High Priestess Marli - Shadowed Loa Spirit'),
(69132, 3,  0, 'Another offering to da loa!', 14, 0, 100, 0, 0, 35436, 'High Priestess Marli - Kill01'),
(69132, 3,  1, 'She will feast on yer soul!', 14, 0, 100, 0, 0, 35437, 'High Priestess Marli - Kill02'),
(69132, 4,  0, 'Shadra... save... me...', 14, 0, 100, 0, 0, 35433, 'High Priestess Marli - On Death'),
(69132, 5,  0, '|TInterface\\Icons\\spell_priest_divinestar:20|t%s summons a |cFFF00000|Hspell:137203|h[Blessed Loa Spirit]|h|r!', 41, 0, 100, 0, 0, 0, 'High Priestess Mar''li'),
(69132, 6,  0, '|TInterface\\Icons\\achievement_moguraid_03:20|t%s is |cFFF00000|Hspell:136442|h[Possessed]|h|r by the spirit of Gara''jal!', 41, 0, 100, 0, 0, 0, 'Possess emote'),
(69132, 7,  0, '|TInterface\\Icons\\spell_priest_divinestar_shadow2:20|t%s summons a |cFFF00000|Hspell:137350|h[Shadowed Loa Spirit]|h|r!', 41, 0, 100, 0, 0, 0, 'High Priestess Mar''li'),

(69134, 0,  0, 'Dis is gonna hurt!', 14, 0, 100, 0, 0, 35566, 'Kazrajin - Aggro'),
(69134, 1,  0, 'Lei Shen! Give us strength!', 14, 0, 100, 0, 0, 35568, 'Kazrajin - On Possess'),
(69134, 2,  0, 'Shocking!', 14, 0, 100, 0, 0, 35576, 'Kazrajin - Overload /Discharge'),
(69134, 3,  0, 'Incoming!', 14, 0, 100, 0, 0, 35574, 'Kazrajin - Reckless Charge'),
(69134, 3,  1, 'Out da way!', 14, 0, 100, 0, 0, 35573, 'Kazrajin - Reckless Charge'),
(69134, 4,  0, 'Ya shouldn\'t be messin\' wit da Zandalari!', 14, 0, 100, 0, 0, 35571, 'Kazrajin - Kill01'),
(69134, 4,  1, 'On ya knees!', 14, 0, 100, 0, 0, '35572', 'Kazrajin - Kill02'),
(69134, 5,  0, 'Da thunder king... promised...', 14, 0, 100, 0, 0, 35567, 'Kazrajin - On Death'),
(69134, 6,  0, '|TInterface\\Icons\\achievement_moguraid_03:20|t%s is |cFFF00000|Hspell:136442|h[Possessed]|h|r by the spirit of Gara''jal!', 41, 0, 100, 0, 0, 0, 'Possess emote'),
(69134, 7,  0, '|TInterface\\Icons\\spell_shaman_staticshock:20|t%s |cFFF00000|Hspell:137149|h[Overloads]|h|r!', 41, 0, 100, 0, 0, 0, 'Kazra''jin to Reckless Charge'),

(69135, 0,  0, 'Let me share dis gift with ya!', 14, 0, 100, 11, 0, 35395, 'Gara\'jal the Spiritbinder- Event01'),

(70056, 0,  0, 'Let me share dis gift with ya!', 14, 0, 100, 11, 0, 35395, 'Gara\'jal the Spiritbinder- Event01'),
(70056, 1,  0, 'Ya\' spirit is mine', 14, 0, 100, 11, 0, 35396, 'Gara\'jal the Spiritbinder- Event02'),
(70056, 2,  0, 'De Thunder King will reward us for stoppin\' ya!', 14, 0, 100, 11, 0, 35397, 'Gara\'jal the Spiritbinder- Event03'),
(70056, 3,  0, 'Witness the power of da spiritbinder!', 14, 0, 100, 11, 0, '35398', 'Gara\'jal the Spiritbinder- Event04'),
(70056, 4,  0, 'Time to die!', 14, 0, 100, 0, 0, '35399', 'Gara\'jal the Spiritbinder- Event05'),
(70056, 5,  0, 'Da Zandalari cannot be stopped!', 14, 0, 100, 53, 0, 35400, 'Gara\'jal the Spiritbinder- Event06'),
(70056, 6,  0, 'Wit de Thunder King\'s power de Zandalari will be reborn!', 14, 0, 100, 53, 0, 35401, 'Gara\'jal the Spiritbinder- Event07'),
(70056, 7,  0, 'Lei Shen\'s power makes us stronger', 14, 0, 100, 53, 0, 35402, 'Gara\'jal the Spiritbinder- Event08'),
(70056, 8,  0, 'Ya\' soul will pay da price', 14, 0, 100, 0, 0, 35403, 'Gara\'jal the Spiritbinder- Event09'),
(70056, 9,  0, 'Des fools thought they beat me once before. Dey only make me stronger. Now we show dem da true power of the Zandalari!', 14, 0, 100, 11, 0, 35404,  'Gara\'jal the Spiritbinder- Event10'),
(70056, 10, 0, 'Lei Shen let us prove ta ya the might of the Zandalari. We will crush des intruders where dey stand!', 14, 0, 100, 0, 0, 35405, 'Gara\'jal the Spiritbinder- Event11'),
(70056, 11, 0, 'We will never fail ya!', 14, 0, 100, 0, 0, 35406, 'Gara\'jal the Spiritbinder- Event12');

/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Gameobjects templates
REPLACE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `AIName`, `ScriptName`, `WDBVerified`) VALUES
(218655, 0, 13679, 'Zandalari Council Entry - Left', '', '', '', 1375, 48, 1.1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 1),
(218656, 0, 13679, 'Zandalari Council Entry - Right', '', '', '', 1375, 48, 1.1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 1),
(218657, 0, 13679, 'Zandalari Council Exit - Inner', '', '', '', 1375, 48, 1.1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218658, 0, 13679, 'Zandalari Council Door', '', '', '', 0, 32, 1.1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218659, 0, 13679, 'Zandalari Council Trash Door One - Outer', '', '', '', 1375, 48, 1.1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218660, 0, 13679, 'Zandalari Council Door', '', '', '', 0, 32, 1.1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218661, 0, 13679, 'Zandalari Council Trash Door Two - Outer', '', '', '', 1375, 48, 1.1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218668, 0, 13679, 'Wrought Iron Door', '', '', '', 1375, 48, 1.1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Gameobjects spawns
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(@OGUID+9, 218657, 1098, 760, 1, 6045.99, 5331.18, 141.935, 4.71239, 0, 0, 1, -4.37114E-8, 86400, 255, 1),
(@OGUID+10, 218658, 1098, 760, 1, 5883.67, 5753.58, 130.073, 3.14159, 0, 0, 1, -4.37114E-8, 86400, 255, 1),
(@OGUID+11, 218659, 1098, 760, 1, 5944.73, 5753.58, 130.073, 3.14159, 0, 0, 1, -4.37114E-8, 86400, 255, 0),
(@OGUID+12, 218660, 1098, 760, 1, 6046.06, 5652.13, 130.073, 4.71239, 0, 0, 1, -4.37114E-8, 86400, 255, 1),
(@OGUID+13, 218661, 1098, 760, 1, 6046.1, 5591.22, 130.073, 4.71239, 0, 0, 1, -4.37114E-8, 86400, 255, 0),
(@OGUID+14, 218668, 1098, 760, 1, 5627.85, 5753.58, 124.029, 3.14159, 0, 0, 1, -4.37114E-8, 86400, 255, 0),
(@OGUID+24, 218655, 1098, 760, 1, 6101.59, 5473.14, 136.046, 4.71239, 0, 0, 1, -4.37114E-8, 86400, 255, 0),
(@OGUID+25, 218656, 1098, 760, 1, 5990.46, 5473.14, 136.046, 4.71239, 0, 0, 1, -4.37114E-8, 86400, 255, 0);
/*---------------------------------------------------------------------------------------------------------------*/
/* END Council of Elders */





/* BEGIN Tortos */
/*---------------------------------------------------------------------------------------------------------------*/
-- Spell Scripts
UPDATE creature_template SET ScriptName = "boss_tortos" WHERE entry = 67977;
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Templates
REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES 
(66305, 0, 0, 0, 0, 0, 22903, 11686, 0, 0, 'Generic Bunny - PRK (Gigantic AOI)', '', '', 0, 60, 60, 4, 35, 35, 0, 1, 1.14286, 1, 0, 9839, 14339, 0, 42299, 1, 2000, 2000, 1, 33554688, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1048576, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 128, '', 16048),
(67966, 0, 0, 0, 0, 0, 47499, 0, 0, 0, 'Whirl Turtle', '', '', 0, 93, 93, 4, 16, 16, 0, 2.4, 2.4, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 7.22, 1, 1, 0, 0, 0, 0, 0, 0, 0, 164, 1, 0, 0, 'npc_whirl_turtle', 17614),
(67977, 0, 0, 0, 0, 0, 46559, 0, 0, 0, 'Tortos', '', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 64839, 78339, 0, 31299, 2, 2000, 2000, 4, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 1, 76, 67977, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2115, 3100000, 3500000, '', 0, 3, 1, 330.17, 100, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 652165119, 1, 'boss_tortos', 17614),
(68219, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Rockfall', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 0, 9839, 14339, 0, 42299, 1, 2000, 2000, 1, 33554436/*33587200*/, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'npc_rockfall_tortos', 17614),
(68497, 0, 0, 0, 0, 0, 46824, 0, 0, 0, 'Vampiric Cave Bat', '', '', 0, 93, 93, 4, 0, 0, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 4.13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 1, 0, 0, 'npc_vampiric_cave_bat', 1),
(69351, 0, 0, 0, 0, 0, 48248, 0, 0, 0, 'Greater Cave Bat', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 4, 1, 30, 1, 1, 0, 0, 0, 0, 0, 0, 0, 130, 1, 0, 0, '', 17614),
(69352, 0, 0, 0, 0, 0, 46824, 0, 0, 0, 'Vampiric Cave Bat', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 4, 1, 4.13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 1, 0, 0, '', 17614),
(70147, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Waterspout', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 0, 9839, 14339, 0, 42299, 1, 2000, 2000, 1, 33554438, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 128, '', 17614),
(70437, 0, 0, 0, 0, 0, 46770, 0, 0, 0, 'Lei Shen', 'The Thunder King', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1.14286, 3, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 600, 100, 1, 0, 0, 0, 0, 0, 0, 0, 164, 1, 0, 0, 'npc_lei_shen_tortos', 17614),

(@RAID_DIFF_10N + 67966, 0, 0, 0, 0, 0, 47499, 0, 0, 0, 'Whirl Turtle', '', '', 0, 93, 93, 4, 16, 16, 0, 2.4, 2.4, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 7.22, 1, 1, 0, 0, 0, 0, 0, 0, 0, 164, 1, 0, 0, '', 17614),
(@RAID_DIFF_10N + 67977, 0, 0, 0, 0, 0, 46559, 0, 0, 0, 'Tortos', '', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 64839, 78339, 0, 31299, 2, 2000, 2000, 4, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 1, 76, 67977, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2115, 3100000, 3500000, '', 0, 3, 1, 330.17, 100, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 652165119, 1, 'boss_tortos', 17614),
(@RAID_DIFF_10N + 68497, 0, 0, 0, 0, 0, 46824, 0, 0, 0, 'Vampiric Cave Bat', '', '', 0, 93, 93, 4, 0, 0, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 4.13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 1, 0, 0, '', 1),
(@RAID_DIFF_10N + 69351, 0, 0, 0, 0, 0, 48248, 0, 0, 0, 'Greater Cave Bat', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 4, 1, 30, 1, 1, 0, 0, 0, 0, 0, 0, 0, 130, 1, 0, 0, '', 17614),
(@RAID_DIFF_10N + 69352, 0, 0, 0, 0, 0, 46824, 0, 0, 0, 'Vampiric Cave Bat', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 4, 1, 4.13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 1, 0, 0, '', 17614),

(@RAID_DIFF_25N + 67966, 0, 0, 0, 0, 0, 47499, 0, 0, 0, 'Whirl Turtle', '', '', 0, 93, 93, 4, 16, 16, 0, 2.4, 2.4, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 7.22, 1, 1, 0, 0, 0, 0, 0, 0, 0, 164, 1, 0, 0, '', 17614),
(@RAID_DIFF_25N + 67977, 0, 0, 0, 0, 0, 46559, 0, 0, 0, 'Tortos', '', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 64839, 78339, 0, 31299, 2, 2000, 2000, 4, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 1, 76, 67977, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2115, 3100000, 3500000, '', 0, 3, 1, 330.17, 100, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 652165119, 1, 'boss_tortos', 17614),
(@RAID_DIFF_25N + 68497, 0, 0, 0, 0, 0, 46824, 0, 0, 0, 'Vampiric Cave Bat', '', '', 0, 93, 93, 4, 0, 0, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 4.13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 1, 0, 0, '', 1),
(@RAID_DIFF_25N + 69351, 0, 0, 0, 0, 0, 48248, 0, 0, 0, 'Greater Cave Bat', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 4, 1, 30, 1, 1, 0, 0, 0, 0, 0, 0, 0, 130, 1, 0, 0, '', 17614),
(@RAID_DIFF_25N + 69352, 0, 0, 0, 0, 0, 46824, 0, 0, 0, 'Vampiric Cave Bat', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 4, 1, 4.13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 1, 0, 0, '', 17614),

(@RAID_DIFF_10H + 67966, 0, 0, 0, 0, 0, 47499, 0, 0, 0, 'Whirl Turtle', '', '', 0, 93, 93, 4, 16, 16, 0, 2.4, 2.4, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 7.22, 1, 1, 0, 0, 0, 0, 0, 0, 0, 164, 1, 0, 0, '', 17614),
(@RAID_DIFF_10H + 67977, 0, 0, 0, 0, 0, 46559, 0, 0, 0, 'Tortos', '', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 64839, 78339, 0, 31299, 2, 2000, 2000, 4, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 1, 76, 67977, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2115, 3100000, 3500000, '', 0, 3, 1, 330.17, 100, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 652165119, 1, 'boss_tortos', 17614),
(@RAID_DIFF_10H + 68497, 0, 0, 0, 0, 0, 46824, 0, 0, 0, 'Vampiric Cave Bat', '', '', 0, 93, 93, 4, 0, 0, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 4.13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 1, 0, 0, '', 1),
(@RAID_DIFF_10H + 69351, 0, 0, 0, 0, 0, 48248, 0, 0, 0, 'Greater Cave Bat', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 4, 1, 30, 1, 1, 0, 0, 0, 0, 0, 0, 0, 130, 1, 0, 0, '', 17614),
(@RAID_DIFF_10H + 69352, 0, 0, 0, 0, 0, 46824, 0, 0, 0, 'Vampiric Cave Bat', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 4, 1, 4.13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 1, 0, 0, '', 17614),

(@RAID_DIFF_25H + 67966, 0, 0, 0, 0, 0, 47499, 0, 0, 0, 'Whirl Turtle', '', '', 0, 93, 93, 4, 16, 16, 0, 2.4, 2.4, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 7.22, 1, 1, 0, 0, 0, 0, 0, 0, 0, 164, 1, 0, 0, '', 17614),
(@RAID_DIFF_25H + 67977, 0, 0, 0, 0, 0, 46559, 0, 0, 0, 'Tortos', '', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 64839, 78339, 0, 31299, 2, 2000, 2000, 4, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 1, 76, 67977, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2115, 3100000, 3500000, '', 0, 3, 1, 330.17, 100, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 652165119, 1, 'boss_tortos', 17614),
(@RAID_DIFF_25H + 68497, 0, 0, 0, 0, 0, 46824, 0, 0, 0, 'Vampiric Cave Bat', '', '', 0, 93, 93, 4, 0, 0, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 4.13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 1, 0, 0, '', 1),
(@RAID_DIFF_25H + 69351, 0, 0, 0, 0, 0, 48248, 0, 0, 0, 'Greater Cave Bat', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 4, 1, 30, 1, 1, 0, 0, 0, 0, 0, 0, 0, 130, 1, 0, 0, '', 17614),
(@RAID_DIFF_25H + 69352, 0, 0, 0, 0, 0, 46824, 0, 0, 0, 'Vampiric Cave Bat', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 4, 1, 4.13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 1, 0, 0, '', 17614),

(@RAID_DIFF_25R + 67966, 0, 0, 0, 0, 0, 47499, 0, 0, 0, 'Whirl Turtle', '', '', 0, 93, 93, 4, 16, 16, 0, 2.4, 2.4, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 7.22, 1, 1, 0, 0, 0, 0, 0, 0, 0, 164, 1, 0, 0, '', 17614),
(@RAID_DIFF_25R + 67977, 0, 0, 0, 0, 0, 46559, 0, 0, 0, 'Tortos', '', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 64839, 78339, 0, 31299, 2, 2000, 2000, 4, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 1, 76, 67977, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2115, 3100000, 3500000, '', 0, 3, 1, 330.17, 100, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 652165119, 1, 'boss_tortos', 17614),
(@RAID_DIFF_25R + 68497, 0, 0, 0, 0, 0, 46824, 0, 0, 0, 'Vampiric Cave Bat', '', '', 0, 93, 93, 4, 0, 0, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 4.13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 1, 0, 0, '', 1),
(@RAID_DIFF_25R + 69351, 0, 0, 0, 0, 0, 48248, 0, 0, 0, 'Greater Cave Bat', '', '', 0, 92, 92, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 4, 1, 30, 1, 1, 0, 0, 0, 0, 0, 0, 0, 130, 1, 0, 0, '', 17614),
(@RAID_DIFF_25R + 69352, 0, 0, 0, 0, 0, 46824, 0, 0, 0, 'Vampiric Cave Bat', '', '', 0, 91, 91, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 4, 1, 4.13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 1, 0, 0, '', 17614);
/*---------------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------------------*/
-- Updates
-- Creature Trash Mobs Damage, HP, LootId
-- Base HP Gamepedia Raid 25N
-- 25R -10%
-- 25H +25%
-- 10N (BASE HP: 25 = X : 10)
-- 10H=(10N+25%)

-- Immunity and bind bosses
UPDATE creature_template SET flags_extra = flags_extra | 1, mechanic_immune_mask = 667893759 WHERE entry IN 
(67977,3167977,3267977,3367977,3467977,3567977);

-- Difficulty entries bosses and Trash Mobs
UPDATE creature_template SET difficulty_entry_3 = @RAID_DIFF_10N + entry, difficulty_entry_4 = @RAID_DIFF_25N + entry, difficulty_entry_5 = @RAID_DIFF_10H + entry, difficulty_entry_6 = @RAID_DIFF_25H + entry, difficulty_entry_7 = @RAID_DIFF_25R + entry WHERE entry IN 
(67966,67977,68497,69351,69352);
UPDATE creature_template SET flags_extra = flags_extra | 1, mechanic_immune_mask = 667893759 WHERE entry IN (67977,3167977,3267977,3367977,3467977,3567977);

-- Whirl Turtle
UPDATE creature_template SET InhabitType = 1, speed_walk = 1.5, speed_run = 1.5, LootId = 0, dmg_multiplier = 5, Health_Mod = 4199999  / @HP_MOD_93 WHERE entry = @RAID_DIFF_10N + 67966;
UPDATE creature_template SET InhabitType = 1, speed_walk = 1.5, speed_run = 1.5, LootId = 0, LootId = 0, dmg_multiplier = 6, Health_Mod = 5249999  / @HP_MOD_93 WHERE entry = @RAID_DIFF_10H + 67966;
UPDATE creature_template SET InhabitType = 1, speed_walk = 1.5, speed_run = 1.5, LootId = 0, LootId = 0, dmg_multiplier = 8, Health_Mod = 10499999 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25N + 67966;
UPDATE creature_template SET InhabitType = 1, speed_walk = 1.5, speed_run = 1.5, LootId = 0, LootId = 0, dmg_multiplier = 9, Health_Mod = 13124999 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25H + 67966;
UPDATE creature_template SET InhabitType = 1, speed_walk = 1.5, speed_run = 1.5, LootId = 0, dmg_multiplier = 7, Health_Mod = 9449999  / @HP_MOD_93 WHERE entry = @RAID_DIFF_25R + 67966;

-- Vampiric Cave Bat
UPDATE creature_template SET LootId = 0, dmg_multiplier = 5, Health_Mod = 3988400  / @HP_MOD_93 WHERE entry = @RAID_DIFF_10N + 68497;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 6, Health_Mod = 4810500  / @HP_MOD_93 WHERE entry = @RAID_DIFF_10H + 68497;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 8, Health_Mod = 8921000  / @HP_MOD_93 WHERE entry = @RAID_DIFF_25N + 68497;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 9, Health_Mod = 17276250 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25H + 68497;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 7, Health_Mod = 8098900  / @HP_MOD_93 WHERE entry = @RAID_DIFF_25R + 68497;

-- Greater Cave Bat
UPDATE creature_template SET LootId = 69388,   dmg_multiplier = 5, Health_Mod = 16863600 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 69351;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 6, Health_Mod = 21079500 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 69351;
UPDATE creature_template SET LootId = 69388,   dmg_multiplier = 8, Health_Mod = 42159000 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 69351;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 9, Health_Mod = 52698750 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 69351;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 7, Health_Mod = 37943100 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 69351;

-- Vampiric Cave Bat
UPDATE creature_template SET LootId = 69388,   dmg_multiplier = 4, Health_Mod = 3288400  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 69352;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 5, Health_Mod = 4110500  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 69352;
UPDATE creature_template SET LootId = 69388,   dmg_multiplier = 7, Health_Mod = 8221000  / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 69352;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 8, Health_Mod = 10276250 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 69352;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 6, Health_Mod = 7398900  / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 69352;

-- Tortos
UPDATE creature_template SET LootId = @RAID_DIFF_10N + 67977, dmg_multiplier = 11, Health_Mod = 179999840  / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_10N + 67977;
UPDATE creature_template SET LootId = @RAID_DIFF_10H + 67977, dmg_multiplier = 12, Health_Mod = 319999808  / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_10H + 67977;
UPDATE creature_template SET LootId = @RAID_DIFF_25N + 67977, dmg_multiplier = 14, Health_Mod = 599998016  / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_25N + 67977;
UPDATE creature_template SET LootId = @RAID_DIFF_25H + 67977, dmg_multiplier = 15, Health_Mod = 1000001152 / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_25H + 67977;
UPDATE creature_template SET LootId = @RAID_DIFF_25R + 67977, dmg_multiplier = 13, Health_Mod = 389998496  / @HP_MOD_93_4 WHERE entry = @RAID_DIFF_25R + 67977;
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Trash Mobs SAI
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (68497,69351,69352);
DELETE FROM `smart_scripts` WHERE `entryorguid`IN (68497,69351,69352) AND `source_type`= 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
-- (68497, 0, 0, 0, 1, 0, 100, 1, 1, 1, 0, 0, 11, 135103, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Vampiric Cave Bat(Tortos) - Cast Drain the Weak"),

(69351, 0, 0, 0, 0, 0, 50, 0, 5000, 7000, 15000, 18000, 11, 136753, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Greater Cave Bat - Slashing Talons"),
(69351, 0, 1, 0, 0, 0, 30, 0, 9000, 10000, 12000, 20000, 11, 136751, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Greater Cave Bat - Sonic Screech"),

(69352, 0, 0, 0, 1, 0, 100, 1, 1, 1, 0, 0, 11, 135103, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Vampiric Cave Bat - Cast Drain the Weak");
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Tortos Loots
DELETE FROM `creature_loot_template` WHERE entry IN(3167977, 3267977, 3367977, 3467977, 3567977);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
-- 10 Normal
(3167977, 94782, 0, 1, 1, 1, 1),
(3167977, 94779, 0, 1, 1, 1, 1),
(3167977, 94783, 0, 1, 1, 1, 1),
(3167977, 94773, 0, 1, 1, 1, 1),
(3167977, 94781, 0, 1, 1, 1, 1),
(3167977, 94770, 0, 1, 1, 1, 1),
(3167977, 94775, 0, 1, 1, 1, 1),
(3167977, 94771, 0, 1, 1, 1, 1),
(3167977, 94780, 0, 1, 1, 1, 1),
(3167977, 94786, 0, 1, 1, 1, 1),
(3167977, 94772, 0, 1, 1, 1, 1),
(3167977, 94787, 0, 1, 1, 1, 1),
(3167977, 94784, 0, 1, 1, 1, 1),
(3167977, 94777, 0, 1, 1, 1, 1),
(3167977, 94774, 0, 1, 2, 1, 1),
(3167977, 94785, 0, 1, 2, 1, 1),
(3167977, 94776, 0, 1, 2, 1, 1),
(3167977, 94768, 0, 1, 2, 1, 1),
(3167977, 94769, 0, 1, 2, 1, 1),
(3167977, 94778, 0, 1, 2, 1, 1),
(3167977, 94593, 1, 1, 3, 1, 1),
(3167977, 94594, 1, 1, 3, 1, 1),
(3167977, 87208, 1, 1, 3, 1, 1),
(3167977, 87209, 1, 1, 3, 1, 1),
(3167977, 95498, 5, 1, 10, -95498, 1),

-- 10 Heroic
(3267977, 96433, 0, 1, 1, 1, 1),
(3267977, 96438, 0, 1, 1, 1, 1),
(3267977, 96439, 0, 1, 1, 1, 1),
(3267977, 96427, 0, 1, 1, 1, 1),
(3267977, 96432, 0, 1, 1, 1, 1),
(3267977, 96423, 0, 1, 1, 1, 1),
(3267977, 96425, 0, 1, 1, 1, 1),
(3267977, 96428, 0, 1, 1, 1, 1),
(3267977, 96434, 0, 1, 1, 1, 1),
(3267977, 96437, 0, 1, 1, 1, 1),
(3267977, 96426, 0, 1, 1, 1, 1),
(3267977, 96441, 0, 1, 1, 1, 1),
(3267977, 96440, 0, 1, 1, 1, 1),
(3267977, 96424, 0, 1, 1, 1, 1),
(3267977, 96431, 0, 1, 2, 1, 1),
(3267977, 96435, 0, 1, 2, 1, 1),
(3267977, 96429, 0, 1, 2, 1, 1),
(3267977, 96430, 0, 1, 2, 1, 1),
(3267977, 96422, 0, 1, 2, 1, 1),
(3267977, 96436, 0, 1, 2, 1, 1),
(3267977, 94593, 1, 1, 3, 1, 1),
(3267977, 94594, 1, 1, 3, 1, 1),
(3267977, 87208, 1, 1, 3, 1, 1),
(3267977, 87209, 1, 1, 3, 1, 1),
(3267977, 96621, 5, 1, 10, -96621, 1),

-- 25 Normal
(3367977, 94782, 0, 1, 1, 1, 1),
(3367977, 94779, 0, 1, 1, 1, 1),
(3367977, 94783, 0, 1, 1, 1, 1),
(3367977, 94773, 0, 1, 1, 1, 1),
(3367977, 94781, 0, 1, 2, 1, 1),
(3367977, 94770, 0, 1, 2, 1, 1),
(3367977, 94775, 0, 1, 2, 1, 1),
(3367977, 94771, 0, 1, 2, 1, 1),
(3367977, 94780, 0, 1, 3, 1, 1),
(3367977, 94786, 0, 1, 3, 1, 1),
(3367977, 94772, 0, 1, 3, 1, 1),
(3367977, 94787, 0, 1, 3, 1, 1),
(3367977, 94774, 0, 1, 4, 1, 1),
(3367977, 94777, 0, 1, 4, 1, 1),
(3367977, 94785, 0, 1, 4, 1, 1),
(3367977, 94784, 0, 1, 4, 1, 1),
(3367977, 94776, 0, 1, 5, 1, 1),
(3367977, 94768, 0, 1, 5, 1, 1),
(3367977, 94769, 0, 1, 5, 1, 1),
(3367977, 94778, 0, 1, 5, 1, 1),
(3367977, 94593, 1, 1, 6, 1, 1),
(3367977, 94594, 1, 1, 6, 1, 1),
(3367977, 87208, 1, 1, 6, 1, 1),
(3367977, 87209, 1, 1, 6, 1, 1),
(3367977, 95498, 5, 1, 10, -95498, 1),

-- 25 Heroic
(3467977, 96433, 0, 1, 1, 1, 1),
(3467977, 96438, 0, 1, 1, 1, 1),
(3467977, 96439, 0, 1, 1, 1, 1),
(3467977, 96427, 0, 1, 1, 1, 1),
(3467977, 96432, 0, 1, 2, 1, 1),
(3467977, 96423, 0, 1, 2, 1, 1),
(3467977, 96425, 0, 1, 2, 1, 1),
(3467977, 96428, 0, 1, 2, 1, 1),
(3467977, 96434, 0, 1, 3, 1, 1),
(3467977, 96437, 0, 1, 3, 1, 1),
(3467977, 96426, 0, 1, 3, 1, 1),
(3467977, 96441, 0, 1, 3, 1, 1),
(3467977, 96431, 0, 1, 4, 1, 1),
(3467977, 96424, 0, 1, 4, 1, 1),
(3467977, 96435, 0, 1, 4, 1, 1),
(3467977, 96440, 0, 1, 4, 1, 1),
(3467977, 96429, 0, 1, 5, 1, 1),
(3467977, 96430, 0, 1, 5, 1, 1),
(3467977, 96422, 0, 1, 5, 1, 1),
(3467977, 96436, 0, 1, 5, 1, 1),
(3467977, 94593, 1, 1, 6, 1, 1),
(3467977, 94594, 1, 1, 6, 1, 1),
(3467977, 87208, 1, 1, 6, 1, 1),
(3467977, 87209, 1, 1, 6, 1, 1),
(3467977, 96621, 5, 1, 10, -96621, 1),

-- 25 LFR
(3567977, 95689, 0, 1, 1, 1, 1),
(3567977, 95694, 0, 1, 1, 1, 1),
(3567977, 95695, 0, 1, 1, 1, 1),
(3567977, 95683, 0, 1, 1, 1, 1),
(3567977, 95688, 0, 1, 2, 1, 1),
(3567977, 95679, 0, 1, 2, 1, 1),
(3567977, 95681, 0, 1, 2, 1, 1),
(3567977, 95684, 0, 1, 2, 1, 1),
(3567977, 95690, 0, 1, 3, 1, 1),
(3567977, 95693, 0, 1, 3, 1, 1),
(3567977, 95682, 0, 1, 3, 1, 1),
(3567977, 95697, 0, 1, 3, 1, 1),
(3567977, 95687, 0, 1, 4, 1, 1),
(3567977, 95680, 0, 1, 4, 1, 1),
(3567977, 95691, 0, 1, 4, 1, 1),
(3567977, 95696, 0, 1, 4, 1, 1),
(3567977, 95685, 0, 1, 5, 1, 1),
(3567977, 95686, 0, 1, 5, 1, 1),
(3567977, 95678, 0, 1, 5, 1, 1),
(3567977, 95692, 0, 1, 5, 1, 1),
(3567977, 95877, 5, 1, 10, -95877, 1);

 DELETE FROM `creature_template_currency` WHERE `entry` IN(3167977,3267977,3367977,3467977);
 INSERT INTO `creature_template_currency` VALUES
(3167977, 396, 40),
(3267977, 396, 40),
(3367977, 396, 40),
(3467977, 396, 40);
/*----------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Spawn
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`npcflag2`,`unit_flags`,`unit_flags2`,`dynamicflags`,`isActive`,`protec_anti_doublet`) VALUES
(@CGUID+252, 69352, 1098, 0, 0, 248, 1, 0, 0, 6043.57, 5024.67, -45.1575, 4.02787, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+253, 69352, 1098, 0, 0, 248, 1, 0, 0, 6035.2, 5034.17, -44.6659, 2.12154, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+254, 69352, 1098, 0, 0, 248, 1, 0, 0, 6044.73, 5039.48, -43.0669, 2.29622, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+255, 69352, 1098, 0, 0, 248, 1, 0, 0, 6038.34, 5038.32, -47.4582, 1.14578, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+256, 69352, 1098, 0, 0, 248, 1, 0, 0, 6050.57, 5036.44, -44.6835, 1.0141, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+257, 69352, 1098, 0, 0, 248, 1, 0, 0, 6044.19, 5032.21, -48.5718, 3.61766, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+258, 69352, 1098, 0, 0, 248, 1, 0, 0, 6036.25, 5027.81, -49.2093, 5.33996, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+259, 69352, 1098, 0, 0, 248, 1, 0, 0, 6050.03, 5027.6, -46.8344, 5.84741, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+260, 69352, 1098, 0, 0, 248, 1, 0, 0, 6074.6, 4961.67, -53.7021, 3.46128, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+261, 69352, 1098, 0, 0, 248, 1, 0, 0, 6021.18, 4962.99, -48.4501, 3.25903, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+262, 69352, 1098, 0, 0, 248, 1, 0, 0, 6010.36, 4978.15, -51.4841, 1.50119, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+263, 69352, 1098, 0, 0, 248, 1, 0, 0, 6022.64, 4963.94, -51.8095, 2.03271, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+264, 69352, 1098, 0, 0, 248, 1, 0, 0, 6057.7, 4966.91, -59.6045, 5.6567, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+265, 69352, 1098, 0, 0, 248, 1, 0, 0, 6062.98, 4974.89, -58.5278, 2.44762, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+266, 69352, 1098, 0, 0, 248, 1, 0, 0, 6005.02, 4973.16, -48.833, 0.856589, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+267, 69352, 1098, 0, 0, 248, 1, 0, 0, 6012.12, 4980.3, -52.9108, 3.39593, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+268, 69352, 1098, 0, 0, 248, 1, 0, 0, 6068.35, 4968.09, -56.8125, 3.47262, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+269, 69352, 1098, 0, 0, 248, 1, 0, 0, 6022.88, 4975.72, -48.585, 0.380812, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+270, 69352, 1098, 0, 0, 248, 1, 0, 0, 6076.43, 4972.5, -58.3398, 1.41455, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+271, 69352, 1098, 0, 0, 248, 1, 0, 0, 6024.04, 4976.68, -52.6364, 1.99241, 86400, 0, 0, 1801246, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),

(@CGUID+272, 69351, 1098, 0, 0, 248, 1, 0, 0, 6065.22, 4956.06, -52.6922, 3.55908, 86400, 0, 0, 12647760, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+273, 69351, 1098, 0, 0, 248, 1, 0, 0, 6012.18, 4968.28, -49.9244, 1.46249, 86400, 0, 0, 12647760, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+274, 69351, 1098, 0, 0, 248, 1, 0, 0, 6061.41, 4960.45, -55.404, 0.787075, 86400, 0, 0, 12647760, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+275, 69351, 1098, 0, 0, 248, 1, 0, 0, 6016.01, 4973.92, -48.6481, 2.14406, 86400, 0, 0, 12647760, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),
(@CGUID+276, 69351, 1098, 0, 0, 248, 1, 0, 0, 6066.52, 4964.4, -56.2341, 4.12029, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 2048, 0, 0, NULL),

(@CGUID+277, 67977, 1098, 0, 0, 248, 1, 0, 0, 6040.5, 4919.05, -61.1904, 1.54503, 604800, 0, 0, 536137, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+278, 70437, 1098, 0, 0, 248, 1, 0, 0, 6045.95, 4988.01, 148.02, 1.58414, 86400, 0, 0, 261682208, 0, 0, 0, 0, 0, 0, 0, 1, NULL),

(@CGUID+467, 70147, 1098, 0, 0, 248, 1, 0, 0, 6050.44, 5078.89, -43.7225, 4.72581, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+468, 70147, 1098, 0, 0, 248, 1, 0, 0, 6057.92, 5092.84, -42.132, 4.72581, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+469, 70147, 1098, 0, 0, 248, 1, 0, 0, 6023.34, 5084.92, -42.6315, 4.72581, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+470, 70147, 1098, 0, 0, 248, 1, 0, 0, 6061.68, 5107.14, -42.2052, 4.72581, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+471, 70147, 1098, 0, 0, 248, 1, 0, 0, 6048.51, 5115.09, -42.5244, 4.72581, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+472, 70147, 1098, 0, 0, 248, 1, 0, 0, 6033.59, 5114.4, -42.6068, 4.72581, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+473, 70147, 1098, 0, 0, 248, 1, 0, 0, 6035.4, 5076.46, -43.7633, 4.72581, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+474, 70147, 1098, 0, 0, 248, 1, 0, 0, 6025.6, 5100.95, -42.4843, 4.72581, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+475, 70147, 1098, 0, 0, 248, 1, 0, 0, 6041.07, 5045.28, -53.7597, 4.72581, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+476, 70147, 1098, 0, 0, 248, 1, 0, 0, 6036.37, 5012.54, -61.1061, 4.72581, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+618, 70147, 1098, 0, 0, 248, 1, 0, 0, 6042.98, 5097.39, -43.1555, 1.36953, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

/*
(@CGUID+477, 67966, 1098, 0, 0, 248, 1, 0, 0, 6078.54, 4830.96, -62.2238, 1.49548, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+478, 67966, 1098, 0, 0, 248, 1, 0, 0, 6065.28, 4892.32, -61.1895, 4.70813, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+479, 67966, 1098, 0, 0, 248, 1, 0, 0, 6078.38, 4893.15, -61.1895, 2.64174, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+480, 67966, 1098, 0, 0, 248, 1, 0, 0, 6071.43, 4866.6, -61.1893, 1.45336, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+481, 67966, 1098, 0, 0, 248, 1, 0, 0, 6050.37, 4827.97, -62.2417, 6.08787, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+482, 67966, 1098, 0, 0, 248, 1, 0, 0, 6073.93, 4882.41, -61.1894, 3.99132, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+483, 67966, 1098, 0, 0, 248, 1, 0, 0, 6061.32, 4850.06, -61.1893, 0.673142, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+484, 67966, 1098, 0, 0, 248, 1, 0, 0, 6096.2, 4876.28, -61.1893, 3.96597, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+485, 67966, 1098, 0, 0, 248, 1, 0, 0, 6022.92, 4870.82, -61.1893, 4.80973, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+486, 67966, 1098, 0, 0, 248, 1, 0, 0, 6008.38, 4887.23, -61.1894, 0.162594, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+487, 67966, 1098, 0, 0, 248, 1, 0, 0, 6093.66, 4858.37, -61.1893, 6.06862, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+488, 67966, 1098, 0, 0, 248, 1, 0, 0, 6047.94, 4859.46, -61.1893, 0.808521, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+489, 67966, 1098, 0, 0, 248, 1, 0, 0, 6029.36, 4861.76, -61.1893, 2.94339, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+490, 67966, 1098, 0, 0, 248, 1, 0, 0, 6035.02, 4833.29, -61.8691, 5.34923, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+491, 67966, 1098, 0, 0, 248, 1, 0, 0, 6036.38, 4872.56, -61.1893, 2.29072, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+492, 67966, 1098, 0, 0, 248, 1, 0, 0, 6085.74, 4874.79, -61.1893, 2.91476, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+493, 67966, 1098, 0, 0, 248, 1, 0, 0, 6055.85, 4861.88, -61.1893, 4.75011, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+494, 67966, 1098, 0, 0, 248, 1, 0, 0, 6075.26, 4849.81, -61.1893, 3.54483, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+495, 67966, 1098, 0, 0, 248, 1, 0, 0, 6056.23, 4880.12, -61.1894, 4.31428, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
*/

(@CGUID+604, 66305, 1098, 0, 0, 248, 1, 0, 0, 6047.33, 5087.78, 153.908, 4.57215, 300, 0, 0, 4979, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+605, 66305, 1098, 0, 0, 248, 1, 0, 0, 6026.5, 5106.91, 153.804, 2.42827, 300, 0, 0, 4979, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+606, 66305, 1098, 0, 0, 248, 1, 0, 0, 6025.67, 5087.18, 153.356, 0.771854, 300, 0, 0, 4979, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+607, 66305, 1098, 0, 0, 248, 1, 0, 0, 6049.28, 5062.07, 148.096, 5.31224, 300, 0, 0, 4979, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+608, 66305, 1098, 0, 0, 248, 1, 0, 0, 6039.79, 5051.89, 148.096, 3.87497, 300, 0, 0, 4979, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+609, 66305, 1098, 0, 0, 248, 1, 0, 0, 6070.26, 5091.01, 153.377, 1.50228, 300, 0, 0, 4979, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+610, 66305, 1098, 0, 0, 248, 1, 0, 0, 6044.54, 5143.17, 148.346, 0.0351406, 300, 0, 0, 4979, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+611, 66305, 1098, 0, 0, 248, 1, 0, 0, 6055.46, 5150.01, 148.127, 0.559787, 300, 0, 0, 4979, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+612, 66305, 1098, 0, 0, 248, 1, 0, 0, 6046.37, 5161.77, 148.22, 2.58847, 300, 0, 0, 4979, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+613, 66305, 1098, 0, 0, 248, 1, 0, 0, 6050.36, 5115.4, 153.698, 4.79206, 300, 0, 0, 4979, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+614, 66305, 1098, 0, 0, 248, 1, 0, 0, 6065.62, 5103.77, 153.908, 4.50539, 300, 0, 0, 4979, 0, 0, 0, 0, 0, 0, 0, 0, NULL);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Spell Scripts

REPLACE INTO spell_script_names VALUES
(139853, 'spell_lei_shen_tortos_bridge_call_lightning'),
(139852, 'spell_teleport_all'),
(136294, 'spell_call_of_tortos'),
(134364, 'spell_rockfall_trigger_tortos'),
(140431, 'spell_rockfall_trigger_tortos'),
(134476, 'spell_rockfall_aoe_damage'),
(134539, 'spell_rockfall_damage'),
(140443, 'spell_spinning_shell'),
(136431, 'spell_shell_concussion'),
(135103, 'spell_drain_the_weak'),
(137633, 'spell_crystal_shell_aura'),
(134920, 'spell_tortos_quake_stomp'),
(133939, 'spell_furious_stone_breath'),
(139158, 'spell_waterspout_aura'),
(135101, 'spell_drain_the_weak_damage');

REPLACE INTO spell_target_position VALUES
(139165, 0, 1098, 6064.06, 5210.70, 148.12, 1.57);

/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Speeches
DELETE FROM `creature_text` WHERE `entry` IN (67977, 70437);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(67977, 0, 0, '%s lets out a booming call, attracting nearby turtles.', 41, 0, 100, 0, 0, 0, 'Tortos'),
(67977, 1, 0, '%s prepares to unleash a |cFFFF0000|Hspell:133939|h[Furious Stone Breath]|h|r!', 41, 0, 100, 0, 0, 0, 'Tortos'),

(70437, 0, 0, 'You have swept the filth from my doorstep. Perhaps, you are worthy of my attention.', 14, 0, 100, 0, 0, 35587, 'Lei shen event 1'),
(70437, 1, 0, 'But...your trespass ends here. None may enter my forbidden stronghold. I shall rebuild this bridge with your bones for bricks.', 14, 0, 100, 0, 0, 35588, 'Lei Shen event 2');
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Gameobjects templates
REPLACE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `AIName`, `ScriptName`, `WDBVerified`) VALUES 
(218869, 33, 13669, 'Lightning Span', '', '', '', 1375, 32, 1, 0, 0, 0, 0, 0, 0, 0, 0, 88, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35752, 0, 0, 0, 161, 0, 0, 0, 0, 0, '', '', 17658),
(218980, 0, 13603, 'Doodad_Thunderking_TurtleDoor001', '', '', '', 1375, 32, 1.4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218987, 0, 13716, 'Tortos Death Collision', '', '', '', 1375, 48, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 1);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Gameobjects spawns
INSERT INTO `gameobject` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
(@OGUID+27, 218980, 1098, 0, 0, 760, 1, 6041.98, 4917.88, -61.1905, 1.53089, 0, 0, 1, -4.37114E-8, 86400, 255, 1, 0, NULL),
(@OGUID+28, 218987, 1098, 0, 0, 760, 1, 6038.69, 4923.87, -61.1953, 1.51382, 0, 0, 0, 1, 86400, 255, 0, 0, NULL),
(@OGUID+29, 218869, 1098, 0, 0, 760, 1, 6046, 5100.75, 72.8265, 3.14159, 2.25609E-43, 0, 0, 0, 86400, 255, 1, 0, NULL);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Tortos Conditions
DELETE FROM conditions WHERE sourceentry IN (134091, 136431, 136294);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
('13','1','134091','0','0','31','0','3','67977','0','0','0','','Tortos'),
('13','1','136431','0','0','31','0','3','67977','0','0','0','','Tortos'),
('13','1','136294','0','0','31','0','3','67977','0','0','0','','Tortos');
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Area Trigger
DELETE FROM areatrigger_scripts WHERE entry = 8937;
INSERT INTO areatrigger_scripts VALUES
(8937,'at_tortos_intro');

DELETE FROM spell_areatrigger_template WHERE Entry IN (832, 575);
INSERT INTO spell_areatrigger_template (Entry, Flags, CollisionType, Radius, ScaleX, ScaleY, ScriptName) VALUES
(832, 0, 0, 8.0, 8.0, 8.0, 'sat_waterspout'),
(575, 0, 0, 5.0, 5.0, 5.0, 'sat_shell_spin');
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Template Auras
DELETE FROM creature_aura WHERE guid BETWEEN (@CGUID+467) AND (@CGUID+476);
DELETE FROM creature_aura WHERE guid = @CGUID+618;
INSERT INTO creature_aura VALUES
(@CGUID+467, 140809),
(@CGUID+468, 140809),
(@CGUID+469, 140809),
(@CGUID+470, 140809),
(@CGUID+471, 140809),
(@CGUID+472, 140809),
(@CGUID+473, 140809),
(@CGUID+474, 140809),
(@CGUID+475, 140809),
(@CGUID+476, 140809),

(@CGUID+618, 139149),
(@CGUID+618, 139159);
/*---------------------------------------------------------------------------------------------------------------*/
/* END Tortos */





/* BEGIN Megaera */
/*---------------------------------------------------------------------------------------------------------------*/
-- Spell Scripts
UPDATE creature_template SET ScriptName = "boss_megaera" WHERE entry = 68065;
UPDATE creature_template SET ScriptName = "npc_flaming_head_megaera" WHERE entry = 70212;
UPDATE creature_template SET ScriptName = "npc_frozen_head_megaera" WHERE entry = 70235;
UPDATE creature_template SET ScriptName = "npc_venomous_head_megaera" WHERE entry = 70247;
UPDATE creature_template SET ScriptName = "npc_arcane_head_megaera" WHERE entry = 70252;
UPDATE creature_template SET modelid1 = 11686, modelid2 = 0 WHERE entry = 68553;

REPLACE INTO spell_script_names VALUES
(137973, 'spell_concealing_fog_megaera'),
(139548, 'spell_rampage_flaming_head_megaera'),
(139549, 'spell_rampage_frozen_head_megaera'),
(139551, 'spell_rampage_venomous_head_megaera'),
(139552, 'spell_rampage_arcane_head_megaera'),
(139822, 'spell_cinders_megaera'),
(139843, 'spell_arctic_freeze_megaera'),
(134343, 'spell_acid_glob_megaera'),
(139877, 'spell_icy_ground_dummy'),
(139832, 'spell_megaera_submerged'),
(139845, 'spell_acid_rain_summon'),
(139848, 'spell_acid_rain_missile'),
(139850, 'spell_acid_rain_damage'),
(140630, 'spell_siphon_life_tot');

DELETE FROM spell_areatrigger_template WHERE Entry IN (853);
INSERT INTO spell_areatrigger_template (Entry, Flags, CollisionType, Radius, ScaleX, ScaleY, ScriptName) VALUES
(853, 0, 0, 9, 9, 9, 'sat_icy_ground');

/*---------------------------------------------------------------------------------------------------------------*/

-- Megaera Conditions
DELETE FROM conditions WHERE SourceEntry IN (139877, 139848);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
('13','1','139877','0','0','31','0','3','70432','0','0','0','','Icy Ground - Remove Cinders'),
('13','1','139848','0','0','31','0','3','70435','0','0','0','','Acid Rain Missle - Target Acid Rain Dummy');

/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Templates
REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES 
(68065, 0, 0, 0, 0, 0, 48113, 0, 0, 0, 'Megaera', 'Terror of the Depths', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 76, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 7, 1, 603.75, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 'boss_megaera', 17614),
(70153, 0, 0, 3170153, 0, 0, 28133, 0, 0, 0, 'Fungal Growth', '', '', 0, 92, 92, 4, 16, 16, 0, 2.4, 1.714286, 1, 1, 30000, 55000, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 70153, 0, 0, 0, 0, 0, 0, 0, 0, 140620, 140626, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(70157, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Geyser', '', '', 0, 91, 91, 0, 35, 35, 0, 1, 1.14286, 1, 0, 22000, 28000, 0, 42000, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 128, '', 1),
(70175, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Front Head Spawner', '', '', 0, 90, 90, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 17826816, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614),
(70212, 0, 0, 0, 0, 0, 47414, 48240, 48241, 0, 'Flaming Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 37839, 51339, 0, 31299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 86.25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_flaming_head_megaera', 17614),
(70235, 0, 0, 0, 0, 0, 48242, 47415, 48243, 0, 'Frozen Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 37839, 51339, 0, 31299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 86.25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_frozen_head_megaera', 17614),
(70247, 0, 0, 0, 0, 0, 48245, 48244, 47416, 0, 'Venomous Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 37839, 51339, 0, 31299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 86.25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_venomous_head_megaera', 17614),
(70252, 0, 0, 0, 0, 0, 48238, 47417, 48239, 0, 'Arcane Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 41839, 59339, 0, 35299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 90, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_arcane_head_megaera', 17614),
(70439, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Torrent of Ice', '', '', 0, 90, 90, 4, 16, 16, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 98, 1, 0, 0, 'npc_torrent_of_ice', 17614),
(70545, 0, 0, 0, 0, 0, 48183, 48184, 48185, 0, 'Mysterious Mushroom', '', '', 0, 91, 91, 4, 16, 16, 0, 0.1, 0.1, 1, 1, 0, 0, 0, 0, 1, 2000, 2000, 1, 33685508, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1076887624, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614),
(70586, 0, 0, 3170586, 0, 0, 45735, 0, 0, 0, 'Eternal Guardian', '', '', 0, 92, 92, 4, 14, 14, 0, 4.8, 1.714286, 1, 1, 30000, 55000, 0, 45000, 1, 2000, 2000, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 70586, 0, 0, 0, 0, 0, 0, 0, 0, 140629, 140628, 140630, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 42, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(70587, 0, 0, 3170587, 0, 0, 48160, 0, 0, 0, 'Shale Stalker', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.714286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 8, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614),
(70589, 0, 0, 0, 0, 0, 48177, 0, 0, 0, 'Cavern Burrower', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 45000, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 70589, 0, 0, 0, 0, 0, 0, 0, 0, 140618, 140619, 140600, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(70594, 0, 0, 0, 0, 0, 44690, 0, 0, 0, 'Mist Lurker', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 45000, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 70594, 0, 0, 0, 0, 0, 0, 0, 0, 140682, 140684, 140686, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(70446, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Icy Ground', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 17826816, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 'npc_icy_ground_megaera', 17614),
(70432, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Cinders', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 17826816, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 'npc_cinders_megaera', 17614),
(70435, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Acid Rain', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 33685508, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 17826816, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 17614),

(@RAID_DIFF_10N + 70153, 0, 0, 3170153, 0, 0, 28133, 0, 0, 0, 'Fungal Growth', '', '', 0, 92, 92, 4, 16, 16, 0, 2.4, 1.714286, 1, 1, 30000, 55000, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 70153, 0, 0, 0, 0, 0, 0, 0, 0, 140620, 140626, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(@RAID_DIFF_10N + 70212, 0, 0, 0, 0, 0, 47414, 48240, 48241, 0, 'Flaming Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 37839, 51339, 0, 31299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 86.25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_flaming_head_megaera', 17614),
(@RAID_DIFF_10N + 70235, 0, 0, 0, 0, 0, 48242, 47415, 48243, 0, 'Frozen Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 37839, 51339, 0, 31299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 86.25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_frozen_head_megaera', 17614),
(@RAID_DIFF_10N + 70247, 0, 0, 0, 0, 0, 48245, 48244, 47416, 0, 'Venomous Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 37839, 51339, 0, 31299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 86.25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_venomous_head_megaera', 17614),
(@RAID_DIFF_10N + 70252, 0, 0, 0, 0, 0, 48238, 47417, 48239, 0, 'Arcane Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 41839, 59339, 0, 35299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 90, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_arcane_head_megaera', 17614),
(@RAID_DIFF_10N + 70586, 0, 0, 3170586, 0, 0, 45735, 0, 0, 0, 'Eternal Guardian', '', '', 0, 92, 92, 4, 14, 14, 0, 4.8, 1.714286, 1, 1, 30000, 55000, 0, 45000, 1, 2000, 2000, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 70586, 0, 0, 0, 0, 0, 0, 0, 0, 140629, 140628, 140630, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 42, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(@RAID_DIFF_10N + 70587, 0, 0, 3170587, 0, 0, 48160, 0, 0, 0, 'Shale Stalker', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.714286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 8, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614),
(@RAID_DIFF_10N + 70589, 0, 0, 0, 0, 0, 48177, 0, 0, 0, 'Cavern Burrower', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 45000, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 70589, 0, 0, 0, 0, 0, 0, 0, 0, 140618, 140619, 140600, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(@RAID_DIFF_10N + 70594, 0, 0, 0, 0, 0, 44690, 0, 0, 0, 'Mist Lurker', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 45000, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 70594, 0, 0, 0, 0, 0, 0, 0, 0, 140682, 140684, 140686, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),

(@RAID_DIFF_25N + 70153, 0, 0, 3170153, 0, 0, 28133, 0, 0, 0, 'Fungal Growth', '', '', 0, 92, 92, 4, 16, 16, 0, 2.4, 1.714286, 1, 1, 30000, 55000, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 70153, 0, 0, 0, 0, 0, 0, 0, 0, 140620, 140626, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(@RAID_DIFF_25N + 70212, 0, 0, 0, 0, 0, 47414, 48240, 48241, 0, 'Flaming Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 37839, 51339, 0, 31299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 86.25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_flaming_head_megaera', 17614),
(@RAID_DIFF_25N + 70235, 0, 0, 0, 0, 0, 48242, 47415, 48243, 0, 'Frozen Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 37839, 51339, 0, 31299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 86.25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_frozen_head_megaera', 17614),
(@RAID_DIFF_25N + 70247, 0, 0, 0, 0, 0, 48245, 48244, 47416, 0, 'Venomous Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 37839, 51339, 0, 31299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 86.25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_venomous_head_megaera', 17614),
(@RAID_DIFF_25N + 70252, 0, 0, 0, 0, 0, 48238, 47417, 48239, 0, 'Arcane Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 41839, 59339, 0, 35299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 90, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_arcane_head_megaera', 17614),
(@RAID_DIFF_25N + 70586, 0, 0, 3170586, 0, 0, 45735, 0, 0, 0, 'Eternal Guardian', '', '', 0, 92, 92, 4, 14, 14, 0, 4.8, 1.714286, 1, 1, 30000, 55000, 0, 45000, 1, 2000, 2000, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 70586, 0, 0, 0, 0, 0, 0, 0, 0, 140629, 140628, 140630, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 42, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(@RAID_DIFF_25N + 70587, 0, 0, 3170587, 0, 0, 48160, 0, 0, 0, 'Shale Stalker', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.714286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 8, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614),
(@RAID_DIFF_25N + 70589, 0, 0, 0, 0, 0, 48177, 0, 0, 0, 'Cavern Burrower', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 45000, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 70589, 0, 0, 0, 0, 0, 0, 0, 0, 140618, 140619, 140600, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(@RAID_DIFF_25N + 70594, 0, 0, 0, 0, 0, 44690, 0, 0, 0, 'Mist Lurker', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 45000, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 70594, 0, 0, 0, 0, 0, 0, 0, 0, 140682, 140684, 140686, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),

(@RAID_DIFF_10H + 70153, 0, 0, 3170153, 0, 0, 28133, 0, 0, 0, 'Fungal Growth', '', '', 0, 92, 92, 4, 16, 16, 0, 2.4, 1.714286, 1, 1, 30000, 55000, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 70153, 0, 0, 0, 0, 0, 0, 0, 0, 140620, 140626, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(@RAID_DIFF_10H + 70212, 0, 0, 0, 0, 0, 47414, 48240, 48241, 0, 'Flaming Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 37839, 51339, 0, 31299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 86.25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_flaming_head_megaera', 17614),
(@RAID_DIFF_10H + 70235, 0, 0, 0, 0, 0, 48242, 47415, 48243, 0, 'Frozen Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 37839, 51339, 0, 31299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 86.25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_frozen_head_megaera', 17614),
(@RAID_DIFF_10H + 70247, 0, 0, 0, 0, 0, 48245, 48244, 47416, 0, 'Venomous Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 37839, 51339, 0, 31299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 86.25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_venomous_head_megaera', 17614),
(@RAID_DIFF_10H + 70252, 0, 0, 0, 0, 0, 48238, 47417, 48239, 0, 'Arcane Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 41839, 59339, 0, 35299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 90, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_arcane_head_megaera', 17614),
(@RAID_DIFF_10H + 70586, 0, 0, 3170586, 0, 0, 45735, 0, 0, 0, 'Eternal Guardian', '', '', 0, 92, 92, 4, 14, 14, 0, 4.8, 1.714286, 1, 1, 30000, 55000, 0, 45000, 1, 2000, 2000, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 70586, 0, 0, 0, 0, 0, 0, 0, 0, 140629, 140628, 140630, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 42, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(@RAID_DIFF_10H + 70587, 0, 0, 3170587, 0, 0, 48160, 0, 0, 0, 'Shale Stalker', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.714286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 8, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614),
(@RAID_DIFF_10H + 70589, 0, 0, 0, 0, 0, 48177, 0, 0, 0, 'Cavern Burrower', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 45000, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 70589, 0, 0, 0, 0, 0, 0, 0, 0, 140618, 140619, 140600, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(@RAID_DIFF_10H + 70594, 0, 0, 0, 0, 0, 44690, 0, 0, 0, 'Mist Lurker', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 45000, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 70594, 0, 0, 0, 0, 0, 0, 0, 0, 140682, 140684, 140686, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),

(@RAID_DIFF_25H + 70153, 0, 0, 3170153, 0, 0, 28133, 0, 0, 0, 'Fungal Growth', '', '', 0, 92, 92, 4, 16, 16, 0, 2.4, 1.714286, 1, 1, 30000, 55000, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 70153, 0, 0, 0, 0, 0, 0, 0, 0, 140620, 140626, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(@RAID_DIFF_25H + 70212, 0, 0, 0, 0, 0, 47414, 48240, 48241, 0, 'Flaming Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 37839, 51339, 0, 31299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 86.25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_flaming_head_megaera', 17614),
(@RAID_DIFF_25H + 70235, 0, 0, 0, 0, 0, 48242, 47415, 48243, 0, 'Frozen Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 37839, 51339, 0, 31299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 86.25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_frozen_head_megaera', 17614),
(@RAID_DIFF_25H + 70247, 0, 0, 0, 0, 0, 48245, 48244, 47416, 0, 'Venomous Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 37839, 51339, 0, 31299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 86.25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_venomous_head_megaera', 17614),
(@RAID_DIFF_25H + 70252, 0, 0, 0, 0, 0, 48238, 47417, 48239, 0, 'Arcane Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 41839, 59339, 0, 35299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 90, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_arcane_head_megaera', 17614),
(@RAID_DIFF_25H + 70586, 0, 0, 3170586, 0, 0, 45735, 0, 0, 0, 'Eternal Guardian', '', '', 0, 92, 92, 4, 14, 14, 0, 4.8, 1.714286, 1, 1, 30000, 55000, 0, 45000, 1, 2000, 2000, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 70586, 0, 0, 0, 0, 0, 0, 0, 0, 140629, 140628, 140630, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 42, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(@RAID_DIFF_25H + 70587, 0, 0, 3170587, 0, 0, 48160, 0, 0, 0, 'Shale Stalker', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.714286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 8, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614),
(@RAID_DIFF_25H + 70589, 0, 0, 0, 0, 0, 48177, 0, 0, 0, 'Cavern Burrower', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 45000, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 70589, 0, 0, 0, 0, 0, 0, 0, 0, 140618, 140619, 140600, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(@RAID_DIFF_25H + 70594, 0, 0, 0, 0, 0, 44690, 0, 0, 0, 'Mist Lurker', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 45000, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 70594, 0, 0, 0, 0, 0, 0, 0, 0, 140682, 140684, 140686, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),

(@RAID_DIFF_25R + 70153, 0, 0, 3170153, 0, 0, 28133, 0, 0, 0, 'Fungal Growth', '', '', 0, 92, 92, 4, 16, 16, 0, 2.4, 1.714286, 1, 1, 30000, 55000, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 70153, 0, 0, 0, 0, 0, 0, 0, 0, 140620, 140626, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(@RAID_DIFF_25R + 70212, 0, 0, 0, 0, 0, 47414, 48240, 48241, 0, 'Flaming Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 37839, 51339, 0, 31299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 86.25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_flaming_head_megaera', 17614),
(@RAID_DIFF_25R + 70235, 0, 0, 0, 0, 0, 48242, 47415, 48243, 0, 'Frozen Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 37839, 51339, 0, 31299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 86.25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_frozen_head_megaera', 17614),
(@RAID_DIFF_25R + 70247, 0, 0, 0, 0, 0, 48245, 48244, 47416, 0, 'Venomous Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 37839, 51339, 0, 31299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 86.25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_venomous_head_megaera', 17614),
(@RAID_DIFF_25R + 70252, 0, 0, 0, 0, 0, 48238, 47417, 48239, 0, 'Arcane Head', 'Head of Megaera', '', 0, 93, 93, 4, 16, 16, 0, 1, 1.14286, 1, 3, 41839, 59339, 0, 35299, 2, 2000, 2000, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 6, 1, 90, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 752828415, 0, 'npc_arcane_head_megaera', 17614),
(@RAID_DIFF_25R + 70586, 0, 0, 3170586, 0, 0, 45735, 0, 0, 0, 'Eternal Guardian', '', '', 0, 92, 92, 4, 14, 14, 0, 4.8, 1.714286, 1, 1, 30000, 55000, 0, 45000, 1, 2000, 2000, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 70586, 0, 0, 0, 0, 0, 0, 0, 0, 140629, 140628, 140630, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 42, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(@RAID_DIFF_25R + 70587, 0, 0, 3170587, 0, 0, 48160, 0, 0, 0, 'Shale Stalker', '', '', 0, 92, 92, 4, 16, 16, 0, 4.8, 1.714286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 8, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, '', 17614),
(@RAID_DIFF_25R + 70589, 0, 0, 0, 0, 0, 48177, 0, 0, 0, 'Cavern Burrower', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 45000, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 70589, 0, 0, 0, 0, 0, 0, 0, 0, 140618, 140619, 140600, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1),
(@RAID_DIFF_25R + 70594, 0, 0, 0, 0, 0, 44690, 0, 0, 0, 'Mist Lurker', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 30000, 55000, 0, 45000, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 70594, 0, 0, 0, 0, 0, 0, 0, 0, 140682, 140684, 140686, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 1);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Updates
-- Creature Trash Mobs Damage, HP, LootId
-- Base HP Gamepedia Raid 25N
-- 25R -10%
-- 25H +25%
-- 10N (BASE HP: 25 = X : 10)
-- 10H=(10N+25%)

-- Immunity and bind bosses
UPDATE creature_template SET flags_extra = flags_extra | 1, mechanic_immune_mask = 667893759, InhabitType = 7 WHERE entry IN 
(70212,3170212,3270212,3370212,3470212,3570212,70235,3170235,3270235,3370235,3470235,3570235,70247,3170247,3270247,3370247,3470247,3570247,70252,3170252,3270252,3370252,3470252,3570252);

-- Difficulty entries bosses and Trash Mobs
UPDATE creature_template SET difficulty_entry_3 = @RAID_DIFF_10N + entry, difficulty_entry_4 = @RAID_DIFF_25N + entry, difficulty_entry_5 = @RAID_DIFF_10H + entry, difficulty_entry_6 = @RAID_DIFF_25H + entry, difficulty_entry_7 = @RAID_DIFF_25R + entry WHERE entry IN 
(70153,70212,70235,70247,70252,70586,70587,70589,70594);

-- Fungal Growth
UPDATE creature_template SET LootId = 69388,   dmg_multiplier = 5, Health_Mod = 14755720 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 70153;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 6, Health_Mod = 18444650 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 70153;
UPDATE creature_template SET LootId = 69388,   dmg_multiplier = 8, Health_Mod = 36889300 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 70153;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 9, Health_Mod = 46111625 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 70153;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 7, Health_Mod = 33200370 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 70153;

-- Eternal Guardian
UPDATE creature_template SET LootId = 69388,   dmg_multiplier = 6, Health_Mod = 17706885 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 70586;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 7, Health_Mod = 22133606 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 70586;
UPDATE creature_template SET LootId = 69388,   dmg_multiplier = 9, Health_Mod = 44267212 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 70586;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 10,Health_Mod = 55334015 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 70586;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 8, Health_Mod = 39840491 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 70586;

-- Shale Stalker
UPDATE creature_template SET LootId = 69388,   dmg_multiplier = 5, Health_Mod = 2023641  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 70587;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 6, Health_Mod = 2529552  / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 70587;
UPDATE creature_template SET LootId = 69388,   dmg_multiplier = 8, Health_Mod = 5059104  / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 70587;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 9, Health_Mod = 6323880  / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 70587;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 7, Health_Mod = 4553193  / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 70587;

-- Cavern Burrower
UPDATE creature_template SET LootId = 69388,   dmg_multiplier = 5, Health_Mod = 14755720 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 70589;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 6, Health_Mod = 18444650 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 70589;
UPDATE creature_template SET LootId = 69388,   dmg_multiplier = 8, Health_Mod = 36889300 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 70589;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 9, Health_Mod = 46111625 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 70589;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 7, Health_Mod = 33200370 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 70589;

-- Mist Lurker
UPDATE creature_template SET LootId = 69388,   dmg_multiplier = 5, Health_Mod = 14755720 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10N + 70594;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 6, Health_Mod = 18444650 / @HP_MOD_92 WHERE entry = @RAID_DIFF_10H + 70594;
UPDATE creature_template SET LootId = 69388,   dmg_multiplier = 8, Health_Mod = 36889300 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25N + 70594;
UPDATE creature_template SET LootId = 6938801, dmg_multiplier = 9, Health_Mod = 46111625 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25H + 70594;
UPDATE creature_template SET LootId = 6938802, dmg_multiplier = 7, Health_Mod = 33200370 / @HP_MOD_92 WHERE entry = @RAID_DIFF_25R + 70594;

-- Megaera
-- Flaming Head
UPDATE creature_template SET LootId = 0, dmg_multiplier = 10, Health_Mod = 37616816   / @HP_MOD_93 WHERE entry = @RAID_DIFF_10N + 70212;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 11, Health_Mod = 48899680   / @HP_MOD_93 WHERE entry = @RAID_DIFF_10H + 70212;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 13, Health_Mod = 112850448 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25N + 70212;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 14, Health_Mod = 146703408 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25H + 70212;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 12, Health_Mod = 78993128   / @HP_MOD_93 WHERE entry = @RAID_DIFF_25R + 70212;

-- Frozen Head
UPDATE creature_template SET LootId = 0, dmg_multiplier = 10, Health_Mod = 37616816   / @HP_MOD_93 WHERE entry = @RAID_DIFF_10N + 70235;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 11, Health_Mod = 48899680   / @HP_MOD_93 WHERE entry = @RAID_DIFF_10H + 70235;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 13, Health_Mod = 112850448 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25N + 70235;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 14, Health_Mod = 146703408 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25H + 70235;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 12, Health_Mod = 78993128   / @HP_MOD_93 WHERE entry = @RAID_DIFF_25R + 70235;

-- Venomous Head
UPDATE creature_template SET LootId = 0, dmg_multiplier = 10, Health_Mod = 37616816  / @HP_MOD_93 WHERE entry = @RAID_DIFF_10N + 70247;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 11, Health_Mod = 48899680  / @HP_MOD_93 WHERE entry = @RAID_DIFF_10H + 70247;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 13, Health_Mod = 112850448 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25N + 70247;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 14, Health_Mod = 146703408 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25H + 70247;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 12, Health_Mod = 78993128   / @HP_MOD_93 WHERE entry = @RAID_DIFF_25R + 70247;

-- Arcane Head
UPDATE creature_template SET LootId = 0, dmg_multiplier = 10, Health_Mod = 37616816  / @HP_MOD_93 WHERE entry = @RAID_DIFF_10N + 70252;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 11, Health_Mod = 48899680  / @HP_MOD_93 WHERE entry = @RAID_DIFF_10H + 70252;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 13, Health_Mod = 112850448 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25N + 70252;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 14, Health_Mod = 146703408 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25H + 70252;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 12, Health_Mod = 78993128   / @HP_MOD_93 WHERE entry = @RAID_DIFF_25R + 70252;
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Trash Mobs SAI
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (70153,70545,70586,70587,70589,70594);
DELETE FROM `smart_scripts` WHERE `entryorguid`IN (70153,70545,70586,70587,70589,70594) AND `source_type`= 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(70153, 0, 0, 0, 0, 0, 100, 0, 10000, 12000, 20000, 21000, 11, 140620, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, "Fungal Growth - Cast Fungi Spores on a random target"),
(70153, 0, 1, 0, 2, 0, 100, 1, 0, 60, 0, 0, 11, 140626, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Fungal Growth - Cast Grow"),

(70545, 0, 0, 1, 10, 0, 100, 1, 0, 3, 0, 0, 11, 140598, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Mysterious Mushroom - Cast Fungal Explosion"),
(70545, 0, 1, 0, 61, 0, 100, 1, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Mysterious Mushroom - Despawn after the explosion"),
(70545, 0, 2, 0, 1, 0, 100, 1, 1, 1, 0, 0, 11, 123978, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Mysterious Mushroom - Mushrooms different sizes"),

(70586, 0, 0, 0, 1, 0, 100, 1, 1, 1, 0, 0, 11, 141923, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Eternal Guardian - Meditate OOC"),
(70586, 0, 1, 0, 0, 0, 100, 1, 1, 1, 0, 0, 28, 141923, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Eternal Guardian - Remove Meditate IC"),
(70586, 0, 2, 0, 0, 0, 100, 0, 10000, 11000, 10000, 11000, 11, 140629, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, "Eternal Guardian - Cast Eternal Prison"),
(70586, 0, 3, 0, 0, 0, 100, 0, 14000, 15000, 25000, 26000, 11, 140628, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Eternal Guardian - Cast Lightning Nova"),
(70586, 0, 4, 0, 0, 0, 100, 0, 6000, 8000, 15000, 18000, 11, 140630, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Eternal Guardian - Cast Siphon Life"),

(70587, 0, 0, 0, 0, 0, 80, 0, 4000, 8000, 12000, 13000, 11, 140616, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Shale Stalker - Cast Shale Shards"),
(70587, 0, 1, 0, 8, 0, 100, 0, 140600, 0, 1000, 1000, 38, 0, 0, 0, 0, 0, 0, 17, 0, 45, 0, 0, 0, 0, 0, "Shale Stalker - Enter in combat when called"),

(70589, 0, 0, 0, 1, 0, 100, 1, 1, 1, 0, 0, 75, 140586, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Cavern Burrower - Submerge"),
(70589, 0, 1, 0, 0, 0, 100, 1, 1, 1, 0, 0, 28, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Cavern Burrower - Removeaura IC"),
(70589, 0, 2, 0, 0, 0, 100, 0, 5000, 8000, 12000, 18000, 11, 140618, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Cavern Burrower - Cast Crush Armor"),
(70589, 0, 3, 0, 0, 0, 100, 0, 2000, 3000, 20000, 21000, 11, 140600, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Cavern Burrower - Cast Sonic Call"),
(70589, 0, 4, 0, 0, 0, 100, 0, 5000, 8000, 5000, 6000, 11, 140619, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Cavern Burrower - Cast Crystal Barbs"),

(70594, 0, 0, 0, 0, 0, 100, 0, 5000, 7000, 5000, 8000, 11, 140684, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Mist Lurker - Cast Corrosive Breath"),
(70594, 0, 1, 0, 0, 0, 100, 0, 3000, 4000, 15000, 18000, 11, 140682, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, "Mist Lurker - Cast Choking Mists");
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Megaera Loots
DELETE FROM `gameobject_loot_template` WHERE `entry` IN (218805,218806,218807,218808);
INSERT INTO `gameobject_loot_template` VALUES
-- 10 Normal
(218805, 94801, 0, 1, 1, 1, 1),
(218805, 94800, 0, 1, 1, 1, 1),
(218805, 94797, 0, 1, 1, 1, 1),
(218805, 94789, 0, 1, 1, 1, 1),
(218805, 94804, 0, 1, 1, 1, 1),
(218805, 94791, 0, 1, 1, 1, 1),
(218805, 94790, 0, 1, 1, 1, 1),
(218805, 94792, 0, 1, 1, 1, 1),
(218805, 94799, 0, 1, 1, 1, 1),
(218805, 94802, 0, 1, 1, 1, 1),
(218805, 94798, 0, 1, 1, 1, 1),
(218805, 94793, 0, 1, 2, 1, 1),
(218805, 94803, 0, 1, 2, 1, 1),
(218805, 94794, 0, 1, 2, 1, 1),
(218805, 94795, 0, 1, 2, 1, 1),
(218805, 94521, 0, 1, 2, 1, 1),
(218805, 94520, 0, 1, 2, 1, 1),
(218805, 94788, 0, 1, 2, 1, 1),
(218805, 94796, 0, 1, 2, 1, 1),
(218805, 94593, 1, 1, 3, 1, 1),
(218805, 94594, 1, 1, 3, 1, 1),
(218805, 87208, 1, 1, 3, 1, 1),
(218805, 87209, 1, 1, 3, 1, 1),
(218805, 95498, 5, 1, 10, -95498, 1),
-- 10 Heroic
(218806, 96451, 0, 1, 1, 1, 1),
(218806, 96457, 0, 1, 1, 1, 1),
(218806, 96459, 0, 1, 1, 1, 1),
(218806, 96447, 0, 1, 1, 1, 1),
(218806, 96452, 0, 1, 1, 1, 1),
(218806, 96444, 0, 1, 1, 1, 1),
(218806, 96445, 0, 1, 1, 1, 1),
(218806, 96446, 0, 1, 1, 1, 1),
(218806, 96450, 0, 1, 1, 1, 1),
(218806, 96458, 0, 1, 1, 1, 1),
(218806, 96460, 0, 1, 1, 1, 1),
(218806, 96449, 0, 1, 2, 1, 1),
(218806, 96453, 0, 1, 2, 1, 1),
(218806, 96443, 0, 1, 2, 1, 1),
(218806, 96448, 0, 1, 2, 1, 1),
(218806, 96455, 0, 1, 2, 1, 1),
(218806, 96456, 0, 1, 2, 1, 1),
(218806, 96442, 0, 1, 2, 1, 1),
(218806, 96454, 0, 1, 2, 1, 1),
(218806, 94593, 1, 1, 3, 1, 1),
(218806, 94594, 1, 1, 3, 1, 1),
(218806, 87208, 1, 1, 3, 1, 1),
(218806, 87209, 1, 1, 3, 1, 1),
(218806, 96621, 5, 1, 10, -96621, 1),
-- 25 Normal
(218807, 94801, 0, 1, 1, 1, 1),
(218807, 94800, 0, 1, 1, 1, 1),
(218807, 94797, 0, 1, 1, 1, 1),
(218807, 94789, 0, 1, 1, 1, 1),
(218807, 94804, 0, 1, 2, 1, 1),
(218807, 94791, 0, 1, 2, 1, 1),
(218807, 94790, 0, 1, 2, 1, 1),
(218807, 94792, 0, 1, 2, 1, 1),
(218807, 94799, 0, 1, 3, 1, 1),
(218807, 94802, 0, 1, 3, 1, 1),
(218807, 94798, 0, 1, 3, 1, 1),
(218807, 94793, 0, 1, 4, 1, 1),
(218807, 94803, 0, 1, 4, 1, 1),
(218807, 94794, 0, 1, 4, 1, 1),
(218807, 94795, 0, 1, 4, 1, 1),
(218807, 94521, 0, 1, 5, 1, 1),
(218807, 94520, 0, 1, 5, 1, 1),
(218807, 94788, 0, 1, 5, 1, 1),
(218807, 94796, 0, 1, 5, 1, 1),
(218807, 94593, 1, 1, 6, 1, 1),
(218807, 94594, 1, 1, 6, 1, 1),
(218807, 87208, 1, 1, 6, 1, 1),
(218807, 87209, 1, 1, 6, 1, 1),
(218807, 95498, 5, 1, 10, -95498, 1),
-- 25 Heroic
(218808, 96451, 0, 1, 1, 1, 1),
(218808, 96457, 0, 1, 1, 1, 1),
(218808, 96459, 0, 1, 1, 1, 1),
(218808, 96447, 0, 1, 1, 1, 1),
(218808, 96452, 0, 1, 2, 1, 1),
(218808, 96444, 0, 1, 2, 1, 1),
(218808, 96445, 0, 1, 2, 1, 1),
(218808, 96446, 0, 1, 2, 1, 1),
(218808, 96450, 0, 1, 3, 1, 1),
(218808, 96458, 0, 1, 3, 1, 1),
(218808, 96460, 0, 1, 3, 1, 1),
(218808, 96449, 0, 1, 4, 1, 1),
(218808, 96453, 0, 1, 4, 1, 1),
(218808, 96443, 0, 1, 4, 1, 1),
(218808, 96448, 0, 1, 4, 1, 1),
(218808, 96455, 0, 1, 5, 1, 1),
(218808, 96456, 0, 1, 5, 1, 1),
(218808, 96442, 0, 1, 5, 1, 1),
(218808, 96454, 0, 1, 5, 1, 1),
(218808, 94593, 1, 1, 6, 1, 1),
(218808, 94594, 1, 1, 6, 1, 1),
(218808, 87208, 1, 1, 6, 1, 1),
(218808, 87209, 1, 1, 6, 1, 1),
(218808, 96621, 5, 1, 10, -96621, 1);
/*----------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Spawn
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`npcflag2`,`unit_flags`,`unit_flags2`,`dynamicflags`,`isActive`,`protec_anti_doublet`) VALUES
(@CGUID+279, 70545, 1098, 0, 0, 248, 1, 0, 0, 6161.37, 4913.38, -78.3878, 2.44666, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+280, 70545, 1098, 0, 0, 248, 1, 0, 0, 6162.31, 4910.25, -79.3836, 4.04951, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+281, 70545, 1098, 0, 0, 248, 1, 0, 0, 6165.33, 4915.72, -79.8245, 1.4967, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+282, 70545, 1098, 0, 0, 248, 1, 0, 0, 6286.69, 4854.03, -157.987, 4.23861, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+283, 70545, 1098, 0, 0, 248, 1, 0, 0, 6317.84, 4913.66, -161.491, 0.0703596, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+284, 70545, 1098, 0, 0, 248, 1, 0, 0, 6284.18, 4845.76, -156.109, 3.84788, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+285, 70545, 1098, 0, 0, 248, 1, 0, 0, 6281.93, 4850.9, -155.865, 2.08196, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+286, 70545, 1098, 0, 0, 248, 1, 0, 0, 6285.2, 4842, -154.609, 5.08546, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+287, 70545, 1098, 0, 0, 248, 1, 0, 0, 6326.67, 4912.05, -161.746, 3.37654, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+288, 70545, 1098, 0, 0, 248, 1, 0, 0, 6305.88, 4914.73, -161.321, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+289, 70545, 1098, 0, 0, 248, 1, 0, 0, 6165.91, 4922.54, -79.1794, 5.97328, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+290, 70545, 1098, 0, 0, 248, 1, 0, 0, 6342.72, 4933.03, -161.68, 4.47334, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+291, 70545, 1098, 0, 0, 248, 1, 0, 0, 6317.04, 4921.19, -161.408, 3.12337, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+292, 70545, 1098, 0, 0, 248, 1, 0, 0, 6295.41, 4835.06, -155.721, 3.14253, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+293, 70545, 1098, 0, 0, 248, 1, 0, 0, 6370.61, 4903.09, -163.751, 0.000421845, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+294, 70545, 1098, 0, 0, 248, 1, 0, 0, 6324.6, 4875.99, -162.666, 0.408799, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+295, 70545, 1098, 0, 0, 248, 1, 0, 0, 6335.77, 4885.79, -162.549, 2.30223, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+296, 70545, 1098, 0, 0, 248, 1, 0, 0, 6317.72, 4873.1, -162.206, 3.20569, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+297, 70545, 1098, 0, 0, 248, 1, 0, 0, 6358.6, 4862.19, -163.627, 5.10602, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+298, 70545, 1098, 0, 0, 248, 1, 0, 0, 6298.08, 4846.55, -158.108, 5.16776, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+299, 70545, 1098, 0, 0, 248, 1, 0, 0, 6337.27, 4924.45, -161.931, 0.158766, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+300, 70545, 1098, 0, 0, 248, 1, 0, 0, 6168.27, 4912.94, -81.6285, 0.0649811, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+301, 70545, 1098, 0, 0, 248, 1, 0, 0, 6309.01, 4904.87, -161.452, 5.49988, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+302, 70545, 1098, 0, 0, 248, 1, 0, 0, 6311.02, 4923.88, -161.32, 0.0130793, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+303, 70545, 1098, 0, 0, 248, 1, 0, 0, 6168.51, 4919.07, -79.8128, 4.04951, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+304, 70545, 1098, 0, 0, 248, 1, 0, 0, 6321.37, 4918, -161.489, 4.64937, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+305, 70545, 1098, 0, 0, 248, 1, 0, 0, 6303.88, 4921.44, -161.258, 5.86311, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+306, 70545, 1098, 0, 0, 248, 1, 0, 0, 6319.44, 4907.3, -161.633, 0.373116, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+307, 70545, 1098, 0, 0, 248, 1, 0, 0, 6326.37, 4922.42, -161.533, 4.90503, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+308, 70545, 1098, 0, 0, 248, 1, 0, 0, 6333, 4871.05, -162.172, 5.57149, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+309, 70545, 1098, 0, 0, 248, 1, 0, 0, 6343.31, 4920.28, -162.286, 5.75173, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+310, 70545, 1098, 0, 0, 248, 1, 0, 0, 6334.94, 4878.49, -162.627, 6.00101, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+311, 70545, 1098, 0, 0, 248, 1, 0, 0, 6329.3, 4884.49, -162.852, 3.48955, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+312, 70545, 1098, 0, 0, 248, 1, 0, 0, 6301.23, 4843.54, -158.202, 0.438114, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+313, 70545, 1098, 0, 0, 248, 1, 0, 0, 6335.14, 4915.62, -162.02, 3.37654, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+314, 70545, 1098, 0, 0, 248, 1, 0, 0, 6290.13, 4847.13, -157.046, 3.73172, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+315, 70545, 1098, 0, 0, 248, 1, 0, 0, 6326.07, 4889.74, -162.632, 4.80731, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+316, 70545, 1098, 0, 0, 248, 1, 0, 0, 6321.02, 4884.7, -161.859, 5.75238, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+317, 70545, 1098, 0, 0, 248, 1, 0, 0, 6289.41, 4834.15, -154.143, 5.22785, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+318, 70545, 1098, 0, 0, 248, 1, 0, 0, 6370.86, 4893, -164.385, 1.18023, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+319, 70545, 1098, 0, 0, 248, 1, 0, 0, 6190.32, 4725.29, -171.553, 5.28266, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+320, 70545, 1098, 0, 0, 248, 1, 0, 0, 6367.57, 4886.56, -164.224, 0.664544, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+321, 70545, 1098, 0, 0, 248, 1, 0, 0, 6347.65, 4855.25, -163.058, 5.10602, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+322, 70545, 1098, 0, 0, 248, 1, 0, 0, 6373.4, 4911.42, -162.64, 2.59694, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+323, 70545, 1098, 0, 0, 248, 1, 0, 0, 6344.83, 4846.28, -162.733, 1.81733, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+324, 70545, 1098, 0, 0, 248, 1, 0, 0, 6161.98, 4915.63, -78.9803, 1.69498, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+325, 70545, 1098, 0, 0, 248, 1, 0, 0, 6161.15, 4918.51, -77.9185, 4.04951, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+326, 70545, 1098, 0, 0, 248, 1, 0, 0, 6299.94, 4833.65, -155.665, 5.48911, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+327, 70545, 1098, 0, 0, 248, 1, 0, 0, 6327.2, 4868.17, -162.321, 2.30223, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+328, 70545, 1098, 0, 0, 248, 1, 0, 0, 6289.83, 4839.34, -154.945, 0.268771, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+329, 70545, 1098, 0, 0, 248, 1, 0, 0, 6292.45, 4856.41, -159.281, 0.631754, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+330, 70545, 1098, 0, 0, 248, 1, 0, 0, 6295.27, 4851.98, -158.587, 3.46655, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+331, 70545, 1098, 0, 0, 248, 1, 0, 0, 6341, 4839.92, -163.731, 2.30223, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+332, 70545, 1098, 0, 0, 248, 1, 0, 0, 6311.15, 4908.64, -161.443, 4.11012, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+333, 70545, 1098, 0, 0, 248, 1, 0, 0, 6299.61, 4784.28, -171.939, 0.108605, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+334, 70545, 1098, 0, 0, 248, 1, 0, 0, 6356.84, 4850.15, -164.161, 0.844992, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+335, 70545, 1098, 0, 0, 248, 1, 0, 0, 6296.27, 4776.15, -170.732, 5.99803, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+336, 70545, 1098, 0, 0, 248, 1, 0, 0, 6349.53, 4841.45, -163.545, 5.10602, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+337, 70545, 1098, 0, 0, 248, 1, 0, 0, 6389.25, 4910.17, -163.159, 4.30297, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+338, 70545, 1098, 0, 0, 248, 1, 0, 0, 6200.01, 4722.12, -170.633, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+339, 70545, 1098, 0, 0, 248, 1, 0, 0, 6193.5, 4711.23, -172.403, 4.38063, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+340, 70545, 1098, 0, 0, 248, 1, 0, 0, 6383.75, 4898.82, -163.969, 2.70396, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+341, 70545, 1098, 0, 0, 248, 1, 0, 0, 6182.33, 4716.08, -170.582, 2.33353, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+342, 70545, 1098, 0, 0, 248, 1, 0, 0, 6182.8, 4703.34, -172.942, 2.89756, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+343, 70545, 1098, 0, 0, 248, 1, 0, 0, 6310.23, 4782.33, -171.412, 2.53059, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+344, 70545, 1098, 0, 0, 248, 1, 0, 0, 6377.4, 4882.94, -165.086, 1.09053, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+345, 70545, 1098, 0, 0, 248, 1, 0, 0, 6304.67, 4779.2, -170.759, 5.14585, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+346, 70545, 1098, 0, 0, 248, 1, 0, 0, 6207.87, 4711.9, -170.582, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+347, 70545, 1098, 0, 0, 248, 1, 0, 0, 6292.89, 4767.38, -170.558, 3.19491, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+348, 70545, 1098, 0, 0, 248, 1, 0, 0, 6196.03, 4704.05, -171.208, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+349, 70545, 1098, 0, 0, 248, 1, 0, 0, 6302.32, 4771.56, -171.137, 4.21672, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+350, 70545, 1098, 0, 0, 248, 1, 0, 0, 6221.02, 4718.71, -171.399, 4.07046, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+351, 70545, 1098, 0, 0, 248, 1, 0, 0, 6357.37, 4840.58, -163.829, 5.16592, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+352, 70545, 1098, 0, 0, 248, 1, 0, 0, 6206.1, 4703.51, -170.733, 2.91445, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+353, 70545, 1098, 0, 0, 248, 1, 0, 0, 6385.03, 4880.91, -165.484, 1.93007, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+354, 70545, 1098, 0, 0, 248, 1, 0, 0, 6215.98, 4709.22, -171.185, 5.55117, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+355, 70545, 1098, 0, 0, 248, 1, 0, 0, 6368.65, 4850.72, -164.241, 4.93957, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+356, 70545, 1098, 0, 0, 248, 1, 0, 0, 6222.61, 4708.3, -171.04, 1.22749, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+357, 70545, 1098, 0, 0, 248, 1, 0, 0, 6312.21, 4773.76, -170.615, 2.58657, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+358, 70545, 1098, 0, 0, 248, 1, 0, 0, 6315.58, 4778.79, -170.65, 4.56846, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+359, 70545, 1098, 0, 0, 248, 1, 0, 0, 6185.83, 4695.07, -170.833, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+360, 70545, 1098, 0, 0, 248, 1, 0, 0, 6391.86, 4888.13, -164.826, 1.4823, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+361, 70545, 1098, 0, 0, 248, 1, 0, 0, 6199.24, 4695.98, -170.841, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+362, 70545, 1098, 0, 0, 248, 1, 0, 0, 6193.95, 4692.88, -171.408, 1.22648, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+363, 70545, 1098, 0, 0, 248, 1, 0, 0, 6269.9, 4727.8, -170.548, 3.18273, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+364, 70545, 1098, 0, 0, 248, 1, 0, 0, 6303.49, 4761.42, -170.842, 0.0905539, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+365, 70545, 1098, 0, 0, 248, 1, 0, 0, 6319.02, 4770.77, -170.434, 0.570157, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+366, 70545, 1098, 0, 0, 248, 1, 0, 0, 6263.98, 4724.08, -170.901, 4.89864, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+367, 70545, 1098, 0, 0, 248, 1, 0, 0, 6309.52, 4768.76, -170.967, 1.0092, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+368, 70545, 1098, 0, 0, 248, 1, 0, 0, 6314.81, 4762.95, -170.686, 3.56797, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+369, 70545, 1098, 0, 0, 248, 1, 0, 0, 6276.27, 4732.05, -170.736, 0.535247, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+370, 70545, 1098, 0, 0, 248, 1, 0, 0, 6307.47, 4764.69, -170.459, 0.293672, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+371, 70545, 1098, 0, 0, 248, 1, 0, 0, 6270.45, 4734.47, -170.248, 1.09068, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+372, 70545, 1098, 0, 0, 248, 1, 0, 0, 6284.88, 4737.34, -169.46, 2.26093, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+373, 70545, 1098, 0, 0, 248, 1, 0, 0, 6284.94, 4728.88, -169.639, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+374, 70545, 1098, 0, 0, 248, 1, 0, 0, 6287.46, 4721.85, -169.759, 2.67408, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+375, 70545, 1098, 0, 0, 248, 1, 0, 0, 6223.53, 4678.3, -171.314, 1.31964, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+376, 70545, 1098, 0, 0, 248, 1, 0, 0, 6217.73, 4676.72, -170.905, 1.5634, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+377, 70545, 1098, 0, 0, 248, 1, 0, 0, 6276.73, 4719.79, -170.567, 5.07932, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+378, 70545, 1098, 0, 0, 248, 1, 0, 0, 6370.72, 4840.59, -165.601, 5.10602, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+379, 70545, 1098, 0, 0, 248, 1, 0, 0, 6399.25, 4895.49, -164.406, 2.87858, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+380, 70545, 1098, 0, 0, 248, 1, 0, 0, 6269.77, 4717.29, -170.541, 0.848459, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+381, 70545, 1098, 0, 0, 248, 1, 0, 0, 6232.44, 4681.01, -170.347, 5.60968, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+382, 70545, 1098, 0, 0, 248, 1, 0, 0, 6206.52, 4675.23, -171.076, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+383, 70545, 1098, 0, 0, 248, 1, 0, 0, 6281, 4725.16, -170.647, 1.07993, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+384, 70545, 1098, 0, 0, 248, 1, 0, 0, 6294.14, 4719.41, -168.578, 4.41399, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+385, 70545, 1098, 0, 0, 248, 1, 0, 0, 6282.85, 4714.77, -169.47, 5.4162, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+386, 70545, 1098, 0, 0, 248, 1, 0, 0, 6218.42, 4667.96, -170.926, 1.6782, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+387, 70545, 1098, 0, 0, 248, 1, 0, 0, 6237.33, 4681.64, -170.276, 2.46704, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+388, 70545, 1098, 0, 0, 248, 1, 0, 0, 6277.87, 4711.23, -169.772, 2.26399, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+389, 70545, 1098, 0, 0, 248, 1, 0, 0, 6212.79, 4668.7, -171.049, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+390, 70545, 1098, 0, 0, 248, 1, 0, 0, 6203.04, 4664.81, -170.582, 4.75181, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+391, 70545, 1098, 0, 0, 248, 1, 0, 0, 6234.37, 4671.72, -170.33, 5.0173, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+392, 70545, 1098, 0, 0, 248, 1, 0, 0, 6210.02, 4660.42, -171.095, 1.07768, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+393, 70545, 1098, 0, 0, 248, 1, 0, 0, 6203.91, 4656.84, -170.582, 2.8365, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+394, 70545, 1098, 0, 0, 248, 1, 0, 0, 6226.47, 4669.73, -171.055, 2.39138, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+395, 70545, 1098, 0, 0, 248, 1, 0, 0, 6211.03, 4653.31, -170.582, 5.95404, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+396, 70545, 1098, 0, 0, 248, 1, 0, 0, 6300.48, 4722.36, -168.742, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+397, 70545, 1098, 0, 0, 248, 1, 0, 0, 6226.87, 4662.53, -171.617, 5.97475, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+398, 70545, 1098, 0, 0, 248, 1, 0, 0, 6222.11, 4657.75, -170.529, 4.13269, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+399, 70545, 1098, 0, 0, 248, 1, 0, 0, 6230.49, 4653.92, -170.46, 2.90105, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+400, 70545, 1098, 0, 0, 248, 1, 0, 0, 6221.56, 4652.2, -170.553, 1.06948, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+401, 70545, 1098, 0, 0, 248, 1, 0, 0, 6368.54, 4748.3, -168.925, 1.08732, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+402, 70545, 1098, 0, 0, 248, 1, 0, 0, 6377.88, 4748.75, -169.635, 2.4216, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+403, 70545, 1098, 0, 0, 248, 1, 0, 0, 6386.65, 4762.31, -169.59, 4.14699, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+404, 70545, 1098, 0, 0, 248, 1, 0, 0, 6366.64, 4735.86, -167.255, 5.56156, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+405, 70545, 1098, 0, 0, 248, 1, 0, 0, 6376.08, 4755.39, -169.368, 5.9827, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+406, 70545, 1098, 0, 0, 248, 1, 0, 0, 6373.51, 4742.37, -167.742, 4.31747, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+407, 70545, 1098, 0, 0, 248, 1, 0, 0, 6382.03, 4759.31, -169.862, 5.0954, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+408, 70545, 1098, 0, 0, 248, 1, 0, 0, 6379.08, 4740.53, -167.452, 1.20797, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+409, 70545, 1098, 0, 0, 248, 1, 0, 0, 6375.51, 4734.79, -166.641, 2.52846, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+410, 70545, 1098, 0, 0, 248, 1, 0, 0, 6394.38, 4766.95, -170.788, 2.56009, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+411, 70545, 1098, 0, 0, 248, 1, 0, 0, 6384.61, 4744.02, -168.628, 5.00102, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+412, 70545, 1098, 0, 0, 248, 1, 0, 0, 6382.71, 4735.77, -166.722, 4.29499, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+413, 70545, 1098, 0, 0, 248, 1, 0, 0, 6391.5, 4756.46, -168.917, 0.430325, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+414, 70545, 1098, 0, 0, 248, 1, 0, 0, 6407.53, 4753.48, -170.373, 2.84646, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+415, 70545, 1098, 0, 0, 248, 1, 0, 0, 6387.47, 4733.43, -167.783, 3.51249, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+416, 70545, 1098, 0, 0, 248, 1, 0, 0, 6409.01, 4778.02, -171.206, 0.488434, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+417, 70545, 1098, 0, 0, 248, 1, 0, 0, 6404.52, 4774.04, -170.837, 5.539, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+418, 70545, 1098, 0, 0, 248, 1, 0, 0, 6391.5, 4748.69, -169.059, 2.5525, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+419, 70545, 1098, 0, 0, 248, 1, 0, 0, 6393.87, 4743.47, -168.606, 6.19605, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+420, 70545, 1098, 0, 0, 248, 1, 0, 0, 6398.89, 4750.28, -169.386, 5.74134, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+421, 70545, 1098, 0, 0, 248, 1, 0, 0, 6405.64, 4762.68, -170.522, 3.4425, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+422, 70545, 1098, 0, 0, 248, 1, 0, 0, 6393.7, 4736.75, -170.061, 2.94855, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+423, 70545, 1098, 0, 0, 248, 1, 0, 0, 6399.51, 4764.55, -169.473, 1.61261, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+424, 70545, 1098, 0, 0, 248, 1, 0, 0, 6413.26, 4767.27, -170.817, 4.51947, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+425, 70545, 1098, 0, 0, 248, 1, 0, 0, 6332.77, 4605.79, -199.947, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+426, 70545, 1098, 0, 0, 248, 1, 0, 0, 6352.06, 4610.54, -205.26, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+427, 70545, 1098, 0, 0, 248, 1, 0, 0, 6341.51, 4604.86, -202.761, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+428, 70545, 1098, 0, 0, 248, 1, 0, 0, 6322, 4589.33, -198.89, 6.163, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+429, 70545, 1098, 0, 0, 248, 1, 0, 0, 6343.86, 4608.33, -203.471, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+430, 70545, 1098, 0, 0, 248, 1, 0, 0, 6321.9, 4595.53, -198.745, 2.77407, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+431, 70545, 1098, 0, 0, 248, 1, 0, 0, 6339.1, 4597.92, -202.751, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+432, 70545, 1098, 0, 0, 248, 1, 0, 0, 6343.61, 4613.99, -202.695, 6.18941, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+433, 70545, 1098, 0, 0, 248, 1, 0, 0, 6352.32, 4605.72, -205.865, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+434, 70545, 1098, 0, 0, 248, 1, 0, 0, 6330.92, 4594.85, -201.043, 6.27182, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+435, 70545, 1098, 0, 0, 248, 1, 0, 0, 6361.02, 4607.16, -206.422, 2.2036, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+436, 70545, 1098, 0, 0, 248, 1, 0, 0, 6373.68, 4606.63, -207.609, 2.6679, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+437, 70545, 1098, 0, 0, 248, 1, 0, 0, 6357.35, 4592.77, -209.37, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+438, 70545, 1098, 0, 0, 248, 1, 0, 0, 6345.42, 4591.99, -206.432, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+439, 70545, 1098, 0, 0, 248, 1, 0, 0, 6330.17, 4579.44, -202.456, 0.25394, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+440, 70545, 1098, 0, 0, 248, 1, 0, 0, 6359.48, 4602.27, -207.491, 6.24618, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+441, 70545, 1098, 0, 0, 248, 1, 0, 0, 6351.45, 4595.47, -206.951, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+442, 70545, 1098, 0, 0, 248, 1, 0, 0, 6334.25, 4583.59, -204.009, 1.32562, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+443, 70545, 1098, 0, 0, 248, 1, 0, 0, 6341.44, 4590.28, -205.182, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+444, 70545, 1098, 0, 0, 248, 1, 0, 0, 6358.16, 4583.84, -209.094, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+445, 70545, 1098, 0, 0, 248, 1, 0, 0, 6345.12, 4582.81, -207.197, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+446, 70545, 1098, 0, 0, 248, 1, 0, 0, 6336.93, 4570.19, -206.777, 1.15199, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+447, 70545, 1098, 0, 0, 248, 1, 0, 0, 6350.67, 4578, -209.629, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+448, 70545, 1098, 0, 0, 248, 1, 0, 0, 6368.27, 4590.4, -209.6, 0.524738, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+449, 70545, 1098, 0, 0, 248, 1, 0, 0, 6367.41, 4595.81, -209.275, 3.88748, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+450, 70545, 1098, 0, 0, 248, 1, 0, 0, 6329.52, 4569.84, -204.384, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+451, 70545, 1098, 0, 0, 248, 1, 0, 0, 6371.99, 4587.58, -209.539, 6.06725, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+452, 70545, 1098, 0, 0, 248, 1, 0, 0, 6342.21, 4574.46, -208.166, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+453, 70545, 1098, 0, 0, 248, 1, 0, 0, 6372.77, 4598.7, -208.328, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+454, 70545, 1098, 0, 0, 248, 1, 0, 0, 6361.04, 4579.62, -209.977, 1.24357, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+455, 70545, 1098, 0, 0, 248, 1, 0, 0, 6380.55, 4581.84, -209.555, 2.80555, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+456, 70545, 1098, 0, 0, 248, 1, 0, 0, 6353.86, 4572.83, -209.094, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+457, 70545, 1098, 0, 0, 248, 1, 0, 0, 6341.67, 4567.54, -208.761, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+458, 70545, 1098, 0, 0, 248, 1, 0, 0, 6366.76, 4576.07, -209.742, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+459, 70545, 1098, 0, 0, 248, 1, 0, 0, 6381.99, 4590.87, -209.094, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+460, 70545, 1098, 0, 0, 248, 1, 0, 0, 6354.69, 4565.54, -209.502, 1.0522, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+461, 70545, 1098, 0, 0, 248, 1, 0, 0, 6348.83, 4560.26, -209.652, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+462, 70545, 1098, 0, 0, 248, 1, 0, 0, 6360.28, 4566.86, -209.094, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+463, 70545, 1098, 0, 0, 248, 1, 0, 0, 6367.25, 4569.35, -209.094, 0.957014, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+464, 70545, 1098, 0, 0, 248, 1, 0, 0, 6372.46, 4570.77, -209.792, 4.70675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+465, 70545, 1098, 0, 0, 248, 1, 0, 0, 6169.99, 4915.78, -81.5819, 5.68837, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+466, 70545, 1098, 0, 0, 248, 1, 0, 0, 6166.45, 4908.45, -81.9798, 4.04951, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+496, 70587, 1098, 0, 0, 248, 1, 0, 0, 6163.69, 4920.41, -78.1711, 0, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+497, 70587, 1098, 0, 0, 248, 1, 0, 0, 6317.62, 4909.68, -161.654, 2.81632, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+498, 70587, 1098, 0, 0, 248, 1, 0, 0, 6356.66, 4858.32, -163.228, 0.581196, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+499, 70587, 1098, 0, 0, 248, 1, 0, 0, 6346.52, 4846.26, -162.72, 0.814264, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+500, 70587, 1098, 0, 0, 248, 1, 0, 0, 6319.79, 4916.09, -161.649, 3.3794, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+501, 70587, 1098, 0, 0, 248, 1, 0, 0, 6338.3, 4929.97, -161.913, 1.87372, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+502, 70587, 1098, 0, 0, 248, 1, 0, 0, 6164.85, 4913.19, -79.7577, 0, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+503, 70587, 1098, 0, 0, 248, 1, 0, 0, 6332.1, 4922.05, -161.813, 6.23176, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+504, 70587, 1098, 0, 0, 248, 1, 0, 0, 6324.01, 4923.94, -161.616, 1.32264, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+505, 70587, 1098, 0, 0, 248, 1, 0, 0, 6295.57, 4844.82, -157.194, 0.502669, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+506, 70587, 1098, 0, 0, 248, 1, 0, 0, 6323.98, 4884.52, -161.985, 3.43453, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+507, 70587, 1098, 0, 0, 248, 1, 0, 0, 6327.2, 4876.63, -162.135, 3.22213, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+508, 70587, 1098, 0, 0, 248, 1, 0, 0, 6289.15, 4835.87, -154.422, 3.93134, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+509, 70587, 1098, 0, 0, 248, 1, 0, 0, 6328.5, 4881.43, -162.113, 0.808733, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+510, 70587, 1098, 0, 0, 248, 1, 0, 0, 6311.25, 4919.39, -161.441, 3.49112, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+511, 70587, 1098, 0, 0, 248, 1, 0, 0, 6304.48, 4912.61, -161.4, 1.58753, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+512, 70587, 1098, 0, 0, 248, 1, 0, 0, 6320.35, 4880.02, -161.957, 0.34206, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+513, 70587, 1098, 0, 0, 248, 1, 0, 0, 6293.68, 4849.11, -157.764, 4.31342, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+514, 70587, 1098, 0, 0, 248, 1, 0, 0, 6187.17, 4709.67, -171.117, 4.73787, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+515, 70587, 1098, 0, 0, 248, 1, 0, 0, 6196.32, 4718.14, -170.687, 2.7948, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+516, 70587, 1098, 0, 0, 248, 1, 0, 0, 6203.52, 4711.32, -170.661, 2.72271, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+517, 70587, 1098, 0, 0, 248, 1, 0, 0, 6362.52, 4855.2, -163.748, 2.43922, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+518, 70587, 1098, 0, 0, 248, 1, 0, 0, 6188.8, 4718.32, -170.709, 2.74751, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+519, 70587, 1098, 0, 0, 248, 1, 0, 0, 6357.77, 4845.66, -163.712, 0.860483, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+520, 70587, 1098, 0, 0, 248, 1, 0, 0, 6217.33, 4714.51, -170.635, 1.70076, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+521, 70587, 1098, 0, 0, 248, 1, 0, 0, 6303.13, 4779, -170.872, 0.280907, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+522, 70587, 1098, 0, 0, 248, 1, 0, 0, 6188.02, 4699.02, -171.09, 5.76855, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+523, 70587, 1098, 0, 0, 248, 1, 0, 0, 6199.6, 4703.37, -170.961, 2.82027, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+524, 70587, 1098, 0, 0, 248, 1, 0, 0, 6311.33, 4775.87, -170.738, 4.97498, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+525, 70587, 1098, 0, 0, 248, 1, 0, 0, 6301.27, 4767.76, -170.655, 0.632324, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+526, 70587, 1098, 0, 0, 248, 1, 0, 0, 6367.38, 4843.57, -164.497, 5.75612, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+527, 70587, 1098, 0, 0, 248, 1, 0, 0, 6267.96, 4730.96, -170.31, 2.53712, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+528, 70587, 1098, 0, 0, 248, 1, 0, 0, 6314.25, 4768.1, -170.561, 1.66765, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+529, 70587, 1098, 0, 0, 248, 1, 0, 0, 6271.93, 4722.25, -170.173, 3.57644, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+530, 70587, 1098, 0, 0, 248, 1, 0, 0, 6277.54, 4727.03, -170.18, 1.37126, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+531, 70587, 1098, 0, 0, 248, 1, 0, 0, 6202.65, 4672.03, -170.665, 2.51062, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+532, 70587, 1098, 0, 0, 248, 1, 0, 0, 6215.22, 4672.5, -170.528, 0, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+533, 70587, 1098, 0, 0, 248, 1, 0, 0, 6225.35, 4683.97, -170.526, 2.04793, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+534, 70587, 1098, 0, 0, 248, 1, 0, 0, 6276.44, 4710.57, -169.954, 0.30269, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+535, 70587, 1098, 0, 0, 248, 1, 0, 0, 6203.92, 4660.17, -170.665, 2.82379, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+536, 70587, 1098, 0, 0, 248, 1, 0, 0, 6288.03, 4717.94, -169.108, 0, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+537, 70587, 1098, 0, 0, 248, 1, 0, 0, 6214.34, 4661.21, -170.616, 1.68071, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+538, 70587, 1098, 0, 0, 248, 1, 0, 0, 6216.64, 4665.59, -170.602, 3.09712, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+539, 70587, 1098, 0, 0, 248, 1, 0, 0, 6234.45, 4671.48, -170.412, 6.02017, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+540, 70587, 1098, 0, 0, 248, 1, 0, 0, 6225.19, 4658.33, -170.559, 2.87335, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+541, 70587, 1098, 0, 0, 248, 1, 0, 0, 6382.86, 4758.43, -169.424, 0.0288818, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+542, 70587, 1098, 0, 0, 248, 1, 0, 0, 6372.37, 4743.43, -167.937, 1.8152, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+543, 70587, 1098, 0, 0, 248, 1, 0, 0, 6379.26, 4750.17, -168.725, 0.250415, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+544, 70587, 1098, 0, 0, 248, 1, 0, 0, 6390.96, 4766.39, -170.057, 4.76201, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+545, 70587, 1098, 0, 0, 248, 1, 0, 0, 6391.59, 4753.41, -168.779, 5.11012, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+546, 70587, 1098, 0, 0, 248, 1, 0, 0, 6383.35, 4738.99, -167.188, 0.813147, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+547, 70587, 1098, 0, 0, 248, 1, 0, 0, 6404.16, 4757.84, -170, 1.34259, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+548, 70587, 1098, 0, 0, 248, 1, 0, 0, 6410.41, 4764.69, -170.196, 0.54559, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+549, 70587, 1098, 0, 0, 248, 1, 0, 0, 6390.21, 4737.51, -167.682, 5.61148, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+550, 70587, 1098, 0, 0, 248, 1, 0, 0, 6404.08, 4769.75, -169.865, 5.87707, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+551, 70587, 1098, 0, 0, 248, 1, 0, 0, 6411.24, 4774.62, -170.692, 4.54675, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+552, 70587, 1098, 0, 0, 248, 1, 0, 0, 6334.44, 4603.15, -200.341, 3.92146, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+553, 70587, 1098, 0, 0, 248, 1, 0, 0, 6346.63, 4601.01, -204.835, 1.121, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+554, 70587, 1098, 0, 0, 248, 1, 0, 0, 6333.94, 4576.2, -204.469, 5.62238, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+555, 70587, 1098, 0, 0, 248, 1, 0, 0, 6359.06, 4596.76, -208.545, 5.73281, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+556, 70587, 1098, 0, 0, 248, 1, 0, 0, 6327.44, 4584.37, -200.849, 4.81637, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+557, 70587, 1098, 0, 0, 248, 1, 0, 0, 6354.23, 4599.84, -207.017, 5.171, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+558, 70587, 1098, 0, 0, 248, 1, 0, 0, 6365.89, 4605.37, -207.514, 0.399958, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+559, 70587, 1098, 0, 0, 248, 1, 0, 0, 6334.19, 4590.86, -202.5, 4.27305, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+560, 70587, 1098, 0, 0, 248, 1, 0, 0, 6377.46, 4588.79, -209.177, 3.05383, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+561, 70587, 1098, 0, 0, 248, 1, 0, 0, 6351.9, 4586.94, -208.409, 5.25339, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+562, 70587, 1098, 0, 0, 248, 1, 0, 0, 6360.12, 4588.19, -209.177, 1.67542, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+563, 70587, 1098, 0, 0, 248, 1, 0, 0, 6361.33, 4573.01, -209.177, 2.79068, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+564, 70587, 1098, 0, 0, 248, 1, 0, 0, 6371.95, 4580.62, -209.177, 5.46908, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+565, 70587, 1098, 0, 0, 248, 1, 0, 0, 6347.49, 4566.96, -209.177, 0.441825, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+566, 70153, 1098, 0, 0, 248, 1, 0, 0, 6170.83, 4871.85, -93.7686, 4.52646, 86400, 0, @CGUID+566, 1, 0, 2, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+567, 70153, 1098, 0, 0, 248, 1, 0, 0, 6141.45, 4715.5, -172.214, 0.940205, 86400, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+568, 70153, 1098, 0, 0, 248, 1, 0, 0, 6203.21, 4752.33, -172.212, 0.0902003, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+569, 70153, 1098, 0, 0, 248, 1, 0, 0, 6299.86, 4799.72, -172.213, 3.31379, 86400, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+570, 70153, 1098, 0, 0, 248, 1, 0, 0, 6381.07, 4895.02, -164.128, 0.594793, 86400, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+571, 70153, 1098, 0, 0, 248, 1, 0, 0, 6239.28, 4722.95, -170.874, 2.53034, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+572, 70153, 1098, 0, 0, 248, 1, 0, 0, 6163.17, 4649.38, -172.213, 2.59461, 86400, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+573, 70153, 1098, 0, 0, 248, 1, 0, 0, 6406.86, 4830.85, -172.102, 0.835624, 86400, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+574, 70153, 1098, 0, 0, 248, 1, 0, 0, 6349.04, 4662.74, -203.517, 0.273601, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+575, 70153, 1098, 0, 0, 248, 1, 0, 0, 6436.2, 4728.92, -172.213, 0.371628, 86400, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+576, 70153, 1098, 0, 0, 248, 1, 0, 0, 6286.96, 4557.95, -192.782, 1.14753, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+577, 70589, 1098, 0, 0, 248, 1, 0, 0, 6279.71, 4859.76, -157.499, 4.91022, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+578, 70589, 1098, 0, 0, 248, 1, 0, 0, 6302.24, 4915.89, -161.357, 3.89822, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+579, 70589, 1098, 0, 0, 248, 1, 0, 0, 6324.36, 4889.36, -161.955, 2.64846, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+580, 70589, 1098, 0, 0, 248, 1, 0, 0, 6193.22, 4714.58, -170.719, 3.50686, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+581, 70589, 1098, 0, 0, 248, 1, 0, 0, 6317.19, 4766.65, -170.437, 6.23547, 86400, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+582, 70589, 1098, 0, 0, 248, 1, 0, 0, 6361.06, 4847.54, -163.88, 0.913586, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+583, 70589, 1098, 0, 0, 248, 1, 0, 0, 6209.47, 4657.52, -170.665, 0.178937, 86400, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+584, 70589, 1098, 0, 0, 248, 1, 0, 0, 6260.52, 4727.39, -170.327, 2.04788, 86400, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+585, 70589, 1098, 0, 0, 248, 1, 0, 0, 6372.21, 4750.29, -168.9, 1.05964, 86400, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+586, 70589, 1098, 0, 0, 248, 1, 0, 0, 6333.96, 4594.94, -201.655, 4.58811, 86400, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+587, 70594, 1098, 0, 0, 248, 1, 0, 0, 6330.44, 4849.58, -161.55, 0.980299, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+588, 70594, 1098, 0, 0, 248, 1, 0, 0, 6313.43, 4895.58, -161.695, 0.103773, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+589, 70594, 1098, 0, 0, 248, 1, 0, 0, 6286.5, 4868.56, -160.475, 6.05115, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+590, 70594, 1098, 0, 0, 248, 1, 0, 0, 6155.64, 4684.54, -172.214, 2.99204, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+591, 70594, 1098, 0, 0, 248, 1, 0, 0, 6226.92, 4692.25, -170.496, 0.257595, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+592, 70594, 1098, 0, 0, 248, 1, 0, 0, 6336.49, 4773.57, -170.779, 2.34687, 86400, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+593, 70594, 1098, 0, 0, 248, 1, 0, 0, 6466.41, 4762.36, -172.213, 2.10317, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+594, 70594, 1098, 0, 0, 248, 1, 0, 0, 6462.97, 4706.18, -172.213, 4.98676, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+595, 70586, 1098, 0, 0, 248, 1, 0, 0, 6289.08, 4916.77, -161.146, 5.0826, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+596, 70586, 1098, 0, 0, 248, 1, 0, 0, 6114.24, 4672.23, -172.13, 0.208407, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+597, 70586, 1098, 0, 0, 248, 1, 0, 0, 6488.63, 4739.99, -172.274, 3.15051, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

(@CGUID+598, 70157, 1098, 0, 0, 248, 1, 0, 0, 6270.05, 4602.21, -193.453, 0, 86400, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),

-- (@CGUID+599, 70247, 1098, 0, 0, 248, 1, 0, 0, 6395.12, 4494.94, -209.609, 1.82608, 604800, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
-- (@CGUID+600, 70235, 1098, 0, 0, 248, 1, 0, 0, 6419.33, 4504.38, -209.609, 2.3032,  604800, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
-- (@CGUID+601, 70212, 1098, 0, 0, 248, 1, 0, 0, 6457.67, 4470.42, -209.609, 2.00024, 604800, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
-- (@CGUID+602, 70252, 1098, 0, 0,  99, 1, 0, 0, 6475.09, 4490.12, -210.033, 2.42966, 604800, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+603, 68065, 1098, 0, 0, 248, 1, 0, 0, 6467.56, 4483.96, -210.032, 2.50183, 604800, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+619, 68553, 1098, 0, 0, 248, 1, 0, 0, 6285.99, 4923.5, -161.133, 5.26395,  604800, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+620, 68553, 1098, 0, 0, 248, 1, 0, 0, 6106.54, 4670.81, -172.214, 0.220267,604800, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+621, 68553, 1098, 0, 0, 248, 1, 0, 0, 6496.56, 4740.43, -172.019, 0.044118,604800, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Auras
DELETE FROM creature_template_aura WHERE entry IN (67966,3167966,3267966,3367966,3467966,3567966,70147,70157,70586,3170586,3270586,3370586,3470586,3570586);
INSERT INTO creature_template_aura VALUES
(67966,110470),
(3167966,110470),
(3267966,110470),
(3367966,110470),
(3467966,110470),
(3567966,110470),
(70147,140809),
(70586,140577),
(3170586,140577),
(3270586,140577),
(3370586,140577),
(3470586,140577),
(3570586,140577);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Path & Waypoints
DELETE FROM `creature_path` WHERE `guid` IN (@CGUID+566);
INSERT INTO `creature_path` VALUES
(@CGUID+566,@CGUID+566);

DELETE FROM `waypoint_data` WHERE `id`IN (@CGUID+566);
INSERT INTO `waypoint_data` VALUES
(@CGUID+566,  1,  6170.83, 4871.85, -93.7686, 0, 0, 0, 0, 100, 0),
(@CGUID+566,  2,  6170.69, 4850.14, -101.017, 0, 0, 0, 0, 100, 0),
(@CGUID+566,  3,  6176.6, 4837, -106.608, 0, 0, 0, 0, 100, 0),
(@CGUID+566,  4,  6190.48, 4822.75, -115.202, 0, 0, 0, 0, 100, 0),
(@CGUID+566,  5,  6211.69, 4815.95, -125.731, 0, 0, 0, 0, 100, 0),
(@CGUID+566,  6,  6233.71, 4819.48, -134.481, 0, 0, 0, 0, 100, 0),
(@CGUID+566,  7,  6257.94, 4830.83, -144.843, 0, 0, 0, 0, 100, 0),
(@CGUID+566,  8,  6233.71, 4819.48, -134.481, 0, 0, 0, 0, 100, 0),
(@CGUID+566,  9,  6211.69, 4815.95, -125.731, 0, 0, 0, 0, 100, 0),
(@CGUID+566, 10,  6190.48, 4822.75, -115.202, 0, 0, 0, 0, 100, 0),
(@CGUID+566, 11,  6176.6, 4837, -106.608, 0, 0, 0, 0, 100, 0),
(@CGUID+566, 12,  6170.69, 4850.14, -101.017, 0, 0, 0, 0, 100, 0),
(@CGUID+566, 13,  6170.83, 4871.85, -93.7686, 0, 0, 0, 0, 100, 0),
(@CGUID+566, 14,  6177.05, 4894.25, -87.3037, 0, 0, 0, 0, 100, 0);
/*---------------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Auras
DELETE FROM creature_template_aura WHERE entry IN (70212, 70235, 70247, 70252);
INSERT INTO creature_template_aura VALUES
(70212, 139832),
(70235, 139832),
(70247, 139832),
(70252, 139832);

/*---------------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------------------*/

-- Creature Speeches
DELETE FROM creature_text WHERE entry IN (68065, 68553);
INSERT INTO creature_text VALUES
(68065, 0, 0,'Megaera begins to |cFFFF0000|Hspell:137175|h[Rampage]|h|r!',41,0,100,0,0,0,'Megaera'),
(68065, 1, 0,"Megaera's rage subsides.",41,0,100,0,0,0,'Megaera'),
(68553, 0, 0, 'The cavern trembles violently!', 41, 0, 100, 0, 0, 36511, 'SLG Generic MoP (Large AOI) to Player'),
(68553, 1, 0, 'An ancient beast stirs within the mists!', 41, 0, 100, 0, 0, 36512,'SLG Generic MoP (Large AOI) to Player'),
(68553, 2, 0, '|cFFF00000Megaera|r rises from the mists!', 41, 0, 100, 0, 0, 36513, 'SLG Generic MoP (Large AOI) to Player');
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Gameobjects templates
REPLACE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `AIName`, `ScriptName`, `WDBVerified`) VALUES 
(218721, 0, 13620, 'Geyser', '', '', '', 1375, 32, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218723, 0, 13677, 'Ancient Mogu Bell', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 'go_ancient_mogu_bell', 18291),
(218746, 0, 13710, 'Hydra Exit', '', '', '', 1375, 32, 0.619047, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658),
(218805, 3, 10317, 'Cache of Ancient Treasures', '', '', '', 35, 0, 3.5, 0, 0, 0, 0, 0, 0, 57, 218805, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 18019),
(218806, 3, 10317, 'Cache of Ancient Treasures', '', '', '', 35, 0, 3.5, 0, 0, 0, 0, 0, 0, 57, 218806, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 18019),
(218807, 3, 10317, 'Cache of Ancient Treasures', '', '', '', 35, 0, 3.5, 0, 0, 0, 0, 0, 0, 57, 218807, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 18019),
(218808, 3, 10317, 'Cache of Ancient Treasures', '', '', '', 35, 0, 3.5, 0, 0, 0, 0, 0, 0, 57, 218808, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 18019);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Gameobjects spawns
INSERT INTO `gameobject` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
(@OGUID+30, 218723, 1098, 0, 0, 760, 1, 6285.99, 4923.5, -161.133, 5.26395, 0, 0, 0, 1, 86400, 255, 1, 0, NULL),
(@OGUID+31, 218723, 1098, 0, 0, 760, 1, 6106.54, 4670.81, -172.214, 0.220267, 0, 0, 0, 1, 86400, 255, 1, 0, NULL),
(@OGUID+32, 218723, 1098, 0, 0, 760, 1, 6496.56, 4740.43, -172.019, 0.0441186, 0, 0, 0, 1, 86400, 255, 1, 0, NULL),
(@OGUID+33, 218721, 1098, 0, 0, 760, 1, 6270.81, 4602.14, -194.966, 0, 0, 0, 0, 1, 86400, 255, 1, 0, NULL),
(@OGUID+34, 218746, 1098, 0, 0, 760, 1, 6229.31, 4481.72, -173.37, 3.97902, 0, 0, 1, -4.37114E-8, 86400, 0, 1, 0, NULL),
(@OGUID+35, 218805, 1098, 0, 0, 8, 1, 6390.89, 4546.46, -209.102, 5.10657, 0, 0, 0.554954, -0.831881, -1, 0, 1, 1, NULL),
(@OGUID+36, 218806, 1098, 0, 0, 35, 1, 6390.89, 4546.46, -209.102, 5.10657, 0, 0, 0.554954, -0.831881, -1, 0, 1, 1, NULL),
(@OGUID+37, 218807, 1098, 0, 0, 16, 1, 6390.89, 4546.46, -209.102, 5.10657, 0, 0, 0.554954, -0.831881, -1, 0, 1, 1, NULL),
(@OGUID+38, 218808, 1098, 0, 0, 64, 1, 6390.89, 4546.46, -209.102, 5.10657, 0, 0, 0.554954, -0.831881, -1, 0, 1, 1, NULL);
/*---------------------------------------------------------------------------------------------------------------*/
/* END Megaera */





/* BEGIN Ji-Kun */
/*---------------------------------------------------------------------------------------------------------------*/
-- Spell Scripts
DELETE FROM `spell_script_names` WHERE  `spell_id` = 140094;
INSERT INTO `spell_script_names` SET `spell_id` = 140094 ,`ScriptName` = 'spell_infected_talons';

UPDATE `creature_template` SET `ScriptName` = "young_hatchling_jikun" WHERE `entry` = 68192;
UPDATE `creature_template` SET `ScriptName` = "jikun_beam_target" WHERE `entry` = 68208;
UPDATE `creature_template` SET `ScriptName` = "egg_of_jikun" WHERE `entry` IN (68194,68202,69628);
UPDATE `creature_template` SET `ScriptName` = "boss_jikun" WHERE entry = 69712;
UPDATE `creature_template` SET `ScriptName` = "npc_juvenile" WHERE `entry`= 70095;
UPDATE `creature_template` SET `ScriptName` = "npc_jikun_feed" WHERE `entry` IN (68178,70130);
UPDATE `creature_template` SET `ScriptName` = "pool_of_feed_dmg" WHERE `entry` IN (68188,70216);
UPDATE `creature_template` SET `ScriptName` = "npc_jikun_teleport" WHERE `entry`= 70640;
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Templates
REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES 
(63420, 150124, 0, 0, 0, 0, 11686, 0, 0, 0, 'SLG Generic MoP', '', '', 0, 90, 90, 4, 14, 14, 0, 1, 1.14286, 1, 1, 9838, 14331, 0, 42296, 3, 2000, 2000, 1, 33555200, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 17826816, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 16048),
(68136, 0, 0, 0, 0, 0, 31379, 0, 0, 0, 'Back Head Spawner', '', '', 0, 90, 90, 4, 16, 16, 0, 1, 1.14286, 1, 0, 9839, 14339, 0, 42299, 1, 2000, 2000, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1091568704, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 7, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614),
(68178, 0, 0, 0, 0, 0, 48142, 0, 0, 0, 'Feed', '', '', 0, 1, 1, 4, 35, 35, 0, 1, 1.14286, 1, 0, 9839, 14339, 0, 42299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1074790416, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'npc_jikun_feed', 17614),
(68188, 0, 0, 0, 0, 0, 46710, 42428, 0, 0, 'Feed Pool', '', '', 0, 1, 1, 4, 35, 35, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1074790400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'pool_of_feed_dmg', 17614),
(68192, 0, 0, 0, 0, 0, 46680, 0, 0, 0, 'Hatchling', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'young_hatchling_jikun', 17614),
(68193, 0, 0, 0, 0, 0, 46681, 0, 0, 0, 'Fledgling', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 100, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614),
(68194, 0, 0, 0, 0, 0, 46683, 0, 0, 0, 'Young Egg of Ji-Kun', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'egg_of_jikun', 17614),
(68202, 0, 0, 0, 0, 0, 47701, 0, 0, 0, 'Ji-Kun Fledgling\'s Egg', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 100, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'egg_of_jikun', 17614),
(68208, 0, 0, 0, 0, 0, 46675, 0, 0, 0, 'Ji-Kun Dummy', '', '', 0, 1, 1, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 100, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 128, 'jikun_beam_target', 17614),
(68221, 0, 0, 0, 0, 0, 15963, 0, 0, 0, 'Bore Worm', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1075839048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 14, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614),
(68222, 0, 0, 0, 0, 0, 47984, 47985, 47986, 47987, 'Bow Fly Swarm', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2762, 0, 0, '', 0, 3, 1, 28, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614),
(68248, 0, 0, 0, 0, 0, 46713, 0, 0, 0, 'Corpse Spider', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'SmartAI', 0, 3, 1, 32, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614),
(68249, 0, 0, 0, 0, 0, 46710, 0, 0, 0, 'Web', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 0, 9839, 14339, 0, 42299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1074790400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2747, 0, 0, '', 0, 3, 1, 100, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614),
(68262, 0, 0, 0, 0, 0, 46710, 28016, 38497, 0, 'Corpse Spider', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1074790400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 100, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614),
(68266, 0, 0, 0, 0, 0, 46710, 0, 0, 0, 'Web', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1074790400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2741, 0, 0, '', 0, 3, 1, 100, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614),
(69626, 0, 0, 0, 0, 0, 46710, 0, 0, 0, 'Incubater', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 0, 9839, 14339, 0, 42299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1074790400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614),
(69628, 0, 0, 0, 0, 0, 47702, 0, 0, 0, 'Mature Egg of Ji-Kun', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'egg_of_jikun', 17614),
(69712, 0, 0, 0, 0, 0, 46675, 0, 0, 0, 'Ji-Kun', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 3, 57000, 70000, 0, 120000, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000000, 3100000, '', 0, 3, 1, 560, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 667893759, 0, 'boss_jikun', 17614),
(69836, 0, 0, 0, 0, 0, 47709, 0, 0, 0, 'Juvenile', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 100, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'npc_juvenile', 17614),
(70095, 0, 0, 0, 0, 0, 47709, 0, 0, 0, 'Juvenile', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'npc_juvenile', 17614),
(70130, 0, 0, 0, 0, 0, 48210, 0, 0, 0, 'Feed', '', '', 0, 1, 1, 4, 35, 35, 0, 1, 1.14286, 1, 0, 9839, 14339, 0, 42299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1074790400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'npc_jikun_feed', 17614),
(70216, 0, 0, 0, 0, 0, 46710, 0, 0, 0, 'Feed Pool', '', '', 0, 1, 1, 4, 35, 35, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1074790400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'pool_of_feed_dmg', 17614),
(70294, 0, 0, 0, 0, 0, 47984, 47985, 47986, 47987, 'Bow Fly', '', '', 0, 92, 92, 4, 14, 14, 0, 4, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 18875464, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 28, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614),
(70640, 0, 0, 0, 0, 0, 46675, 0, 0, 0, 'Ji-Kun Area Trigger Dummy [DNT]', '', '', 0, 1, 1, 4, 35, 35, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 100, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 128, 'npc_jikun_teleport', 17614),
(73193, 0, 0, 0, 0, 0, 47983, 0, 0, 0, 'Hatchling', '', '', 0, 92, 92, 4, 14, 14, 0, 1, 1.14286, 1, 1, 11839, 17339, 0, 45299, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 100, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17614),

(@RAID_DIFF_10N + 69712, 0, 0, 0, 0, 0, 46675, 0, 0, 0, 'Ji-Kun', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 3, 57000, 70000, 0, 120000, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000000, 3100000, '', 0, 3, 1, 560, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 667893759, 0, 'boss_jikun', 17614),

(@RAID_DIFF_10H + 69712, 0, 0, 0, 0, 0, 46675, 0, 0, 0, 'Ji-Kun', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 3, 57000, 70000, 0, 120000, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000000, 3100000, '', 0, 3, 1, 560, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 667893759, 0, 'boss_jikun', 17614),

(@RAID_DIFF_25N + 69712, 0, 0, 0, 0, 0, 46675, 0, 0, 0, 'Ji-Kun', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 3, 57000, 70000, 0, 120000, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000000, 3100000, '', 0, 3, 1, 560, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 667893759, 0, 'boss_jikun', 17614),

(@RAID_DIFF_25H + 69712, 0, 0, 0, 0, 0, 46675, 0, 0, 0, 'Ji-Kun', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 3, 57000, 70000, 0, 120000, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000000, 3100000, '', 0, 3, 1, 560, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 667893759, 0, 'boss_jikun', 17614),

(@RAID_DIFF_25R + 69712, 0, 0, 0, 0, 0, 46675, 0, 0, 0, 'Ji-Kun', '', '', 0, 93, 93, 4, 14, 14, 0, 1, 1.14286, 1, 3, 57000, 70000, 0, 120000, 1, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097260, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000000, 3100000, '', 0, 3, 1, 560, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 667893759, 0, 'boss_jikun', 17614);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Updates
-- Creature Trash Mobs Damage, HP, LootId
-- Base HP Gamepedia Raid 25N
-- 25R -10%
-- 25H +25%
-- 10N (BASE HP: 25 = X : 10)
-- 10H=(10N+25%)

-- Immunity and bind bosses
UPDATE creature_template SET flags_extra = flags_extra | 1, mechanic_immune_mask = 667893759 WHERE entry IN 
(69712,3169712,3269712,3369712,3469712,3569712);

-- Difficulty entries bosses and Trash Mobs
UPDATE creature_template SET difficulty_entry_3 = @RAID_DIFF_10N + entry, difficulty_entry_4 = @RAID_DIFF_25N + entry, difficulty_entry_5 = @RAID_DIFF_10H + entry, difficulty_entry_6 = @RAID_DIFF_25H + entry, difficulty_entry_7 = @RAID_DIFF_25R + entry WHERE entry IN 
(69712);

-- Jin-Kun
UPDATE creature_template SET LootId = 0, dmg_multiplier = 11, Health_Mod = 244236720   / @HP_MOD_93 WHERE entry = @RAID_DIFF_10N + 69712;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 12, Health_Mod = 366355072   / @HP_MOD_93 WHERE entry = @RAID_DIFF_10H + 69712;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 14, Health_Mod = 732710144   / @HP_MOD_93 WHERE entry = @RAID_DIFF_25N + 69712;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 15, Health_Mod = 1099065216 / @HP_MOD_93 WHERE entry = @RAID_DIFF_25H + 69712;
UPDATE creature_template SET LootId = 0, dmg_multiplier = 13, Health_Mod = 402990592   / @HP_MOD_93 WHERE entry = @RAID_DIFF_25R + 69712;
/*---------------------------------------------------------------------------------------------------------------*
/
/*---------------------------------------------------------------------------------------------------------------*/
-- Trash Mobs SAI
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Jin-Kun Loots
-- 10 Normal
-- 10 Heroic
-- 25 Normal
-- 25 Heroic
-- 25 LFR
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Spawn
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`npcflag2`,`unit_flags`,`unit_flags2`,`dynamicflags`,`isActive`,`protec_anti_doublet`) VALUES
(@CGUID+615, 70640, 1098, 0, 0, 248, 1, 0, 0, 6099.37, 4236.19, -3.7873, 4.17527, 86400, 0, 0, 100, 0, 0, 0, 0, 33554432, 0, 0, 0, NULL),
(@CGUID+616, 68202, 1098, 0, 0, 248, 1, 0, 0, 6147, 4318, -31.1, 1.08972, 86400, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, NULL),
(@CGUID+617, 69712, 1098, 0, 0, 248, 1, 0, 0, 6192.68, 4268.42, -70.5738, 1.08972, 604800, 0, 0, 244236720, 0, 0, 0, 0, 0, 0, 0, 0, NULL);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Speeches
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Gameobjects templates
REPLACE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `AIName`, `ScriptName`, `WDBVerified`) VALUES 
(218543, 22, 13599, 'Feather of Ji-Kun', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 140013, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17658);
/*---------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------*/
-- Gameobjects spawns
INSERT INTO `gameobject` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
(@OGUID+39, 218543, 1098, 0, 0, 248, 1, 6132.86, 4343.46, -31.863, 2.739, 0, 0, 0, 0, -604800, 0, 0, 0, NULL);
/*---------------------------------------------------------------------------------------------------------------*/
/* END Ji-Kun */





/* Other Stuff */
/*---------------------------------------------------------------------------------------------------------------*/
-- Creature Model
REPLACE INTO `creature_model_info` (`modelid`, `bounding_radius`, `combat_reach`, `gender`) VALUES
(169, 0.903, 2, 2),
(958, 1.122, 0.9, 2),
(1126, 2, 2, 2),
(11686, 0.5, 1, 2),
(15963, 1.5, 2.5, 2),
(22263, 0.625, 2.7, 0),
(22307, 0.5902774, 2.55, 0),
(22308, 0.5902774, 2.55, 0),
(23767, 0.39, 1, 2),
(27056, 0.465, 1.5, 0),
(27510, 0.75, 2.5, 2),
(28016, 0.325, 0.65, 2),
(28118, 0.3672, 1.8, 1),
(28120, 0.306, 1.5, 0),
(28133, 1.75, 5.25, 2),
(31379, 0.5, 1, 2),
(32832, 5, 10, 2),
(39633, 0.5, 1, 2),
(42055, 1.95, 5, 2),
(42413, 2, 5, 0),
(42720, 0.8, 1.2, 0), 
(42722, 0.8, 1.2, 0), 
(44690, 2.0175, 2.25, 2),
(44691, 2.69, 3, 2),
(45735, 0.45, 2.25, 0),
(45738, 0.45, 2.25, 0),
(46559, 0.3, 21, 2),
(46675, 4.5, 13.5, 2),
(46683, 1.5, 5, 2),
(46710, 0.5, 1, 2),
(46713, 0.62, 4, 2),
(46803, 0.383, 1.5, 1),
(47189, 6, 29, 2),
(47229, 3, 4.5, 0),
(47230, 1.041666, 4.5, 0),
(47325, 0.3, 15, 2),
(47331, 0.868055, 3.75, 0),
(47332, 0.868055, 3.75, 0),
(47333, 0.868055, 3.75, 0),
(47334, 0.868055, 3.75, 0),
(47335, 0.5902774, 2.55, 0),
(47336, 0.5902774, 2.55, 0),
(47338, 0.6249996, 2.7, 0),
(47341, 2.5, 1.5, 0),
(47378, 0.765, 3.75, 1),
(47414, 1.52778, 25, 2),
(47442, 0.612, 3, 0),
(47443, 0.612, 3, 0),
(47446, 0.465, 1.5, 0),
(47475, 0.765, 3.75, 0),
(47494, 2.088, 6, 2),
(47495, 2.088, 6, 2),
(47499, 0.93, 6, 2),
(47502, 2.61, 7.5, 2),
(47505, 3, 4.5, 0),
(47506, 3, 4.5, 0),
(47507, 1.566, 4.5, 2),
(47527, 40, 16, 2),
(47528, 1.305, 3.75, 2),
(47536, 1.305, 3.75, 2),
(47552, 0.3, 10, 2),
(47556, 0.62, 2, 0),
(47557, 0.62, 2, 0),
(47558, 0.465, 1.5, 0),
(47559, 0.459, 2.25, 1),
(47561, 0.459, 2.25, 1),
(47693, 1.5, 1.5, 2),
(47698, 0.5, 1, 2),
(47702, 1.875, 6.25, 2), 
(47730, 2.5, 3.75, 1),
(47760, 1.827, 10.5, 0),
(47761, 1.305, 3.75, 0),
(47766, 0.868055, 3.75, 0),
(47767, 0.868055, 3.75, 0),
(47769, 0.5902774, 2.55, 0),
(47775, 0.5202, 2.55, 0),
(47781, 0.5902774, 2.55, 0),
(47782, 0.5202, 2.55, 0),
(47783, 0.4896, 2.4, 1),
(47785, 10.5, 17.5, 2),
(47786, 10.5, 17.5, 2),
(47787, 0, 0, 2),
(47788, 10.5, 17.5, 2),
(47853, 0.520833, 2.25, 0),
(47885, 1.566, 4.5, 0),
(47941, 1.044, 3, 2),
(47951, 1.224, 6, 0),
(47984, 0.0465, 0.15, 0),
(47985, 0.0465, 0.15, 0),
(47986, 0.0465, 0.15, 0),
(47987, 0.0465, 0.15, 0),
(47998, 1.86, 12, 2),
(48048, 4, 1.5, 2),
(48049, 2.088, 6, 2),
(48052, 1.75, 7.5, 2),
(48113, 0.39, 1, 2),
(48121, 1.566, 4.5, 0),
(48160, 2, 2.5, 2),
(48177, 0.93, 9, 2),
(48181, 4.5, 4.5, 0),
(48183, 0.5, 1.470588, 2),
(48184, 0.5, 1.415094, 2),
(48185, 2.5, 1.315789, 2),
(48242, 1.52778, 25, 2),
(48244, 1.52778, 25, 2),
(48245, 0, 0, 2);

UPDATE `creature_model_info` SET `bounding_radius`='9',`combat_reach`='9' WHERE `modelid` IN 
(47414,47415,47416,47417,48238,48239,48240,48241,48242,48243,48244,48245);

-- Creatures Bytes (Sniffed datas)
DELETE FROM `creature_template_bytes` WHERE `entry` IN (54020,69135,69182,69184,69185,70056,70060,70137,70246,70341,70441,73400);
INSERT INTO `creature_template_bytes` (`entry`, `index`, `bytes`) VALUES
(54020,0,0x3000000),
(69135,0,0x2000000),
(69182,0,0x3000000),
(69184,0,0x1),
(69185,0,0x1),
(70056,0,0x3000000),
(70060,0,0x3000000),
(70137,0,0x3000000),
(70246,0,0x3000000),
(70341,0,0x3000000),
(70441,0,0x3000000),
(73400,0,0x3000000);

-- Shared Boss Loots Normal
DELETE FROM `reference_loot_template` WHERE `entry` IN (95498,95877,96621);
INSERT INTO `reference_loot_template` VALUES
(95498,95498,0,1,10,1,1),
(95498,95507,0,1,10,1,1),
(95498,95502,0,1,10,1,1),
(95498,95501,0,1,10,1,1),
(95498,95505,0,1,10,1,1),
(95498,95499,0,1,10,1,1),
(95498,95500,0,1,10,1,1),
(95498,95503,0,1,10,1,1),
(95498,95506,0,1,10,1,1),
(95498,97126,0,1,10,1,1),
(95498,95504,0,1,10,1,1),
(95498,95516,0,1,10,1,1),
(95498,95061,0,1,10,1,1),
(95498,95067,0,1,10,1,1),
(95498,95066,0,1,10,1,1),
(95498,95065,0,1,10,1,1),
(95498,95062,0,1,10,1,1),
(95498,95060,0,1,10,1,1),
(95498,95064,0,1,10,1,1),
(95498,95068,0,1,10,1,1),
(95498,95063,0,1,10,1,1),
(95498,95069,0,1,10,1,1),

-- Shared Boss Loots Heroic
(96621,96621,0,1,10,1,1),
(96621,96606,0,1,10,1,1),
(96621,96620,0,1,10,1,1),
(96621,96604,0,1,10,1,1),
(96621,96619,0,1,10,1,1),
(96621,96602,0,1,10,1,1),
(96621,96611,0,1,10,1,1),
(96621,96603,0,1,10,1,1),
(96621,96610,0,1,10,1,1),
(96621,97127,0,1,10,1,1),
(96621,96605,0,1,10,1,1),
(96621,96622,0,1,10,1,1),
(96621,96607,0,1,10,1,1),
(96621,96609,0,1,10,1,1),
(96621,96608,0,1,10,1,1),
(96621,96612,0,1,10,1,1),
(96621,96613,0,1,10,1,1),
(96621,96614,0,1,10,1,1),
(96621,96615,0,1,10,1,1),
(96621,96618,0,1,10,1,1),
(96621,96617,0,1,10,1,1),
(96621,96616,0,1,10,1,1),

-- Shared Boss Loots LFR
(95877,95877,0,1,10,1,1),
(95877,95862,0,1,10,1,1),
(95877,95876,0,1,10,1,1),
(95877,95860,0,1,10,1,1),
(95877,95875,0,1,10,1,1),
(95877,95858,0,1,10,1,1),
(95877,95867,0,1,10,1,1),
(95877,95859,0,1,10,1,1),
(95877,95866,0,1,10,1,1),
(95877,97129,0,1,10,1,1),
(95877,95861,0,1,10,1,1),
(95877,95878,0,1,10,1,1),
(95877,95863,0,1,10,1,1),
(95877,95865,0,1,10,1,1),
(95877,95864,0,1,10,1,1),
(95877,95868,0,1,10,1,1),
(95877,95869,0,1,10,1,1),
(95877,95870,0,1,10,1,1),
(95877,95871,0,1,10,1,1),
(95877,95874,0,1,10,1,1),
(95877,95873,0,1,10,1,1),
(95877,95872,0,1,10,1,1);

-- Trash Mobs Loots Normal
DELETE FROM `creature_loot_template` WHERE `entry` IN (69388,6938801,6938802);
INSERT INTO `creature_loot_template` VALUES
(69388, 95202, 0.005, 1, 0, 1, 1),
(69388, 95203, 0.005, 1, 0, 1, 1),
(69388, 95204, 0.005, 1, 0, 1, 1),
(69388, 95205, 0.005, 1, 0, 1, 1),
(69388, 95206, 0.005, 1, 0, 1, 1),
(69388, 95207, 0.005, 1, 0, 1, 1),
(69388, 95208, 0.005, 1, 0, 1, 1),
(69388, 95209, 0.005, 1, 0, 1, 1),
(69388, 95210, 0.005, 1, 0, 1, 1),
(69388, 95211, 0.005, 1, 0, 1, 1),
(69388, 95212, 0.005, 1, 0, 1, 1),
(69388, 95213, 0.005, 1, 0, 1, 1),
(69388, 95214, 0.005, 1, 0, 1, 1),
(69388, 95215, 0.005, 1, 0, 1, 1),
(69388, 95216, 0.005, 1, 0, 1, 1),
(69388, 95217, 0.005, 1, 0, 1, 1),
(69388, 95218, 0.005, 1, 0, 1, 1),
(69388, 95219, 0.005, 1, 0, 1, 1),
(69388, 95220, 0.005, 1, 0, 1, 1),
(69388, 95221, 0.005, 1, 0, 1, 1),
(69388, 95222, 0.005, 1, 0, 1, 1),
(69388, 95223, 0.005, 1, 0, 1, 1),
(69388, 95224, 0.005, 1, 0, 1, 1),
(69388, 94125, 0.5, 1, 0, 1, 1),
(69388, 95467, 0.1, 1, 0, 1, 1),
(69388, 95468, 0.03, 1, 0, 1, 1),
(69388, 100864, 0.05, 1, 0, 1, 1),
(69388, 100865, 0.1, 1, 0, 1, 1),
(69388, 100910, 0.02, 1, 0, 1, 1),
(69388, 102513, 0.02, 1, 0, 1, 1),
(69388, 102534, 0.07, 1, 0, 1, 1),
(69388, 104197, 0.02, 1, 0, 1, 1),
(69388, 104212, 0.02, 1, 0, 1, 1),
(69388, 104214, 0.02, 1, 0, 1, 1),
(69388, 104218, 0.02, 1, 0, 1, 1),
(69388, 104221, 0.02, 1, 0, 1, 1),
(69388, 104234, 0.02, 1, 0, 1, 1),
(69388, 104242, 0.02, 1, 0, 1, 1),
(69388, 72988, 39, 1, 0, 1, 6),
(69388, 89112, 12, 1, 0, 1, 1),
(69388, 81413, 17, 1, 0, 1, 1),
(69388, 81414, 8, 1, 0, 1, 1),
(69388, 88567, 2, 1, 0, 1, 1),
(69388, 95411, 0.9, 1, 0, 1, 1),
(69388, 95408, 0.7, 1, 0, 1, 1),
(69388, 82284, 1.2, 1, 0, 1, 1),
(69388, 82264, 1.2, 1, 0, 1, 1),
(69388, 82280, 1.2, 1, 0, 1, 1),
(69388, 82256, 1.2, 1, 0, 1, 1),
(69388, 82274, 1.2, 1, 0, 1, 1),
(69388, 82285, 1.2, 1, 0, 1, 1),
(69388, 82251, 1.2, 1, 0, 1, 1),
(69388, 82249, 1.2, 1, 0, 1, 1),
(69388, 82241, 1.2, 1, 0, 1, 1),
(69388, 82243, 1.2, 1, 0, 1, 1),
(69388, 94059, 1.2, 1, 0, 1, 1),
(69388, 94089, 1.2, 1, 0, 1, 1),
(69388, 82257, 1.2, 1, 0, 1, 1),
(69388, 82261, 1.2, 1, 0, 1, 1),
(69388, 82244, 1.2, 1, 0, 1, 1),

-- Trash Mobs Loots Heroic
(6938801, 96705, 0.005, 1, 0, 1, 1),
(6938801, 96706, 0.005, 1, 0, 1, 1),
(6938801, 96709, 0.005, 1, 0, 1, 1),
(6938801, 96707, 0.005, 1, 0, 1, 1),
(6938801, 96715, 0.005, 1, 0, 1, 1),
(6938801, 96714, 0.005, 1, 0, 1, 1),
(6938801, 96710, 0.005, 1, 0, 1, 1),
(6938801, 96703, 0.005, 1, 0, 1, 1),
(6938801, 96716, 0.005, 1, 0, 1, 1),
(6938801, 96717, 0.005, 1, 0, 1, 1),
(6938801, 96704, 0.005, 1, 0, 1, 1),
(6938801, 96711, 0.005, 1, 0, 1, 1),
(6938801, 96720, 0.005, 1, 0, 1, 1),
(6938801, 96719, 0.005, 1, 0, 1, 1),
(6938801, 96718, 0.005, 1, 0, 1, 1),
(6938801, 96723, 0.005, 1, 0, 1, 1),
(6938801, 96722, 0.005, 1, 0, 1, 1),
(6938801, 96712, 0.005, 1, 0, 1, 1),
(6938801, 96702, 0.005, 1, 0, 1, 1),
(6938801, 96708, 0.005, 1, 0, 1, 1),
(6938801, 96713, 0.005, 1, 0, 1, 1),
(6938801, 96724, 0.005, 1, 0, 1, 1),
(6938801, 96721, 0.005, 1, 0, 1, 1),
(6938801, 94125, 0.5, 1, 0, 1, 1),
(6938801, 95467, 0.1, 1, 0, 1, 1),
(6938801, 95468, 0.03, 1, 0, 1, 1),
(6938801, 100864, 0.05, 1, 0, 1, 1),
(6938801, 100865, 0.1, 1, 0, 1, 1),
(6938801, 100910, 0.02, 1, 0, 1, 1),
(6938801, 102513, 0.02, 1, 0, 1, 1),
(6938801, 102534, 0.07, 1, 0, 1, 1),
(6938801, 104197, 0.02, 1, 0, 1, 1),
(6938801, 104212, 0.02, 1, 0, 1, 1),
(6938801, 104214, 0.02, 1, 0, 1, 1),
(6938801, 104218, 0.02, 1, 0, 1, 1),
(6938801, 104221, 0.02, 1, 0, 1, 1),
(6938801, 104234, 0.02, 1, 0, 1, 1),
(6938801, 104242, 0.02, 1, 0, 1, 1),
(6938801, 72988, 39, 1, 0, 1, 6),
(6938801, 89112, 12, 1, 0, 1, 1),
(6938801, 81413, 17, 1, 0, 1, 1),
(6938801, 81414, 8, 1, 0, 1, 1),
(6938801, 88567, 2, 1, 0, 1, 1),
(6938801, 95411, 0.9, 1, 0, 1, 1),
(6938801, 95408, 0.7, 1, 0, 1, 1),
(6938801, 82284, 1.2, 1, 0, 1, 1),
(6938801, 82264, 1.2, 1, 0, 1, 1),
(6938801, 82280, 1.2, 1, 0, 1, 1),
(6938801, 82256, 1.2, 1, 0, 1, 1),
(6938801, 82274, 1.2, 1, 0, 1, 1),
(6938801, 82285, 1.2, 1, 0, 1, 1),
(6938801, 82251, 1.2, 1, 0, 1, 1),
(6938801, 82249, 1.2, 1, 0, 1, 1),
(6938801, 82241, 1.2, 1, 0, 1, 1),
(6938801, 82243, 1.2, 1, 0, 1, 1),
(6938801, 94059, 1.2, 1, 0, 1, 1),
(6938801, 94089, 1.2, 1, 0, 1, 1),
(6938801, 82257, 1.2, 1, 0, 1, 1),
(6938801, 82261, 1.2, 1, 0, 1, 1),
(6938801, 82244, 1.2, 1, 0, 1, 1),

-- Trash Mobs Loots LFR
(6938802, 95961, 0.005, 1, 0, 1, 1),
(6938802, 95962, 0.005, 1, 0, 1, 1),
(6938802, 95965, 0.005, 1, 0, 1, 1),
(6938802, 95963, 0.005, 1, 0, 1, 1),
(6938802, 95971, 0.005, 1, 0, 1, 1),
(6938802, 95970, 0.005, 1, 0, 1, 1),
(6938802, 95966, 0.005, 1, 0, 1, 1),
(6938802, 95959, 0.005, 1, 0, 1, 1),
(6938802, 95972, 0.005, 1, 0, 1, 1),
(6938802, 95973, 0.005, 1, 0, 1, 1),
(6938802, 95960, 0.005, 1, 0, 1, 1),
(6938802, 95967, 0.005, 1, 0, 1, 1),
(6938802, 95976, 0.005, 1, 0, 1, 1),
(6938802, 95975, 0.005, 1, 0, 1, 1),
(6938802, 95974, 0.005, 1, 0, 1, 1),
(6938802, 95979, 0.005, 1, 0, 1, 1),
(6938802, 95978, 0.005, 1, 0, 1, 1),
(6938802, 95968, 0.005, 1, 0, 1, 1),
(6938802, 95958, 0.005, 1, 0, 1, 1),
(6938802, 95964, 0.005, 1, 0, 1, 1),
(6938802, 95969, 0.005, 1, 0, 1, 1),
(6938802, 95980, 0.005, 1, 0, 1, 1),
(6938802, 95977, 0.005, 1, 0, 1, 1),
(6938802, 94125, 0.1, 1, 0, 1, 1),
(6938802, 95467, 0.1, 1, 0, 1, 1),
(6938802, 95468, 0.03, 1, 0, 1, 1),
(6938802, 100864, 0.05, 1, 0, 1, 1),
(6938802, 100865, 0.1, 1, 0, 1, 1),
(6938802, 100910, 0.02, 1, 0, 1, 1),
(6938802, 102513, 0.02, 1, 0, 1, 1),
(6938802, 102534, 0.07, 1, 0, 1, 1),
(6938802, 104197, 0.02, 1, 0, 1, 1),
(6938802, 104212, 0.02, 1, 0, 1, 1),
(6938802, 104214, 0.02, 1, 0, 1, 1),
(6938802, 104218, 0.02, 1, 0, 1, 1),
(6938802, 104221, 0.02, 1, 0, 1, 1),
(6938802, 104234, 0.02, 1, 0, 1, 1),
(6938802, 104242, 0.02, 1, 0, 1, 1),
(6938802, 72988, 39, 1, 0, 1, 6),
(6938802, 89112, 12, 1, 0, 1, 1),
(6938802, 81413, 17, 1, 0, 1, 1),
(6938802, 81414, 8, 1, 0, 1, 1),
(6938802, 88567, 2, 1, 0, 1, 1),
(6938802, 95411, 0.9, 1, 0, 1, 1),
(6938802, 95408, 0.7, 1, 0, 1, 1),
(6938802, 82284, 1.2, 1, 0, 1, 1),
(6938802, 82264, 1.2, 1, 0, 1, 1),
(6938802, 82280, 1.2, 1, 0, 1, 1),
(6938802, 82256, 1.2, 1, 0, 1, 1),
(6938802, 82274, 1.2, 1, 0, 1, 1),
(6938802, 82285, 1.2, 1, 0, 1, 1),
(6938802, 82251, 1.2, 1, 0, 1, 1),
(6938802, 82249, 1.2, 1, 0, 1, 1),
(6938802, 82241, 1.2, 1, 0, 1, 1),
(6938802, 82243, 1.2, 1, 0, 1, 1),
(6938802, 94059, 1.2, 1, 0, 1, 1),
(6938802, 94089, 1.2, 1, 0, 1, 1),
(6938802, 82257, 1.2, 1, 0, 1, 1),
(6938802, 82261, 1.2, 1, 0, 1, 1),
(6938802, 82244, 1.2, 1, 0, 1, 1);
/*---------------------------------------------------------------------------------------------------------------*/
SET FOREIGN_KEY_CHECKS = 1;