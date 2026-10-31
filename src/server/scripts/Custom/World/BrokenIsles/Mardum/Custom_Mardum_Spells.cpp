/*
 * This file is part of the Stefal WoW Project.
 * It is designed to work exclusively with the TrinityCore framework.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * This code is provided for personal and educational use within the
 * Stefal WoW Project. It is not intended for commercial distribution,
 * resale, or any form of monetization.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Creature.h"
#include "GameObject.h"
#include "Player.h"
#include "Random.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "TemporarySummon.h"
#include "Unit.h"

#include "Custom_Mardum_Defines.h"

namespace Scripts::Custom::Mardum
{
    // 200596 - Open Portal (cast by Eredar Summoner 95046 on aggro)
    // Retail chains two script/dummy effects whose base points are summon spells:
    // Effect 0 SCRIPT_EFFECT -> 200597 (summons Fel Portal 101692)
    // Effect 1 DUMMY         -> 200603 (summons Shadow Portal 101696)
    class spell_mardum_open_portal : public SpellScript
    {
        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
                caster->CastSpell(caster, GetEffectValueAsInt(), true);
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(spell_mardum_open_portal::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            OnEffectHit += SpellEffectFn(spell_mardum_open_portal::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    // 190610 - Legion Portal use spell (player casts it on self when using GO 243873)
    // Using a Legion Portal pulls every Eredar Summoner within 10y of that portal
    // into combat with the player - their SAI aggro event then opens the portals.
    class spell_legion_portal_use : public SpellScript
    {
        void PreventMissingSpellTrigger(SpellEffIndex effIndex)
        {
            // Effect 1 triggers spell 191881, which does not exist in this build's
            // spell data - block the effect so the trigger never executes.
            PreventHitEffect(effIndex);
        }

        void HandleCast()
        {
            Unit* caster = GetCaster();
            GameObject* portal = caster->FindNearestGameObject(GameObjects::LegionPortal, Misc::PortalSearchRange);
            if (!portal)
                return;

            for (uint32 entry : { Creatures::EredarSummoner, Creatures::EredarSummonerFemale })
            {
                std::list<Creature*> summoners;
                portal->GetCreatureListWithEntryInGrid(summoners, entry, Misc::PortalSummonerSearchRange);
                for (Creature* summoner : summoners)
                    if (!summoner->IsInCombat())
                        summoner->EngageWithTarget(caster);
            }
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_legion_portal_use::HandleCast);
            // TRIGGER_SPELL executes in launch phase - must prevent before OnEffectHit
            OnEffectLaunch += SpellEffectFn(spell_legion_portal_use::PreventMissingSpellTrigger, EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
            OnEffectLaunchTarget += SpellEffectFn(spell_legion_portal_use::PreventMissingSpellTrigger, EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
        }
    };

    // 191466 - Illidari Reinforcements (5s periodic dummy, Mardum-only area aura)
    // In combat, periodically summons one of the three Illidari helpers for the
    // player - never a second one while any of them is still out.
    class spell_illidari_reinforcements : public AuraScript
    {
        void HandlePeriodic(AuraEffect const* /*aurEff*/)
        {
            Unit* target = GetTarget();
            if (!target->IsInCombat())
                return;

            for (uint32 entry : { Creatures::SummonedAshtongueStalker,
                                  Creatures::CoilskarSeaCaller,
                                  Creatures::ShivarraDestroyer })
            {
                std::list<TempSummon*> minions;
                target->GetAllMinionsByEntry(minions, entry);
                if (!minions.empty())
                    return;
            }

            static constexpr uint32 SummonSpells[3] =
            {
                Spells::SummonAshtongueStalker,
                Spells::SummonCoilskarSeaCaller,
                Spells::SummonShivarraDestroyer
            };
            target->CastSpell(target, SummonSpells[urand(0, 2)], true);
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_illidari_reinforcements::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };
}

void AddSC_custom_mardum_spells()
{
    using namespace Scripts::Custom::Mardum;

    RegisterSpellScript(spell_mardum_open_portal);
    RegisterSpellScript(spell_legion_portal_use);
    RegisterSpellScript(spell_illidari_reinforcements);
}
