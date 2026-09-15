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

#include "HouseMgr.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "HousingEntities.h"

HouseMgr::HouseMgr()
{
}

HouseMgr::~HouseMgr()
{
    _houses.clear();
    _playerHouseMap.clear();
}

void HouseMgr::LoadHouses()
{
    uint32 oldMSTime = getMSTime();

    // Load all houses from database
    QueryResult result = CharacterDatabase.Query("SELECT houseId, ownerGuid, plotId, houseTemplateId, houseType, houseStyle, houseLevel, houseXP, interiorDecorBudget, roomPlacementBudget, exteriorDecorBudget, isActive FROM player_houses");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 houses. DB table `player_houses` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        uint32 houseId = fields[0].Get<uint32>();
        ObjectGuid ownerGuid = ObjectGuid::Create<HighGuid::Player>(fields[1].Get<uint64>());
        uint32 plotId = fields[2].Get<uint32>();
        uint32 houseTemplateId = fields[3].Get<uint32>();
        uint8 houseType = fields[4].Get<uint8>();
        uint8 houseStyle = fields[5].Get<uint8>();
        uint8 houseLevel = fields[6].Get<uint8>();
        uint32 houseXP = fields[7].Get<uint32>();
        uint32 interiorDecorBudget = fields[8].Get<uint32>();
        uint32 roomPlacementBudget = fields[9].Get<uint32>();
        uint32 exteriorDecorBudget = fields[10].Get<uint32>();
        bool isActive = fields[11].Get<bool>();

        auto house = std::make_unique<House>();
        house->SetPlotId(plotId);
        house->SetHouseLevel(houseLevel);
        house->SetHouseXP(houseXP);
        house->SetInteriorDecorBudget(interiorDecorBudget);
        house->SetRoomPlacementBudget(roomPlacementBudget);
        house->SetExteriorDecorBudget(exteriorDecorBudget);
        house->SetActive(isActive);

        _houses[houseId] = std::move(house);
        _playerHouseMap[ownerGuid].push_back(houseId);

        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded {} houses in {} ms", count, GetMSTimeDiffToNow(oldMSTime));
}

std::vector<House*> HouseMgr::GetPlayerHouses(ObjectGuid playerGuid) const
{
    std::vector<House*> houses;
    auto it = _playerHouseMap.find(playerGuid);
    if (it != _playerHouseMap.end())
    {
        for (uint32 houseId : it->second)
        {
            auto houseIt = _houses.find(houseId);
            if (houseIt != _houses.end())
            {
                houses.push_back(houseIt->second.get());
            }
        }
    }
    return houses;
}

House* HouseMgr::GetPlayerHouse(ObjectGuid playerGuid, uint32 houseId) const
{
    auto it = _houses.find(houseId);
    if (it != _houses.end() && it->second->GetOwnerGuid() == playerGuid)
    {
        return it->second.get();
    }
    return nullptr;
}

House* HouseMgr::GetActiveHouse(ObjectGuid playerGuid) const
{
    auto houses = GetPlayerHouses(playerGuid);
    if (!houses.empty())
    {
        // Return the first active house as the "active" one
        for (House* house : houses)
        {
            if (house->IsActive())
            {
                return house;
            }
        }
        return houses[0];
    }
    return nullptr;
}

House* HouseMgr::GetHouse(uint32 houseId) const
{
    auto it = _houses.find(houseId);
    if (it != _houses.end())
    {
        return it->second.get();
    }
    return nullptr;
}

bool HouseMgr::CreateHouse(ObjectGuid playerGuid, uint32 plotId, uint32 houseTemplateId)
{
    // TODO: Implement house creation with database persistence
    return false;
}

bool HouseMgr::RelinquishHouse(ObjectGuid playerGuid, uint32 houseId)
{
    // TODO: Implement house relinquishment with database persistence
    return false;
}

bool HouseMgr::MoveHouse(uint32 houseId, uint32 newPlotId)
{
    // TODO: Implement house movement with database persistence
    return false;
}

bool HouseMgr::AddHouseXP(ObjectGuid playerGuid, uint32 houseId, uint32 xp)
{
    House* house = GetHouse(houseId);
    if (!house || house->GetOwnerGuid() != playerGuid)
        return false;

    house->SetHouseXP(house->GetXP() + xp);
    return house->CheckLevelUp();
}

bool HouseMgr::UpgradeHouse(ObjectGuid playerGuid, uint32 houseId)
{
    House* house = GetHouse(houseId);
    if (!house || house->GetOwnerGuid() != playerGuid)
        return false;

    // TODO: Implement house level up logic with budget updates
    return false;
}

bool HouseMgr::SaveBlueprint(ObjectGuid playerGuid, uint32 houseId, std::string const& name, std::string const& description)
{
    // TODO: Implement blueprint saving
    return false;
}

bool HouseMgr::LoadBlueprint(ObjectGuid playerGuid, uint32 blueprintId, uint32 targetHouseId)
{
    // TODO: Implement blueprint loading
    return false;
}

bool HouseMgr::DeleteBlueprint(ObjectGuid playerGuid, uint32 blueprintId)
{
    // TODO: Implement blueprint deletion
    return false;
}

std::vector<uint32> HouseMgr::GetPlayerBlueprints(ObjectGuid playerGuid) const
{
    // TODO: Implement blueprint retrieval
    return {};
}