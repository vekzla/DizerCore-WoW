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

#ifndef TRINITYCORE_ROOM_MGR_H
#define TRINITYCORE_ROOM_MGR_H

#include "Define.h"
#include "ObjectGuid.h"
#include <map>
#include <memory>
#include <vector>

class TC_GAME_API RoomMgr
{
public:
    RoomMgr();
    ~RoomMgr();

    // Database loading
    void LoadRooms();

    // Room operations
    bool AddRoom(ObjectGuid playerGuid, uint32 houseId, uint32 roomTemplateId, float posX, float posY, uint8 orientation);
    bool RemoveRoom(ObjectGuid playerGuid, uint32 roomId);
    bool MoveRoom(ObjectGuid playerGuid, uint32 roomId, float posX, float posY, uint8 orientation);
    bool CustomizeRoom(ObjectGuid playerGuid, uint32 roomId, uint8 ceilingType, uint8 doorType, uint32 componentTheme);

private:
    std::map<uint32, std::unique_ptr<class Room>> _rooms;
};

#endif // TRINITYCORE_ROOM_MGR_H