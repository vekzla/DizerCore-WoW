-- Add scale column to character_housing_decor for Advanced Mode decor scaling  
-- Default 1.0 = original size. Sniff shows values like 0.45 to 1.62.  
-- Idempotent: only adds the column if it does not already exist.  
SET @col_exists := (  
    SELECT COUNT(*) FROM information_schema.COLUMNS  
    WHERE TABLE_SCHEMA = DATABASE()  
      AND TABLE_NAME   = 'character_housing_decor'  
      AND COLUMN_NAME  = 'scale'  
);  
SET @ddl := IF(@col_exists = 0,  
    'ALTER TABLE `character_housing_decor` ADD COLUMN `scale` FLOAT NOT NULL DEFAULT 1.0 AFTER `rotW`',  
    'DO 0');  
PREPARE stmt FROM @ddl;  
EXECUTE stmt;  
DEALLOCATE PREPARE stmt;