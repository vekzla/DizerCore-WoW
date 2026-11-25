#include "FCBots_Autologin.h"

#include "DatabaseEnv.h"
#include "Log.h"
#include "SharedDefines.h"
#include "WorldSession.h"

#include <algorithm>
#include <random>
#include <vector>

#include "../Accounts/FCBots_Accounts.h"
#include "../Factory/FCBots_Factory.h"
#include "../Login/FCBots_Login.h"
#include "../../FCBots_Config.h"
#include "../../Ai/Class/Warrior/Warrior_Bot.h"
#include "../../Ai/Class/Warrior/Warrior_Talents.h"

namespace FCBots::Autologin
{
namespace
{
// First character guid on the account, or 0 if it has none.
ObjectGuid::LowType FindCharacter(std::uint32_t accountId)
{
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARS_BY_ACCOUNT_ID);
    stmt->setUInt32(0, accountId);
    if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
        return result->Fetch()[0].GetUInt64();
    return 0;
}
}

void RunAutologin()
{
    FCBots::Config::Settings const& cfg = FCBots::Config::Get();
    if (!cfg.Enabled || !cfg.WarriorEnabled || !cfg.AutologinEnabled)
        return;

    std::vector<FCBots::Accounts::BotAccount> accounts =
        FCBots::Accounts::EnsureBotAccounts(cfg.AccountPrefix, static_cast<std::uint32_t>(cfg.AccountPoolSize));

    // Pick AutologinCount accounts at random from the pool.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(accounts.begin(), accounts.end(), gen);
    if (accounts.size() > static_cast<std::size_t>(cfg.AutologinCount))
        accounts.resize(static_cast<std::size_t>(cfg.AutologinCount));

    for (FCBots::Accounts::BotAccount const& account : accounts)
    {
        WorldSession* session = FCBots::Login::MakeBotSession(account.AccountId, account.Username);

        ObjectGuid::LowType guid = FindCharacter(account.AccountId);
        if (!guid)
            guid = FCBots::Factory::CreateCharacter(session, account.AccountId, RACE_HUMAN, CLASS_WARRIOR, GENDER_MALE);

        if (!guid || !FCBots::Login::StartBotSession(session, guid))
        {
            TC_LOG_ERROR("scripts", "FCBots: could not log in bot account {}", account.Username);
            delete session;
            continue;
        }

        FCBots::Class::Warrior::Engine::BotSetup setup{
            FCBots::Class::Warrior::Talents::Spec::Arms,
            FCBots::Class::Warrior::Talents::HeroTree::Slayer };
        FCBots::Class::Warrior::Bot::RegisterWarriorBot(ObjectGuid::Create<HighGuid::Player>(guid), setup);
        TC_LOG_INFO("scripts", "FCBots: logged in Human Warrior on account {}", account.Username);
    }
}
}
