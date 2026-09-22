-- ============================================================================  
-- Housing Decor Duel Template  
-- ============================================================================  
-- DecorDuelMgr::Initialize() queries `decor_duel_template` on the WORLD DB:  
--   SELECT mapId, enabled FROM decor_duel_template WHERE id = 1  
-- TrinityCore treats a missing table (ER_NO_SUCH_TABLE / 1146) as fatal, so  
-- the table MUST exist or world load aborts at MySQLConnection.cpp:600.  
-- The seed row leaves the manager DISABLED (enabled=0, mapId=0) until a  
-- Decor Duel capture confirms the minigame map id.  
-- ============================================================================  
  
DROP TABLE IF EXISTS `decor_duel_template`;  
CREATE TABLE `decor_duel_template` (  
  `id` INT UNSIGNED NOT NULL DEFAULT 0,  
  `mapId` INT UNSIGNED NOT NULL DEFAULT 0,  
  `enabled` TINYINT UNSIGNED NOT NULL DEFAULT 0,  
  PRIMARY KEY (`id`)  
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;  
  
INSERT INTO `decor_duel_template` (`id`, `mapId`, `enabled`) VALUES (1, 0, 0);