-- ============================ house ============================  
-- Housing System Hotfix (DB2) Schema for DizerCore  
-- Retail-accurate columns matching DB2LoadInfo.h Fields[] and Phase-2 SELECTs  
  
-- Remove stale hotfix_blob/hotfix_data rows for housing tables now loaded as DB2 stores  
DELETE FROM `hotfix_data` WHERE `TableHash` IN (0x137145D0, 0x24EE8F09, 0xBBA2E8F8);  
DELETE FROM `hotfix_blob` WHERE `TableHash` IN (0x137145D0, 0x24EE8F09, 0xBBA2E8F8);  
  
  
DROP TABLE IF EXISTS `house`;  
CREATE TABLE `house` (  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0, `Name` TEXT,  
  `Field_12_0_0_63534_001` INT NOT NULL DEFAULT 0,  
  `Field_12_0_0_63534_002` INT NOT NULL DEFAULT 0,  
  `Field_12_0_0_63534_003` INT NOT NULL DEFAULT 0,  
  `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `house_decor`;  
CREATE TABLE `house_decor` (  
  `Name` TEXT,  
  `InitialRotation1` FLOAT UNSIGNED NOT NULL DEFAULT 0,  
  `InitialRotation2` FLOAT UNSIGNED NOT NULL DEFAULT 0,  
  `InitialRotation3` FLOAT UNSIGNED NOT NULL DEFAULT 0,  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0,  
  `GameObjectID` INT NOT NULL DEFAULT 0, `Flags` INT NOT NULL DEFAULT 0,  
  `Type` TINYINT UNSIGNED NOT NULL DEFAULT 0, `ModelType` TINYINT UNSIGNED NOT NULL DEFAULT 0,  
  `ModelFileDataID` INT NOT NULL DEFAULT 0, `ThumbnailFileDataID` INT NOT NULL DEFAULT 0,  
  `WeightCost` INT NOT NULL DEFAULT 0, `ItemID` INT NOT NULL DEFAULT 0,  
  `InitialScale` FLOAT UNSIGNED NOT NULL DEFAULT 0,  
  `FirstTimeAcquisitionXP` INT NOT NULL DEFAULT 0, `OrderIndex` INT NOT NULL DEFAULT 0,  
  `Field_12_0_0_63534_015` TINYINT NOT NULL DEFAULT 0,  
  `StartingQuantity` INT NOT NULL DEFAULT 0, `UiModelSceneID` INT NOT NULL DEFAULT 0,  
  `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `house_decor_material`;  
CREATE TABLE `house_decor_material` (  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0,  
  `WMOMaterialReference` BIGINT UNSIGNED NOT NULL DEFAULT 0,  
  `MaterialTextureIndex` INT NOT NULL DEFAULT 0, `HouseThemeID` INT NOT NULL DEFAULT 0,  
  `TextureAFileDataID` INT NOT NULL DEFAULT 0, `TextureBFileDataID` INT NOT NULL DEFAULT 0,  
  `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `house_decor_theme_set`;  
CREATE TABLE `house_decor_theme_set` (  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0, `Name` TEXT,  
  `Field_12_0_0_63534_001` INT NOT NULL DEFAULT 0,  
  `Field_12_0_0_63534_002` INT NOT NULL DEFAULT 0,  
  `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `house_exterior_wmo_data`;  
CREATE TABLE `house_exterior_wmo_data` (  
  `Name` TEXT, `ID` INT UNSIGNED NOT NULL DEFAULT 0,  
  `Flags` INT NOT NULL DEFAULT 0,  
  `Field_12_0_5_66330_003` INT NOT NULL DEFAULT 0,  
  `Field_12_0_5_66330_004` INT NOT NULL DEFAULT 0,  
  `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `house_level_data`;  
CREATE TABLE `house_level_data` (  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0, `Level` TINYINT UNSIGNED NOT NULL DEFAULT 0,  
  `QuestID` INT UNSIGNED NOT NULL DEFAULT 0,  
  `Field_12_0_7_67808_003` INT NOT NULL DEFAULT 0,  
  `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `house_level_reward_info`;  
CREATE TABLE `house_level_reward_info` (  
  `Name` TEXT, `Description` TEXT,  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0,  
  `HouseLevelDataID` INT NOT NULL DEFAULT 0,  
  `Field_12_0_0_63967_004` INT NOT NULL DEFAULT 0,  
  `IconFileDataID` INT NOT NULL DEFAULT 0,  
  `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `house_room`;  
CREATE TABLE `house_room` (  
  `Name` TEXT, `ID` INT UNSIGNED NOT NULL DEFAULT 0,  
  `Field_12_0_0_63967_001` TINYINT NOT NULL DEFAULT 0, `Flags` INT NOT NULL DEFAULT 0,  
  `Field_12_0_0_63724_002` INT NOT NULL DEFAULT 0, `RoomWmoDataID` INT NOT NULL DEFAULT 0,  
  `UiTextureAtlasElementID` INT NOT NULL DEFAULT 0, `WeightCost` INT NOT NULL DEFAULT 0,  
  `ItemID` INT NOT NULL DEFAULT 0, `SortPriority` INT NOT NULL DEFAULT 0,  
  `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `house_theme`;  
CREATE TABLE `house_theme` (  
  `Name` TEXT, `ID` INT UNSIGNED NOT NULL DEFAULT 0,  
  `Flags` INT NOT NULL DEFAULT 0, `ParentThemeID` INT NOT NULL DEFAULT 0,  
  `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `neighborhood_initiative`;  
CREATE TABLE `neighborhood_initiative` (  
  `Name` TEXT, `Description` TEXT,  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0, `Flags` INT NOT NULL DEFAULT 0,  
  `Field_12_0_0_63534_004` INT NOT NULL DEFAULT 0,  
  `Field_12_0_0_63534_005` INT NOT NULL DEFAULT 0,  
  `Field_12_0_0_63534_006` INT NOT NULL DEFAULT 0,  
  `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `neighborhood_map`;  
CREATE TABLE `neighborhood_map` (  
  `EntryPosition1` FLOAT UNSIGNED NOT NULL DEFAULT 0,  
  `EntryPosition2` FLOAT UNSIGNED NOT NULL DEFAULT 0,  
  `EntryPosition3` FLOAT UNSIGNED NOT NULL DEFAULT 0,  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0, `MapID` INT NOT NULL DEFAULT 0,  
  `EntryRotation` FLOAT UNSIGNED NOT NULL DEFAULT 0,  
  `UiTextureKitID` INT UNSIGNED NOT NULL DEFAULT 0, `Flags` INT NOT NULL DEFAULT 0,  
  `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `neighborhood_name_gen`;  
CREATE TABLE `neighborhood_name_gen` (  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0,  
  `Prefix` TEXT, `Middle` TEXT, `Suffix` TEXT,  
  `NeighborhoodMapID` INT UNSIGNED NOT NULL DEFAULT 0,  
  `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `neighborhood_plot`;  
CREATE TABLE `neighborhood_plot` (  
  `Cost` BIGINT UNSIGNED NOT NULL DEFAULT 0, `Name` TEXT,  
  `Field_0041` FLOAT NOT NULL DEFAULT 0, `Field_0042` FLOAT NOT NULL DEFAULT 0, `Field_0043` FLOAT NOT NULL DEFAULT 0,  
  `Field_0051` FLOAT NOT NULL DEFAULT 0, `Field_0052` FLOAT NOT NULL DEFAULT 0, `Field_0053` FLOAT NOT NULL DEFAULT 0,  
  `CornerstonePosition1` FLOAT NOT NULL DEFAULT 0, `CornerstonePosition2` FLOAT NOT NULL DEFAULT 0, `CornerstonePosition3` FLOAT NOT NULL DEFAULT 0,  
  `CornerstoneRotation1` FLOAT NOT NULL DEFAULT 0, `CornerstoneRotation2` FLOAT NOT NULL DEFAULT 0, `CornerstoneRotation3` FLOAT NOT NULL DEFAULT 0,  
  `TeleportPosition1` FLOAT NOT NULL DEFAULT 0, `TeleportPosition2` FLOAT NOT NULL DEFAULT 0, `TeleportPosition3` FLOAT NOT NULL DEFAULT 0,  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0, `NeighborhoodMapID` INT UNSIGNED NOT NULL DEFAULT 0,  
  `Field_010` INT NOT NULL DEFAULT 0, `CornerstoneGameObjectID` INT UNSIGNED NOT NULL DEFAULT 0,  
  `PlotIndex` INT NOT NULL DEFAULT 0, `WorldState` INT NOT NULL DEFAULT 0,  
  `PlotGameObjectID` INT UNSIGNED NOT NULL DEFAULT 0,  
  `Field_014` FLOAT NOT NULL DEFAULT 0, `Field_016` INT NOT NULL DEFAULT 0,  
  `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
-- ============================================================================  
-- LOCALE TABLES (one per table that has a PREPARE_LOCALE_STMT)  
-- ============================================================================  
  
DROP TABLE IF EXISTS `house_decor_locale`;  
CREATE TABLE `house_decor_locale` (  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0, `locale` VARCHAR(4) NOT NULL,  
  `Name_lang` TEXT, `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`locale`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `house_decor_theme_set_locale`;  
CREATE TABLE `house_decor_theme_set_locale` (  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0, `locale` VARCHAR(4) NOT NULL,  
  `Name_lang` TEXT, `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`locale`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `house_exterior_wmo_data_locale`;  
CREATE TABLE `house_exterior_wmo_data_locale` (  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0, `locale` VARCHAR(4) NOT NULL,  
  `Name_lang` TEXT, `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`locale`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `house_level_reward_info_locale`;  
CREATE TABLE `house_level_reward_info_locale` (  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0, `locale` VARCHAR(4) NOT NULL,  
  `Name_lang` TEXT, `Description_lang` TEXT, `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`locale`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `house_room_locale`;  
CREATE TABLE `house_room_locale` (  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0, `locale` VARCHAR(4) NOT NULL,  
  `Name_lang` TEXT, `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`locale`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `house_theme_locale`;  
CREATE TABLE `house_theme_locale` (  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0, `locale` VARCHAR(4) NOT NULL,  
  `Name_lang` TEXT, `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`locale`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `neighborhood_initiative_locale`;  
CREATE TABLE `neighborhood_initiative_locale` (  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0, `locale` VARCHAR(4) NOT NULL,  
  `Name_lang` TEXT, `Description_lang` TEXT, `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`locale`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `neighborhood_name_gen_locale`;  
CREATE TABLE `neighborhood_name_gen_locale` (  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0, `locale` VARCHAR(4) NOT NULL,  
  `Prefix_lang` TEXT, `Middle_lang` TEXT, `Suffix_lang` TEXT, `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`locale`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;  
  
DROP TABLE IF EXISTS `neighborhood_plot_locale`;  
CREATE TABLE `neighborhood_plot_locale` (  
  `ID` INT UNSIGNED NOT NULL DEFAULT 0, `locale` VARCHAR(4) NOT NULL,  
  `Name_lang` TEXT, `VerifiedBuild` INT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`ID`,`locale`,`VerifiedBuild`)) ENGINE=INNODB DEFAULT CHARSET=utf8mb4;