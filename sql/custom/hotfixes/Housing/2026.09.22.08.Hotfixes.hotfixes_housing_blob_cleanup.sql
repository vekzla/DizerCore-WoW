-- hotfixes_housing_blob_cleanup.sql  
-- One-time purge: housing DB2 stores have typed hotfixes tables, so their  
-- records in hotfix_blob are dead data (skipped at load). Remove them.  
DELETE FROM `hotfix_blob`  
WHERE `TableHash` IN (0x137145D0, 0x24EE8F09, 0x3CC17DDB, 0x485E51AC, 0x5373CFEB,  
                      0x6FF1C5BA, 0xA16AF1A0, 0xBBA2E8F8, 0xD94B42D9, 0xD99D1A0B, 0xE5F16D06);