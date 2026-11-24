#ifndef FCBOTS_HUMAN_RACIALS_H_H
#define FCBOTS_HUMAN_RACIALS_H_H

#include <cstdint>

// Human racial spell IDs, Midnight 12.1.5 DB2 (SpellName.csv).
namespace FCBots::Race::Human
{
constexpr std::uint32_t SPELL_THE_HUMAN_SPIRIT = 20598u;   // passive
constexpr std::uint32_t SPELL_DIPLOMACY = 20599u;
// UNVERIFIED: "Every Man for Himself" was not found in SpellName.csv.
// UNVERIFIED: "Perception" (1265001) is a candidate Human racial, not confirmed.
}

#endif // FCBOTS_HUMAN_RACIALS_H_H
