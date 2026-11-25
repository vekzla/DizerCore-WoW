#ifndef FCBOTS_ACCOUNTS_H
#define FCBOTS_ACCOUNTS_H

#include <cstdint>
#include <string>
#include <vector>

namespace FCBots::Accounts
{
struct BotAccount
{
    std::uint32_t AccountId;     // game account id (auth.account)
    std::string Username;        // e.g. "fcbot1"
};

// Creates real game accounts in the auth database for bots: fcbot1..fcbotN (email fcbotN@fcbotN).
// Existing accounts are reused, not recreated. Returns every bot account that exists after the call.
std::vector<BotAccount> EnsureBotAccounts(std::string const& prefix, std::uint32_t count);
}

#endif // FCBOTS_ACCOUNTS_H
