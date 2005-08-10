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

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#define WIN32_LEAN_AND_MEAN
	#include "windows.h"
#else
	#include "SDL.h"
#endif

void startupRl(bool developerMode, Ogre::String module)
{
	rl::CoreSubsystem* core = NULL;
	rl::SoundSubsystem* sound = NULL;
	rl::RulesSubsystem* rules = NULL;
	rl::DialogSubsystem* dialog = NULL;
	rl::UiSubsystem* ui =  NULL;
	rl::ScriptSubsystem* script =  NULL;

#ifndef _DEBUG
	try {
#endif // #ifndef _DEBUG
		core = new rl::CoreSubsystem();
		core->setDeveloperMode(developerMode);
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
#ifndef _DEBUG
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
#endif // #ifndef _DEBUG
		delete script;
		delete ui;
		delete dialog;
		delete rules;
		delete sound;
		delete core;
#ifndef _DEBUG
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
#endif //#ifndef _DEBUG

#if OGRE_PLATFORM != OGRE_PLATFORM_WIN32
	SDL_Quit();
#endif
}

void analyzeParameters(int argc, char** argv, bool& developerMode, Ogre::String& startModule)
{
	developerMode = false;
	startModule = "";

	for (int argIdx = 0; argIdx < argc; argIdx++)
	{
		if (strncmp(argv[argIdx], "--dev", 5) == 0)
			developerMode = true;
		else if (strncmp(argv[argIdx], "--module", 8) == 0
				&& argc > argIdx + 2) // Nächster Parameter existiert
			startModule = argv[argIdx + 1];
	}
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
{
	int argc;
	LPWSTR* argList = CommandLineToArgvW(GetCommandLineW(), &argc);
	char** argv = new char*[argc];
	for (int argIdx = 0; argIdx < argc; argIdx++)
	{
		int len = wcslen(argList[argIdx])+1;
		argv[argIdx] = new char[len];
		wcstombs(argv[argIdx], argList[argIdx], len);
		argv[argIdx][len-1] = '\0';
	}

	bool developer; 
	Ogre::String module;
	analyzeParameters(argc, argv, developer, module);

	delete[] argv;
	LocalFree(argList);
	
    startupRl(developer, module);

	return 0;
}

#else // if OGRE_PLATFORM != OGRE_PLATFORM_WIN32

int main(int argc, char **argv)
{
	bool developer; 
	Ogre::String module;

	analyzeParameters(argc, argv, developer, module);
    startupRl(developer, module);

	return 0;
}

#endif
