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

#ifndef TRINITYCORE_PERMISSION_MGR_H
#define TRINITYCORE_PERMISSION_MGR_H

#include "Define.h"
#include "ObjectGuid.h"
#include <map>
#include <memory>
#include <vector>

class TC_GAME_API PermissionMgr
{
public:
    PermissionMgr();
    ~PermissionMgr();

    // Database loading
    void LoadPermissions();

    // Permission operations
    bool SetHousePermission(ObjectGuid playerGuid, uint32 houseId, ObjectGuid targetGuid, uint8 permissionType, bool canEnterInterior, bool canEnterExterior, bool canEditInterior, bool canEditExterior);
    bool RemoveHousePermission(ObjectGuid playerGuid, uint32 houseId, ObjectGuid targetGuid);
    bool CanPlayerEnterHouse(ObjectGuid playerGuid, uint32 houseId, bool isInterior) const;
    bool CanPlayerEditHouse(ObjectGuid playerGuid, uint32 houseId, bool isInterior) const;

private:
    std::map<uint32, std::vector<std::unique_ptr<class HousePermission>>> _housePermissions;
};

#endif // TRINITYCORE_PERMISSION_MGR_H