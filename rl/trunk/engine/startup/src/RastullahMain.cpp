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

#include <xercesc/util/XMemory.hpp>	// Muss vor Ogre stehen (zumindest für VS)

#include <Ogre.h>
#include <OgreLogManager.h>
#include <stdexcept>
#include <errno.h>


#include "CoreSubsystem.h"
#include "SoundSubsystem.h"
#include "RulesSubsystem.h"
#include "DialogSubsystem.h"
#include "UiSubsystem.h"
#include "ScriptSubsystem.h"

#include "Exception.h"
#include <CEGUIExceptions.h>


#if OGRE_PLATFORM == PLATFORM_WIN32

#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
	rl::CoreSubsystem* core = NULL;
	rl::SoundSubsystem* sound = NULL;
	rl::RulesSubsystem* rules = NULL;
	rl::DialogSubsystem* dialog = NULL;
	rl::UiSubsystem* ui =  NULL;
	rl::ScriptSubsystem* script =  NULL;

	try {

		/**@todo das nach RastullahApplication
		* und RastullahApplication nach Startup. */
		core = new rl::CoreSubsystem();
		core->log("CoreSubsystem gestartet");
			
		sound = new rl::SoundSubsystem();
		core->log("SoundSubsystem gestartet");
			
		rules = new rl::RulesSubsystem();
		core->log("RulesSubsystem gestartet");
			
		dialog = new rl::DialogSubsystem();
		core->log("DialogSubsystem gestartet");
			
		ui = new rl::UiSubsystem();
		core->log("UiSubsystem gestartet");

		script = new rl::ScriptSubsystem();
		core->log("ScriptSubsystem gestartet");

		core->log("Starte...");
		core->startCore();
	} 
	catch(Ogre::Exception& oe) {
		rl::showError(oe.getFullDescription());
	} 
	catch(rl::Exception& re) {
		rl::showError(re.toString());
	} 
	catch(CEGUI::Exception& ce) {
		rl::showError(ce.getMessage().c_str());
	}
	catch(std::runtime_error& rte) {
		rl::showError(rte.what());
	} 
	catch(std::exception& exp) {
		rl::showError( exp.what() );
	} 
	catch(std::string& err) {
		rl::showError( err );
	} 
	catch(...) {
		rl::showError( "Unknown exception occured" );
	}

	try
	{
		delete script;
		delete ui;
		delete dialog;
		delete rules;
		delete sound;
		delete core;
	}
	catch(Ogre::Exception& oe) {
		rl::showError(oe.getFullDescription());
	} 
	catch(rl::Exception& re) {
		rl::showError(re.toString());
	} 
	catch(std::runtime_error& rte) {
		rl::showError(rte.what());
	} 
	catch(std::exception& exp) {
		rl::showError( exp.what() );
	} 
	catch(std::string& err) {
		rl::showError( err );
	} 
	catch(...) {
		rl::showError( "Unknown exception occured" );
	}
 
		
	return 0;
}
