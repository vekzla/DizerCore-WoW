#ifndef FCBOTS_GROUP_H
#define FCBOTS_GROUP_H

class Player;

namespace FCBots::Group
{
// Accepts a pending group invite for a bot, the same steps as WorldSession::HandleGroupAcceptOpcode.
// Does nothing if the bot has no invite.
void AcceptPendingInvite(Player* bot);
}

#endif // FCBOTS_GROUP_H
