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

#ifndef __UiSubsystem_H__
#define __UiSubsystem_H__

#include "UiPrerequisites.h"

#include <OgreSingleton.h>
#include <string>

#include <OgreNoMemoryMacros.h>
#include "FixRubyHeaders.h"		// die Makros sind hier vor Ruby schon definiert
#include <ruby.h>
#include "FixRubyHeaders.h"
#include <OgreMemoryMacros.h>
#undef min

namespace rl {

    class Interpreter;
    class CharacterController;
    class GameActor;
	class GameObject;
	class Person;
	class GameLoggerWindow;
	class CharacterStateWindow;
	class CharacterSheetWindow;
	class Container;
	class InGameMenuWindow;
	class Creature;
	class DialogCharacter;
	class Combat;
	class JournalWindow;
	class LogWindow;

    class _RlUiExport UiSubsystem : protected Ogre::Singleton<UiSubsystem>
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

		void closeCurrentWindow();

		/** Writes Text into the Console or other output media */
		void writeToConsole(std::string text);
		static VALUE consoleWrite(VALUE self, VALUE str);
		void toggleConsole();
		void toggleDebugWindow();
		void toggleGameLogWindow();
		void toggleObjectPicking();
		void toggleCharacterStateWindow();
		void toggleInGameGlobalMenu();

		void useDefaultAction(GameObject* obj, Creature* actor);
		void usePickedObjectDefaultActions();

		void showCharacterActionChoice();
		void showPickedObjectActions();
		void showActionChoice(GameObject* obj);
		void showDescriptionWindow(GameObject* obj);
		void showTargetWindow();
		void showContainerContent(Container* container);

		void showMessageWindow(const CeGuiString& message);
		void showMainMenu();
		void showCharacterSheet();
		void showCharacterSheet(Person* chara);
		bool showInputOptionsMenu(Creature* actionHolder);
		void showDialog(DialogCharacter* bot);
        void showPlaylist();
		void showAboutWindow();
		void showJournalWindow();
		
		void checkForErrors();

		Person* getActiveCharacter();
		void setActiveCharacter(Person* person);
        CharacterController* getCharacterController();
		GameLoggerWindow* getGameLogger();

		void setCombatMode(bool inCombat);
		bool isInCombatMode();
		void startCombat(Combat* combat);

		void update();

		static const char* CEGUI_ROOT;

    private:
        void initializeUiSubsystem( void );
        void runTest();
        
        CharacterController* mCharacterController;
        GameActor* mHero;
        Person* mCharacter;
        bool mInCombat;
		GameLoggerWindow* mGameLogger;
		CharacterStateWindow* mCharacterStateWindow;
		InGameMenuWindow* mInGameMenuWindow;
		CharacterSheetWindow* mCharacterSheet;
		JournalWindow* mJournalWindow;
		LogWindow* mLogWindow;
    };
}

#endif
