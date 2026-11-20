#ifndef FCBOTS_DB_FCBOTS_DB_H_H
#define FCBOTS_DB_FCBOTS_DB_H_H

#include <string_view>

// Table names in the fcbots database (sql/base/fcbots_database.sql).
// Queries are added in later stages. No synchronous queries on map threads.
namespace FCBots::Db
{
constexpr std::string_view TableUpdates = "updates";
constexpr std::string_view TableUpdatesInclude = "updates_include";
constexpr std::string_view TableBotAccounts = "bot_accounts";
constexpr std::string_view TableBotTemplates = "bot_templates";
constexpr std::string_view TableBotOwners = "bot_owners";
}

#endif // FCBOTS_DB_FCBOTS_DB_H_H
