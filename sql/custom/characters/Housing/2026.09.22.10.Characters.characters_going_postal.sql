DROP TABLE IF EXISTS `character_going_postal`;  
CREATE TABLE `character_going_postal` (  
  `guid` INT UNSIGNED NOT NULL DEFAULT 0,  
  `routeId` INT UNSIGNED NOT NULL DEFAULT 0,  
  `bestTimeMs` INT UNSIGNED NOT NULL DEFAULT 0,  
  `updateTime` INT UNSIGNED NOT NULL DEFAULT 0,  
  PRIMARY KEY (`guid`,`routeId`)  
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;