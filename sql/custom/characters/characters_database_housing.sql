-- Housing System Database Schema for DizerCore
-- This file contains all housing-related tables for the characters database

-- ============================================================================
-- NEIGHBORHOOD SYSTEM
-- ============================================================================

-- Neighborhoods table - stores neighborhood information
CREATE TABLE IF NOT EXISTS `neighborhoods` (
  `neighborhoodId` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'Unique neighborhood identifier',
  `name` varchar(100) NOT NULL COMMENT 'Neighborhood name',
  `type` tinyint unsigned NOT NULL DEFAULT '0' COMMENT '0=Public, 1=Guild, 2=Private',
  `faction` tinyint unsigned NOT NULL COMMENT '0=Alliance, 1=Horde',
  `guildId` int unsigned DEFAULT NULL COMMENT 'Guild ID for guild neighborhoods',
  `ownerAccountId` int unsigned DEFAULT NULL COMMENT 'Account ID for private neighborhoods',
  `creationDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'When neighborhood was created',
  `isActive` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'Whether neighborhood is active',
  `settings` text COMMENT 'JSON serialized neighborhood settings',
  PRIMARY KEY (`neighborhoodId`),
  KEY `idx_type` (`type`),
  KEY `idx_faction` (`faction`),
  KEY `idx_guild` (`guildId`),
  KEY `idx_account` (`ownerAccountId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='Housing neighborhoods';

-- Neighborhood plots table - stores individual plot information
CREATE TABLE IF NOT EXISTS `neighborhood_plots` (
  `plotId` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'Unique plot identifier',
  `neighborhoodId` int unsigned NOT NULL COMMENT 'Reference to neighborhoods table',
  `plotIndex` tinyint unsigned NOT NULL COMMENT 'Plot index within neighborhood (0-54)',
  `ownerGuid` bigint unsigned DEFAULT NULL COMMENT 'Player GUID who owns this plot',
  `houseId` int unsigned DEFAULT NULL COMMENT 'Reference to player_houses table',
  `isReserved` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Whether plot is reserved',
  `reservedByAccountId` int unsigned DEFAULT NULL COMMENT 'Account that reserved the plot',
  `reservationExpiry` timestamp NULL DEFAULT NULL COMMENT 'When reservation expires',
  `positionX` float NOT NULL DEFAULT '0' COMMENT 'Plot X position in neighborhood',
  `positionY` float NOT NULL DEFAULT '0' COMMENT 'Plot Y position in neighborhood',
  `positionZ` float NOT NULL DEFAULT '0' COMMENT 'Plot Z position in neighborhood',
  `orientation` float NOT NULL DEFAULT '0' COMMENT 'Plot orientation',
  `isActive` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'Whether plot is active',
  PRIMARY KEY (`plotId`),
  UNIQUE KEY `uniq_neighborhood_plot` (`neighborhoodId`, `plotIndex`),
  KEY `idx_owner` (`ownerGuid`),
  KEY `idx_house` (`houseId`),
  KEY `idx_reservation` (`reservedByAccountId`, `reservationExpiry`),
  CONSTRAINT `fk_plot_neighborhood` FOREIGN KEY (`neighborhoodId`) REFERENCES `neighborhoods` (`neighborhoodId`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='Neighborhood plots';

-- Neighborhood endeavors table - tracks neighborhood activities
CREATE TABLE IF NOT EXISTS `neighborhood_endeavors` (
  `endeavorId` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'Unique endeavor identifier',
  `neighborhoodId` int unsigned NOT NULL COMMENT 'Reference to neighborhoods table',
  `endeavorTemplateId` int unsigned NOT NULL COMMENT 'DBC template ID for this endeavor',
  `startDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'When endeavor started',
  `endDate` timestamp NULL DEFAULT NULL COMMENT 'When endeavor ends',
  `progress` int unsigned NOT NULL DEFAULT '0' COMMENT 'Current progress toward completion',
  `targetProgress` int unsigned NOT NULL DEFAULT '100' COMMENT 'Target progress to complete',
  `isActive` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'Whether endeavor is active',
  `isCompleted` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Whether endeavor is completed',
  PRIMARY KEY (`endeavorId`),
  KEY `idx_neighborhood` (`neighborhoodId`),
  KEY `idx_template` (`endeavorTemplateId`),
  KEY `idx_active` (`isActive`, `isCompleted`),
  CONSTRAINT `fk_endeavor_neighborhood` FOREIGN KEY (`neighborhoodId`) REFERENCES `neighborhoods` (`neighborhoodId`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='Neighborhood endeavors';

-- ============================================================================
-- PLAYER HOUSES
-- ============================================================================

-- Player houses table - stores house information
CREATE TABLE IF NOT EXISTS `player_houses` (
  `houseId` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'Unique house identifier',
  `ownerGuid` bigint unsigned NOT NULL COMMENT 'Player GUID who owns this house',
  `plotId` int unsigned DEFAULT NULL COMMENT 'Reference to neighborhood_plots table',
  `houseTemplateId` int unsigned NOT NULL COMMENT 'DBC House template ID',
  `houseType` tinyint unsigned NOT NULL DEFAULT '0' COMMENT '0=Small, 1=Medium, 2=Large',
  `houseStyle` tinyint unsigned NOT NULL COMMENT 'Faction-specific house style',
  `houseLevel` tinyint unsigned NOT NULL DEFAULT '1' COMMENT 'Current house level (1-10)',
  `houseXP` int unsigned NOT NULL DEFAULT '0' COMMENT 'Current house experience points',
  `interiorDecorBudget` int unsigned NOT NULL DEFAULT '910' COMMENT 'Interior decor placement budget',
  `roomPlacementBudget` int unsigned NOT NULL DEFAULT '19' COMMENT 'Room placement budget',
  `exteriorDecorBudget` int unsigned NOT NULL DEFAULT '200' COMMENT 'Exterior decor placement budget',
  `creationDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'When house was created',
  `lastModified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Last modification time',
  `isActive` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'Whether house is active',
  `settings` text COMMENT 'JSON serialized house settings',
  PRIMARY KEY (`houseId`),
  KEY `idx_owner` (`ownerGuid`),
  KEY `idx_plot` (`plotId`),
  KEY `idx_level` (`houseLevel`),
  CONSTRAINT `fk_house_plot` FOREIGN KEY (`plotId`) REFERENCES `neighborhood_plots` (`plotId`) ON DELETE SET NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='Player houses';

-- House fixtures table - stores exterior fixture configurations
CREATE TABLE IF NOT EXISTS `house_fixtures` (
  `fixtureId` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'Unique fixture identifier',
  `houseId` int unsigned NOT NULL COMMENT 'Reference to player_houses table',
  `fixtureSlot` tinyint unsigned NOT NULL COMMENT 'Fixture slot position',
  `fixtureTemplateId` int unsigned NOT NULL COMMENT 'DBC fixture template ID',
  `variantId` int unsigned NOT NULL DEFAULT '0' COMMENT 'Fixture variant/style',
  `positionX` float NOT NULL DEFAULT '0' COMMENT 'Fixture X position',
  `positionY` float NOT NULL DEFAULT '0' COMMENT 'Fixture Y position',
  `positionZ` float NOT NULL DEFAULT '0' COMMENT 'Fixture Z position',
  `orientation` float NOT NULL DEFAULT '0' COMMENT 'Fixture orientation',
  PRIMARY KEY (`fixtureId`),
  UNIQUE KEY `uniq_house_fixture` (`houseId`, `fixtureSlot`),
  KEY `idx_template` (`fixtureTemplateId`),
  CONSTRAINT `fk_fixture_house` FOREIGN KEY (`houseId`) REFERENCES `player_houses` (`houseId`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='House exterior fixtures';

-- ============================================================================
-- ROOM SYSTEM
-- ============================================================================

-- House rooms table - stores room placements
CREATE TABLE IF NOT EXISTS `house_rooms` (
  `roomId` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'Unique room identifier',
  `houseId` int unsigned NOT NULL COMMENT 'Reference to player_houses table',
  `roomTemplateId` int unsigned NOT NULL COMMENT 'DBC Room template ID',
  `positionX` float NOT NULL DEFAULT '0' COMMENT 'Room X position in house grid',
  `positionY` float NOT NULL DEFAULT '0' COMMENT 'Room Y position in house grid',
  `orientation` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'Room rotation (0-3)',
  `ceilingType` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'Ceiling style/type',
  `doorType` tinyint unsigned NOT NULL DEFAULT '0' COMMENT 'Door style/type',
  `componentTheme` int unsigned NOT NULL DEFAULT '0' COMMENT 'Component theme ID',
  `isActive` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'Whether room is active',
  PRIMARY KEY (`roomId`),
  KEY `idx_house` (`houseId`),
  KEY `idx_template` (`roomTemplateId`),
  CONSTRAINT `fk_room_house` FOREIGN KEY (`houseId`) REFERENCES `player_houses` (`houseId`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='House room placements';

-- Room component materials table - stores material applications
CREATE TABLE IF NOT EXISTS `room_materials` (
  `materialId` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'Unique material identifier',
  `roomId` int unsigned NOT NULL COMMENT 'Reference to house_rooms table',
  `componentSlot` tinyint unsigned NOT NULL COMMENT 'Component slot to apply material to',
  `materialId` int unsigned NOT NULL COMMENT 'DBC material template ID',
  PRIMARY KEY (`materialId`),
  UNIQUE KEY `uniq_room_component` (`roomId`, `componentSlot`),
  CONSTRAINT `fk_material_room` FOREIGN KEY (`roomId`) REFERENCES `house_rooms` (`roomId`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='Room material applications';

-- ============================================================================
-- DECOR SYSTEM
-- ============================================================================

-- Player decor collection table - stores decor items owned by players
CREATE TABLE IF NOT EXISTS `player_decor_collection` (
  `collectionId` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'Unique collection entry',
  `ownerGuid` bigint unsigned NOT NULL COMMENT 'Player GUID who owns this decor',
  `decorTemplateId` int unsigned NOT NULL COMMENT 'DBC Decor template ID',
  `quantity` smallint unsigned NOT NULL DEFAULT '1' COMMENT 'Quantity of this decor item',
  `isNew` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'Whether this is a newly acquired item',
  `acquisitionDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'When decor was acquired',
  PRIMARY KEY (`collectionId`),
  UNIQUE KEY `uniq_owner_decor` (`ownerGuid`, `decorTemplateId`),
  KEY `idx_template` (`decorTemplateId`),
  KEY `idx_new` (`isNew`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='Player decor collections';

-- Placed decor table - stores decor items placed in houses/plots
CREATE TABLE IF NOT EXISTS `placed_decor` (
  `placedDecorId` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'Unique placed decor identifier',
  `houseId` int unsigned NOT NULL COMMENT 'Reference to player_houses table',
  `roomId` int unsigned DEFAULT NULL COMMENT 'Reference to house_rooms table (NULL if exterior)',
  `decorTemplateId` int unsigned NOT NULL COMMENT 'DBC Decor template ID',
  `positionX` float NOT NULL DEFAULT '0' COMMENT 'Decor X position',
  `positionY` float NOT NULL DEFAULT '0' COMMENT 'Decor Y position',
  `positionZ` float NOT NULL DEFAULT '0' COMMENT 'Decor Z position',
  `orientationX` float NOT NULL DEFAULT '0' COMMENT 'Decor X rotation',
  `orientationY` float NOT NULL DEFAULT '0' COMMENT 'Decor Y rotation',
  `orientationZ` float NOT NULL DEFAULT '0' COMMENT 'Decor Z rotation',
  `scale` float NOT NULL DEFAULT '1' COMMENT 'Decor scale',
  `isExterior` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Whether decor is placed exterior',
  `isLocked` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Whether decor is locked in place',
  `dyeSlot1` int unsigned DEFAULT NULL COMMENT 'First dye slot color',
  `dyeSlot2` int unsigned DEFAULT NULL COMMENT 'Second dye slot color',
  `dyeSlot3` int unsigned DEFAULT NULL COMMENT 'Third dye slot color',
  `customizationData` text COMMENT 'Additional customization data (JSON)',
  `placementCost` int unsigned NOT NULL DEFAULT '1' COMMENT 'Budget cost for this placement',
  `placementDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'When decor was placed',
  PRIMARY KEY (`placedDecorId`),
  KEY `idx_house` (`houseId`),
  KEY `idx_room` (`roomId`),
  KEY `idx_template` (`decorTemplateId`),
  KEY `idx_exterior` (`isExterior`),
  CONSTRAINT `fk_decor_house` FOREIGN KEY (`houseId`) REFERENCES `player_houses` (`houseId`) ON DELETE CASCADE,
  CONSTRAINT `fk_decor_room` FOREIGN KEY (`roomId`) REFERENCES `house_rooms` (`roomId`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='Placed decor items';

-- ============================================================================
-- PERMISSION SYSTEM
-- ============================================================================

-- House permissions table - stores access permissions for houses
CREATE TABLE IF NOT EXISTS `house_permissions` (
  `permissionId` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'Unique permission identifier',
  `houseId` int unsigned NOT NULL COMMENT 'Reference to player_houses table',
  `targetGuid` bigint unsigned DEFAULT NULL COMMENT 'Player GUID (NULL for guild/general)',
  `targetGuildId` int unsigned DEFAULT NULL COMMENT 'Guild ID (if applicable)',
  `permissionType` tinyint unsigned NOT NULL COMMENT '0=Neighbor, 1=Guild, 2=Party, 3=Friend, 4=Specific',
  `canEnterInterior` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Can enter house interior',
  `canEnterExterior` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Can enter plot exterior',
  `canEditInterior` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Can edit interior',
  `canEditExterior` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Can edit exterior',
  `canCopyBlueprint` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Can copy house blueprint',
  `grantedDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'When permission was granted',
  `grantedByGuid` bigint unsigned NOT NULL COMMENT 'Player GUID who granted permission',
  PRIMARY KEY (`permissionId`),
  KEY `idx_house` (`houseId`),
  KEY `idx_target` (`targetGuid`),
  KEY `idx_guild` (`targetGuildId`),
  KEY `idx_type` (`permissionType`),
  CONSTRAINT `fk_permission_house` FOREIGN KEY (`houseId`) REFERENCES `player_houses` (`houseId`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='House access permissions';

-- ============================================================================
-- BLUEPRINT SYSTEM
-- ============================================================================

-- House blueprints table - stores saved house configurations
CREATE TABLE IF NOT EXISTS `house_blueprints` (
  `blueprintId` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'Unique blueprint identifier',
  `ownerGuid` bigint unsigned NOT NULL COMMENT 'Player GUID who owns this blueprint',
  `houseId` int unsigned DEFAULT NULL COMMENT 'Reference to player_houses table (NULL if not linked)',
  `name` varchar(100) NOT NULL COMMENT 'Blueprint name',
  `description` varchar(255) DEFAULT NULL COMMENT 'Blueprint description',
  `houseTemplateId` int unsigned NOT NULL COMMENT 'House template used',
  `houseData` longtext NOT NULL COMMENT 'Serialized house configuration data',
  `thumbnail` blob COMMENT 'Blueprint thumbnail image data',
  `isPublic` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Whether blueprint is publicly shareable',
  `shareCode` varchar(20) DEFAULT NULL COMMENT 'Unique share code for public blueprints',
  `creationDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'When blueprint was created',
  `lastModified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Last modification time',
  PRIMARY KEY (`blueprintId`),
  KEY `idx_owner` (`ownerGuid`),
  KEY `idx_house` (`houseId`),
  KEY `idx_public` (`isPublic`),
  KEY `idx_share` (`shareCode`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='House blueprints';

-- ============================================================================
-- HOUSE LEVELING SYSTEM
-- ============================================================================

-- House level rewards table - tracks unlocked rewards per house
CREATE TABLE IF NOT EXISTS `house_level_rewards` (
  `rewardId` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'Unique reward identifier',
  `houseId` int unsigned NOT NULL COMMENT 'Reference to player_houses table',
  `level` tinyint unsigned NOT NULL COMMENT 'House level this reward belongs to',
  `rewardType` tinyint unsigned NOT NULL COMMENT '0=Room, 1=Exterior, 2=Decor, 3=Feature',
  `rewardTemplateId` int unsigned NOT NULL COMMENT 'Template ID for the reward',
  `isUnlocked` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Whether reward is unlocked',
  `unlockedDate` timestamp NULL DEFAULT NULL COMMENT 'When reward was unlocked',
  PRIMARY KEY (`rewardId`),
  UNIQUE KEY `uniq_house_level_reward` (`houseId`, `level`, `rewardType`, `rewardTemplateId`),
  KEY `idx_house` (`houseId`),
  KEY `idx_level` (`level`),
  CONSTRAINT `fk_reward_house` FOREIGN KEY (`houseId`) REFERENCES `player_houses` (`houseId`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='House level rewards';

-- ============================================================================
-- HOUSE SETTINGS
-- ============================================================================

-- House settings table - stores per-house configuration
CREATE TABLE IF NOT EXISTS `house_settings` (
  `settingId` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'Unique setting identifier',
  `houseId` int unsigned NOT NULL COMMENT 'Reference to player_houses table',
  `settingKey` varchar(50) NOT NULL COMMENT 'Setting key/name',
  `settingValue` text NOT NULL COMMENT 'Setting value (can be JSON)',
  `settingType` tinyint unsigned NOT NULL DEFAULT '0' COMMENT '0=String, 1=Integer, 2=Float, 3=Boolean, 4=JSON',
  PRIMARY KEY (`settingId`),
  UNIQUE KEY `uniq_house_setting` (`houseId`, `settingKey`),
  CONSTRAINT `fk_setting_house` FOREIGN KEY (`houseId`) REFERENCES `player_houses` (`houseId`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='House configuration settings';

-- ============================================================================
-- INDEXES FOR PERFORMANCE
-- ============================================================================

-- Additional indexes for common queries
CREATE INDEX `idx_player_houses_owner_level` ON `player_houses` (`ownerGuid`, `houseLevel`);
CREATE INDEX `idx_neighborhood_plots_active` ON `neighborhood_plots` (`neighborhoodId`, `isActive`);
CREATE INDEX `idx_placed_decor_location` ON `placed_decor` (`houseId`, `isExterior`, `positionX`, `positionY`);
CREATE INDEX `idx_house_permissions_target` ON `house_permissions` (`targetGuid`, `canEnterInterior`, `canEnterExterior`);
