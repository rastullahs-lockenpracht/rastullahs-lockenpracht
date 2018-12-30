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

#include "JumpLongMovement.h"
#include "PhysicalThing.h"
#include "PhysicsManager.h"

using namespace std;
using namespace Ogre;

namespace rl
{
    JumpLongMovement::JumpLongMovement(CreatureController* creature)
        : AbstractMovement(creature)
        , mState(DOWN)
        , mWidth(0)
        , mJumpNow(false)
        , mTimer(0)
        , mApplyForceTime(0.12)
        , mApplyForceTimer(0)
        , mLastForce(Vector3::ZERO)
        , mVelocityBeforeJump(0)
        , mTanJumpAngle(Math::Tan(Degree(27)))
    {
        mAnimation = creature->getCreature()->getAnimation("weitsprung");
        mAnimationAbsprung = creature->getCreature()->getAnimation("weitsprung_absprung");
        mAnimationLandung = creature->getCreature()->getAnimation("weitsprung_landung");
        mAnimationForCollision = creature->getCreature()->getAnimation("rennen"); // wird das hier gebraucht?
    }

    void JumpLongMovement::activate()
    {
        AbstractMovement::activate();
        mState = DOWNTOUP;
        mMovingCreature->setAnimation(
            mAnimationAbsprung.first, mAnimationAbsprung.second, 1, mAnimationForCollision.first);
        mTimer = 0;
        calculateBaseVelocity(mWidth);

        // the person will only achieve this width if it is running
        // retrieve run movement
        AbstractMovement* run = mMovingCreature->getMovementFromId(CreatureController::MT_RENNEN);
        if (run != nullptr)
        {
            Real vel(0);
            run->calculateBaseVelocity(vel);
            Real factor = -mMovingCreature->getVelocity().z / vel;
            factor = std::max(Real(0), factor);
            // without moving before, the width will be 1/3
            mWidth = mWidth / 3. + mWidth * 2. / 3. * factor;
        }

        if (mMovingCreature->getCreature()->hasTalent("Athletik"))
        {
            int taw = mMovingCreature->getCreature()->doTalentprobe("Athletik", 0);
            if (taw > 0)
            {
                if (taw == RESULT_SPEKT_AUTOERFOLG)
                {
                    mWidth += mMovingCreature->getCreature()->getTalent("Athletik") / 25.0;
                }
                else if (taw == RESULT_AUTOERFOLG)
                {
                    mWidth += mMovingCreature->getCreature()->getTalent("Athletik") / 50.0;
                }
                else
                {
                    mWidth += taw / 50.0;
                }
            }
        }

        mMovingCreature->getCreature()->damageAu(2. / 3);
    }

    void JumpLongMovement::deactivate()
    {
        AbstractMovement::deactivate();
    }

    bool JumpLongMovement::calculateBaseVelocity(Real& velocity)
    {
        velocity = (mMovingCreature->getCreature()->getEigenschaft("GE")
                       + mMovingCreature->getCreature()->getEigenschaft("KK"))
            / 5.0;

        // steht nicht in den Regeln aber finde ich sinnvoll
        // velocityBase *= (1 - getrageneLast/KK);
        // steht in den Regeln: pro Ersch�pfung ein KK abziehen
        // if( mErschoepfung > getEigenschaft("KO") )
        //    velocity -= (mErschoepfung - getEigenschaft("KO")) / 5.0;
        // steht nicht in den Regeln, aber finde ich sinnvoll
        // if( getAu() < getAuBasis() / 3.0 )
        //    velocity -= (getAu() / getAuBasis() * 3.0) * getEigenschaft("GE") / 5.0;
        return isPossible();
    }

    bool JumpLongMovement::isPossible() const
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

    void JumpLongMovement::calculateForceAndTorque(Vector3& force, Vector3& torque, Real timestep)
    {
        Real mass;
        Vector3 inertia;
        OgreNewt::Body* body = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody();
        body->getMassMatrix(mass, inertia);

        if (mJumpNow && timestep != 0)
        {
            mApplyForceTimer += timestep;

            if (mApplyForceTimer == timestep // first time
                && mApplyForceTimer < mApplyForceTime) // this is handled below
            {
                Real t1 = mApplyForceTime;
                Real sx = mWidth;
                Real g = -PhysicsManager::getSingleton().getGravity().y;
                Real v0 = mVelocityBeforeJump;
                Real ax = 0;
                Real ay = 0;

                if (t1 != 0 || 2 * sx >= v0 * t1)
                {
                    Real v1x = (sx - v0 * t1 / 2) / (Math::Sqrt(mTanJumpAngle * (2 * sx / g - v0 * t1 / g)) + t1 / 2);
                    Real v1y = mTanJumpAngle * v1x;
                    ax = (v1x - v0) / t1;
                    ay = v1y / t1 + g;
                }

                mLastForce.x = 0;
                mLastForce.y = ay * mass;
                mLastForce.z = -ax * mass;
            }

            if (mApplyForceTimer < mApplyForceTime)
            {
                force = mLastForce;
            }
            else
            {
                mJumpNow = false;
                // adoption of the formula to our real jump time
                Real t1 = mApplyForceTimer;
                Real sx = mWidth;
                Real g = -PhysicsManager::getSingleton().getGravity().y;
                Real v0 = mVelocityBeforeJump;
                Real ax = 0;
                Real ay = 0;

                if (t1 != 0 || 2 * sx >= v0 * t1)
                {
                    Real v1x = (sx - v0 * t1 / 2) / (Math::Sqrt(mTanJumpAngle * (2 * sx / g - v0 * t1 / g)) + t1 / 2);
                    Real v1y = mTanJumpAngle * v1x;

                    Vector3 v_now = mMovingCreature->getVelocity();
                    ax = (v1x - -v_now.z) / timestep;
                    ay = (v1y - v_now.y) / timestep + g;
                }

                force.x = 0;
                force.y = ay * mass;
                force.z = -ax * mass;
            }
        }

        Vector3 omega = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody()->getOmega();
        torque = -omega / PhysicsManager::getSingleton().getMaxTimestep() * 2 * inertia;
    }

    bool JumpLongMovement::run(Ogre::Real elapsedTime, Ogre::Vector3 direction, Ogre::Vector3 rotation)
    {
        if (mState == DOWN)
        {
            mMovingCreature->setMovement(CreatureController::MT_STEHEN, direction, rotation);
        }
        if (mState == UPTODOWN)
        {
            mTimer += elapsedTime;
            if (mTimer >= 0.2f)
            {
                mState = DOWN;
                LOG_MESSAGE(Logger::RULES, "Weitsprung: DOWN");
            }
        }
        if (mState == UP)
        {
            mTimer += elapsedTime;
            if (mTimer < 0.2f)
            {
                mMovingCreature->setAbstractLocation(CreatureController::AL_AIRBORNE);
            }
            else if (mMovingCreature->getAbstractLocation() != CreatureController::AL_AIRBORNE)
            {
                mState = UPTODOWN;
                mMovingCreature->setAnimation(
                    mAnimationLandung.first, mAnimationLandung.second, 1, mAnimationForCollision.first);
                mTimer = 0;
                LOG_MESSAGE(Logger::RULES, "Weitsprung: UPTODOWN");
            }
        }
        if (mState == DOWNTOUP)
        {
            mTimer += elapsedTime;
            if (mTimer >= 0.3f)
            {
                mState = UP;
                mMovingCreature->setAbstractLocation(CreatureController::AL_AIRBORNE);
                mMovingCreature->setAnimation(mAnimation.first, mAnimation.second, 1, mAnimationForCollision.first);
                mJumpNow = true;
                mApplyForceTimer = 0;
                mVelocityBeforeJump = -mMovingCreature->getVelocity().z;
                mTimer = 0;
                LOG_MESSAGE(Logger::RULES, "Weitsprung: UP");
            }
        }
        return false;
    }

    bool JumpLongMovement::isDirectionPossible(Ogre::Vector3& direction) const
    {
        Vector3 oldDirection(direction);
        direction.x = 0;
        if (direction.y <= 0 || direction.z >= 0)
            direction.y = direction.z = 0;
        return oldDirection.x == 0 && oldDirection.z < 0 && oldDirection.y > 0;
    }

    bool JumpLongMovement::isRotationPossible(Ogre::Vector3& rotation) const
    {
        rotation = Vector3::ZERO;
        return false;
    }

    bool JumpLongMovement::canChangeToMovement(CreatureController::MovementType id)
    {
        return mState == DOWN;
    }
}
