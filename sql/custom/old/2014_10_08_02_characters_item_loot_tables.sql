DROP TABLE IF EXISTS `item_loot_items`;
CREATE TABLE `item_loot_items` (
  `container_id` int(10) unsigned NOT NULL COMMENT 'guid of container (item_instance.guid)',
  `item_id` int(10) unsigned NOT NULL COMMENT 'loot item entry (item_instance.itemEntry)',
  `item_count` int(10) NOT NULL DEFAULT '0' COMMENT 'stack size',
  `follow_rules` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'follow loot rules',
  `ffa` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'free-for-all',
  `blocked` tinyint(1) NOT NULL DEFAULT '0',
  `counted` tinyint(1) NOT NULL DEFAULT '0',
  `under_threshold` tinyint(1) NOT NULL DEFAULT '0',
  `needs_quest` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'quest drop',
  `rnd_prop` int(10) NOT NULL DEFAULT '0' COMMENT 'random enchantment added when originally rolled',
  `rnd_suffix` int(10) NOT NULL DEFAULT '0' COMMENT 'random suffix added when originally rolled',
  KEY `fk__item_loot_items__item_instance` (`container_id`),
  CONSTRAINT `fk__item_loot_items__item_instance`
    FOREIGN KEY (`container_id`) REFERENCES `item_instance` (`guid`)
    ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

DROP TABLE IF EXISTS `item_loot_money`;
CREATE TABLE `item_loot_money` (
  `container_id` int(10) unsigned NOT NULL COMMENT 'guid of container (item_instance.guid)',
  `money` int(10) NOT NULL DEFAULT '0' COMMENT 'money loot (in copper)',
  PRIMARY KEY (`container_id`),
  CONSTRAINT `fk__item_loot_money__item_instance`
    FOREIGN KEY (`container_id`) REFERENCES `item_instance` (`guid`)
    ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;
