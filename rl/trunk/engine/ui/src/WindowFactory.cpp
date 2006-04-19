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
#include "CeGuiWindow.h"
#include "WindowFactory.h"

#include "AboutWindow.h"
#include "ActionChoiceWindow.h"
#include "CharacterSheetWindow.h"
#include "CharacterStateWindow.h"
#include "CloseConfirmationWindow.h"
#include "CombatWindow.h"
#include "CommandMapperWindow.h"
#include "Console.h"
#include "ContainerContentWindow.h"
#include "CoreSubsystem.h"
#include "DataLoadingProgressWindow.h"
#include "DebugWindow.h"
#include "DialogCharacter.h"
#include "DialogCharacterController.h"
#include "DialogWindow.h"
#include "Exception.h"
#include "GameLoggerWindow.h"
#include "GameObject.h"
#include "InGameMenuWindow.h"
#include "InputManager.h"
#include "JournalWindow.h"
#include "LogWindow.h"
#include "MessageWindow.h"
#include "MainMenuWindow.h"
#include "MainMenuEngineWindow.h"
#include "Person.h"
#include "PlaylistWindow.h"
#include "QuestBook.h"
#include "RBCombat.h"
#include "RubyInterpreter.h"
#include "RulesSubsystem.h"
#include "SoundConfig.h"
#include "TargetSelectionWindow.h"
#include "UiSubsystem.h"
#include "WindowManager.h"

#undef max

template<> rl::WindowFactory* Ogre::Singleton<rl::WindowFactory>::ms_Singleton = 0;

using namespace CEGUI;

namespace rl {

	WindowFactory::WindowFactory()
	{
		mConsole = new Console();
		new DebugWindow();
		CoreSubsystem::getSingleton().getRubyInterpreter()->
			setOutputFunction( 
				(VALUE(*)(...))&WindowFactory::consoleWrite );

		new TargetSelectionWindow();
			      
		mGameLogger = new GameLoggerWindow();
		mCharacterStateWindow = new CharacterStateWindow();
		mInGameMenuWindow = new InGameMenuWindow();
		mCharacterSheet = new CharacterSheetWindow();
		mJournalWindow = new JournalWindow();
		RulesSubsystem::getSingleton().getQuestBook()->addQuestChangeListener(mJournalWindow);
		CoreSubsystem::getSingleton().addCoreEventListener(new DataLoadingProgressWindow());

		mLogWindow = new LogWindow();
	}

	WindowFactory::~WindowFactory()
	{
		delete mLogWindow;
		delete mJournalWindow;
		delete mGameLogger;
		delete mCharacterSheet;
		delete mInGameMenuWindow;
		delete mCharacterStateWindow;
		delete mConsole;

		delete DebugWindow::getSingletonPtr();
	}

	WindowFactory& WindowFactory::getSingleton()
	{
		return Ogre::Singleton<WindowFactory>::getSingleton();
	}

	WindowFactory* WindowFactory::getSingletonPtr()
	{
		return Ogre::Singleton<WindowFactory>::getSingletonPtr();
	}

	void WindowFactory::showCharacterActionChoice()
	{
		showActionChoice(UiSubsystem::getSingleton().getActiveCharacter());
	}

	void WindowFactory::showPickedObjectActions()
	{
		GameObject* pickedObject = InputManager::getSingleton().getPickedObject();

		if (pickedObject != NULL)
			showActionChoice(pickedObject);
	}

	void WindowFactory::showContainerContent(Container* container)
	{
		ContainerContentWindow* wnd = new ContainerContentWindow(container);
		wnd->setVisible(true);
	}

	bool WindowFactory::showInputOptionsMenu(Creature* actionHolder)
	{
		CommandMapperWindow* wnd = 
			new CommandMapperWindow(
				actionHolder, 
				NULL);
		wnd->setVisible(true);

		return true;
	}

	void WindowFactory::showMessageWindow(const CeGuiString& message)
	{
		MessageWindow* w = new MessageWindow();
		w->setText(message);
		w->setVisible(true);
	}

	void WindowFactory::showMainMenu()
	{
		(new MainMenuWindow(new MainMenuEngineWindow()))->setVisible(true);
	}

	void WindowFactory::showTargetWindow()
	{
		TargetSelectionWindow::getSingleton().setAction(NULL);
		TargetSelectionWindow::getSingleton().setVisible(true);
	}

	void WindowFactory::toggleConsole()
	{
		mConsole->setVisible(!mConsole->isVisible());
	}

	void WindowFactory::toggleDebugWindow()
	{
		DebugWindow* dbgwnd = DebugWindow::getSingletonPtr();
		dbgwnd->setVisible(!dbgwnd->isVisible());
	}

	void WindowFactory::toggleGameLogWindow()
	{
		mGameLogger->setVisible(!mGameLogger->isVisible());
	}

	void WindowFactory::showCharacterSheet()
	{
		if (mCharacterSheet->isVisible())
		{
			mCharacterSheet->setCharacter(NULL);
			mCharacterSheet->setVisible(false);
		}
		else
		{
			mCharacterSheet->setCharacter(UiSubsystem::getSingleton().getActiveCharacter());
			mCharacterSheet->setVisible(true);
		}
	}

	void WindowFactory::showJournalWindow()
	{
		if (mJournalWindow->isVisible())
		{
			mJournalWindow->setVisible(false);
		}
		else
		{
			mJournalWindow->setVisible(true);
		}
	}

	void WindowFactory::showAboutWindow()
	{
		(new AboutWindow())->setVisible(true);
	}

	void WindowFactory::showCharacterSheet(Person* chara)
	{
		CharacterSheetWindow* wnd = new CharacterSheetWindow();
		wnd->setCharacter(chara);
		wnd->setVisible(true);
	}

	void WindowFactory::showDescriptionWindow(GameObject* obj)
	{
		MessageWindow* wnd = new MessageWindow();
		wnd->setText(obj->getDescription());
		wnd->setVisible(true);
	}

	void WindowFactory::toggleCharacterStateWindow()
	{
		mCharacterStateWindow->setVisible(!mCharacterStateWindow->isVisible());
	}

	void WindowFactory::toggleInGameGlobalMenu()
	{
		mInGameMenuWindow->setVisible(!mInGameMenuWindow->isVisible());
	}

	GameLoggerWindow* WindowFactory::getGameLogger()
	{
		return mGameLogger;
	}

    void WindowFactory::showPlaylist()
    {
        PlaylistWindow* wnd = new PlaylistWindow();
        wnd->setVisible(true);
    }
	
	void WindowFactory::checkForErrors()
	{
		if (Logger::getSingleton().isErrorPresent())
		{
			mLogWindow->setVisible(true);
			Logger::getSingleton().resetErrorState();
		}
	}

	void WindowFactory::update()
	{
		mInGameMenuWindow->update();
	}

	void WindowFactory::showExitConfirmation()
    {
		Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "Start", "UiSubsystem::requestExit");
		(new CloseConfirmationWindow())->setVisible(true);
	}

	void WindowFactory::writeToConsole(Ogre::String text)
	{
		mConsole->write(text);
	}

	VALUE WindowFactory::consoleWrite(VALUE self, VALUE str)
	{
		if (WindowFactory::getSingleton().mConsole != NULL)
        {
            WindowFactory::getSingleton().mConsole->
				write(RubyInterpreter::val2ceguistr(str) + " \n");
        }
		return Qnil;
	}

	void WindowFactory::setActiveCharacter(Creature* character)
	{
		mCharacterStateWindow->setCharacter(character);
		mCharacterStateWindow->update();
		Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "CharacterStateWindow updated");
	}

	void WindowFactory::showActionChoice(GameObject* obj)
	{
		ActionChoiceWindow* w = new ActionChoiceWindow(UiSubsystem::getSingleton().getActiveCharacter());
		int numActions = w->showActionsOfObject(obj);
		if (numActions > 0)
		{
			w->setVisible(true);
		}
		else
		{
			WindowManager::getSingleton().destroyWindow(w);
		}
	}

	void WindowFactory::showCombatWindow(RBCombat* combat, Creature* activeCreature)
	{
		CombatWindow* wnd = new CombatWindow(combat, combat->getGroupOf(activeCreature));
		combat->setLogger(mGameLogger);
		wnd->setVisible(true);
	}

	void WindowFactory::showDialog(DialogCharacter* bot)
	{
		if (bot->getDialogCharacter() == NULL)
		{
			bot->setDialogCharacter(UiSubsystem::getSingleton().getActiveCharacter());
		}

		UiSubsystem::getSingleton().setCharacterController(UiSubsystem::CTRL_DIALOG);
		DialogCharacterController* controller = 
			dynamic_cast<DialogCharacterController*>(
				UiSubsystem::getSingleton().getCharacterController());
		controller->setDialogPartner(bot->getDialogPartner()->getActor());
		(new DialogWindow(bot, mGameLogger, controller))->setVisible(true);
	}

	void WindowFactory::showLogfiles()
	{
		(new LogWindow())->setVisible(true);
	}

	void WindowFactory::showSoundConfig()
	{
		(new SoundConfig())->setVisible(true);
	}
}
