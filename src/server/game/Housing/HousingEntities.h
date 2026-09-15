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

#ifndef TRINITYCORE_HOUSING_ENTITIES_H
#define TRINITYCORE_HOUSING_ENTITIES_H

#include "Define.h"
#include "ObjectGuid.h"
#include "DB2Structure.h"
#include <string>
#include <vector>

// ============================================================================
// HOUSE ENTITY
// ============================================================================

class TC_GAME_API House
{
public:
    House();
    ~House();

    // Getters
    uint32 GetId() const { return _houseId; }
    ObjectGuid GetOwnerGuid() const { return _ownerGuid; }
    uint32 GetPlotId() const { return _plotId; }
    uint32 GetHouseTemplateId() const { return _houseTemplateId; }
    uint8 GetHouseType() const { return _houseType; }
    uint8 GetHouseStyle() const { return _houseStyle; }
    uint8 GetLevel() const { return _houseLevel; }
    uint32 GetXP() const { return _houseXP; }
    uint32 GetInteriorDecorBudget() const { return _interiorDecorBudget; }
    uint32 GetRoomPlacementBudget() const { return _roomPlacementBudget; }
    uint32 GetExteriorDecorBudget() const { return _exteriorDecorBudget; }
    bool IsActive() const { return _isActive; }

    // Setters
    void SetPlotId(uint32 plotId) { _plotId = plotId; }
    void SetHouseLevel(uint8 level) { _houseLevel = level; }
    void SetHouseXP(uint32 xp) { _houseXP = xp; }
    void SetInteriorDecorBudget(uint32 budget) { _interiorDecorBudget = budget; }
    void SetRoomPlacementBudget(uint32 budget) { _roomPlacementBudget = budget; }
    void SetExteriorDecorBudget(uint32 budget) { _exteriorDecorBudget = budget; }
    void SetActive(bool active) { _isActive = active; }

    // Operations
    bool AddXP(uint32 xp);
    bool CheckLevelUp();
    void UpdateBudgets();

    // Database operations
    bool SaveToDB();
    bool LoadFromDB(uint32 houseId);

private:
    uint32 _houseId;
    ObjectGuid _ownerGuid;
    uint32 _plotId;
    uint32 _houseTemplateId;
    uint8 _houseType;           // 0=Small, 1=Medium, 2=Large
    uint8 _houseStyle;          // Faction-specific style
    uint8 _houseLevel;          // 1-10
    uint32 _houseXP;
    uint32 _interiorDecorBudget;
    uint32 _roomPlacementBudget;
    uint32 _exteriorDecorBudget;
    bool _isActive;
};

// ============================================================================
// NEIGHBORHOOD ENTITY
// ============================================================================

class TC_GAME_API Neighborhood
{
public:
    Neighborhood();
    ~Neighborhood();

    // Getters
    uint32 GetId() const { return _neighborhoodId; }
    std::string const& GetName() const { return _name; }
    uint8 GetType() const { return _type; }        // 0=Public, 1=Guild, 2=Private
    uint8 GetFaction() const { return _faction; }  // 0=Alliance, 1=Horde
    uint32 GetGuildId() const { return _guildId; }
    uint32 GetOwnerAccountId() const { return _ownerAccountId; }
    bool IsActive() const { return _isActive; }

    // Setters
    void SetName(std::string const& name) { _name = name; }
    void SetType(uint8 type) { _type = type; }
    void SetFaction(uint8 faction) { _faction = faction; }
    void SetGuildId(uint32 guildId) { _guildId = guildId; }
    void SetOwnerAccountId(uint32 accountId) { _ownerAccountId = accountId; }
    void SetActive(bool active) { _isActive = active; }

    // Database operations
    bool SaveToDB();
    bool LoadFromDB(uint32 neighborhoodId);

private:
    uint32 _neighborhoodId;
    std::string _name;
    uint8 _type;
    uint8 _faction;
    uint32 _guildId;
    uint32 _ownerAccountId;
    bool _isActive;
};

// ============================================================================
// NEIGHBORHOOD PLOT ENTITY
// ============================================================================

class TC_GAME_API NeighborhoodPlot
{
public:
    NeighborhoodPlot();
    ~NeighborhoodPlot();

    // Getters
    uint32 GetId() const { return _plotId; }
    uint32 GetNeighborhoodId() const { return _neighborhoodId; }
    uint8 GetPlotIndex() const { return _plotIndex; }
    ObjectGuid GetOwnerGuid() const { return _ownerGuid; }
    uint32 GetHouseId() const { return _houseId; }
    bool IsReserved() const { return _isReserved; }
    uint32 GetReservedByAccountId() const { return _reservedByAccountId; }
    bool IsActive() const { return _isActive; }

    // Position data
    float GetPositionX() const { return _positionX; }
    float GetPositionY() const { return _positionY; }
    float GetPositionZ() const { return _positionZ; }
    float GetOrientation() const { return _orientation; }

    // Setters
    void SetOwnerGuid(ObjectGuid guid) { _ownerGuid = guid; }
    void SetHouseId(uint32 houseId) { _houseId = houseId; }
    void SetReserved(bool reserved) { _isReserved = reserved; }
    void SetReservedByAccountId(uint32 accountId) { _reservedByAccountId = accountId; }
    void SetActive(bool active) { _isActive = active; }

    // Database operations
    bool SaveToDB();
    bool LoadFromDB(uint32 plotId);

private:
    uint32 _plotId;
    uint32 _neighborhoodId;
    uint8 _plotIndex;           // 0-54
    ObjectGuid _ownerGuid;
    uint32 _houseId;
    bool _isReserved;
    uint32 _reservedByAccountId;
    float _positionX;
    float _positionY;
    float _positionZ;
    float _orientation;
    bool _isActive;
};

// ============================================================================
// ROOM ENTITY
// ============================================================================

class TC_GAME_API Room
{
public:
    Room();
    ~Room();

    // Getters
    uint32 GetId() const { return _roomId; }
    uint32 GetHouseId() const { return _houseId; }
    uint32 GetRoomTemplateId() const { return _roomTemplateId; }
    float GetPositionX() const { return _positionX; }
    float GetPositionY() const { return _positionY; }
    uint8 GetOrientation() const { return _orientation; }
    uint8 GetCeilingType() const { return _ceilingType; }
    uint8 GetDoorType() const { return _doorType; }
    uint32 GetComponentTheme() const { return _componentTheme; }
    bool IsActive() const { return _isActive; }

    // Setters
    void SetHouseId(uint32 houseId) { _houseId = houseId; }
    void SetRoomTemplateId(uint32 templateId) { _roomTemplateId = templateId; }
    void SetPositionX(float x) { _positionX = x; }
    void SetPositionY(float y) { _positionY = y; }
    void SetOrientation(uint8 orientation) { _orientation = orientation; }
    void SetCeilingType(uint8 type) { _ceilingType = type; }
    void SetDoorType(uint8 type) { _doorType = type; }
    void SetComponentTheme(uint32 theme) { _componentTheme = theme; }
    void SetActive(bool active) { _isActive = active; }

    // Database operations
    bool SaveToDB();
    bool LoadFromDB(uint32 roomId);

private:
    uint32 _roomId;
    uint32 _houseId;
    uint32 _roomTemplateId;
    float _positionX;
    float _positionY;
    uint8 _orientation;       // 0-3
    uint8 _ceilingType;
    uint8 _doorType;
    uint32 _componentTheme;
    bool _isActive;
};

// ============================================================================
// PLACED DECOR ENTITY
// ============================================================================

class TC_GAME_API PlacedDecor
{
public:
    PlacedDecor();
    ~PlacedDecor();

    // Getters
    uint32 GetId() const { return _placedDecorId; }
    uint32 GetHouseId() const { return _houseId; }
    uint32 GetRoomId() const { return _roomId; }
    uint32 GetDecorTemplateId() const { return _decorTemplateId; }
    float GetPositionX() const { return _positionX; }
    float GetPositionY() const { return _positionY; }
    float GetPositionZ() const { return _positionZ; }
    float GetOrientationX() const { return _orientationX; }
    float GetOrientationY() const { return _orientationY; }
    float GetOrientationZ() const { return _orientationZ; }
    float GetScale() const { return _scale; }
    bool IsExterior() const { return _isExterior; }
    bool IsLocked() const { return _isLocked; }
    uint32 GetPlacementCost() const { return _placementCost; }

    // Setters
    void SetHouseId(uint32 houseId) { _houseId = houseId; }
    void SetRoomId(uint32 roomId) { _roomId = roomId; }
    void SetPosition(float x, float y, float z) { _positionX = x; _positionY = y; _positionZ = z; }
    void SetOrientation(float x, float y, float z) { _orientationX = x; _orientationY = y; _orientationZ = z; }
    void SetScale(float scale) { _scale = scale; }
    void SetExterior(bool exterior) { _isExterior = exterior; }
    void SetLocked(bool locked) { _isLocked = locked; }

    // Database operations
    bool SaveToDB();
    bool LoadFromDB(uint32 placedDecorId);

private:
    uint32 _placedDecorId;
    uint32 _houseId;
    uint32 _roomId;
    uint32 _decorTemplateId;
    float _positionX;
    float _positionY;
    float _positionZ;
    float _orientationX;
    float _orientationY;
    float _orientationZ;
    float _scale;
    bool _isExterior;
    bool _isLocked;
    uint32 _dyeSlot1;
    uint32 _dyeSlot2;
    uint32 _dyeSlot3;
    uint32 _placementCost;
};

// ============================================================================
// HOUSE PERMISSION ENTITY
// ============================================================================

class TC_GAME_API HousePermission
{
public:
    HousePermission();
    ~HousePermission();

    // Getters
    uint32 GetId() const { return _permissionId; }
    uint32 GetHouseId() const { return _houseId; }
    ObjectGuid GetTargetGuid() const { return _targetGuid; }
    uint32 GetTargetGuildId() const { return _targetGuildId; }
    uint8 GetPermissionType() const { return _permissionType; }
    bool CanEnterInterior() const { return _canEnterInterior; }
    bool CanEnterExterior() const { return _canEnterExterior; }
    bool CanEditInterior() const { return _canEditInterior; }
    bool CanEditExterior() const { return _canEditExterior; }
    bool CanCopyBlueprint() const { return _canCopyBlueprint; }

    // Setters
    void SetHouseId(uint32 houseId) { _houseId = houseId; }
    void SetTargetGuid(ObjectGuid guid) { _targetGuid = guid; }
    void SetTargetGuildId(uint32 guildId) { _targetGuildId = guildId; }
    void SetPermissionType(uint8 type) { _permissionType = type; }
    void SetCanEnterInterior(bool can) { _canEnterInterior = can; }
    void SetCanEnterExterior(bool can) { _canEnterExterior = can; }
    void SetCanEditInterior(bool can) { _canEditInterior = can; }
    void SetCanEditExterior(bool can) { _canEditExterior = can; }
    void SetCanCopyBlueprint(bool can) { _canCopyBlueprint = can; }

    // Database operations
    bool SaveToDB();
    bool LoadFromDB(uint32 permissionId);

private:
    uint32 _permissionId;
    uint32 _houseId;
    ObjectGuid _targetGuid;
    uint32 _targetGuildId;
    uint8 _permissionType;      // 0=Neighbor, 1=Guild, 2=Party, 3=Friend, 4=Specific
    bool _canEnterInterior;
    bool _canEnterExterior;
    bool _canEditInterior;
    bool _canEditExterior;
    bool _canCopyBlueprint;
    ObjectGuid _grantedByGuid;
};

#endif // TRINITYCORE_HOUSING_ENTITIES_H