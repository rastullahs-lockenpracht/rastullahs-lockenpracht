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
#include "OalSoundSample.h"
#include "OalSoundChannel.h"
#include "SoundManager.h"
#include "SoundResource.h"

using namespace Ogre;
using namespace boost;

namespace rl {
 
String OalSoundSample::msMovableType = "OalSoundSample";

/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
OalSoundSample::OalSoundSample(const String &name):
    Sound(name)
{
}
 
/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
OalSoundSample::OalSoundSample(const SoundResourcePtr &soundres):
    Sound(soundres)
{
}

/**
 * @author JoSch
 * @date 07-04-2005
 */
OalSoundSample::~OalSoundSample()
{
    unload();
}


/**
 * @author JoSch
 * @date 03-11-2005
 * @return Den Objekttypen
 */
const String& OalSoundSample::getMovableType() const
{
    return msMovableType;
}

/**
 * @author JoSch
 * @date 07-12-2005
 */
void OalSoundSample::load() throw (RuntimeException)
{
    getSoundResource()->load();
}

/**
 * @author JoSch
 * @date 07-22-2005
 */
void OalSoundSample::unload() throw (RuntimeException)
{
    getSoundResource()->unload(); // TODO Ist das wirklich nötig?
}

/**
 * @return TRUE wenn der Sound gueltig ist.
 * @author JoSch
 * @date 07-12-2005
 */
bool OalSoundSample::isValid() const throw (RuntimeException)
{
    return true;
}

/**
 * @return Der erzeugte Channel
 * @author JoSch
 * @date 08-08-2005
 */
SoundChannel *OalSoundSample::createChannel() throw (RuntimeException)
{
    OalSoundChannel *nullchannel = new OalSoundChannel(this, getName());
    return nullchannel; 
}



void OalSoundSamplePtr::destroy()
{
    SharedPtr<OalSoundSample>::destroy();
}

} // Namespace
