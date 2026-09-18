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
  
#include "PermissionMgr.h"  
#include "DatabaseEnv.h"  
#include "Log.h"  
#include "HousingEntities.h"  
  
PermissionMgr::PermissionMgr()  
{  
}  
  
PermissionMgr::~PermissionMgr()  
{  
    _housePermissions.clear();  
}  
  
void PermissionMgr::LoadPermissions()  
{  
    uint32 oldMSTime = getMSTime();  
  
    QueryResult result = CharacterDatabase.Query("SELECT permissionId, houseId, targetGuid, targetGuildId, permissionType, canEnterInterior, canEnterExterior, canEditInterior, canEditExterior, canCopyBlueprint, grantedByGuid FROM house_permissions");  
  
    if (!result)  
    {  
        TC_LOG_INFO("server.loading", ">> Loaded 0 house permissions. DB table `house_permissions` is empty.");  
        return;  
    }  
  
    uint32 count = 0;  
    do  
    {  
        Field* fields = result->Fetch();  
        uint32 permissionId       = fields[0].GetUInt32();  
        uint32 houseId            = fields[1].GetUInt32();  
        ObjectGuid targetGuid     = ObjectGuid::Create<HighGuid::Player>(fields[2].GetUInt64());  
        uint32 targetGuildId      = fields[3].GetUInt32();  
        uint8 permissionType      = fields[4].GetUInt8();  
        bool canEnterInterior     = fields[5].GetBool();  
        bool canEnterExterior     = fields[6].GetBool();  
        bool canEditInterior      = fields[7].GetBool();  
        bool canEditExterior      = fields[8].GetBool();  
        bool canCopyBlueprint     = fields[9].GetBool();  
        ObjectGuid grantedByGuid  = ObjectGuid::Create<HighGuid::Player>(fields[10].GetUInt64());  
  
        auto permission = std::make_unique<HousePermission>();  
        permission->SetId(permissionId);              // ADDED  
        permission->SetHouseId(houseId);  
        permission->SetTargetGuid(targetGuid);  
        permission->SetTargetGuildId(targetGuildId);  
        permission->SetPermissionType(permissionType);  
        permission->SetCanEnterInterior(canEnterInterior);  
        permission->SetCanEnterExterior(canEnterExterior);  
        permission->SetCanEditInterior(canEditInterior);  
        permission->SetCanEditExterior(canEditExterior);  
        permission->SetCanCopyBlueprint(canCopyBlueprint);  
        permission->SetGrantedByGuid(grantedByGuid);  // ADDED  
  
        _housePermissions[houseId].push_back(std::move(permission));  
        ++count;  
    } while (result->NextRow());  
  
    TC_LOG_INFO("server.loading", ">> Loaded {} house permissions in {} ms", count, GetMSTimeDiffToNow(oldMSTime));  
}  
  
bool PermissionMgr::SetHousePermission(ObjectGuid playerGuid, uint32 houseId, ObjectGuid targetGuid, uint8 permissionType, bool canEnterInterior, bool canEnterExterior, bool canEditInterior, bool canEditExterior)  
{  
    // TODO: Implement permission setting with database persistence  
    return false;  
}  
  
bool PermissionMgr::RemoveHousePermission(ObjectGuid playerGuid, uint32 houseId, ObjectGuid targetGuid)  
{  
    // TODO: Implement permission removal with database persistence  
    return false;  
}  
  
bool PermissionMgr::CanPlayerEnterHouse(ObjectGuid playerGuid, uint32 houseId, bool isInterior) const  
{  
    // TODO: Implement permission checking for house entry  
    return false;  
}  
  
bool PermissionMgr::CanPlayerEditHouse(ObjectGuid playerGuid, uint32 houseId, bool isInterior) const  
{  
    // TODO: Implement permission checking for house editing  
    return false;  
}