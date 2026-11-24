#include "FCBots_Config.h"

#include "Config.h"

namespace FCBots::Config
{
Settings const& Get()
{
    static Settings const settings = []
    {
        Settings s;
        s.Enabled = sConfigMgr->GetBoolDefault("FCBots.Enabled", false);
        s.WarriorEnabled = sConfigMgr->GetBoolDefault("FCBots.Warrior.Enabled", false);
        s.AccountPrefix = sConfigMgr->GetStringDefault("FCBots.AccountPrefix", "fcbot");
        s.AccountPoolSize = sConfigMgr->GetIntDefault("FCBots.AccountPoolSize", 10);
        s.TickIntervalMs = sConfigMgr->GetIntDefault("FCBots.Tick.IntervalMs", 200);
        s.ClientBuild = sConfigMgr->GetIntDefault("FCBots.ClientBuild", 0);
        s.SummonAllowInCombat = sConfigMgr->GetBoolDefault("FCBots.Summon.AllowInCombat", true);
        s.SummonAllowWhenMasterDead = sConfigMgr->GetBoolDefault("FCBots.Summon.AllowWhenMasterDead", true);
        s.SummonAllowWhenBotDead = sConfigMgr->GetBoolDefault("FCBots.Summon.AllowWhenBotDead", true);
        return s;
    }();
    return settings;
}

bool SummonAllowed(bool masterInCombat, bool masterDead, bool botDead)
{
    Settings const& s = Get();
    if (masterInCombat && !s.SummonAllowInCombat)
        return false;
    if (masterDead && !s.SummonAllowWhenMasterDead)
        return false;
    if (botDead && !s.SummonAllowWhenBotDead)
        return false;
    return true;
}
}
