/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#include "PlayAnimationJob.h"
#include "MeshAnimation.h"
#include "MeshObject.h"

using namespace Ogre;

namespace rl
{

    PlayAnimationJob::PlayAnimationJob(Actor* actor, const Ogre::String& anim, Ogre::Real duration,
        bool replaceAllAnims)
        : Job(false, true, TimeSource::REALTIME_INTERRUPTABLE),
          mActor(actor),
          mAnimName(anim),
          mDuration(duration),
          mReplaceAllAnims(replaceAllAnims),
          mLoops(-1),
          mTimeToGo(0.0),
          mAnimation(NULL),
          mAnimationRunning(false)
    {
    }

    PlayAnimationJob::PlayAnimationJob(Actor* actor, const Ogre::String& anim, int loops,
        bool replaceAllAnims)
        : Job(false, true, TimeSource::REALTIME_INTERRUPTABLE),
          mActor(actor),
          mAnimName(anim),
          mLoops(loops),
          mDuration(-1),
          mReplaceAllAnims(replaceAllAnims),
          mTimeToGo(0.0),
          mAnimation(NULL),
          mAnimationRunning(false)
    {
    }

    PlayAnimationJob::~PlayAnimationJob()
    {
    }

    bool PlayAnimationJob::execute(Ogre::Real time)
    {
        if (mActor && !mAnimationRunning)
        {
            MeshObject* mo = dynamic_cast<MeshObject*>(mActor->getControlledObject());
            if (mDuration > 0)
            {
                mAnimation = mo->startAnimation(mAnimName, 1.0f, 0);
                mTimeToGo = mDuration;
                mAnimationRunning = true;
            }
            else
            {
                MeshAnimation* animation = mo->startAnimation(mAnimName, 1.0f, mLoops);
                mTimeToGo = animation->getLength() * (float)mLoops;
                mAnimationRunning = true;
            }

            if (mReplaceAllAnims)
            {
                mo->stopAllAnimationsExcept(mAnimName);
            }                
        }

        mTimeToGo -= time;

        if (mTimeToGo <= 0.0f)
        {
            if (mAnimation) 
            {
                mAnimation->pause();
                mAnimation = NULL;
            }
            return true;
        }

        return false;
    }
}
