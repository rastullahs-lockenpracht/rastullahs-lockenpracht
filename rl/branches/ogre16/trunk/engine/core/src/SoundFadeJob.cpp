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

#include "SoundFadeJob.h"
#include "SoundFadeFunctor.h"
#include "SoundObject.h"
#include "Sound.h"

namespace rl
{
    SoundFadeJob::SoundFadeJob(SoundObject* soundObject, SoundFadeFunctor* fadeFunctor, bool destroyWhenDone)
        : Job(true, destroyWhenDone),
        mSoundObject(soundObject),
        mSoundFadeFunctor(fadeFunctor),
        mTime(0)
    {
        mStartVolume = mSoundObject->getSound()->getVolume();
    }

    bool SoundFadeJob::execute(Ogre::Real time)
    {
        // Precondition: mSoundObject != NULL
        if (mSoundObject == NULL)
        {
            discard();
            return true;
        }

        // add passed time to total time
        mTime += time;
        Ogre::Real volume = (*mSoundFadeFunctor)(mTime) * mStartVolume;
        // Are we done now?
        if (volume <= 0.0)
        {
            discard();
            return true;
        }
		mSoundObject->setVolume(volume);

        return false;
    }

    void SoundFadeJob::discard()
    {
        mSoundObject->setVolume(0.0);
        mSoundObject->stop();
    }

}
