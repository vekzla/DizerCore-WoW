#include "FCBots_Accounts.h"

#include "AccountMgr.h"
#include "BattlenetAccountMgr.h"
#include "DatabaseEnv.h"
#include "Log.h"

#include <random>

namespace FCBots::Accounts
{
namespace
{
std::string RandomPassword(std::size_t length)
{
    static constexpr char const Alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> pick(0, sizeof(Alphabet) - 2);
    std::string out;
    out.reserve(length);
    for (std::size_t i = 0; i < length; ++i)
        out.push_back(Alphabet[pick(gen)]);
    return out;
}

// Game account linked to the Battle.net account with this email (LOGIN_SEL_BNET_GAME_ACCOUNT_LIST_SMALL).
bool FindLinkedGameAccount(std::string const& email, BotAccount& out)
{
    LoginDatabasePreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_BNET_GAME_ACCOUNT_LIST_SMALL);
    stmt->setString(0, email);
    PreparedQueryResult result = LoginDatabase.Query(stmt);
    if (!result)
        return false;

    Field* fields = result->Fetch();
    out.AccountId = fields[0].GetUInt32();
    out.Username = fields[1].GetString();
    return true;
}
}

std::vector<BotAccount> EnsureBotAccounts(std::string const& prefix, std::uint32_t count)
{
    std::vector<BotAccount> accounts;
    accounts.reserve(count);

    for (std::uint32_t i = 1; i <= count; ++i)
    {
        // Battle.net account (`.bnetaccount create fcbotN@fcbotN`) with its linked game account.
        std::string const name = prefix + std::to_string(i);
        std::string const email = name + "@" + name;

        if (Battlenet::AccountMgr::GetId(email) == 0)
        {
            std::string gameAccountName;
            AccountOpResult const result = Battlenet::AccountMgr::CreateBattlenetAccount(
                email, RandomPassword(16), true, &gameAccountName);
            if (result != AccountOpResult::AOR_OK)
            {
                TC_LOG_ERROR("scripts", "FCBots: could not create Battle.net account {} (result {})",
                    email, static_cast<int>(result));
                continue;
            }
            TC_LOG_INFO("scripts", "FCBots: created Battle.net account {} (game account {})", email, gameAccountName);
        }

        BotAccount account{};
        if (!FindLinkedGameAccount(email, account))
        {
            TC_LOG_ERROR("scripts", "FCBots: no game account linked to Battle.net account {}", email);
            continue;
        }
        accounts.push_back(account);
    }

    return accounts;
}
}
