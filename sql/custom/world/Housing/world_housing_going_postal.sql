DROP TABLE IF EXISTS `going_postal_route`;  
CREATE TABLE `going_postal_route` (  
  `id` INT UNSIGNED NOT NULL DEFAULT 0,  
  `team` TINYINT UNSIGNED NOT NULL DEFAULT 0,  
  `routeIndex` TINYINT UNSIGNED NOT NULL DEFAULT 0,  
  `currencyId` INT UNSIGNED NOT NULL DEFAULT 0,  
  `enabled` TINYINT UNSIGNED NOT NULL DEFAULT 0,  
  `name` TEXT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci,  
  PRIMARY KEY (`id`)  
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;  
  
INSERT INTO `going_postal_route` (`id`,`team`,`routeIndex`,`currencyId`,`enabled`,`name`) VALUES  
(1, 0, 1, 3431, 1, 'Alliance Route 1'),  
(2, 0, 2, 3432, 1, 'Alliance Route 2'),  
(3, 0, 3, 3433, 1, 'Alliance Route 3'),  
(4, 1, 1, 3434, 1, 'Horde Route 1'),  
(5, 1, 2, 3435, 1, 'Horde Route 2'),  
(6, 1, 3, 3436, 1, 'Horde Route 3');  
  
DROP TABLE IF EXISTS `going_postal_route_checkpoint`;  
CREATE TABLE `going_postal_route_checkpoint` (  
  `routeId` INT UNSIGNED NOT NULL DEFAULT 0,  
  `seq` INT UNSIGNED NOT NULL DEFAULT 0,  
  `mapId` INT UNSIGNED NOT NULL DEFAULT 0,  
  `posX` FLOAT NOT NULL DEFAULT 0,  
  `posY` FLOAT NOT NULL DEFAULT 0,  
  `posZ` FLOAT NOT NULL DEFAULT 0,  
  PRIMARY KEY (`routeId`,`seq`)  
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;  
-- Intentionally empty: checkpoint coords are CAPTURE-BLOCKED.