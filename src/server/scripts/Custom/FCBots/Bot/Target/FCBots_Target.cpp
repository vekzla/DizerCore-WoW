#include "FCBots_Target.h"

#include <cstdint>

#include "CellImpl.h"
#include "Log.h"
#include "ConditionMgr.h"
#include "Creature.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "MotionMaster.h"
#include "MovementDefines.h"
#include "Player.h"
#include "Unit.h"

namespace FCBots::Target
{
namespace
{
// Yards. A new target is searched for within AcquireRange; the current one is dropped beyond LeashRange.
constexpr float AcquireRange = 30.0f;
constexpr float LeashRange = 40.0f;

// Chase distance that puts the bot in melee range.
constexpr float MeleeChaseDistance = 0.5f;

// Where a bot walks to look for targets when nothing hostile is in range (map 0 = Eastern Kingdoms).
// Placeholder: Northshire position from the earlier character export. Confirm hostile mobs spawn there.
constexpr float HuntX = -8914.57f;
constexpr float HuntY = -133.909f;
constexpr float HuntZ = 80.5378f;
constexpr float HuntArriveRange = 15.0f;
constexpr std::uint32_t HuntPointId = 1u;

// Nearest attackable hostile creature. Players are never picked, so the bot does not attack
// real players or their pets. Shaped like Trinity::NearestAttackableUnitInObjectRangeCheck.
class NearestHostileMobCheck
{
public:
    NearestHostileMobCheck(Player const* bot, float range) : _bot(bot), _range(range) { }

    bool operator()(Unit* unit)
    {
        if (unit->GetTypeId() != TYPEID_UNIT)
            return false;
        if (!unit->isTargetableForAttack())
            return false;
        if (!unit->GetCharmerOrOwnerGUID().IsEmpty())
            return false;
        if (!_bot->IsHostileTo(unit))
            return false;
        // Visibility check removed for testing: every creature reported canSee=false for bots.
        if (!_bot->IsWithinDistInMap(unit, _range))
            return false;

        _range = _bot->GetDistance(unit); // nearer units win on the following checks
        return true;
    }

private:
    Player const* _bot;
    float _range;
};

// Debug only: logs up to five creatures near the bot, with why each one is or is not a target.
class NearbyDiagnosticCheck
{
public:
    explicit NearbyDiagnosticCheck(Player const* bot) : _bot(bot) { }

    bool operator()(Unit* unit)
    {
        if (unit == _bot || unit->GetTypeId() != TYPEID_UNIT || _logged >= 5)
            return false;
        ++_logged;
        TC_LOG_ERROR("scripts", "FCBots target diag: {} sees {} entry {} dist {:.1f} hostile={} attackable={} canSee={} alive={}",
            _bot->GetName(), unit->GetName(), unit->GetEntry(), _bot->GetDistance(unit),
            _bot->IsHostileTo(unit), unit->isTargetableForAttack(), _bot->CanSeeOrDetect(unit), unit->IsAlive());
        return false;
    }

private:
    Player const* _bot;
    int _logged = 0;
};

void LogNearbyCreatures(Player* bot)
{
    NearbyDiagnosticCheck diag(bot);
    Unit* none = nullptr;
    Trinity::UnitLastSearcher<NearbyDiagnosticCheck> searcher(bot, none, diag);
    Cell::VisitAllObjects(bot, searcher, AcquireRange);
}

Unit* FindNearestHostileMob(Player* bot)
{
    NearestHostileMobCheck check(bot, AcquireRange);
    Unit* found = nullptr;
    Trinity::UnitLastSearcher<NearestHostileMobCheck> searcher(bot, found, check);
    Cell::VisitAllObjects(bot, searcher, AcquireRange);
    return found;
}

bool IsStillValid(Player* bot, Unit* target)
{
    return target
        && target->IsInWorld()
        && target->IsAlive()
        && target->isTargetableForAttack()
        && bot->IsWithinDistInMap(target, LeashRange);
}
}

// The nearest hostile creature currently attacking the bot, if any.
Unit* FindAttacker(Player* bot)
{
    Unit* found = nullptr;
    for (Unit* attacker : bot->getAttackers())
    {
        if (!attacker || attacker->GetTypeId() != TYPEID_UNIT)
            continue;
        if (!IsStillValid(bot, attacker) || !bot->IsHostileTo(attacker))
            continue;
        if (!found || bot->GetDistance(attacker) < bot->GetDistance(found))
            found = attacker;
    }
    return found;
}

Unit* UpdateTarget(Player* bot)
{
    Unit* target = bot->GetVictim();
	// Retaliate: a hostile creature attacking the bot takes priority over the current victim.
    if (Unit* attacker = FindAttacker(bot); attacker && attacker != target)
    {
        if (target)
        {
            bot->AttackStop();
            bot->GetMotionMaster()->Remove(CHASE_MOTION_TYPE);
        }
        if (!bot->Attack(attacker, true))
            return nullptr;

        TC_LOG_ERROR("scripts", "FCBots target: {} switching to attacker {} (entry {})", bot->GetName(), attacker->GetName(), attacker->GetEntry());
        bot->GetMotionMaster()->Remove(CHASE_MOTION_TYPE);
        if (!bot->IsWithinMeleeRange(attacker))
            bot->GetMotionMaster()->MoveChase(attacker, ChaseRange(MeleeChaseDistance));
        return attacker;
    }
    if (IsStillValid(bot, target))
    {
        // Diagnostic: once every 2 seconds, report which swing checks pass for the current victim.
        static std::uint32_t swingDiagMs = 0u;
        swingDiagMs += 200u;
                if (swingDiagMs >= 2000u)
        {
            swingDiagMs = 0u;
            TC_LOG_ERROR("scripts", "FCBots swing diag: {} vs {} los={} canSee={} validTarget={} inMelee={} attackReady={}",
                bot->GetName(), target->GetName(),
                bot->IsWithinLOSInMap(target), bot->CanSeeOrDetect(target), bot->IsValidAttackTarget(target),
                bot->IsWithinMeleeRange(target), bot->isAttackReady(BASE_ATTACK));

            SpawnTrackingStateData const* spawnState = target->GetSpawnTrackingStateDataForPlayer(bot);
            TC_LOG_ERROR("scripts", "FCBots vis diag: samePhase={} spawnTracked={} spawnVisible={}",
                bot->InSamePhase(target), spawnState != nullptr, spawnState ? spawnState->Visible : true);
            
			TC_LOG_ERROR("scripts", "FCBots sight2 diag: privateOwnerOk={} conditionsOk={} withinSight={}",
                target->CheckPrivateObjectOwnerVisibility(bot) && bot->CheckPrivateObjectOwnerVisibility(target),
                sConditionMgr->IsObjectMeetingVisibilityByObjectIdConditions(target, bot),
                bot->IsWithinDist(target, bot->GetSightRange(target), false));
        }

        if (bot->IsWithinMeleeRange(target))
        {
            // In melee range: stand still, face the target, and swing.
            if (bot->GetMotionMaster()->GetCurrentMovementGeneratorType() == CHASE_MOTION_TYPE)
                bot->GetMotionMaster()->MoveIdle();
            if (!bot->HasInArc(2 * float(M_PI) / 3, target))
                bot->SetFacingToObject(target);
        }
		
        else if (bot->GetMotionMaster()->GetCurrentMovementGeneratorType() != CHASE_MOTION_TYPE)
        {
            bot->GetMotionMaster()->MoveChase(target, ChaseRange(MeleeChaseDistance));
        }
        return target;
    }

    // The old victim is dead, gone or out of leash: drop it and stop chasing it.
    if (target)
    {
        bot->AttackStop();
        bot->GetMotionMaster()->Remove(CHASE_MOTION_TYPE);
    }

    target = FindNearestHostileMob(bot);
    if (!target)
    {
        // Throttled debug line: shows the search runs and finds nothing (about every 5 seconds).
        static std::uint32_t sinceLogMs = 5000u;
        sinceLogMs += 200u;
        if (sinceLogMs >= 5000u)
        {
            sinceLogMs = 0u;
            TC_LOG_ERROR("scripts", "FCBots target: {} found no hostile creature within {} yards, walking to hunt point", bot->GetName(), AcquireRange);
            LogNearbyCreatures(bot);
        }

        // Walk to the hunt point when not there yet and not already walking to it.
        // Hunting-point walk disabled for the summon test. Re-enable once summon works.
        // if (bot->GetMotionMaster()->GetCurrentMovementGeneratorType() != POINT_MOTION_TYPE
        //     && !bot->IsWithinDist2d(HuntX, HuntY, HuntArriveRange))
        //     bot->GetMotionMaster()->MovePoint(HuntPointId, HuntX, HuntY, HuntZ);
        return nullptr;
    }

    if (!bot->Attack(target, true))
    {
        TC_LOG_ERROR("scripts", "FCBots target: {} could not attack {}", bot->GetName(), target->GetName());
        return nullptr;
    }

    TC_LOG_ERROR("scripts", "FCBots target: {} attacking {} (entry {})", bot->GetName(), target->GetName(), target->GetEntry());

    bot->GetMotionMaster()->Remove(CHASE_MOTION_TYPE);
    if (!bot->IsWithinMeleeRange(target))
        bot->GetMotionMaster()->MoveChase(target, ChaseRange(MeleeChaseDistance));
    return target;
}
}
