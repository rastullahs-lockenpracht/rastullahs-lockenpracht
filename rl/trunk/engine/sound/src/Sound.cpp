/* Sound.cpp - Diese Klassse kapselt einen Sound.
 * (C) 2004. Team Pantheon. www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
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
    mIs3d(true)
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
    mIs3d(true)
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
 * @return der zugeordneten SoundResource.
 * @author JoSch
 * @date 07-12-2005
 */
SoundResourcePtr Sound::getSoundResource() const
{
    return mSoundResource;
}


void SoundPtr::destroy()
{
    SharedPtr<Sound>::destroy();
}

} // Namespace
