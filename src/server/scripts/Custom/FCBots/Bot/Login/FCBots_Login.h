#ifndef FCBOTS_BOT_LOGIN_FCBOTS_LOGIN_H_H
#define FCBOTS_BOT_LOGIN_FCBOTS_LOGIN_H_H

#include <cstdint>
#include <string>

#include "ObjectGuid.h"

class WorldSession;

// Logs a bot character in on a headless session (no client socket).
// Requires the core setter WorldSession::SetBotLoginGuid (see the core patch note).
namespace FCBots::Login
{
// Returns the new session, or nullptr when the account is already online or the login cannot start.
WorldSession* LoginBot(std::uint32_t accountId, std::uint32_t battlenetAccountId,
    std::string const& accountName, ObjectGuid::LowType characterGuid);
}

#endif // FCBOTS_BOT_LOGIN_FCBOTS_LOGIN_H_H
