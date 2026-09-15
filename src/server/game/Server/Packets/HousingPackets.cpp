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

#include "HousingPackets.h"
#include "PacketOperators.h"

namespace WorldPackets::Housing
{
void DeclineNeighborhoodInvites::Read()
{
    _worldPacket >> Bits<1>(Allow);
}

void HousingDecorDeleteFromStorage::Read()
{
    _worldPacket >> DecorID;
    _worldPacket >> Count;
}

void HousingDecorPlace::Read()
{
    _worldPacket >> DecorID;
    _worldPacket >> HouseID;
    _worldPacket >> RoomID;
    _worldPacket >> PositionX;
    _worldPacket >> PositionY;
    _worldPacket >> PositionZ;
    _worldPacket >> OrientationX;
    _worldPacket >> OrientationY;
    _worldPacket >> OrientationZ;
    _worldPacket >> Scale;
}

void HousingDecorMove::Read()
{
    _worldPacket >> PlacedDecorID;
    _worldPacket >> PositionX;
    _worldPacket >> PositionY;
    _worldPacket >> PositionZ;
    _worldPacket >> OrientationX;
    _worldPacket >> OrientationY;
    _worldPacket >> OrientationZ;
    _worldPacket >> Scale;
}

void HousingDecorRemove::Read()
{
    _worldPacket >> PlacedDecorID;
}

void HousingDecorRequestStorage::Read()
{
    _worldPacket >> HouseID;
}

void HousingDecorSetEditMode::Read()
{
    _worldPacket >> Bits<1>(EditMode);
}

void HousingDecorLock::Read()
{
    _worldPacket >> PlacedDecorID;
    _worldPacket >> Bits<1>(Lock);
}

void HousingDecorRedeemDeferredDecor::Read()
{
    _worldPacket >> HouseID;
}

void HousingDecorSetDyeSlots::Read()
{
    _worldPacket >> PlacedDecorID;
    _worldPacket >> DyeSlot1;
    _worldPacket >> DyeSlot2;
    _worldPacket >> DyeSlot3;
}

void HousingDecorSetPet::Read()
{
    _worldPacket >> PlacedDecorID;
    _worldPacket >> PetID;
}

void HousingFixtureSetHouseSize::Read()
{
    _worldPacket >> HouseSize;
}

void HousingFixtureSetHouseType::Read()
{
    _worldPacket >> HouseType;
}

void HousingFixtureCreateFixture::Read()
{
    _worldPacket >> HouseID;
    _worldPacket >> FixtureTemplateID;
    _worldPacket >> FixtureSlot;
    _worldPacket >> VariantID;
}

void HousingFixtureDeleteFixture::Read()
{
    _worldPacket >> FixtureID;
}

void HousingFixtureSetCoreFixture::Read()
{
    _worldPacket >> HouseID;
    _worldPacket >> FixtureID;
}

void HousingFixtureSetEditMode::Read()
{
    _worldPacket >> Bits<1>(EditMode);
}

void HousingGetCurrentHouseInfo::Read()
{
    _worldPacket >> HouseID;
}

void HousingHouseStatus::Read()
{
    _worldPacket >> HouseID;
}

void HousingGetPlayerPermissions::Read()
{
    _worldPacket >> HouseID;
    _worldPacket >> TargetGuid;
}

void HousingPhotoSharingClearAuthorization::Read()
{
    _worldPacket >> HouseID;
}

void HousingPhotoSharingCompleteAuthorization::Read()
{
    _worldPacket >> HouseID;
    _worldPacket >> PhotoID;
}

void HousingResetHouse::Read()
{
    _worldPacket >> HouseID;
}

void HousingResetKioskMode::Read()
{
    _worldPacket >> HouseID;
}

void HousingRoomAdd::Read()
{
    _worldPacket >> HouseID;
    _worldPacket >> RoomTemplateID;
    _worldPacket >> PositionX;
    _worldPacket >> PositionY;
    _worldPacket >> Orientation;
}

void HousingRoomRemove::Read()
{
    _worldPacket >> RoomID;
}

void HousingRoomMove::Read()
{
    _worldPacket >> RoomID;
    _worldPacket >> PositionX;
    _worldPacket >> PositionY;
    _worldPacket >> Orientation;
}

void HousingRoomApplyComponentMaterials::Read()
{
    _worldPacket >> RoomID;
    _worldPacket >> MaterialTemplateID;
    _worldPacket >> ComponentSlot;
}

void HousingRoomRotate::Read()
{
    _worldPacket >> RoomID;
    _worldPacket >> Orientation;
}

void HousingRoomSetCeilingType::Read()
{
    _worldPacket >> RoomID;
    _worldPacket >> CeilingType;
}

void HousingRoomSetComponentTheme::Read()
{
    _worldPacket >> RoomID;
    _worldPacket >> ComponentTheme;
}

void HousingRoomSetDoorType::Read()
{
    _worldPacket >> RoomID;
    _worldPacket >> DoorType;
}

void HousingRoomSetLayoutEditMode::Read()
{
    _worldPacket >> Bits<1>(EditMode);
}

void HousingSvcsGetPlayerHousesInfo::Read()
{
    _worldPacket >> PlayerGuid;
}

void HousingSvcsNeighborhoodReservePlot::Read()
{
    _worldPacket >> PlotID;
}

void HousingSvcsRelinquishHouse::Read()
{
    _worldPacket >> HouseID;
}

void HousingSvcsTeleportToPlot::Read()
{
    _worldPacket >> PlotID;
}

void HousingSvcsUpdateHouseSettings::Read()
{
    _worldPacket >> HouseID;
    _worldPacket >> SettingsJSON;
}

void HousingSvcsAcceptNeighborhoodOwnership::Read()
{
    _worldPacket >> NeighborhoodID;
}

void HousingSvcsDeleteAllNeighborhoodInvites::Read()
{
    // No data
}

void HousingSvcsGetBnetFriendNeighborhoods::Read()
{
    // No data
}

void HousingSvcsGetHouseFinderInfo::Read()
{
    _worldPacket >> NeighborhoodID;
}

void HousingSvcsGetHouseFinderNeighborhood::Read()
{
    _worldPacket >> NeighborhoodID;
}

void HousingSvcsGetPotentialHouseOwners::Read()
{
    _worldPacket >> NeighborhoodID;
}

void HousingSvcsGuildCreateNeighborhood::Read()
{
    _worldPacket >> NeighborhoodName;
}

void HousingSvcsHouseFinderIgnoreNeighborhood::Read()
{
    _worldPacket >> NeighborhoodID;
    _worldPacket >> Bits<1>(Ignore);
}

void HousingSvcsPlayerViewHousesByBNetAccount::Read()
{
    _worldPacket >> BNetAccountGuid;
}

void HousingSvcsPlayerViewHousesByPlayer::Read()
{
    _worldPacket >> PlayerGuid;
}

void HousingSvcsRejectNeighborhoodOwnership::Read()
{
    _worldPacket >> NeighborhoodID;
}

void HousingSvcsStartTutorial::Read()
{
    _worldPacket >> TutorialID;
}
}
