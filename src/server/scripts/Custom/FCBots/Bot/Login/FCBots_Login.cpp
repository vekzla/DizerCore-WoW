#include "FCBots_Login.h"

#include "Common.h"
#include "ClientBuildInfo.h"
#include "FCBots_Config.h"
#include "World.h"
#include "WorldSession.h"

namespace FCBots::Login
{
WorldSession* LoginBot(std::uint32_t accountId, std::uint32_t battlenetAccountId,
    std::string const& accountName, ObjectGuid::LowType characterGuid)
{
    if (!FCBots::Config::Get().Enabled)
        return nullptr;

    // Already online: nothing to do.
    if (sWorld->FindSession(accountId))
        return nullptr;

    ClientBuild::VariantId variant
    {
        .Platform = ClientBuild::Platform::Win_x64,
        .Arch = ClientBuild::Arch::x64,
        .Type = ClientBuild::Type::Retail
    };

    // TODO(UNVERIFIED): build and expansion values. The real login reads them from the client.
    // Set FCBots.ClientBuild in worldserver.conf to the build the server runs. 0 is a placeholder.
    std::uint32_t const build = static_cast<std::uint32_t>(FCBots::Config::Get().ClientBuild);

    WorldSession* session = new WorldSession(accountId, std::string(accountName), battlenetAccountId,
        std::string(), nullptr, SEC_PLAYER, 11u, 0, std::string("Win"), Minutes(0),
        build, variant, LOCALE_enUS, 0u, false);

    session->SetBotLoginGuid(ObjectGuid::Create<HighGuid::Player>(characterGuid));
    sWorld->AddSession(session);
    session->HandleContinuePlayerLogin();
    return session;
}
}
