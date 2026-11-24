#ifndef FCBOTS_WARRIOR_ENGINE_H_H
#define FCBOTS_WARRIOR_ENGINE_H_H

#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>

#include "Warrior_Combat.h"
#include "Warrior_Talents.h"

// Warrior bot decision engine. Pure logic: no TrinityCore types here, so it compiles and
// tests on its own. The TrinityCore adapter (Warrior_Bot.cpp) fills BotView from the bot
// and performs the result.
namespace FCBots::Class::Warrior::Engine
{
// Spec and hero choice the bot was assigned (bot_templates.spec_id / hero_tree_id).
struct BotSetup
{
    Talents::Spec Spec;
    Talents::HeroTree Hero;
};

// Everything the engine reads from the live bot, filled once per tick by the adapter.
struct BotView
{
    std::uint32_t TalentPointsAtLevel = 0u;     // sDB2Manager.GetNumTalentsAtLevel(level, class)
    Combat::CombatState Combat;                 // rage, health, aura and stack state
    Combat::SpellCastableFn CanCast;            // known, off cooldown, in range, resources ok
    Talents::TraitApplyFn ApplyTrait;           // Player::ApplyTraitEntry
};

// Per-bot progress through its build. Keep one per bot; the engine updates it.
struct BotProgress
{
    std::size_t TalentCursor = 0u;
};

struct TickResult
{
    std::uint32_t TalentPointsSpent = 0u;
    std::uint32_t SpellToCast = 0u;             // 0 = nothing to cast this tick
};

// Returns the build for a setup, or nullptr if no build is recorded for it.
// Colossus Arms is not recorded yet (see the cant-be-used notes in Warrior_Talents.h).
Talents::TalentPick const* FindBuild(BotSetup const& setup, std::size_t& count);

// Points still to spend: the level cap minus points already spent by the build cursor.
std::uint32_t AvailablePoints(BotSetup const& setup, BotView const& view, BotProgress const& progress);

// One decision step: spend available talent points in build order, then choose the spell.
TickResult Tick(BotSetup const& setup, BotView const& view, BotProgress& progress);
}

#endif // FCBOTS_WARRIOR_ENGINE_H_H
