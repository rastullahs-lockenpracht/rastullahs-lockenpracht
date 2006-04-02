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

#ifndef __WindowManager_H__
#define __WindowManager_H__

#include "UiPrerequisites.h"
#include <OgreSingleton.h>

#include <OgreNoMemoryMacros.h>
#include "FixRubyHeaders.h"		// die Makros sind hier vor Ruby schon definiert
#include <ruby.h>
#include "FixRubyHeaders.h"
#include <OgreMemoryMacros.h>
#undef min

#include <CEGUIVector.h>
#include <vector>
#include "GameTask.h"

namespace rl {

	class CeGuiWindow;
	class CharacterStateWindow;
	class CharacterSheetWindow;
	class Combat;
	class Console;
	class Container;
	class Creature;
	class DialogCharacter;
	class GameLoggerWindow;
	class GameObject;
	class InGameMenuWindow;
	class JournalWindow;
	class LogWindow;
	class Person;
	class WindowUpdater;

	class _RlUiExport WindowManager : public Ogre::Singleton<WindowManager>
	{
	public:
		WindowManager();
		~WindowManager();

		static WindowManager& getSingleton();
		static WindowManager* getSingletonPtr();

		void registerWindow(CeGuiWindow* window);
		bool destroyWindow(CeGuiWindow* window);
		void _doDestroyWindow(CeGuiWindow* window);
		void closeTopWindow();
		bool handleMovedToFront(CeGuiWindow* window);
		bool handleMovedToBack(CeGuiWindow* window);

		void setActiveCharacter(Creature* character);

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
		void showCombatWindow(Combat* combat, Creature* activeCreature);

		void toggleConsole();
		void toggleDebugWindow();
		void toggleGameLogWindow();
		void toggleCharacterStateWindow();
		void toggleInGameGlobalMenu();
		
		void checkForErrors();
		GameLoggerWindow* getGameLogger();

		void update();

		/** Writes Text into the Console or other output media */
		void writeToConsole(std::string text);
		static VALUE consoleWrite(VALUE self, VALUE str);

        /** Requests the application's exit */
        void requestExit();

		void _fadeIn(CeGuiWindow* window, Ogre::Real time, float targetAlpha = 1.0);
		void _fadeOut(CeGuiWindow* window, Ogre::Real time, bool destroy);
		void _moveOutLeft(CeGuiWindow* window, Ogre::Real time, bool destroy);

	private:
		std::list<CeGuiWindow*> mWindowList;

		GameLoggerWindow* mGameLogger;
		CharacterStateWindow* mCharacterStateWindow;
		InGameMenuWindow* mInGameMenuWindow;
		CharacterSheetWindow* mCharacterSheet;
		JournalWindow* mJournalWindow;
		LogWindow* mLogWindow;
		Console* mConsole;

		WindowUpdater* mWindowUpdater;
	};

	class WindowUpdateTask {
	public:
		enum WindowUpdateAction
		{
			WND_SHOW = 1,
			WND_HIDE,
			WND_DESTROY
		};
		
		WindowUpdateTask(CeGuiWindow* window, Ogre::Real time, WindowUpdateAction action,
			int targetX, int targetY, Ogre::Real targetAlpha = -1);
		WindowUpdateTask(CeGuiWindow* window, Ogre::Real time, WindowUpdateAction action, 
			Ogre::Real targetAlpha);

		void run(Ogre::Real elapsedTime);
		const CEGUI::Point& getCurrentPosition() const;
		const Ogre::Real& getCurrentAlpha() const;
		const Ogre::Real& getNormalAlpha() const;
		const Ogre::Real& getTimeLeft() const;
		CeGuiWindow* getWindow();
		WindowUpdateAction getAction() const;

	private:
		void initialize();

		CeGuiWindow* mWindow;
		Ogre::Real mTime; 
		CEGUI::Point mTargetPoint;
		CEGUI::Point mRatePoint;
		CEGUI::Point mCurrentPoint;
		Ogre::Real mTargetAlpha;
		Ogre::Real mRateAlpha;
		Ogre::Real mCurrentAlpha;
		Ogre::Real mNormalAlpha;
		bool mCalculatePoint;
		bool mCalculateAlpha;
		WindowUpdateAction mAction;
	};

	class WindowUpdater : public GameTask
	{
	public:
		void run( Ogre::Real elapsedTime );
		void fadeIn(CeGuiWindow* window, Ogre::Real time, float targetAlpha = 1.0);
		void fadeOut(CeGuiWindow* window, Ogre::Real time, bool destroy);
		void moveOutLeft(CeGuiWindow* window, Ogre::Real time, bool destroy);
	private:
		std::vector<WindowUpdateTask*> mTasks;
	};


}

#endif
