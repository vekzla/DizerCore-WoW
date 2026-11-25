#include "FCBots_Login.h"

#include "ClientBuildInfo.h"
#include "Common.h"
#include "World.h"
#include "WorldSession.h"

namespace FCBots::Login
{
WorldSession* MakeBotSession(std::uint32_t accountId, std::string const& accountName)
{
    ClientBuild::VariantId variant
    {
        .Platform = ClientBuild::Platform::Win_x64,
        .Arch = ClientBuild::Arch::x64,
        .Type = ClientBuild::Type::Retail
    };

    // Expansion 11 = Midnight. Build 0 is a placeholder: set the real build in fcbots.conf.
    return new WorldSession(accountId, std::string(accountName), 0u, std::string(), nullptr, SEC_PLAYER, 11u, 0,
        std::string("Win"), Minutes(0), 0u, variant, LOCALE_enUS, 0u, false);
}

bool StartBotSession(WorldSession* session, ObjectGuid::LowType characterGuid)
{
    if (!session)
        return false;

    if (sWorld->FindSession(session->GetAccountId()))
        return false;

    sWorld->AddSession(session);
    session->SetBotLoginGuid(ObjectGuid::Create<HighGuid::Player>(characterGuid));
    session->HandleContinuePlayerLogin();
    return true;
}
}
