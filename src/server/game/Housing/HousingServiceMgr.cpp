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
 
#include "HousingServiceMgr.h"
#include "HouseMgr.h"
#include "NeighborhoodMgr.h"
#include "DecorMgr.h"
#include "RoomMgr.h"
#include "PermissionMgr.h"
#include "HousingEntities.h"   // <- add: full House/Neighborhood/NeighborhoodPlot defs  
#include "DB2Stores.h"         // <- add: sHouseStore + HouseEntry  
#include "DatabaseEnv.h"
#include "Log.h"
#include "Player.h"
#include "World.h"

HousingServiceMgr::HousingServiceMgr()
    : _houseMgr(nullptr), _neighborhoodMgr(nullptr), _decorMgr(nullptr), _roomMgr(nullptr), _permissionMgr(nullptr)
{
}

HousingServiceMgr::~HousingServiceMgr()
{
    delete _houseMgr;
    delete _neighborhoodMgr;
    delete _decorMgr;
    delete _roomMgr;
    delete _permissionMgr;
}

HousingServiceMgr* HousingServiceMgr::instance()
{
    static HousingServiceMgr instance;
    return &instance;
}

void HousingServiceMgr::Initialize()
{
    TC_LOG_INFO("server.loading", "Loading Housing Service Manager...");

    // Initialize sub-managers
    _houseMgr = new HouseMgr();
    _neighborhoodMgr = new NeighborhoodMgr();
    _decorMgr = new DecorMgr();
    _roomMgr = new RoomMgr();
    _permissionMgr = new PermissionMgr();

    TC_LOG_INFO("server.loading", "Housing Service Manager initialized");
}

void HousingServiceMgr::LoadFromDB()
{
    TC_LOG_INFO("server.loading", "Loading housing data from database...");

    uint32 oldMSTime = getMSTime();

    // Load neighborhoods
    _neighborhoodMgr->LoadNeighborhoods();
    _neighborhoodMgr->LoadPlots();

    // Load houses
    _houseMgr->LoadHouses();

    // Load decor collections
    _decorMgr->LoadDecorCollections();

    // Load placed decor
    _decorMgr->LoadPlacedDecor();

    // Load rooms
    _roomMgr->LoadRooms();

    // Load permissions
    _permissionMgr->LoadPermissions();

    TC_LOG_INFO("server.loading", ">> Loaded housing data in {} ms", GetMSTimeDiffToNow(oldMSTime));
}

std::vector<House*> HousingServiceMgr::GetPlayerHouses(ObjectGuid playerGuid) const
{
    return _houseMgr->GetPlayerHouses(playerGuid);
}

House* HousingServiceMgr::GetPlayerHouse(ObjectGuid playerGuid, uint32 houseId) const
{
    return _houseMgr->GetPlayerHouse(playerGuid, houseId);
}

House* HousingServiceMgr::GetActiveHouse(ObjectGuid playerGuid) const
{
    return _houseMgr->GetActiveHouse(playerGuid);
}

bool HousingServiceMgr::CreateHouse(Player* player, uint32 plotId, uint32 houseTemplateId)  
{  
    if (!player)  
        return false;  
  
    // Check if plot is available  
    NeighborhoodPlot* plot = _neighborhoodMgr->GetPlot(plotId);  
    if (!plot || !plot->GetOwnerGuid().IsEmpty()) 
    {  
        LOG_ERROR("housing", "Plot {} is not available for house creation", plotId);  
        return false;  
    }  
  
    // Determine the faction of the target plot's neighborhood  
    Neighborhood* targetNeighborhood = _neighborhoodMgr->GetNeighborhood(plot->GetNeighborhoodId());  
    if (!targetNeighborhood)  
    {  
        LOG_ERROR("housing", "Plot {} has no valid neighborhood", plotId);  
        return false;  
    }  
    uint8 targetFaction = targetNeighborhood->GetFaction();  
    
	// Strict rule: player may only create a house matching their own faction  
    if (targetFaction != static_cast<uint8>(player->GetTeamId()))  
    {  
        LOG_ERROR("housing", "Player {} (team {}) cannot create a house in faction {} neighborhood",  
            player->GetGUID().ToString(), static_cast<uint8>(player->GetTeamId()), targetFaction);  
        return false;  
    }
	
    // Enforce one house per faction: reject if the player already owns a  
    // house on a plot belonging to a same-faction neighborhood  
    for (House* existing : GetPlayerHouses(player->GetGUID()))  
    {  
        NeighborhoodPlot* existingPlot = _neighborhoodMgr->GetPlot(existing->GetPlotId());  
        if (!existingPlot)  
            continue;  
  
        Neighborhood* existingNeighborhood = _neighborhoodMgr->GetNeighborhood(existingPlot->GetNeighborhoodId());  
        if (existingNeighborhood && existingNeighborhood->GetFaction() == targetFaction)  
        {  
            LOG_ERROR("housing", "Player {} already owns a house for faction {}",  
                player->GetGUID().ToString(), targetFaction);  
            return false;  
        }  
    }  
  
    // Check house template validity  
    HouseEntry const* houseTemplate = sHouseStore->LookupEntry(houseTemplateId);
    if (!houseTemplate)  
    {  
        LOG_ERROR("housing", "Invalid house template ID {}", houseTemplateId);  
        return false;  
    }  
  
    // Create the house  
    return _houseMgr->CreateHouse(player->GetGUID(), plotId, houseTemplateId);  
}

bool HousingServiceMgr::RelinquishHouse(ObjectGuid playerGuid, uint32 houseId)
{
    return _houseMgr->RelinquishHouse(playerGuid, houseId);
}

bool HousingServiceMgr::MoveHouse(ObjectGuid playerGuid, uint32 houseId, uint32 newPlotId)
{
    House* house = GetPlayerHouse(playerGuid, houseId);
    if (!house)
        return false;

    NeighborhoodPlot* newPlot = _neighborhoodMgr->GetPlot(newPlotId);
    if (!newPlot || newPlot->GetOwnerGuid())
        return false;

    return _houseMgr->MoveHouse(houseId, newPlotId);
}

Neighborhood* HousingServiceMgr::GetNeighborhood(uint32 neighborhoodId) const
{
    return _neighborhoodMgr->GetNeighborhood(neighborhoodId);
}

std::vector<Neighborhood*> HousingServiceMgr::GetNeighborhoods(uint8 faction) const
{
    return _neighborhoodMgr->GetNeighborhoods(faction);
}

NeighborhoodPlot* HousingServiceMgr::GetPlot(uint32 plotId) const
{
    return _neighborhoodMgr->GetPlot(plotId);
}

std::vector<NeighborhoodPlot*> HousingServiceMgr::GetAvailablePlots(uint32 neighborhoodId) const
{
    return _neighborhoodMgr->GetAvailablePlots(neighborhoodId);
}

bool HousingServiceMgr::ReservePlot(ObjectGuid playerGuid, uint32 plotId)
{
    return _neighborhoodMgr->ReservePlot(playerGuid, plotId);
}

bool HousingServiceMgr::ClearPlotReservation(uint32 plotId)
{
    return _neighborhoodMgr->ClearPlotReservation(plotId);
}

bool HousingServiceMgr::AddDecorToCollection(ObjectGuid playerGuid, uint32 decorTemplateId, uint32 quantity)
{
    return _decorMgr->AddDecorToCollection(playerGuid, decorTemplateId, quantity);
}

bool HousingServiceMgr::RemoveDecorFromCollection(ObjectGuid playerGuid, uint32 decorTemplateId, uint32 quantity)
{
    return _decorMgr->RemoveDecorFromCollection(playerGuid, decorTemplateId, quantity);
}

std::vector<uint32> HousingServiceMgr::GetPlayerDecorCollection(ObjectGuid playerGuid) const
{
    return _decorMgr->GetPlayerDecorCollection(playerGuid);
}

bool HousingServiceMgr::PlaceDecor(ObjectGuid playerGuid, uint32 houseId, uint32 decorTemplateId, float posX, float posY, float posZ, float orientation)
{
    return _decorMgr->PlaceDecor(playerGuid, houseId, decorTemplateId, posX, posY, posZ, orientation);
}

bool HousingServiceMgr::RemoveDecor(ObjectGuid playerGuid, uint32 placedDecorId)
{
    return _decorMgr->RemoveDecor(playerGuid, placedDecorId);
}

bool HousingServiceMgr::MoveDecor(ObjectGuid playerGuid, uint32 placedDecorId, float posX, float posY, float posZ, float orientation)
{
    return _decorMgr->MoveDecor(playerGuid, placedDecorId, posX, posY, posZ, orientation);
}

bool HousingServiceMgr::AddRoom(ObjectGuid playerGuid, uint32 houseId, uint32 roomTemplateId, float posX, float posY, uint8 orientation)
{
    return _roomMgr->AddRoom(playerGuid, houseId, roomTemplateId, posX, posY, orientation);
}

bool HousingServiceMgr::RemoveRoom(ObjectGuid playerGuid, uint32 roomId)
{
    return _roomMgr->RemoveRoom(playerGuid, roomId);
}

bool HousingServiceMgr::MoveRoom(ObjectGuid playerGuid, uint32 roomId, float posX, float posY, uint8 orientation)
{
    return _roomMgr->MoveRoom(playerGuid, roomId, posX, posY, orientation);
}

bool HousingServiceMgr::CustomizeRoom(ObjectGuid playerGuid, uint32 roomId, uint8 ceilingType, uint8 doorType, uint32 componentTheme)
{
    return _roomMgr->CustomizeRoom(playerGuid, roomId, ceilingType, doorType, componentTheme);
}

bool HousingServiceMgr::AddHouseXP(ObjectGuid playerGuid, uint32 houseId, uint32 xp)
{
    return _houseMgr->AddHouseXP(playerGuid, houseId, xp);
}

bool HousingServiceMgr::UpgradeHouse(ObjectGuid playerGuid, uint32 houseId)
{
    return _houseMgr->UpgradeHouse(playerGuid, houseId);
}

uint32 HousingServiceMgr::GetHouseLevel(uint32 houseId) const
{
    House* house = _houseMgr->GetHouse(houseId);
    return house ? house->GetLevel() : 0;
}

uint32 HousingServiceMgr::GetHouseXP(uint32 houseId) const
{
    House* house = _houseMgr->GetHouse(houseId);
    return house ? house->GetXP() : 0;
}

uint32 HousingServiceMgr::GetInteriorDecorBudget(uint32 houseId) const
{
    House* house = _houseMgr->GetHouse(houseId);
    return house ? house->GetInteriorDecorBudget() : 0;
}

uint32 HousingServiceMgr::GetRoomPlacementBudget(uint32 houseId) const
{
    House* house = _houseMgr->GetHouse(houseId);
    return house ? house->GetRoomPlacementBudget() : 0;
}

uint32 HousingServiceMgr::GetExteriorDecorBudget(uint32 houseId) const
{
    House* house = _houseMgr->GetHouse(houseId);
    return house ? house->GetExteriorDecorBudget() : 0;
}

bool HousingServiceMgr::SetHousePermission(ObjectGuid playerGuid, uint32 houseId, ObjectGuid targetGuid, uint8 permissionType, bool canEnterInterior, bool canEnterExterior, bool canEditInterior, bool canEditExterior)
{
    return _permissionMgr->SetHousePermission(playerGuid, houseId, targetGuid, permissionType, canEnterInterior, canEnterExterior, canEditInterior, canEditExterior);
}

bool HousingServiceMgr::RemoveHousePermission(ObjectGuid playerGuid, uint32 houseId, ObjectGuid targetGuid)
{
    return _permissionMgr->RemoveHousePermission(playerGuid, houseId, targetGuid);
}

bool HousingServiceMgr::CanPlayerEnterHouse(ObjectGuid playerGuid, uint32 houseId, bool isInterior) const
{
    return _permissionMgr->CanPlayerEnterHouse(playerGuid, houseId, isInterior);
}

bool HousingServiceMgr::CanPlayerEditHouse(ObjectGuid playerGuid, uint32 houseId, bool isInterior) const
{
    return _permissionMgr->CanPlayerEditHouse(playerGuid, houseId, isInterior);
}

bool HousingServiceMgr::SaveBlueprint(ObjectGuid playerGuid, uint32 houseId, std::string const& name, std::string const& description)
{
    return _houseMgr->SaveBlueprint(playerGuid, houseId, name, description);
}

bool HousingServiceMgr::LoadBlueprint(ObjectGuid playerGuid, uint32 blueprintId, uint32 targetHouseId)
{
    return _houseMgr->LoadBlueprint(playerGuid, blueprintId, targetHouseId);
}

bool HousingServiceMgr::DeleteBlueprint(ObjectGuid playerGuid, uint32 blueprintId)
{
    return _houseMgr->DeleteBlueprint(playerGuid, blueprintId);
}

std::vector<uint32> HousingServiceMgr::GetPlayerBlueprints(ObjectGuid playerGuid) const
{
    return _houseMgr->GetPlayerBlueprints(playerGuid);
}