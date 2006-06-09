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
#include "WindowFactory.h"

#include "OalDriver.h"
#include "OpenALConfig.h"
#include "SoundManager.h"

namespace Ogre
{
	using namespace rl;

	OalDriver* openaldriver;

	//Called from Ogre's dynload when loading plugins
	extern "C" void dllStartPlugin( void )
	{
		// Create the driver
		openaldriver = new OalDriver(SoundManager::getSingletonPtr());

		// Register driver
		SoundManager::getSingleton().registerDriver(openaldriver);

		// Register config window
		WindowFactory::getSingleton().registerSoundConfigWindow(new OpenALConfig(openaldriver));
	}

	//Called when unloading plugins
	extern "C" void dllStopPlugin( void )
	{
		//Just delete the driver :P
		delete openaldriver;
	}
}
