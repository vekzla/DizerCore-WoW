#include "Warrior_Engine.h"

namespace FCBots::Class::Warrior::Engine
{
namespace
{
Talents::TalentPick const* BuildFor(BotSetup const& setup, std::size_t& count)
{
    using Talents::HeroTree;
    using Talents::Spec;
    count = 0u;
    switch (setup.Spec)
    {
        case Spec::Arms:
            if (setup.Hero == HeroTree::Slayer)
            {
                count = std::size(Talents::ARMS_SLAYER_BUILD);
                return Talents::ARMS_SLAYER_BUILD;
            }
            return nullptr; // Colossus not recorded yet
        case Spec::Fury:
            if (setup.Hero == HeroTree::Slayer)
            {
                count = std::size(Talents::FURY_SLAYER_BUILD);
                return Talents::FURY_SLAYER_BUILD;
            }
            if (setup.Hero == HeroTree::MountainThane)
            {
                count = std::size(Talents::FURY_MOUNTAIN_THANE_BUILD);
                return Talents::FURY_MOUNTAIN_THANE_BUILD;
            }
            return nullptr;
        case Spec::Protection:
            if (setup.Hero == HeroTree::MountainThane)
            {
                count = std::size(Talents::PROT_MOUNTAIN_THANE_BUILD);
                return Talents::PROT_MOUNTAIN_THANE_BUILD;
            }
            if (setup.Hero == HeroTree::Colossus)
            {
                count = std::size(Talents::PROT_COLOSSUS_BUILD);
                return Talents::PROT_COLOSSUS_BUILD;
            }
            return nullptr;
    }
    return nullptr;
}

std::uint32_t PickSpell(Talents::Spec spec, BotView const& view)
{
    switch (spec)
    {
        case Talents::Spec::Arms:       return Combat::PickArmsSpell(view.Combat, view.CanCast);
        case Talents::Spec::Fury:       return Combat::PickFurySpell(view.Combat, view.CanCast);
        case Talents::Spec::Protection: return Combat::PickProtSpell(view.Combat, view.CanCast);
    }
    return 0u;
}
}

Talents::TalentPick const* FindBuild(BotSetup const& setup, std::size_t& count)
{
    return BuildFor(setup, count);
}

std::uint32_t AvailablePoints(BotSetup const& setup, BotView const& view, BotProgress const& progress)
{
    std::size_t count = 0u;
    Talents::TalentPick const* build = BuildFor(setup, count);
    if (!build)
        return 0u;
    std::uint32_t spent = Talents::BuildTotalPoints(build, progress.TalentCursor);
    return view.TalentPointsAtLevel > spent ? view.TalentPointsAtLevel - spent : 0u;
}

TickResult Tick(BotSetup const& setup, BotView const& view, BotProgress& progress)
{
    TickResult result;

    std::size_t count = 0u;
    Talents::TalentPick const* build = BuildFor(setup, count);
    if (build && view.ApplyTrait)
    {
        result.TalentPointsSpent = Talents::SpendTalentPoints(build, count,
            AvailablePoints(setup, view, progress), progress.TalentCursor, view.ApplyTrait);
    }

    if (view.CanCast)
        result.SpellToCast = PickSpell(setup.Spec, view);

    return result;
}
}
