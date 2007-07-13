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

#include "PlaySoundJob.h"
#include "Sound.h"
#include "SoundManager.h"
#include "SoundDriver.h"

using namespace Ogre;

namespace rl
{

    PlaySound2dJob::PlaySound2dJob(const String& sound, Real volume, int priority)
        : Job(false, true), mSound(NULL), mExecuted(false)
    {
        mSound = SoundManager::getSingleton().createSound(sound, ST_SAMPLE);
        mSound->set3d(false);
        mSound->setVolume(volume);
        mSound->setLooping(false);
        mSound->setPriority(priority);
    }

    PlaySound2dJob::~PlaySound2dJob()
    {
        // Only destroy sound, if not played,
        // because else the sound gets destroyed automatically.
        if(!mExecuted)
        {
            SoundManager::getSingleton().destroySound(mSound);
        }
    }

    bool PlaySound2dJob::execute(Ogre::Real time)
    {
        if (mSound) mSound->play(true);
        mExecuted = true;
        return true;
    }

    PlaySound3dJob::PlaySound3dJob(const String& sound, const Ogre::Vector3& pos, Real volume, int priority)
        : Job(false, true), mSound(NULL), mExecuted(false)
    {
        mSound = SoundManager::getSingleton().createSound(sound, ST_SAMPLE);
        mSound->set3d(true);
        mSound->setPosition(pos);
        mSound->setVolume(volume);
        mSound->setLooping(false);
        mSound->setPriority(priority);
    }

    PlaySound3dJob::~PlaySound3dJob()
    {
        // Only destroy sound, if not played,
        // because else the sound gets destroyed automatically.
        if(!mExecuted)
        {
            SoundManager::getSingleton().destroySound(mSound);
        }
    }

    bool PlaySound3dJob::execute(Ogre::Real time)
    {
        if (mSound) mSound->play(true);
        mExecuted = true;
        return true;
    }
}
