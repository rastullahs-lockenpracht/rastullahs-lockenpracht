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

#include "AnimationManager.h"

#include "Actor.h"
#include "Exception.h"
#include "FadeAnimation.h"
#include "MeshAnimation.h"
#include "ScriptWrapper.h"
#include "TrackAnimation.h"

template <> rl::AnimationManager* Ogre::Singleton<rl::AnimationManager>::msSingleton = 0;

namespace rl
{

    AnimationManager::AnimationManager()
        : GameTask(true)
        , mStateAnimationMap()
        , mFadeAnimSet()
        , mGlobalAnimationSpeed(1.0f)
    {
    }

    AnimationManager::~AnimationManager()
    {
        removeAllAnimations();
    }

    void AnimationManager::setGlobalAnimationSpeed(Ogre::Real speed)
    {
        mGlobalAnimationSpeed = speed;
    }

    Ogre::Real AnimationManager::getGlobalAnimationSpeed() const
    {
        return mGlobalAnimationSpeed;
    }

    void AnimationManager::setDefaultInterpolationMode(AnimationManager::InterpolationMode im)
    {
        Ogre::Animation::setDefaultInterpolationMode(Ogre::Animation::InterpolationMode(im));
    }

    AnimationManager::InterpolationMode AnimationManager::getDefaultInterpolationMode() const
    {
        return AnimationManager::InterpolationMode(Ogre::Animation::getDefaultInterpolationMode());
    }

    void AnimationManager::setDefaultRotationInterpolationMode(AnimationManager::RotationInterpolationMode rim)
    {
        Ogre::Animation::setDefaultRotationInterpolationMode(Ogre::Animation::RotationInterpolationMode(rim));
    }

    AnimationManager::RotationInterpolationMode AnimationManager::getDefaultRotationInterpolationMode() const
    {
        return AnimationManager::RotationInterpolationMode(Ogre::Animation::getDefaultRotationInterpolationMode());
    }

    MeshAnimation* AnimationManager::addMeshAnimation(
        Ogre::AnimationState* animState, MeshObject* mesh, Ogre::Real speed, unsigned int timesToPlay, bool paused)
    {
        StateAnimMap::iterator iter = mStateAnimationMap.find(animState);

        MeshAnimation* anim = 0;

        // Noch nicht vorhanden
        if (iter == mStateAnimationMap.end())
        {
            anim = new MeshAnimation(animState, mesh, speed, timesToPlay, paused);
            mStateAnimationMap.insert(std::pair<Ogre::AnimationState*, MeshAnimation*>(animState, anim));
            animState->setEnabled(true);
        }
        // Bereits vorhanden
        else
        {
            anim = dynamic_cast<MeshAnimation*>(iter->second);
            anim->resetTimesPlayed();
            anim->setTimesToPlay(timesToPlay);
            anim->setSpeed(speed);
            anim->setPaused(paused);
        }

        return anim;
    }

    TrackAnimation* AnimationManager::createTrackAnimation(Actor* actor, const Ogre::String& name, Ogre::Real length)
    {
        if (actor == 0)
            Throw(NullPointerException, "Actor darf nicht null sein");

        ///@todo Namen abfangen
        TrackAnimation* trackAnim = new TrackAnimation(name, actor, length);
        mStateAnimationMap.insert(
            std::pair<Ogre::AnimationState*, BaseAnimation*>(trackAnim->getAnimationState(), trackAnim));

        return trackAnim;
    }

    BaseAnimation* AnimationManager::getAnimation(Ogre::AnimationState* animState) const
    {
        StateAnimMap::const_iterator iter = mStateAnimationMap.find(animState);

        if (iter == mStateAnimationMap.end())
            return NULL;
        else
            return iter->second;
    }

    void AnimationManager::removeTrackAnimation(Actor* act, const Ogre::String& name)
    {
        StateAnimMap::iterator it;
        for (it = mStateAnimationMap.begin(); it != mStateAnimationMap.end();)
        {
            TrackAnimation* anim = dynamic_cast<TrackAnimation*>(it->second);

            if (anim != 0 && anim->getActor() == act && anim->getName() == name)
            {
                AnimationManager::stopAnimation(anim);
                ScriptWrapper::getSingleton().deleted(anim);
                delete anim;
                mStateAnimationMap.erase(it++); // ++i geht nicht
            }
            else
            {
                ++it; // Oder i++
            }
        }
    }

    bool AnimationManager::isStillInUse(BaseAnimation* anim) const
    {
        FadeAnimSet::const_iterator it;
        for (it = mFadeAnimSet.begin(); it != mFadeAnimSet.end(); it++)
        {
            if ((*it)->containsAnimation(anim))
                return true;
        }

        return false;
    }

    void AnimationManager::removeAllTrackAnimations(Actor* act)
    {
        StateAnimMap::iterator it;
        for (it = mStateAnimationMap.begin(); it != mStateAnimationMap.end();)
        {
            TrackAnimation* anim = dynamic_cast<TrackAnimation*>(it->second);

            if (anim != 0 && anim->getActor() == act)
            {
                AnimationManager::stopAnimation(anim);
                ScriptWrapper::getSingleton().deleted(anim);
                delete anim;
                mStateAnimationMap.erase(it++); // ++i geht nicht
            }
            else
            {
                ++it; // Oder i++
            }
        }
    }

    void AnimationManager::removeAllAnimations()
    {
        {
            FadeAnimSet::iterator it;
            for (it = mFadeAnimSet.begin(); it != mFadeAnimSet.end();)
            {
                FadeAnimation* anim = *it;
                AnimationManager::stopAnimation(anim);
                ScriptWrapper::getSingleton().deleted(anim);
                delete anim;
                it++;
            }
            mFadeAnimSet.clear();
        }
        {
            StateAnimMap::iterator it;
            for (it = mStateAnimationMap.begin(); it != mStateAnimationMap.end();)
            {
                BaseAnimation* anim = it->second;
                AnimationManager::stopAnimation(anim);
                ScriptWrapper::getSingleton().deleted(anim);
                delete anim;
                it++;
            }
            mStateAnimationMap.clear();
        }
    }

    void AnimationManager::removeAnimation(Ogre::AnimationState* animState)
    {
        StateAnimMap::iterator iter = mStateAnimationMap.find(animState);

        if (iter != mStateAnimationMap.end())
        {
            BaseAnimation* anim = iter->second;

            // Nicht loeschen, wird noch benutzt
            if (isStillInUse(anim))
                return;

            AnimationManager::stopAnimation(anim);
            mStateAnimationMap.erase(iter);
            ScriptWrapper::getSingleton().deleted(anim);
            delete anim;
        }
    }

    FadeAnimation* AnimationManager::fadeAnimation(MeshAnimation* from, MeshAnimation* to, Ogre::Real time)
    {
        RlAssert(from != NULL, "StartAnimation must not be NULL!");
        RlAssert(to != NULL, "EndAnimation must not be NULL!");
        RlAssert(time > 0.0, "Time must be greater then zero");

        FadeAnimation* anim = new FadeAnimation();

        anim->addAnimation(from, 0.0, time, 1.0, 0.0);
        anim->addAnimation(to, 0.0, time, 0.0, 1.0);
        anim->setPaused(false);

        mFadeAnimSet.insert(anim);

        return anim;
    }

    FadeAnimation* AnimationManager::fadeAnimation(
        MeshAnimation* fromLoop, MeshAnimation* blendAnim, MeshAnimation* toLoop, Ogre::Real loopDuration)
    {
        RlAssert(fromLoop != NULL, "StartAnimation must not be NULL!");
        RlAssert(blendAnim != NULL, "LoopAnimation must not be NULL!");
        RlAssert(toLoop != NULL, "EndAnimation must not be NULL!");
        RlAssert(loopDuration > 0.0, "Time must be greater then zero");

        FadeAnimation* anim = new FadeAnimation();

        if (loopDuration <= 0.0)
            loopDuration = blendAnim->getLength();

        anim->addAnimation(fromLoop, 0.0, 0.0, 1.0, 0.0);
        blendAnim->resetTimesPlayed();
        anim->addAnimation(blendAnim, 0.001, loopDuration + 0.001, 1.0, 0.0);
        anim->addAnimation(toLoop, loopDuration + 0.001, loopDuration + 0.002, 0.0, 1.0);
        anim->setPaused(false);

        mFadeAnimSet.insert(anim);
        return anim;
    }

    void AnimationManager::stopAnimation(BaseAnimation* anim)
    {
        anim->resetTimesPlayed();
        anim->setTimesToPlay(1);
        anim->setSpeed(1.0);
        anim->setPaused(true);
    }

    // @todo Check ob das das selbe MeshObject ist
    MeshAnimation* AnimationManager::replaceAnimation(
        MeshAnimation* oldAnim, Ogre::AnimationState* newAnimState, Ogre::Real speed, unsigned int timesToPlay)
    {
        removeAnimation(oldAnim);
        return addMeshAnimation(newAnimState, oldAnim->getMeshObject(), speed, timesToPlay);
    }

    void AnimationManager::removeAnimation(FadeAnimation* anim)
    {
        FadeAnimSet::iterator iter = mFadeAnimSet.find(anim);
        if (iter != mFadeAnimSet.end())
        {
            AnimationManager::stopAnimation(anim);
            mFadeAnimSet.erase(iter);
            ScriptWrapper::getSingleton().deleted(anim);
            delete anim;
        }
    }

    void AnimationManager::removeAnimation(MeshAnimation* anim)
    {
        removeAnimation(anim->getAnimationState());
    }

    void AnimationManager::removeAnimation(TrackAnimation* anim)
    {
        removeAnimation(anim->getAnimationState());
    }

    void AnimationManager::run(Ogre::Real timePassed)
    {
        // Zuerst faden (Weights modifizieren)
        for (FadeAnimSet::iterator it = mFadeAnimSet.begin(); it != mFadeAnimSet.end();)
        {
            FadeAnimation* anim = *it;
            anim->addTime(timePassed * mGlobalAnimationSpeed);

            if (anim->isDeleteOnFinish() && anim->getTimePlayed() > anim->getLength())
            {
                AnimationManager::stopAnimation(anim);
                ScriptWrapper::getSingleton().deleted(anim);
                delete anim;
                FadeAnimSet::iterator toDelete = it;
                it++;
                mFadeAnimSet.erase(toDelete);
            }
            else
                ++it;
        }

        // Dann normal animieren
        for (StateAnimMap::iterator it = mStateAnimationMap.begin(); it != mStateAnimationMap.end(); it++)
        {
            it->second->addTime(timePassed * mGlobalAnimationSpeed);
        }
    }

    const Ogre::String& AnimationManager::getName() const
    {
        static Ogre::String NAME = "AnimationManager";

        return NAME;
    }
}
