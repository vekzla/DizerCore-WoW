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

#pragma once

#include "Followship_bots_spells_handler.h"

// Stage 0.2: Demon Hunter, first pass. Spell IDs are unverified against the 12.x SpellName export.
enum FSB_DEMONHUNTER_SPELLS
{
    // Havoc (melee damage)
    SPELL_DH_CHAOS_STRIKE = 162794,
    SPELL_DH_BLADE_DANCE = 188499,
    SPELL_DH_EYE_BEAM = 198013,
    SPELL_DH_FELBLADE = 232893,
    SPELL_DH_FEL_RUSH = 195072,
    SPELL_DH_DISRUPT = 183752,
    SPELL_DH_METAMORPHOSIS = 191427,
    SPELL_DH_THROW_GLAIVE = 185123,
    SPELL_DH_VENGEFUL_RETREAT = 198793,

    // Vengeance (tank)
    SPELL_DH_DEMON_SPIKES = 203720,
    SPELL_DH_FIERY_BRAND = 204021,
    SPELL_DH_SIGIL_OF_FLAME = 204596,
    SPELL_DH_TORMENT = 185245,
    SPELL_DH_IMMOLATION_AURA = 258920,
};

extern std::vector<FSBSpellDefinition> DemonHunterSpellsTable;

namespace FSBDemonHunter
{
}
