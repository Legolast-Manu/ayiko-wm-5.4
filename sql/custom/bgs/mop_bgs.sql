DELETE FROM trinity_string WHERE entry IN (773, 774, 775, 776);
INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`, `content_loc9`, `content_loc10`) VALUES 
(773, 'The battle begins in 2 minutes.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(774, 'The battle begins in 1 minute.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(775, 'The battle begins in 30 seconds.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(776, 'The battle has begun!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

DELETE FROM `creature_template` WHERE `entry` = 53194 OR `entry` = 71071 OR `entry` = 71073;
DELETE FROM `gameobject_template` WHERE `entry` = 188215 OR `entry` = 220163 OR `entry` = 220164 OR `entry` = 220165 OR `entry` = 220166 OR `entry` = 220174;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (53194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 38214, 0, 0, 0, 'Capture Point', '', 'interact', 0, 90, 90, 4, 0, 35, 35, 16777216, 1, 1.14286, 1, 0, 1, 3, 0, 0, 1, 0, 0, 1, 0, 67110912, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1628439554, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 17956);
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (71071, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 49028, 0, 0, 0, 'Alliance Mine Cart', '', '', 0, 0, 0, 0, 0, 35, 35, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 2000, 2000, 0, 33554570, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 0.02381, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 0, 0, 'npc_DG_cart', 17956);
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (71073, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 49028, 0, 0, 0, 'Horde Mine Cart', '', '', 0, 0, 0, 0, 0, 35, 35, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 2000, 2000, 0, 33554570, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 0.02381, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 180, 1, 0, 0, 'npc_DG_cart', 17956);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `unkInt32`, `AIName`, `ScriptName`, `WDBVerified`) VALUES (188215, 5, 7735, 'Collision', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 1);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `unkInt32`, `AIName`, `ScriptName`, `WDBVerified`) VALUES (220164, 36, 13817, 'Alliance Mine Cart', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 140876, 0, 1, 0, 1, 0, 10000, 5000, 220165, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17956);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `unkInt32`, `AIName`, `ScriptName`, `WDBVerified`) VALUES (220165, 37, 13817, 'Alliance Mine Cart', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17956);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `unkInt32`, `AIName`, `ScriptName`, `WDBVerified`) VALUES (220166, 36, 13817, 'Horde Mine Cart', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 141210, 0, 1, 0, 1, 0, 10000, 5000, 220174, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17956);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `unkInt32`, `AIName`, `ScriptName`, `WDBVerified`) VALUES (220174, 37, 13817, 'Horde Mine Cart', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 17956);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `unkInt32`, `AIName`, `ScriptName`, `WDBVerified`) VALUES (220163, 0, 10215, 'Gate', '', '', '', 0, 22, 1.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 1);
DELETE FROM `npc_spellclick_spells` WHERE npc_entry = 53194;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES (53194, 97388, 1, 0);
DELETE FROM `creature_model_info` WHERE modelid = 49028;
INSERT INTO `creature_model_info` (`modelid`, `modelid_other_gender`) VALUES ('49028', '0');
DELETE FROM `spell_script_names` WHERE ScriptName = 'mine_cart_DG_spell';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('140876', 'mine_cart_DG_spell');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('141210', 'mine_cart_DG_spell');

UPDATE gameobject_template SET faction = 1995 WHERE entry IN (220164, 220165);
UPDATE gameobject_template SET faction = 1997 WHERE entry IN (220166, 220174);

DELETE FROM trinity_string WHERE entry BETWEEN 1353 AND 1374;
INSERT INTO trinity_string (entry, content_default) VALUES
(1353, 'alliance'),
(1354, 'horde'),
(1355, 'Goblin Mine'),
(1356, 'Center Mine'),
(1357, 'Pandaren Mine'),
(1358, 'The %s has taken the %s'),
(1359, '$n has defended the %s'),
(1360, '$n has assualted the %s'),
(1361, '$n claims the %s! If left unchallenged, the %s will control it in 1 minute!'),
(1362, '$n has stolen gold from the horde'),
(1363, '$n has stolen gold from the alliance'),
(1364, '$n has dropped the horde gold'),
(1365, '$n has dropped the alliance gold'),
(1366, ''),
(1367, ''),
(1368, '$n has picked up the gold from the Horde'),
(1369, '$n has picked up the gold from the Alliance'),
(1370, ''),
(1371, 'The alliance flag has been respawned'),
(1372, 'The horde flag has been respawned'),
(1373, 'The alliance is near victory!'),
(1374, 'The horde is near victory!')

DELETE FROM creature_template WHERE entry = 400464;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES 
(400464, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 169, 16925, 0, 0, 'Trigger', '', NULL, 0, 60, 60, 0, 0, 35, 35, 0, 1, 0.992063, 0.5, 0, 104, 138, 0, 252, 4, 2000, 0, 1, 33555200, 0, 8, 0, 0, 0, 0, 72, 106, 26, 10, 16778240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 0, 1.35, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 130, '', 1);

delete from gameobject_template where entry in (400433, 400434);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `unkInt32`, `AIName`, `ScriptName`, `WDBVerified`) VALUES
(400433, 5, 11667, 'Mine Port', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 1),
(400434, 0, 850, 'Mine Gate', '', '', '', 114, 32, 1.45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 1);

delete from trinity_string where entry between 1404 and 1412;
INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES
(1404, 'The Alliance has taken control of a Mine Cart!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(1405, 'The Horde has taken control of a Mine Cart!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(1406, 'A Mine Cart has arrived from the center depot.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(1407, 'The Alliance has captured a Mine Cart!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(1408, 'The Horde has captured a Mine Cart!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(1409, 'The Southern Crossroads changed tracks!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(1410, 'The Northern Crossroads changed tracks!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(1411, 'The Alliance has gathered $6437W resources, and is near victory!', NULL, 'L''Alliance a rÐ¹unis $6437W ressources et est proche de la victoire !', '', '', '', '', '', ''),
(1412, 'The Horde has gathered $6438W resources, and is near victory!', NULL, 'La Horde a rÐ¹unis $6438W ressources et est proche de la victoire !', '', '', '', '', '', '');

INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES
(60379, 1, 736.548, 206.805, 319.452, 0, 0, 0, 0, 100, 0),
(60379, 2, 732.146, 214.183, 320.143, 0, 0, 0, 0, 100, 0),
(60379, 3, 729.917, 219.983, 320.318, 0, 0, 0, 0, 100, 0),
(60379, 4, 729, 226.652, 320.614, 0, 0, 0, 0, 100, 0),
(60379, 5, 724.547, 238.792, 320.744, 0, 0, 0, 0, 100, 0),
(60379, 6, 720.361, 244.472, 321.131, 0, 0, 0, 0, 100, 0),
(60379, 7, 715.631, 251.413, 321.044, 0, 0, 0, 0, 100, 0),
(60379, 8, 710.624, 261.126, 320.713, 0, 0, 0, 0, 100, 0),
(60379, 9, 708.395, 267.442, 320.558, 0, 0, 0, 0, 100, 0),
(60379, 10, 703.663, 277.893, 320.466, 0, 0, 0, 0, 100, 0),
(60379, 11, 696.197, 290.645, 320.586, 0, 0, 0, 0, 100, 0),
(60379, 12, 687.134, 300.56, 320.928, 0, 0, 0, 0, 100, 0),
(60379, 13, 675.485, 310.659, 321.804, 0, 0, 0, 0, 100, 0),
(60379, 14, 661.001, 323.359, 324.457, 0, 0, 0, 0, 100, 0),
(60379, 15, 650.727, 331.558, 327.443, 0, 0, 0, 0, 100, 0),
(60379, 16, 636.847, 343.759, 333.088, 0, 0, 0, 0, 100, 0),
(60379, 17, 631.819, 345.491, 335.22, 0, 0, 0, 0, 100, 0),
(60379, 18, 626.499, 345.424, 337.202, 0, 0, 0, 0, 100, 0),
(60379, 19, 601.887, 340.112, 344.394, 0, 0, 0, 0, 100, 0),
(60379, 20, 592.689, 339.309, 345.928, 0, 0, 0, 0, 100, 0),
(60379, 21, 580.856, 337.289, 346.313, 0, 0, 0, 0, 100, 0),
(60379, 22, 571.85, 336.916, 346.569, 0, 0, 0, 0, 100, 0),
(60378, 1, 739.813, 171.528, 319.366, 0, 0, 0, 0, 100, 0),
(60378, 2, 734.332, 160.725, 319.002, 0, 0, 0, 0, 100, 0),
(60378, 3, 729.062, 152.014, 319.669, 0, 0, 0, 0, 100, 0),
(60378, 4, 725.965, 146.377, 319.708, 0, 0, 0, 0, 100, 0),
(60378, 5, 721.922, 137.435, 319.589, 0, 0, 0, 0, 100, 0),
(60378, 6, 718.783, 127.548, 319.538, 0, 0, 0, 0, 100, 0),
(60378, 7, 717.631, 121.792, 320.194, 0, 0, 0, 0, 100, 0),
(60378, 8, 717.228, 114.484, 320.805, 0, 0, 0, 0, 100, 0),
(603780, 1, 715.531, 108.876, 320.43, 0, 0, 0, 0, 100, 0),
(603780, 2, 713.416, 104.558, 320.069, 0, 0, 0, 0, 100, 0),
(603780, 3, 711.746, 102.69, 319.64, 0, 0, 0, 0, 100, 0),
(603780, 4, 709.642, 100.842, 319.294, 0, 0, 0, 0, 100, 0),
(603780, 5, 708.178, 100.004, 319, 0, 0, 0, 0, 100, 0),
(603780, 6, 705.397, 98.7144, 318.396, 0, 0, 0, 0, 100, 0),
(603780, 7, 702.835, 97.6023, 317.759, 0, 0, 0, 0, 100, 0),
(603780, 8, 697.029, 96.5836, 316.458, 0, 0, 0, 0, 100, 0),
(603780, 9, 683.955, 95.0268, 313.616, 0, 0, 0, 0, 100, 0),
(603780, 10, 680.408, 94.2097, 312.532, 0, 0, 0, 0, 100, 0),
(603780, 11, 675.415, 92.4758, 310.801, 0, 0, 0, 0, 100, 0),
(603780, 12, 672.125, 91.0206, 309.622, 0, 0, 0, 0, 100, 0),
(603780, 13, 661.984, 85.0794, 305.739, 0, 0, 0, 0, 100, 0),
(603780, 14, 653.964, 82.3568, 303.486, 0, 0, 0, 0, 100, 0),
(603780, 15, 645.297, 81.5135, 300.888, 0, 0, 0, 0, 100, 0),
(603780, 16, 635.492, 80.9613, 298.712, 0, 0, 0, 0, 100, 0),
(603780, 17, 627.331, 80.2791, 298.515, 0, 0, 0, 0, 100, 0),
(603780, 18, 624.142, 79.9377, 298.447, 0, 0, 0, 0, 100, 0),
(6037800, 1, 716.947, 109.533, 320.581, 0, 0, 0, 0, 100, 0),
(6037800, 2, 717.158, 107.275, 320.577, 0, 0, 0, 0, 100, 0),
(6037800, 3, 717.906, 104.245, 320.656, 0, 0, 0, 0, 100, 0),
(6037800, 4, 719.95, 101.224, 320.88, 0, 0, 0, 0, 100, 0),
(6037800, 5, 723.498, 98.4203, 321.518, 0, 0, 0, 0, 100, 0),
(6037800, 6, 729.429, 95.95, 322.576, 0, 0, 0, 0, 100, 0),
(6037800, 7, 737.529, 93.7233, 325.273, 0, 0, 0, 0, 100, 0),
(6037800, 8, 748.309, 91.8956, 328.89, 0, 0, 0, 0, 100, 0),
(6037800, 9, 757.356, 89.9509, 331.927, 0, 0, 0, 0, 100, 0),
(6037800, 10, 767.714, 85.7086, 336.007, 0, 0, 0, 0, 100, 0),
(6037800, 11, 773.101, 82.8226, 338.373, 0, 0, 0, 0, 100, 0),
(6037800, 12, 779.201, 80.4309, 340.968, 0, 0, 0, 0, 100, 0),
(6037800, 13, 790.073, 78.9228, 344.601, 0, 0, 0, 0, 100, 0),
(6037800, 14, 794.567, 77.7644, 346.229, 0, 0, 0, 0, 100, 0),
(6037800, 15, 800.644, 76.0276, 348.893, 0, 0, 0, 0, 100, 0),
(6037800, 16, 803.136, 74.812, 349.561, 0, 0, 0, 0, 100, 0),
(6037800, 17, 806.982, 72.4138, 350.599, 0, 0, 0, 0, 100, 0),
(6037800, 18, 809.964, 70.1494, 351.493, 0, 0, 0, 0, 100, 0),
(6037800, 19, 815.473, 66.1927, 353.279, 0, 0, 0, 0, 100, 0),
(6037800, 20, 820.528, 63.7024, 354.622, 0, 0, 0, 0, 100, 0),
(6037800, 21, 824.337, 62.7775, 355.605, 0, 0, 0, 0, 100, 0),
(6037800, 22, 828.828, 62.3146, 356.733, 0, 0, 0, 0, 100, 0),
(6037800, 23, 833.417, 64.0282, 357.726, 0, 0, 0, 0, 100, 0),
(6037800, 24, 837.725, 67.406, 358.628, 0, 0, 0, 0, 100, 0),
(6037800, 25, 842.537, 70.2675, 359.654, 0, 0, 0, 0, 100, 0),
(6037800, 26, 846.829, 71.5992, 360.651, 0, 0, 0, 0, 100, 0),
(6037800, 27, 851.902, 71.3907, 361.48, 0, 0, 0, 0, 100, 0),
(6037800, 28, 856.336, 70.1419, 362.187, 0, 0, 0, 0, 100, 0),
(6037800, 29, 863.724, 66.298, 363.305, 0, 0, 0, 0, 100, 0),
(6037800, 30, 872.97, 58.4015, 364.351, 0, 0, 0, 0, 100, 0),
(6037800, 31, 878.484, 52.6757, 364.342, 0, 0, 0, 0, 100, 0),
(6037800, 32, 884.031, 45.2952, 363.741, 0, 0, 0, 0, 100, 0),
(6037800, 33, 888.753, 38.6393, 363.604, 0, 0, 0, 0, 100, 0),
(6037800, 34, 893.231, 33.0419, 363.857, 0, 0, 0, 0, 100, 0),
(6037800, 35, 894.109, 31.8979, 363.903, 0, 0, 0, 0, 100, 0),
(60380, 1, 762.477, 199.89, 319.583, 0, 0, 0, 0, 100, 0),
(60380, 2, 765.152, 202.359, 319.897, 0, 0, 0, 0, 100, 0),
(60380, 3, 767.123, 204.728, 319.969, 0, 0, 0, 0, 100, 0),
(60380, 4, 770.686, 210.459, 321.337, 0, 0, 0, 0, 100, 0),
(60380, 5, 773.192, 215.373, 322.098, 0, 0, 0, 0, 100, 0),
(60380, 6, 776.042, 221.111, 322.779, 0, 0, 0, 0, 100, 0),
(60380, 7, 778.197, 226.702, 323.663, 0, 0, 0, 0, 100, 0),
(60380, 8, 780.86, 235.445, 326.113, 0, 0, 0, 0, 100, 0),
(60380, 9, 785.627, 246.242, 329.131, 0, 0, 0, 0, 100, 0),
(60380, 10, 789.242, 252.73, 332.243, 0, 0, 0, 0, 100, 0),
(60380, 11, 794.445, 259.695, 336.467, 0, 0, 0, 0, 100, 0),
(60380, 12, 800.921, 267.035, 339.987, 0, 0, 0, 0, 100, 0),
(60380, 13, 807.956, 274.195, 342.559, 0, 0, 0, 0, 100, 0),
(60380, 14, 814.702, 280.892, 344.462, 0, 0, 0, 0, 100, 0),
(60380, 15, 821.345, 287.324, 345.47, 0, 0, 0, 0, 100, 0),
(60380, 16, 828.885, 293.503, 346.333, 0, 0, 0, 0, 100, 0),
(60380, 17, 834.802, 299.641, 346.814, 0, 0, 0, 0, 100, 0),
(603800, 1, 838.299, 303.651, 346.869, 0, 0, 0, 0, 100, 0),
(603800, 2, 841.428, 308.21, 346.912, 0, 0, 0, 0, 100, 0),
(603800, 3, 844.756, 314.221, 347.015, 0, 0, 0, 0, 100, 0),
(603800, 4, 847.569, 320.454, 347.146, 0, 0, 0, 0, 100, 0),
(603800, 5, 848.459, 325.685, 346.852, 0, 0, 0, 0, 100, 0),
(603800, 6, 848.214, 330.158, 347.114, 0, 0, 0, 0, 100, 0),
(603800, 7, 846.538, 340.906, 347.172, 0, 0, 0, 0, 100, 0),
(603800, 8, 845.114, 346.511, 347.103, 0, 0, 0, 0, 100, 0),
(603800, 9, 837.194, 362.91, 346.901, 0, 0, 0, 0, 100, 0),
(603800, 10, 833.275, 369.036, 346.856, 0, 0, 0, 0, 100, 0),
(603800, 11, 830.817, 374.828, 346.754, 0, 0, 0, 0, 100, 0),
(603800, 12, 826.987, 384.754, 346.599, 0, 0, 0, 0, 100, 0),
(603800, 13, 821.844, 399.795, 346.437, 0, 0, 0, 0, 100, 0),
(603800, 14, 818.729, 406.383, 347.448, 0, 0, 0, 0, 100, 0),
(603800, 15, 816.959, 413.134, 348.709, 0, 0, 0, 0, 100, 0),
(603800, 16, 816.845, 419.167, 349.887, 0, 0, 0, 0, 100, 0),
(603800, 17, 816.607, 426.345, 352.09, 0, 0, 0, 0, 100, 0),
(603800, 18, 814.784, 432.833, 354.16, 0, 0, 0, 0, 100, 0),
(603800, 19, 812.513, 438.346, 355.59, 0, 0, 0, 0, 100, 0),
(603800, 20, 810.702, 442.726, 356.727, 0, 0, 0, 0, 100, 0),
(603800, 21, 809.238, 448.982, 357.445, 0, 0, 0, 0, 100, 0),
(603800, 22, 807.673, 456.378, 358.182, 0, 0, 0, 0, 100, 0),
(603800, 23, 806.689, 465.029, 358.637, 0, 0, 0, 0, 100, 0),
(603800, 24, 804.531, 471.067, 358.976, 0, 0, 0, 0, 100, 0),
(603800, 25, 801.525, 476.126, 359.208, 0, 0, 0, 0, 100, 0),
(603800, 26, 798.482, 480.118, 359.192, 0, 0, 0, 0, 100, 0),
(603800, 27, 794.889, 484.412, 359.333, 0, 0, 0, 0, 100, 0),
(603800, 28, 789.291, 489.534, 359.266, 0, 0, 0, 0, 100, 0),
(603800, 29, 785.138, 493.456, 359.315, 0, 0, 0, 0, 100, 0),
(603800, 30, 783.313, 495.286, 359.321, 0, 0, 0, 0, 100, 0),
(603800, 31, 781.314, 497.576, 359.328, 0, 0, 0, 0, 100, 0),
(6038000, 1, 837.828, 302.35, 346.86, 0, 0, 0, 0, 100, 0),
(6038000, 2, 841.867, 303.567, 346.89, 0, 0, 0, 0, 100, 0),
(6038000, 3, 845.365, 303.699, 346.92, 0, 0, 0, 0, 100, 0),
(6038000, 4, 851.728, 303.484, 346.967, 0, 0, 0, 0, 100, 0),
(6038000, 5, 856.458, 302.576, 347.015, 0, 0, 0, 0, 100, 0),
(6038000, 6, 862.241, 300.385, 347.063, 0, 0, 0, 0, 100, 0),
(6038000, 7, 867.288, 297.62, 347.007, 0, 0, 0, 0, 100, 0),
(6038000, 8, 872.097, 293.145, 347.096, 0, 0, 0, 0, 100, 0),
(6038000, 9, 876.515, 288.124, 347.068, 0, 0, 0, 0, 100, 0),
(6038000, 10, 881.695, 279.532, 346.77, 0, 0, 0, 0, 100, 0),
(6038000, 11, 883.794, 274.745, 346.361, 0, 0, 0, 0, 100, 0),
(6038000, 12, 886.592, 269.298, 346.023, 0, 0, 0, 0, 100, 0),
(6038000, 13, 888.886, 262.982, 345.738, 0, 0, 0, 0, 100, 0),
(6038000, 14, 890.814, 258.054, 345.62, 0, 0, 0, 0, 100, 0),
(6038000, 15, 892.383, 252.914, 345.493, 0, 0, 0, 0, 100, 0),
(6038000, 16, 893.526, 247.444, 345.519, 0, 0, 0, 0, 100, 0),
(6038000, 17, 894.84, 241.692, 345.737, 0, 0, 0, 0, 100, 0),
(6038000, 18, 895.343, 236.129, 346.965, 0, 0, 0, 0, 100, 0),
(6038000, 19, 896.554, 230.642, 348.41, 0, 0, 0, 0, 100, 0),
(6038000, 20, 898.72, 224.644, 350.104, 0, 0, 0, 0, 100, 0),
(6038000, 21, 900.815, 219.293, 351.663, 0, 0, 0, 0, 100, 0),
(6038000, 22, 903.171, 213.808, 353.278, 0, 0, 0, 0, 100, 0),
(6038000, 23, 905.427, 207.215, 355.232, 0, 0, 0, 0, 100, 0),
(6038000, 24, 906.886, 201.117, 357.014, 0, 0, 0, 0, 100, 0),
(6038000, 25, 908.49, 194.291, 359.015, 0, 0, 0, 0, 100, 0),
(6038000, 26, 910.714, 182.699, 362.34, 0, 0, 0, 0, 100, 0),
(6038000, 27, 912.146, 174.145, 364.93, 0, 0, 0, 0, 100, 0),
(6038000, 28, 912.509, 170.198, 366.133, 0, 0, 0, 0, 100, 0),
(6038000, 29, 911.992, 164.993, 366.294, 0, 0, 0, 0, 100, 0),
(6038000, 30, 911.112, 160.172, 366.373, 0, 0, 0, 0, 100, 0),
(6038000, 31, 909.908, 155.99, 366.418, 0, 0, 0, 0, 100, 0),
(6038000, 32, 908.492, 152.966, 366.436, 0, 0, 0, 0, 100, 0),
(6038000, 33, 906.588, 149.528, 366.431, 0, 0, 0, 0, 100, 0),
(6038000, 34, 903.286, 144.345, 366.412, 0, 0, 0, 0, 100, 0),
(6038000, 35, 900.942, 140.511, 366.323, 0, 0, 0, 0, 100, 0),
(6038000, 36, 896.853, 136.453, 366.272, 0, 0, 0, 0, 100, 0),
(6038000, 37, 894.221, 133.493, 366.239, 0, 0, 0, 0, 100, 0),
(6038000, 38, 891.69, 130.435, 366.216, 0, 0, 0, 0, 100, 0),
(6038000, 39, 888.906, 126.811, 366.131, 0, 0, 0, 0, 100, 0),
(6038000, 40, 887.541, 124.667, 366.072, 0, 0, 0, 0, 100, 0),
(6038000, 41, 884.967, 119.464, 365.879, 0, 0, 0, 0, 100, 0),
(6038000, 42, 883.253, 112.678, 365.593, 0, 0, 0, 0, 100, 0),
(6038000, 43, 882.509, 107.984, 365.393, 0, 0, 0, 0, 100, 0),
(6038000, 44, 882.227, 104.39, 365.255, 0, 0, 0, 0, 100, 0),
(6038000, 45, 883.121, 98.8977, 365.043, 0, 0, 0, 0, 100, 0),
(6038000, 46, 885.631, 93.4154, 364.883, 0, 0, 0, 0, 100, 0),
(6038000, 47, 887.995, 90.2794, 364.806, 0, 0, 0, 0, 100, 0),
(6038000, 48, 889.728, 87.0795, 364.732, 0, 0, 0, 0, 100, 0),
(6038000, 49, 891.275, 81.9749, 364.601, 0, 0, 0, 0, 100, 0),
(6038000, 50, 891.068, 76.7711, 364.459, 0, 0, 0, 0, 100, 0),
(6038000, 51, 890.494, 72.6387, 364.551, 0, 0, 0, 0, 100, 0),
(6038000, 52, 890.062, 70.2632, 364.539, 0, 0, 0, 0, 100, 0),
(6038000, 53, 890.258, 62.7693, 364.479, 0, 0, 0, 0, 100, 0),
(6038000, 54, 891.335, 58.019, 364.319, 0, 0, 0, 0, 100, 0),
(6038000, 55, 892.202, 54.3413, 364.058, 0, 0, 0, 0, 100, 0),
(6038000, 56, 893.22, 51.1704, 363.776, 0, 0, 0, 0, 100, 0),
(6038000, 57, 893.571, 44.8804, 363.724, 0, 0, 0, 0, 100, 0),
(6038000, 58, 893.296, 40.7391, 363.68, 0, 0, 0, 0, 100, 0),
(6038000, 59, 893.186, 36.3522, 363.765, 0, 0, 0, 0, 100, 0),
(6038000, 60, 893.328, 33.1407, 363.858, 0, 0, 0, 0, 100, 0),
(6038000, 61, 894.002, 31.8534, 363.9, 0, 0, 0, 0, 100, 0);

delete from creature_template where entry = 60309;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES 
(60309, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42102, 0, 0, 0, 'Track Switch', '', 'interact', 0, 90, 90, 4, 0, 35, 35, 1, 1, 1.14286, 1, 0, 1, 3, 0, 0, 1, 0, 0, 1, 0, 67110912, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1628439554, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'npc_track_switch_north', 16769);

UPDATE creature_template SET scriptname = 'npc_track_switch_east', npcflag = 1, IconName = 'interact' where entry = 60283;
UPDATE creature_template SET scriptname = 'npc_track_switch_north', npcflag = 1, IconName = 'interact' where entry = 60309;

DELETE FROM trinity_string WHERE entry BETWEEN 1375 AND 1385;
INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`, `content_loc9`, `content_loc10`) VALUES 
(1375, 'The battle begins in 1 minute.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(1376, 'The battle begins in 30 seconds.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(1377, 'The battle has begun!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(1378, 'The %s orb has been dropped by %s!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(1379, '$n has taken the %s orb!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(1380, 'The %s orb has been returned!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(1381, '|cffB115EAPurple|r', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(1382, '|cffF26110Orange|r', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(1383, '|cff32D000Green|r', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(1384, '|cff52bae3Blue|r', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(1385, 'The orbs have appeared!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

DELETE FROM gameobject_template WHERE entry IN (180322, 212091, 212092, 212093, 212094, 213172);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `unkInt32`, `AIName`, `ScriptName`, `WDBVerified`) VALUES 
(180322, 5, 6391, 'Ghost Gate', '', '', '', 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
(212091, 24, 11533, 'Orb of Power', '', 'Capturing', '', 35, 0, 4, 0, 0, 0, 0, 0, 0, 0, 121164, 0, 121164, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
(212092, 24, 11533, 'Orb of Power', '', 'Capturing', '', 35, 0, 4, 0, 0, 0, 0, 0, 0, 0, 121175, 0, 121175, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
(212093, 24, 11533, 'Orb of Power', '', 'Capturing', '', 35, 0, 4, 0, 0, 0, 0, 0, 0, 0, 121176, 0, 121176, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
(212094, 24, 11533, 'Orb of Power', '', 'Capturing', '', 35, 0, 4, 0, 0, 0, 0, 0, 0, 0, 121177, 0, 121177, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
(213172, 0, 11920, 'Grand Wall', '', '', '', 0, 63, 0.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 1);