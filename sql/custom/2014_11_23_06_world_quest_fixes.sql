-- An Improper Burial
DELETE FROM `smart_scripts` WHERE `source_type` = '0' AND `entryorguid` = '21846';
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
('21846', '0', '0', '1', '8', '0', '100', '1', '39189', '0', '0', '0', '11', '59216', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', 'Slain Sha tar Vindicator - On Spellhit (Sha tari Torch) - Cast Burning Corpse'),
('21846', '0', '1', '2', '61', '0', '100', '0', '0', '0', '0', '0', '33', '21846', '0', '0', '0', '0', '0', '7', '0', '0', '0', '0', '0', '0', '0', 'Slain Sha tar Vindicator - Linked with Previous Event - Give Kill Credit'),
('21846', '0', '2', '3', '61', '0', '100', '0', '0', '0', '0', '0', '11', '37759', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Slain Sha tar Vindicator - Linked with Previous Event - Cast Bone Wastes - Summon Draenei Guardian Spirit'),
('21846', '0', '3', '0', '61', '0', '100', '0', '0', '0', '0', '0', '41', '15000', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', 'Slain Sha tar Vindicator - Linked with Previous Event - Despawn after 15 seconds');

-- The Final Code
UPDATE `gameobject_template` SET `AIName`='SmartGameObjectAI' WHERE (`entry`='184725');
DELETE FROM `smart_scripts` WHERE `source_type` = '1' AND `entryorguid` = '184725';
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('184725', '1', '0', '0', '70', '0', '100', '0', '2', '0', '0', '0', '33', '21039', '0', '0', '0', '0', '0', '7', '0', '0', '0', '0', '0', '0', '0', 'Raufen - give KC on activation');

-- Incantations Fae and Primal
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN('62766', '62765');
DELETE FROM `smart_scripts` WHERE `source_type` = '0' AND `entryorguid` IN('62766', '62765');
INSERT INTO `smart_scripts` (`entryorguid`, `id`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `target_type`, `comment`) VALUES
('62766', '0', '3000', '3000', '12000', '12000', '11', '128049', '1', 'Raufen - In Combat - Cast Spell'),
('62766', '1', '1000', '1000', '6000', '6000', '11', '128056', '1', 'Raufen - In Combat - Cast Spell'),
('62766', '2', '8000', '8000', '18000', '18000', '11', '127373', '1', 'Raufen - In Combat - Cast Spell'),
('62765', '0', '3000', '3000', '12000', '12000', '11', '118510', '1', 'Raufen - In Combat - Cast Spell'),
('62765', '1', '9000', '9000', '18000', '18000', '11', '128056', '1', 'Raufen - In Combat - Cast Spell');

DELETE FROM `creature` WHERE `id` IN('62766', '62765');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES
('7344275', '62766', '870', '0', '0', '1', '65535', '0', '0', '706.442', '4360.89', '164.858', '0.208731', '300', '0', '0', '787882', '0', '0', '0', '0', '0', '2048', '0', '0', NULL),
('7344273', '62765', '870', '0', '0', '1', '65535', '0', '0', '766.714', '4413.24', '155.442', '4.47344', '300', '0', '0', '787882', '0', '0', '0', '0', '0', '2048', '0', '0', NULL);

-- Wood and Shade
DELETE FROM `areatrigger_scripts` WHERE `entry` IN('8123', '8124');
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
('8123', 'at_q_wood_and_shade'),
('8124', 'at_q_wood_and_shade');

DELETE FROM `gameobject` WHERE `id` = '212642';
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `protect_anti_doublet`) VALUES ('10000', '212642', '870', '1', '65535', '879.94', '3832.15', '276.86', '3.23956', '0', '0', '0.9988', '-0.0489659', '300', '0', '1', '0', NULL);

-- Bound with Wood
DELETE FROM `smart_scripts` WHERE (`entryorguid`=62876 AND `source_type`=0);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(62876, 0, 0, 0, 73, 0, 100, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Nepenthe-Withered Husk - On Spellclick - Despawn");

-- Fiery Wings
UPDATE `smart_scripts` SET `event_type`='73' WHERE (`entryorguid`='62764') AND (`source_type`='0') AND (`id`='0') AND (`link`='1');

-- The Search for Restless Leng
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` = '65586';
DELETE FROM `smart_scripts` WHERE `source_type` = '0' AND `entryorguid` = '65586';
INSERT INTO `smart_scripts` (`entryorguid`, `id`, `event_type`, `event_flags`, `event_param1`, `event_param2`, `action_type`, `action_param1`, `target_type`, `comment`) VALUES
('65586', '0', '1', '1', '0', '0', '1', '0', '1', 'Raufen - On Summon - Talk'),
('65586', '1', '1', '1', '5000', '5000', '1', '1', '1', 'Raufen - On Summon - Talk');
DELETE FROM `creature_text` WHERE `entry` = '65586';
INSERT INTO `creature_text` (`entry`, `groupid`, `text`, `type`, `probability`, `comment`) VALUES
('65586', '0', 'Thank you. I owe you greatly.', '12', '100', 'Raufen - On Spawn - Talk'),
('65586', '1', 'I overheard a great deal through the cage. Rensai will be pleased with what I have to tell him.', '12', '100', 'Raufen - On Spawn - Talk');
UPDATE `gameobject_template` SET `ScriptName`='go_sikthik_cage' WHERE (`entry`='214734');

-- Running Rampant
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN('60669', '60739');
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`) VALUES
('60669', '76309', '0'),
('60739', '76309', '0');
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN('60669', '60739');
DELETE FROM `smart_scripts` WHERE `source_type` = '0' AND `entryorguid` IN('60669', '60739');
INSERT INTO `smart_scripts` (`entryorguid`, `id`, `event_type`, `event_param1`, `action_type`, `action_param1`, `target_type`, `comment`) VALUES
('60669', '0', '8', '76309', '41', '2000', '1', 'Raufen - On Spell Click - Force Despawn'),
('60669', '1', '73', '76309', '33', '60669', '7', 'Raufen - On Spell Click - Give KC'),
('60669', '2', '8', '76309', '83', '16777216', '1', 'Raufen - On Spell Click - Remove SpellClick Flag'),
('60669', '3', '8', '76309', '59', '1', '1', 'Raufen - On Spell Click - Set Run'),
('60669', '4', '8', '76309', '46', '15', '1', 'Raufen - On Spell Click - Move Forward'),

('60739', '0', '8', '76309', '41', '2000', '1', 'Raufen - On Spell Click - Force Despawn'),
('60739', '1', '73', '76309', '33', '60669', '7', 'Raufen - On Spell Click - Give KC'),
('60739', '2', '8', '76309', '83', '16777216', '1', 'Raufen - On Spell Click - Remove SpellClick Flag'),
('60739', '3', '8', '76309', '59', '1', '1', 'Raufen - On Spell Click - Set Run'),
('60739', '4', '8', '76309', '46', '15', '1', 'Raufen - On Spell Click - Move Forward');

DELETE FROM `conditions` WHERE (`SourceTypeOrReferenceId`='18') AND (`SourceGroup`='60669') AND (`SourceEntry`='76309');
DELETE FROM `conditions` WHERE (`SourceTypeOrReferenceId`='18') AND (`SourceGroup`='60739') AND (`SourceEntry`='76309');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `Comment`) VALUES
('18', '60669', '76309', '9', '30770', 'Raufen - Require quest for spellclick'),
('18', '60739', '76309', '9', '30770', 'Raufen - Require quest for spellclick');

-- Back on Their Feet
UPDATE `creature` SET `spawndist`='0', `currentwaypoint`='0', `unit_flags`='32768' WHERE (`id`='61692');
DELETE FROM `spell_script_names` WHERE `spell_id` = '120573';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('120573', 'spell_item_cintron_infused_bandage');

-- Pitching In
UPDATE `creature` SET `spawndist`='0', `MovementType`='0' WHERE (`id`='60705');
DELETE FROM `conditions` WHERE (`SourceTypeOrReferenceId`='13') AND (`SourceEntry`='117631');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES ('13', '1', '117631', '31', '0', '3', '60705', 'Raufen - Require proper target for effect');
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE (`entry`='60705');
DELETE FROM `smart_scripts` WHERE `entryorguid` = '60705' AND `source_type` = '0';
INSERT INTO `smart_scripts` (`entryorguid`, `id`, `event_type`, `event_param1`, `action_type`, `action_param1`, `target_type`, `comment`) VALUES
('60705', '0', '8', '117631', '33', '60759', '7', 'Raufen - On Spell Hit - Give KC'),
('60705', '1', '8', '117631', '37', '0', '1', 'Raufen - On Spell Hit - Die'),
('60705', '2', '8', '117631', '41', '3000', '1', 'Raufen - On Spell Hit - Force Despawn');
DELETE FROM `creature_template_aura` WHERE (`entry`='60705') AND (`aura`='107477');
