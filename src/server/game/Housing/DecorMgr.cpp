/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "DecorMgr.h"
#include "DatabaseEnv.h"
#include "Log.h"

DecorMgr::DecorMgr()
{
}

DecorMgr::~DecorMgr()
{
    _decorCollections.clear();
}

void DecorMgr::LoadDecorCollections()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = CharacterDatabase.Query("SELECT ownerGuid, decorTemplateId, quantity FROM player_decor_collection");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 decor collections. DB table `player_decor_collection` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        ObjectGuid ownerGuid    = ObjectGuid::Create<HighGuid::Player>(fields[0].GetUInt64());  
		uint32 decorTemplateId  = fields[1].GetUInt32();  
		uint32 quantity         = fields[2].GetUInt32();
        _decorCollections[ownerGuid][decorTemplateId] = quantity;
        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded {} decor collection entries in {} ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void DecorMgr::LoadPlacedDecor()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = CharacterDatabase.Query("SELECT placedDecorId, houseId, roomId, decorTemplateId, positionX, positionY, positionZ, orientationX, orientationY, orientationZ, scale, isExterior, isLocked, placementCost FROM placed_decor");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 placed decor. DB table `placed_decor` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        // TODO: Store placed decor in memory for runtime access
        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded {} placed decor items in {} ms", count, GetMSTimeDiffToNow(oldMSTime));
}

bool DecorMgr::AddDecorToCollection(ObjectGuid playerGuid, uint32 decorTemplateId, uint32 quantity)
{
    // TODO: Implement decor addition with database persistence
    return false;
}

bool DecorMgr::RemoveDecorFromCollection(ObjectGuid playerGuid, uint32 decorTemplateId, uint32 quantity)
{
    // TODO: Implement decor removal with database persistence
    return false;
}

std::vector<uint32> DecorMgr::GetPlayerDecorCollection(ObjectGuid playerGuid) const
{
    std::vector<uint32> decorIds;
    auto it = _decorCollections.find(playerGuid);
    if (it != _decorCollections.end())
    {
        for (auto const& [decorId, quantity] : it->second)
        {
            decorIds.push_back(decorId);
        }
    }
    return decorIds;
}

bool DecorMgr::PlaceDecor(ObjectGuid playerGuid, uint32 houseId, uint32 decorTemplateId, float posX, float posY, float posZ, float orientation)
{
    // TODO: Implement decor placement with database persistence
    return false;
}

bool DecorMgr::RemoveDecor(ObjectGuid playerGuid, uint32 placedDecorId)
{
    // TODO: Implement decor removal with database persistence
    return false;
}

bool DecorMgr::MoveDecor(ObjectGuid playerGuid, uint32 placedDecorId, float posX, float posY, float posZ, float orientation)
{
    // TODO: Implement decor movement with database persistence
    return false;
}