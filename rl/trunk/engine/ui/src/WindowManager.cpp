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
#include "WindowManager.h"
#include <CEGUIWindowManager.h>

#include "AboutWindow.h"
#include "ActionChoiceWindow.h"
#include "CharacterSheetWindow.h"
#include "CharacterStateWindow.h"
#include "CloseConfirmationWindow.h"
#include "Combat.h"
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
#include "GameLoop.h"
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
#include "RubyInterpreter.h"
#include "RulesSubsystem.h"
#include "TargetSelectionWindow.h"
#include "UiSubsystem.h"

#undef max

template<> rl::WindowManager* Ogre::Singleton<rl::WindowManager>::ms_Singleton = 0;

using namespace CEGUI;

namespace rl {

	WindowManager::WindowManager()
	{
		mWindowUpdater = new WindowUpdater();
		GameLoopManager::getSingleton().addSynchronizedTask(mWindowUpdater, FRAME_STARTED);
		//mWindowUpdater->setPaused(true);

		mConsole = new Console();
		new DebugWindow();
		CoreSubsystem::getSingleton().getRubyInterpreter()->
			setOutputFunction( 
				(VALUE(*)(...))&WindowManager::consoleWrite );

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

	WindowManager::~WindowManager()
	{
		delete mLogWindow;
		delete mJournalWindow;
		delete mGameLogger;
		delete mCharacterSheet;
		delete mInGameMenuWindow;
		delete mCharacterStateWindow;
		delete mConsole;
		delete mWindowUpdater;

		delete DebugWindow::getSingletonPtr();
	}
	
	void WindowManager::registerWindow(CeGuiWindow* window)
	{
		mWindowList.push_back(window);
	}

	bool WindowManager::destroyWindow(CeGuiWindow* window)
	{
		mWindowUpdater->fadeOut(window, 0.25, true);
		return true;
	}

	void WindowManager::_doDestroyWindow(CeGuiWindow* window)
	{
		mWindowList.remove(window);
		CeGuiWindow::getRoot()->removeChildWindow(window->getWindow());
		window->setVisible(false);
		CEGUI::WindowManager::getSingleton().destroyWindow(window->getWindow());
		delete window;		
	}

	void WindowManager::closeTopWindow()
	{
		for(std::list<CeGuiWindow*>::iterator it = mWindowList.begin(); it != mWindowList.end(); it++)
		{
			CeGuiWindow* cur = *it;
			if (cur->isVisible() && cur->isClosingOnEscape())
			{
				break;
			}
		}
	}



	bool WindowManager::handleMovedToFront(CeGuiWindow* window)
	{
		mWindowList.remove(window);
		mWindowList.push_front(window);
		return true;
	}
	
	bool WindowManager::handleMovedToBack(CeGuiWindow* window)
	{
		mWindowList.remove(window);
		mWindowList.push_back(window);
		return true;
	}

	WindowManager& WindowManager::getSingleton()
	{
		return Ogre::Singleton<WindowManager>::getSingleton();
	}

	WindowManager* WindowManager::getSingletonPtr()
	{
		return Ogre::Singleton<WindowManager>::getSingletonPtr();
	}

	void WindowManager::showCharacterActionChoice()
	{
		showActionChoice(UiSubsystem::getSingleton().getActiveCharacter());
	}

	void WindowManager::showPickedObjectActions()
	{
		GameObject* pickedObject = InputManager::getSingleton().getPickedObject();

		if (pickedObject != NULL)
			showActionChoice(pickedObject);
	}

	void WindowManager::showContainerContent(Container* container)
	{
		ContainerContentWindow* wnd = new ContainerContentWindow(container);
		wnd->setVisible(true);
	}

	bool WindowManager::showInputOptionsMenu(Creature* actionHolder)
	{
		CommandMapperWindow* wnd = 
			new CommandMapperWindow(
				actionHolder, 
				InputManager::getSingleton().getCommandMapper());
		wnd->setVisible(true);

		return true;
	}

	void WindowManager::showMessageWindow(const CeGuiString& message)
	{
		MessageWindow* w = new MessageWindow();
		w->setText(message);
		w->setVisible(true);
	}

	void WindowManager::showMainMenu()
	{
		(new MainMenuWindow(new MainMenuEngineWindow()))->setVisible(true);
	}

	void WindowManager::showTargetWindow()
	{
		TargetSelectionWindow::getSingleton().setAction(NULL);
		TargetSelectionWindow::getSingleton().setVisible(true);
	}

	void WindowManager::toggleConsole()
	{
		mConsole->setVisible(!mConsole->isVisible());
	}

	void WindowManager::toggleDebugWindow()
	{
		DebugWindow* dbgwnd = DebugWindow::getSingletonPtr();
		dbgwnd->setVisible(!dbgwnd->isVisible());
	}

	void WindowManager::toggleGameLogWindow()
	{
		mGameLogger->setVisible(!mGameLogger->isVisible());
	}

	void WindowManager::showCharacterSheet()
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

	void WindowManager::showJournalWindow()
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

	void WindowManager::showAboutWindow()
	{
		(new AboutWindow())->setVisible(true);
	}

	void WindowManager::showCharacterSheet(Person* chara)
	{
		CharacterSheetWindow* wnd = new CharacterSheetWindow();
		wnd->setCharacter(chara);
		wnd->setVisible(true);
	}

	void WindowManager::showDescriptionWindow(GameObject* obj)
	{
		MessageWindow* wnd = new MessageWindow();
		wnd->setText(obj->getDescription());
		wnd->setVisible(true);
	}

	void WindowManager::toggleCharacterStateWindow()
	{
		mCharacterStateWindow->setVisible(!mCharacterStateWindow->isVisible());
	}

	void WindowManager::toggleInGameGlobalMenu()
	{
		mInGameMenuWindow->setVisible(!mInGameMenuWindow->isVisible());
	}

	GameLoggerWindow* WindowManager::getGameLogger()
	{
		return mGameLogger;
	}

    void WindowManager::showPlaylist()
    {
        PlaylistWindow* wnd = new PlaylistWindow();
        wnd->setVisible(true);
    }
	
	void WindowManager::checkForErrors()
	{
		if (Logger::getSingleton().isErrorPresent())
		{
			mLogWindow->setVisible(true);
			Logger::getSingleton().resetErrorState();
		}
	}

	void WindowManager::_fadeIn(CeGuiWindow* window, Ogre::Real time, float targetAlpha)
	{
		mWindowUpdater->fadeIn(window, time, targetAlpha);
	}

	void WindowManager::_fadeOut(CeGuiWindow* window, Ogre::Real time, bool destroy)
	{
		mWindowUpdater->fadeOut(window, time, destroy);
	}

	void WindowManager::_moveOutLeft(CeGuiWindow* window, Ogre::Real time, bool destroy)
	{
		mWindowUpdater->moveOutLeft(window, time, destroy);
	}


	void WindowManager::update()
	{
		mInGameMenuWindow->update();
	}

    void WindowManager::requestExit()
    {
		Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "Start", "UiSubsystem::requestExit");
		(new CloseConfirmationWindow())->setVisible(true);
	}

	void WindowManager::writeToConsole(Ogre::String text)
	{
		mConsole->write(text);
	}

	VALUE WindowManager::consoleWrite(VALUE self, VALUE str)
	{
		if (WindowManager::getSingleton().mConsole != NULL)
        {
            WindowManager::getSingleton().mConsole->
				write(RubyInterpreter::val2ceguistr(str) + " \n");
        }
		return Qnil;
	}

	void WindowManager::setActiveCharacter(Creature* character)
	{
		mCharacterStateWindow->setCharacter(character);
		mCharacterStateWindow->update();
		Logger::getSingleton().log(Logger::UI, Ogre::LML_TRIVIAL, "CharacterStateWindow updated");
	}

	void WindowManager::showActionChoice(GameObject* obj)
	{
		ActionChoiceWindow* w = new ActionChoiceWindow(UiSubsystem::getSingleton().getActiveCharacter());
		int numActions = w->showActionsOfObject(obj);
		if (numActions > 0)
		{
			w->setVisible(true);
		}
		else
		{
			destroyWindow(w);
		}
	}

	void WindowManager::showCombatWindow(Combat* combat, Creature* activeCreature)
	{
		(new CombatWindow(combat, combat->getGroupOf(activeCreature)))->setVisible(true);
	}


	WindowUpdateTask::WindowUpdateTask(
		CeGuiWindow* window, Ogre::Real time, 
		WindowUpdateTask::WindowUpdateAction action,
		int targetX, int targetY, Ogre::Real targetAlpha)
	:	mWindow(window),
		mTime(time), 
		mTargetPoint(targetX, targetY), 
		mTargetAlpha(targetAlpha),
		mAction(action)
	{
		initialize();
	}

	WindowUpdateTask::WindowUpdateTask(
		CeGuiWindow* window, Ogre::Real ftime, 
		WindowUpdateTask::WindowUpdateAction action,
		Ogre::Real targetAlpha)
	:	mWindow(window),
		mTime(ftime), 
		mTargetPoint(-9999999, -9999999), 
		mTargetAlpha(targetAlpha),
		mAction(action)
	{
		initialize();
	}

	void WindowUpdateTask::initialize()
	{
		mCalculatePoint =(mTargetPoint.d_x != -9999999);
		mCalculateAlpha =(mTargetAlpha != -1);
			
		CEGUI::Point delta = mTargetPoint - mWindow->getWindow()->getAbsolutePosition();
		mRatePoint = CEGUI::Point(delta.d_x / mTime, delta.d_y / mTime);
		mRateAlpha = 
			(mTargetAlpha - mWindow->getWindow()->getAlpha()) / mTime;
		mCurrentPoint = mWindow->getWindow()->getAbsolutePosition();
		mCurrentAlpha = mWindow->getWindow()->getAlpha();

		if (mCalculateAlpha)
		{
			mNormalAlpha = std::max(
				mWindow->getWindow()->getAlpha(),
				mTargetAlpha);
		}
		else
		{
			mNormalAlpha = mWindow->getWindow()->getAlpha();
		}
	}

	void WindowUpdateTask::run(Ogre::Real elapsedTime)
	{
		mTime -= elapsedTime;
		if (mTime < 0)
		{
			mTime = 0;
		}

		if (mTime > 0)
		{
			if (mCalculatePoint)
			{
				mCurrentPoint += CEGUI::Point(mRatePoint.d_x * elapsedTime, mRatePoint.d_y * elapsedTime);
			}
			if (mCalculateAlpha)
			{
				mCurrentAlpha += mRateAlpha * elapsedTime;
			}			
		}
		else if (mTime == 0)
		{
			if (mCalculatePoint)
			{
				mCurrentPoint = mTargetPoint;
			}
			if (mCalculateAlpha)
			{
				mCurrentAlpha = mTargetAlpha;
			}
		}
	}

	const CEGUI::Point& WindowUpdateTask::getCurrentPosition() const
	{
		return mCurrentPoint;
	}

	const Ogre::Real& WindowUpdateTask::getCurrentAlpha() const
	{
		return mCurrentAlpha;
	}

	const Ogre::Real& WindowUpdateTask::getNormalAlpha() const
	{
		return mNormalAlpha;
	}

	const Ogre::Real& WindowUpdateTask::getTimeLeft() const
	{
		return mTime;
	}

	WindowUpdateTask::WindowUpdateAction WindowUpdateTask::getAction() const
	{
		return mAction;
	}

	CeGuiWindow* WindowUpdateTask::getWindow()
	{
		return mWindow;
	}

	void WindowUpdater::run( Ogre::Real elapsedTime )
	{
		RL_LONGLONG start = CoreSubsystem::getSingleton().getClock();

		if (mTasks.empty())
			return;

		for (std::vector<WindowUpdateTask*>::iterator it = mTasks.begin(); it != mTasks.end();)
		{
			WindowUpdateTask* task = *it;
			task->run(elapsedTime);
			task->getWindow()->getWindow()->setPosition(CEGUI::Absolute, task->getCurrentPosition());
			task->getWindow()->getWindow()->setAlpha(task->getCurrentAlpha());
			if (task->getTimeLeft() <= 0)
			{
				switch (task->getAction())
				{
				case WindowUpdateTask::WND_DESTROY:
					WindowManager::getSingleton().destroyWindow(task->getWindow());
					break;
				case WindowUpdateTask::WND_HIDE:
					task->getWindow()->getWindow()->hide();
					task->getWindow()->getWindow()->setAlpha(task->getNormalAlpha());
					break;
				}
				delete task;
				*it = NULL;
				it = mTasks.erase(it);
			}
			else
			{
				++it;
			}
		}
		Logger::getSingleton().log(
			Logger::CORE, 
			Ogre::LML_TRIVIAL, 
			"    WU time "
			 + Ogre::StringConverter::toString(
					Ogre::Real((double)(CoreSubsystem::getSingleton().getClock()-start))));
	}

	void WindowUpdater::fadeIn(CeGuiWindow* window, Ogre::Real time, Ogre::Real targetAlpha)
	{
		if (window == NULL)
			Throw(InvalidArgumentException, "NULL argument");

		mTasks.push_back(
			new WindowUpdateTask(
				window, 
				time, 
				WindowUpdateTask::WND_SHOW, 
				targetAlpha));
	}

	void WindowUpdater::fadeOut(CeGuiWindow* window, Ogre::Real time, bool destroy)
	{
		if (window == NULL)
			Throw(InvalidArgumentException, "NULL argument");

		mTasks.push_back(
			new WindowUpdateTask(
				window, 
				time, 
				destroy ? WindowUpdateTask::WND_DESTROY : WindowUpdateTask::WND_HIDE, 
				0.0));
	}

	void WindowUpdater::moveOutLeft(CeGuiWindow* window, Ogre::Real time, bool destroy)
	{
		if (window == NULL)
			Throw(InvalidArgumentException, "NULL argument");

		CEGUI::Window* wnd = window->getWindow();
		mTasks.push_back(
			new WindowUpdateTask(
				window, 
				time,
				destroy ? WindowUpdateTask::WND_DESTROY : WindowUpdateTask::WND_HIDE, 
				-wnd->getAbsoluteWidth(), 
				wnd->getAbsoluteYPosition()));
	}

	void WindowManager::showDialog(DialogCharacter* bot)
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
}
