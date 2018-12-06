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

#ifndef __RL_PLAY_ANIMATION_JOB_H__
#define __RL_PLAY_ANIMATION_JOB_H__

#include "CorePrerequisites.h"
#include "Actor.h"
#include "Job.h"

namespace rl
{
    class MeshAnimation;

    /// A rl#Job to play an animation.
    class _RlCoreExport PlayAnimationJob : public Job
    {
    public:

        /**
        * Job for playing an animation on an actor.
        * @param actor  the actor to invoke the animation from. Animation will be played only, if
        *               the actor has a rl#MeshObject attached.
        * @param anim   the name of the animation to be played.
        * @param duration the time the animation will be played, if below zero, the animation will be played exactly 1 time
        * @param replaceAllAnims <code>true</code> if all other animations should be stopped,
        *                        when this animation is started, <code>false</code> else.
        */
        PlayAnimationJob(Actor* actor, const Ogre::String& anim, Ogre::Real duration,
            bool replaceAllAnims=true);

        /**
        * Job for playing an animation on an actor.
        * @param actor  the actor to invoke the animation from. Animation will be played only, if
        *               the actor has a rl#MeshObject attached.
        * @param anim   the name of the animation to be played.
        * @param loops  how often the will animation be played
        * @param replaceAllAnims <code>true</code> if all other animations should be stopped,
        *                        when this animation is started, <code>false</code> else.
        */
        PlayAnimationJob(Actor* actor, const Ogre::String& anim, int loops = 1,
            bool replaceAllAnims=true);

        virtual ~PlayAnimationJob();

        virtual bool execute(Ogre::Real time);

        virtual void setHoldOnEnd(bool hold);

    private:
        Actor* mActor;
        Ogre::String mAnimName;
        Ogre::Real mDuration;
        int mLoops;
        Ogre::Real mTimeToGo;
        bool mReplaceAllAnims;
        MeshAnimation* mAnimation;
        bool mAnimationRunning;
        bool mHoldOnEnd;
    };
}

#endif
