#ifndef FCBOTS_TARGET_H_H
#define FCBOTS_TARGET_H_H

class Player;
class Unit;

namespace FCBots::Target
{
// Chooses this tick's attack target for a bot and keeps it in melee.
//  - Keeps the current victim while it is alive, attackable and within the leash range.
//  - Otherwise drops it and attacks the nearest hostile non-player creature within the acquire range.
//  - Starts a chase when the target is outside melee range.
// Returns the target, or nullptr when there is nothing to fight.
Unit* UpdateTarget(Player* bot);
}

#endif // FCBOTS_TARGET_H_H
