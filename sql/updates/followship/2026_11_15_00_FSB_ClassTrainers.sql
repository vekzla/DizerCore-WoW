-- Add Death Knight and Demon Hunter bot templates  
DELETE FROM `bot_templates` WHERE `entry` IN (29195, 29196, 88110);  
INSERT INTO `bot_templates` (`entry`, `bot_class`, `bot_race`) VALUES  
(29195, 11, 10), -- Lady Alistra, Death Knight, Undead  
(29196, 11, 1),  -- Lord Thorval, Death Knight, Human  
(88110, 12, 3);  -- Izal Whitemoon, Demon Hunter, Night Elf  