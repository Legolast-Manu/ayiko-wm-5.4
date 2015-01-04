/* TODO
- There's an event where the 'Physician's Assistant' (42552) says something, walks somewhere and teleports it away when a new Rescued Survivor appears. (Rescued Survivors says: Thank the Light! I've made it. Physician's Assistant says: Ah, a new arrival. Right this way, sir.
- Physician's Assistant also needs some pathing. He walks around all Rescued Survivors, stands in front of them and then moves to the next one. (SCRIPT ON GUID)
- Quest 'Strike From Above' requires core support (25841)
- Quest 'It's Raid Night Every Night' requires core support

86298 - Force Cast Refresh Gnomeregan Zone Aura

78957 - [DND] Force Say 0x
78963 - Goblin Nurse Healing Aura (78965 - Tender Touch)

78958 - Resonating Clash Air Clear

The Ultrasafe Personnel Launcher
The Fight Continues
Rallying the Defenders
Oh, A Hunter's Life For Me
Harnessing the Flames
Hallowed Rune
Getting Battle-Ready
Filling Up the Spellbook
Evisceratin' the Enemy
*/

-- Gnomeregan Quest Links
UPDATE `quest_template` SET `PrevQuestId`=27635 WHERE `Id`=27674;
UPDATE `quest_template` SET `PrevQuestId`=27674 WHERE `Id` IN (26206, 26197, 26199, 26202, 26203);
UPDATE `quest_template` SET `PrevQuestId`=26222 WHERE `Id`=26264;
UPDATE `quest_template` SET `PrevQuestId`=26205 WHERE `Id`=26316;
UPDATE `quest_template` SET `PrevQuestId`=26316 WHERE `Id`=26284;
UPDATE `quest_template` SET `PrevQuestId`=26316,`EndText`='',`EmoteOnComplete`=153 WHERE `Id`=26285;
UPDATE `quest_template` SET `PrevQuestId`=26329 WHERE `Id`=26333;
UPDATE `quest_template` SET `PrevQuestId`=26333 WHERE `Id`=26339;
UPDATE `quest_template` SET `PrevQuestId`=26364 WHERE `Id`=26373;

-- Quests - Pinned Down has been scripted
-- Nevin Twistwrench
SET @NPC := 45966;
SET @QUEST := 27670;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14, `maxdmg` = 4, `mingold` = 1, `maxgold` = 4 WHERE `entry` IN (46363, 46391);
UPDATE `creature_template` SET `faction_A` = 63, `faction_H` = 63 WHERE `entry` = 46363;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,19,0,100,0,@QUEST,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Nevin Twistwrench - On Quest Accept - Say Line 0");
DELETE FROM `creature_text` WHERE `entry`=@NPC;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC,0,0,"We have to break through those mindless leper gnomes if we want to get out of here alive!",12,0,100,0,0,0,"Nevin Twistwrench");
-- Crazed Leper Gnome
SET @CGUID := 5013957;
DELETE FROM `creature` WHERE `id`=46391;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID+0, 46391, 0, 1, 1, 0, 46391, -4992.78, 836.801, 276.234, 0.584423, 30, 0, 0, 42, 0, 0, 0, 0, 0),
(@CGUID+1, 46391, 0, 1, 1, 0, 46391, -4985.42, 829.445, 276.233, 1.43442, 30, 0, 0, 42, 0, 0, 0, 0, 0),
(242729, 46391, 0, 1, 1, 0, 46391, -5146.89, 765.158, 287.397, 2.96706, 30, 0, 0, 42, 0, 0, 0, 0, 0),
(243189, 46391, 0, 1, 1, 0, 46391, -4978.19, 837.786, 276.239, 1.81812, 30, 0, 0, 42, 0, 0, 0, 0, 0),
(243789, 46391, 0, 1, 1, 0, 46391, -5131.15, 762.643, 287.437, 3.68265, 30, 0, 0, 42, 0, 0, 0, 0, 0),
(244256, 46391, 0, 1, 1, 0, 46391, -5147.41, 758.16, 287.397, 3.05434, 30, 0, 0, 42, 0, 0, 0, 0, 0);
-- Crazed Leper Gnome
DELETE FROM `creature_addon` WHERE `guid` IN (SELECT `guid` FROM `creature` WHERE `id` = 46363);
DELETE FROM `creature_template_addon` WHERE `entry`=46363;
INSERT INTO `creature_template_addon` (`entry`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(46363,0,0,0,0,'95205 86400 86414'); -- Irradiated (NPC), Leper Gnome Slime Drip, Leper Gnome Zombie Anim

-- S.A.F.E. Operative assists Injured Gnomes
SET @NPC := 46449;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid` = @NPC  AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,1,0,50,0,8000,10000,60000,90000,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-S.A.F.E. Operative - Out of Combat - Say Line 0");
DELETE FROM `creature_text` WHERE `entry`=@NPC;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC,0,0,"Stay with me. You're going to make it out of here.",12,0,100,0,0,0,"S.A.F.E. Operative");

-- S.A.F.E. Operative (hallway defense)
SET @AT_LEFT := 6455;
SET @AT_RIGHT := 6454;
SET @NPC := 45847;
SET @SPELL_SHOOT := 85756;
SET @SPELL_ATTACK := 74413;
DELETE FROM `areatrigger_scripts` WHERE `entry` IN (@AT_LEFT,@AT_RIGHT);
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(@AT_LEFT,"SmartTrigger"),
(@AT_RIGHT,"SmartTrigger");
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@AT_LEFT,@AT_RIGHT) AND `source_type`=2;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@AT_LEFT,2,0,0,46,0,100,0,@AT_LEFT,0,0,0,45,10,10,0,0,0,0,19,@NPC,5,0,0,0,0,0,"Nepenthe-AT Near Blocked Hallway Gnomeregan - On Trigger 6455 - Set Data S.A.F.E. Operative"),
(@AT_RIGHT,2,0,0,46,0,100,0,@AT_RIGHT,0,0,0,45,10,10,0,0,0,0,19,@NPC,5,0,0,0,0,0,"Nepenthe-AT Near Blocked Hallway Gnomeregan - On Trigger 6454 - Set Data S.A.F.E. Operative");
UPDATE `creature_template` SET `mindmg` = 1, `maxdmg` = 2, `attackpower` = 4, `rangeattacktime` = 2000, `minrangedmg` = 1, `maxrangedmg` = 2, `rangedattackpower` = 4, `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC,@NPC*100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,38,0,100,0,10,10,30000,30000,80,@NPC*100,0,0,0,0,2,1,0,0,0,0,0,0,0,"Nepenthe-S.A.F.E. Operative - On Data Set - Run Script"),
(@NPC,0,1,0,11,0,100,0,0,0,0,0,21,0,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-S.A.F.E. Operative - On Spawn - Disable Move"),
(@NPC,0,2,0,0,0,100,0,1000,4000,7000,10000,11,@SPELL_SHOOT,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-S.A.F.E. Operative - In Combat - Cast Shoot"),
(@NPC,0,3,0,9,0,100,0,1,3,1000,3000,11,@SPELL_ATTACK,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-S.A.F.E. Operative - On 3 yard Range - Cast Attack"),
(@NPC*100,9,0,0,0,0,100,0,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-S.A.F.E. Operative - On Script - Set Phase 1"),
(@NPC*100,9,1,0,0,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-S.A.F.E. Operative - On Script - Say Line 0"),
(@NPC*100,9,2,0,0,0,100,0,0,0,0,0,22,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-S.A.F.E. Operative - On Script - Set Phase 0");
DELETE FROM `creature_text` WHERE `entry`=@NPC;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC,0,0,"Our men have secured the walkway. Focus on helping those in the main room.",12,0,100,0,0,0,"S.A.F.E. Operative");

-- Quests - See to the Survivors has been scripted
-- Survivor
SET @NPC := 46268;
SET @SPELL_ACTIVATE_TELEPORT_BEACON := 86264;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `creature_template_addon` WHERE `entry`=@NPC;
INSERT INTO `creature_template_addon` (`entry`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(@NPC,0,0,0,431, ''); -- STATE_COWER
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=17 AND `SourceEntry`=@SPELL_ACTIVATE_TELEPORT_BEACON;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17,0,@SPELL_ACTIVATE_TELEPORT_BEACON,0,31,1,3,@NPC,0,0,'','Teleport Beacon can be used only on Survivors');
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,1,0,100,1,0,0,0,0,8,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Survivor - Out of Combat - Set React State Passive"),
(@NPC,0,1,0,1,0,100,0,3000,44000,66000,79000,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Survivor - Out of Combat - Say Line 0 (random)"),
(@NPC,0,2,3,8,0,100,0,@SPELL_ACTIVATE_TELEPORT_BEACON,0,0,0,33,@NPC,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Survivor - On Spellhit - Quest Credit"),
(@NPC,0,3,0,61,0,100,0,0,0,0,0,41,2500,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Survivor - On Spellhit - Forced Despawn");
DELETE FROM `creature_text` WHERE `entry`=@NPC;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC,0,0,"Don't let them eat me!",12,0,0,0,0,0,"Survivor"),
(@NPC,0,1,"Help! I'm too weak to make it on my own.",12,0,0,0,0,0,"Survivor"),
(@NPC,0,2,"I don't want to die down here!",12,0,0,0,0,0,"Survivor"),
(@NPC,0,3,"I'm really going to die down here, aren't I?",12,0,0,0,0,0,"Survivor"),
(@NPC,0,4,"I'm too young to die!",12,0,0,0,0,0,"Survivor"),
(@NPC,0,5,"Please help me!",12,0,0,0,0,0,"Survivor"),
(@NPC,0,6,"Some help! Anyone!",12,0,0,0,0,0,"Survivor");

-- Quests - Withdraw to the Loading Room! has been scripted
-- Carvo Blastbolt
SET @NPC := 47250;
SET @QUEST := 28169;
SET @SPELL_SUMMON := 88988;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,19,0,100,0,@QUEST,0,0,0,85,@SPELL_SUMMON,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Carvo Blastbolt - On Quest Accept - Invoker Cast Summon Loading Room Guide");
-- S.A.F.E. Operative
SET @NPC := 47836;
SET @WPEND := 11;
UPDATE `creature_template` SET `minlevel` = 5,`maxlevel` = 5,`faction_A` = 2300,`faction_H` = 2300,`mindmg` = 7,`maxdmg` = 12,`attackpower` = 46,`baseattacktime` = 2000,`unit_flags` = 768,`dynamicflags` = 2048,`speed_walk` = 1.6,`speed_run` = 1.6,`AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC,@NPC*100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,54,0,100,0,0,0,0,0,80,@NPC*100,0,2,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-S.A.F.E. Operative - Just Summoned - Run Script"),
(@NPC*100,9,0,0,0,0,100,0,0,0,0,0,1,0,0,0,0,0,0,23,0,0,0,0,0,0,0,"Nepenthe-S.A.F.E. Operative - On Script - Say Line 0"), -- SMART_TARGET_OWNER_OR_SUMMONER because we have cross-casted the summon spell
(@NPC*100,9,1,0,0,0,100,0,4000,4000,0,0,53,0,@NPC,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-S.A.F.E. Operative - On Script - Start WP"),
(@NPC,0,1,0,40,0,100,0,@WPEND,@NPC,0,0,5,66,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-S.A.F.E. Operative - On WP end - Emote ONESHOT_SALUTE");
DELETE FROM `creature_text` WHERE `entry`=@NPC;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC,0,0,"You can follow me to the Loading Room, $N.",12,0,100,25,0,0,"S.A.F.E. Operative"); -- ONESHOT_POINT
DELETE FROM `spell_target_position` WHERE `id` = @SPELL_SUMMON;
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(@SPELL_SUMMON,0,-4981.137695,781.203735,288.484863,3.388015);
DELETE FROM `waypoints` WHERE `entry`=@NPC;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(@NPC,1,-4989.066895,779.311584,288.484863,"S.A.F.E. Operative"),
(@NPC,2,-4996.347656,776.893555,288.484650,"S.A.F.E. Operative"),
(@NPC,3,-5006.137207,772.944092,286.985291,"S.A.F.E. Operative"),
(@NPC,4,-5018.198730,771.053833,285.083893,"S.A.F.E. Operative"),
(@NPC,5,-5090.164551,775.764893,283.313660,"S.A.F.E. Operative"),
(@NPC,6,-5090.164551,775.764893,283.313660,"S.A.F.E. Operative"),
(@NPC,7,-5099.698242,775.027771,283.313660,"S.A.F.E. Operative"),
(@NPC,8,-5132.132324,765.237549,287.367584,"S.A.F.E. Operative"),
(@NPC,9,-5142.485352,763.445312,287.395020,"S.A.F.E. Operative"),
(@NPC,10,-5154.397461,763.145142,287.395111,"S.A.F.E. Operative"),
(@NPC,@WPEND,-5173.188477,767.107117,285.474060,"S.A.F.E. Operative");
-- Target Acquisition Device
UPDATE `creature` SET `spawndist` = 10, `MovementType` = 1 WHERE `id` = 46012;
UPDATE `creature_template` SET `InhabitType` = 4, `unit_flags` = 33554688, `VehicleId` = 1160, `ScriptName` = '' WHERE `entry` = 46012;
-- unit_flag not-selectable hack, requires core to don't accept players on vehicle without ride vehicle aura

-- Quests - Decontamination scripted
UPDATE `creature_template` SET `subname` = 'With Dunk-o-Matic Action', `ScriptName` = 'npc_sanitron_500' WHERE `entry` = 46185;
UPDATE quest_template SET StartScript = 0 WHERE Id = 27635;

DELETE FROM waypoint_data WHERE id = 4618501;
INSERT INTO waypoint_data (id, point, position_x, position_y, position_z, orientation, delay, move_flag, action, action_chance, wpguid) VALUES
(4618501, 1, -5173.24, 726.755, 290.398, 0, 5000, 1, 0, 100, 0),
(4618501, 2, -5174.43, 717.94, 290.398, 0, 6000, 1, 0, 100, 0),
(4618501, 3, -5175.32, 703.42, 290.398, 0, 5000, 1, 0, 100, 0);

DELETE FROM creature_text WHERE entry = 46185;
INSERT INTO creature_text (entry, groupid, id, text, type, language, probability, emote, duration, sound, comment) VALUES
(46185, 0, 0, 'Commencing decontamination sequence...', 12, 0, 100, 0, 0, 0, 'Sanitron 500'),
(46185, 1, 0, 'Decontamination complete. Standby for delivery.', 12, 0, 100, 0, 0, 0, 'Sanitron 500'),
(46185, 2, 0, 'Warning, system overload. Malfunction imminent!', 12, 0, 100, 0, 0, 0, 'Sanitron 500');

-- Quests - To the Surface has been scripted
-- Torben Zapblast
SET @NPC := 46293;
SET @GOSSIP := 12104;
SET @TEXTID := 16995;
SET @QUEST := 27674;
SET @SPELL_TELEPORT := 86278;
UPDATE `creature_template` SET `AIName`='SmartAI',`ScriptName` = '',`gossip_menu_id`=@GOSSIP WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,62,0,100,0,@GOSSIP,0,0,0,11,@SPELL_TELEPORT,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Torben Zapblast - On Gossip Select - Cast Teleport to the Surface"),
(@NPC,0,1,0,62,0,100,0,@GOSSIP,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Torben Zapblast - On Gossip Select - Close Gossip");
DELETE FROM `gossip_menu` WHERE `entry`=@GOSSIP AND `text_id`=@TEXTID;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(@GOSSIP,@TEXTID);
-- Static text and cleanup
DELETE FROM `npc_text` WHERE `id`=46293;
DELETE FROM `npc_text` WHERE `id`=@TEXTID;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`, `WDBVerified`) VALUES
(@TEXTID,'Torben Zapblast, teleportation solutions engineer at your service!','','0','1','0','5','0','0','0','0','','','0','0','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0','1');
-- Menu option
DELETE FROM `gossip_menu_option` WHERE `menu_id`=@GOSSIP;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(@GOSSIP,0,0,"Teleport me to the surface, Torben.",1,1,0,0,0,0,NULL);
DELETE FROM `spell_target_position` WHERE `id` = @SPELL_TELEPORT;
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(@SPELL_TELEPORT,0,-5202.198242,477.385620,388.233032,5.491077);

-- [SQL] Quests - Trolling for Information corrected (Fixes #489)
SET @NPC_SHIKALA := 37108;
SET @NPC_RIKKARI := 37173;
SET @NPC_MIRIMKO := 37174;
SET @KILLCREDITBUNNY_SE := 37109;
SET @KILLCREDITBUNNY_SW := 37110;
SET @KILLCREDITBUNNY_W := 37111;
SET @QUEST_TROLLING_FOR_INFORMATION := 24489;
SET @CGUID := 5013954; -- needs 3
UPDATE `gameobject_template` SET `flags` = 4 WHERE `entry` IN (178085, 178084, 148499);
DELETE FROM `creature` WHERE `id` IN (@KILLCREDITBUNNY_SE,@KILLCREDITBUNNY_SW,@KILLCREDITBUNNY_W);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID+0, @KILLCREDITBUNNY_SE, 0, 1, 1, 11686, 0, -6485.77, 503.768, 386.047, 6.21123, 30, 0, 0, 42, 0, 0, 0, 0, 0), -- Southeast
(@CGUID+1, @KILLCREDITBUNNY_SW, 0, 1, 1, 11686, 0, -6508.45, 683.899, 387.274, 5.55935, 30, 0, 0, 42, 0, 0, 0, 0, 0), -- Southwest
(@CGUID+2, @KILLCREDITBUNNY_W, 0, 1, 1, 11686, 0, -6392.86, 776.492, 386.213, 5.85937, 30, 0, 0, 42, 0, 0, 0, 0, 0); -- West
UPDATE `creature_template` SET `InhabitType` = 7, `flags_extra` = 0, `AIName`='SmartAI', `ScriptName`='' WHERE `entry` IN (@KILLCREDITBUNNY_SE,@KILLCREDITBUNNY_SW,@KILLCREDITBUNNY_W);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@KILLCREDITBUNNY_SE,@KILLCREDITBUNNY_SW,@KILLCREDITBUNNY_W) AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@KILLCREDITBUNNY_SE,0,0,1,10,0,100,0,1,12,90000,90000,33,@KILLCREDITBUNNY_SE,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Trolling for Information Kill Credit Bunny SE - On friendly player LoS - Kill Credit SE - q24489"),
(@KILLCREDITBUNNY_SE,0,1,0,61,0,100,0,0,0,0,0,45,1,1,0,0,0,0,19,@NPC_SHIKALA,0,0,0,0,0,0,"Nepenthe-Trolling for Information Kill Credit Bunny SE - On LoS - Set Data Soothsayer Shi'kala - q24489"),
(@KILLCREDITBUNNY_SW,0,0,1,10,0,100,0,1,12,90000,90000,33,@KILLCREDITBUNNY_SW,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Trolling for Information Kill Credit Bunny SW - On friendly player LoS - Set Data Soothsayer Rikkari - q24489"),
(@KILLCREDITBUNNY_SW,0,1,0,61,0,100,0,0,0,0,0,45,1,1,0,0,0,0,19,@NPC_RIKKARI,0,0,0,0,0,0,"Nepenthe-Trolling for Information Kill Credit Bunny SW - On LoS - Kill Credit SW - q24489"),
(@KILLCREDITBUNNY_W,0,0,1,10,0,100,0,1,12,90000,90000,33,@KILLCREDITBUNNY_W,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Trolling for Information Kill Credit Bunny W - On friendly player LoS - Set Data Soothsayer Mirim'koa - q24489"),
(@KILLCREDITBUNNY_W,0,1,0,61,0,100,0,0,0,0,0,45,1,1,0,0,0,0,19,@NPC_MIRIMKO,0,0,0,0,0,0,"Nepenthe-Trolling for Information Kill Credit Bunny W - On LoS - Kill Credit W - q24489");
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceEntry` IN (@KILLCREDITBUNNY_SE,@KILLCREDITBUNNY_SW,@KILLCREDITBUNNY_W);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 0, @KILLCREDITBUNNY_SE, 0, 0, 9, 0, @QUEST_TROLLING_FOR_INFORMATION, 0, 0, 0, 0, 0, "", "SAI - Execute only if player in range have no quest"),
(22, 0, @KILLCREDITBUNNY_SW, 0, 0, 9, 0, @QUEST_TROLLING_FOR_INFORMATION, 0, 0, 0, 0, 0, "", "SAI - Execute only if player in range have no quest"),
(22, 0, @KILLCREDITBUNNY_W, 0, 0, 9, 0, @QUEST_TROLLING_FOR_INFORMATION, 0, 0, 0, 0, 0, "", "SAI - Execute only if player in range have no quest");
-- Soothsayer Shi'kala
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_SHIKALA;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_SHIKALA,@NPC_SHIKALA*100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_SHIKALA,0,0,0,38,0,100,0,1,1,0,0,80,@NPC_SHIKALA*100,2,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Soothsayer Shi'kala - On Data Set - Run Script - q24489"),
(@NPC_SHIKALA*100,9,0,0,0,0,100,0,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Soothsayer Shi'kala - On Script - Set Phase 1"),
(@NPC_SHIKALA*100,9,1,0,0,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Soothsayer Shi'kala - On Script - Say Line 0"),
(@NPC_SHIKALA*100,9,2,0,0,0,100,0,7000,7000,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Soothsayer Shi'kala - On Script - Say Line 1"),
(@NPC_SHIKALA*100,9,3,0,0,0,100,0,7000,7000,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Soothsayer Shi'kala - On Script - Say Line 2"),
(@NPC_SHIKALA*100,9,4,0,0,0,100,0,8000,8000,0,0,22,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Soothsayer Shi'kala - On Script - Set Phase 0");
-- Soothsayer Rikkari
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_RIKKARI;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_RIKKARI,@NPC_RIKKARI*100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_RIKKARI,0,0,0,38,0,100,0,1,1,0,0,80,@NPC_RIKKARI*100,2,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Soothsayer Rikkari - On Data Set - Run Script - q24489"),
(@NPC_RIKKARI*100,9,0,0,0,0,100,0,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Soothsayer Rikkari - On Script - Set Phase 1"),
(@NPC_RIKKARI*100,9,1,0,0,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Soothsayer Rikkari - On Script - Say Line 0"),
(@NPC_RIKKARI*100,9,2,0,0,0,100,0,7000,7000,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Soothsayer Rikkari - On Script - Say Line 1"),
(@NPC_RIKKARI*100,9,3,0,0,0,100,0,7000,7000,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Soothsayer Rikkari - On Script - Say Line 2"),
(@NPC_RIKKARI*100,9,4,0,0,0,100,0,8000,8000,0,0,22,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Soothsayer Rikkari - On Script - Set Phase 0");
-- Soothsayer Mirim'koa
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_MIRIMKO;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_MIRIMKO,@NPC_MIRIMKO*100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_MIRIMKO,0,0,0,38,0,100,0,1,1,0,0,80,@NPC_MIRIMKO*100,2,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Soothsayer Mirim'koa - On Data Set - Run Script - q24489"),
(@NPC_MIRIMKO*100,9,0,0,0,0,100,0,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Soothsayer Mirim'koa - On Script - Set Phase 1"),
(@NPC_MIRIMKO*100,9,1,0,0,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Soothsayer Mirim'koa - On Script - Say Line 0"),
(@NPC_MIRIMKO*100,9,2,0,0,0,100,0,7000,7000,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Soothsayer Mirim'koa - On Script - Say Line 1"),
(@NPC_MIRIMKO*100,9,3,0,0,0,100,0,7000,7000,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Soothsayer Mirim'koa - On Script - Say Line 2"),
(@NPC_MIRIMKO*100,9,4,0,0,0,100,0,8000,8000,0,0,22,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Soothsayer Mirim'koa - On Script - Set Phase 0");
-- Texts
DELETE FROM `creature_text` WHERE `entry` IN (@NPC_SHIKALA,@NPC_RIKKARI,@NPC_MIRIMKO);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC_SHIKALA,0,0,"Da spirits be angry with us.",12,0,100,0,0,0,"Soothsayer Shi'kala"),
(@NPC_SHIKALA,1,0,"I don' know why the spirits be rejectin' us so.",12,0,100,0,0,0,"Soothsayer Shi'kala"),
(@NPC_SHIKALA,2,0,"Don' worry, child. Grik'nir gonna help us get through this.",12,0,100,0,0,0,"Soothsayer Shi'kala"),
(@NPC_RIKKARI,0,0,"What we gon' do now, you ask? We wait.",12,0,100,0,0,0,"Soothsayer Rikkari"),
(@NPC_RIKKARI,1,0,"Grik'nir says he gon' talk to the elemental, get it to fight on our side.",12,0,100,0,0,0,"Soothsayer Rikkari"),
(@NPC_RIKKARI,2,0,"Soon enough we take over dis valley. Soon enough.",12,0,100,0,0,0,"Soothsayer Rikkari"),
(@NPC_MIRIMKO,0,0,"Our land be a land of ice an' snow.",12,0,100,0,0,0,"Soothsayer Mirim'koa"),
(@NPC_MIRIMKO,1,0,"But beneath the earth, child, there always be fire.",12,0,100,0,0,0,"Soothsayer Mirim'koa"),
(@NPC_MIRIMKO,2,0,"De spirit come from deep down to talk with Grik'nir.",12,0,100,0,0,0,"Soothsayer Mirim'koa");

-- Quests - Pushing Forward has been scripted (Feedback #729)
-- Constriction Totem
SET @NPC_CTOTEM := 41202;
SET @NPC_KMOUNTAINEER := 41237;
SET @SPELL_ROOT := 77311;
SET @SPELL_BURN := 77314;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_CTOTEM;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC_CTOTEM AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_CTOTEM,0,0,0,60,0,100,0,8000,8000,12000,12000,11,@SPELL_ROOT,0,0,0,0,0,19,@NPC_KMOUNTAINEER,0,0,0,0,0,0,"Nepenthe-Constriction Totem - Update - Cast Constriction Totem Aura"),
(@NPC_CTOTEM,0,1,2,8,0,100,0,@SPELL_BURN,0,0,0,33,@NPC_CTOTEM,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Constriction Totem - On Spellhit - Quest Credit"),
(@NPC_CTOTEM,0,2,3,61,0,100,0,0,0,0,0,45,1,1,0,0,0,0,19,@NPC_KMOUNTAINEER,0,0,0,0,0,0,"Nepenthe-Constriction Totem - On Spellhit - Set Data Kharanos Mountaineer"),
(@NPC_CTOTEM,0,3,0,61,0,100,0,0,0,0,0,41,2000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Constriction Totem - On Spellhit - Forced Despawn");
-- Kharanos Mountaineer
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_KMOUNTAINEER;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_KMOUNTAINEER,@NPC_KMOUNTAINEER*100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_KMOUNTAINEER,0,0,1,1,0,100,1,0,0,0,0,5,393,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Kharanos Mountaineer - Out of Combat - Emote Choke"),
(@NPC_KMOUNTAINEER,0,1,0,61,0,100,0,0,0,0,0,8,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Kharanos Mountaineer - Out of Combat - React State Passive"),
(@NPC_KMOUNTAINEER,0,2,0,38,0,100,0,1,1,0,0,80,@NPC_KMOUNTAINEER*100,0,0,0,0,2,1,0,0,0,0,0,0,0,"Nepenthe-Kharanos Mountaineer - On Data Set - Run Script"),
(@NPC_KMOUNTAINEER*100,9,0,0,0,0,100,0,0,0,0,0,5,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Kharanos Mountaineer - On Script - Stop Emoting"),
(@NPC_KMOUNTAINEER*100,9,1,0,0,0,100,0,1000,1000,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Kharanos Mountaineer - On Script - Say Line 0"),
(@NPC_KMOUNTAINEER*100,9,2,0,0,0,100,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Kharanos Mountaineer - On Script - Say Line 1"),
(@NPC_KMOUNTAINEER*100,9,3,0,0,0,100,0,2000,2000,0,0,46,5,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Kharanos Mountaineer - On Script - Move Forward"),
(@NPC_KMOUNTAINEER*100,9,4,0,0,0,100,0,4000,4000,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Kharanos Mountaineer - On Script - Forced Despawn");
-- Texts
DELETE FROM `creature_text` WHERE `entry`=@NPC_KMOUNTAINEER;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC_KMOUNTAINEER,0,0,"%s gasps for air.",16,0,100,0,0,0,"Kharanos Mountaineer"),
(@NPC_KMOUNTAINEER,1,0,"Thank you for freeing me!",12,0,100,0,0,0,"Kharanos Mountaineer");
DELETE FROM `conditions` WHERE `SourceEntry`=@SPELL_BURN AND `SourceTypeOrReferenceId`=13;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13,1,@SPELL_BURN,0,31,1,3,@NPC_CTOTEM,0,0,'',"Burn Constriction Totem");

-- Quests - Rams on the Lam has been scripted (Feedback #761)
-- Stolen Ram
SET @NPC_RAM := 41539;
SET @NPC_FROSTMANERAIDER := 41544;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_RAM;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC_RAM;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_RAM,0,0,1,22,0,100,0,104,0,0,0,33,43064,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Stolen Ram - Emote Receive 'Whistle' - Quest Credit"),
(@NPC_RAM,0,1,0,61,0,100,0,0,0,0,0,41,2000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Stolen Ram - Emote Receive 'Whistle' - Forced Despawn");
DELETE FROM `creature_template_addon` WHERE `entry`=@NPC_FROSTMANERAIDER;

-- [SQL] Quests - Entombed in Ice corrected (Fixes #542)
SET @NPC_ICY_TOMB := 41768;
SET @NPC_FROZEN_MOUNTAINEER := 41763;
SET @SPELL_FREEZE_ANIM := 16245;
SET @CGUID := 49822;
UPDATE `creature` SET `spawntimesecs` = 90, `curhealth` = 141, `spawndist` = 0, `MovementType` = 0 WHERE `id` = @NPC_FROZEN_MOUNTAINEER;
DELETE FROM `creature` WHERE `id` = @NPC_ICY_TOMB;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`) VALUES
(@CGUID+0, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5736.01, -2054.17, 396.456, 1.09956, 90, 0, 0, 141, 0),
(@CGUID+1, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5655.28, -2087.21, 400.642, 1.37881, 90, 0, 0, 141, 0),
(@CGUID+2, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5618.07, -2052.23, 396.456, 0.383972, 90, 0, 0, 141, 0),
(@CGUID+3, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5666.14, -2033.83, 396.37, 4.43314, 90, 0, 0, 141, 0),
(@CGUID+4, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5636.21, -2033.33, 396.281, 4.83456, 90, 0, 0, 141, 0),
(@CGUID+5, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5598.48, -2018.34, 396.309, 5.2709, 90, 0, 0, 141, 0),
(@CGUID+6, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5563.93, -2025.25, 396.456, 2.23402, 90, 0, 0, 141, 0),
(@CGUID+7, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5604, -1969.36, 396.208, 5.37561, 90, 0, 0, 141, 0),
(@CGUID+8, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5567.8, -1975.57, 396.456, 2.44346, 90, 0, 0, 141, 0),
(@CGUID+9, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5654.29, -1995.44, 396.177, 2.70526, 90, 0, 0, 141, 0),
(@CGUID+10, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5687.05, -1927.1, 396.456, 5.13127, 90, 0, 0, 141, 0),
(@CGUID+11, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5664.72, -1957.09, 396.254, 1.51844, 90, 0, 0, 141, 0),
(@CGUID+12, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5656.17, -1934.24, 396.194, 1.72788, 90, 0, 0, 141, 0),
(@CGUID+13, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5659.23, -1885.82, 396.456, 4.18879, 90, 0, 0, 141, 0),
(@CGUID+14, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5627.28, -1906.04, 396.456, 3.82227, 90, 0, 0, 141, 0),
(@CGUID+15, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5700.1, -1999.36, 396.456, 2.21657, 90, 0, 0, 141, 0),
(@CGUID+16, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5715.08, -1960.67, 396.456, 5.63741, 90, 0, 0, 141, 0),
(@CGUID+17, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5744.9, -1994.26, 396.456, 0.20944, 90, 0, 0, 141, 0),
(@CGUID+18, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5775.43, -1991.7, 400.771, 3.68265, 90, 0, 0, 141, 0),
(@CGUID+19, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5564.92, -1930.33, 396.456, 3.68265, 90, 0, 0, 141, 0),
(@CGUID+20, @NPC_ICY_TOMB, 0, 1, 1, 0, 0, -5614.44, -1932.03, 396.29, 5.96903, 90, 0, 0, 141, 0);
DELETE FROM `creature_template_aura` WHERE `entry` IN (@NPC_ICY_TOMB, @NPC_FROZEN_MOUNTAINEER);
INSERT INTO `creature_template_aura` (`entry`, `aura`) VALUES
(@NPC_ICY_TOMB, @SPELL_FREEZE_ANIM),  -- Freeze Anim
(@NPC_FROZEN_MOUNTAINEER, @SPELL_FREEZE_ANIM);
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry` IN (@NPC_ICY_TOMB, @NPC_FROZEN_MOUNTAINEER);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_ICY_TOMB,@NPC_FROZEN_MOUNTAINEER,@NPC_FROZEN_MOUNTAINEER*100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_ICY_TOMB,0,0,1,1,0,100,1,1000,1000,10000,10000,8,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Icy Tomb - Out of Combat - React State Passive"),
(@NPC_ICY_TOMB,0,1,2,61,0,100,1,0,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Icy Tomb - Out of Combat - Avoid Combat Movement"),
(@NPC_ICY_TOMB,0,2,0,61,0,100,1,0,0,0,0,20,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Icy Tomb - Out of Combat - Avoid Autoattack"),
(@NPC_ICY_TOMB,0,3,4,6,0,100,0,0,0,0,0,45,1,1,0,0,0,0,19,@NPC_FROZEN_MOUNTAINEER,8,0,0,0,0,0,"Nepenthe-Icy Tomb - On Death - Set Data Frozen Mountaineer"),
(@NPC_ICY_TOMB,0,4,0,61,0,100,0,0,0,0,0,12,@NPC_FROZEN_MOUNTAINEER,8,10000,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Icy Tomb - On Death - Summon new Frozen Mountaineer"),
(@NPC_FROZEN_MOUNTAINEER,0,0,0,38,0,100,0,1,1,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Frozen Mountaineer - On Data Set - Despawn default Frozen Mountaineer"),
(@NPC_FROZEN_MOUNTAINEER,0,1,0,54,0,100,0,0,0,0,0,80,@NPC_FROZEN_MOUNTAINEER*100,0,2,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Frozen Mountaineer - Just Summoned - Run Script"),
(@NPC_FROZEN_MOUNTAINEER*100,9,0,0,0,0,100,0,0,0,0,0,28,@SPELL_FREEZE_ANIM,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Frozen Mountaineer - On Script - Remove Aura Freeze Anim"),
(@NPC_FROZEN_MOUNTAINEER*100,9,1,0,0,0,100,0,1000,1000,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Frozen Mountaineer - On Script - Say Line 0"),
(@NPC_FROZEN_MOUNTAINEER*100,9,2,0,0,0,100,0,5000,5000,0,0,46,15,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Frozen Mountaineer - On Script - Move Forward"),
(@NPC_FROZEN_MOUNTAINEER*100,9,3,0,0,0,100,0,5000,5000,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Frozen Mountaineer - On Script - Forced Despawn");
DELETE FROM `creature_text` WHERE `entry`=@NPC_FROZEN_MOUNTAINEER;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC_FROZEN_MOUNTAINEER,0,0,"%s gasps for air and shivers in the cold.",16,0,100,20,0,0,"Frozen Mountaineer");  -- ONESHOT_BEG

-- Quests - Aid for the Wounded has been scripted (Feedback #3165)
-- Wounded Coldridge Mountaineer
SET @NPC_WMOUNTAINEER := 37080;
SET @SPELL_AID := 69855;
SET @QUEST := 24471;
UPDATE `quest_template` SET `RequiredNpcOrGo1`=37079 WHERE `Id`=@QUEST;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_WMOUNTAINEER;
DELETE FROM `creature_template_addon` WHERE `entry`=@NPC_WMOUNTAINEER;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_WMOUNTAINEER,@NPC_WMOUNTAINEER*100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_WMOUNTAINEER,0,0,0,1,0,100,1,0,0,0,0,90,8,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Wounded Coldridge Mountaineer - Out of Combat - Emote State Kneel"),
(@NPC_WMOUNTAINEER,0,1,0,8,0,100,0,@SPELL_AID,0,0,0,80,@NPC_WMOUNTAINEER*100,0,2,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Wounded Coldridge Mountaineer - On Spellhit - Run Script"),
(@NPC_WMOUNTAINEER*100,9,0,0,0,0,100,0,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Wounded Coldridge Mountaineer - On Script - Set Phase 1"),
(@NPC_WMOUNTAINEER*100,9,1,0,0,0,100,0,0,0,0,0,91,8,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Wounded Coldridge Mountaineer - On Script - Stop Kneeling"),
(@NPC_WMOUNTAINEER*100,9,2,0,0,0,100,0,1500,1500,0,0,66,0,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Wounded Coldridge Mountaineer - On Script - Face Player"),
(@NPC_WMOUNTAINEER*100,9,3,0,0,0,100,0,1500,1500,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Wounded Coldridge Mountaineer - On Script - Say Line 0"),
(@NPC_WMOUNTAINEER*100,9,4,0,0,0,100,0,4500,4500,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Wounded Coldridge Mountaineer - On Script - Forced Despawn");
-- Texts
DELETE FROM `creature_text` WHERE `entry`=@NPC_WMOUNTAINEER;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC_WMOUNTAINEER,0,0,"Ahh, much better. I owe ya one.",12,0,100,0,0,0,"Wounded Coldridge Mountaineer"),
(@NPC_WMOUNTAINEER,0,1,"Thanks! I can make it from here.",12,0,100,0,0,0,"Wounded Coldridge Mountaineer"),
(@NPC_WMOUNTAINEER,0,2,"Yer a lifesaver! For Ironforge!",12,0,100,0,0,0,"Wounded Coldridge Mountaineer");
DELETE FROM `conditions` WHERE `SourceEntry`=@SPELL_AID AND `SourceTypeOrReferenceId`=17;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17,0,@SPELL_AID,0,31,1,3,@NPC_WMOUNTAINEER,0,0,'',"Heal Wounded Mountaineer");

-- Quests - No Tanks! has been scripted (Feedback #1740)
-- Repaired Mechano-Tank
SET @NPC := 42224;
SET @SPELL_DESTROY_TANK := 79751;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,1,8,0,100,0,@SPELL_DESTROY_TANK,0,0,0,33,@NPC,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Repaired Mechano-Tank - On Spellhit - Quest Credit"),
(@NPC,0,1,2,61,0,100,0,0,0,0,0,37,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Repaired Mechano-Tank - On Spellhit - Die"),
(@NPC,0,2,0,61,0,100,0,0,0,0,0,41,5000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Repaired Mechano-Tank - On Spellhit - Forced Despawn");
-- Dun Morogh Mountaineer
UPDATE `creature_template` SET `faction_A` = 57, `faction_H` = 57, `mindmg` = 1007, `maxdmg` = 1354, `attackpower` = 1571, `baseattacktime` = 2000 WHERE `entry` = 51912;
UPDATE `creature_template` SET `faction_A` = 57, `faction_H` = 57, `lootid` = 0 WHERE `entry` = 41897;
DELETE FROM `creature_loot_template` WHERE `entry`=41897;

-- Quests - One More Thing completion event has been scripted (Feedback #3166)
-- Captain Tread Sparknozzle
SET @NPC_CAPT := 42489;
SET @NPC_HINK := 42491;
SET @NPC_KELS := 42366;
SET @NPC_ELGI := 42490;
SET @NPC_TINKER2 := 42317;
SET @NPC_IMAGE_RAZLO := 42505;
SET @GUID_IMAGE_RAZLO := 243621;
SET @NPC_IMAGE_MECHANO_TANK := 43133;
SET @NPC_IMAGE_DWARF_MOUNTAINEER := 43132;
SET @NPC_IMAGE_GNOMEREGAN_INFANTRY := 43131;
DELETE FROM `creature` WHERE `id`=@NPC_TINKER2;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(244430, @NPC_TINKER2, 0, 1, 1, 0, 0, -5130.65, 488.842, 395.586, 2.30383, 120, 0, 0, 68713600, 0, 0, 0, 0, 0);
DELETE FROM `creature` WHERE `id`=@NPC_IMAGE_RAZLO;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@GUID_IMAGE_RAZLO, @NPC_IMAGE_RAZLO, 0, 1, 1, 0, 0, -5138.53, 499.372, 396.624, 4.10152, 120, 0, 0, 98, 115, 0, 0, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` IN (SELECT `guid` FROM `creature` WHERE `id`=@NPC_IMAGE_RAZLO);
DELETE FROM `creature_template_addon` WHERE `entry` IN (@NPC_IMAGE_RAZLO,@NPC_IMAGE_MECHANO_TANK);
INSERT INTO `creature_template_addon` (`entry`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(@NPC_IMAGE_RAZLO,0,0,0,0,16245), -- Freeze Anim
(@NPC_IMAGE_MECHANO_TANK,0,0,0,0,16245); -- Freeze Anim
-- make images unselectable
UPDATE `creature_template` SET `unit_flags` = `unit_flags`|33554432 WHERE `entry` IN (@NPC_IMAGE_RAZLO, @NPC_IMAGE_GNOMEREGAN_INFANTRY, @NPC_IMAGE_DWARF_MOUNTAINEER);
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry` IN (@NPC_CAPT, @NPC_IMAGE_RAZLO, @NPC_IMAGE_GNOMEREGAN_INFANTRY, @NPC_IMAGE_DWARF_MOUNTAINEER);
UPDATE `creature_template` SET `modelid1` = 35960, `modelid2` = 0 WHERE `entry`=@NPC_IMAGE_MECHANO_TANK;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_CAPT,@NPC_CAPT*100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_CAPT,0,0,0,1,0,100,0,15000,30000,120000,160000,80,@NPC_CAPT*100,0,2,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Captain Tread Sparknozzle - Out of Combat - Run Script"),
(@NPC_CAPT*100,9,0,0,0,0,100,0,0,0,0,0,70,0,0,0,0,0,0,10,@GUID_IMAGE_RAZLO,@NPC_IMAGE_RAZLO,0,0,0,0,0,"Nepenthe-Captain Tread Sparknozzle - On Script - Respawn Image of Razlo Crushcog"),
(@NPC_CAPT*100,9,1,0,0,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Captain Tread Sparknozzle - On Script - Say Line 0"),
(@NPC_CAPT*100,9,2,0,0,0,100,0,9000,9000,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Captain Tread Sparknozzle - On Script - Say Line 1"),
(@NPC_CAPT*100,9,3,0,0,0,100,0,8000,8000,0,0,1,0,0,0,0,0,0,9,@NPC_HINK,0,5,0,0,0,0,"Nepenthe-Hinkles Fastblast - On Script - Say Line 0"),
(@NPC_CAPT*100,9,4,0,0,0,100,0,0,0,0,0,12,@NPC_IMAGE_MECHANO_TANK,1,42000,0,0,0,8,0,0,0,-5137.05,499.769,396.4573,3.106686,"Nepenthe-Captain Tread Sparknozzle - On Script - Summon Image of Mechano-Tank"),
(@NPC_CAPT*100,9,5,0,0,0,100,0,0,0,0,0,12,@NPC_IMAGE_MECHANO_TANK,1,42000,0,0,0,8,0,0,0,-5138.56,500.873,396.4693,4.34587,"Nepenthe-Captain Tread Sparknozzle - On Script - Summon Image of Mechano-Tank"),
(@NPC_CAPT*100,9,6,0,0,0,100,0,0,0,0,0,45,1,1,0,0,0,0,19,@NPC_IMAGE_RAZLO,0,0,0,0,0,0,"Nepenthe-Captain Tread Sparknozzle - On Script - Set Data Image of Razlo Crushcog"),
(@NPC_CAPT*100,9,7,0,0,0,100,0,9000,9000,0,0,1,1,0,0,0,0,0,9,@NPC_HINK,0,5,0,0,0,0,"Nepenthe-Hinkles Fastblast - On Script - Say Line 1"),
(@NPC_CAPT*100,9,8,0,0,0,100,0,9000,9000,0,0,1,0,0,0,0,0,0,9,@NPC_KELS,0,5,0,0,0,0,"Nepenthe-Kelsey Steelspark - On Script - Say Line 0"),
(@NPC_CAPT*100,9,9,0,0,0,100,0,9000,9000,0,0,1,0,0,0,0,0,0,9,@NPC_ELGI,0,5,0,0,0,0,"Nepenthe-Elgin Clickspring - On Script - Say Line 0"),
(@NPC_CAPT*100,9,10,0,0,0,100,0,0,0,0,0,12,@NPC_IMAGE_DWARF_MOUNTAINEER,1,22000,0,0,0,8,0,0,0,-5138.01,498.262,396.4203,1.500983,"Nepenthe-Captain Tread Sparknozzle - On Script - Summon Image of Dwarf Mountaineer"),
(@NPC_CAPT*100,9,11,0,0,0,100,0,0,0,0,0,12,@NPC_IMAGE_DWARF_MOUNTAINEER,1,22000,0,0,0,8,0,0,0,-5138.44,498.372,396.4214,1.413717,"Nepenthe-Captain Tread Sparknozzle - On Script - Summon Image of Dwarf Mountaineer"),
(@NPC_CAPT*100,9,12,0,0,0,100,0,0,0,0,0,12,@NPC_IMAGE_DWARF_MOUNTAINEER,1,22000,0,0,0,8,0,0,0,-5138.9,498.429,396.4363,1.413717,"Nepenthe-Captain Tread Sparknozzle - On Script - Summon Image of Dwarf Mountaineer"),
(@NPC_CAPT*100,9,13,0,0,0,100,0,0,0,0,0,12,@NPC_IMAGE_GNOMEREGAN_INFANTRY,1,26000,0,0,0,8,0,0,0,-5139.73,499.203,396.4203,6.195919,"Nepenthe-Captain Tread Sparknozzle - On Script - Summon Image of Gnomeregan Infantry"),
(@NPC_CAPT*100,9,14,0,0,0,100,0,0,0,0,0,12,@NPC_IMAGE_GNOMEREGAN_INFANTRY,1,26000,0,0,0,8,0,0,0,-5139.75,499.571,396.4563,6.195919,"Nepenthe-Captain Tread Sparknozzle - On Script - Summon Image of Gnomeregan Infantry"),
(@NPC_CAPT*100,9,15,0,0,0,100,0,0,0,0,0,12,@NPC_IMAGE_GNOMEREGAN_INFANTRY,1,26000,0,0,0,8,0,0,0,-5139.76,499.977,396.4433,6.195919,"Nepenthe-Captain Tread Sparknozzle - On Script - Summon Image of Gnomeregan Infantry"),
(@NPC_CAPT*100,9,16,0,0,0,100,0,1500,1500,0,0,45,2,2,0,0,0,0,19,@NPC_IMAGE_RAZLO,0,0,0,0,0,0,"Nepenthe-Captain Tread Sparknozzle - On Script - Set Data Image of Razlo Crushcog"),
(@NPC_CAPT*100,9,17,0,0,0,100,0,9000,9000,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Captain Tread Sparknozzle - On Script - Say Line 2"),
(@NPC_CAPT*100,9,18,0,0,0,100,0,0,0,0,0,45,1,1,0,0,0,0,19,@NPC_IMAGE_DWARF_MOUNTAINEER,0,0,0,0,0,0,"Nepenthe-Captain Tread Sparknozzle - On Script - Set Data Image of Dwarf Mountaineer"),
(@NPC_CAPT*100,9,19,0,0,0,100,0,13000,13000,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Captain Tread Sparknozzle - On Script - Say Line 3"),
(@NPC_CAPT*100,9,20,0,0,0,100,0,0,0,0,0,45,1,1,0,0,0,0,19,@NPC_IMAGE_GNOMEREGAN_INFANTRY,0,0,0,0,0,0,"Nepenthe-Captain Tread Sparknozzle - On Script - Set Data Image of Gnomeregan Infantry"),
(@NPC_CAPT*100,9,21,0,0,0,100,0,0,0,0,0,45,3,3,0,0,0,0,19,@NPC_IMAGE_RAZLO,0,0,0,0,0,0,"Nepenthe-Captain Tread Sparknozzle - On Script - Set Data Image of Razlo Crushcog");
-- Texts
DELETE FROM `creature_text` WHERE `entry` IN (@NPC_CAPT,@NPC_HINK,@NPC_KELS,@NPC_ELGI);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC_CAPT,0,0,"We can't afford to let our enemy, Thermaplugg, continue to tighten his grip on the rest of Gnomeregan.",12,0,100,1,0,0,"Captain Tread Sparknozzle"),
(@NPC_CAPT,1,0,"He has ordered his crony, Razlo Crushcog, to keep us on the defensive. Hinkles, what's the latest intelligence on Razlo and his men?",12,0,100,1,0,0,"Captain Tread Sparknozzle"),
(@NPC_HINK,0,0,"It's been hard for our scouts to get information on what Crushcog is planning.",12,0,100,1,0,0,"Hinkles Fastblast"),
(@NPC_HINK,1,0,"What we do know is that Crushcog is using an advanced mechano-tank prototype and his irradiated gnomes are trying to repair more at the old arsenal.",12,0,100,1,0,0,"Hinkles Fastblast"),
(@NPC_KELS,0,0,"We're going to have a hard time handling this one alone. I sent a message to the dwarven capital of Ironforge asking for their support.",12,0,100,1,0,0,"Kelsey Steelspark"),
(@NPC_ELGI,0,0,"A messenger reached us from Brewnall Village this morning. The dwarves are sending a squad of mountaineers to help us.",12,0,100,25,0,0,"Elgin Clickspring"),
(@NPC_CAPT,2,0,"We have to be in position to strike against Crushcog when the mountaineers arrive.",12,0,100,1,0,0,"Captain Tread Sparknozzle"),
(@NPC_CAPT,3,0,"With Crushcog defeated, Thermaplugg is sure to be quaking in his mechano-tank, and rightly so. You're next, Thermaplugg. You're next!",12,0,100,25,0,0,"Captain Tread Sparknozzle");
-- Image of Razlo Crushcog
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC_IMAGE_RAZLO AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_IMAGE_RAZLO,0,0,0,38,0,100,0,1,1,0,0,53,0,@NPC_IMAGE_RAZLO,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Image of Razlo Crushcog - On Data Set - Start WP"),
(@NPC_IMAGE_RAZLO,0,1,0,40,0,100,0,2,@NPC_IMAGE_RAZLO,0,0,66,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Image of Razlo Crushcog - On WP 2 - Set Orientation"),
(@NPC_IMAGE_RAZLO,0,2,0,38,0,100,0,2,2,0,0,5,5,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Image of Razlo Crushcog - On Data Set - Emote Exclamation"),
(@NPC_IMAGE_RAZLO,0,3,4,38,0,100,0,3,3,0,0,5,4,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Image of Razlo Crushcog - On Data Set - Emote Cheer"),
(@NPC_IMAGE_RAZLO,0,4,0,61,0,100,0,0,0,0,0,41,6000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Image of Razlo Crushcog - On Data Set - Forced Despawn");
-- Waypoint
DELETE FROM `waypoints` WHERE `entry`=@NPC_IMAGE_RAZLO;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(@NPC_IMAGE_RAZLO,1,-5138.42,499.655,395.8836,"Image of Razlo Crushcog"),
(@NPC_IMAGE_RAZLO,2,-5136.975,500.2635,396.004,"Image of Razlo Crushcog");
-- Image of Dwarf Mountaineer
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC_IMAGE_DWARF_MOUNTAINEER AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_IMAGE_DWARF_MOUNTAINEER,0,0,0,38,0,100,0,1,1,0,0,5,4,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Image of Dwarf Mountaineer - On Data Set - Emote Cheer");
-- Image of Gnomeregan Infantry
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC_IMAGE_GNOMEREGAN_INFANTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_IMAGE_GNOMEREGAN_INFANTRY,0,0,0,38,0,100,0,1,1,0,0,5,4,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Image of Gnomeregan Infantry - On Data Set - Emote Cheer");

-- Npcs - Teo Hammerstorm will now have a script to randomly summon his totems
SET @NPC_TEO := 37115;
SET @SPELL_ELEMENTAL_TOTEM := 79429;
SET @SPELL_FLAMETONGUE_TOTEM := 79431;
SET @SPELL_WINDFURY_TOTEM := 78159;
SET @SPELL_TREMOR_TOTEM := 78156;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_TEO;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_TEO,@NPC_TEO*100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_TEO,0,0,0,1,0,100,0,2000,2000,310000,310000,80,@NPC_TEO*100,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Teo Hammerstorm - OOC - Run Script"),
(@NPC_TEO*100,9,0,0,0,0,100,0,1000,1000,0,0,11,@SPELL_ELEMENTAL_TOTEM,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Teo Hammerstorm - On Script - Cast Elemental Resistance Totem"),
(@NPC_TEO*100,9,1,0,0,0,100,0,1000,1000,0,0,11,@SPELL_FLAMETONGUE_TOTEM,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Teo Hammerstorm - On Script - Cast Flametongue Totem"),
(@NPC_TEO*100,9,2,0,0,0,100,0,1000,1000,0,0,11,@SPELL_WINDFURY_TOTEM,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Teo Hammerstorm - On Script - Cast Windfury Totem"),
(@NPC_TEO*100,9,3,0,0,0,100,0,1000,1000,0,0,11,@SPELL_TREMOR_TOTEM,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Teo Hammerstorm - On Script - Cast Tremor Totem");

-- Npcs - Yarlyn Amberstill will now have a script to chase Fluffy (Fixes #3167)
-- Fluffy
SET @NPC_FLUFFY := 1352;
UPDATE `creature_template` SET `exp`=2,`minlevel`=35,`maxlevel`=35,`unit_flags`=`unit_flags`|2048,`unit_class`=0,`speed_run` = 0.85814,`AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_FLUFFY;
UPDATE `creature_template_addon` SET `bytes1`=0,`bytes2`=1,`mount`=0,`emote`=0,`auras`='' WHERE `entry`=@NPC_FLUFFY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC_FLUFFY;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_FLUFFY,0,0,0,1,0,100,1,2000,2000,0,0,53,1,@NPC_FLUFFY,2,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Fluffy - Out of Combat - Start WP"); -- Running and CanRepeat
-- Waypoint
DELETE FROM `waypoints` WHERE `entry`=@NPC_FLUFFY;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(@NPC_FLUFFY,1,-5540.473633,-1303.131958,398.989197,"Fluffy"),
(@NPC_FLUFFY,2,-5534.039063,-1309.336670,399.399048,"Fluffy"),
(@NPC_FLUFFY,3,-5528.043457,-1310.621582,402.100983,"Fluffy"),
(@NPC_FLUFFY,4,-5519.485840,-1308.773682,403.696869,"Fluffy"),
(@NPC_FLUFFY,5,-5534.207031,-1317.866455,400.123535,"Fluffy"),
(@NPC_FLUFFY,6,-5545.675293,-1320.292114,398.653046,"Fluffy"),
(@NPC_FLUFFY,7,-5552.579590,-1312.840942,398.575714,"Fluffy"),
(@NPC_FLUFFY,8,-5550.864746,-1306.694580,398.274536,"Fluffy");
-- Yarlyn Amberstill
SET @NPC_YARLYN := 1263;
UPDATE `creature_template` SET `exp`=2,`minlevel`=55,`maxlevel`=55,`unit_flags`=`unit_flags`|2048,`unit_class`=0,`speed_run`=0.85714,`AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_YARLYN;
UPDATE `creature_template_addon` SET `bytes1`=0,`bytes2`=1,`mount`=0,`emote`=0,`auras`='' WHERE `entry`=@NPC;
DELETE FROM `creature_equip_template` WHERE `entry`=@NPC_YARLYN;
INSERT INTO `creature_equip_template` (`entry`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES
(@NPC_YARLYN,0,0,768);
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC_YARLYN;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_YARLYN,0,0,0,1,0,100,1,3000,3000,0,0,53,1,@NPC_FLUFFY,2,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Yarlyn Amberstill - Out of Combat - Follow Fluffy"),
(@NPC_YARLYN,0,1,0,1,0,100,0,2000,2000,9000,11000,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Yarlyn Amberstill - Out of Combat - Say Line 0 (random)");
-- Texts (dwarvish - 6)
DELETE FROM `creature_text` WHERE `entry`=@NPC_YARLYN;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC_YARLYN,0,0,"C'mon, Fluffy! I just want to give you a bath!",12,6,100,0,0,0,"Yarlyn Amberstill"),
(@NPC_YARLYN,0,1,"Carrots every day for a year if you stop!",12,6,100,0,0,0,"Yarlyn Amberstill"),
(@NPC_YARLYN,0,2,"Come back, Fluffy!",12,6,100,0,0,0,"Yarlyn Amberstill"),
(@NPC_YARLYN,0,3,"Don't make me get my pappy to catch you! He likes to catch bunnies using his rifle!",12,6,100,0,0,0,"Yarlyn Amberstill"),
(@NPC_YARLYN,0,4,"Fluffy! Fluffy, stop running!",12,6,100,0,0,0,"Yarlyn Amberstill"),
(@NPC_YARLYN,0,5,"Why do you run from me, Fluffy?!",12,6,100,0,0,0,"Yarlyn Amberstill");

-- Quests - Missing in Action has been scripted (Feedback #727)
SET @NPC_CAPTURED_DEMOLITIONIST := 42645;
SET @GO_MAKESHIFT_CAGE := 204019;
SET @OGUID := 40005;
UPDATE `creature_template` SET `flags_extra` = 128 WHERE `entry` = 43422; --  Captured Demolitionist Credit was spawned
UPDATE `creature` SET `spawntimesecs` = 90 WHERE `id` = @NPC_CAPTURED_DEMOLITIONIST;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry` = 79606;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13,2,79606,0,31,1,3,@NPC_CAPTURED_DEMOLITIONIST,0,0,'',"Attacking - Makeshift Cage");
DELETE FROM `gameobject` WHERE `id` IN (@GO_MAKESHIFT_CAGE, 204023, 204024, 204025, 204026, 204027, 204028, 204029, 204030, 204031, 204032, 204033, 204034, 204035, 204036, 204037, 204038);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(@OGUID+0, @GO_MAKESHIFT_CAGE, 0, 1, 1, -5631.97, 372.951, 384.83, 3.03684, 0, 0, 0, 1, 90, 100, 1),
(@OGUID+1, @GO_MAKESHIFT_CAGE, 0, 1, 1, -5635.78, 428.49, 383.028, 2.44346, 0, 0, 0, 1, 90, 100, 1),
(@OGUID+2, @GO_MAKESHIFT_CAGE, 0, 1, 1, -5578.84, 305.986, 395.117, -0.471238, 0, 0, 0, 1, 90, 100, 1),
(@OGUID+3, @GO_MAKESHIFT_CAGE, 0, 1, 1, -5576.14, 500.811, 382.41, 1.11701, 0, 0, 0, 1, 90, 100, 1),
(@OGUID+4, @GO_MAKESHIFT_CAGE, 0, 1, 1, -5633.09, 334.359, 388.603, 1.0821, 0, 0, 0, 1, 90, 100, 1),
(@OGUID+5, @GO_MAKESHIFT_CAGE, 0, 1, 1, -5634.88, 304.021, 389.287, 1.48353, 0, 0, 0, 1, 90, 100, 1),
(@OGUID+6, @GO_MAKESHIFT_CAGE, 0, 1, 1, -5587.77, 444.616, 382.395, 0.296705, 0, 0, 0, 1, 90, 100, 1),
(@OGUID+7, @GO_MAKESHIFT_CAGE, 0, 1, 1, -5593.77, 394.306, 382.48, -1.97222, 0, 0, 0, 1, 90, 100, 1),
(@OGUID+8, @GO_MAKESHIFT_CAGE, 0, 1, 1, -5582.34, 359.095, 388.245, 2.42601, 0, 0, 0, 1, 90, 100, 1),
(@OGUID+9, @GO_MAKESHIFT_CAGE, 0, 1, 1, -5602.97, 336.608, 389.04, 2.60053, 0, 0, 0, 1, 90, 100, 1);
DELETE FROM `smart_scripts` WHERE `entryorguid` = @GO_MAKESHIFT_CAGE AND `source_type` = 1;
DELETE FROM `smart_scripts` WHERE `entryorguid` = @NPC_CAPTURED_DEMOLITIONIST AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `event_type`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `comment`) VALUES
(@GO_MAKESHIFT_CAGE, 1, 37, 58, 4, @NPC_CAPTURED_DEMOLITIONIST, 1, 0, 0, 0, 1, 'Makeshift Cage - Cage part Say when Npc is Free (random)- q26284'),
(@NPC_CAPTURED_DEMOLITIONIST, 0, 37, 58, 5, @GO_MAKESHIFT_CAGE, 1000, 1, 10, 0, 1, 'Captured Demolitionist - Npc part Run away when free and Despawn - q26284');
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI', `ScriptName` = '' WHERE `entry` = @GO_MAKESHIFT_CAGE;
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = @NPC_CAPTURED_DEMOLITIONIST;
UPDATE `gameobject_template` SET `AIName` = '', `ScriptName` = '' WHERE `entry` IN (204023, 204024, 204025, 204026, 204027, 204028, 204029, 204030, 204031, 204032, 204033, 204034, 204035, 204036, 204037, 204038);
DELETE FROM `creature_text` WHERE `entry`=@NPC_CAPTURED_DEMOLITIONIST;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC_CAPTURED_DEMOLITIONIST,0,0,"Finally, someone who's not a trogg!",12,0,100,0,0,0,"Captured Demolitionist"),
(@NPC_CAPTURED_DEMOLITIONIST,0,1,"I don't ever want to smell unwashed trogg again!",12,0,100,0,0,0,"Captured Demolitionist"),
(@NPC_CAPTURED_DEMOLITIONIST,0,2,"I'm free! I'm really free!",12,0,100,0,0,0,"Captured Demolitionist"),
(@NPC_CAPTURED_DEMOLITIONIST,0,3,"Thank you for breaking me out of here!",12,0,100,0,0,0,"Captured Demolitionist"),
(@NPC_CAPTURED_DEMOLITIONIST,0,4,"Thanks. Now, let's blow up that cave!",12,0,100,0,0,0,"Captured Demolitionist"),
(@NPC_CAPTURED_DEMOLITIONIST,0,5,"You have no idea how happy I am to see you!",12,0,100,0,0,0,"Captured Demolitionist");

-- Quests - Paint it Black corrected (Fixes #3171)
SET @NPC_CRUSHCOG_SENTRY_BOT := 42291;
SET @KILL_CREDIT := 42796;
SET @SPELL_BLIND_SENTRY := 79781;
SET @SPELL_WAILING_SIREN := 84152;
SET @QUEST_PAINT_IT_BLACK := 26342;
UPDATE `quest_template` SET `RequiredNpcOrGo1` = @KILL_CREDIT WHERE `Id` = @QUEST_PAINT_IT_BLACK;
UPDATE `creature_template` SET `minlevel`=4,`maxlevel`=5,`mindmg` = 9, `maxdmg` = 15, `attackpower` = 36,`baseattacktime`=2000,`mingold`=1,`maxgold`=3,`AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_CRUSHCOG_SENTRY_BOT ;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_CRUSHCOG_SENTRY_BOT, @NPC_CRUSHCOG_SENTRY_BOT*100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_CRUSHCOG_SENTRY_BOT,0,0,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_WAILING_SIREN,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Crushcog Sentry-Bot - In Combat - Cast Wailing Siren"),
(@NPC_CRUSHCOG_SENTRY_BOT,0,1,2,8,0,100,0,@SPELL_BLIND_SENTRY,0,10000,10000,33,@KILL_CREDIT,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Crushcog Sentry-Bot - On Spellhit - KC"),
(@NPC_CRUSHCOG_SENTRY_BOT,0,2,0,61,0,100,0,0,0,0,0,80,@NPC_CRUSHCOG_SENTRY_BOT*100,2,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Sentry-Bot - On Spellhit - Run Script"),
(@NPC_CRUSHCOG_SENTRY_BOT*100,9,0,0,0,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Sentry-Bot - On Script - Say Line 0"),
(@NPC_CRUSHCOG_SENTRY_BOT*100,9,1,0,0,0,100,0,0,0,0,0,20,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Sentry-Bot - On Script - Disable Autoattack"),
(@NPC_CRUSHCOG_SENTRY_BOT*100,9,2,0,0,0,100,0,0,0,0,0,46,20,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Sentry-Bot - On Script - Move Forward 20 yards"),
(@NPC_CRUSHCOG_SENTRY_BOT*100,9,3,0,0,0,100,0,4000,4000,0,0,37,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Sentry-Bot - On Script - Die");
DELETE FROM `creature_text` WHERE `entry`=@NPC_CRUSHCOG_SENTRY_BOT;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC_CRUSHCOG_SENTRY_BOT,0,0,"Optical sensor malfunction detected! Shutting down...",12,0,0,0,0,0,"Crushcog Sentry-Bot");
-- Spawns
DELETE FROM `creature` WHERE `id`=@NPC_CRUSHCOG_SENTRY_BOT;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(6614473, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5316.32, 234.19, 386.112, 0.957587, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614472, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5228.3, 187.759, 386.111, 5.91737, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614471, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5251.91, 194.584, 386.111, 5.28906, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614470, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5269.05, 215.738, 386.111, 0.663061, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614469, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5295.98, 198.327, 386.111, 0.37639, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614468, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5318.46, 199.797, 386.111, 5.77601, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614467, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5342.27, 198.172, 386.111, 1.33458, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614466, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5374.45, 173.049, 386.111, 4.47617, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614465, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5371.08, 140.822, 386.111, 1.0754, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614464, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5378.02, 115.236, 386.113, 2.4145, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614463, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5345.24, 115.686, 386.112, 3.52192, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614462, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5342.84, 144.094, 386.189, 5.24586, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614461, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5335.63, 176.233, 386.203, 4.11097, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614460, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5293.91, 176.496, 386.126, 2.72081, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614459, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5265.34, 165.002, 386.112, 3.30593, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614458, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5231.91, 169.62, 386.114, 4.24056, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614457, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5216, 200.796, 386.112, 4.24056, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614456, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5225.32, 239.339, 386.111, 5.56002, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614455, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5389.52, 154.644, 386.115, 4.92385, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614454, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5250.73, 253.673, 386.112, 0.325341, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614453, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5391.38, 179.448, 386.111, 0.820142, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614452, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5381.47, 203.505, 386.122, 0.13685, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614451, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5285.56, 249.865, 386.112, 0.706259, 90, 8, 0, 42, 0, 1, 0, 0, 0),
(6614450, @NPC_CRUSHCOG_SENTRY_BOT, 0, 1, 1, 0, 0, -5347.89, 222.313, 386.111, 0.0111861, 90, 8, 0, 42, 0, 1, 0, 0, 0);

-- Quests - Forced To Watch from Afar has been scripted (Feedback #726)
-- Note: no gossip conditions on retail
-- Mountaineer Dunstan
-- Mountaineer Lewin
-- Mountaineer Valgrum
SET @NPC_DUNS := 40991;
SET @NPC_LEWI := 40994;
SET @NPC_VALG := 41056;
SET @NPC_REMOTE_BOT := 41052;
SET @GOSSIP_DUNS := 11455;
SET @GOSSIP_LEWI := 11456;
SET @GOSSIP_VALG := 11457;
SET @TEXTID_DUNS := 15972;
SET @TEXTID_LEWI := 15973;
SET @TEXTID_VALG := 15974;
SET @NPC_THAR := 40950;
SET @NPC_GLYN := 40951;
SET @NPC_COVE := 41335;
SET @QUEST := 313;
UPDATE `creature_template` SET `minlevel`=9, `maxlevel`=9, `faction_A`=55, `faction_H`=55, `npcflag`=1, `baseattacktime`=2000, `AIName`='SmartAI', `ScriptName`='', `gossip_menu_id`=@GOSSIP_DUNS WHERE `entry`=@NPC_DUNS;
UPDATE `creature_template` SET `minlevel`=9, `maxlevel`=9, `faction_A`=55, `faction_H`=55, `npcflag`=1, `baseattacktime`=2000, `AIName`='SmartAI', `ScriptName`='', `gossip_menu_id`=@GOSSIP_LEWI WHERE `entry`=@NPC_LEWI;
UPDATE `creature_template` SET `minlevel`=9, `maxlevel`=9, `faction_A`=55, `faction_H`=55, `npcflag`=1, `baseattacktime`=2000, `AIName`='SmartAI', `ScriptName`='', `gossip_menu_id`=@GOSSIP_VALG WHERE `entry`=@NPC_VALG;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_THAR;
DELETE FROM `creature` WHERE `id` IN (@NPC_DUNS,@NPC_LEWI,@NPC_VALG);
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(145525, @NPC_DUNS, 0, 1, 1, 0, 0, -5475.65, -231.772, 354.444, 3.36621, 90, 0, 0, 176, 0, 0, 0, 0, 0),
(145521, @NPC_VALG, 0, 1, 1, 0, 0, -5387.79, -279.681, 363.202, 2.5761, 90, 0, 0, 176, 0, 0, 0, 0, 0),
(145993, @NPC_LEWI, 0, 1, 1, 0, 0, -5485.81, -252.5, 354.782, 2.09465, 90, 0, 0, 176, 0, 0, 0, 0, 0);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_DUNS,@NPC_LEWI,@NPC_VALG,@NPC_REMOTE_BOT,@NPC_THAR,@NPC_THAR*100);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@NPC_DUNS,0,0,1,62,0,100,0,@GOSSIP_DUNS,0,0,0,33,@NPC_DUNS,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Dunstan - On Gossip Select - Quest Credit"),
(@NPC_DUNS,0,1,2,61,0,100,0,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Dunstan - On Gossip Select - Say Line 0"),
(@NPC_DUNS,0,2,0,61,0,100,0,0,0,0,0,12,@NPC_REMOTE_BOT,8,10000,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Dunstan - On Gossip Select - Summon Remote Observation Bot"),
(@NPC_LEWI,0,0,1,62,0,100,0,@GOSSIP_LEWI,0,0,0,33,@NPC_LEWI,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Lewin - On Gossip Select - Quest Credit"),
(@NPC_LEWI,0,1,2,61,0,100,0,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Lewin - On Gossip Select - Say Line 0"),
(@NPC_LEWI,0,2,0,61,0,100,0,0,0,0,0,12,@NPC_REMOTE_BOT,8,10000,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Lewin - On Gossip Select - Summon Remote Observation Bot"),
(@NPC_VALG,0,0,1,62,0,100,0,@GOSSIP_VALG,0,0,0,33,@NPC_VALG,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Valgrum - On Gossip Select - Quest Credit"),
(@NPC_VALG,0,1,2,61,0,100,0,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Valgrum - On Gossip Select - Say Line 0"),
(@NPC_VALG,0,2,0,61,0,100,0,0,0,0,0,12,@NPC_REMOTE_BOT,8,10000,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Valgrum - On Gossip Select - Summon Remote Observation Bot"),
(@NPC_REMOTE_BOT,0,0,0,54,0,100,0,0,0,0,0,89,10,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Remote Observation Bot - Just Summoned - Set Random Movement");
-- Captain Tharran
-- Setting phase so we won't say the on quest complete line during the event
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@NPC_THAR,0,0,0,19,0,100,0,@QUEST,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Captain Tharran - On Quest Accept - Say Line 0"),
(@NPC_THAR,0,1,0,1,0,100,0,15000,55000,95000,680000,80,@NPC_THAR*100,0,0,0,0,2,1,0,0,0,0,0,0,0,"Nepenthe-Captain Tharran - Out of Combat - Run Script"),
(@NPC_THAR*100,9,0,0,0,0,100,0,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Captain Tharran - On Script - Set Phase 1"),
(@NPC_THAR*100,9,1,0,0,0,100,0,1000,1000,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Captain Tharran - On Script - Say Line 1"),
(@NPC_THAR*100,9,2,0,0,0,100,0,6000,6000,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Captain Tharran - On Script - Say Line 2"),
(@NPC_THAR*100,9,3,0,0,0,100,0,8000,8000,0,0,1,0,0,0,0,0,0,9,@NPC_GLYN,0,5,0,0,0,0,"Nepenthe-Quartermaster Glynna - On Script - Say Line 0"),
(@NPC_THAR*100,9,4,0,0,0,100,0,9000,9000,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Captain Tharran - On Script - Say Line 3"),
(@NPC_THAR*100,9,5,0,0,0,100,0,8000,8000,0,0,1,0,0,0,0,0,0,9,@NPC_COVE,0,5,0,0,0,0,"Nepenthe-Quartermaster Glynna - On Script - Say Line 0"),
(@NPC_THAR*100,9,6,0,0,0,100,0,9000,9000,0,0,1,1,0,0,0,0,0,9,@NPC_COVE,0,5,0,0,0,0,"Nepenthe-Quartermaster Glynna - On Script - Say Line 1"),
(@NPC_THAR*100,9,7,0,0,0,100,0,8000,8000,0,0,1,4,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Captain Tharran - On Script - Say Line 4"),
(@NPC_THAR*100,9,8,0,0,0,100,0,5000,5000,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Captain Tharran - On Script - Set Phase 0");
-- Texts
DELETE FROM `creature_text` WHERE `entry` IN (@NPC_DUNS,@NPC_LEWI,@NPC_VALG,@NPC_THAR,@NPC_GLYN,@NPC_COVE);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(@NPC_DUNS,0,0,"Tell Captain Tharran that I'll be back in Kharanos as soon as I've verified that the bot is working correctly.",12,0,100,66,0,0,"Mountaineer Dunstan"),
(@NPC_LEWI,0,0,"I can't wait to get out there and help in the fight against those trolls.",12,0,100,66,0,0,"Mountaineer Lewin"),
(@NPC_VALG,0,0,"We've been fighting nonstop since the cataclysm. It'll be nice to get a rest, if a brief one.",12,0,100,66,0,0,"Mountaineer Valgrum"),
(@NPC_THAR,0,0,"There's no substitute for live observers, but I need those mountaineers here. We'll have to rely on those little gnomish observer bots for now...",12,0,100,25,0,0,"Captain Tharran"),
(@NPC_THAR,1,0,"We're getting bogged down in the fight against the Frostmane trolls.",12,0,100,5,0,0,"Captain Tharran"), -- ONESHOT_EXCLAMATION
(@NPC_THAR,2,0,"Most of our steam tanks are out of commission and we're losing mountaineers quickly.",12,0,100,5,0,0,"Captain Tharran"), -- ONESHOT_EXCLAMATION
(@NPC_GLYN,0,0,"It won't be long before we're out of supplies too, Captain. Between the trolls and the yeti raids, we'll be down to half rations within a week.",12,0,100,274,0,0,"Quartermaster Glynna"), -- ONESHOT_NO
(@NPC_THAR,3,0,"We can't keep up the operation much longer. Delber and his team must launch their attack from Steelgrill's Depot as soon as possible!",12,0,100,25,0,0,"Captain Tharran"), -- ONESHOT_EXCLAMATION
(@NPC_COVE,0,0,"Delber and his men are working overtime, captain. He has devised a means of delivering an assault team to the rear flank of the battle.",12,0,100,0,0,0,"Covert Operative"),
(@NPC_COVE,1,0,"We've also secured air support for the operation and will be ready to deploy soon. If any of your men want to join the assault team, just send them to Delber.",12,0,100,0,0,0,"Covert Operative"),
(@NPC_THAR,4,0,"Glad to hear it. I'll send Delber any forces I can spare.",12,0,100,25,0,0,"Captain Tharran"); -- ONESHOT_YES
-- Link gossips
DELETE FROM `gossip_menu` WHERE `entry`=@GOSSIP_DUNS AND `text_id`=@TEXTID_DUNS;
INSERT INTO `gossip_menu` (`entry`,`text_id`) VALUES
(@GOSSIP_DUNS,@TEXTID_DUNS);
DELETE FROM `gossip_menu` WHERE `entry`=@GOSSIP_LEWI AND `text_id`=@TEXTID_LEWI;
INSERT INTO `gossip_menu` (`entry`,`text_id`) VALUES
(@GOSSIP_LEWI,@TEXTID_LEWI);
DELETE FROM `gossip_menu` WHERE `entry`=@GOSSIP_VALG AND `text_id`=@TEXTID_VALG;
INSERT INTO `gossip_menu` (`entry`,`text_id`) VALUES
(@GOSSIP_VALG,@TEXTID_VALG);
-- Static text
DELETE FROM `npc_text` WHERE `id` IN (@TEXTID_DUNS,@TEXTID_LEWI,@TEXTID_VALG);
INSERT INTO `npc_text` (`id`,`text0_0`) VALUES
(@TEXTID_DUNS,"Watch yourself down here, $c. These wendigos are savage fighters."),
(@TEXTID_LEWI,"I've seen enough of this cave for a lifetime. I ca't wait to get back to Kharanos."),
(@TEXTID_VALG,"Those wendigos are an annoyance, sure, but the real fight is with those Frostmane refugees.");
-- Text options
DELETE FROM `gossip_menu_option` WHERE `menu_id` IN (@GOSSIP_DUNS,@GOSSIP_LEWI,@GOSSIP_VALG);
INSERT INTO `gossip_menu_option` (`menu_id`,`id`,`option_icon`,`option_text`,`option_id`,`npc_option_npcflag`,`action_menu_id`,`action_poi_id`,`box_coded`,`box_money`,`box_text`) VALUES
(@GOSSIP_DUNS,0,0,"Captain Tharran wants you to deploy your remote observation bots and withdraw to Kharanos.",1,1,0,0,0,0,NULL),
(@GOSSIP_LEWI,0,0,"Captain Tharran wants you to deploy your remote observation bots and withdraw to Kharanos.",1,1,0,0,0,0,NULL),
(@GOSSIP_VALG,0,0,"Captain Tharran wants you to deploy your remote observation bots and withdraw to Kharanos.",1,1,0,0,0,0,NULL);

-- Quests - Down with Crushcog! has been scripted (Feedback #3172)
-- TODO: battle music doesn't play with this targeting, the technicians don't channel the spell, don't fight and don't mount on their guardians
SET @QUEST := 26364;
SET @NPC_TINKER := 42849;
SET @NPC_STONEGRIND := 42852;
SET @NPC_RAZLO := 42839;
SET @NPC_JARVI := 42353;
SET @NPC_INFANTRY := 42316;
SET @NPC_INFANTRY2 := 44701;
SET @NPC_MOUNTAINEER := 13076;
SET @NPC_TECH := 43230;
SET @NPC_GUARDIAN := 42294;
SET @NPC_DNDTARGET := 42929;
SET @SPELL_MEKKCANNON := 80098;
SET @SPELL_MEKKRAY := 80148;
SET @SPELL_STATE := 78858;
SET @SPELL_IRRADIATION_GUN := 84148;
SET @SPELL_MACHINE_GUN := 74438;
SET @SPELL_SUMMON_TECHNICIANS := 80783;
SET @SPELL_SHOOT := 80173;
SET @SPELL_CHARGE := 80183;
SET @SPELL_KC := 79931;
SET @SPELL_JUMP := 108516; -- TEMP
SET @FACTION_CRUSHCOG := 1771;
SET @CGUID := 5013939; -- needs 14
SET @OGUID := 6604140; -- needs 4
SET @NPC_IFBANNER := 203451;
SET @NPC_GRBANNER := 203756;
SET @GOSSIP := 11662;
SET @TEXT_ID := 16305;
-- [DND] Target
UPDATE `creature_template` SET `unit_flags`=33554432, `modelid2`=0, `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_DNDTARGET;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_DNDTARGET, @NPC_DNDTARGET*100, @NPC_DNDTARGET*100+1);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_DNDTARGET,0,0,0,11,0,100,0,0,0,0,0,87,@NPC_DNDTARGET*100,@NPC_DNDTARGET*100+1,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-[DND] Target - On Spawn - Random Script"),
(@NPC_DNDTARGET*100,9,0,0,0,0,100,0,0,0,0,0,45,2,2,0,0,0,0,19,@NPC_TINKER,0,0,0,0,0,0,"Nepenthe-[DND] Target - On Script 0 - Set Data 2 High Tinker Mekkatorque"),
(@NPC_DNDTARGET*100+1,9,1,0,0,0,100,0,0,0,0,0,45,3,3,0,0,0,0,19,@NPC_TINKER,0,0,0,0,0,0,"Nepenthe-[DND] Target - On Script 1 - Set Data 3 High Tinker Mekkatorque");
UPDATE `creature_template` SET `unit_flags` = 33554432, `spell1` = 80172 WHERE `entry` = 42935;
-- High Tinker Mekkatorque
DELETE FROM `creature` WHERE `id`=@NPC_TINKER;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(243585, @NPC_TINKER, 0, 1, 1, 31658, 0, -5313.49, 148.394, 389.51, 5.60251, 60, 0, 0, 5578000, 0, 0, 0, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` IN (SELECT `guid` FROM `creature` WHERE `id`=@NPC_TINKER);
DELETE FROM `creature_template_addon` WHERE `entry`=@NPC_TINKER;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(@NPC_TINKER, 0, 31692, 0, 0, 0, '');
UPDATE `creature_template` SET `faction_A`=64, `faction_H`=64, `unit_flags`=32832, `gossip_menu_id`=@GOSSIP, `npcflag`=`npcflag`|1, `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_TINKER;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_TINKER,@NPC_TINKER*100,@NPC_TINKER*100+1);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_TINKER,0,0,1,62,0,100,0,@GOSSIP,0,0,0,4,23798,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Gossip Select - Play Battle Music"),
(@NPC_TINKER,0,1,2,61,0,100,0,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Gossip Select - Store Target player"),
(@NPC_TINKER,0,2,3,61,0,100,0,0,0,0,0,8,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Gossip Select - Set React State Passive"),
(@NPC_TINKER,0,3,0,61,0,100,0,0,0,0,0,80,@NPC_TINKER*100,0,0,0,0,2,1,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Gossip Select - Run Script"),
(@NPC_TINKER*100,9,0,0,0,0,100,0,0,0,0,0,83,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Script - Remove Gossip Flag"),
(@NPC_TINKER*100,9,1,0,0,0,100,0,3000,3000,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Script - Say Line 0"),
(@NPC_TINKER*100,9,2,0,0,0,100,0,14000,14000,0,0,1,1,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Script - Say Line 1"),
(@NPC_TINKER*100,9,3,0,0,0,100,0,10000,10000,0,0,1,2,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Script - Say Line 2"),
(@NPC_TINKER*100,9,4,0,0,0,100,0,2000,2000,0,0,45,1,1,0,0,0,0,19,@NPC_STONEGRIND,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Script - Set Data 1 Mountaineer Stonegrind"),
(@NPC_TINKER*100,9,5,0,0,0,100,0,2000,2000,0,0,53,0,@NPC_TINKER,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Script - Start WP To Battle"),
(@NPC_TINKER,0,4,0,40,0,100,0,2,@NPC_TINKER,0,0,45,1,1,0,0,0,0,19,@NPC_RAZLO,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On WP - Set Data 1 Razlo Crushcog"),
(@NPC_TINKER,0,5,6,38,0,100,0,4,4,0,0,8,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Data Set 4 - Set React State Aggressive"),
(@NPC_TINKER,0,6,0,61,0,100,0,0,0,0,0,19,32832,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Data Set 4 - Remove Unit Flag"),
(@NPC_TINKER,0,7,8,38,0,100,0,1,1,0,0,80,@NPC_TINKER*100+1,0,0,0,0,2,1,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Data Set 1 - Run Script"),
(@NPC_TINKER,0,8,0,61,0,100,0,0,0,0,0,8,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Data Set 1 - Set React State Passive"),
(@NPC_TINKER*100+1,9,0,0,0,0,100,0,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,-5261.567871,119.521042,393.758728,5.50301,"Nepenthe-High Tinker Mekkatorque - On Script - Move To Point"), -- Face battlefield
(@NPC_TINKER*100+1,9,1,0,0,0,100,0,1500,1500,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Script - Say Line 3"),
(@NPC_TINKER*100+1,9,2,0,0,0,100,0,4000,4000,0,0,1,4,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Script - Say Line 4"),
(@NPC_TINKER*100+1,9,3,0,0,0,100,0,5000,5000,0,0,33,42860,0,0,0,0,0,18,20,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Script - KC on stored target"),
(@NPC_TINKER*100+1,9,4,0,0,0,100,0,8000,8000,0,0,4,23810,0,0,0,0,0,18,20,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Script - Stop Battle Music"),
(@NPC_TINKER*100+1,9,5,0,0,0,100,0,10000,10000,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Script - Forced Despawn"),
(@NPC_TINKER,0,8,9,54,0,100,0,0,0,0,0,53,0,@NPC_TINKER*10,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - Just Summoned - Start WP Quest Complete"),
(@NPC_TINKER,0,9,0,61,0,100,0,0,0,0,0,83,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - Just Summoned - Remove Gossip Flag"),
(@NPC_TINKER,0,10,11,38,0,100,0,2,2,0,0,11,@SPELL_MEKKCANNON,0,0,0,0,0,9,@NPC_DNDTARGET,0,40,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Data Set 2 - Cast Mekkatorque-Cannon on DND Target"),
(@NPC_TINKER,0,11,0,61,0,100,0,0,0,0,0,1,6,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Data Set 2 - Say Line 6"),
(@NPC_TINKER,0,12,13,38,0,100,0,3,3,0,0,11,@SPELL_MEKKRAY,0,0,0,0,0,9,@NPC_DNDTARGET,0,40,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Data Set 3 - Cast Mekkatorque-Ray on DND Target"),
(@NPC_TINKER,0,13,0,61,0,100,0,0,0,0,0,1,7,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-High Tinker Mekkatorque - On Data Set 3 - Say Line 7");
-- Mountaineer Stonegrind
UPDATE `creature_template` SET `unit_flags`=256, `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_STONEGRIND;
DELETE FROM `creature_template_addon` WHERE `entry`=@NPC_STONEGRIND;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(@NPC_STONEGRIND, 0, 13340, 0, 0, 0, ''); -- Ram Mount
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_STONEGRIND,@NPC_STONEGRIND*100,@NPC_STONEGRIND*100+1);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_STONEGRIND,0,0,0,0,0,100,0,3000,4000,8000,9000,11,@SPELL_SHOOT,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Stonegrind - In Combat - Cast Shoot"),
(@NPC_STONEGRIND,0,1,0,4,0,100,0,0,0,0,0,11,@SPELL_CHARGE,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Stonegrind - On Aggro - Cast Charge"),
(@NPC_STONEGRIND,0,2,3,38,0,100,0,1,1,0,0,80,@NPC_STONEGRIND*100,0,0,0,0,2,1,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Stonegrind - On Data Set 1 - Run Script"),
(@NPC_STONEGRIND,0,3,0,61,0,100,0,0,0,0,0,8,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Stonegrind - On Data Set 1 - Set React State Passive"),
(@NPC_STONEGRIND*100,9,0,0,0,0,100,0,3000,3000,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Stonegrind - On Script - Say Line 0"),
(@NPC_STONEGRIND*100,9,1,0,0,0,100,0,1000,1000,0,0,53,0,@NPC_STONEGRIND,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Stonegrind - On Script - Start WP To Battle"),
(@NPC_STONEGRIND,0,4,5,38,0,100,0,3,3,0,0,8,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Stonegrind - On Data Set 3 - Set React State Aggressive"),
(@NPC_STONEGRIND,0,5,0,61,0,100,0,0,0,0,0,19,256,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Stonegrind - On Data Set 3 - Remove Unit Flag"),
(@NPC_STONEGRIND,0,6,7,38,0,100,0,2,2,0,0,80,@NPC_STONEGRIND*100+1,0,0,0,0,2,1,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Stonegrind - On Data Set 2 - Run Script"),
(@NPC_STONEGRIND,0,7,0,61,0,100,0,0,0,0,0,8,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Stonegrind - On Data Set 2 - Set React State Passive"),
(@NPC_STONEGRIND*100+1,9,0,0,0,0,100,0,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,-5260.556152,123.674019,393.863525,5.50301,"Nepenthe-Mountaineer Stonegrind - On Script - Move To Point"), -- Face battlefield
(@NPC_STONEGRIND*100+1,9,1,0,0,0,100,0,3000,3000,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Stonegrind - On Script - Say Line 1"),
(@NPC_STONEGRIND*100+1,9,2,0,0,0,100,0,1500,1500,0,0,45,1,1,0,0,0,0,19,@NPC_TINKER,0,0,0,0,0,0,"Nepenthe-Mountaineer Stonegrind - On Script - Set Data 1 High Tinker Mekkatorque"),
(@NPC_STONEGRIND*100+1,9,3,0,0,0,100,0,6000,6000,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Stonegrind - On Script - Forced Despawn"),
(@NPC_STONEGRIND,0,8,0,54,0,100,0,0,0,0,0,53,0,@NPC_STONEGRIND*10,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Mountaineer Stonegrind - Just Summoned - Start WP Quest Complete");
-- Razlo Crushcog
UPDATE `creature_template` SET `exp`=3, `faction_A`=@FACTION_CRUSHCOG, `faction_H`=@FACTION_CRUSHCOG, `baseattacktime`=2000, `unit_flags`=256, `KillCredit1`=0, `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_RAZLO;
DELETE FROM `creature_template_addon` WHERE `entry`=@NPC_RAZLO;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(@NPC_RAZLO, 0, 33087, 0, 0, 0, '');
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_RAZLO,@NPC_RAZLO*100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_RAZLO,0,0,0,11,0,100,0,0,0,0,0,43,42494,32915,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Razlo Crushcog - On Spawn - Change Model"), -- avoids template hack
(@NPC_RAZLO,0,1,0,0,0,100,0,3000,8000,11000,23000,11,@SPELL_MACHINE_GUN,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Razlo Crushcog - In Combat - Cast Machine Gun"),
(@NPC_RAZLO,0,2,0,0,0,100,0,5000,9000,110000,190000,11,@SPELL_SUMMON_TECHNICIANS,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Razlo Crushcog - In Combat - Cast [DND] Summon Technician"),
(@NPC_RAZLO,0,3,0,38,0,100,0,1,1,0,0,80,@NPC_RAZLO*100,0,0,0,0,2,1,0,0,0,0,0,0,0,"Nepenthe-Razlo Crushcog - On Data Set 1 - Run Script"),
(@NPC_RAZLO*100,9,0,0,0,0,100,0,2000,2000,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Razlo Crushcog - On Script - Say Line 0"),
(@NPC_RAZLO*100,9,1,0,0,0,100,0,7000,7000,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Razlo Crushcog - On Script - Say Line 1"),
(@NPC_RAZLO*100,9,2,0,0,0,100,0,11000,11000,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Razlo Crushcog - On Script - Say Line 2"),
(@NPC_RAZLO*100,9,3,0,0,0,100,0,3000,3000,0,0,45,1,1,0,0,0,0,10,@CGUID+6,@NPC_TECH,0,0,0,0,0,"Nepenthe-Razlo Crushcog - On Script - Set Data 1 Crushcog Technician 1"),
(@NPC_RAZLO*100,9,4,0,0,0,100,0,0,0,0,0,45,1,1,0,0,0,0,10,@CGUID+7,@NPC_TECH,0,0,0,0,0,"Nepenthe-Razlo Crushcog - On Script - Set Data 1 Crushcog Technician 2"),
(@NPC_RAZLO*100,9,5,0,0,0,100,0,0,0,0,0,45,1,1,0,0,0,0,10,@CGUID+8,@NPC_TECH,0,0,0,0,0,"Nepenthe-Razlo Crushcog - On Script - Set Data 1 Crushcog Technician 3"),
(@NPC_RAZLO*100,9,6,0,0,0,100,0,0,0,0,0,45,1,1,0,0,0,0,10,@CGUID+9,@NPC_TECH,0,0,0,0,0,"Nepenthe-Razlo Crushcog - On Script - Set Data 1 Crushcog Technician 4"),
(@NPC_RAZLO*100,9,7,0,0,0,100,0,10000,10000,0,0,45,4,4,0,0,0,0,19,@NPC_TINKER,0,0,0,0,0,0,"Nepenthe-Razlo Crushcog - On Script - Set Data 4 High Tinker Mekkatorque"),
(@NPC_RAZLO*100,9,8,0,0,0,100,0,0,0,0,0,45,3,3,0,0,0,0,19,@NPC_STONEGRIND,0,0,0,0,0,0,"Nepenthe-Razlo Crushcog - On Script - Set Data 3 Mountaineer Stonegrind"),
(@NPC_RAZLO*100,9,9,0,0,0,100,0,0,0,0,0,19,256,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Razlo Crushcog - On Script - Remove Unit Flag"),
(@NPC_RAZLO*100,9,10,0,0,0,100,0,1000,1000,0,0,49,0,0,0,0,0,0,19,@NPC_TINKER,0,0,0,0,0,0,"Nepenthe-Razlo Crushcog - On Script - Attack High Tinker Mekkatorque"),
(@NPC_RAZLO,0,4,0,6,0,100,0,0,0,0,0,45,2,2,0,0,0,0,19,@NPC_STONEGRIND,0,0,0,0,0,0,"Nepenthe-Razlo Crushcog - On Death - Set Data 2 Mountaineer Stonegrind");
-- Crushcog Technician
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='', `faction_A`=@FACTION_CRUSHCOG, `faction_H`=@FACTION_CRUSHCOG, `baseattacktime`=2000 WHERE `entry`=@NPC_TECH;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_TECH,-5013945,-5013946,-5013947,-5013948) AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_TECH,0,0,0,11,0,100,0,0,0,0,0,49,0,0,0,0,0,0,2,0,0,0,0,0,0,0,'Nepenthe-Crushcog Technician - On Respawn - Attack Start'),
(@NPC_TECH,0,1,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_IRRADIATION_GUN,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician - In Combat - Cast Irradiation Gun"),
(@NPC_TECH,0,2,0,1,0,100,0,60000,60000,120000,120000,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician - OOC - Despawn Timer"),
(-5013945,0,0,1,1,0,100,0,0,0,0,0,11,@SPELL_STATE,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 1 - OOC - Cast Irradiated Technician State"),
(-5013945,0,1,0,61,0,100,0,0,0,0,0,18,256,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 1 - OOC - Set Unit Flag OOC Non Attackable"),
(-5013945,0,2,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_IRRADIATION_GUN,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 1 - In Combat - Cast Irradiation Gun"),
(-5013945,0,3,4,38,0,100,0,1,1,0,0,11,@SPELL_JUMP,0,0,0,0,0,10,@CGUID+10,@NPC_GUARDIAN,0,0,0,0,0,"Nepenthe-Crushcog Technician 1 - On Data Set - Jump to Crushcog's Guardian"),
(-5013945,0,4,5,61,0,100,0,0,0,0,0,43,@NPC_GUARDIAN,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 1 - On Data Set - Mount Crushcog's Guardian"),
(-5013945,0,5,6,61,0,100,0,0,0,0,0,19,256,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 1 - On Data Set - Remove Unit Flag"),
(-5013945,0,6,0,61,0,100,0,0,0,0,0,49,0,0,0,0,0,0,19,@NPC_TINKER,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 1 - On Data Set - Attack Start High Tinker Mekkatorque"),
(-5013946,0,0,1,1,0,100,0,0,0,0,0,11,@SPELL_STATE,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 2 - OOC - Cast Irradiated Technician State"),
(-5013946,0,1,0,61,0,100,0,0,0,0,0,18,256,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 2 - OOC - Set Unit Flag OOC Non Attackable"),
(-5013946,0,2,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_IRRADIATION_GUN,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 2 - In Combat - Cast Irradiation Gun"),
(-5013946,0,3,4,38,0,100,0,1,1,0,0,11,@SPELL_JUMP,0,0,0,0,0,10,@CGUID+11,@NPC_GUARDIAN,0,0,0,0,0,"Nepenthe-Crushcog Technician 2 - On Data Set - Jump to Crushcog's Guardian"),
(-5013946,0,4,5,61,0,100,0,0,0,0,0,43,@NPC_GUARDIAN,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 2 - On Data Set - Mount Crushcog's Guardian"),
(-5013946,0,5,6,61,0,100,0,0,0,0,0,19,256,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 2 - On Data Set - Remove Unit Flag"),
(-5013946,0,6,0,61,0,100,0,0,0,0,0,49,0,0,0,0,0,0,19,@NPC_TINKER,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 2 - On Data Set - Attack Start High Tinker Mekkatorque"),
(-5013947,0,0,1,1,0,100,0,0,0,0,0,11,@SPELL_STATE,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 3 - OOC - Cast Irradiated Technician State"),
(-5013947,0,1,0,61,0,100,0,0,0,0,0,18,256,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 3 - OOC - Set Unit Flag OOC Non Attackable"),
(-5013947,0,2,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_IRRADIATION_GUN,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 3 - In Combat - Cast Irradiation Gun"),
(-5013947,0,3,4,38,0,100,0,1,1,0,0,11,@SPELL_JUMP,0,0,0,0,0,10,@CGUID+12,@NPC_GUARDIAN,0,0,0,0,0,"Nepenthe-Crushcog Technician 3 - On Data Set - Jump to Crushcog's Guardian"),
(-5013947,0,4,5,61,0,100,0,0,0,0,0,43,@NPC_GUARDIAN,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 3 - On Data Set - Mount Crushcog's Guardian"),
(-5013947,0,5,6,61,0,100,0,0,0,0,0,19,256,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 3 - On Data Set - Remove Unit Flag"),
(-5013947,0,6,0,61,0,100,0,0,0,0,0,49,0,0,0,0,0,0,19,@NPC_STONEGRIND,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 3 - On Data Set - Attack Start Mountaineer Stonegrind"),
(-5013948,0,0,1,1,0,100,0,0,0,0,0,11,@SPELL_STATE,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 4 - OOC - Cast Irradiated Technician State"),
(-5013948,0,1,0,61,0,100,0,0,0,0,0,18,256,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 4 - OOC - Set Unit Flag OOC Non Attackable"),
(-5013948,0,2,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_IRRADIATION_GUN,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 4 - In Combat - Cast Irradiation Gun"),
(-5013948,0,3,4,38,0,100,0,1,1,0,0,11,@SPELL_JUMP,0,0,0,0,0,10,@CGUID+13,@NPC_GUARDIAN,0,0,0,0,0,"Nepenthe-Crushcog Technician 4 - On Data Set - Jump to Crushcog's Guardian"),
(-5013948,0,4,5,61,0,100,0,0,0,0,0,43,@NPC_GUARDIAN,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 4 - On Data Set - Mount Crushcog's Guardian"),
(-5013948,0,5,6,61,0,100,0,0,0,0,0,19,256,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 4 - On Data Set - Remove Unit Flag"),
(-5013948,0,6,0,61,0,100,0,0,0,0,0,49,0,0,0,0,0,0,19,@NPC_STONEGRIND,0,0,0,0,0,0,"Nepenthe-Crushcog Technician 4 - On Data Set - Attack Start Mountaineer Stonegrind");
-- Crushcog's Guardian
UPDATE `creature_template` SET `minlevel`=5, `maxlevel`=5, `faction_A`=@FACTION_CRUSHCOG, `faction_H`=@FACTION_CRUSHCOG, `baseattacktime`=2000, `unit_flags`=256, `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_GUARDIAN;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (-5013949,-5013950,-5013951,-5013952) AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-5013949,0,0,0,8,0,100,0,@SPELL_JUMP,0,10000,10000,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog's Guardian - On Spellhit - Despawn"),
(-5013950,0,0,0,8,0,100,0,@SPELL_JUMP,0,10000,10000,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog's Guardian - On Spellhit - Despawn"),
(-5013951,0,0,0,8,0,100,0,@SPELL_JUMP,0,10000,10000,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog's Guardian - On Spellhit - Despawn"),
(-5013952,0,0,0,8,0,100,0,@SPELL_JUMP,0,10000,10000,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Crushcog's Guardian - On Spellhit - Despawn");
-- Jarvi Shadowstep
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_JARVI;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_JARVI,@NPC_JARVI*100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_JARVI,0,0,0,20,0,100,0,@QUEST,0,0,0,80,@NPC_JARVI*100,0,0,0,0,2,1,0,0,0,0,0,0,0,"Nepenthe-Jarvi Shadowstep - On Quest Complete - Run Script"),
(@NPC_JARVI*100,9,0,0,0,0,100,0,2000,2000,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Jarvi Shadowstep - On Script - Yell Line 0"),
(@NPC_JARVI*100,9,1,0,0,0,100,0,1000,1000,0,0,12,@NPC_TINKER,8,60000,0,0,0,8,0,0,0,-5360.973633,307.487335,394.401154,3.434302,"Nepenthe-Jarvi Shadowstep - On Script - Summon High Tinker Mekkatorque"),
(@NPC_JARVI*100,9,2,0,0,0,100,0,0,0,0,0,12,@NPC_STONEGRIND,8,60000,0,0,0,8,0,0,0,-5370.026367,290.511078,394.069427,2.554657,"Nepenthe-Jarvi Shadowstep - On Script - Summon Mountaineer Stonegrind"), 
(@NPC_JARVI*100,9,3,0,0,0,100,0,0,0,0,0,45,1,1,0,0,0,0,10,6537184,@NPC_INFANTRY2,0,0,0,0,0,"Nepenthe-Jarvi Shadowstep - On Script - Set Data 1 Gnomeregan Infantry"),
(@NPC_JARVI*100,9,4,0,0,0,100,0,2000,2000,0,0,45,2,2,0,0,0,0,19,@NPC_INFANTRY2,0,0,0,0,0,0,"Nepenthe-Jarvi Shadowstep - On Script - Set Data 2 Gnomeregan Infantry"),
(@NPC_JARVI*100,9,5,0,0,0,100,0,9000,9000,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Jarvi Shadowstep - On Script - Say Line 1"),
(@NPC_JARVI*100,9,6,0,0,0,100,0,0,0,0,0,45,1,1,0,0,0,0,19,@NPC_MOUNTAINEER,0,0,0,0,0,0,"Nepenthe-Jarvi Shadowstep - On Script - Set Data 1 Dun Morogh Mountaineer");
-- Dun Morogh Mountaineer & Gnomeregan Infantry
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry` IN (@NPC_MOUNTAINEER, @NPC_INFANTRY2);
UPDATE `creature_template` SET `minlevel`=90, `maxlevel`=90 WHERE `entry`=@NPC_INFANTRY2;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_MOUNTAINEER, @NPC_INFANTRY2, -6537184) AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-6537184,0,0,1,38,0,100,0,1,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Gnomeregan Infantry - On Data Set 1 - Say Line 0"),
(-6537184,0,1,0,61,0,100,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Gnomeregan Infantry - On Data Set 1 - Say Line 1"),
(@NPC_INFANTRY2,0,0,1,38,0,100,0,2,2,0,0,5,4,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Gnomeregan Infantry - On Data Set 2 - Emote Cheer"),
(@NPC_INFANTRY2,0,1,2,61,0,100,0,0,0,0,0,5,4,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Gnomeregan Infantry - On Data Set 2 - Emote Cheer"),
(@NPC_INFANTRY2,0,2,0,61,0,100,0,0,0,0,0,5,4,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Gnomeregan Infantry - On Data Set 2 - Emote Cheer"),
(@NPC_MOUNTAINEER,0,0,1,38,0,100,0,1,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Dun Morogh Mountaineer - On Data Set 1 - Say Line 1"),
(@NPC_MOUNTAINEER,0,1,0,61,0,100,0,0,0,0,0,5,4,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Dun Morogh Mountaineer - On Data Set 1 - Emote Cheer");
-- Spawns
DELETE FROM `creature` WHERE `id`=@NPC_INFANTRY AND `guid` IN (@CGUID+0, @CGUID+1);
DELETE FROM `creature` WHERE `id`=@NPC_MOUNTAINEER AND `guid` IN (@CGUID+2, @CGUID+3);
DELETE FROM `creature` WHERE `id`=@NPC_STONEGRIND;
DELETE FROM `creature` WHERE `id`=@NPC_RAZLO;
DELETE FROM `creature` WHERE `id`=@NPC_TECH;
DELETE FROM `creature` WHERE `id`=@NPC_GUARDIAN;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(@CGUID+0, @NPC_INFANTRY, 0, 1, 1, 0, 42316, -5313.26, 141.595, 388.814, 5.68646, 60, 0, 0, 87120, 0, 0, 0, 0, 0),
(@CGUID+1, @NPC_INFANTRY, 0, 1, 1, 0, 42316, -5318, 150.148, 389.588, 5.5333, 60, 0, 0, 87120, 0, 0, 0, 0, 0),
(@CGUID+2, @NPC_MOUNTAINEER, 0, 1, 1, 0, 13076, -5301.44, 152.345, 390.014, 5.58697, 60, 0, 0, 656, 0, 0, 0, 0, 0),
(@CGUID+3, @NPC_MOUNTAINEER, 0, 1, 1, 0, 13076, -5313.46, 159.593, 390.831, 5.29795, 60, 0, 0, 656, 0, 0, 0, 0, 0),
(@CGUID+4, @NPC_STONEGRIND, 0, 1, 1, 0, 42852, -5309.34, 150.747, 390.017, 5.50301, 60, 0, 0, 53420, 0, 0, 0, 0, 0),
(@CGUID+5, @NPC_RAZLO, 0, 1, 1, 0, 0, -5246.75, 118.814, 394.287, 2.78923, 60, 0, 0, 116160, 0, 0, 0, 0, 0),
(@CGUID+6, @NPC_TECH, 0, 1, 1, 0, 0, -5244.11, 121.006, 394.315, 4.07641, 90, 0, 0, 71, 0, 0, 0, 0, 0),
(@CGUID+7, @NPC_TECH, 0, 1, 1, 0, 0, -5247.2, 122.337, 394.304, 5.04492, 90, 0, 0, 71, 0, 0, 0, 0, 0),
(@CGUID+8, @NPC_TECH, 0, 1, 1, 0, 0, -5249.21, 116.281, 394.128, 0.974603, 90, 0, 0, 86, 0, 0, 0, 0, 0),
(@CGUID+9, @NPC_TECH, 0, 1, 1, 0, 0, -5245.94, 115.459, 393.885, 1.92965, 90, 0, 0, 86, 0, 0, 0, 0, 0),
(@CGUID+10, @NPC_GUARDIAN, 0, 1, 1, 0, 0, -5250.87, 130.37, 394.378, 5.02603, 90, 0, 0, 42, 0, 0, 0, 0, 0),
(@CGUID+11, @NPC_GUARDIAN, 0, 1, 1, 0, 0, -5255.05, 107.764, 392.576, 0.794312, 90, 0, 0, 42, 0, 0, 0, 0, 0),
(@CGUID+12, @NPC_GUARDIAN, 0, 1, 1, 0, 0, -5244.74, 102.783, 391.964, 2.0431, 90, 0, 0, 42, 0, 0, 0, 0, 0),
(@CGUID+13, @NPC_GUARDIAN, 0, 1, 1, 0, 0, -5238.59, 127.788, 394.419, 3.31151, 90, 0, 0, 42, 0, 0, 0, 0, 0);
DELETE FROM `creature_addon` WHERE `guid` IN (@CGUID+2, @CGUID+3);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(@CGUID+2, 0, 0, 0, 0, 0, 18373),
(@CGUID+3, 0, 0, 0, 0, 0, 18373);
DELETE FROM `gameobject` WHERE `id` = @NPC_IFBANNER AND `guid` IN (@OGUID+0, @OGUID+1);
DELETE FROM `gameobject` WHERE `id` = @NPC_GRBANNER AND `guid` IN (166092, 165947, @OGUID+2, @OGUID+3);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(@OGUID+0, @NPC_IFBANNER, 0, 1, 1, -5314.61, 161.36, 390.885, 5.37995, 0, 0, 0.436423, -0.899742, 60, 0, 1),
(@OGUID+1, @NPC_IFBANNER, 0, 1, 1, -5303.05, 153.793, 390.309, 5.57708, 0, 0, 0.345765, -0.938321, 60, 0, 1),
(@OGUID+2, @NPC_GRBANNER, 0, 1, 1, -5319.42, 151.28, 389.745, -0.715585, 0, 0, 0, 1, 60, 100, 1),
(@OGUID+3, @NPC_GRBANNER, 0, 1, 1, -5314.46, 142.615, 388.752, -0.628317, 0, 0, 0, 1, 60, 100, 1);
-- Waypoints
DELETE FROM `waypoints` WHERE `entry`=@NPC_TINKER;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(@NPC_TINKER,1,-5295.045898,134.664993,386.114258,"High Tinker Mekkatorque to Battle"),
(@NPC_TINKER,2,-5261.567871,119.521042,393.758728,"High Tinker Mekkatorque to Battle");
DELETE FROM `waypoints` WHERE `entry`=@NPC_STONEGRIND;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(@NPC_STONEGRIND,1,-5292.336426,138.289490,386.114258,"Mountaineer Stonegrind to Battle"),
(@NPC_STONEGRIND,2,-5260.556152,123.674019,393.863525,"Mountaineer Stonegrind to Battle");
DELETE FROM `waypoints` WHERE `entry`=@NPC_TINKER*10;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(@NPC_TINKER*10,1,-5370.931152,304.627747,393.946716,"High Tinker Mekkatorque Quest complete");
DELETE FROM `waypoints` WHERE `entry`=@NPC_STONEGRIND*10;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(@NPC_STONEGRIND*10,1,-5379.304199,296.682190,393.627808,"Mountaineer Stonegrind Quest complete"),
(@NPC_STONEGRIND*10,2,-5383.827148,304.953644,393.861969,"Mountaineer Stonegrind Quest complete"),
(@NPC_STONEGRIND*10,3,-5387.896484,308.077545,394.275787,"Mountaineer Stonegrind Quest complete"),
(@NPC_STONEGRIND*10,4,-5386.983398,307.339386,394.132629,"Mountaineer Stonegrind Quest complete");
-- Texts
DELETE FROM `creature_text` WHERE `entry` IN (@NPC_TINKER,@NPC_STONEGRIND,@NPC_RAZLO,@NPC_JARVI,@NPC_INFANTRY2,@NPC_MOUNTAINEER);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC_TINKER,0,0,"Mekgineer Thermaplugg refuses to acknowledge that his defeat is imminent! He has sent Razlo Crushcog to prevent us from rebuilding our beloved Gnomeregan!",12,0,100,0,0,20890,"High Tinker Mekkatorque"),
(@NPC_TINKER,1,0,"But $N has thwarted his plans at every turn, and the dwarves of Ironforge stand with us.",12,0,100,0,0,20891,"High Tinker Mekkatorque"),
(@NPC_TINKER,2,0,"Let's send him crawling back to his master in defeat!",12,0,100,0,0,20892,"High Tinker Mekkatorque"),
(@NPC_STONEGRIND,0,0,"Aye, let's teach this addle-brained gnome a lesson!",12,0,100,0,0,0,"Mountaineer Stonegrind"),
(@NPC_RAZLO,0,0,"You! How did you escape detection by my sentry-bots?",12,0,100,0,0,0,"Razlo Crushcog"),
(@NPC_RAZLO,1,0,"No matter! My guardians and I will make short work of you. To arms, men!",12,0,100,0,0,0,"Razlo Crushcog"),
(@NPC_RAZLO,2,0,"You will never defeat the true sons of Gnomeregan!",12,0,100,0,0,21245,"Razlo Crushcog"),
(@NPC_STONEGRIND,1,0,"That'll teach you to mess with the might of Ironforge and Gnomeregan!",12,0,100,0,0,0,"Mountaineer Stonegrind"),
(@NPC_TINKER,3,0,"We've done it! We're victorious!",12,0,100,0,0,20893,"High Tinker Mekkatorque"),
(@NPC_TINKER,4,0,"With Crushcog defeated, Thermaplugg is sure to be quaking in his mechano-tank, and rightly so. You're next Thermaplugg. You're next!",12,0,100,0,0,20894,"High Tinker Mekkatorque"),
(@NPC_TINKER,6,0,"Mekkatorque-Cannon!",14,0,100,0,0,17561,"High Tinker Mekkatorque"),
(@NPC_TINKER,7,0,"Mekkatorque-Ray!",14,0,100,0,0,17563,"High Tinker Mekkatorque"),
(@NPC_JARVI,0,0,"$N, along with High Tinker Mekkatorque and Mountaineer Stonegrind, has defeated Razlo Crushcog!",14,0,100,0,0,0,"Jarvi Shadowstep"),
(@NPC_INFANTRY2,0,0,"Victory at last!",12,0,100,4,0,0,"Gnomeregan Infantry"), -- ONESHOT_CHEER
(@NPC_INFANTRY2,1,0,"Three cheers for Mekkatorque!",12,0,100,4,0,0,"Gnomeregan Infantry"), -- ONESHOT_CHEER
(@NPC_JARVI,1,0,"The victorious heroes have returned! Control of Chill Breeze Valley is ours once again!",12,0,100,0,0,0,"Jarvi Shadowstep"),
(@NPC_MOUNTAINEER,0,0,"I'll drink to that!",12,0,100,4,0,0,"Dun Morogh Mountaineer"), -- ONESHOT_CHEER
(@NPC_TINKER,5,0,"Razlo Crushcog is no more! The people of Ironforge and Gnomeregan speak with one voice this day. Hear us well, Thermaplugg. The day of your defeat approaches!",12,0,100,0,0,20889,"High Tinker Mekkatorque");
-- Gossip Menues
-- Link them
DELETE FROM `gossip_menu` WHERE `entry`=@GOSSIP AND `text_id`=@TEXT_ID;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(@GOSSIP,@TEXT_ID);
-- Static text
DELETE FROM `npc_text` WHERE `id`=@TEXT_ID;
INSERT INTO `npc_text` (`id`, `text0_0`) VALUES
(@TEXT_ID,"Crushcog's defeat will send a powerful message to his master, Thermaplugg. The gnomes of Gnomeregan will not be stopped!");
-- Text options
DELETE FROM `gossip_menu_option` WHERE `menu_id`=@GOSSIP;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(@GOSSIP,0,0,"I'm ready to start the assault!",1,1,0,0,0,0, NULL);
-- Condition
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=@GOSSIP;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15,@GOSSIP,0,0,9,@QUEST,0,0,0,'',"Only show gossip option if player has q26364 Down With Crushcog!");

-- Misc
-- Irradiated Technician & Crushcog Battle Suit
UPDATE `creature_template` SET `mingold` = 1, `maxgold` = 5 WHERE `entry` IN (42223, 42226);

-- Dead Crag Boar
DELETE FROM `creature_template_addon` WHERE `entry`=40939;
INSERT INTO `creature_template_addon` (`entry`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(40939,0,3,1,0,29266);

-- Dead Frostmane Troll
DELETE FROM `creature_template_addon` WHERE `entry`=42220;
INSERT INTO `creature_template_addon` (`entry`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(42220,0,3,1,0,29266);

-- Clean Cannon X-2 mounted by S.A.F.E. Operative
-- Not sure if uses addon...
DELETE FROM `creature_template_addon` WHERE `entry`=42220;
INSERT INTO `creature_template_addon` (`entry`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(42220,27669,0,0,0,'');

-- Living Contamination
SET @NPC := 43089;
SET @SPELL_ENRAGE := 63227;
SET @SPELL_POISON_BOLT := 21067;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_POISON_BOLT,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Living Contamination - In Combat - Cast Poison Bolt"),
(@NPC,0,1,0,2,0,100,1,0,30,0,0,11,@SPELL_ENRAGE,1,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Living Contamination - At 30% HP - Cast Enrage");
-- Living Contamination
SET @NPC := 42185;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_POISON_BOLT,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Living Contamination - In Combat - Cast Poison Bolt"),
(@NPC,0,1,0,2,0,100,1,0,30,0,0,11,@SPELL_ENRAGE,1,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Living Contamination - At 30% HP - Cast Enrage");

-- Gnomeregan Recruit
SET @NPC := 43092;
SET @SPELL_SHOOT := 85756;
UPDATE `creature_template` SET `mindmg` = 2, `maxdmg` = 4, `attackpower` = 6, `dmg_multiplier` = 1, `AIName` = 'SmartAI' WHERE `entry` = @NPC;
DELETE FROM `creature_addon` WHERE `guid` IN (SELECT `guid` FROM `creature` WHERE `id` = @NPC);
DELETE FROM `creature_template_addon` WHERE `entry` = @NPC;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(@NPC, 0, 0, 0, 2, 0, 78174);
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,11,0,100,0,0,0,0,0,21,0,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Gnomeregan Recruit - On Respawn - Disable Move"),
(@NPC,0,1,0,0,0,100,0,1000,4000,5000,8000,11,@SPELL_SHOOT,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Gnomeregan Recruit - In Combat - Cast Shoot");

-- Toxic Sludge
SET @NPC := 42184;
SET @SPELL_TOXIC := 84150;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,6,0,100,0,0,0,0,0,11,@SPELL_TOXIC,2,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Toxic Sludge - On Death - Cast Tixuc Residue");

-- Young Snow Leopard
SET @NPC := 42286;
SET @SPELL_SPRINT := 32720;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,17,0,100,0,3,8,0,0,11,@SPELL_SPRINT,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Young Snow Leopard - On Player Range - Cast Sprint");

-- Rockjaw Marauder
SET @NPC := 42222;
SET @SPELL_CLUB := 69851;
UPDATE `creature_template` SET `mingold` = 1, `maxgold` = 3, `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_CLUB,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Rockjaw Marauder - In Combat - Cast Club");

-- Rockjaw Goon
SET @NPC := 37073;
SET @SPELL_CLUB := 69851;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,31,0,100,0,@SPELL_CLUB,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Rockjaw Goon - Spellhit Target - Say Line 0 (random)"),
(@NPC,0,1,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_CLUB,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Rockjaw Goon - In Combat - Cast Club");
DELETE FROM `creature_text` WHERE `entry`=@NPC;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC,0,0,"Trogg cave all gone! You pay for this!!",0,0,0,0,0,0,"Rockjaw Goon"),
(@NPC,0,1,"Why you break our home?!",0,0,0,0,0,0,"Rockjaw Goon"),
(@NPC,0,2,"You break our cave, I break your skull!",0,0,0,0,0,0,"Rockjaw Goon");

-- Rockjaw Bonepicker
SET @NPC := 42221;
SET @SPELL_BONE_TOSS := 82625;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_BONE_TOSS,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Rockjaw Bonepicker - In Combat - Cast Bone Toss");

-- Rockjaw Fungus-Flinger
SET @NPC := 43325;
SET @SPELL_FLING_FUNGUS := 80944;
SET @SPELL_POISONOUS_MUSHROOM := 87347;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,4,0,100,0,0,0,0,0,11,@SPELL_POISONOUS_MUSHROOM,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Rockjaw Fungus-Flinger - On Aggro - Cast Poisonous Mushroom"),
(@NPC,0,1,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_FLING_FUNGUS,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Rockjaw Fungus-Flinger - In Combat - Cast Fling Fungus");

-- Boss Bruggor
SET @NPC := 42773;
SET @SPELL_ENRAGE := 63227;
SET @SPELL_CLUB := 69851;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_CLUB,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Boss Bruggor - In Combat - Cast Club"),
(@NPC,0,1,2,2,0,100,1,0,30,0,0,11,@SPELL_ENRAGE,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Boss Bruggor - At 30% HP - Cast Enrage"),
(@NPC,0,2,0,61,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Boss Bruggor - At 30% HP - Say Line 0");
DELETE FROM `creature_text` WHERE `entry`=@NPC;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC,0,0,"%s becomes enraged!",16,0,0,0,0,0,"Boss Bruggor");

-- Crushcog Battle Suit
SET @NPC := 42226;
SET @SPELL_RAZOR_EDGE := 84146;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_RAZOR_EDGE,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Crushcog Battle Suit - In Combat - Cast Razor Edge");

-- Winter Wolf
SET @NPC := 42290;
SET @SPELL_SNARL := 32919;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_SNARL,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Winter Wolf - In Combat - Cast Snarl");

-- Frostmane Seer
SET @NPC := 41121;
SET @SPELL_LIGHTNING_SHIELD := 12550;
SET @SPELL_LIGHTNING_BOLT := 9532;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,1,0,100,0,0,0,600000,600000,11,@SPELL_LIGHTNING_SHIELD,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Frostmane Seer - Out of Combat - Cast Lightning Shield"),
(@NPC,0,1,0,0,0,100,0,1000,3000,6000,7000,11,@SPELL_LIGHTNING_BOLT,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Frostmane Seer - In Combat - Cast Lightning Bolt");

-- Frostmane Snowstrider
SET @NPC := 41122;
SET @SPELL_SNOWBALL := 84118;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,4,0,30,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Frostmane Snowstrider - On Aggro - Say Line 0 (random)"),
(@NPC,0,1,0,0,0,100,0,1000,5000,7000,12000,11,@SPELL_SNOWBALL,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Frostmane Snowstrider - In Combat - Cast Lightning Bolt");
DELETE FROM `creature_text` WHERE `entry`=@NPC;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC,0,0,"I gonna make you into mojo!",12,0,0,0,0,0,"Frostmane Snowstrider"),
(@NPC,0,1,"Killing you be easy.",12,0,0,0,0,0,"Frostmane Snowstrider"),
(@NPC,0,2,"My weapon be thirsty!",12,0,0,0,0,0,"Frostmane Snowstrider"),
(@NPC,0,3,"You be dead soon!",12,0,0,0,0,0,"Frostmane Snowstrider");

-- Wendigo
SET @NPC := 40941;
SET @SPELL_FROST_BREATH := 3131;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_FROST_BREATH,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Wendigo - In Combat - Cast Frost Breath");

-- Young Wendigo
SET @NPC := 40940;
SET @SPELL_FROST_BREATH := 3131;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_FROST_BREATH,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Young Wendigo - In Combat - Cast Frost Breath");

-- Frostmane Blade
SET @NPC := 37507;
SET @SPELL_POISONED_BLADE := 70133;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,4,0,30,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Frostmane Blade - On Aggro - Say Line 0 (random)"),
(@NPC,0,1,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_POISONED_BLADE,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Frostmane Blade - In Combat - Cast Poisoned Blade");
DELETE FROM `creature_text` WHERE `entry`=@NPC;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC,0,0,"I gonna make you into mojo!",12,0,0,0,0,0,"Frostmane Blade"),
(@NPC,0,1,"Killing you be easy.",12,0,0,0,0,0,"Frostmane Blade"),
(@NPC,0,2,"My weapon be thirsty!",12,0,0,0,0,0,"Frostmane Blade"),
(@NPC,0,3,"You be dead soon!",12,0,0,0,0,0,"Frostmane Blade");

-- Frostmane Scavenger
SET @NPC := 41146;
SET @SPELL_THROW_GEARS := 75775;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_THROW_GEARS,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Frostmane Scavenger - In Combat - Cast Throw Gears");

-- Frostmane Scout
SET @NPC := 41175;
SET @SPELL_THROW := 38557;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,4,0,30,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Frostmane Scout - On Aggro - Say Line 0 (random)"),
(@NPC,0,1,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_THROW,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Frostmane Scout - In Combat - Cast Throw");
DELETE FROM `creature_text` WHERE `entry`=@NPC;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC,0,0,"I gonna make you into mojo!",12,0,0,0,0,0,"Frostmane Scout"),
(@NPC,0,1,"Killing you be easy.",12,0,0,0,0,0,"Frostmane Scout"),
(@NPC,0,2,"My weapon be thirsty!",12,0,0,0,0,0,"Frostmane Scout"),
(@NPC,0,3,"You be dead soon!",12,0,0,0,0,0,"Frostmane Scout");

-- Frostmane Warrior
SET @NPC := 41258;
SET @SPELL_BATTLE_SHOUT := 77808;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,4,0,100,0,0,0,0,0,11,@SPELL_BATTLE_SHOUT,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Frostmane Warrior - On Aggro - Cast Battle Shout");

-- Battok the Berserker
SET @NPC := 41284;
SET @SPELL_THROW := 38557;
SET @SPELL_TRANSFORM := 77369;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_THROW,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Battok the Berserker - In Combat - Cast Throw"),
(@NPC,0,1,0,2,0,100,1,0,50,0,0,11,@SPELL_TRANSFORM,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Battok the Berserker - At 50% HP - Cast Battok Transform");

-- Amberstill Mountaineer
SET @NPC := 41611;
SET @SPELL_DEVOTION_AURA := 8258;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,1,0,100,1,0,0,0,0,11,@SPELL_DEVOTION_AURA,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Amberstill Mountaineer - Out of Combat - Cast Devotion Aura");

-- Tannok Frosthammer
SET @NPC := 6806;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,1,0,100,0,1000,3000,13000,18000,10,7,273,274,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Tannok Frosthammer - Out of Combat - Play Random Emote");

-- Innkeeper Belm
SET @NPC := 1247;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,1,0,100,0,1000,3000,13000,18000,10,7,273,274,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Innkeeper Belm - Out of Combat - Play Random Emote");

-- Granis Swiftaxe
SET @NPC := 1229;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,1,0,100,0,1000,3000,13000,18000,10,7,273,274,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Granis Swiftaxe - Out of Combat - Play Random Emote");

-- Maxan Anvol
SET @NPC := 1226;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,1,0,100,0,1000,3000,13000,18000,10,7,273,274,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Maxan Anvol - Out of Combat - Play Random Emote");

-- Thamner Pol
SET @NPC := 2326;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,1,0,100,0,1000,3000,13000,18000,10,7,273,274,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Thamner Pol - Out of Combat - Play Random Emote");

-- Snow Tracker Wolf
SET @NPC := 41478;
SET @SPELL_BITE := 17253;
SET @SPELL_CHILLING_HOWL := 32918;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_CHILLING_HOWL,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Snow Tracker Wolf - In Combat - Cast Chilling Howl"),
(@NPC,0,1,0,0,0,100,0,3000,4000,5000,9000,11,@SPELL_BITE,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Snow Tracker Wolf - In Combat - Cast Bite");

-- Miner Grothor
SET @NPC := 1358;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,1,0,100,0,1000,3000,7000,9000,10,5,273,274,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Miner Grothor - Out of Combat - Play Random Emote");

-- Frostmane Novice
SET @NPC := 946;
SET @SPELL_WEAK_FROSTBOLT := 6949;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,4,0,30,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Frostmane Novice - On Aggro - Say Line 0 (random)"),
(@NPC,0,1,0,0,0,100,0,1000,5000,7000,12000,11,@SPELL_WEAK_FROSTBOLT,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Frostmane Novice - In Combat - Cast Weak Frostbolt");
DELETE FROM `creature_text` WHERE `entry`=@NPC;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC,0,0,"I gonna make you into mojo!",12,0,0,0,0,0,"Frostmane Novice"),
(@NPC,0,1,"Killing you be easy.",12,0,0,0,0,0,"Frostmane Novice"),
(@NPC,0,2,"My weapon be thirsty!",12,0,0,0,0,0,"Frostmane Novice"),
(@NPC,0,3,"You be dead soon!",12,0,0,0,0,0,"Frostmane Novice");

-- Snow Leopard
SET @NPC := 1201;
SET @SPELL_LEAPING_RUSH := 75002;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,4,0,100,0,0,0,0,0,11,@SPELL_LEAPING_RUSH,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Snow Leopard - On Aggro - Cast Leaping Rush");

-- Helm's Bed Surger
SET @NPC := 41762;
SET @SPELL_SURGE := 33970;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,0,0,100,0,3000,8000,11000,16000,11,@SPELL_SURGE,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Helm's Bed Surger - In Combat - Cast Surge");

-- Gnomeregan Trainee
SET @NPC := 42329;
SET @SPELL_ATTACK := 78959;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,1,0,100,1,0,0,0,0,8,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Gnomeregan Trainee - Out of Combat - React State Passive"),
(@NPC,0,1,0,1,0,100,0,3000,6000,8000,17000,11,@SPELL_ATTACK,0,0,0,0,0,19,42328,0,0,0,0,0,0,"Nepenthe-Gnomeregan Trainee - Out of Combat - Cast Cosmetic - Combat Attack 1H (Thrust)"); -- At Practice Dummy
-- Practice Dummy
SET @NPC := 42328;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC,@NPC*100+0,@NPC*100+1,@NPC*100+2);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,8,0,100,8,@SPELL_ATTACK,0,0,0,87,@NPC*100+0,@NPC*100+1,@NPC*100+2,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Practice Dummy - On Spellhit - Run Random Script"),
(@NPC*100+0,9,0,0,0,0,100,0,0,0,0,0,11,46577,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Practice Dummy - On Script 0 - Cast Wounded"),
(@NPC*100+1,9,0,0,0,0,100,0,0,0,0,0,11,78960,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Practice Dummy - On Script 1 - Cast Wound Impact"),
(@NPC*100+2,9,0,0,0,0,100,0,0,0,0,0,11,78961,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Practice Dummy - On Script 2 - Cast Wound Impact Critical");

-- Jona Ironstock
SET @NPC := 41121;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,1,0,100,0,1500,9000,12000,69000,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Jona Ironstock - Out of Combat - Say Line 0 (random)");
DELETE FROM `creature_text` WHERE `entry`=@NPC;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC,0,0,"What's Joren doin' out there, anyhow?",12,0,0,0,0,0,"Jona Ironstock"),
(@NPC,0,1,"Let's keep this fire going, dwarves!",12,0,0,0,0,0,"Jona Ironstock"),
(@NPC,0,2,"Get ta work, boys!",12,0,0,0,0,0,"Jona Ironstock"),
(@NPC,0,3,"Don't fret, we'll be outta here soon.",12,0,0,0,0,0,"Jona Ironstock");

-- Grik'nir the Cold
-- Not using link on aggro because the first line has a 30% chance to happen
SET @NPC := 808;
SET @SPELL_FROST_STRIKE := 79895;
SET @SPELL_STRENGTH := 6957;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,4,0,30,0,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Grik'nir the Cold - On Aggro - Say Line 0"),
(@NPC,0,1,0,4,0,100,0,0,0,0,0,11,@SPELL_STRENGTH,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Grik'nir the Cold - On Aggro - Cast Frostmane Strength"),
(@NPC,0,2,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_FROST_STRIKE,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Grik'nir the Cold - In Combat - Cast Frost Strike");
DELETE FROM `creature_text` WHERE `entry`=@NPC;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC,0,0,"First da fire, and now da $r! You gonna die!",12,0,0,0,0,0,"Grik'nir the Cold");

-- Wayward Fire Elemental
SET @NPC := 37112;
SET @SPELL_FIREBALL := 9053;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,4,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Wayward Fire Elemental - On Aggro - Say Line 0"),
(@NPC,0,1,0,0,0,100,0,1000,5000,7000,10000,11,@SPELL_FIREBALL,0,0,0,0,0,2,0,0,0,0,0,0,0,"Nepenthe-Wayward Fire Elemental - In Combat - Cast Fireball");
DELETE FROM `creature_text` WHERE `entry`=@NPC;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC,0,0,"T MA TORO NUK KRAST'VEN FMERK",12,12,0,0,0,0,"Wayward Fire Elemental");

-- Frostmane Troll Whelp
SET @NPC := 706;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='', `npcflag` = 2 WHERE `entry`=@NPC;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC,0,0,0,4,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Frostmane Troll Whelp - On Aggro - Say Line 0 (random)");
DELETE FROM `creature_text` WHERE `entry`=@NPC;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC,0,0,"I gonna make you into mojo!",12,0,0,0,0,0,"Frostmane Troll Whelp"),
(@NPC,0,1,"Killing you be easy.",12,0,0,0,0,0,"Frostmane Troll Whelp"),
(@NPC,0,2,"My weapon be thirsty!",12,0,0,0,0,0,"Frostmane Troll Whelp"),
(@NPC,0,3,"You be dead soon!",12,0,0,0,0,0,"Frostmane Troll Whelp");

-- [SQL] Generic - Constriction Totem Aura will no longer be stuck on players (Refs #5370)
DELETE FROM `spell_area` WHERE `spell` = 77311;
INSERT INTO `spell_area` (`spell`, `area`, `gender`) VALUES
(77311, 5097, 2);

UPDATE `quest_template` SET `EndText` = "" WHERE `Id` = 27671;

-- Quests - A Job for the Multi-Bot has been scripted (Feedback #284)
-- Engineer Grindspark
SET @NPC_GRINDSPARK := 42553;
SET @QUEST := 26205;
SET @GOSSIP := 12634;
SET @TEXTID := 17763;
SET @SPELL_FORCE_SUMMON := 79419;
SET @NPC_PROTOTYPE := 42945;
SET @SPELL_FEIGN_DEATH := 29266;
SET @SPELL_EXPLOSION := 62987;
SET @NPC_MULTIBOT := 42598;
SET @SPELL_CLEANUP_NPC_TOXIC_POOL := 79424;
SET @OBJECT_TOXIC_POOL := 203975;
SET @SPELL_DESPAWN_OBJECT_TOXIC_POOL := 79421;
SET @SPELL_DESPAWN_MULTIBOT := 79435;
SET @NPC_TOXIC_POOL := 42563;
SET @SPELL_ERUPTION := 79391;
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='', `gossip_menu_id`=@GOSSIP WHERE `entry`=@NPC_GRINDSPARK;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_GRINDSPARK,@NPC_GRINDSPARK*100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_GRINDSPARK,0,0,1,19,0,100,0,@QUEST,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Engineer Grindspark - On Quest Accept - Say Line 0"),
(@NPC_GRINDSPARK,0,1,2,61,0,100,0,0,0,0,0,80,@NPC_GRINDSPARK*100,0,0,0,0,2,1,0,0,0,0,0,0,0,"Nepenthe-Engineer Grindspark - On Quest Accept - Run Script"),
(@NPC_GRINDSPARK,0,2,3,61,0,100,0,0,0,0,0,83,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Engineer Grindspark - On Quest Accept - Remove Quest Flag"),
(@NPC_GRINDSPARK,0,3,4,61,0,100,0,0,0,0,0,85,@SPELL_FORCE_SUMMON,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Engineer Grindspark - On Quest Accept - Cast Force Cast Summon GS-9x Multi-Bot"),
(@NPC_GRINDSPARK,0,4,0,61,0,100,0,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Engineer Grindspark - On Quest Accept - Set Phase 1"),
(@NPC_GRINDSPARK*100,9,0,0,0,0,100,0,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,-5069.842285,456.424530,410.453583,3.385522,"Nepenthe-Engineer Grindspark - On Script - Move To Point"), -- XYZO (near bot)
(@NPC_GRINDSPARK*100,9,1,0,0,0,100,0,3000,3000,0,0,17,233,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Engineer Grindspark - On Script - Emote STATE_WORK_MINING"),
(@NPC_GRINDSPARK*100,9,2,0,0,0,100,0,6000,6000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Engineer Grindspark - On Script - Emote None"),
(@NPC_GRINDSPARK*100,9,3,0,0,0,100,0,1000,1000,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Engineer Grindspark - On Script - Say Line 1"),
(@NPC_GRINDSPARK*100,9,4,0,0,0,100,0,5000,5000,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Engineer Grindspark - On Script - Say Line 2"),
(@NPC_GRINDSPARK*100,9,5,0,0,0,100,0,6000,6000,0,0,45,1,1,0,0,0,0,19,@NPC_PROTOTYPE,0,0,0,0,0,0,"Nepenthe-Engineer Grindspark - On Script - Set Data GS-9x Prototype"),
(@NPC_GRINDSPARK*100,9,6,0,0,0,100,0,3000,3000,0,0,69,0,0,0,0,0,0,8,0,0,0,-5072.79,455.677,410.757,2.32129,"Nepenthe-Engineer Grindspark - On Script - Move To Point"), -- XYZO (homepos)
(@NPC_GRINDSPARK*100,9,7,0,0,0,100,0,0,0,0,0,82,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Engineer Grindspark - On Script - Add Quest Flag"),
(@NPC_GRINDSPARK*100,9,8,0,0,0,100,0,9000,9000,0,0,22,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Engineer Grindspark - On Script - Set Phase 0"), -- Event may not occur too often according to retail, so we put a delay on setting phase 0
(@NPC_GRINDSPARK,0,5,0,38,0,100,0,1,1,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Engineer Grindspark - On Data Set - Say Line 3"),
(@NPC_GRINDSPARK,0,6,0,62,0,100,0,@GOSSIP,0,0,0,85,@SPELL_FORCE_SUMMON,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Engineer Grindspark - On Gossip Select - Cast Force Cast Summon GS-9x Multi-Bot"),
(@NPC_GRINDSPARK,0,7,0,62,0,100,0,@GOSSIP,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,"Nepenthe-Engineer Grindspark - On Gossip Select - Close Gossip");
-- GS-9x Prototype
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='', `unit_flags`=0, `speed_run` = 1, `dynamicflags` = 0, `mechanic_immune_mask` = 0 WHERE `entry`=@NPC_PROTOTYPE;
DELETE FROM `creature_template_aura` WHERE `entry`=@NPC_PROTOTYPE;
DELETE FROM `creature` WHERE `id`=@NPC_PROTOTYPE;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(244863, @NPC_PROTOTYPE, 0, 1, 1, 0, 0, -5069.47, 457.511, 410.605, 2.17201, 10, 0, 0, 71, 0, 0, 0, 0, 0);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_PROTOTYPE,@NPC_PROTOTYPE*100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_PROTOTYPE,0,0,0,11,0,100,0,0,0,0,0,11,@SPELL_FEIGN_DEATH,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-GS-9x Prototype - On Respawn - Cast Permanent Feign Death"), -- Casting this through SAI so we can also remove it, unlike using creature_(template_)addon
(@NPC_PROTOTYPE,0,1,0,38,0,100,0,1,1,0,0,80,@NPC_PROTOTYPE*100,0,0,0,0,2,1,0,0,0,0,0,0,0,"Nepenthe-GS-9x Prototype - On Data Set - Run Script"),
(@NPC_PROTOTYPE*100,9,0,0,0,0,100,0,0,0,0,0,28,@SPELL_FEIGN_DEATH,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-GS-9x Prototype - On Script - Remove Permanent Feign Death"),
(@NPC_PROTOTYPE*100,9,1,0,0,0,100,0,1000,1000,0,0,69,0,0,0,0,0,0,8,0,0,0,-5073.289062,455.561096,410.654877,5.802108,"Nepenthe-GS-9x Prototype - On Script - Move To Pos"), -- XYZO (near Engineer Grindspark)
(@NPC_PROTOTYPE*100,9,2,0,0,0,100,0,8000,8000,0,0,11,@SPELL_EXPLOSION,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-GS-9x Prototype - On Script - Cast Explosion"),
(@NPC_PROTOTYPE*100,9,3,0,0,0,100,0,1000,1000,0,0,11,@SPELL_FEIGN_DEATH,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-GS-9x Prototype - On Script - Cast Permanent Feign Death"),
(@NPC_PROTOTYPE*100,9,4,0,0,0,100,0,1000,1000,0,0,45,1,1,0,0,0,0,19,@NPC_GRINDSPARK,0,0,0,0,0,0,"Nepenthe-GS-9x Prototype - On Script - Set Data Engineer Grindspark"),
(@NPC_PROTOTYPE*100,9,5,0,0,0,100,0,1000,1000,0,0,41,5000,0,0,0,0,0,0,0,0,0,0,0,0,0,"Nepenthe-GS-9x Prototype - On Script - Despawn");
DELETE FROM `creature_text` WHERE `entry`=@NPC_GRINDSPARK;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC_GRINDSPARK,0,0,"Time to put the finishing touches on my bot.",12,0,0,0,0,0,"Engineer Grindspark"),
(@NPC_GRINDSPARK,1,0,"At last, it's finished.",12,0,0,25,0,0,"Engineer Grindspark"), -- ONESHOT_POINT
(@NPC_GRINDSPARK,2,0,"Arise my, uh... what shall I call you? How about 'GS-9x Prototype'? Arise, my GS-9x Prototype!",12,0,0,0,0,0,"Engineer Grindspark"),
(@NPC_GRINDSPARK,3,0,"Uh... a couple more tweaks should do it, I think...",12,0,0,0,0,0,"Engineer Grindspark");
-- Toxic Pool
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=@NPC_TOXIC_POOL;
UPDATE `creature` SET `spawntimesecs` = 60 WHERE `id` = @NPC_TOXIC_POOL;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@NPC_TOXIC_POOL AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_TOXIC_POOL,0,0,0,1,0,100,0,1000,1000,6000,6000,11,@SPELL_ERUPTION,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nepenthe-Toxic Pool - OOC timer - Cast Toxic Eruption");
-- GS-9x Multi-Bot
UPDATE `creature_template` SET `ScriptName`='' WHERE `entry`=@NPC_MULTIBOT;
DELETE FROM `creature_template_aura` WHERE `entry`=@NPC_MULTIBOT;
INSERT INTO `creature_template_aura` (`entry`, `aura`) VALUES
(@NPC_MULTIBOT, 79415); -- Multi-Bot Aura
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry` IN (@SPELL_DESPAWN_MULTIBOT, @SPELL_CLEANUP_NPC_TOXIC_POOL, @SPELL_DESPAWN_OBJECT_TOXIC_POOL);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13,1,@SPELL_DESPAWN_MULTIBOT,0,31,1,3,@NPC_MULTIBOT,0,0,'',"Despawn GS-9x Multi-Bot"),
(13,1,@SPELL_CLEANUP_NPC_TOXIC_POOL,0,31,1,3,@NPC_TOXIC_POOL,0,0,'',"Clean Up Toxic Pool"),
(13,1,@SPELL_DESPAWN_OBJECT_TOXIC_POOL,0,31,1,5,@OBJECT_TOXIC_POOL,0,0,'',"Despawn Toxic Pool");
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = @SPELL_CLEANUP_NPC_TOXIC_POOL;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(@SPELL_CLEANUP_NPC_TOXIC_POOL,'94516','0','Clean Up Toxic Pool');
-- Gossip
DELETE FROM `npc_text` WHERE `id`=@TEXTID;
INSERT INTO `npc_text` (`id`, `text0_0`, `WDBVerified`) VALUES
(@TEXTID,"Have you seen my latest invention? It's going to be a hit... If it doesn't explode.",1);
-- Text options
DELETE FROM `gossip_menu_option` WHERE `menu_id`=@GOSSIP;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(@GOSSIP,0,0,"I need a new GS-9x Multi-Bot.",1,1,0,0,0,0,NULL);
DELETE FROM `gossip_menu` WHERE `entry`=@GOSSIP AND `text_id`=@TEXTID;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(@GOSSIP,@TEXTID);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=@GOSSIP;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15,@GOSSIP,0,0,9,@QUEST,0,0,0,'',"Only show gossip option if player has q26205 A Job for the Multi-Bot");
-- Core part
DELETE FROM `spell_script_names` WHERE `spell_id` = 79416;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(79416, 'spell_q26205_multibot_aura');
DELETE FROM `creature_text` WHERE `entry`=@NPC_MULTIBOT;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC_MULTIBOT,0,0,"Initiating cleanup ... ...",12,0,0,0,0,0,"GS-9x Multi-Bot");

-- [SQL] Quests - A Trip to Ironforge quest completion event scripted
-- [SQL] Quests - Follow that Gyro-Copter! quest accept event scripted
SET @NPC_HANDS_SPRINGSPROCKET := 6782;
SET @QUEST_A_TRIP_TO_IRONFORGE := 24490;
SET @QUEST_FOLLOW_TRAT_GYROCOPTER := 24491;
SET @SPELL_SEE_ROCKS := 70046;
SET @SPELL_FOLLOW_TRAT_GYROCOPTER_START_SEE_MILO_GEARTWINGE := 70047;
SET @SPELL_SIGNAL_FLARE := 70048;
SET @SPELL_SHAKE := 44681;
-- SET @SPELL_STONE_EFFECT := ; -- TODO: Add stone effect (spells 70040+)
UPDATE `creature_template` SET `AIName` = "SmartAI", `ScriptName` = "" WHERE `entry` = @NPC_HANDS_SPRINGSPROCKET;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_HANDS_SPRINGSPROCKET, @NPC_HANDS_SPRINGSPROCKET*100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_HANDS_SPRINGSPROCKET, 0, 0, 1, 20, 0, 100, 0, @QUEST_A_TRIP_TO_IRONFORGE, 0, 0, 0, 11, @SPELL_SEE_ROCKS, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Nepenthe-Hands Springsprocket - On Quest Complete - Cast See Coldridge Tunnel Rocks - See Quest Invis 1"),
(@NPC_HANDS_SPRINGSPROCKET, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 11, @SPELL_SHAKE, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Nepenthe-Hands Springsprocket - On Quest Complete - Cast Camera Shake (No Sound)"),
(@NPC_HANDS_SPRINGSPROCKET, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Nepenthe-Hands Springsprocket - On Quest Complete - Say Line 0"),
(@NPC_HANDS_SPRINGSPROCKET, 0, 3, 0, 19, 0, 100, 0, @QUEST_FOLLOW_TRAT_GYROCOPTER, 0, 0, 0, 80, @NPC_HANDS_SPRINGSPROCKET*100, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Nepenthe-Hands Springsprocket - On Quest Accept - Run Script"),
(@NPC_HANDS_SPRINGSPROCKET*100, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Nepenthe-Hands Springsprocket - On Script - Say Line 1"),
(@NPC_HANDS_SPRINGSPROCKET*100, 9, 1, 0, 0, 0, 100, 0, 500, 500, 0, 0, 11, @SPELL_SIGNAL_FLARE, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Nepenthe-Hands Springsprocket - On Script - Cast Signal Flare"),
(@NPC_HANDS_SPRINGSPROCKET*100, 9, 2, 0, 0, 0, 100, 0, 8000, 8000, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Nepenthe-Hands Springsprocket - On Script - Say Line 2"),
(@NPC_HANDS_SPRINGSPROCKET*100, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 11, @SPELL_FOLLOW_TRAT_GYROCOPTER_START_SEE_MILO_GEARTWINGE, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Nepenthe-Hands Springsprocket - On Script - Cast See Milo Geartwinge - See Quest Invis 2");
DELETE FROM `creature_text` WHERE `entry` = @NPC_HANDS_SPRINGSPROCKET;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@NPC_HANDS_SPRINGSPROCKET, 0, 0, "Alright, so you're just going to head through this tunnel and whaaaaa....?", 12, 0, 100, 0, 0, 0, "Hands Springsprocket"),
(@NPC_HANDS_SPRINGSPROCKET, 1, 0, "I recommend that you return to Anvilmar, $N... and quickly! I'll send the signal to the Gnomeregan Airmen.", 12, 0, 100, 430, 0, 0, "Hands Springsprocket"),
(@NPC_HANDS_SPRINGSPROCKET, 2, 0, "Look for a fellow by the name of Milo Geartwinge. I suppose I'll see what I can do about this cave-in.", 12, 0, 100, 1, 0, 0, "Hands Springsprocket");

-- Gnomeregan Infantry was unselectable
UPDATE `creature_template` SET `unit_flags` = 32768, `mechanic_immune_mask` = 652951551, `flags_extra` = 32768 WHERE `entry` = 42316;
-- Healing Shield was visible and moving
UPDATE `creature` SET `modelid` = 23257, `spawndist` = 0, `curhealth` = 42, `MovementType` = 0 WHERE `id` = 42557;

-- Quests - Scrounging for Parts: Spare Parts will only be selectable while on quest
UPDATE `gameobject_template` SET `flags` = `flags`|4 WHERE `entry` IN (203968, 203443, 203964, 203965, 203966, 203967);

-- TODO
/*-- [SQL] Quests - Pack Your Bags quest completion event scripted
SET @NPC_MILO_GEARTWINGE := 37113;
SET @NPC_GYROCOPTER := 37169;
SET @SPELL_FORCECAST_PACK_YOUR_BAGS_COMPLETE_SUMMON_GYROCOPTER := 70032;
SET @SPELL_RIDE_MILOS_GYROCOPTER := 70036;
SET @SPELL_MILO_GEARTWINGE_INVIS := 70045;
SET @QUEST_PACK_YOUR_BAGS := 24492;
DELETE FROM `creature_template_aura` WHERE `entry` IN (@NPC_MILO_GEARTWINGE, @NPC_GYROCOPTER);
INSERT INTO `creature_template_aura` (`entry`, `aura`) VALUES
(@NPC_MILO_GEARTWINGE, @SPELL_MILO_GEARTWINGE_INVIS),
(@NPC_GYROCOPTER, @SPELL_MILO_GEARTWINGE_INVIS); -- Milo Geartwinge Invisibility - Quest Invis 2
UPDATE `creature_template` SET `AIName` = "SmartAI", `ScriptName` = "" WHERE `entry` = @NPC_MILO_GEARTWINGE;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@NPC_MILO_GEARTWINGE, @NPC_MILO_GEARTWINGE*100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@NPC_MILO_GEARTWINGE, 0, 0, 0, 20, 0, 100, 0, @QUEST_PACK_YOUR_BAGS, 0, 0, 0, 80, @NPC_MILO_GEARTWINGE*100, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Nepenthe-Milo Geartwinge - On Quest Complete - Run Script"),
*/