#ifndef FCBOTS_ACCOUNTS_H
#define FCBOTS_ACCOUNTS_H

#include <cstdint>
#include <string>
#include <vector>

namespace FCBots::Accounts
{
struct BotAccount
{
    std::uint32_t AccountId;  // game account id (auth.account)
    std::string Username;     // game account name, assigned by the core for a linked Battle.net account
};

// Creates Battle.net accounts (email fcbotN@fcbotN) with linked game accounts for bots, fcbot1..fcbotN.
// Existing accounts are reused. Returns every bot game account that exists after the call.
std::vector<BotAccount> EnsureBotAccounts(std::string const& prefix, std::uint32_t count);
}

#endif // FCBOTS_ACCOUNTS_H
