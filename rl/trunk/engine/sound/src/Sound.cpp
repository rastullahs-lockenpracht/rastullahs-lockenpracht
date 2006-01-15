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
#include "Sound.h"
#include "SoundManager.h"
#include <boost/thread.hpp>


using namespace Ogre;
using namespace boost;

namespace rl {
 

/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 03-12-2005
 */
Sound::Sound(const String &name):
    mIs3d(true),
    mIsLooping(false)
{
    mName = name;
    mSoundResource = SoundResourcePtr(
        SoundManager::getSingleton().getByName(name));
}
 
/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 03-12-2005
 */
Sound::Sound(const SoundResourcePtr &soundres):
    mSoundResource(soundres),
    mIs3d(true),
    mIsLooping(false)
{
    if (!soundres.isNull())
    {
        mName = soundres->getName();
    }
}

/**
 * @author JoSch
 * @date 03-12-2005
 */
Sound::~Sound()
{
    mSoundResource->unload();
}


/**
 * @return TRUE wenn der Sound 3D sein soll.
 * @author JoSch
 * @date 07-12-2005
 */
bool Sound::is3d() const
{
    return mIs3d;
}

/**
 * @param is3d, ob der Sound 3D sein soll.
 * @author JoSch
 * @date 07-12-2005
 */
void Sound::set3d(bool is3d)
{
    mIs3d = is3d;
}

/**
 * @return TRUE wenn der Sound loopen soll.
 * @author JoSch
 * @date 08-25-2005
 */
bool Sound::isLooping() const
{
    return mIsLooping;
}

/**
 * @param is3d, ob der Sound loopen soll.
 * @author JoSch
 * @date 08-25-2005
 */
void Sound::setLooping(bool looping)
{
    mIsLooping = looping;
}

/**
 * @return der zugeordneten SoundResource.
 * @author JoSch
 * @date 07-12-2005
 */
SoundResourcePtr Sound::getSoundResource() const
{
    return mSoundResource;
}

/**
 * @return der Name der zugeordneten SoundResource.
 * @author JoSch
 * @date 08-22-2005
 */
const String& Sound::getName() const
{
    return mName;
}

void SoundPtr::destroy()
{
    SharedPtr<Sound>::destroy();
}

} // Namespace
