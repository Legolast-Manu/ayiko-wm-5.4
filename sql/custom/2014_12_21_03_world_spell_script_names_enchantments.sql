-- Windsong
DELETE FROM `spell_proc_event` WHERE `entry` = '104561';
INSERT INTO `spell_proc_event` (`entry`, `ppmRate`) VALUES ('104561', '2');
DELETE FROM `spell_script_names` WHERE `spell_id` = '104561';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('104561', 'spell_gen_ench_windsong');
-- Dancing Steel
DELETE FROM `spell_proc_event` WHERE `entry` = '118333';
INSERT INTO `spell_proc_event` (`entry`, `ppmRate`) VALUES ('118333', '2.3');
DELETE FROM `spell_script_names` WHERE `spell_id` = '118333';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('118333', 'spell_gen_ench_dancing_steel');
-- Jade Spirit
DELETE FROM `spell_proc_event` WHERE `entry` = '120033';
INSERT INTO `spell_proc_event` (`entry`, `ppmRate`) VALUES ('120033', '2');
DELETE FROM `spell_script_names` WHERE `spell_id` = '120033';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('120033', 'spell_gen_ench_jade_spirit');
DELETE FROM `spell_script_names` WHERE `spell_id` = '104993';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('104993', 'spell_gen_ench_jade_spirit_eff');
