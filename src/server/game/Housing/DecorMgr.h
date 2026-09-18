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
  
#ifndef TRINITYCORE_DECOR_MGR_H  
#define TRINITYCORE_DECOR_MGR_H  
  
#include "Define.h"  
#include "ObjectGuid.h"  
#include <map>  
#include <memory>       // ADDED  
#include <vector>  
  
class PlacedDecor;       // ADDED  
  
class TC_GAME_API DecorMgr  
{  
public:  
    DecorMgr();  
    ~DecorMgr();  
  
    // Database loading  
    void LoadDecorCollections();  
    void LoadPlacedDecor();  
  
    // Decor operations  
    bool AddDecorToCollection(ObjectGuid playerGuid, uint32 decorTemplateId, uint32 quantity);  
    bool RemoveDecorFromCollection(ObjectGuid playerGuid, uint32 decorTemplateId, uint32 quantity);  
    std::vector<uint32> GetPlayerDecorCollection(ObjectGuid playerGuid) const;  
    bool PlaceDecor(ObjectGuid playerGuid, uint32 houseId, uint32 decorTemplateId, float posX, float posY, float posZ, float orientation);  
    bool RemoveDecor(ObjectGuid playerGuid, uint32 placedDecorId);  
    bool MoveDecor(ObjectGuid playerGuid, uint32 placedDecorId, float posX, float posY, float posZ, float orientation);  
  
private:  
    std::map<ObjectGuid, std::map<uint32, uint32>> _decorCollections;  
    std::map<uint32, std::unique_ptr<PlacedDecor>> _placedDecor;   // ADDED  
};  
  
#endif // TRINITYCORE_DECOR_MGR_H