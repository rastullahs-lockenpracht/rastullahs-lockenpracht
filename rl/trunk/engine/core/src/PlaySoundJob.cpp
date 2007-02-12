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

#include "PlaySoundJob.h"
#include "Sound.h"
#include "SoundManager.h"
#include "SoundDriver.h"

using namespace Ogre;

namespace rl
{

    PlaySound2dJob::PlaySound2dJob(const String& sound, Real volume,
        bool looping, bool isDiscardable) : Job(isDiscardable, true), mSound()
    {
        mSound = SoundManager::getSingleton().getActiveDriver()->
            createStream(SoundManager::getSingleton().getByName(sound));
        mSound->set3d(false);
        mSound->setVolume(volume);
        mSound->setLooping(looping);
    }

    PlaySound2dJob::~PlaySound2dJob()
    {
        delete mSound;
    }

    bool PlaySound2dJob::execute(Ogre::Real time)
    {
        if (mSound) mSound->play();
        return true;
    }

    void PlaySound2dJob::discard()
    {
    }
}
