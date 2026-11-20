#ifndef FCBOTS_DEFINES_H
#define FCBOTS_DEFINES_H

#include <cstdint>

// WoW IDs from DB2 ChrClasses / ChrRaces / CharBaseInfo (16 Nov 2026 exports).
// Stage 1 placeholder: enums only, no logic.

namespace FCBots
{
enum class Class : std::uint8_t
{
    Warrior = 1, Paladin = 2, Hunter = 3, Rogue = 4, Priest = 5,
    DeathKnight = 6, Shaman = 7, Mage = 8, Warlock = 9, Monk = 10,
    Druid = 11, DemonHunter = 12, Evoker = 13
};

enum class Race : std::uint8_t
{
    Human = 1, Orc = 2, Dwarf = 3, NightElf = 4, Undead = 5, Tauren = 6,
    Gnome = 7, Troll = 8, Goblin = 9, BloodElf = 10, Draenei = 11, Worgen = 22,
    Pandaren24 = 24, Pandaren25 = 25, Pandaren26 = 26, Nightborne = 27,
    HighmountainTauren = 28, VoidElf = 29, LightforgedDraenei = 30,
    ZandalariTroll = 31, KulTiran = 32, DarkIronDwarf = 34, Vulpera = 35,
    MagharOrc = 36, Mechagnome = 37, Dracthyr52 = 52, Dracthyr70 = 70,
    Earthen84 = 84, Earthen85 = 85, Haranir86 = 86, Haranir91 = 91
};
// Values that must match sql/base/fcbots_database.sql.
// Bot creature entries (world.creature_template.entry) live in this block only.
constexpr std::uint32_t BotEntryMin = 10000000u;
constexpr std::uint32_t BotEntryMax = 19999999u;

// bot_templates.gender (tinyint unsigned, default 2).
enum class BotGender : std::uint8_t
{
    Male = 0,
    Female = 1,
    Unknown = 2
};

// Faction. Races that exist on both sides share one folder; the faction is kept here.
// Alliance = 0, Horde = 1, Neutral = 2 (ChrRaces Alliance column).
enum class Faction : std::uint8_t
{
    Alliance = 0,
    Horde = 1,
    Neutral = 2
};
}

#endif // FCBOTS_DEFINES_H
