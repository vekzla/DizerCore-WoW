START TRANSACTION;

INSERT INTO followship.bot_templates (entry, bot_class, bot_race)
SELECT v.entry, v.bot_class, v.bot_race
FROM (
    SELECT 29195 AS entry, 11 AS bot_class, 10 AS bot_race   -- Lady Alistra, Death Knight, Undead
    UNION ALL
    SELECT 29196, 11, 1                                      -- Lord Thorval, Death Knight, Human
    UNION ALL
    SELECT 88110, 12, 3                                      -- Izal Whitemoon, Demon Hunter, Night Elf
) v
WHERE NOT EXISTS (
    SELECT 1 FROM followship.bot_templates b WHERE b.entry = v.entry
);

SELECT bt.entry, bt.bot_class, bt.bot_race, ct.name
FROM followship.bot_templates bt
JOIN world.creature_template ct ON ct.entry = bt.entry
WHERE bt.entry IN (29195, 29196, 88110);

COMMIT;