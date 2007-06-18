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

#ifndef __RL_SOUND_FADE_JOB_H__
#define __RL_SOUND_FADE_JOB_H__

#include "CorePrerequisites.h"
#include <OgreSingleton.h>

#include "Job.h"

namespace rl {

    class SoundObject;
    class SoundFadeFunctor;

    class _RlCoreExport SoundFadeJob : public Job
    {
    public:
        SoundFadeJob(SoundObject* soundObject, SoundFadeFunctor* functor, bool destroyWhenDone);

        virtual bool execute(Ogre::Real time);
        virtual void discard();

    protected:
        /// The object we're operating on
        SoundObject* mSoundObject;
        /// The time passed since job start.
        Ogre::Real mTime;
        /// The starting volume for this job.
        Ogre::Real mStartVolume;
        /// Functor which calculates the fading curve.
        SoundFadeFunctor* mSoundFadeFunctor;
    };
}

#endif
