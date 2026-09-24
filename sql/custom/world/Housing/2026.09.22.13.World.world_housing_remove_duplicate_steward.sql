-- Remove duplicate Lyssabel Dawnpetal (entry 249850) from map 2735.  
-- A second, inert copy (npcflag=0, no script) spawns at the same position  
-- as the scripted steward 233063 and swallows player clicks.  
-- Source is base world data (TDB), not a custom file — delete by entry+map  
-- rather than a fixed guid so it works regardless of the dump's guids.  
DELETE ca FROM creature_addon ca  
  INNER JOIN creature c ON c.guid = ca.guid  
  WHERE c.id = 249850 AND c.map = 2735;  
DELETE FROM creature WHERE id = 249850 AND map = 2735;