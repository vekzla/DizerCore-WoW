-- Add placement_time column to character_housing_decor
-- Tracks when each decoration was placed, used for the refund window (2 hours)
-- Existing rows default to 0 (no refund eligibility for pre-existing placements)

SET @col := (  
  SELECT COUNT(*) FROM information_schema.COLUMNS  
  WHERE TABLE_SCHEMA = DATABASE()  
    AND TABLE_NAME = 'character_housing_decor'  
    AND COLUMN_NAME = 'placementTime'  
);  
SET @stmt := IF(@col = 0,  
  'ALTER TABLE `character_housing_decor` ADD COLUMN `placementTime` BIGINT UNSIGNED NOT NULL DEFAULT 0 COMMENT ''Unix timestamp when decor was placed (for refund window)'' AFTER `locked`',  
  'SELECT 1');  
PREPARE s FROM @stmt;  
EXECUTE s;  
DEALLOCATE PREPARE s;