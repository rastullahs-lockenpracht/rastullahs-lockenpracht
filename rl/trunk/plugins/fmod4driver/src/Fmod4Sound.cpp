/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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
#include "Fmod4Sound.h"
#include "Fmod4SoundChannel.h"
#include "SoundManager.h"
#include "SoundResource.h"

using namespace Ogre;
using namespace boost;

namespace rl {
 
Fmod4Sound::Fmod4Sound(Fmod4Driver* driver, const rl::SoundResourcePtr &soundres, SoundType type)
  : Sound(soundres),
    mType(type),
    mSound(NULL),
    mDriver(driver)
{
}

FMOD::Sound* Fmod4Sound::_getFmodSound() const
{
    return mSound;
}

float Fmod4Sound::getLength() const
{
    unsigned int length;
    mSound->getLength(&length, FMOD_TIMEUNIT_MS);
    return (float)length/1000.0;
}

void Fmod4Sound::load()
{
    FMOD_MODE mode = FMOD_DEFAULT;

    if (is3d())
    {
        mode |= FMOD_3D;
    }
    else
    {
        mode |= FMOD_2D;
    }

    if (isLooping())
    {
        mode |= FMOD_LOOP_NORMAL;
    }
    else
    {
        mode |= FMOD_LOOP_OFF;
    }

    if (mType == Fmod4Sound::SAMPLE)
    {
        FMOD_RESULT res = mDriver->_getFmodSystem()->createSound(
            getSoundResource()->getName().c_str(), 
            mode, 
            NULL, 
            &mSound);

        mDriver->checkErrors(res);
    }
    else if (mType == Fmod4Sound::STREAM)
    {
        FMOD_RESULT res = mDriver->_getFmodSystem()->createStream(
            getSoundResource()->getName().c_str(), 
            mode, 
            NULL, 
            &mSound);

        mDriver->checkErrors(res);
    }
}

void Fmod4Sound::unload()
{
    mSound->release();
}

bool Fmod4Sound::isValid() const
{
    return mSound != NULL;
}


void Fmod4SoundPtr::destroy()
{
    SharedPtr<Fmod4Sound>::destroy();
}

} // Namespace
