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

#include <Ogre.h>
#include <stdexcept>
#include <errno.h>

#include "CoreSubsystem.h"
#include "UiSubsystem.h"
#include "RulesSubsystem.h"
#include "SoundSubsystem.h"
#include "DialogSubsystem.h"
#include "Exception.h"

#if OGRE_PLATFORM == PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

using namespace rl;

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
    
    /**@todo das nach RastullahApplication
     * und RastullahApplication nach Startup. */
	rl::CoreSubsystem* core = new rl::CoreSubsystem();
	Ogre::Log* log = Ogre::LogManager::getSingleton().getLog( "logs/rlCore.log" );
    log->logMessage("CoreSubsystem gestartet");
    
	rl::SoundSubsystem* sound = new rl::SoundSubsystem();
	log->logMessage("SoundSubsystem gestartet");
    
	rl::RulesSubsystem* rules = new rl::RulesSubsystem();
	log->logMessage("RulesSubsystem gestartet");
	
	rl::DialogSubsystem* dialog = new rl::DialogSubsystem();
	log->logMessage("DialogSubsystem gestartet");
    
	rl::UiSubsystem* ui = new rl::UiSubsystem();
	log->logMessage("UiSubsystem gestartet");


    try {
        core->startCore();
    } catch(Ogre::Exception& oe) {
        rl::showError(oe.getFullDescription());
    } catch(rl::Exception& re) {
        rl::showError(re.toString());
    } catch(std::runtime_error& rte) {
        rl::showError(rte.what());
    } catch(std::exception& exp) {
        rl::showError( exp.what() );
    } catch(std::string& err) {
        rl::showError( err );
    } catch(...) {
		rl::showError( "Unknown exception occured" );
    } 
    
    delete ui;
	delete dialog;
    delete rules;
    delete sound;
    delete core;


    return 0;
}
