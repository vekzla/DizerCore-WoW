-- Per-surface theme columns for character_housing_rooms.  
--  
-- Previously a single themeId column was shared by walls/floors/ceilings,  
-- so dyeing the ceiling's style overwrote the wall's style. Split into  
-- three independent theme IDs. New rows default to 0 and the load path  
-- seeds them from the legacy themeId so existing houses keep their look.  
-- Idempotent: each column is only added if it does not already exist.  
  
-- wallThemeId  
SET @col_exists := (SELECT COUNT(*) FROM information_schema.COLUMNS  
    WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'character_housing_rooms' AND COLUMN_NAME = 'wallThemeId');  
SET @ddl := IF(@col_exists = 0,  
    'ALTER TABLE `character_housing_rooms` ADD COLUMN `wallThemeId` INT UNSIGNED NOT NULL DEFAULT 0 AFTER `ceilingSlot`',  
    'DO 0');  
PREPARE stmt FROM @ddl; EXECUTE stmt; DEALLOCATE PREPARE stmt;  
  
-- floorThemeId  
SET @col_exists := (SELECT COUNT(*) FROM information_schema.COLUMNS  
    WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'character_housing_rooms' AND COLUMN_NAME = 'floorThemeId');  
SET @ddl := IF(@col_exists = 0,  
    'ALTER TABLE `character_housing_rooms` ADD COLUMN `floorThemeId` INT UNSIGNED NOT NULL DEFAULT 0 AFTER `wallThemeId`',  
    'DO 0');  
PREPARE stmt FROM @ddl; EXECUTE stmt; DEALLOCATE PREPARE stmt;  
  
-- ceilingThemeId  
SET @col_exists := (SELECT COUNT(*) FROM information_schema.COLUMNS  
    WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'character_housing_rooms' AND COLUMN_NAME = 'ceilingThemeId');  
SET @ddl := IF(@col_exists = 0,  
    'ALTER TABLE `character_housing_rooms` ADD COLUMN `ceilingThemeId` INT UNSIGNED NOT NULL DEFAULT 0 AFTER `floorThemeId`',  
    'DO 0');  
PREPARE stmt FROM @ddl; EXECUTE stmt; DEALLOCATE PREPARE stmt;