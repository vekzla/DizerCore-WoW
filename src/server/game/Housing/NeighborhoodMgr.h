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

#ifndef TRINITYCORE_NEIGHBORHOOD_MGR_H
#define TRINITYCORE_NEIGHBORHOOD_MGR_H

#include "Define.h"
#include "ObjectGuid.h"
#include <map>
#include <memory>
#include <vector>

class Neighborhood;
class NeighborhoodPlot;

class TC_GAME_API NeighborhoodMgr
{
public:
    NeighborhoodMgr();
    ~NeighborhoodMgr();

    // Database loading
    void LoadNeighborhoods();
    void LoadPlots();

    // Neighborhood operations
    Neighborhood* GetNeighborhood(uint32 neighborhoodId) const;
    std::vector<Neighborhood*> GetNeighborhoods(uint8 faction) const;
    NeighborhoodPlot* GetPlot(uint32 plotId) const;
    std::vector<NeighborhoodPlot*> GetAvailablePlots(uint32 neighborhoodId) const;
    bool ReservePlot(ObjectGuid playerGuid, uint32 plotId);
    bool ClearPlotReservation(uint32 plotId);

private:
    std::map<uint32, std::unique_ptr<Neighborhood>> _neighborhoods;
    std::map<uint32, std::unique_ptr<NeighborhoodPlot>> _plots;
};

#endif // TRINITYCORE_NEIGHBORHOOD_MGR_H