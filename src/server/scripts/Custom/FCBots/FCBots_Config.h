#ifndef FCBOTS_FCBOTS_CONFIG_H_H
#define FCBOTS_FCBOTS_CONFIG_H_H

#include <cstdint>
#include <string>

// FCBots settings, read from worldserver.conf (see the FCBots block in worldserver.conf.dist).
namespace FCBots::Config
{
struct Settings
{
    bool Enabled = false;                       // FCBots.Enabled
    bool WarriorEnabled = false;                // FCBots.Warrior.Enabled
    std::string AccountPrefix = "fcbot";        // FCBots.AccountPrefix
    std::int32_t AccountPoolSize = 10;          // FCBots.AccountPoolSize
    std::int32_t TickIntervalMs = 200;          // FCBots.Tick.IntervalMs
    std::int32_t ClientBuild = 0;               // FCBots.ClientBuild (build the server runs)
    bool SummonAllowInCombat = true;            // FCBots.Summon.AllowInCombat
    bool SummonAllowWhenMasterDead = true;      // FCBots.Summon.AllowWhenMasterDead
    bool SummonAllowWhenBotDead = true;         // FCBots.Summon.AllowWhenBotDead
};

// Reads the settings once from sConfigMgr and returns them. Safe to call every tick.
Settings const& Get();

// Summon rules from the config. Returns true when a summon is allowed in the given state.
bool SummonAllowed(bool masterInCombat, bool masterDead, bool botDead);
}

#endif // FCBOTS_FCBOTS_CONFIG_H_H
