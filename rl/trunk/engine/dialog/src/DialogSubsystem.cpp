/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#include "AimlProcessorManager.h"
#include "DialogSubsystem.h"

#include "OgreLogManager.h"

template<> rl::DialogSubsystem* Singleton<rl::DialogSubsystem>::ms_Singleton = 0;

namespace rl 
{
	DialogSubsystem& DialogSubsystem::getSingleton(void)
	{
		return Singleton<DialogSubsystem>::getSingleton();
	}

	DialogSubsystem* DialogSubsystem::getSingletonPtr(void)
	{
		return Singleton<DialogSubsystem>::getSingletonPtr();
	}

	DialogSubsystem::DialogSubsystem()
	{
		Log* log = LogManager::getSingleton().createLog( "logs/rlDialog.log" );
        log->setLogDetail( LL_BOREME );
		log->logMessage("Test");
		AimlProcessorManager::addStandardProcessors();
//		Log* log = LogManager::getSingleton().createLog( "rlDialog.log" );
//      log->setLogDetail( LL_BOREME );
//		log->logMessage("Test");
	}

    DialogSubsystem::~DialogSubsystem() 
    {  

    }
}
