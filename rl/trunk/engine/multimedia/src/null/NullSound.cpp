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
#include "NullSound.h"
#include "NullSoundChannel.h"
#include "SoundManager.h"
#include "SoundResource.h"
#include "VorbisFile.h"

using namespace Ogre;
using namespace boost;

namespace rl {
 
String NullSound::msMovableType = "NullSound";

/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
NullSound::NullSound(const String &name):
    Sound(name)
{
    mTotalTime.sec = mTotalTime.nsec = 0;
}
 
/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
NullSound::NullSound(const SoundResourcePtr &soundres):
    Sound(soundres)
{
}

/**
 * @author JoSch
 * @date 07-04-2005
 */
NullSound::~NullSound()
{
    unload();
}


/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& NullSound::getMovableType() const
{
    return msMovableType;
}

/**
 * @author JoSch
 * @date 07-12-2005
 */
void NullSound::load() throw (RuntimeException)
{
    getSoundResource()->load();
    VorbisFile file(getSoundResource());
    mTotalTime = file.getTotalTime();
    getSoundResource()->unload();
}

/**
 * @author JoSch
 * @date 07-22-2005
 */
void NullSound::unload() throw (RuntimeException)
{
    if (getSoundResource()->isLoaded())
    {
        getSoundResource()->unload();
    }
}

/**
 * @return TRUE wenn der Sound gueltig ist.
 * @author JoSch
 * @date 07-12-2005
 */
bool NullSound::isValid() const throw (RuntimeException)
{
    return getSoundResource()->isLoaded();
}

/**
 * @return Der erzeugte Channel
 * @author JoSch
 * @date 08-08-2005
 */
SoundChannel *NullSound::createChannel() throw (RuntimeException)
{
    NullSoundChannel *nullchannel = new NullSoundChannel(this, getName());
    return nullchannel; 
}

/**
 * @return Die gesamte Spiellänge des Sounds
 * @author JoSch
 * @date 03-18-2005
 */
const boost::xtime &NullSound::getTotalTime() const
{
    return mTotalTime;
}

void NullSoundPtr::destroy()
{
    SharedPtr<NullSound>::destroy();
}

} // Namespace
