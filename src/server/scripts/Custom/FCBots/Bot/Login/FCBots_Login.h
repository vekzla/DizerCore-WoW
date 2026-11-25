#ifndef FCBOTS_LOGIN_H
#define FCBOTS_LOGIN_H

#include <cstdint>
#include <string>

#include "ObjectGuid.h"

class WorldSession;

namespace FCBots::Login
{
// Builds a session for a bot account. The session is NOT added to the world yet.
WorldSession* MakeBotSession(std::uint32_t accountId, std::string const& accountName);

// Adds the session to the world and logs in the given character on it.
// Returns false if the account is already online or the session is invalid.
bool StartBotSession(WorldSession* session, ObjectGuid::LowType characterGuid);
}

#endif // FCBOTS_LOGIN_H
