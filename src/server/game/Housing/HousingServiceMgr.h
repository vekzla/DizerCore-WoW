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

#ifndef TRINITYCORE_HOUSING_SERVICE_MGR_H
#define TRINITYCORE_HOUSING_SERVICE_MGR_H

#include "Define.h"
#include "ObjectGuid.h"
#include "Optional.h"
#include <map>
#include <memory>
#include <vector>

class Player;
class WorldSession;
class House;
class Neighborhood;
class NeighborhoodPlot;

// Forward declarations for housing managers
class HouseMgr;
class NeighborhoodMgr;
class DecorMgr;
class RoomMgr;
class PermissionMgr;

class TC_GAME_API HousingServiceMgr
{
private:
    HousingServiceMgr();
    ~HousingServiceMgr();

public:
    HousingServiceMgr(HousingServiceMgr const&) = delete;
    HousingServiceMgr(HousingServiceMgr&&) = delete;
    HousingServiceMgr& operator=(HousingServiceMgr const&) = delete;
    HousingServiceMgr& operator=(HousingServiceMgr&&) = delete;

    static HousingServiceMgr* instance();

    // Initialization
    void Initialize();
    void LoadFromDB();

    // Player house operations
    std::vector<House*> GetPlayerHouses(ObjectGuid playerGuid) const;
    House* GetPlayerHouse(ObjectGuid playerGuid, uint32 houseId) const;
    House* GetActiveHouse(ObjectGuid playerGuid) const;
    bool CreateHouse(Player* player, uint32 plotId, uint32 houseTemplateId);
    bool RelinquishHouse(ObjectGuid playerGuid, uint32 houseId);
    bool MoveHouse(ObjectGuid playerGuid, uint32 houseId, uint32 newPlotId);

    // Neighborhood operations
    Neighborhood* GetNeighborhood(uint32 neighborhoodId) const;
    std::vector<Neighborhood*> GetNeighborhoods(uint8 faction) const;
    NeighborhoodPlot* GetPlot(uint32 plotId) const;
    std::vector<NeighborhoodPlot*> GetAvailablePlots(uint32 neighborhoodId) const;
    bool ReservePlot(ObjectGuid playerGuid, uint32 plotId);
    bool ClearPlotReservation(uint32 plotId);

    // Decor operations
    bool AddDecorToCollection(ObjectGuid playerGuid, uint32 decorTemplateId, uint32 quantity = 1);
    bool RemoveDecorFromCollection(ObjectGuid playerGuid, uint32 decorTemplateId, uint32 quantity = 1);
    std::vector<uint32> GetPlayerDecorCollection(ObjectGuid playerGuid) const;
    bool PlaceDecor(ObjectGuid playerGuid, uint32 houseId, uint32 decorTemplateId, float posX, float posY, float posZ, float orientation);
    bool RemoveDecor(ObjectGuid playerGuid, uint32 placedDecorId);
    bool MoveDecor(ObjectGuid playerGuid, uint32 placedDecorId, float posX, float posY, float posZ, float orientation);

    // Room operations
    bool AddRoom(ObjectGuid playerGuid, uint32 houseId, uint32 roomTemplateId, float posX, float posY, uint8 orientation);
    bool RemoveRoom(ObjectGuid playerGuid, uint32 roomId);
    bool MoveRoom(ObjectGuid playerGuid, uint32 roomId, float posX, float posY, uint8 orientation);
    bool CustomizeRoom(ObjectGuid playerGuid, uint32 roomId, uint8 ceilingType, uint8 doorType, uint32 componentTheme);

    // House leveling
    bool AddHouseXP(ObjectGuid playerGuid, uint32 houseId, uint32 xp);
    bool UpgradeHouse(ObjectGuid playerGuid, uint32 houseId);
    uint32 GetHouseLevel(uint32 houseId) const;
    uint32 GetHouseXP(uint32 houseId) const;
    uint32 GetInteriorDecorBudget(uint32 houseId) const;
    uint32 GetRoomPlacementBudget(uint32 houseId) const;
    uint32 GetExteriorDecorBudget(uint32 houseId) const;

    // Permission operations
    bool SetHousePermission(ObjectGuid playerGuid, uint32 houseId, ObjectGuid targetGuid, uint8 permissionType, bool canEnterInterior, bool canEnterExterior, bool canEditInterior, bool canEditExterior);
    bool RemoveHousePermission(ObjectGuid playerGuid, uint32 houseId, ObjectGuid targetGuid);
    bool CanPlayerEnterHouse(ObjectGuid playerGuid, uint32 houseId, bool isInterior) const;
    bool CanPlayerEditHouse(ObjectGuid playerGuid, uint32 houseId, bool isInterior) const;

    // Blueprint operations
    bool SaveBlueprint(ObjectGuid playerGuid, uint32 houseId, std::string const& name, std::string const& description);
    bool LoadBlueprint(ObjectGuid playerGuid, uint32 blueprintId, uint32 targetHouseId);
    bool DeleteBlueprint(ObjectGuid playerGuid, uint32 blueprintId);
    std::vector<uint32> GetPlayerBlueprints(ObjectGuid playerGuid) const;

    // Access to sub-managers
    HouseMgr* GetHouseMgr() const { return _houseMgr; }
    NeighborhoodMgr* GetNeighborhoodMgr() const { return _neighborhoodMgr; }
    DecorMgr* GetDecorMgr() const { return _decorMgr; }
    RoomMgr* GetRoomMgr() const { return _roomMgr; }
    PermissionMgr* GetPermissionMgr() const { return _permissionMgr; }

private:
    // Sub-managers
    HouseMgr* _houseMgr;
    NeighborhoodMgr* _neighborhoodMgr;
    DecorMgr* _decorMgr;
    RoomMgr* _roomMgr;
    PermissionMgr* _permissionMgr;

    // Caches
    std::map<uint32, std::unique_ptr<House>> _houses;
    std::map<uint32, std::unique_ptr<Neighborhood>> _neighborhoods;
    std::map<uint32, std::unique_ptr<NeighborhoodPlot>> _plots;
};

#define sHousingServiceMgr HousingServiceMgr::instance()

#endif // TRINITYCORE_HOUSING_SERVICE_MGR_H