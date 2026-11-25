// Periodic tick for FCBots Warrior bots. Runs from the world update.
// Bots are registered with RegisterWarriorBot by autologin at worldserver start (OnStartup).
#include <cstdint>
#include <vector>

#include "ObjectAccessor.h"
#include "Player.h"
#include "../../Bot/Autologin/FCBots_Autologin.h"
#include "FCBots_Config.h"
#include "ScriptMgr.h"
#include "Unit.h"
#include "Warrior_Bot.h"

namespace FCBots::Class::Warrior::Bot
{
namespace
{
struct RegisteredBot
{
    ObjectGuid Guid;
    Engine::BotSetup Setup;
    Engine::BotProgress Progress;
};

std::vector<RegisteredBot>& Registry()
{
    static std::vector<RegisteredBot> bots;
    return bots;
}

class FCBotsWarriorWorld : public WorldScript
{
public:
    FCBotsWarriorWorld() : WorldScript("fcbots_warrior_world") { }

    void OnStartup() override
    {
        FCBots::Autologin::RunAutologin();
    }

    void OnUpdate(uint32 diff) override
    {
        FCBots::Config::Settings const& cfg = FCBots::Config::Get();
        if (!cfg.Enabled || !cfg.WarriorEnabled)
            return;

        // Tick at FCBots.Tick.IntervalMs, not every world update.
        _elapsedMs += diff;
        if (_elapsedMs < static_cast<std::uint32_t>(cfg.TickIntervalMs))
            return;
        _elapsedMs = 0u;

        for (RegisteredBot& bot : Registry())
        {
            Player* player = ObjectAccessor::FindPlayer(bot.Guid);
            if (!player || !player->IsAlive())
                continue;
            TickBot(player, player->GetVictim(), bot.Setup, bot.Progress);
        }
    }

private:
    std::uint32_t _elapsedMs = 0u;
};
}

void RegisterWarriorBot(ObjectGuid guid, Engine::BotSetup const& setup)
{
    Registry().push_back(RegisteredBot{ guid, setup, Engine::BotProgress{} });
}
}

void AddSC_fcbots_warrior_world()
{
    new FCBots::Class::Warrior::Bot::FCBotsWarriorWorld();
}
