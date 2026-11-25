#ifndef FCBOTS_AUTOLOGIN_H
#define FCBOTS_AUTOLOGIN_H

namespace FCBots::Autologin
{
// Run at worldserver start (WorldScript::OnStartup). Makes the bot accounts, creates a Human Warrior
// on each account that has no character yet, and logs in FCBots.Autologin.Count of them.
void RunAutologin();
}

#endif // FCBOTS_AUTOLOGIN_H
