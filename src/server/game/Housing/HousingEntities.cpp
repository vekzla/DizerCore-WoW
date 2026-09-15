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

#include "HousingEntities.h"
#include "DatabaseEnv.h"
#include "DB2Stores.h"

// ============================================================================
// HOUSE ENTITY IMPLEMENTATION
// ============================================================================

House::House()
    : _houseId(0), _ownerGuid(), _plotId(0), _houseTemplateId(0), _houseType(0),
      _houseStyle(0), _houseLevel(1), _houseXP(0), _interiorDecorBudget(910),
      _roomPlacementBudget(19), _exteriorDecorBudget(200), _isActive(true)
{
}

House::~House()
{
}

bool House::AddXP(uint32 xp)
{
    _houseXP += xp;
    return CheckLevelUp();
}

bool House::CheckLevelUp()
{
    HouseLevelDataEntry const* levelData = sHouseLevelDataStore.LookupEntry(_houseLevel + 1);
    if (levelData && _houseXP >= levelData->RequiredXP)
    {
        _houseLevel++;
        UpdateBudgets();
        return true;
    }
    return false;
}

void House::UpdateBudgets()
{
        HouseLevelDataEntry const* levelData = sHouseLevelDataStore.LookupEntry(_houseLevel);
    if (levelData)
    {
        _interiorDecorBudget = levelData->InteriorDecorBudget;
        _roomPlacementBudget = levelData->RoomPlacementBudget;
        _exteriorDecorBudget = levelData->ExteriorDecorBudget;
    }
}

bool House::SaveToDB()
{
    // TODO: Implement database save
    return false;
}

bool House::LoadFromDB(uint32 houseId)
{
    // TODO: Implement database load
    return false;
}

// ============================================================================
// NEIGHBORHOOD ENTITY IMPLEMENTATION
// ============================================================================

Neighborhood::Neighborhood()
    : _neighborhoodId(0), _name(), _type(0), _faction(0), _guildId(0),
      _ownerAccountId(0), _isActive(true)
{
}

Neighborhood::~Neighborhood()
{
}

bool Neighborhood::SaveToDB()
{
    // TODO: Implement database save
    return false;
}

bool Neighborhood::LoadFromDB(uint32 neighborhoodId)
{
    // TODO: Implement database load
    return false;
}

// ============================================================================
// NEIGHBORHOOD PLOT ENTITY IMPLEMENTATION
// ============================================================================

NeighborhoodPlot::NeighborhoodPlot()
    : _plotId(0), _neighborhoodId(0), _plotIndex(0), _ownerGuid(), _houseId(0),
      _isReserved(false), _reservedByAccountId(0), _positionX(0.0f), _positionY(0.0f),
      _positionZ(0.0f), _orientation(0.0f), _isActive(true)
{
}

NeighborhoodPlot::~NeighborhoodPlot()
{
}

bool NeighborhoodPlot::SaveToDB()
{
    // TODO: Implement database save
    return false;
}

bool NeighborhoodPlot::LoadFromDB(uint32 plotId)
{
    // TODO: Implement database load
    return false;
}

// ============================================================================
// ROOM ENTITY IMPLEMENTATION
// ============================================================================

Room::Room()
    : _roomId(0), _houseId(0), _roomTemplateId(0), _positionX(0.0f), _positionY(0.0f),
      _orientation(0), _ceilingType(0), _doorType(0), _componentTheme(0), _isActive(true)
{
}

Room::~Room()
{
}

bool Room::SaveToDB()
{
    // TODO: Implement database save
    return false;
}

bool Room::LoadFromDB(uint32 roomId)
{
    // TODO: Implement database load
    return false;
}

// ============================================================================
// PLACED DECOR ENTITY IMPLEMENTATION
// ============================================================================

PlacedDecor::PlacedDecor()
    : _placedDecorId(0), _houseId(0), _roomId(0), _decorTemplateId(0),
      _positionX(0.0f), _positionY(0.0f), _positionZ(0.0f),
      _orientationX(0.0f), _orientationY(0.0f), _orientationZ(0.0f),
      _scale(1.0f), _isExterior(false), _isLocked(false),
      _dyeSlot1(0), _dyeSlot2(0), _dyeSlot3(0), _placementCost(1)
{
}

PlacedDecor::~PlacedDecor()
{
}

bool PlacedDecor::SaveToDB()
{
    // TODO: Implement database save
    return false;
}

bool PlacedDecor::LoadFromDB(uint32 placedDecorId)
{
    // TODO: Implement database load
    return false;
}

// ============================================================================
// HOUSE PERMISSION ENTITY IMPLEMENTATION
// ============================================================================

HousePermission::HousePermission()
    : _permissionId(0), _houseId(0), _targetGuid(), _targetGuildId(0),
      _permissionType(0), _canEnterInterior(false), _canEnterExterior(false),
      _canEditInterior(false), _canEditExterior(false), _canCopyBlueprint(false),
      _grantedByGuid()
{
}

HousePermission::~HousePermission()
{
}

bool HousePermission::SaveToDB()
{
    // TODO: Implement database save
    return false;
}

bool HousePermission::LoadFromDB(uint32 permissionId)
{
    // TODO: Implement database load
    return false;
}