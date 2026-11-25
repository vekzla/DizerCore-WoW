#ifndef FCBOTS_FACTORY_H
#define FCBOTS_FACTORY_H

#include <cstdint>

#include "ObjectGuid.h"

class WorldSession;

namespace FCBots::Factory
{
// Creates a character on the session's account. The name comes from NameGen.db2 (sDB2Manager),
// checked against the character name rules and the characters table.
// Returns the new character's low guid, or 0 on failure.
ObjectGuid::LowType CreateCharacter(WorldSession* session, std::uint32_t accountId,
    std::uint8_t race, std::uint8_t classId, std::uint8_t gender);
}

#endif // FCBOTS_FACTORY_H
