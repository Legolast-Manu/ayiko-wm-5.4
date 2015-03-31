/* Prince Liam Greymane now is not attackable and don't start fight */
UPDATE `creature_template` SET `unit_flags` = 131074 WHERE `entry` = 36140;

/* Wrong Title, Details, Description, Objective for MoP Warlock quests - 14724 Worgen and 14012 Goblin */
UPDATE `quest_template` SET `Title` = 'Corruption', `Objectives` = 'Reach level 3 to learn Corruption.$BPractice casting it on a Bloodfang Worgen.', `Details` = 'It is true then! You live!$B$BI never got a chance to share my greatest discovery with you. After I began my training, and began to become accomplished, I heard the voice Balcephoth of the felshadows in my head... he taught me the forbidden secrets of...$B$BCorruption!$B$BGo now, gain some more experience, maybe you''ll learn a new spell, too. You can go use it against the Bloodfang Worgen to our west.', `ObjectiveText1` = 'Practice Corruption' WHERE `Id` = 14274;
UPDATE `quest_template` SET `Title` = 'Corruption', `Objectives` = 'Reach level 3 to learn Corruption, then cast it 3 times on a Training Dummy.', `Details` = 'I''m Evol Fingers, your personal trainer, $g sir : ma''am;.$B$BI understand that you''re interested in some new insight about being a $c? Look. Just get some more experience , and you''ll learn how to do more of what it is you do best. Helpful, right?$B$BSo go, get some more experience, then when you learn how to do something new go ahead and try it out on one of the dummies here.', `ObjectiveText1` = 'Practiced Corruption' WHERE `Id` = 14012;

/* Ling of the Six Pools now give quests to horde as well */
UPDATE `quest_template` SET `RequiredRaces` = 0 WHERE `Id` IN (31196,31197,31198,31199,31200,31201,31203,31204,31220,31221);
