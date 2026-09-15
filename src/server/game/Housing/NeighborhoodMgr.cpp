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

#include "NeighborhoodMgr.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "HousingEntities.h"

NeighborhoodMgr::NeighborhoodMgr()
{
}

NeighborhoodMgr::~NeighborhoodMgr()
{
    _neighborhoods.clear();
    _plots.clear();
}

void NeighborhoodMgr::LoadNeighborhoods()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = CharacterDatabase.Query("SELECT neighborhoodId, name, type, faction, guildId, ownerAccountId, isActive FROM neighborhoods");

    if (!result)
    {
        LOG_INFO("server.loading", ">> Loaded 0 neighborhoods. DB table `neighborhoods` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        uint32 neighborhoodId = fields[0].Get<uint32>();
        std::string name = fields[1].Get<std::string>();
        uint8 type = fields[2].Get<uint8>();
        uint8 faction = fields[3].Get<uint8>();
        uint32 guildId = fields[4].Get<uint32>();
        uint32 ownerAccountId = fields[5].Get<uint32>();
        bool isActive = fields[6].Get<bool>();

        auto neighborhood = std::make_unique<Neighborhood>();
        neighborhood->SetName(name);
        neighborhood->SetType(type);
        neighborhood->SetFaction(faction);
        neighborhood->SetGuildId(guildId);
        neighborhood->SetOwnerAccountId(ownerAccountId);
        neighborhood->SetActive(isActive);

        _neighborhoods[neighborhoodId] = std::move(neighborhood);
        ++count;
    } while (result->NextRow());

    LOG_INFO("server.loading", ">> Loaded {} neighborhoods in {} ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void NeighborhoodMgr::LoadPlots()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = CharacterDatabase.Query("SELECT plotId, neighborhoodId, plotIndex, ownerGuid, houseId, isReserved, reservedByAccountId, positionX, positionY, positionZ, orientation, isActive FROM neighborhood_plots");

    if (!result)
    {
        LOG_INFO("server.loading", ">> Loaded 0 plots. DB table `neighborhood_plots` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        uint32 plotId = fields[0].Get<uint32>();
        uint32 neighborhoodId = fields[1].Get<uint32>();
        uint8 plotIndex = fields[2].Get<uint8>();
        ObjectGuid ownerGuid = ObjectGuid::Create<HighGuid::Player>(fields[3].Get<uint64>());
        uint32 houseId = fields[4].Get<uint32>();
        bool isReserved = fields[5].Get<bool>();
        uint32 reservedByAccountId = fields[6].Get<uint32>();
        float positionX = fields[7].Get<float>();
        float positionY = fields[8].Get<float>();
        float positionZ = fields[9].Get<float>();
        float orientation = fields[10].Get<float>();
        bool isActive = fields[11].Get<bool>();

        auto plot = std::make_unique<NeighborhoodPlot>();
        plot->SetOwnerGuid(ownerGuid);
        plot->SetHouseId(houseId);
        plot->SetReserved(isReserved);
        plot->SetReservedByAccountId(reservedByAccountId);
        plot->SetActive(isActive);

        _plots[plotId] = std::move(plot);
        ++count;
    } while (result->NextRow());

    LOG_INFO("server.loading", ">> Loaded {} plots in {} ms", count, GetMSTimeDiffToNow(oldMSTime));
}

Neighborhood* NeighborhoodMgr::GetNeighborhood(uint32 neighborhoodId) const
{
    auto it = _neighborhoods.find(neighborhoodId);
    if (it != _neighborhoods.end())
    {
        return it->second.get();
    }
    return nullptr;
}

std::vector<Neighborhood*> NeighborhoodMgr::GetNeighborhoods(uint8 faction) const
{
    std::vector<Neighborhood*> neighborhoods;
    for (auto const& [id, neighborhood] : _neighborhoods)
    {
        if (neighborhood->GetFaction() == faction && neighborhood->IsActive())
        {
            neighborhoods.push_back(neighborhood.get());
        }
    }
    return neighborhoods;
}

NeighborhoodPlot* NeighborhoodMgr::GetPlot(uint32 plotId) const
{
    auto it = _plots.find(plotId);
    if (it != _plots.end())
    {
        return it->second.get();
    }
    return nullptr;
}

std::vector<NeighborhoodPlot*> NeighborhoodMgr::GetAvailablePlots(uint32 neighborhoodId) const
{
    std::vector<NeighborhoodPlot*> availablePlots;
    for (auto const& [id, plot] : _plots)
    {
        if (plot->GetNeighborhoodId() == neighborhoodId && !plot->GetOwnerGuid() && !plot->IsReserved() && plot->IsActive())
        {
            availablePlots.push_back(plot.get());
        }
    }
    return availablePlots;
}

bool NeighborhoodMgr::ReservePlot(ObjectGuid playerGuid, uint32 plotId)
{
    // TODO: Implement plot reservation with database persistence
    return false;
}

bool NeighborhoodMgr::ClearPlotReservation(uint32 plotId)
{
    // TODO: Implement plot reservation clearing with database persistence
    return false;
}