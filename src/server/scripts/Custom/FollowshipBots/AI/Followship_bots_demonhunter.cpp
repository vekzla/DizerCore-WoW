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

#include "Followship_bots_utils.h"

std::vector<FSBSpellDefinition> DemonHunterSpellsTable =
{
    // Spell ID                             Spell Type              ManaCost %  HP % for heal   Chance           Dist/Range     SelfCast    Cooldown Ms     RoleMask

    // Havoc
    { SPELL_DH_CHAOS_STRIKE,                FSBSpellType::Damage,   0.f,        0.f,            100.f,           2.f,           false,      1000,           FSB_RoleMask::FSB_ROLEMASK_MELEE_DAMAGE },
    { SPELL_DH_BLADE_DANCE,                 FSBSpellType::Damage,   0.f,        0.f,            90.f,            8.f,           false,      25000,          FSB_RoleMask::FSB_ROLEMASK_MELEE_DAMAGE },
    { SPELL_DH_EYE_BEAM,                    FSBSpellType::Damage,   0.f,        0.f,            80.f,            20.f,          false,      40000,          FSB_RoleMask::FSB_ROLEMASK_MELEE_DAMAGE },
    { SPELL_DH_FELBLADE,                    FSBSpellType::Damage,   0.f,        0.f,            80.f,            15.f,          false,      20000,          FSB_RoleMask::FSB_ROLEMASK_MELEE_DAMAGE },
    { SPELL_DH_THROW_GLAIVE,                FSBSpellType::Damage,   0.f,        0.f,            70.f,            30.f,          false,      9000,           FSB_RoleMask::FSB_ROLEMASK_MELEE_DAMAGE },
    { SPELL_DH_METAMORPHOSIS,               FSBSpellType::Damage,   0.f,        0.f,            100.f,           0.f,           true,       180000,         FSB_RoleMask::FSB_ROLEMASK_MELEE_DAMAGE },
    { SPELL_DH_FEL_RUSH,                    FSBSpellType::Damage,   0.f,        0.f,            60.f,            15.f,          false,      10000,          FSB_RoleMask::FSB_ROLEMASK_MELEE_DAMAGE },
    { SPELL_DH_DISRUPT,                     FSBSpellType::Damage,   0.f,        0.f,            100.f,           20.f,          false,      15000,          FSB_RoleMask::FSB_ROLEMASK_MELEE_DAMAGE },
    { SPELL_DH_VENGEFUL_RETREAT,            FSBSpellType::Damage,   0.f,        0.f,            50.f,            15.f,          false,      25000,          FSB_RoleMask::FSB_ROLEMASK_MELEE_DAMAGE },

    // Vengeance
    { SPELL_DH_DEMON_SPIKES,                FSBSpellType::Heal,     0.f,        70.f,           100.f,           0.f,           true,       20000,          FSB_RoleMask::FSB_ROLEMASK_TANK },
    { SPELL_DH_FIERY_BRAND,                 FSBSpellType::Damage,   0.f,        0.f,            90.f,            15.f,          false,      60000,          FSB_RoleMask::FSB_ROLEMASK_TANK },
    { SPELL_DH_SIGIL_OF_FLAME,              FSBSpellType::Damage,   0.f,        0.f,            80.f,            30.f,          false,      30000,          FSB_RoleMask::FSB_ROLEMASK_TANK },
    { SPELL_DH_TORMENT,                     FSBSpellType::Damage,   0.f,        0.f,            70.f,            5.f,           false,      10000,          FSB_RoleMask::FSB_ROLEMASK_TANK },
    { SPELL_DH_IMMOLATION_AURA,             FSBSpellType::Damage,   0.f,        0.f,            60.f,            8.f,           true,       30000,          FSB_RoleMask::FSB_ROLEMASK_TANK },
};
