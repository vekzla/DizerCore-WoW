#ifndef FCBOTS_WARRIOR_BOT_H_H
#define FCBOTS_WARRIOR_BOT_H_H

#include <cstdint>

#include "Warrior_Engine.h"

#include "ObjectGuid.h"

class Player;
class Unit;

// TrinityCore adapter. Implementation in Warrior_Bot.cpp (compiles inside the TrinityCore tree).
namespace FCBots::Class::Warrior::Bot
{
// Runs one engine tick for a bot: reads its state, spends talents, casts the chosen spell.
// Returns the spell cast (0 if none).
std::uint32_t TickBot(Player* bot, Unit* target, Engine::BotSetup const& setup, Engine::BotProgress& progress);

// Adds a bot to the world tick. Called when the bot is summoned.
void RegisterWarriorBot(ObjectGuid guid, Engine::BotSetup const& setup);
}

#endif // FCBOTS_WARRIOR_BOT_H_H
