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
#include "NullSoundStream.h"
#include "FmodSoundChannel.h"
#include "SoundManager.h"


using namespace Ogre;
using namespace boost;

namespace rl {
 

/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
NullSoundStream::NullSoundStream(const String &name):
    Sound(name)
{
}
 
/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
NullSoundStream::NullSoundStream(const SoundResourcePtr &soundres):
    Sound(soundres)
{
}

/**
 * @author JoSch
 * @date 07-04-2005
 */
NullSoundStream::~NullSoundStream()
{
    unload();
}


/**
 * @author JoSch
 * @date 07-12-2005
 */
void NullSoundStream::load() throw (RuntimeException)
{
    unsigned int mode = 0;
}

/**
 * @author JoSch
 * @date 07-22-2005
 */
void NullSoundStream::unload() throw (RuntimeException)
{
    getSoundResource()->unload();
}


/**
 * @return TRUE wenn der Sound unterbrochen wurde.
 * @author JoSch
 * @date 07-12-2005
 */
bool NullSoundStream::isValid() const throw (RuntimeException)
{
    return true;
}


void NullSoundStreamPtr::destroy()
{
    SharedPtr<NullSoundStream>::destroy();
}

} // Namespace
