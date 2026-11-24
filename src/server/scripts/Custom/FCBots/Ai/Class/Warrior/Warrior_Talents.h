#ifndef FCBOTS_WARRIOR_TALENTS_H_H
#define FCBOTS_WARRIOR_TALENTS_H_H

#include <cstddef>
#include <cstdint>
#include <functional>

// Warrior talent builds, Midnight 12.1.5 (max level). Source: wow.gg talent calculator
// build strings, decoded. Each entry is spellID and rank. Names are from SpellName.csv.
//
// Point placement: a bot walks its build list in order and spends each point as it
// becomes available. Level gates and prerequisites are bypassed for bots (see the
// bypass constants below), so the list order is the order of purchase.
// Tree caps: class tree 34 points, spec tree 34 points, Hero tree 4 points.
// Level-by-level unlock rate is UNVERIFIED. It is not encoded here.
namespace FCBots::Class::Warrior::Talents
{
enum class Spec : std::uint8_t
{
    Arms = 1,
    Fury = 2,
    Protection = 3
};

enum class HeroTree : std::uint8_t
{
    Colossus = 1,
    Slayer = 2,
    MountainThane = 3
};

// Point caps. Class and spec nodes share tree 850 and total 68 points in the builds
// (34 + 34). Hero points are a separate pool: 14 in Arms Slayer, 13 in Fury Mountain Thane.
// TODO(UNVERIFIED): confirm the caps against NumTalentsAtLevel and the in-game counter.
constexpr std::uint8_t CAP_CLASS_TREE = 34u;
constexpr std::uint8_t CAP_SPEC_TREE = 34u;
constexpr std::uint8_t CAP_HERO_TREE = 14u;

// Bypass rules for bots. Taken from the requirement that bots learn any talent without
// checks. The engine must not test prerequisites or level gates for these.
constexpr bool BYPASS_PREREQUISITES = true;
constexpr bool BYPASS_LEVEL_GATES = true;
constexpr bool BYPASS_POINT_CAPS = false;   // keep caps; set true to remove them too

struct TalentPick
{
    std::uint32_t SpellId;
    std::uint32_t Rank;       // points needed for this talent (sum of ranks = points spent)
    std::uint32_t Tier;       // row in the trait tree, ordered top to bottom
    std::uint32_t Column;     // position across the row
    std::uint32_t SubTree;    // 0 class/spec tree, 60 Slayer, 61 Mountain Thane, 62 Colossus
    std::uint32_t TraitNodeEntryId; // TraitNodeEntry.ID, passed to Player::ApplyTraitEntry
};

// Applies one trait node entry. Matches Player::ApplyTraitEntry(nodeEntryId, rank, grantedRanks, apply).
using TraitApplyFn = std::function<void(std::int32_t nodeEntryId, std::int32_t rank, std::int32_t grantedRanks, bool apply)>;

// Points a build needs in total (sum of ranks).
std::uint32_t BuildTotalPoints(TalentPick const* build, std::size_t count);

// Spends available points on the build, in list order. Stops at the first pick that cannot
// be afforded so the order is kept. cursor is the number of picks already applied; it is
// updated. Returns the number of points spent in this call.
std::uint32_t SpendTalentPoints(TalentPick const* build, std::size_t count,
    std::uint32_t availablePoints, std::size_t& cursor, TraitApplyFn const& apply);

// ARMS_SLAYER: ordered by tier then column. Tier from the trait tree row (see Talents.h note).
constexpr TalentPick ARMS_SLAYER_BUILD[] =
{
    { 12294u, 1u, 7u, 12000u, 0u, 112122u },  // Mortal Strike
    { 382260u, 1u, 9u, 1800u, 0u, 112206u },  // Fast Footwork
    { 262231u, 1u, 9u, 3000u, 0u, 112185u },  // War Machine
    { 382258u, 1u, 9u, 5400u, 0u, 112238u },  // Leeching Strikes
    { 7384u, 1u, 9u, 11400u, 0u, 112123u },  // Overpower
    { 29725u, 1u, 9u, 12600u, 0u, 112126u },  // Sudden Death
    { 444769u, 1u, 11u, 6900u, 60u, 117385u },  // Imminent Demise
    { 444772u, 1u, 11u, 7500u, 60u, 117407u },  // Overwhelming Blades
    { 444776u, 1u, 11u, 8101u, 60u, 117392u },  // Relentless Pursuit
    { 1270717u, 1u, 11u, 8700u, 60u, 136076u },  // Violent Euphoria
    { 202168u, 1u, 14u, 2400u, 0u, 112183u },  // Impending Victory
    { 6544u, 1u, 14u, 3600u, 0u, 112208u },  // Heroic Leap
    { 107570u, 1u, 14u, 4800u, 0u, 112198u },  // Storm Bolt
    { 383103u, 1u, 14u, 11400u, 0u, 112128u },  // Fueled by Violence
    { 118038u, 1u, 14u, 12600u, 0u, 112121u },  // Die by the Sword
    { 444770u, 1u, 15u, 6900u, 60u, 117410u },  // Death Drive
    { 444778u, 1u, 15u, 7500u, 60u, 117383u },  // Culling Cyclone
    { 444774u, 1u, 15u, 8100u, 60u, 123770u },  // Opportunist
    { 1270718u, 1u, 15u, 8700u, 60u, 136075u },  // Deadly Focus
    { 772u, 1u, 17u, 1800u, 0u, 135597u },  // Rend
    { 392792u, 1u, 17u, 3000u, 0u, 112216u },  // Frothing Berserker
    { 202163u, 1u, 17u, 3600u, 0u, 112219u },  // Bounding Stride
    { 382549u, 1u, 17u, 4200u, 0u, 112217u },  // Pain and Gain
    { 385571u, 1u, 17u, 11400u, 0u, 112131u },  // Improved Overpower
    { 316405u, 1u, 17u, 12600u, 0u, 112125u },  // Improved Execute
    { 202316u, 1u, 17u, 13200u, 0u, 135932u },  // Fervor of Battle
    { 444771u, 1u, 19u, 6900u, 60u, 117381u },  // Show No Mercy
    { 444775u, 1u, 19u, 7500u, 60u, 117406u },  // Reap the Storm
    { 444779u, 1u, 19u, 8101u, 60u, 117398u },  // Slayer's Malice
    { 386628u, 1u, 19u, 8700u, 60u, 136074u },  // Unhinged
    { 46968u, 1u, 21u, 2400u, 0u, 112242u },  // Shockwave
    { 382767u, 1u, 21u, 3000u, 0u, 112245u },  // Overwhelming Rage
    { 97462u, 1u, 21u, 3600u, 0u, 112188u },  // Rallying Cry
    { 1271926u, 1u, 21u, 4200u, 0u, 136626u },  // Field Dressing
    { 23920u, 1u, 21u, 4800u, 0u, 112253u },  // Spell Reflection
    { 184783u, 1u, 21u, 10800u, 0u, 112134u },  // Tactician
    { 167105u, 1u, 21u, 12000u, 0u, 112144u },  // Colossus Smash
    { 383430u, 1u, 21u, 13200u, 0u, 112146u },  // Impale
    { 444780u, 1u, 23u, 7850u, 60u, 117417u },  // Unrelenting Onslaught
    { 275339u, 1u, 25u, 2400u, 0u, 112241u },  // Rumbling Earth
    { 1271925u, 1u, 25u, 3000u, 0u, 112210u },  // Fearless
    { 391271u, 1u, 25u, 5400u, 0u, 118850u },  // Honed Reflexes
    { 1261060u, 1u, 25u, 10200u, 0u, 136451u },  // Deep Wounds
    { 400803u, 2u, 25u, 11400u, 0u, 135944u },  // Strength of Arms
    { 1261049u, 1u, 25u, 12000u, 0u, 135942u },  // Broad Strokes
    { 383341u, 2u, 25u, 12600u, 0u, 112320u },  // Sharpened Blades
    { 845u, 1u, 25u, 13200u, 0u, 112147u },  // Cleave
    { 1261050u, 1u, 25u, 13800u, 0u, 114739u },  // Powerful Momentum
    { 384124u, 2u, 28u, 2400u, 0u, 112233u },  // Armored to the Teeth
    { 382939u, 2u, 28u, 4800u, 0u, 112235u },  // Reinforced Plates
    { 262150u, 1u, 28u, 10800u, 0u, 112137u },  // Dreadnaught
    { 1261051u, 1u, 28u, 12000u, 0u, 135939u },  // Tactical Edge
    { 281001u, 1u, 28u, 12600u, 0u, 112145u },  // Massacre
    { 334779u, 1u, 28u, 13200u, 0u, 135937u },  // Collateral Damage
    { 103827u, 1u, 29u, 3602u, 0u, 112249u },  // Double Time
    { 383082u, 1u, 33u, 1800u, 0u, 112209u },  // Barbaric Training
    { 1243660u, 1u, 33u, 4200u, 0u, 134225u },  // Resonant Voice
    { 227847u, 1u, 33u, 12000u, 0u, 112314u },  // Bladestorm
    { 107574u, 1u, 33u, 13200u, 0u, 112305u },  // Avatar
    { 389306u, 2u, 33u, 13200u, 0u, 112317u },  // Critical Thinking
    { 392777u, 2u, 36u, 2400u, 0u, 112248u },  // Cruel Strikes
    { 382896u, 2u, 36u, 3600u, 0u, 112179u },  // Two-Handed Weapon Specialization
    { 382946u, 2u, 36u, 4800u, 0u, 112224u },  // Wild Strikes
    { 383154u, 1u, 36u, 11400u, 0u, 112310u },  // Bloodletting
    { 386634u, 1u, 36u, 12600u, 0u, 112318u },  // Executioner's Precision
    { 152278u, 1u, 39u, 1800u, 0u, 134032u },  // Anger Management
    { 1280961u, 1u, 39u, 4200u, 0u, 134031u },  // Stance Mastery
    { 424742u, 1u, 39u, 5400u, 0u, 134033u },  // Battlefield Commander
    { 1261062u, 1u, 39u, 12000u, 0u, 135941u },  // Mortal Wounds
    { 1269306u, 2u, 41u, 12000u, 0u, 136988u },  // Master of Warfare
    { 1269307u, 1u, 41u, 12000u, 0u, 136987u },  // Master of Warfare
    { 1269314u, 1u, 41u, 12000u, 0u, 136989u },  // Master of Warfare
};

// FURY_SLAYER: ordered by tier then column. Tier from the trait tree row (see Talents.h note).
constexpr TalentPick FURY_SLAYER_BUILD[] =
{
    { 23881u, 1u, 6u, 12002u, 0u, 112261u },  // Bloodthirst
    { 382260u, 1u, 9u, 1800u, 0u, 112206u },  // Fast Footwork
    { 262231u, 1u, 9u, 3000u, 0u, 112185u },  // War Machine
    { 382258u, 1u, 9u, 5400u, 0u, 112238u },  // Leeching Strikes
    { 85288u, 1u, 9u, 12000u, 0u, 112265u },  // Raging Blow
    { 29725u, 1u, 9u, 12600u, 0u, 112126u },  // Sudden Death
    { 444769u, 1u, 11u, 6900u, 60u, 117385u },  // Imminent Demise
    { 444772u, 1u, 11u, 7500u, 60u, 117407u },  // Overwhelming Blades
    { 444776u, 1u, 11u, 8101u, 60u, 117392u },  // Relentless Pursuit
    { 1270717u, 1u, 11u, 8700u, 60u, 136076u },  // Violent Euphoria
    { 202168u, 1u, 14u, 2400u, 0u, 112183u },  // Impending Victory
    { 6544u, 1u, 14u, 3600u, 0u, 112208u },  // Heroic Leap
    { 107570u, 1u, 14u, 4800u, 0u, 112198u },  // Storm Bolt
    { 383848u, 1u, 14u, 12000u, 0u, 112267u },  // Frenzied Enrage
    { 316402u, 1u, 14u, 13200u, 0u, 112300u },  // Improved Execute
    { 444770u, 1u, 15u, 6900u, 60u, 117410u },  // Death Drive
    { 444778u, 1u, 15u, 7500u, 60u, 117383u },  // Culling Cyclone
    { 444774u, 1u, 15u, 8100u, 60u, 123770u },  // Opportunist
    { 1270718u, 1u, 15u, 8700u, 60u, 136075u },  // Deadly Focus
    { 1299025u, 1u, 17u, 1800u, 0u, 137472u },  // Storm of Blood
    { 392792u, 1u, 17u, 3000u, 0u, 112216u },  // Frothing Berserker
    { 202163u, 1u, 17u, 3600u, 0u, 112219u },  // Bounding Stride
    { 382549u, 1u, 17u, 4200u, 0u, 112217u },  // Pain and Gain
    { 1244088u, 1u, 17u, 5400u, 0u, 134213u },  // Interpose
    { 383486u, 1u, 17u, 10200u, 0u, 112272u },  // Focus in Chaos
    { 208154u, 1u, 17u, 12600u, 0u, 112263u },  // Warpaint
    { 12950u, 1u, 17u, 13800u, 0u, 112298u },  // Improved Whirlwind
    { 444771u, 1u, 19u, 6900u, 60u, 117381u },  // Show No Mercy
    { 444775u, 1u, 19u, 7500u, 60u, 117406u },  // Reap the Storm
    { 444779u, 1u, 19u, 8101u, 60u, 117398u },  // Slayer's Malice
    { 386628u, 1u, 19u, 8700u, 60u, 136074u },  // Unhinged
    { 46968u, 1u, 21u, 2400u, 0u, 112242u },  // Shockwave
    { 382767u, 1u, 21u, 3000u, 0u, 112245u },  // Overwhelming Rage
    { 97462u, 1u, 21u, 3600u, 0u, 112188u },  // Rallying Cry
    { 23920u, 1u, 21u, 4800u, 0u, 112253u },  // Spell Reflection
    { 215568u, 1u, 21u, 10800u, 0u, 112268u },  // Fresh Meat
    { 383854u, 1u, 21u, 13200u, 0u, 112259u },  // Improved Raging Blow
    { 1265357u, 1u, 21u, 13800u, 0u, 112260u },  // Rampaging Ruin
    { 184367u, 1u, 22u, 12007u, 0u, 112277u },  // Rampage
    { 444780u, 1u, 23u, 7850u, 60u, 117417u },  // Unrelenting Onslaught
    { 275339u, 1u, 25u, 2400u, 0u, 112241u },  // Rumbling Earth
    { 1271925u, 1u, 25u, 3000u, 0u, 112210u },  // Fearless
    { 391271u, 1u, 25u, 5400u, 0u, 118850u },  // Honed Reflexes
    { 1261060u, 1u, 25u, 10200u, 0u, 136451u },  // Deep Wounds
    { 383852u, 2u, 25u, 10800u, 0u, 112266u },  // Spite
    { 383877u, 1u, 25u, 12600u, 0u, 112276u },  // Hack and Slash
    { 392931u, 2u, 25u, 13200u, 0u, 112299u },  // Cruelty
    { 280392u, 1u, 25u, 13800u, 0u, 136452u },  // Meat Cleaver
    { 1719u, 1u, 27u, 12002u, 0u, 112281u },  // Recklessness
    { 384124u, 2u, 28u, 2400u, 0u, 112233u },  // Armored to the Teeth
    { 382939u, 2u, 28u, 4800u, 0u, 112235u },  // Reinforced Plates
    { 392936u, 1u, 28u, 13800u, 0u, 112255u },  // Wrath and Fury
    { 103827u, 1u, 29u, 3602u, 0u, 112249u },  // Double Time
    { 383082u, 1u, 33u, 1800u, 0u, 112209u },  // Barbaric Training
    { 1243660u, 1u, 33u, 4200u, 0u, 134225u },  // Resonant Voice
    { 227847u, 1u, 33u, 12000u, 0u, 112314u },  // Bladestorm
    { 335077u, 1u, 33u, 12000u, 0u, 112275u },  // Frenzy
    { 383297u, 2u, 33u, 13200u, 0u, 112296u },  // Critical Thinking
    { 392777u, 2u, 36u, 2400u, 0u, 112248u },  // Cruel Strikes
    { 382900u, 2u, 36u, 3600u, 0u, 112240u },  // Dual Wield Specialization
    { 382946u, 2u, 36u, 4800u, 0u, 112224u },  // Wild Strikes
    { 385059u, 1u, 36u, 11400u, 0u, 136735u },  // Odyn's Fury
    { 385703u, 1u, 36u, 12600u, 0u, 112270u },  // Bloodborne
    { 1265359u, 1u, 36u, 13800u, 0u, 136449u },  // Surge of Adrenaline
    { 152278u, 1u, 39u, 1800u, 0u, 134032u },  // Anger Management
    { 1280961u, 1u, 39u, 4200u, 0u, 134031u },  // Stance Mastery
    { 424742u, 1u, 39u, 5400u, 0u, 134033u },  // Battlefield Commander
    { 1265570u, 1u, 39u, 10800u, 0u, 136453u },  // Executioner's Wrath
    { 396749u, 1u, 39u, 12000u, 0u, 119139u },  // Reckless Abandon
    { 1269308u, 1u, 41u, 12000u, 0u, 137004u },  // Rampaging Berserker
    { 1269309u, 2u, 41u, 12000u, 0u, 137003u },  // Rampaging Berserker
    { 1269310u, 1u, 41u, 12000u, 0u, 137002u },  // Rampaging Berserker
};

// PROT_MOUNTAIN_THANE: ordered by tier then column. Tier from the trait tree row (see Talents.h note).
constexpr TalentPick PROT_MOUNTAIN_THANE_BUILD[] =
{
    { 436707u, 1u, 3u, 14081u, 61u, 117413u },  // Crashing Thunder
    { 436148u, 1u, 4u, 14697u, 61u, 117397u },  // Ground Current
    { 437068u, 1u, 5u, 15311u, 61u, 117405u },  // Strength of the Mountain
    { 190456u, 1u, 6u, 12002u, 0u, 112149u },  // Ignore Pain
    { 275336u, 1u, 7u, 15900u, 61u, 136070u },  // Storm Surge
    { 262231u, 1u, 9u, 3000u, 0u, 112185u },  // War Machine
    { 6343u, 1u, 9u, 4200u, 0u, 112205u },  // Thunder Clap
    { 382258u, 1u, 9u, 5400u, 0u, 112238u },  // Leeching Strikes
    { 1160u, 1u, 9u, 11400u, 0u, 112159u },  // Demoralizing Shout
    { 6572u, 1u, 9u, 12600u, 0u, 112152u },  // Revenge
    { 435607u, 1u, 9u, 14102u, 61u, 117382u },  // Thunder Blast
    { 436162u, 1u, 9u, 14700u, 61u, 117414u },  // Storm Bolts
    { 1270723u, 1u, 9u, 15900u, 61u, 136069u },  // Conductivity
    { 438590u, 1u, 10u, 15311u, 61u, 117395u },  // Keep Your Feet on the Ground
    { 436152u, 1u, 12u, 14697u, 61u, 118834u },  // Thorim's Might
    { 456270u, 1u, 13u, 14102u, 61u, 118833u },  // Snap Induction
    { 202168u, 1u, 14u, 2400u, 0u, 112183u },  // Impending Victory
    { 6544u, 1u, 14u, 3600u, 0u, 112208u },  // Heroic Leap
    { 203201u, 1u, 14u, 4200u, 0u, 118853u },  // Crackling Thunder
    { 107570u, 1u, 14u, 4800u, 0u, 112198u },  // Storm Bolt
    { 386030u, 1u, 14u, 10800u, 0u, 112158u },  // Brace For Impact
    { 383103u, 1u, 14u, 11400u, 0u, 112128u },  // Fueled by Violence
    { 1234769u, 1u, 14u, 11700u, 0u, 112160u },  // Armor Specialization
    { 236279u, 1u, 14u, 13200u, 0u, 112151u },  // Devastator
    { 437118u, 1u, 14u, 15300u, 61u, 117404u },  // Burst of Power
    { 1270724u, 1u, 14u, 15900u, 61u, 136068u },  // Capacitance
    { 384041u, 1u, 16u, 12002u, 0u, 112157u },  // Strategist
    { 384277u, 1u, 17u, 1800u, 0u, 137471u },  // Blood and Thunder
    { 202163u, 1u, 17u, 3600u, 0u, 112219u },  // Bounding Stride
    { 382549u, 1u, 17u, 4200u, 0u, 112217u },  // Pain and Gain
    { 3411u, 1u, 17u, 5400u, 0u, 112186u },  // Intervene
    { 386071u, 1u, 17u, 11400u, 0u, 132886u },  // Disrupting Shout
    { 437134u, 1u, 18u, 15008u, 61u, 117402u },  // Avatar of the Storm
    { 394311u, 1u, 20u, 11402u, 0u, 112155u },  // Instigate
    { 46968u, 1u, 21u, 2400u, 0u, 112242u },  // Shockwave
    { 97462u, 1u, 21u, 3600u, 0u, 112188u },  // Rallying Cry
    { 23920u, 1u, 21u, 4800u, 0u, 112253u },  // Spell Reflection
    { 384036u, 1u, 21u, 10800u, 0u, 112325u },  // Brutal Vitality
    { 202560u, 1u, 21u, 13200u, 0u, 112163u },  // Best Served Cold
    { 871u, 1u, 22u, 12002u, 0u, 112156u },  // Shield Wall
    { 385840u, 1u, 24u, 10802u, 0u, 112162u },  // Thunderlord
    { 384110u, 1u, 25u, 1800u, 0u, 112215u },  // Wrecking Throw
    { 275339u, 1u, 25u, 2400u, 0u, 112241u },  // Rumbling Earth
    { 5246u, 1u, 25u, 4200u, 0u, 134254u },  // Intimidating Shout
    { 391271u, 1u, 25u, 5400u, 0u, 118850u },  // Honed Reflexes
    { 397103u, 1u, 25u, 11400u, 0u, 112174u },  // Defender's Aegis
    { 1243659u, 1u, 25u, 12000u, 0u, 132879u },  // Last Stand
    { 275334u, 1u, 25u, 13200u, 0u, 112321u },  // Punish
    { 386027u, 2u, 27u, 12002u, 0u, 112166u },  // Enduring Defenses
    { 384124u, 2u, 28u, 2400u, 0u, 112233u },  // Armored to the Teeth
    { 382939u, 2u, 28u, 4800u, 0u, 112235u },  // Reinforced Plates
    { 385888u, 1u, 28u, 10200u, 0u, 112177u },  // Tough as Nails
    { 1235047u, 1u, 28u, 13200u, 0u, 112323u },  // Unyielding Stance
    { 103827u, 1u, 29u, 3602u, 0u, 112249u },  // Double Time
    { 386011u, 1u, 32u, 12002u, 0u, 112171u },  // Shield Specialization
    { 1271948u, 1u, 33u, 3000u, 0u, 136625u },  // Javelineer
    { 1243660u, 1u, 33u, 4200u, 0u, 134225u },  // Resonant Voice
    { 203177u, 1u, 33u, 10800u, 0u, 112167u },  // Heavy Repercussions
    { 107574u, 1u, 33u, 13200u, 0u, 112305u },  // Avatar
    { 202743u, 1u, 35u, 11402u, 0u, 112170u },  // Booming Voice
    { 202095u, 1u, 35u, 12002u, 0u, 112306u },  // Indomitable
    { 386477u, 1u, 35u, 12602u, 0u, 112116u },  // Violent Outburst
    { 392777u, 2u, 36u, 2400u, 0u, 112248u },  // Cruel Strikes
    { 382895u, 2u, 36u, 3600u, 0u, 112181u },  // One-Handed Weapon Specialization
    { 382946u, 2u, 36u, 4800u, 0u, 112224u },  // Wild Strikes
    { 386394u, 1u, 38u, 12002u, 0u, 112307u },  // Battle-Scarred Veteran
    { 152278u, 1u, 39u, 1800u, 0u, 134032u },  // Anger Management
    { 1280961u, 1u, 39u, 4200u, 0u, 134031u },  // Stance Mastery
    { 424742u, 1u, 39u, 5400u, 0u, 134033u },  // Battlefield Commander
    { 385952u, 1u, 39u, 10800u, 0u, 112173u },  // Shield Charge
    { 1235113u, 1u, 39u, 13200u, 0u, 112304u },  // Whirling Blade
    { 1269311u, 1u, 41u, 12000u, 0u, 137001u },  // Phalanx
    { 1269312u, 2u, 41u, 12000u, 0u, 137000u },  // Phalanx
    { 1269313u, 1u, 41u, 12000u, 0u, 136999u },  // Phalanx
};

// cant be used: ARMS_COLOSSUS. wow.gg link tagged hero=colossus decodes to Slayer nodes (subtree 60).
//   Re-export with Colossus selected in the calculator (subtree 62).


// PROT_COLOSSUS: ordered by tier then column. Tier from the trait tree row (see Talents.h note).
constexpr TalentPick PROT_COLOSSUS_BUILD[] =
{
    { 385704u, 1u, 25u, 12600u, 0u, 112115u },  // Bloodborne
    { 386477u, 1u, 35u, 12602u, 0u, 112116u },  // Violent Outburst
    { 190456u, 1u, 6u, 12002u, 0u, 112149u },  // Ignore Pain
    { 236279u, 1u, 14u, 13200u, 0u, 112151u },  // Devastator
    { 6572u, 1u, 9u, 12600u, 0u, 112152u },  // Revenge
    { 394311u, 1u, 20u, 11402u, 0u, 112155u },  // Instigate
    { 871u, 1u, 22u, 12002u, 0u, 112156u },  // Shield Wall
    { 384041u, 1u, 16u, 12002u, 0u, 112157u },  // Strategist
    { 386030u, 1u, 14u, 10800u, 0u, 112158u },  // Brace For Impact
    { 1160u, 1u, 9u, 11400u, 0u, 112159u },  // Demoralizing Shout
    { 1234769u, 1u, 14u, 11700u, 0u, 112160u },  // Armor Specialization
    { 385840u, 1u, 24u, 10802u, 0u, 112162u },  // Thunderlord
    { 202560u, 1u, 21u, 13200u, 0u, 112163u },  // Best Served Cold
    { 386027u, 2u, 27u, 12002u, 0u, 112166u },  // Enduring Defenses
    { 203177u, 1u, 33u, 10800u, 0u, 112167u },  // Heavy Repercussions
    { 202743u, 1u, 35u, 11402u, 0u, 112170u },  // Booming Voice
    { 386011u, 1u, 32u, 12002u, 0u, 112171u },  // Shield Specialization
    { 385952u, 1u, 39u, 10800u, 0u, 112173u },  // Shield Charge
    { 397103u, 1u, 25u, 11400u, 0u, 112174u },  // Defender's Aegis
    { 383103u, 1u, 14u, 11400u, 0u, 112128u },  // Fueled by Violence
    { 385888u, 1u, 28u, 10200u, 0u, 112177u },  // Tough as Nails
    { 382895u, 2u, 36u, 3600u, 0u, 112181u },  // One-Handed Weapon Specialization
    { 202168u, 1u, 14u, 2400u, 0u, 112183u },  // Impending Victory
    { 262231u, 1u, 9u, 3000u, 0u, 112185u },  // War Machine
    { 3411u, 1u, 17u, 5400u, 0u, 112186u },  // Intervene
    { 97462u, 1u, 21u, 3600u, 0u, 112188u },  // Rallying Cry
    { 107570u, 1u, 14u, 4800u, 0u, 112198u },  // Storm Bolt
    { 6343u, 1u, 9u, 4200u, 0u, 112205u },  // Thunder Clap
    { 6544u, 1u, 14u, 3600u, 0u, 112208u },  // Heroic Leap
    { 384110u, 1u, 25u, 1800u, 0u, 112215u },  // Wrecking Throw
    { 382549u, 1u, 17u, 4200u, 0u, 112217u },  // Pain and Gain
    { 202163u, 1u, 17u, 3600u, 0u, 112219u },  // Bounding Stride
    { 382946u, 2u, 36u, 4800u, 0u, 112224u },  // Wild Strikes
    { 384124u, 2u, 28u, 2400u, 0u, 112233u },  // Armored to the Teeth
    { 382939u, 2u, 28u, 4800u, 0u, 112235u },  // Reinforced Plates
    { 382258u, 1u, 9u, 5400u, 0u, 112238u },  // Leeching Strikes
    { 275339u, 1u, 25u, 2400u, 0u, 112241u },  // Rumbling Earth
    { 46968u, 1u, 21u, 2400u, 0u, 112242u },  // Shockwave
    { 392777u, 2u, 36u, 2400u, 0u, 112248u },  // Cruel Strikes
    { 103827u, 1u, 29u, 3602u, 0u, 112249u },  // Double Time
    { 23920u, 1u, 21u, 4800u, 0u, 112253u },  // Spell Reflection
    { 1235113u, 1u, 39u, 13200u, 0u, 112304u },  // Whirling Blade
    { 107574u, 1u, 33u, 13200u, 0u, 112305u },  // Avatar
    { 275334u, 1u, 25u, 13200u, 0u, 112321u },  // Punish
    { 1235047u, 1u, 28u, 13200u, 0u, 112323u },  // Unyielding Stance
    { 384036u, 1u, 21u, 10800u, 0u, 112325u },  // Brutal Vitality
    { 391271u, 1u, 25u, 5400u, 0u, 118850u },  // Honed Reflexes
    { 203201u, 1u, 14u, 4200u, 0u, 118853u },  // Crackling Thunder
    { 384063u, 1u, 33u, 12600u, 0u, 132882u },  // Enduring Alacrity
    { 384067u, 1u, 33u, 11400u, 0u, 132883u },  // Focused Vigor
    { 386071u, 1u, 17u, 11400u, 0u, 132886u },  // Disrupting Shout
    { 1280961u, 1u, 39u, 4200u, 0u, 134031u },  // Stance Mastery
    { 152278u, 1u, 39u, 1800u, 0u, 134032u },  // Anger Management
    { 424742u, 1u, 39u, 5400u, 0u, 134033u },  // Battlefield Commander
    { 1243660u, 1u, 33u, 4200u, 0u, 134225u },  // Resonant Voice
    { 5246u, 1u, 25u, 4200u, 0u, 134254u },  // Intimidating Shout
    { 1271948u, 1u, 33u, 3000u, 0u, 136625u },  // Javelineer
    { 1269311u, 1u, 41u, 12000u, 0u, 137001u },  // Phalanx
    { 1269312u, 2u, 41u, 12000u, 0u, 137000u },  // Phalanx
    { 1269313u, 1u, 41u, 12000u, 0u, 136999u },  // Phalanx
    { 384277u, 1u, 17u, 1800u, 0u, 137471u },  // Blood and Thunder
    { 429638u, 1u, 30u, 6900u, 62u, 117409u },  // Martial Expert
    { 429634u, 1u, 30u, 7500u, 62u, 117416u },  // Colossal Might
    { 429637u, 1u, 34u, 6900u, 62u, 117396u },  // One Against Many
    { 429641u, 1u, 34u, 7500u, 62u, 117408u },  // Tide of Battle
    { 431548u, 1u, 37u, 7500u, 62u, 117391u },  // Precise Might
    { 429647u, 1u, 37u, 6900u, 62u, 117393u },  // Practiced Strikes
    { 440992u, 1u, 30u, 8100u, 62u, 119858u },  // Earthquaker
    { 440993u, 1u, 34u, 8100u, 62u, 119857u },  // Veteran Vitality
    { 1270704u, 1u, 31u, 8700u, 62u, 136073u },  // Decimator
    { 1270709u, 1u, 34u, 8700u, 62u, 136072u },  // Cut to the Bone
    { 1270710u, 1u, 37u, 8700u, 62u, 136071u },  // Celeritous Conclusion
    { 429642u, 1u, 37u, 8100u, 62u, 117403u },  // Mountain of Muscle and Scars
    { 429636u, 1u, 40u, 7850u, 62u, 117390u },  // Dominance of the Colossus
};

// FURY_MOUNTAIN_THANE: ordered by tier then column. Tier from the trait tree row (see Talents.h note).
constexpr TalentPick FURY_MOUNTAIN_THANE_BUILD[] =
{
    { 202168u, 1u, 14u, 2400u, 0u, 112183u },  // Impending Victory
    { 262231u, 1u, 9u, 3000u, 0u, 112185u },  // War Machine
    { 97462u, 1u, 21u, 3600u, 0u, 112188u },  // Rallying Cry
    { 107570u, 1u, 14u, 4800u, 0u, 112198u },  // Storm Bolt
    { 382260u, 1u, 9u, 1800u, 0u, 112206u },  // Fast Footwork
    { 6544u, 1u, 14u, 3600u, 0u, 112208u },  // Heroic Leap
    { 383082u, 1u, 33u, 1800u, 0u, 112209u },  // Barbaric Training
    { 1271925u, 1u, 25u, 3000u, 0u, 112210u },  // Fearless
    { 392792u, 1u, 17u, 3000u, 0u, 112216u },  // Frothing Berserker
    { 382549u, 1u, 17u, 4200u, 0u, 112217u },  // Pain and Gain
    { 202163u, 1u, 17u, 3600u, 0u, 112219u },  // Bounding Stride
    { 382946u, 2u, 36u, 4800u, 0u, 112224u },  // Wild Strikes
    { 384124u, 2u, 28u, 2400u, 0u, 112233u },  // Armored to the Teeth
    { 382939u, 2u, 28u, 4800u, 0u, 112235u },  // Reinforced Plates
    { 382258u, 1u, 9u, 5400u, 0u, 112238u },  // Leeching Strikes
    { 382900u, 2u, 36u, 3600u, 0u, 112240u },  // Dual Wield Specialization
    { 275339u, 1u, 25u, 2400u, 0u, 112241u },  // Rumbling Earth
    { 46968u, 1u, 21u, 2400u, 0u, 112242u },  // Shockwave
    { 382767u, 1u, 21u, 3000u, 0u, 112245u },  // Overwhelming Rage
    { 392777u, 2u, 36u, 2400u, 0u, 112248u },  // Cruel Strikes
    { 103827u, 1u, 29u, 3602u, 0u, 112249u },  // Double Time
    { 23920u, 1u, 21u, 4800u, 0u, 112253u },  // Spell Reflection
    { 392936u, 1u, 28u, 13800u, 0u, 112255u },  // Wrath and Fury
    { 396749u, 1u, 39u, 12000u, 0u, 119139u },  // Reckless Abandon
    { 383854u, 1u, 21u, 13200u, 0u, 112259u },  // Improved Raging Blow
    { 1265357u, 1u, 21u, 13800u, 0u, 112260u },  // Rampaging Ruin
    { 23881u, 1u, 6u, 12002u, 0u, 112261u },  // Bloodthirst
    { 208154u, 1u, 17u, 12600u, 0u, 112263u },  // Warpaint
    { 85288u, 1u, 9u, 12000u, 0u, 112265u },  // Raging Blow
    { 383852u, 2u, 25u, 10800u, 0u, 112266u },  // Spite
    { 383848u, 1u, 14u, 12000u, 0u, 112267u },  // Frenzied Enrage
    { 215568u, 1u, 21u, 10800u, 0u, 112268u },  // Fresh Meat
    { 385703u, 1u, 36u, 12600u, 0u, 112270u },  // Bloodborne
    { 383486u, 1u, 17u, 10200u, 0u, 112272u },  // Focus in Chaos
    { 335077u, 1u, 33u, 12000u, 0u, 112275u },  // Frenzy
    { 383877u, 1u, 25u, 12600u, 0u, 112276u },  // Hack and Slash
    { 184367u, 1u, 22u, 12007u, 0u, 112277u },  // Rampage
    { 1719u, 1u, 27u, 12002u, 0u, 112281u },  // Recklessness
    { 227847u, 1u, 39u, 13200u, 0u, 112284u },  // Bladestorm
    { 383297u, 2u, 33u, 13200u, 0u, 112296u },  // Critical Thinking
    { 12950u, 1u, 17u, 13800u, 0u, 112298u },  // Improved Whirlwind
    { 392931u, 2u, 25u, 13200u, 0u, 112299u },  // Cruelty
    { 316402u, 1u, 14u, 13200u, 0u, 112300u },  // Improved Execute
    { 29725u, 1u, 9u, 12600u, 0u, 112126u },  // Sudden Death
    { 391271u, 1u, 25u, 5400u, 0u, 118850u },  // Honed Reflexes
    { 1280961u, 1u, 39u, 4200u, 0u, 134031u },  // Stance Mastery
    { 152278u, 1u, 39u, 1800u, 0u, 134032u },  // Anger Management
    { 424742u, 1u, 39u, 5400u, 0u, 134033u },  // Battlefield Commander
    { 1244088u, 1u, 17u, 5400u, 0u, 134213u },  // Interpose
    { 1243660u, 1u, 33u, 4200u, 0u, 134225u },  // Resonant Voice
    { 1265359u, 1u, 36u, 13800u, 0u, 136449u },  // Surge of Adrenaline
    { 1261060u, 1u, 28u, 13800u, 0u, 112165u },  // Deep Wounds
    { 280392u, 1u, 25u, 13800u, 0u, 136452u },  // Meat Cleaver
    { 1265570u, 1u, 39u, 10800u, 0u, 136453u },  // Executioner's Wrath
    { 385059u, 1u, 36u, 11400u, 0u, 136735u },  // Odyn's Fury
    { 1269308u, 1u, 41u, 12000u, 0u, 137004u },  // Rampaging Berserker
    { 1269309u, 2u, 41u, 12000u, 0u, 137003u },  // Rampaging Berserker
    { 1269310u, 1u, 41u, 12000u, 0u, 137002u },  // Rampaging Berserker
    { 1299025u, 1u, 17u, 1800u, 0u, 137472u },  // Storm of Blood
    { 436707u, 1u, 3u, 14081u, 61u, 117413u },  // Crashing Thunder
    { 436148u, 1u, 4u, 14697u, 61u, 117397u },  // Ground Current
    { 437068u, 1u, 5u, 15311u, 61u, 117405u },  // Strength of the Mountain
    { 275336u, 1u, 7u, 15900u, 61u, 136070u },  // Storm Surge
    { 435607u, 1u, 9u, 14102u, 61u, 117382u },  // Thunder Blast
    { 436162u, 1u, 9u, 14700u, 61u, 117414u },  // Storm Bolts
    { 438590u, 1u, 10u, 15311u, 61u, 117395u },  // Keep Your Feet on the Ground
    { 1270723u, 1u, 9u, 15900u, 61u, 136069u },  // Conductivity
    { 437079u, 1u, 13u, 14102u, 61u, 117394u },  // Flashing Skies
    { 437134u, 1u, 18u, 15008u, 61u, 117402u },  // Avatar of the Storm
    { 437118u, 1u, 14u, 15300u, 61u, 117404u },  // Burst of Power
    { 1270724u, 1u, 14u, 15900u, 61u, 136068u },  // Capacitance
    { 436201u, 1u, 12u, 14697u, 61u, 117389u },  // Gathering Clouds
};
}

// TODO(UNVERIFIED): Tier is the trait tree row rank (tree 850 has 41 distinct rows).
// The game's own tier count comes from NumTalentsAtLevel, which is not yet matched to rows.
// TODO: Arms Colossus is not usable yet. Its wow.gg link decodes to Slayer nodes (subtree 60),
//   not Colossus (subtree 62). Re-export it with Colossus selected.
// FINDING: Fury Mountain Thane (subtree 61) is 13 hero points. Hero caps are not confirmed.
// FINDING: Slayer hero nodes (SubTree 60) are 10 nodes, each MaxRanks 1 (TraitNodeEntry).
//   The decoded strings put 13 ranks in hero-tagged entries, 3 of which (Unhinged 386628,
//   Deadly Focus 1270718, Violent Euphoria 1270717) are spec nodes. So Slayer = 10 points.
//   The calculator counter reads 4/4 and is not explained by node counts yet.
//   TODO(UNVERIFIED): CAP_HERO_TREE is set to 4 from the calculator counter, not the data.

#endif // FCBOTS_WARRIOR_TALENTS_H_H
