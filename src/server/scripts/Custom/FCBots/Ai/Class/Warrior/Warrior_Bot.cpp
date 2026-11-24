#include "Warrior_Bot.h"

#include "DB2Stores.h"
#include "DBCEnums.h"
#include "Player.h"
#include "SpellAuras.h"
#include "SpellHistory.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "Unit.h"
#include "Warrior_Spells.h"
#include "Warrior_Talents.h"

namespace FCBots::Class::Warrior::Bot
{
namespace
{
// Massacre (281001) is the Protection talent that moves Execute to 35% health.
constexpr std::uint32_t SPELL_MASSACRE_TALENT = 281001u;

// Known, off cooldown, and in range of the target.
bool CanCastOn(Player* bot, Unit* target, std::uint32_t spellId)
{
    if (!bot->HasSpell(spellId))
        return false;

    SpellInfo const* info = sSpellMgr->GetSpellInfo(spellId, DIFFICULTY_NONE);
    if (!info)
        return false;

    if (!bot->GetSpellHistory()->IsReady(info))
        return false;

    if (target)
    {
        float const maxRange = info->GetMaxRange(true, bot);
        bool const inRange = (maxRange == 0.0f) ? bot->IsWithinMeleeRange(target)
                                                : bot->IsWithinCombatRange(target, maxRange);
        if (!inRange)
            return false;
    }
    return true;
}

Engine::BotView ReadView(Player* bot, Unit* target)
{
    Engine::BotView view;

    // Talent points at this level, from the server's own table.
    view.TalentPointsAtLevel = static_cast<std::uint32_t>(
        sDB2Manager.GetNumTalentsAtLevel(bot->GetLevel(), static_cast<Classes>(bot->GetClass())));

    view.ApplyTrait = [bot](std::int32_t nodeEntryId, std::int32_t rank, std::int32_t granted, bool on)
    {
        bot->ApplyTraitEntry(nodeEntryId, rank, granted, on);
    };

    // Rage is stored as raw points (Player.cpp compares it to max rage directly).
    view.Combat.Rage = static_cast<std::uint32_t>(bot->GetPower(POWER_RAGE));
    view.Combat.PlayerHealthPct = static_cast<std::uint32_t>(bot->GetHealthPct());
    view.Combat.TargetHealthPct = target ? static_cast<std::uint32_t>(target->GetHealthPct()) : 100u;

    // Bot-side auras.
    view.Combat.SuddenDeathActive = bot->HasAura(SPELL_SUDDEN_DEATH);
    if (Aura* sudden = bot->GetAura(SPELL_SUDDEN_DEATH))
        view.Combat.SuddenDeathStacks = sudden->GetStackAmount();
    // TODO(UNVERIFIED): Opportunist stack buff is read from the talent spell 444774. Confirm the buff ID.
    if (Aura* opportunist = bot->GetAura(444774u))
        view.Combat.OpportunistStacks = opportunist->GetStackAmount();
    view.Combat.HackAndSlashActive = bot->HasAura(SPELL_HACK_AND_SLASH);
    view.Combat.HasProtExecuteTalent = bot->HasSpell(SPELL_MASSACRE_TALENT);
    view.Combat.CooldownsActive = bot->HasAura(SPELL_RECKLESSNESS);
    if (Aura* imminent = bot->GetAura(SPELL_IMMINENT_DEMISE))
        view.Combat.ImminentDemiseStacks = imminent->GetStackAmount();

    // Target-side auras applied by this bot.
    if (target)
    {
        view.Combat.ColossusSmashActive = target->HasAura(SPELL_COLOSSUS_SMASH, bot->GetGUID());
        if (Aura* rend = target->GetAura(SPELL_REND_ARMS, bot->GetGUID()))
            view.Combat.RendRemainingMs = static_cast<std::uint32_t>(rend->GetDuration());
    }

    view.CanCast = [bot, target](std::uint32_t spellId) { return CanCastOn(bot, target, spellId); };
    return view;
}
}

std::uint32_t TickBot(Player* bot, Unit* target, Engine::BotSetup const& setup, Engine::BotProgress& progress)
{
    Engine::BotView view = ReadView(bot, target);
    Engine::TickResult result = Engine::Tick(setup, view, progress);

    if (result.SpellToCast)
    {
        WorldObject* castTarget = target ? static_cast<WorldObject*>(target) : static_cast<WorldObject*>(bot);
        bot->CastSpell(castTarget, result.SpellToCast, CastSpellExtraArgs{});
    }
    return result.SpellToCast;
}
}
