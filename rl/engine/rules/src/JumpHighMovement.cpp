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

#include "JumpHighMovement.h"
#include "PhysicalThing.h"

using namespace std;
using namespace Ogre;

namespace rl
{
    JumpHighMovement::JumpHighMovement(CreatureController* creature)
        : AbstractMovement(creature)
        , mState(DOWN)
        , mHeight(0)
        , mJumpNow(false)
        , mTimer(0)
        , mMoveForward(0)
    {
        mAnimation = creature->getCreature()->getAnimation("hochsprung");
        mAnimationAbsprung = creature->getCreature()->getAnimation("hochsprung_absprung");
        mAnimationLandung = creature->getCreature()->getAnimation("hochsprung_landung");
        mAnimationForCollision = creature->getCreature()->getAnimation("stehen"); // wird das hier gebraucht?
    }

    void JumpHighMovement::activate()
    {
        AbstractMovement::activate();
        mState = DOWNTOUP;
        mMovingCreature->setAnimation(
            mAnimationAbsprung.first, mAnimationAbsprung.second, 1, mAnimationForCollision.first);
        mTimer = 0;
        calculateBaseVelocity(mHeight);

        if (mMovingCreature->getCreature()->hasTalent("Athletik"))
        {
            int tap = mMovingCreature->getCreature()->doTalentprobe("Athletik", 0);
            if (tap > 0)
            {
                if (tap == RESULT_SPEKT_AUTOERFOLG)
                {
                    mHeight += mMovingCreature->getCreature()->getTalent("Athletik") / 12.5;
                }
                else if (tap == RESULT_AUTOERFOLG)
                {
                    mHeight += mMovingCreature->getCreature()->getTalent("Athletik") / 25.0;
                }
                else
                {
                    mHeight += tap / 25.0;
                }
            }
        }
        else
        {
        }
        mMovingCreature->getCreature()->damageAu(2. / 3);
    }

    void JumpHighMovement::deactivate()
    {
        AbstractMovement::deactivate();
    }

    bool JumpHighMovement::calculateBaseVelocity(Real& velocity)
    {
        velocity = (mMovingCreature->getCreature()->getEigenschaft("GE")
                       + mMovingCreature->getCreature()->getEigenschaft("KK"))
            / 4.0 / 5.0;

        // steht nicht in den Regeln aber finde ich sinnvoll
        // velocityBase *= (1 - getrageneLast/KK);
        // steht in den Regeln: pro Ersch�pfung ein KK abziehen
        // if( mErschoepfung > getEigenschaft("KO") )
        //    velocity -= (mErschoepfung - getEigenschaft("KO")) / 4.0 / 5.0;
        // steht nicht in den Regeln, aber finde ich sinnvoll
        // if( getAu() < getAuBasis() / 3.0 )
        //    velocity -= (getAu() / getAuBasis() * 3.0) * getEigenschaft("GE") / 4.0 / 5.0;
        return isPossible();
    }

    bool JumpHighMovement::isPossible() const
    {
        if (mMovingCreature->getMovement() == this)
            return (mMovingCreature->getAbstractLocation() == CreatureController::AL_FLOOR
                       || mMovingCreature->getAbstractLocation() == CreatureController::AL_AIRBORNE)
                && mMovingCreature->getCreature()->getAu() > 6
                && !(mMovingCreature->getCreature()->getLifeState() & (Effect::LS_IMMOBILE));
        else
            return mMovingCreature->getAbstractLocation() == CreatureController::AL_FLOOR
                && mMovingCreature->getCreature()->getAu() > 8
                && mMovingCreature->getCreature()->getAu() > mMovingCreature->getCreature()->getAuMax() / 3.0
                && !(mMovingCreature->getCreature()->getLifeState() & (Effect::LS_IMMOBILE));
    }

    void JumpHighMovement::calculateForceAndTorque(Vector3& force, Vector3& torque, Real timestep)
    {
        Real mass;
        Vector3 inertia;
        OgreNewt::Body* body = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody();
        body->getMassMatrix(mass, inertia);

        if (mJumpNow)
        {
            mJumpNow = false;

            Real m = mass;
            Real g = PhysicsManager::getSingleton().getGravity().length();
            Real t = timestep;
            Real h = mHeight;
            Real jumpForce = 0.5f * g * m * (Math::Sqrt(1 + 8 * h / (g * t * t)) - 1);
            force = Vector3(0, jumpForce, 0);
            mMovingCreature->setAbstractLocation(CreatureController::AL_AIRBORNE);
        }
        else if (mState == UP && mMoveForward != 0) // try to move forward, if wanted
        {
            // low velocity towards this direction
            if (mMoveForward > 0)
                mMoveForward = 1.0;
            else
                mMoveForward = -1.0;

            Real mass;
            Vector3 inertia;
            OgreNewt::Body* body = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody();
            body->getMassMatrix(mass, inertia);

            Vector3 vel = mMovingCreature->getVelocity();
            Real delay = 0.05; //(2 * PhysicsManager::getSingleton().getMaxTimestep());
            Real diff = (mMoveForward - vel.z);
            force.z += mass * diff / delay;
        }

        Vector3 omega = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody()->getOmega();
        torque = -omega / PhysicsManager::getSingleton().getMaxTimestep() * 2 * inertia;
    }

    bool JumpHighMovement::run(Ogre::Real elapsedTime, Ogre::Vector3 direction, Ogre::Vector3 rotation)
    {
        mMoveForward = direction.z;

        if (mState == DOWN)
        {
            mMovingCreature->setMovement(CreatureController::MT_STEHEN, direction, rotation);
        }
        if (mState == UPTODOWN)
        {
            mTimer += elapsedTime;
            if (mTimer >= 0.5f)
            {
                mState = DOWN;
            }
        }
        if (mState == UP)
        {
            mTimer += elapsedTime;
            if (mTimer < 0.5f)
            {
                mMovingCreature->setAbstractLocation(CreatureController::AL_AIRBORNE);
            }
            else if (mMovingCreature->getAbstractLocation() != CreatureController::AL_AIRBORNE)
            {
                mState = UPTODOWN;
                mMovingCreature->setAnimation(
                    mAnimationLandung.first, mAnimationLandung.second, 1, mAnimationForCollision.first);
                mTimer = 0;
            }
        }
        if (mState == DOWNTOUP)
        {
            mTimer += elapsedTime;
            if (mTimer >= 0.25f)
            {
                mState = UP;
                mMovingCreature->setAbstractLocation(CreatureController::AL_AIRBORNE);
                mMovingCreature->setAnimation(mAnimation.first, mAnimation.second);
                mJumpNow = true;
                mTimer = 0;
            }
        }
        return false;
    }

    bool JumpHighMovement::isDirectionPossible(Ogre::Vector3& direction) const
    {
        Vector3 oldDirection(direction);
        direction.x = 0;
        if (direction.y < 0)
            direction.y = 0;
        return oldDirection.x == 0 && oldDirection.y > 0;
    }

    bool JumpHighMovement::isRotationPossible(Ogre::Vector3& rotation) const
    {
        rotation = Vector3::ZERO;
        return false;
    }

    bool JumpHighMovement::canChangeToMovement(CreatureController::MovementType id)
    {
        return mState == DOWN;
    }
}
