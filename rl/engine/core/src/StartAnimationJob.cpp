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

#include "MeshObject.h"
#include "StartAnimationJob.h"

using namespace Ogre;

namespace rl
{

    StartAnimationJob::StartAnimationJob(Actor* actor, const Ogre::String& anim, bool doLoop, bool replaceAllAnims)
        : Job(false, true, TimeSource::REALTIME_INTERRUPTABLE)
        , mActor(actor)
        , mAnimName(anim)
        , mLoop(doLoop)
        , mReplaceAllAnims(replaceAllAnims)
    {
    }

    StartAnimationJob::~StartAnimationJob()
    {
    }

    bool StartAnimationJob::execute(Ogre::Real time)
    {
        if (mActor != NULL)
        {
            MeshObject* mo = dynamic_cast<MeshObject*>(mActor->getControlledObject());
            if (mReplaceAllAnims)
            {
                mo->stopAllAnimations();
            }
            mo->startAnimation(mAnimName, 1.0f, mLoop ? 0 : 1);
        }
        return true;
    }
}
