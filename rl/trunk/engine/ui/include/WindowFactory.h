/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __WindowFactory_H__
#define __WindowFactory_H__

#include "UiPrerequisites.h"
#include <OgreSingleton.h>

#include <OgreNoMemoryMacros.h>
#include "FixRubyHeaders.h"		// die Makros sind hier vor Ruby schon definiert
#include <ruby.h>
#include "FixRubyHeaders.h"
#include <OgreMemoryMacros.h>
#undef min

#include <CEGUIVector.h>

namespace rl {

	class Actor;
	class AbstractWindow;
	class CharacterStateWindow;
	class CharacterSheetWindow;
	class Console;
	class Container;
	class Creature;
	class DataLoadingProgressWindow;
	class DebugWindow;
    class DialogCharacter;
	class GameLoggerWindow;
	class GameObject;
	class InfoPopup;
	class InGameMenuWindow;
	class InventoryWindow;
	class JournalWindow;
	class LogWindow;
	class ObjectDescriptionWindow;
	class Person;
	class WindowUpdater;
    class MainMenuWindow;
    class CloseConfirmationWindow;
    class GameSettings;

	class _RlUiExport WindowFactory : public Ogre::Singleton<WindowFactory>
	{
	public:
		enum PopupIcon
		{
			ICON_ERROR = 1,
			ICON_QUEST
		};

		WindowFactory();
		~WindowFactory();

        void initialize();

		static WindowFactory& getSingleton();
		static WindowFactory* getSingletonPtr();

		void setActiveCharacter(Creature* character);

		void showActionChoice(GameObject* obj);
		void showAboutWindow();
		void showCharacterActionChoice();
		void showCharacterSheet();
		void showCharacterSheet(Person* chara);
		void showContainerContent(Container* container);
		void showDescriptionWindow(GameObject* obj);
		void showDialog(DialogCharacter* bot);
        void showExitConfirmation();
		bool showInputOptionsMenu(Creature* actionHolder);
		void showJournalWindow();
		void showLogfiles();
		void showMessageWindow(const CeGuiString& message);
		void showMainMenu();
		void showObjectDescription(GameObject* object);
		void showObjectName(GameObject* object);
		void showPlaylist();
		void showPopupMessage(int popupTypes);
		/**
		 * Creates a GameSettings object and displays its layout file on screen
		 */
		void showGameSettings();

		void toggleConsole();
		void toggleDebugWindow();
		void toggleGameLogWindow();
		void toggleCharacterStateWindow();
		void toggleInGameGlobalMenu();
		void toggleInventoryWindow();

		InventoryWindow* getInventoryWindow();

		void checkForErrors();
		GameLoggerWindow* getGameLogger();

		void update();

		/// Writes Text into the Console or other output media
		void writeToConsole(std::string text);
		static VALUE consoleWrite(VALUE self, VALUE str);

        /// Change shown DebugWindow page
        void showNextDebugWindowPage();

	private:
        void logAllWindows();

		GameLoggerWindow* mGameLogger;
		CharacterStateWindow* mCharacterStateWindow;
		InGameMenuWindow* mInGameMenuWindow;
		CharacterSheetWindow* mCharacterSheet;
		JournalWindow* mJournalWindow;
		InventoryWindow* mInventoryWindow;
		LogWindow* mLogWindow;
		DebugWindow* mDebugWindow;
		Console* mConsole;
		InfoPopup* mInfoPopup;
		Actor* mObjectNameText;
		GameObject* mShownObject;
		ObjectDescriptionWindow* mObjectDescriptionWindow;
        DataLoadingProgressWindow* mDataLoadingProgressWindow;
        MainMenuWindow* mMainMenuWindow;
        CloseConfirmationWindow* mCloseConfirmationWindow;
        GameSettings* mGameSettings;
    };
}

#endif
