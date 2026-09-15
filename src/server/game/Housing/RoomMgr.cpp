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

#include "RoomMgr.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "HousingEntities.h"

RoomMgr::RoomMgr()
{
}

RoomMgr::~RoomMgr()
{
    _rooms.clear();
}

void RoomMgr::LoadRooms()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = CharacterDatabase.Query("SELECT roomId, houseId, roomTemplateId, positionX, positionY, orientation, ceilingType, doorType, componentTheme, isActive FROM house_rooms");

    if (!result)
    {
        LOG_INFO("server.loading", ">> Loaded 0 rooms. DB table `house_rooms` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        uint32 roomId = fields[0].Get<uint32>();
        uint32 houseId = fields[1].Get<uint32>();
        uint32 roomTemplateId = fields[2].Get<uint32>();
        float positionX = fields[3].Get<float>();
        float positionY = fields[4].Get<float>();
        uint8 orientation = fields[5].Get<uint8>();
        uint8 ceilingType = fields[6].Get<uint8>();
        uint8 doorType = fields[7].Get<uint8>();
        uint32 componentTheme = fields[8].Get<uint32>();
        bool isActive = fields[9].Get<bool>();

        auto room = std::make_unique<Room>();
        room->SetHouseId(houseId);
        room->SetRoomTemplateId(roomTemplateId);
        room->SetPositionX(positionX);
        room->SetPositionY(positionY);
        room->SetOrientation(orientation);
        room->SetCeilingType(ceilingType);
        room->SetDoorType(doorType);
        room->SetComponentTheme(componentTheme);
        room->SetActive(isActive);

        _rooms[roomId] = std::move(room);
        ++count;
    } while (result->NextRow());

    LOG_INFO("server.loading", ">> Loaded {} rooms in {} ms", count, GetMSTimeDiffToNow(oldMSTime));
}

bool RoomMgr::AddRoom(ObjectGuid playerGuid, uint32 houseId, uint32 roomTemplateId, float posX, float posY, uint8 orientation)
{
    // TODO: Implement room addition with database persistence
    return false;
}

bool RoomMgr::RemoveRoom(ObjectGuid playerGuid, uint32 roomId)
{
    // TODO: Implement room removal with database persistence
    return false;
}

bool RoomMgr::MoveRoom(ObjectGuid playerGuid, uint32 roomId, float posX, float posY, uint8 orientation)
{
    // TODO: Implement room movement with database persistence
    return false;
}

bool RoomMgr::CustomizeRoom(ObjectGuid playerGuid, uint32 roomId, uint8 ceilingType, uint8 doorType, uint32 componentTheme)
{
    // TODO: Implement room customization with database persistence
    return false;
}