/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "OpenALSoundSample.h"

#include "OpenALDriver.h"
#include "SoundManager.h"
#include "SoundResource.h"

using namespace Ogre;
using namespace boost;

namespace rl {
 
/**
 * @param name Der Name des Sounds.
 * @author JoSch
 * @date 07-04-2005
 */
OpenALSoundSample::OpenALSoundSample(OpenALDriver* driver, const SoundResourcePtr &soundres):
    OpenALSound(driver, soundres)
{
}

/**
 * @author JoSch
 * @date 07-04-2005
 */
OpenALSoundSample::~OpenALSoundSample()
{
    unload();
}

/**
 * @author JoSch
 * @date 07-12-2005
 */
void OpenALSoundSample::load() throw (RuntimeException)
{
    getSoundResource()->load();
}

/**
 * @author JoSch
 * @date 07-22-2005
 */
void OpenALSoundSample::unload() throw (RuntimeException)
{
    getSoundResource()->unload(); // TODO Ist das wirklich nötig?
}

float OpenALSoundSample::getLength() const
{
	return 0.0;
}

void OpenALSoundSamplePtr::destroy()
{
    SharedPtr<OpenALSoundSample>::destroy();
}

} // Namespace
