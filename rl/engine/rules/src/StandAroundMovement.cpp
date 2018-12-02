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

#include "StandAroundMovement.h"

#include "PhysicalThing.h"

using namespace std;
using namespace Ogre;

namespace rl
{
    StandAroundMovement::StandAroundMovement(CreatureController* creature)
        : AbstractMovement(creature)
        , mVelocity(Vector3::ZERO)
        , mRotationMovement(nullptr)
        , mStepRecognitionMovement(nullptr)
    {
        mAnimStehenLinks = creature->getCreature()->getAnimation("stehen_links");
        mAnimStehenRechts = creature->getCreature()->getAnimation("stehen_rechts");
        mAnimStehen = creature->getCreature()->getAnimation("stehen");
    }

    StandAroundMovement::~StandAroundMovement()
    {
    }

    void StandAroundMovement::activate()
    {
        AbstractMovement::activate();
        getRotationMovement()->activate();
        getStepRecognitionMovement()->activate();
    }

    void StandAroundMovement::deactivate()
    {
        AbstractMovement::deactivate();
        getRotationMovement()->deactivate();
        getStepRecognitionMovement()->deactivate();
    }

    bool StandAroundMovement::calculateBaseVelocity(Real& velocity)
    {
        velocity = 0.0f;
        return isPossible();
    }

    bool StandAroundMovement::isPossible() const
    {
        return mMovingCreature->getAbstractLocation() == CreatureController::AL_FLOOR
            && mMovingCreature->getCreature()->getAu() > 0
            && !(mMovingCreature->getCreature()->getLifeState() & Effect::LS_IMMOBILE);
    }

    void StandAroundMovement::calculateForceAndTorque(Vector3& force, Vector3& torque, Real timestep)
    {
        getRotationMovement()->calculateForceAndTorque(force, torque, timestep);

        Real mass;
        Vector3 inertia;
        OgreNewt::Body* body = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody();
        body->getMassMatrix(mass, inertia);

        Vector3 vel = mMovingCreature->getVelocity();
        vel.y = 0;
        Real delay = 0.05; //(2 * PhysicsManager::getSingleton().getMaxTimestep());
        // if(vel.squaredLength() > mVelocity.squaredLength())
        //    delay *= 1.5;
        Vector3 diff = (mVelocity - vel);
        force = mass * diff / delay;
        // additional remove movement in wrong direction
        force += mass * (mVelocity.normalisedCopy() * vel.length() - vel)
            / delay; // do we need this, does this improve the movement behaviour
        // (changing direction during slow movement makes char slide sideways)
        force.y = 0;

        getStepRecognitionMovement()->calculateForceAndTorque(force, torque, timestep);
    }

    bool StandAroundMovement::run(Ogre::Real elapsedTime, Ogre::Vector3 direction, Ogre::Vector3 rotation)
    {
        direction.normalise();
        Real velocity;
        calculateBaseVelocity(velocity);
        mVelocity = direction * velocity;
        applyAuChanges(elapsedTime);
        setAnimation(elapsedTime);
        bool ret = false;
        if (getRotationMovement()->isPossible())
            if (getRotationMovement()->run(elapsedTime, direction, rotation))
                ret = true;
        if (getStepRecognitionMovement()->isPossible())
            if (getStepRecognitionMovement()->run(elapsedTime, direction, rotation))
                ret = true;
        return ret;
    }

    void StandAroundMovement::setAnimation(Ogre::Real elapsedTime)
    {
        Real omegaY = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody()->getOmega().y;
        if (omegaY > Degree(20).valueRadians())
            mMovingCreature->setAnimation(mAnimStehenRechts.first, mAnimStehenRechts.second);
        else
        {
            if (omegaY < Degree(-20).valueRadians())
                mMovingCreature->setAnimation(mAnimStehenLinks.first, mAnimStehenLinks.second);
            else
                mMovingCreature->setAnimation(mAnimStehen.first, mAnimStehen.second);
        }
    }

    void StandAroundMovement::applyAuChanges(Ogre::Real elapsedTime)
    {
        mMovingCreature->getCreature()->regenerateAu(2, 0.5, elapsedTime);
    }

    bool StandAroundMovement::isDirectionPossible(Ogre::Vector3& direction) const
    {
        Vector3 oldDirection(direction);
        direction = Vector3::ZERO;
        return oldDirection == Vector3::ZERO;
    }

    bool StandAroundMovement::isRotationPossible(Ogre::Vector3& rotation) const
    {
        return getRotationMovement()->isRotationPossible(rotation);
    }
}
