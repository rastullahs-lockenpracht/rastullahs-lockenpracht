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

#ifndef __RL_PLAY_SOUND_JOB_H__
#define __RL_PLAY_SOUND_JOB_H__

#include "CorePrerequisites.h"
#include "Job.h"

namespace rl
{
    class Sound;

    class _RlCoreExport PlaySound2dJob : public Job
    {
    public:

        PlaySound2dJob(const Ogre::String& sound, Ogre::Real volume=1.0f,
            bool looping=false, bool isDiscardable=false);
        virtual ~PlaySound2dJob();

        /// Play the sound.
        /// @return true, when the sound is done playing, false else.
        /// @todo this return-behaviour is actually a hack to prevent a memory leak.
        ///       We have to find a way to get rid of the sound automatically, when it is done
        ///       playing.
        virtual bool execute(Ogre::Real time);

        /// Do nothing, since the sound can be either played or not.
        /// Once started, it is not aborted.
        virtual void discard();
    private:
        Sound* mSound;
    };
}

#endif
