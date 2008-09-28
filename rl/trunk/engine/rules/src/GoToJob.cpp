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
#include "CreatureController.h"
#include "CreatureControllerManager.h"
#include "MathUtil.h"

using namespace Ogre;

namespace rl
{
	GoToJob::GoToJob(Creature* actor, const Vector3& targetPos, Real maxDistance, Real duration)
		: Job(false, true, TimeSource::REALTIME_INTERRUPTABLE),
		  mActor(NULL),
		  mTarget(NULL),
		  mTargetPos(targetPos),
		  mMaxDistance(maxDistance),
		  mTimeLeft(duration)
	{
		mActor = CreatureControllerManager::getSingleton().getCreatureController(actor);
	}

	GoToJob::GoToJob(Creature* actor, GameObject* target, Real maxDistance, Real duration)
		: Job(false, true, TimeSource::REALTIME_INTERRUPTABLE),
		  mActor(NULL),
		  mTarget(target),
		  mTargetPos(Vector3::ZERO),
		  mMaxDistance(maxDistance),
		  mTimeLeft(duration)
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
			mActor->setMovement(CreatureController::MT_STEHEN, Vector3::ZERO, Vector3::ZERO);
			return true;
		}
		// update target position
		if (mTarget)
		{
			mTargetPos = mTarget->getPosition();
		}

		// Are we there now?
		Ogre::Real distance = MathUtil::distance(mTarget->getWorldBoundingBox(),
			mActor->getCreature()->getWorldBoundingBox());
		if (distance < mMaxDistance)
		{
			// Stay put where ever we are.
			mActor->setMovement(CreatureController::MT_STEHEN, Vector3::ZERO, Vector3::ZERO);
			return true;
		}

		// Hard set orientation.
		/// @todo Use CreatureController properly to turn smoothly
		Vector3 pos = mActor->getCreature()->getPosition();
		pos.y = 0;
		Vector3 targetPos = mTargetPos;
		targetPos.y = 0;
		mActor->getCreature()->setOrientation(Vector3::NEGATIVE_UNIT_Z.getRotationTo(targetPos - pos));
		mActor->setMovement(CreatureController::MT_LAUFEN, Vector3::NEGATIVE_UNIT_Z, Vector3::ZERO);

		mTimeLeft  -= time;
		return false;
	}
}
