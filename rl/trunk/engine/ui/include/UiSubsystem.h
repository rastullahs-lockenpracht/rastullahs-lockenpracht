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

#ifndef __UiSubsystem_H__
#define __UiSubsystem_H__

#include <OgreSingleton.h>
#include <string>

#include <OgreNoMemoryMacros.h>
#include <ruby.h>
#include "FixRubyHeaders.h"
#include <OgreMemoryMacros.h>
#undef min
#include "UiPrerequisites.h"

namespace rl {

    class Interpreter;
    class GameController;
    class GameActor;
	class GameObject;
	class Person;

    class _RlUiExport UiSubsystem : protected Singleton<UiSubsystem>
    {
    public:
        /** Default Constructor */
        UiSubsystem();
        /** Default Deconstructor */
        virtual ~UiSubsystem();

        /** Returns the Singleton */
        static UiSubsystem & getSingleton(void);
        static UiSubsystem * getSingletonPtr(void);

		void log(const std::string& msg, const std::string& ident = "");
	
        /** Requests the application's exit */
        void requestExit();

		/** Writes Text into the Console or other output media */
		void writeToConsole(std::string text);
		static VALUE consoleWrite(VALUE self, VALUE str);
		void toggleConsole();
		void toggleDebugWindow();

		void showCharacterActionChoice();
		void showActionChoice(GameObject* obj);
		void showMessageWindow(const CeGuiString& message);
		void showMainMenu();
		bool showInputOptionsMenu(GameObject* actionHolder);
		Person* getActiveCharacter();
		void setActiveCharacter(Person* person);
        GameController* getGameController();

		void setBattleMode(bool inBattle);
		bool isInBattleMode();

		static const char* CEGUI_ROOT;

    private:
        void initializeUiSubsystem( void );
        void runTest();
        
        GameController* mGameController;
        GameActor* mHero;
        Person* mCharacter;
        bool mInBattle;
    };
}

#endif
