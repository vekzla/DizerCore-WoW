#ifndef FCBOTS_WARRIOR_SPELLS_H_H
#define FCBOTS_WARRIOR_SPELLS_H_H

#include <cstdint>

// Warrior spell IDs and cooldowns, Midnight 12.1.5 (DB2 SpellName.csv, checked by name).
// Cooldowns in ms (SpellCooldowns.csv, DifficultyID 0). UNVERIFIED marks a cooldown that
// does not match the spell's known cooldown. No racials here (see Race/).
namespace FCBots::Class::Warrior
{
// Shared core
constexpr std::uint32_t SPELL_CHARGE = 100u;
constexpr std::uint32_t SPELL_EXECUTE = 163201u;        // Arms/Prot Execute
constexpr std::uint32_t SPELL_EXECUTE_FURY = 5308u;     // Fury Execute
constexpr std::uint32_t SPELL_BERSERKER_RAGE = 18499u;
constexpr std::uint32_t SPELL_PUMMEL = 6552u;
constexpr std::uint32_t SPELL_RALLYING_CRY = 97462u;
constexpr std::uint32_t SPELL_HEROIC_LEAP = 6544u;
constexpr std::uint32_t SPELL_INTERCEPT = 198304u;
constexpr std::uint32_t SPELL_BATTLE_SHOUT = 6673u;
constexpr std::uint32_t SPELL_SPELL_REFLECTION = 23920u;
constexpr std::uint32_t SPELL_SHOCKWAVE = 46968u;
constexpr std::uint32_t SPELL_STORM_BOLT = 107570u;
constexpr std::uint32_t SPELL_IMPENDING_VICTORY = 202168u;
constexpr std::uint32_t SPELL_ENRAGE_AURA = 184361u;    // Fury aura

// Arms
constexpr std::uint32_t SPELL_MORTAL_STRIKE = 12294u;
constexpr std::uint32_t SPELL_COLOSSUS_SMASH = 167105u;
constexpr std::uint32_t SPELL_AVATAR = 107574u;
constexpr std::uint32_t SPELL_BLADESTORM = 227847u;
constexpr std::uint32_t SPELL_CLEAVE = 845u;
constexpr std::uint32_t SPELL_OVERPOWER = 7384u;
constexpr std::uint32_t SPELL_REND_ARMS = 772u;
constexpr std::uint32_t SPELL_HEROIC_STRIKE = 1269383u;
constexpr std::uint32_t SPELL_SLAM = 1464u;
constexpr std::uint32_t SPELL_SWEEPING_STRIKES = 260708u;
constexpr std::uint32_t SPELL_RAVAGER = 228920u;
constexpr std::uint32_t SPELL_DEMOLISH = 436358u;
constexpr std::uint32_t SPELL_SUDDEN_DEATH = 280776u;   // proc
constexpr std::uint32_t SPELL_IMMINENT_DEMISE = 444769u; // Slayer talent

// Fury
constexpr std::uint32_t SPELL_RAMPAGE = 184367u;
constexpr std::uint32_t SPELL_BLOODTHIRST = 23881u;
constexpr std::uint32_t SPELL_RAGING_BLOW = 85288u;
constexpr std::uint32_t SPELL_WHIRLWIND = 190411u;
constexpr std::uint32_t SPELL_CRUSHING_BLOW = 335097u;
constexpr std::uint32_t SPELL_ODYNS_FURY = 385059u;
constexpr std::uint32_t SPELL_BLOODBATH = 335096u;
constexpr std::uint32_t SPELL_RECKLESSNESS = 1719u;
constexpr std::uint32_t SPELL_THUNDER_BLAST = 435607u;
constexpr std::uint32_t SPELL_HACK_AND_SLASH = 383877u; // proc

// Protection
constexpr std::uint32_t SPELL_SHIELD_SLAM = 23922u;
constexpr std::uint32_t SPELL_THUNDER_CLAP = 6343u;
constexpr std::uint32_t SPELL_REVENGE = 6572u;
constexpr std::uint32_t SPELL_IGNORE_PAIN = 190456u;
constexpr std::uint32_t SPELL_REND_PROT = 394062u;
constexpr std::uint32_t SPELL_SHIELD_BLOCK = 2565u;
constexpr std::uint32_t SPELL_DEMORALIZING_SHOUT = 1160u;

// Cooldowns, ms (SpellCooldowns.csv)
constexpr std::uint32_t COOLDOWN_MORTAL_STRIKE_MS = 6000u;
constexpr std::uint32_t COOLDOWN_BLOODTHIRST_MS = 4500u;      // category
constexpr std::uint32_t COOLDOWN_SHIELD_SLAM_MS = 9000u;
constexpr std::uint32_t COOLDOWN_EXECUTE_MS = 6000u;
constexpr std::uint32_t COOLDOWN_BERSERKER_RAGE_MS = 60000u;
constexpr std::uint32_t COOLDOWN_PUMMEL_MS = 15000u;          // category
constexpr std::uint32_t COOLDOWN_RECKLESSNESS_MS = 90000u;
constexpr std::uint32_t COOLDOWN_RALLYING_CRY_MS = 180000u;
constexpr std::uint32_t COOLDOWN_SHIELD_BLOCK_MS = 1000u;
constexpr std::uint32_t COOLDOWN_BLADESTORM_MS = 60000u;
constexpr std::uint32_t COOLDOWN_COLOSSUS_SMASH_MS = 45000u;
// UNVERIFIED: DB2 shows the global cooldown (1500 ms) for Charge, not the known charge cooldown.
constexpr std::uint32_t COOLDOWN_CHARGE_MS = 1500u;
}

#endif // FCBOTS_WARRIOR_SPELLS_H_H
