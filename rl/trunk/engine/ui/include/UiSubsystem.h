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

#include "UiPrerequisites.h"

#include <OgreNoMemoryMacros.h>
#include <ruby.h>
#include "FixRubyHeaders.h"
#include <OgreMemoryMacros.h>

namespace rl {

    class Interpreter;
    class ThirdPersonGameController;
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
		Person* getActiveCharacter();
		void setActiveCharacter(Person* person);

		void setBattleMode(bool inBattle);
		bool isInBattleMode();

		static const char* CEGUI_ROOT;

    private:
        void initializeUiSubsystem( void );		void runTest();        ThirdPersonGameController* mGameController;        GameActor* mHero;		Person* mCharacter;		bool mInBattle;    };

}

#endif
