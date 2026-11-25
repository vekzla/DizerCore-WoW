#include "FCBots_Group.h"

#include "Group.h"
#include "GroupMgr.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "WorldSession.h"

namespace FCBots::Group
{
void AcceptPendingInvite(Player* bot)
{
    if (!bot)
        return;

    ::Group* group = bot->GetGroupInvite();
    if (!group)
        return;

    // The inviter is the group leader. Only a real player's invite is accepted.
    Player* inviter = ObjectAccessor::FindPlayer(group->GetLeaderGUID());
    if (!inviter || !inviter->GetSession() || inviter->GetSession()->IsBotSession())
    {
        group->RemoveInvite(bot);
        return;
    }

    // Same order as HandleGroupAcceptOpcode: drop the invite, then form or join the group.
    group->RemoveInvite(bot);

    if (group->IsFull())
        return;

    if (!group->IsCreated())
    {
        group->RemoveInvite(inviter);
        group->Create(inviter);
        sGroupMgr->AddGroup(group);
    }

    if (!group->AddMember(bot))
        return;

    group->BroadcastGroupUpdate();
    TC_LOG_INFO("scripts", "FCBots: {} accepted a group invite from {}", bot->GetName(), inviter->GetName());
}
}
