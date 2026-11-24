-- FCBots Database Base Schema (standalone)
-- Creates the fcbots database: update tracking tables and bot tables.
-- Column types follow the live DizerCore schema (mysql_export_indexed_DB):
--   auth.account.id              int(10) signed
--   characters.characters.guid   bigint(20) signed
--   world.creature_template.entry int(10) signed

CREATE TABLE `updates` (
  `name` varchar(200) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT 'filename with extension of the update.',
  `hash` char(40) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT '' COMMENT 'sha1 hash of the sql file.',
  `state` enum('RELEASED','ARCHIVED') CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'RELEASED' COMMENT 'defines if an update is released or archived.',
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'timestamp when the query was applied.',
  `speed` int unsigned NOT NULL DEFAULT '0' COMMENT 'time the query takes to apply in ms.',
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='List of all applied updates in this database.';

CREATE TABLE `updates_include` (
  `path` varchar(200) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT 'directory to include. $ means relative to the source directory.',
  `state` enum('RELEASED','ARCHIVED') CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'RELEASED' COMMENT 'defines if the directory contains released or archived updates.',
  PRIMARY KEY (`path`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='List of directories where we want to include sql updates.';

INSERT INTO `updates_include` (`path`, `state`) VALUES
('$/sql/updates/fcbots', 'RELEASED');

CREATE TABLE `bot_accounts` (
  `account_id` int NOT NULL COMMENT 'auth.account.id',
  `account_type` tinyint unsigned NOT NULL DEFAULT 0 COMMENT '0 = unassigned, 1 = FCBot',
  `assignment_date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`account_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='FCBots account assignments';

CREATE TABLE `bot_templates` (
  `entry` int NOT NULL COMMENT 'world.creature_template.entry. FCBots entries use 10000000-19999999.',
  `bot_class` tinyint unsigned NOT NULL COMMENT 'ChrClasses ID, 1-13',
  `bot_race` tinyint unsigned NOT NULL COMMENT 'ChrRaces ID',
  `companion_spell` int NOT NULL DEFAULT 0,
  `chatter_type` tinyint unsigned NOT NULL DEFAULT 0,
  `gender` tinyint unsigned NOT NULL DEFAULT 2 COMMENT '0 male, 1 female, 2 unknown',
  `pet_source` int NOT NULL DEFAULT 0,
  `spec_id` tinyint unsigned NOT NULL DEFAULT 0 COMMENT '0 none, 1 Arms, 2 Fury, 3 Protection (Warrior, Talents.h Spec)',
  `hero_tree_id` tinyint unsigned NOT NULL DEFAULT 0 COMMENT '0 none, 1 Colossus, 2 Slayer, 3 Mountain Thane (Talents.h HeroTree)',
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='FCBots bot template definitions';

CREATE TABLE `bot_owners` (
  `bot_guid` bigint NOT NULL COMMENT 'characters.characters.guid of the bot',
  `owner_guid` bigint NOT NULL COMMENT 'characters.characters.guid of the hiring player',
  `hired_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`bot_guid`),
  KEY `idx_owner_guid` (`owner_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='FCBots hire records';
