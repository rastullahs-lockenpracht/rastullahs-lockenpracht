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

#ifndef __RL_START_ANIMATION_JOB_H__
#define __RL_START_ANIMATION_JOB_H__

#include "CorePrerequisites.h"
#include "Actor.h"
#include "Job.h"

namespace rl
{
    /// A rl#Job to play an animation.
    class _RlCoreExport StartAnimationJob : public Job
    {
    public:

        /**
        * Job for playing an animation on an actor.
        * @param actor  the actor to invoke the animation from. Animation will be played only, if
        *               the actor has a rl#MeshObject attached.
        * @param anim   the name of the animation to be played.
        * @param doLoop <code>true</code> if animation should be played as a loop,
        *               <code>false</code> else.
        * @param replaceAllAnims <code>true</code> if all other animations should be stopped,
        *                        when this animation is started, <code>false</code> else.
        */
        StartAnimationJob(const CeGuiString &id, Actor* actor, const Ogre::String& anim, bool doLoop=false,
            bool replaceAllAnims=true);
        virtual ~StartAnimationJob();

        virtual bool execute(Ogre::Real time);

    private:
        Actor* mActor;
        Ogre::String mAnimName;
        bool mLoop;
        bool mReplaceAllAnims;
    };
}

#endif
