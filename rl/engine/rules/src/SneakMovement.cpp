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

#include "SneakMovement.h"

using namespace std;
using namespace Ogre;

namespace rl
{
    SneakMovement::SneakMovement(CreatureController* creature)
        : WalkMovement(creature)
        , mState(UP)
        , mTimer(0)
    {
        mAnim = creature->getCreature()->getAnimation("schleichen");
        mAnim1 = creature->getCreature()->getAnimation("schleichen_vorwaerts");
        mAnim2 = creature->getCreature()->getAnimation("stehen_zu_schleichen");
        mAnim3 = creature->getCreature()->getAnimation("schleichen_zu_stehen");
        mAnimStehen = creature->getCreature()->getAnimation("stehen");
    }

    bool SneakMovement::calculateBaseVelocity(Real& velocity)
    {
        velocity = mMovingCreature->getCurrentGS() / 6.0;
        return isPossible();
    }

    bool SneakMovement::run(Ogre::Real elapsedTime, Ogre::Vector3 direction, Ogre::Vector3 rotation)
    {
        if (mState == UPTODOWN)
        {
            mTimer += elapsedTime;
            if (mTimer > 0.5f)
                mState = DOWN;
        }
        if (mState == UP)
        {
            mMovingCreature->setMovement(CreatureController::MT_STEHEN, direction, rotation);
        }
        if (mState == DOWNTOUP)
        {
            mTimer += elapsedTime;
            if (mTimer > 0.5f)
                mState = UP;
        }
        if (mState == DOWN)
        {
            // setAnimation(elapsedTime);
            direction.normalise();
            Real velocity;
            calculateBaseVelocity(velocity);
            mVelocity = direction * velocity;
            if (direction == Vector3::ZERO || (-mMovingCreature->getVelocity().z) < 0.1)
                mMovingCreature->setAnimation(mAnim.first, mAnim.second);
            else
                mMovingCreature->setAnimation(mAnim1.first, mAnim1.second * -mMovingCreature->getVelocity().z);
            applyAuChanges(elapsedTime);
            if (getRotationMovement()->isPossible())
                getRotationMovement()->run(elapsedTime, direction, rotation);
        }
        else
            mVelocity = Vector3::ZERO;
        return true;
    }

    void SneakMovement::applyAuChanges(Ogre::Real elapsedTime)
    {
    }

    void SneakMovement::setAnimation(Ogre::Real elapsedTime) // is not used
    {
    }

    void SneakMovement::activate()
    {
        WalkMovement::activate();
        mState = UPTODOWN;
        mMovingCreature->setAnimation(mAnim2.first, mAnim2.second, 1, mAnimStehen.first);
        mTimer = 0;
    }

    bool SneakMovement::canChangeToMovement(CreatureController::MovementType id)
    {
        return mState == UP;
    }

    void SneakMovement::requestChangeToMovement(CreatureController::MovementType id)
    {
        if (mState == DOWN)
        {
            mState = DOWNTOUP;
            mMovingCreature->setAnimation(mAnim3.first, mAnim3.second, 1, mAnimStehen.first);
            mTimer = 0;
        }
    }
}
