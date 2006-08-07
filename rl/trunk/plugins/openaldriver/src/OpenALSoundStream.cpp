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
#include "OpenALSoundStream.h"

#include "OpenALDriver.h"
#include "OpenALSoundChannel.h"
#include "SoundManager.h"


using namespace Ogre;
using namespace boost;

namespace rl {
 

/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
OpenALSoundStream::OpenALSoundStream(OpenALDriver* driver, const SoundResourcePtr &soundres):
    Sound(soundres),
	mDriver(driver),
	mChannel(NULL)
{
}

/**
 * @author JoSch
 * @date 07-04-2005
 */
OpenALSoundStream::~OpenALSoundStream()
{
    unload();
}


/**
 * @author JoSch
 * @date 07-12-2005
 */
void OpenALSoundStream::load() throw (RuntimeException)
{
    getSoundResource()->load(); // TODO Ist das wirklich nötig?
}

/**
 * @author JoSch
 * @date 07-22-2005
 */
void OpenALSoundStream::unload() throw (RuntimeException)
{
    getSoundResource()->unload();
}


/**
 * @return TRUE wenn der Sound unterbrochen wurde.
 * @author JoSch
 * @date 07-12-2005
 */
bool OpenALSoundStream::isValid() const throw (RuntimeException)
{
    return true;
}


/**
 * @return Der erzeugte Channel
 * @author JoSch
 * @date 08-08-2005
 */
SoundChannel *OpenALSoundStream::createChannel() throw (RuntimeException)
{
	if (mChannel == NULL)
	{
		mChannel = new OpenALSoundChannel(mDriver, this, getName());
	}
    return mChannel; 
}

float OpenALSoundStream::getLength() const
{
	return mChannel->getLength();
}


void OpenALSoundStreamPtr::destroy()
{
    SharedPtr<OpenALSoundStream>::destroy();
}

} // Namespace
