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

#include <xercesc/util/XMemory.hpp>	// Muss vor Ogre stehen (zumindest fuer VS)

#include <OgreLogManager.h>
#include <stdexcept>
#include <errno.h>

#include "CoreSubsystem.h"
#include "GameLoop.h"
#include "RulesSubsystem.h"
#include "DialogSubsystem.h"
#include "AiSubsystem.h"
#include "UiSubsystem.h"
#include "ScriptSubsystem.h"

#include "Exception.h"
#include "Logger.h"
#include "ConfigurationManager.h"
#include <CEGUIExceptions.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

void startupRl(bool developerMode, Ogre::String module)
{
	rl::CoreSubsystem* core = NULL;
	rl::RulesSubsystem* rules = NULL;
	rl::DialogSubsystem* dialog = NULL;
	rl::AiSubsystem* ai = NULL;
	rl::UiSubsystem* ui =  NULL;
	rl::ScriptSubsystem* script =  NULL;
	rl::Logger* logger = NULL;
	
#ifndef _DEBUG
	try {
#endif // #ifndef _DEBUG
        rl::ConfigurationManager::getSingleton().loadConfig();

		logger = new rl::Logger(
			rl::ConfigurationManager::getSingleton().getRastullahLogPath(),
			rl::ConfigurationManager::getSingleton().getOgreLogPath());
        // @todo in ConfigDatei auslagern/ oder auch Parameter

		rl::Logger::getSingleton().setLogDetail(
			rl::ConfigurationManager::getSingleton().getLogLevel());

		core = new rl::CoreSubsystem();
		core->setDeveloperMode(developerMode);
        LOG_MESSAGE_SHORT("CoreSubsystem gestartet");

		rules = new rl::RulesSubsystem();
		LOG_MESSAGE_SHORT("RulesSubsystem gestartet");

		dialog = new rl::DialogSubsystem();
		LOG_MESSAGE_SHORT("DialogSubsystem gestartet");
	
		ai = new rl::AiSubsystem();
		LOG_MESSAGE_SHORT("AiSubsystem gestartet");

		ui = new rl::UiSubsystem();
		LOG_MESSAGE_SHORT("UiSubsystem gestartet");

		script = new rl::ScriptSubsystem();
		LOG_MESSAGE_SHORT("ScriptSubsystem gestartet");


		LOG_MESSAGE_SHORT("Starte...");
		if (module != "")
			core->setDefaultActiveModule(module);

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
        LOG_MESSAGE_SHORT("Soundkonfiguration gespeichert");

		delete script;
		delete ui;
		delete ai;
		delete dialog;
		delete rules;
		delete core;

        delete logger;
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
				&& argc > argIdx + 1) // Nächster Parameter existiert
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

	for (int argIdx = 0; argIdx < argc; argIdx++)
	{
		delete[] argv[argIdx];
	}
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
