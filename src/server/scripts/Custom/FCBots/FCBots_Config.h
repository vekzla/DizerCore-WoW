#ifndef FCBOTS_FCBOTS_CONFIG_H_H
#define FCBOTS_FCBOTS_CONFIG_H_H

#include <cstdint>
#include <string>

// FCBots settings, read from fcbots.conf (see APPLY_MANUALLY/fcbots.conf.dist).
// worldserver loads every *.conf in its config folder, so no include line is needed in worldserver.conf.
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

    bool AutologinEnabled = false;              // FCBots.Autologin.Enabled
    std::int32_t AutologinCount = 10;           // FCBots.Autologin.Count (random warriors picked from bot_names)
    bool AutologinOnlyWithRealPlayers = false;  // FCBots.Autologin.OnlyWithRealPlayers
    std::int32_t AutologinLoginDelaySeconds = 30;   // FCBots.Autologin.LoginDelaySeconds
    std::int32_t AutologinLogoutDelaySeconds = 300; // FCBots.Autologin.LogoutDelaySeconds
};

// Reads the settings once from sConfigMgr and returns them. Safe to call every tick.
Settings const& Get();
}

#endif // FCBOTS_FCBOTS_CONFIG_H_H
