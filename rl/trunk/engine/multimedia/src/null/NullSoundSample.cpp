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
#include "NullSoundSample.h"
#include "NullSoundChannel.h"
#include "SoundManager.h"
#include "SoundResource.h"

using namespace Ogre;
using namespace boost;

namespace rl {
 
String NullSoundSample::msMovableType = "NullSoundSample";

/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
NullSoundSample::NullSoundSample(const String &name):
    Sound(name)
{
}
 
/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
NullSoundSample::NullSoundSample(const SoundResourcePtr &soundres):
    Sound(soundres)
{
}

/**
 * @author JoSch
 * @date 07-04-2005
 */
NullSoundSample::~NullSoundSample()
{
    unload();
}


/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& NullSoundSample::getMovableType() const
{
    return msMovableType;
}

/**
 * @author JoSch
 * @date 07-12-2005
 */
void NullSoundSample::load() throw (RuntimeException)
{
    getSoundResource()->load();
}

/**
 * @author JoSch
 * @date 07-22-2005
 */
void NullSoundSample::unload() throw (RuntimeException)
{
    getSoundResource()->unload(); // TODO Ist das wirklich nötig?
}

/**
 * @return TRUE wenn der Sound gueltig ist.
 * @author JoSch
 * @date 07-12-2005
 */
bool NullSoundSample::isValid() const throw (RuntimeException)
{
    return true;
}

/**
 * @return Der erzeugte Channel
 * @author JoSch
 * @date 08-08-2005
 */
SoundChannel *NullSoundSample::createChannel() throw (RuntimeException)
{
    NullSoundChannel *nullchannel = new NullSoundChannel(this, getName());
    return nullchannel; 
}



void NullSoundSamplePtr::destroy()
{
    SharedPtr<NullSoundSample>::destroy();
}

} // Namespace
