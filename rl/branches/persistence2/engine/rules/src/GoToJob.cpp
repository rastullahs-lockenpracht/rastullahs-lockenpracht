/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the Clarified Artistic License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  Clarified Artistic License for more details.
*
*  You should have received a copy of the Clarified Artistic License
*  along with this program; if not you can get it here
*  http://www.jpaulmorrison.com/fbp/artistic2.htm.
*/

#include "stdinc.h"

#include "GoToJob.h"

#include "Creature.h"
#include "CreatureControllerManager.h"
#include "MathUtil.h"

using namespace Ogre;

namespace rl
{
	GoToJob::GoToJob(const CeGuiString &id, Creature* actor, const Vector3& targetPos, Real maxDistance, Real duration, CreatureController::MovementType movementType_moving,
            CreatureController::MovementType movementType_idle)
		: Job(id, false, true, TimeSource::REALTIME_INTERRUPTABLE),
		  mActor(NULL),
		  mTarget(NULL),
		  mTargetPos(targetPos),
		  mMaxDistance(maxDistance),
		  mTimeLeft(duration),
          mMovementType_moving(movementType_moving),
          mMovementType_idle(movementType_idle)
	{
		mActor = CreatureControllerManager::getSingleton().getCreatureController(actor);
	}

	GoToJob::GoToJob(const CeGuiString& id, Creature* actor, GameObject* target, Real maxDistance, Real duration, CreatureController::MovementType movementType_moving,
            CreatureController::MovementType movementType_idle)
		: Job(id, false, true, TimeSource::REALTIME_INTERRUPTABLE),
		  mActor(NULL),
		  mTarget(target),
		  mTargetPos(Vector3::ZERO),
		  mMaxDistance(maxDistance),
		  mTimeLeft(duration),
          mMovementType_moving(movementType_moving),
          mMovementType_idle(movementType_idle)
	{
		mActor = CreatureControllerManager::getSingleton().getCreatureController(actor);
	}

    GoToJob::~GoToJob()
	{
	}

    bool GoToJob::execute(Ogre::Real time)
	{
		// Check now, one frame after done, to make sure everything is
		if (mTimeLeft < 0)
		{
			// Stay put where ever we are.
			mActor->setMovement(mMovementType_idle, Vector3::ZERO, Vector3::ZERO);
			return true;
		}
        
        Ogre::Real distance;
        
		// update target position
		if (mTarget)
		{
			mTargetPos = mTarget->getPosition();
            distance = MathUtil::distance(mTarget->getWorldBoundingBox(),
                                                     mActor->getCreature()->getWorldBoundingBox());
        }
        else 
        {
            distance = MathUtil::distance(mTargetPos, mActor->getCreature()->getWorldBoundingBox());
        }
        
		// Are we there now?
		if (distance < mMaxDistance)
		{
			// Stay put where ever we are.
			mActor->setMovement(mMovementType_idle, Vector3::ZERO, Vector3::ZERO);
			return true;
		}


		Vector3 pos = mActor->getCreature()->getPosition();
		pos.y = 0;
		Vector3 targetPos = mTargetPos;
		targetPos.y = 0;
        Degree realYawDiff = (mActor->getCreature()->getOrientation()*Vector3::NEGATIVE_UNIT_Z).getRotationTo(targetPos - pos).getYaw();
        Vector3 rotation = Vector3::ZERO;
        Quaternion currentOri(mActor->getYaw(), Vector3::UNIT_Y);
        rotation.y = (currentOri*Vector3::NEGATIVE_UNIT_Z).getRotationTo(targetPos - pos).getYaw().valueRadians();

        // old code for rotation:
		//mActor->getCreature()->setOrientation(Vector3::NEGATIVE_UNIT_Z.getRotationTo(targetPos - pos));
        
        // first rotate, then move, is this the "desired" behaviour?
        if( realYawDiff > Degree(5) || realYawDiff < Degree(-5) )
            mActor->setMovement(mMovementType_idle, Vector3::ZERO, rotation);
        else
    		mActor->setMovement(mMovementType_moving, Vector3::NEGATIVE_UNIT_Z, rotation);

		mTimeLeft  -= time;
		return false;
	}
}

