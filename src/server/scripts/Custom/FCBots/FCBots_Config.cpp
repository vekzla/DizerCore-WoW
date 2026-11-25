#include "FCBots_Config.h"

#include "Config.h"
#include "Log.h"

namespace FCBots::Config
{
Settings const& Get()
{
    static Settings const settings = []
    {
        // worldserver.conf holds one line: FCBots.ConfigFile = "fcbots.conf".
        // That file is loaded here, so FCBots keeps its own conf without the core's config.d folder.
        std::string const file = sConfigMgr->GetStringDefault("FCBots.ConfigFile", "fcbots.conf");
        std::string error;
        if (!sConfigMgr->LoadAdditionalFile(file, true, error))
            TC_LOG_ERROR("scripts", "FCBots: could not load config file {}: {}", file, error);

        Settings s;
        s.Enabled = sConfigMgr->GetBoolDefault("FCBots.Enabled", false);
        s.WarriorEnabled = sConfigMgr->GetBoolDefault("FCBots.Warrior.Enabled", false);
        s.AccountPrefix = sConfigMgr->GetStringDefault("FCBots.AccountPrefix", "fcbot");
        s.AccountPoolSize = sConfigMgr->GetIntDefault("FCBots.AccountPoolSize", 10);
        s.TickIntervalMs = sConfigMgr->GetIntDefault("FCBots.Tick.IntervalMs", 200);
        s.ClientBuild = sConfigMgr->GetIntDefault("FCBots.ClientBuild", 0);

        s.AutologinEnabled = sConfigMgr->GetBoolDefault("FCBots.Autologin.Enabled", false);
        s.AutologinCount = sConfigMgr->GetIntDefault("FCBots.Autologin.Count", 10);
        s.AutologinOnlyWithRealPlayers = sConfigMgr->GetBoolDefault("FCBots.Autologin.OnlyWithRealPlayers", false);
        s.AutologinLoginDelaySeconds = sConfigMgr->GetIntDefault("FCBots.Autologin.LoginDelaySeconds", 30);
        s.AutologinLogoutDelaySeconds = sConfigMgr->GetIntDefault("FCBots.Autologin.LogoutDelaySeconds", 300);
        return s;
    }();
    return settings;
}
}
