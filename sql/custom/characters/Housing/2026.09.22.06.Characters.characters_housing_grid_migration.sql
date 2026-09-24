-- Housing: Add 2D grid coordinates and floor index to room placement  
-- GridX/GridY store yard offsets from the interior origin.  
-- FloorIndex: 0=ground, 1+=upper floors (for stairwell rooms).  
-- Idempotent: each column is only added if it does not already exist.  
  
-- gridX  
SET @col_exists := (SELECT COUNT(*) FROM information_schema.COLUMNS  
    WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'character_housing_rooms' AND COLUMN_NAME = 'gridX');  
SET @ddl := IF(@col_exists = 0,  
    'ALTER TABLE `character_housing_rooms` ADD COLUMN `gridX` INT NOT NULL DEFAULT 0 AFTER `slotIndex`',  
    'DO 0');  
PREPARE stmt FROM @ddl; EXECUTE stmt; DEALLOCATE PREPARE stmt;  
  
-- gridY  
SET @col_exists := (SELECT COUNT(*) FROM information_schema.COLUMNS  
    WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'character_housing_rooms' AND COLUMN_NAME = 'gridY');  
SET @ddl := IF(@col_exists = 0,  
    'ALTER TABLE `character_housing_rooms` ADD COLUMN `gridY` INT NOT NULL DEFAULT 0 AFTER `gridX`',  
    'DO 0');  
PREPARE stmt FROM @ddl; EXECUTE stmt; DEALLOCATE PREPARE stmt;  
  
-- floorIndex  
SET @col_exists := (SELECT COUNT(*) FROM information_schema.COLUMNS  
    WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'character_housing_rooms' AND COLUMN_NAME = 'floorIndex');  
SET @ddl := IF(@col_exists = 0,  
    'ALTER TABLE `character_housing_rooms` ADD COLUMN `floorIndex` INT NOT NULL DEFAULT 0 AFTER `gridY`',  
    'DO 0');  
PREPARE stmt FROM @ddl; EXECUTE stmt; DEALLOCATE PREPARE stmt;  
  
-- Migrate existing rooms: slotIndex -> yard offset (slot * 15), floor 0  
-- Guarded: only touches rows still at default zeros, so a re-apply  
-- cannot reset room positions players already placed.  
UPDATE `character_housing_rooms`  
   SET `gridX` = CAST(`slotIndex` AS SIGNED) * 15, `gridY` = 0, `floorIndex` = 0  
 WHERE `slotIndex` <> 0  
   AND `gridX` = 0 AND `gridY` = 0 AND `floorIndex` = 0;