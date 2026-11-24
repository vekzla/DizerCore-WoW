#ifndef FCBOTS_WARRIOR_COMBAT_H_H
#define FCBOTS_WARRIOR_COMBAT_H_H

#include <array>
#include <cstdint>
#include <functional>

#include "Warrior_Spells.h"

// Warrior rotation, Midnight 12.1.5. Priority lists are read top to bottom each GCD.
// The bot casts the first entry that is usable. Each entry's condition is in the comment.
// Mechanics are constants so the engine checks them the same way for every spec.
// Decision logic (range, rage, cooldown checks) is added with the engine.
namespace FCBots::Class::Warrior::Combat
{
// Mechanics (from the Wowhead and wow.gg guides, Midnight)
constexpr std::uint32_t RAGE_RAMPAGE_THRESHOLD = 100u;          // Fury: cast Rampage over this
constexpr std::uint32_t ARMS_REND_REFRESH_MS = 10000u;          // Arms: refresh Rend under 10s
constexpr std::uint32_t ARMS_REND_REFRESH_LATE_MS = 5000u;      // Arms: refresh Rend under 5s
constexpr std::uint32_t ARMS_BLADESTORM_MAX_IMMINENT_DEMISE = 3u; // Arms: Execute before Bladestorm under 3 stacks
constexpr std::uint32_t PROT_EXECUTE_HEALTH_PCT = 20u;          // Prot: Execute at or below 20%
constexpr std::uint32_t PROT_EXECUTE_HEALTH_PCT_TALENT = 35u;   // Prot: with the Execute talent

// Arms, single target. Source: wow.gg Midnight Season 2 Arms guide (fetched 2026-11-24).
constexpr std::array<std::uint32_t, 12> ARMS_SINGLE_TARGET =
{
    SPELL_REND_ARMS,     // Rend: when missing, or under 10s with Colossus Smash ready
    SPELL_AVATAR,        // Avatar: on cooldown
    SPELL_COLOSSUS_SMASH,// Colossus Smash: on cooldown
    SPELL_EXECUTE,       // Execute: with 2 Sudden Death stacks, or before a ready Bladestorm under 3 Imminent Demise
    SPELL_BLADESTORM,    // Bladestorm: during Colossus Smash
    SPELL_HEROIC_STRIKE, // Heroic Strike
    SPELL_OVERPOWER,     // Overpower: with 2 Opportunist stacks
    SPELL_MORTAL_STRIKE, // Mortal Strike
    SPELL_EXECUTE,       // Execute: during Sudden Death, or below 35% target health to spend Rage
    SPELL_OVERPOWER,     // Overpower
    SPELL_REND_ARMS,     // Rend: under 5s remaining
    SPELL_SLAM,          // Slam
};

// Fury, single target. Source: Wowhead Midnight Fury rotation (pasted text, 2026-11-24).
// Two lists: the cooldown list is used while Recklessness is active on the bot, the outside
// list otherwise. Each list is read top to bottom; the first usable entry is cast.
constexpr std::array<std::uint32_t, 8> FURY_COOLDOWN_SINGLE_TARGET =
{
    SPELL_RAMPAGE,       // Rampage: over 100 Rage
    // TODO: Rampage immediately before Bladestorm (Wowhead step 2). Needs Bladestorm timing.
    SPELL_BLADESTORM,    // Bladestorm
    SPELL_EXECUTE_FURY,  // Execute: with Sudden Death
    SPELL_RAMPAGE,       // Rampage: if Hack and Slash is not active
    SPELL_CRUSHING_BLOW, // Crushing Blow
    SPELL_ODYNS_FURY,    // Odyn's Fury
    SPELL_EXECUTE_FURY,  // Execute
    SPELL_BLOODBATH,     // Bloodbath
};

constexpr std::array<std::uint32_t, 8> FURY_OUTSIDE_COOLDOWN_SINGLE_TARGET =
{
    SPELL_RECKLESSNESS,  // Recklessness
    SPELL_RAMPAGE,       // Rampage: over 100 Rage, or Enrage about to expire
    SPELL_ODYNS_FURY,    // Odyn's Fury
    SPELL_EXECUTE_FURY,  // Execute
    SPELL_RAGING_BLOW,   // Raging Blow
    SPELL_RAMPAGE,       // Rampage
    SPELL_BLOODTHIRST,   // Bloodthirst
    SPELL_WHIRLWIND,     // Whirlwind: when nothing else is available
};

// Protection, Mountain Thane, single target.
constexpr std::array<std::uint32_t, 7> PROT_SINGLE_TARGET =
{
    SPELL_CHARGE,        // Charge: always, to open combat
    SPELL_SHIELD_SLAM,   // Shield Slam: on cooldown
    SPELL_THUNDER_CLAP,  // Thunder Clap: on cooldown, and to apply Rend
    SPELL_REVENGE,       // Revenge
    SPELL_IGNORE_PAIN,   // Ignore Pain: spend excess Rage
    SPELL_EXECUTE,       // Execute: target at or below 20% health (35% with the talent)
    SPELL_IMPENDING_VICTORY, // Impending Victory: when health is low
};

// Inputs the engine fills in once per GCD. Times are in milliseconds.
struct CombatState
{
    std::uint32_t Rage = 0u;
    std::uint32_t TargetHealthPct = 100u;
    std::uint32_t PlayerHealthPct = 100u;
    std::uint32_t RendRemainingMs = 0u;
    std::uint32_t ImminentDemiseStacks = 0u;
    bool SuddenDeathActive = false;
    bool ColossusSmashActive = false;
    bool HackAndSlashActive = false;
    bool HasProtExecuteTalent = false;
    bool CooldownsActive = false;               // Recklessness active on the bot (Fury)
    std::uint32_t SuddenDeathStacks = 0u;
    std::uint32_t OpportunistStacks = 0u;
};

// Engine callback: true when the spell is known, off cooldown, in range and affordable.
using SpellCastableFn = std::function<bool(std::uint32_t spellId)>;

// Each returns the first spell in its list whose condition holds and which can be cast.
// Returns 0 when nothing in the list is usable.
std::uint32_t PickArmsSpell(CombatState const& state, SpellCastableFn const& canCast);
std::uint32_t PickFurySpell(CombatState const& state, SpellCastableFn const& canCast);
std::uint32_t PickProtSpell(CombatState const& state, SpellCastableFn const& canCast);

}

#endif // FCBOTS_WARRIOR_COMBAT_H_H
