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

#include "Fmod4Config.h"
#include "Fmod4Driver.h"
#include "SoundManager.h"


namespace Ogre
{
	using namespace rl;

	Fmod4Driver* fmod4driver = NULL;
    Fmod4Config* fmod4config = NULL;

	//Called from Ogre's dynload when loading plugins
	extern "C" void dllStartPlugin( void )
	{
		fmod4driver = new Fmod4Driver(SoundManager::getSingletonPtr());
		SoundManager::getSingleton().registerDriver(fmod4driver);

		// Register config window
		///\todo This ain't working. Dunno why, don't care right now.
        //fmod4config = new Fmod4Config(fmod4driver);
		//WindowFactory::getSingleton().registerSoundConfigComponent(fmod4config);
	}

	//Called when unloading plugins
	extern "C" void dllStopPlugin( void )
	{
        delete fmod4config;

        delete fmod4driver;
	}
}
