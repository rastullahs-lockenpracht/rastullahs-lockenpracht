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

#include "TrackAnimation.h"

#include "Exception.h"

#include "Actor.h"
#include "CoreSubsystem.h"
#include "World.h"

namespace rl
{

    TrackAnimation::TrackAnimation(const Ogre::String& name, Actor* act, Ogre::Real length)
        : BaseAnimation(length, 1.0, 0, true)
        , mActor(act)
    {
        Ogre::SceneManager* mgr = CoreSubsystem::getSingleton().getWorld()->getSceneManager();

        try
        {
            // Gibt es die Animation schon?
            mAnimation = mgr->createAnimation(name, length);
        }
        catch (Ogre::Exception&)
        {
            Throw(RuntimeException, "Eine Animation mit dem Namen '" + name + "' war schon vorhanden.");
        }

        act->_getSceneNode()->setInitialState();
        mAnimationTrack = mAnimation->createNodeTrack(0, act->_getSceneNode());
        setAnimationState(mgr->createAnimationState(name));
    }

    TrackAnimation::~TrackAnimation()
    {
        Ogre::SceneManager* mgr = CoreSubsystem::getSingleton().getWorld()->getSceneManager();
        mAnimState->setEnabled(false);
        mAnimationTrack->getAssociatedNode()->resetToInitialState();
        mAnimation->destroyNodeTrack(0);
        mgr->destroyAnimationState(mAnimation->getName());
        mgr->destroyAnimation(mAnimation->getName());
    }

    void TrackAnimation::setAnimationState(Ogre::AnimationState* animState)
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

    void TrackAnimation::doAddTime(Ogre::Real timePassed)
    {
        mAnimState->addTime(timePassed);

        if (mActor != NULL)
            mActor->_update();
    }

    const Ogre::String& TrackAnimation::getName() const
    {
        return mAnimation->getName();
    }

    void TrackAnimation::addKeyFrame(Ogre::Real timePos)
    {
        mAnimationTrack->createKeyFrame(timePos);
    }

    void TrackAnimation::setKeyFrameTranslation(Ogre::Real timePos, Ogre::Real xPos, Ogre::Real yPos, Ogre::Real zPos)
    {
        Ogre::TransformKeyFrame* frame = getKeyFrameAtTimePos(timePos);

        if (frame != NULL)
            frame->setTranslate(Ogre::Vector3(xPos, yPos, zPos));
    }

    void TrackAnimation::setKeyFrameRotationQuaternion(
        Ogre::Real timePos, Ogre::Real w, Ogre::Real x, Ogre::Real y, Ogre::Real z)
    {
        Ogre::TransformKeyFrame* frame = getKeyFrameAtTimePos(timePos);

        if (frame != NULL)
            frame->setRotation(Ogre::Quaternion(w, x, y, z));
    }

    void TrackAnimation::setKeyFrameRotation(
        Ogre::Real timePos, Ogre::Real xRotAxis, Ogre::Real yRotAxis, Ogre::Real zRotAxis, Ogre::Real angleUnits)
    {
        Ogre::TransformKeyFrame* frame = getKeyFrameAtTimePos(timePos);

        if (frame != NULL)
            frame->setRotation(
                Ogre::Quaternion(Ogre::Radian(Ogre::Degree(angleUnits)), Ogre::Vector3(xRotAxis, yRotAxis, zRotAxis)));
    }

    void TrackAnimation::setKeyFrameScale(Ogre::Real timePos, Ogre::Real xScale, Ogre::Real yScale, Ogre::Real zScale)
    {
        Ogre::TransformKeyFrame* frame = getKeyFrameAtTimePos(timePos);

        if (frame != NULL)
            frame->setScale(Ogre::Vector3(xScale, yScale, zScale));
    }

    void TrackAnimation::setInterpolationMode(AnimationManager::InterpolationMode im)
    {
        mAnimation->setInterpolationMode(Ogre::Animation::InterpolationMode(im));
    }

    AnimationManager::InterpolationMode TrackAnimation::getInterpolationMode() const
    {
        return AnimationManager::InterpolationMode(mAnimation->getInterpolationMode());
    }

    void TrackAnimation::setRotationInterpolationMode(AnimationManager::RotationInterpolationMode im)
    {
        mAnimation->setRotationInterpolationMode(Ogre::Animation::RotationInterpolationMode(im));
    }

    AnimationManager::RotationInterpolationMode TrackAnimation::getRotationInterpolationMode() const
    {
        return AnimationManager::RotationInterpolationMode(mAnimation->getRotationInterpolationMode());
    }

    void TrackAnimation::setUseShortestRotationPath(bool useShortestPath)
    {
        mAnimationTrack->setUseShortestRotationPath(useShortestPath);
    }

    bool TrackAnimation::getUseShortestRotationPath() const
    {
        return mAnimationTrack->getUseShortestRotationPath();
    }

    Actor* TrackAnimation::getActor() const
    {
        return mActor;
    }

    Ogre::TransformKeyFrame* TrackAnimation::getKeyFrameAtTimePos(Ogre::Real timePos)
    {
        // 0 ist bereits definiert
        if (timePos == 0.0)
            return mAnimationTrack->getNodeKeyFrame(0);

        Ogre::KeyFrame* frame1;
        Ogre::KeyFrame* frame2;

        mAnimationTrack->getKeyFramesAtTime(timePos, &frame1, &frame2);

        return static_cast<Ogre::TransformKeyFrame*>(frame1);
    }

    Ogre::AnimationState* TrackAnimation::getAnimationState() const
    {
        return mAnimState;
    }
}
