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

#include "MeshAnimation.h"

#include "ActorManager.h"
#include "AnimationManager.h"
#include "Exception.h"
#include "MeshObject.h"
#include "ScriptWrapper.h"

using namespace Ogre;

namespace rl
{

    MeshAnimation::MeshAnimation(
        Ogre::AnimationState* animState, MeshObject* mesh, Ogre::Real speed, unsigned int timesToPlay, bool paused)
        : BaseAnimation(animState->getLength(), speed, timesToPlay, paused)
        , mMeshObject(mesh)
    {
        setAnimationState(animState);

        mAnimation = static_cast<Entity*>(mMeshObject->getMovableObject())
                         ->getSkeleton()
                         ->getAnimation(animState->getAnimationName());
    }

    MeshAnimation::~MeshAnimation()
    {
        if (mAnimState != NULL)
            mAnimState->setEnabled(false);
    }

    void MeshAnimation::setAnimationState(Ogre::AnimationState* animState)
    {
        if (animState == 0)
            Throw(NullPointerException, "Ogre::AnimationState darf nicht null sein");

        mAnimState = animState;

        if (mTimesToPlay != 1)
            mAnimState->setLoop(true);

        // Wenn die Zeit negativ ist, beginnen wir am Ende
        if (mSpeed < 0)
            mAnimState->setTimePosition(mAnimState->getLength());

        mAnimState->setEnabled(true);
    }

    void MeshAnimation::setSpeed(Ogre::Real speed)
    {
        // Eventuell auf Anfang/Ende setzen
        if (speed < 0 && mAnimState->getTimePosition() == 0)
            mAnimState->setTimePosition(mAnimState->getLength());
        else if (speed > 0 && mAnimState->getTimePosition() == mAnimState->getLength())
            mAnimState->setTimePosition(0);

        BaseAnimation::setSpeed(speed);
    }

    MeshObject* MeshAnimation::getMeshObject()
    {
        return mMeshObject;
    }

    void MeshAnimation::resetTimesPlayed()
    {
        // Zurueckspulen
        if (mSpeed < 0)
            mAnimState->setTimePosition(mLength);
        else if (mSpeed > 0)
            mAnimState->setTimePosition(0);

        BaseAnimation::resetTimesPlayed();
    }

    // Gewicht (Einfluss) der Animation
    Ogre::Real MeshAnimation::getWeight(void) const
    {
        return mAnimState->getWeight();
    }

    void MeshAnimation::setWeight(Ogre::Real weight)
    {
        mAnimState->setWeight(weight);

        if (weight > 0.0 && !mAnimState->getEnabled())
            mAnimState->setEnabled(true);
        else if (weight <= 0.00001 && mAnimState->getEnabled())
            mAnimState->setEnabled(false);
    }

    void MeshAnimation::doAddTime(Ogre::Real timePassed)
    {
        mAnimState->addTime(timePassed);
    }

    void MeshAnimation::setLoop(bool loop)
    {
        mAnimState->setLoop(loop);
    }

    Ogre::AnimationState* MeshAnimation::getAnimationState() const
    {
        return mAnimState;
    }
}
