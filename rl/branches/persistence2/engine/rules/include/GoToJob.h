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

#ifndef __RL_GOTO_JOB_H__
#define __RL_GOTO_JOB_H__

#include "RulesPrerequisites.h"
#include "Creature.h"
#include "CreatureController.h"
#include "Job.h"
#include "CreatureController.h"

namespace rl
{
    class MeshAnimation;

    /// A rl#Job to play an animation.
    class _RlRulesExport GoToJob : public Job
    {
    public:

		GoToJob(const CeGuiString &id, Creature* actor, const Ogre::Vector3& targetPos,
			Ogre::Real maxDistance, Ogre::Real duration,
            CreatureController::MovementType movementType_moving = CreatureController::MT_RENNEN,
            CreatureController::MovementType movementType_idle = CreatureController::MT_STEHEN);
		GoToJob(const CeGuiString &id, Creature* actor, GameObject* target, Ogre::Real maxDistance, Ogre::Real duration,
            bool followTarget = false,
            CreatureController::MovementType movementType_moving = CreatureController::MT_RENNEN,
            CreatureController::MovementType movementType_idle = CreatureController::MT_STEHEN);
        virtual ~GoToJob();

        virtual bool execute(Ogre::Real time);

    private:
		CreatureController* mActor;
		GameObject* mTarget;
        bool mFollowTarget;
		Ogre::Vector3 mTargetPos;
		Ogre::Real mMaxDistance;
		Ogre::Real mTimeLeft;
        CreatureController::MovementType mMovementType_moving, mMovementType_idle;
    };
}

#endif
