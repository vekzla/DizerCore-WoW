#include "FCBots_Factory.h"

#include "CharacterCache.h"
#include "CharacterPackets.h"
#include "DB2Stores.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "SharedDefines.h"
#include "WorldSession.h"

#include <memory>
#include <string>

namespace FCBots::Factory
{
namespace
{
constexpr int MaxNameAttempts = 20;

// Random name from NameGen.db2 that passes the name rules and is not taken.
std::string PickFreeName(std::uint8_t race, std::uint8_t gender)
{
    for (int attempt = 0; attempt < MaxNameAttempts; ++attempt)
    {
        std::string candidate = sDB2Manager.GetNameGenEntry(race, gender);
        if (candidate.empty())
            continue;

        if (ObjectMgr::CheckPlayerName(candidate, LOCALE_enUS, true) != CHAR_NAME_SUCCESS)
            continue;

        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHECK_NAME);
        stmt->setString(0, candidate);
        if (PreparedQueryResult taken = CharacterDatabase.Query(stmt))
            continue;

        return candidate;
    }
    return std::string();
}
}

ObjectGuid::LowType CreateCharacter(WorldSession* session, std::uint32_t accountId,
    std::uint8_t race, std::uint8_t classId, std::uint8_t gender)
{
    if (!session)
        return 0;

    std::string const name = PickFreeName(race, gender);
    if (name.empty())
    {
        TC_LOG_ERROR("scripts", "FCBots: no free NameGen name for race {} gender {}", race, gender);
        return 0;
    }

    auto createInfo = std::make_shared<WorldPackets::Character::CharacterCreateInfo>();
    createInfo->Race = race;
    createInfo->Class = classId;
    createInfo->Sex = gender;
    createInfo->Name = name;
    createInfo->CharCount = 0;

    std::shared_ptr<Player> newChar(new Player(session), [](Player* ptr)
    {
        ptr->CleanupsBeforeDelete();
        delete ptr;
    });
    newChar->GetMotionMaster()->Initialize();

    ObjectGuid::LowType const guid = sObjectMgr->GetGenerator<HighGuid::Player>().Generate();
    if (!newChar->Create(guid, createInfo.get()))
    {
        TC_LOG_ERROR("scripts", "FCBots: Player::Create failed for {}", name);
        return 0;
    }

    newChar->setCinematic(1);
    newChar->SaveToDB(true);
    sCharacterCache->AddCharacterCacheEntry(newChar->GetGUID(), accountId, newChar->GetName(),
        newChar->GetNativeGender(), newChar->GetRace(), newChar->GetClass(), newChar->GetLevel(), false);

    TC_LOG_INFO("scripts", "FCBots: created character {} (guid {}) on account {}", name, guid, accountId);
    return guid;
}
}
