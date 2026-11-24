#include "Warrior_Talents.h"

namespace FCBots::Class::Warrior::Talents
{
namespace
{
// The BYPASS_* constants decide whether the prerequisite and level checks are skipped.
// The bypass path applies nodes with Player::ApplyTraitEntry, which does not check either.
// The checked path is not written yet, so when the bypass is off the spend stops here
// instead of silently skipping the checks.
// TODO: write the checked path (prerequisite and level gates) before turning the bypass off.
bool RequirementsBypassed()
{
    return BYPASS_PREREQUISITES && BYPASS_LEVEL_GATES;
}

bool IsHeroPick(TalentPick const& pick)
{
    return pick.SubTree != 0u;
}

// Points already spent by the first `cursor` picks, split into spec and hero pools.
void CountUsed(TalentPick const* build, std::size_t cursor, std::uint32_t& specUsed, std::uint32_t& heroUsed)
{
    specUsed = 0u;
    heroUsed = 0u;
    for (std::size_t i = 0; i < cursor; ++i)
    {
        if (IsHeroPick(build[i]))
            heroUsed += build[i].Rank;
        else
            specUsed += build[i].Rank;
    }
}
}

std::uint32_t BuildTotalPoints(TalentPick const* build, std::size_t count)
{
    std::uint32_t total = 0u;
    for (std::size_t i = 0; i < count; ++i)
        total += build[i].Rank;
    return total;
}

std::uint32_t SpendTalentPoints(TalentPick const* build, std::size_t count,
    std::uint32_t availablePoints, std::size_t& cursor, TraitApplyFn const& apply)
{
    std::uint32_t specUsed = 0u;
    std::uint32_t heroUsed = 0u;
    CountUsed(build, cursor, specUsed, heroUsed);

    std::uint32_t spent = 0u;
    while (cursor < count)
    {
        if (!RequirementsBypassed())
            break;

        TalentPick const& pick = build[cursor];

        // Level gates and prerequisites are bypassed for bots (BYPASS_* constants).
        // Stop at the first pick that cannot be afforded so the build order is kept.
        if (pick.Rank > availablePoints)
            break;

        if (!BYPASS_POINT_CAPS)
        {
            bool const hero = IsHeroPick(pick);
            std::uint32_t const used = hero ? heroUsed : specUsed;
            std::uint32_t const cap = hero ? CAP_HERO_TREE : (CAP_CLASS_TREE + CAP_SPEC_TREE);
            if (used + pick.Rank > cap)
                break;
        }

        apply(static_cast<std::int32_t>(pick.TraitNodeEntryId),
              static_cast<std::int32_t>(pick.Rank), 0, true);

        if (IsHeroPick(pick))
            heroUsed += pick.Rank;
        else
            specUsed += pick.Rank;

        availablePoints -= pick.Rank;
        spent += pick.Rank;
        ++cursor;
    }
    return spent;
}
}
