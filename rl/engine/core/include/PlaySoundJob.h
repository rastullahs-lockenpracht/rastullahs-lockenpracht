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

#ifndef __RL_PLAY_SOUND_JOB_H__
#define __RL_PLAY_SOUND_JOB_H__

#include "CorePrerequisites.h"
#include "Job.h"
#include "Sound.h"

namespace rl
{

    class _RlCoreExport PlaySound2dJob : public Job
    {
    public:

        PlaySound2dJob(const Ogre::String& sound, Ogre::Real volume=1.0f, int priority = Sound::PRIO_MUSIC);
        virtual ~PlaySound2dJob();

        /// Always returns TRUE, because sound will autodestruct.
        virtual bool execute(Ogre::Real time);

    private:
        Sound* mSound;
        bool mExecuted;
    };

    class _RlCoreExport PlaySound3dJob : public Job
    {
    public:

        PlaySound3dJob(const Ogre::String& sound, const Ogre::Vector3& pos, Ogre::Real volume=1.0f, int priority = Sound::PRIO_SFX_ENVIRONMENT);
        virtual ~PlaySound3dJob();

        /// Always returns TRUE, because sound will autodestruct.
        virtual bool execute(Ogre::Real time);

    private:
        Sound* mSound;
        bool mExecuted;
    };
}

#endif
