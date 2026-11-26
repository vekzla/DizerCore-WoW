#include "FCBots_Login.h"

#include <fmt/format.h>

#include "ClientBuildInfo.h"
#include "Common.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "World.h"
#include "WorldSession.h"

namespace FCBots::Login
{
namespace
{
// Battle.net account id linked to this game account (auth.account.battlenet_account).
// Returns 0 if the account has no link, which is logged so it is visible.
std::uint32_t LookupBattlenetAccountId(std::uint32_t accountId)
{
    QueryResult result = LoginDatabase.Query(fmt::format("SELECT battlenet_account FROM account WHERE id = {}", accountId).c_str());
    if (!result)
    {
        TC_LOG_ERROR("scripts", "FCBots login: account {} not found when looking up its Battle.net id", accountId);
        return 0u;
    }

    std::uint32_t battlenetAccountId = result->Fetch()[0].GetUInt32();
    if (battlenetAccountId == 0u)
        TC_LOG_ERROR("scripts", "FCBots login: account {} has no Battle.net account linked", accountId);
    return battlenetAccountId;
}
}

WorldSession* MakeBotSession(std::uint32_t accountId, std::string const& accountName)
{
    ClientBuild::VariantId variant
    {
        .Platform = ClientBuild::Platform::Win_x64,
        .Arch = ClientBuild::Arch::x64,
        .Type = ClientBuild::Type::Retail
    };

    // Expansion 11 = Midnight. Build 0 is a placeholder: set the real build in fcbots.conf.
    std::uint32_t const battlenetAccountId = LookupBattlenetAccountId(accountId);
    return new WorldSession(accountId, std::string(accountName), battlenetAccountId, std::string(), nullptr, SEC_PLAYER, 11u, 0,
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