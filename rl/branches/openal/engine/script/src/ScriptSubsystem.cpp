/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#include "TimerManager.h"
#include "ScriptObjectRepository.h"
#include "ScriptSubsystem.h"

using Ogre::Singleton;

template<> rl::ScriptSubsystem* Singleton<rl::ScriptSubsystem>::ms_Singleton = 0;

namespace rl {

	ScriptSubsystem& ScriptSubsystem::getSingleton()
	{
		return Singleton<ScriptSubsystem>::getSingleton();
	}

	ScriptSubsystem* ScriptSubsystem::getSingletonPtr()
	{
		return Singleton<ScriptSubsystem>::getSingletonPtr();
	}

	ScriptSubsystem::ScriptSubsystem()
	{
		new TimerManager();
        new ScriptObjectRepository();
	}

	ScriptSubsystem::~ScriptSubsystem()
	{
		delete TimerManager::getSingletonPtr();
        delete ScriptObjectRepository::getSingletonPtr();
	}

}