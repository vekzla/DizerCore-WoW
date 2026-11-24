#include "Warrior_Combat.h"

namespace FCBots::Class::Warrior::Combat
{
namespace
{
// Arms list indices, in the order of ARMS_SINGLE_TARGET.
bool ArmsConditionMet(std::size_t index, CombatState const& s, SpellCastableFn const& canCast)
{
    switch (index)
    {
        case 0: // Rend: missing (0 remaining) or under 10s with Colossus Smash ready
            return s.RendRemainingMs == 0u
                || (s.RendRemainingMs < ARMS_REND_REFRESH_MS && canCast(SPELL_COLOSSUS_SMASH));
        case 3: // Execute: 2 Sudden Death stacks, or before Bladestorm under 3 Imminent Demise
            return s.SuddenDeathStacks >= 2u
                || (s.ImminentDemiseStacks < ARMS_BLADESTORM_MAX_IMMINENT_DEMISE && canCast(SPELL_BLADESTORM));
        case 4: // Bladestorm: during Colossus Smash
            return s.ColossusSmashActive;
        case 6: // Overpower: 2 Opportunist stacks
            return s.OpportunistStacks >= 2u;
        case 8: // Execute: during Sudden Death, or below 35% target health
            return s.SuddenDeathStacks >= 1u || s.TargetHealthPct < 35u;
        case 10: // Rend: under 5s remaining
            return s.RendRemainingMs < ARMS_REND_REFRESH_LATE_MS;
        default:
            return true;
    }
}

// Fury cooldown list indices, in the order of FURY_COOLDOWN_SINGLE_TARGET.
bool FuryCooldownConditionMet(std::size_t index, CombatState const& s)
{
    switch (index)
    {
        case 0: // Rampage: over 100 Rage
            return s.Rage > RAGE_RAMPAGE_THRESHOLD;
        case 2: // Execute: with Sudden Death
            return s.SuddenDeathActive;
        case 3: // Rampage: if Hack and Slash is not active
            return !s.HackAndSlashActive;
        default:
            return true;
    }
}

// Fury outside-cooldown list indices, in the order of FURY_OUTSIDE_COOLDOWN_SINGLE_TARGET.
// TODO: Rampage also fires when Enrage is about to expire. Enrage timing is not tracked yet.
bool FuryOutsideConditionMet(std::size_t index, CombatState const& s)
{
    switch (index)
    {
        case 1: // Rampage: over 100 Rage
            return s.Rage > RAGE_RAMPAGE_THRESHOLD;
        default:
            return true;
    }
}

// Protection list indices, in the order of PROT_SINGLE_TARGET.
bool ProtConditionMet(std::size_t index, CombatState const& s)
{
    switch (index)
    {
        case 5: // Execute: target at or below 20% (35% with the talent)
        {
            std::uint32_t const threshold = s.HasProtExecuteTalent ? PROT_EXECUTE_HEALTH_PCT_TALENT : PROT_EXECUTE_HEALTH_PCT;
            return s.TargetHealthPct <= threshold;
        }
        // TODO: Ignore Pain (spend excess Rage) and Impending Victory (low health) need thresholds
        // that are not in the guide text. They are always eligible until those are set.
        default:
            return true;
    }
}

template <std::size_t N, typename Cond>
std::uint32_t PickFirst(std::array<std::uint32_t, N> const& list, Cond&& cond, SpellCastableFn const& canCast)
{
    for (std::size_t i = 0; i < N; ++i)
    {
        if (cond(i, list[i]) && canCast(list[i]))
            return list[i];
    }
    return 0u;
}
}

std::uint32_t PickArmsSpell(CombatState const& state, SpellCastableFn const& canCast)
{
    return PickFirst(ARMS_SINGLE_TARGET,
        [&](std::size_t i, std::uint32_t) { return ArmsConditionMet(i, state, canCast); }, canCast);
}

std::uint32_t PickFurySpell(CombatState const& state, SpellCastableFn const& canCast)
{
    if (state.CooldownsActive)
    {
        return PickFirst(FURY_COOLDOWN_SINGLE_TARGET,
            [&](std::size_t i, std::uint32_t) { return FuryCooldownConditionMet(i, state); }, canCast);
    }
    return PickFirst(FURY_OUTSIDE_COOLDOWN_SINGLE_TARGET,
        [&](std::size_t i, std::uint32_t) { return FuryOutsideConditionMet(i, state); }, canCast);
}

std::uint32_t PickProtSpell(CombatState const& state, SpellCastableFn const& canCast)
{
    return PickFirst(PROT_SINGLE_TARGET,
        [&](std::size_t i, std::uint32_t) { return ProtConditionMet(i, state); }, canCast);
}
}
