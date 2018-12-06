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
#include "stdinc.h" //precompiled header

#include "DsaManager.h"
#include "MeshAnimation.h"
#include "SprintMovement.h"

using namespace std;
using namespace Ogre;

Real minSquaredSpeed = 0.6f;

namespace rl
{
    SprintMovement::SprintMovement(CreatureController* creature)
        : WalkMovement(creature)
        , mVelocityImprovement(0)
        , mLastProbe(0)
    {
        mAnim = creature->getCreature()->getAnimation("rennen");
    }

    bool SprintMovement::calculateBaseVelocity(Real& velocity)
    {
        velocity = mMovingCreature->getCurrentGS();
        return isPossible();
    }

    bool SprintMovement::isPossible() const
    {
        if (mMovingCreature->getMovement() == this)
            return WalkMovement::isPossible()
                && mMovingCreature->getCreature()->getAu() > mMovingCreature->getCreature()->getAuMax() * 0.3
                && mMovingCreature->getCreature()->getAu() > 6;
        else
            return WalkMovement::isPossible()
                && mMovingCreature->getCreature()->getAu() > mMovingCreature->getCreature()->getAuMax() * 0.5
                && mMovingCreature->getCreature()->getAu() > 8;
    }

    bool SprintMovement::run(Ogre::Real elapsedTime, Ogre::Vector3 direction, Ogre::Vector3 rotation)
    {
        doTalentProbeIfNecessary();
        bool ret = WalkMovement::run(elapsedTime, direction, rotation);
        mVelocity += mVelocityImprovement;
        return ret;
    }

    void SprintMovement::applyAuChanges(Ogre::Real elapsedTime)
    {
        if (mMovingCreature->getVelocity().squaredLength() < minSquaredSpeed)
            StandAroundMovement::applyAuChanges(elapsedTime);
        else
            mMovingCreature->getCreature()->damageAu(elapsedTime / 1.5);
    }

    void SprintMovement::setAnimation(Ogre::Real elapsedTime)
    {
        if (mMovingCreature->getVelocity().squaredLength() < minSquaredSpeed)
        {
            StandAroundMovement::setAnimation(elapsedTime);
            return;
        }

        Real length = 5. / 3.;
        Real weight = 1;
        Real relTimeOffset = 0;

        switch (mMovingCreature->getLastMovementType())
        {
        case CreatureController::MT_NONE:
            break;
        case CreatureController::MT_STEHEN:
            relTimeOffset = 0.25;
            break;
        case CreatureController::MT_GEHEN:
            relTimeOffset = mMovingCreature->getAnimationTimePlayed();
            if (mMovingCreature->getLastMovementChange() < 0.5f)
                weight = mMovingCreature->getLastMovementChange() / 0.5f;
            break;
        default:
            break;
        }

        // apply relTimeOffset only once
        if (mMovingCreature->getLastMovementChange() > elapsedTime)
            relTimeOffset = 0;

        MeshAnimation* meshAnim = mMovingCreature->setAnimation(
            mAnim.first, fabs(mMovingCreature->getVelocity().z) * mAnim.second, 0, mAnim.first, weight);
        if (meshAnim != NULL && relTimeOffset)
            meshAnim->doAddTime(relTimeOffset * meshAnim->getLength());
    }

    void SprintMovement::activate()
    {
        WalkMovement::activate();
        doTalentProbeIfNecessary();
    }

    void SprintMovement::doTalentProbeIfNecessary()
    {
        Date now = DsaManager::getSingleton().getCurrentDate();
        if (mLastProbe + mMovingCreature->getCreature()->getAuMax() * Date::ONE_SECOND >= now || mLastProbe == 0)
        {
            mVelocityImprovement = 0;
            if (mMovingCreature->getCreature()->hasTalent("Athletik"))
            {
                mLastProbe = now;
                int taw = mMovingCreature->getCreature()->doTalentprobe("Athletik", 0);
                if (taw == RESULT_AUTOERFOLG)
                {
                    mVelocityImprovement += mMovingCreature->getCreature()->getTalent("Athletik") * 0.2;
                }
                else if (taw == RESULT_SPEKT_AUTOERFOLG)
                {
                    mVelocityImprovement += mMovingCreature->getCreature()->getTalent("Athletik") * 0.3;
                }
                else
                {
                    mVelocityImprovement += taw * 0.1;
                }
            }
            else
            {
                mVelocityImprovement = 0;
            }
        }
    }
}
