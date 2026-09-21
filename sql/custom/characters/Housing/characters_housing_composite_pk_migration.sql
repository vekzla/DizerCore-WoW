-- ============================================================================  
-- Migration: Change PRIMARY KEY from (id) to (ownerGuid, id)  
-- ============================================================================  
-- Idempotent: only rebuilds the PK when it is not already (ownerGuid, id).  
-- Safe to re-run (auto-updater re-applies the whole file on hash change).  
-- ============================================================================  
  
-- 1. character_housing_decor -----------------------------------------------  
-- How many PK columns does the table currently have?  
SET @pk_cols := (SELECT COUNT(*) FROM information_schema.STATISTICS  
    WHERE TABLE_SCHEMA = DATABASE()  
      AND TABLE_NAME   = 'character_housing_decor'  
      AND INDEX_NAME   = 'PRIMARY');  
  
-- Is the composite PK (ownerGuid, id) already in place?  
SET @pk_ok := (SELECT COUNT(*) FROM information_schema.STATISTICS  
    WHERE TABLE_SCHEMA = DATABASE()  
      AND TABLE_NAME   = 'character_housing_decor'  
      AND INDEX_NAME   = 'PRIMARY'  
      AND COLUMN_NAME IN ('ownerGuid', 'id'));  
  
-- Rebuild only if it is not already the 2-column (ownerGuid, id) PK.  
SET @ddl := IF(@pk_cols = 2 AND @pk_ok = 2,  
    'DO 0',  
    CONCAT(  
        'ALTER TABLE `character_housing_decor` ',  
        'MODIFY COLUMN `id` BIGINT UNSIGNED NOT NULL ',  
        'COMMENT ''Decor instance ID (unique per owner)'', ',  
        IF(@pk_cols > 0, 'DROP PRIMARY KEY, ', ''),  
        'ADD PRIMARY KEY (`ownerGuid`, `id`)'));  
PREPARE stmt FROM @ddl; EXECUTE stmt; DEALLOCATE PREPARE stmt;  
  
-- 2. character_housing_rooms -----------------------------------------------  
SET @pk_cols := (SELECT COUNT(*) FROM information_schema.STATISTICS  
    WHERE TABLE_SCHEMA = DATABASE()  
      AND TABLE_NAME   = 'character_housing_rooms'  
      AND INDEX_NAME   = 'PRIMARY');  
  
SET @pk_ok := (SELECT COUNT(*) FROM information_schema.STATISTICS  
    WHERE TABLE_SCHEMA = DATABASE()  
      AND TABLE_NAME   = 'character_housing_rooms'  
      AND INDEX_NAME   = 'PRIMARY'  
      AND COLUMN_NAME IN ('ownerGuid', 'id'));  
  
SET @ddl := IF(@pk_cols = 2 AND @pk_ok = 2,  
    'DO 0',  
    CONCAT(  
        'ALTER TABLE `character_housing_rooms` ',  
        'MODIFY COLUMN `id` BIGINT UNSIGNED NOT NULL ',  
        'COMMENT ''Room instance ID (unique per owner)'', ',  
        IF(@pk_cols > 0, 'DROP PRIMARY KEY, ', ''),  
        'ADD PRIMARY KEY (`ownerGuid`, `id`)'));  
PREPARE stmt FROM @ddl; EXECUTE stmt; DEALLOCATE PREPARE stmt;  
  
SELECT 'Schema migration complete: composite primary keys applied.' AS status;