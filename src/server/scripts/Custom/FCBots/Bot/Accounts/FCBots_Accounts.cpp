#include "FCBots_Accounts.h"

#include "AccountMgr.h"
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
}

std::vector<BotAccount> EnsureBotAccounts(std::string const& prefix, std::uint32_t count)
{
    std::vector<BotAccount> accounts;
    accounts.reserve(count);

    for (std::uint32_t i = 1; i <= count; ++i)
    {
        // Real game account in the auth database: username fcbotN, email fcbotN@fcbotN.
        std::string const name = prefix + std::to_string(i);
        std::string const email = name + "@" + name;

        std::uint32_t accountId = sAccountMgr->GetId(name);
        if (accountId == 0)
        {
            AccountOpResult const result = sAccountMgr->CreateAccount(name, RandomPassword(16), email);
            if (result != AccountOpResult::AOR_OK)
            {
                TC_LOG_ERROR("scripts", "FCBots: could not create account {} (result {})", name, static_cast<int>(result));
                continue;
            }
            accountId = sAccountMgr->GetId(name);
            TC_LOG_INFO("scripts", "FCBots: created account {} (id {})", name, accountId);
        }

        if (accountId == 0)
        {
            TC_LOG_ERROR("scripts", "FCBots: account {} not found after create", name);
            continue;
        }
        accounts.push_back(BotAccount{ accountId, name });
    }

    return accounts;
}
}
