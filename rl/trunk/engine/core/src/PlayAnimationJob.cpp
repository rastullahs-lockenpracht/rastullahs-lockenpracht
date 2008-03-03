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
          mTimeToGo(0.0),
          mAnimation(NULL)
    {
    }

    PlayAnimationJob::~PlayAnimationJob()
    {
    }

    bool PlayAnimationJob::execute(Ogre::Real time)
    {
        if (!mAnimation)
        {
            if (mActor)
            {
                MeshObject* mo = dynamic_cast<MeshObject*>(mActor->getControlledObject());
                if (mReplaceAllAnims)
                {
                    mo->stopAllAnimations();
                }
                
                if (mDuration < 0.0f)
                {
                    mAnimation = mo->startAnimation(mAnimName, 1.0f, 1);
                    mTimeToGo = mAnimation->getLength();
                }
                else
                {
                    mAnimation = mo->startAnimation(mAnimName, 1.0f, 0);
                    mTimeToGo = mDuration;
                }
            }
        }

        mTimeToGo -= time;

        if (mTimeToGo <= 0.0f)
        {
            if (mAnimation) 
            {
                mAnimation->pause();
            }
            return true;
        }

        return false;
    }
}
