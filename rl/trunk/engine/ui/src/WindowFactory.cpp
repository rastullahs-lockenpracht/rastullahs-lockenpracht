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
#include "stdinc.h" //precompiled header

#include "WindowFactory.h"

#include "AboutWindow.h"
#include "ActionChoiceWindow.h"
#include "ActorManager.h"
#include "CharacterSheetWindow.h"
#include "CharacterStateWindow.h"
#include "CloseConfirmationWindow.h"
#include "CombatWindow.h"
#include "Console.h"
#include "CoreSubsystem.h"
#include "DebugWindow.h"
#include "DialogControlState.h"
#include "DialogWindow.h"
#include "Exception.h"
#include "GameLoggerWindow.h"
#include "GameObject.h"
#include "GameObjectInfoWindow.h"
#include "PropertiesWindow.h"
#include "GameSaveLoadWindow.h"
#include "MainMenuLoadWindow.h"
#include "GameSettings.h"
#include "InfoPopup.h"
#include "InGameMenuWindow.h"
#include "InputManager.h"
#include "InventoryWindow.h"
#include "JournalWindow.h"
#include "LogWindow.h"
#include "MessageWindow.h"
#include "MainMenuWindow.h"
#include "MainMenuEngineWindow.h"
#include "MovableText.h"
#include "ObjectDescriptionWindow.h"
#include "Person.h"
#include "PlaylistWindow.h"
#include "QuestBook.h"
#include "RubyInterpreter.h"
#include "RulesSubsystem.h"
#include "SubtitleWindow.h"
#include "UiSubsystem.h"
#include "WindowManager.h"

#undef max

template<> rl::WindowFactory* Ogre::Singleton<rl::WindowFactory>::ms_Singleton = 0;

using namespace CEGUI;
using namespace Ogre;

namespace rl {

    WindowFactory::WindowFactory()
        : mGameLogger(NULL),
        mCharacterStateWindow(NULL),
        mInGameMenuWindow(NULL),
        mCharacterSheet(NULL),
        mJournalWindow(NULL),
        mInventoryWindow(NULL),
        mLogWindow(NULL),
        mDebugWindow(NULL),
        mConsole(NULL),
        mInfoPopup(NULL),
        mObjectNameText(NULL),
        mShownObject(NULL),
        mObjectDescriptionWindow(NULL),
        mMainMenuWindow(NULL),
		mMainMenuLoadWindow(NULL),
        mGameSettings(NULL),
        mCombatWindow(NULL)
    {
    }

    void WindowFactory::initialize()
    {
        mConsole = new Console();
        mDebugWindow = new DebugWindow();
        CoreSubsystem::getSingleton().getRubyInterpreter()->
            setOutputFunction(
                (VALUE(*)(...))&WindowFactory::consoleWrite );

        mLogWindow = new LogWindow();

        mGameLogger = new GameLoggerWindow();
        mCharacterStateWindow = new CharacterStateWindow();
        mInGameMenuWindow = new InGameMenuWindow();
        mCharacterSheet = new CharacterSheetWindow();
        mJournalWindow = new JournalWindow();
        mInfoPopup = new InfoPopup();
        mObjectDescriptionWindow = new ObjectDescriptionWindow();
        mGameSettings = new GameSettings();

        RulesSubsystem::getSingleton().getQuestBook()->addQuestListener(mJournalWindow);
        RulesSubsystem::getSingleton().getQuestBook()->addQuestListener(mInfoPopup);
        mMainMenuWindow = new MainMenuWindow( new MainMenuEngineWindow() );

        logAllWindows();
    }

    WindowFactory::~WindowFactory()
    {
        delete mLogWindow;
        delete mObjectDescriptionWindow;
        delete mJournalWindow;
        delete mGameLogger;
        delete mCharacterSheet;
        delete mInGameMenuWindow;
        delete mInventoryWindow;
        delete mCharacterStateWindow;
        delete mInfoPopup;
        delete mDebugWindow;
        delete mConsole;
        delete mMainMenuWindow;
        delete mGameSettings;
        delete mCombatWindow;
    }

    void WindowFactory::showCharacterActionChoice()
    {
        showActionChoice(UiSubsystem::getSingleton().getActiveCharacter());
    }

    void WindowFactory::showContainerContent(Container* container)
    {
        if( !mInventoryWindow )
            toggleInventoryWindow();

        mInventoryWindow->showContainerContent(container);
    }

    void WindowFactory::showMessageWindow(const CeGuiString& message)
    {
        MessageWindow* w = new MessageWindow();
        w->setText(message);
        w->setVisible(true);
    }

    void WindowFactory::showMainMenu()
    {
        mMainMenuWindow->setVisible(true);
    }

    void WindowFactory::toggleConsole()
    {
        mConsole->setVisible(!mConsole->isVisible());
    }

    void WindowFactory::toggleDebugWindow()
    {
        mDebugWindow->setVisible(!mDebugWindow->isVisible());
    }

    void WindowFactory::showPropertiesWindow(GameObject* obj)
    {
        PropertiesWindow* wnd = new PropertiesWindow(obj);
        wnd->setVisible(true);
    }

    void WindowFactory::showGameSaveLoadWindow()
    {
        GameSaveLoadWindow* wnd = new GameSaveLoadWindow();
        wnd->initialize();
        wnd->setVisible(true);
    }

    void WindowFactory::showMainMenuLoadWindow(MainMenuWindow* win)
    {
		mMainMenuLoadWindow = new MainMenuLoadWindow(win);
        mMainMenuLoadWindow->initialize();
        mMainMenuLoadWindow->setVisible(true);
    }

	void WindowFactory::hideMainMenuLoadWindow()
    {
		if(mMainMenuLoadWindow != NULL && mMainMenuLoadWindow->isVisible())
		{
			mMainMenuLoadWindow->setVisible(false,true);
			mMainMenuWindow = NULL;
		}
    }

    void WindowFactory::toggleGameLogWindow()
    {
        mGameLogger->setVisible(!mGameLogger->isVisible());
    }

    void WindowFactory::toggleInventoryWindow()
    {
        if (mInventoryWindow != NULL && mInventoryWindow->isVisible())
        {
            mInventoryWindow->setVisible(false, true);
            mInventoryWindow = NULL;
        }
        else
        {
            if (mInventoryWindow != NULL)
            {
                WindowManager::getSingleton().destroyWindow(mInventoryWindow);
                mInventoryWindow = NULL;
            }

            if (UiSubsystem::getSingleton().getActiveCharacter() != NULL)
            {
                Creature* creat = UiSubsystem::getSingleton().getActiveCharacter();

                Ogre::String inventoryWindowType = creat->getInventoryWindowType();
                if (inventoryWindowType == "")
                {
                    inventoryWindowType = "inventory_default.xml"; ///@TODO: Extract
                }
                mInventoryWindow = new InventoryWindow(inventoryWindowType, creat->getInventory());
                mInventoryWindow->setVisible(true);
            }
        }
    }

    void WindowFactory::notifyInventoryWindowDestroyed()
    {
        mInventoryWindow = NULL;
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
        (new GameObjectInfoWindow(obj, UiSubsystem::getSingleton().getActiveCharacter()))->setVisible(true);
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
        AbstractWindow* closeConfirmationWindow = new CloseConfirmationWindow();
        LOG_MESSAGE2(Logger::UI, "Start", "UiSubsystem::requestExit");
        closeConfirmationWindow->setVisible(true);
    }

    void WindowFactory::writeToConsole(Ogre::String text)
    {
        if (mConsole != NULL)
        {
            mConsole->write(text);
        }
        else
        {
            LOG_MESSAGE("Console", text);
        }
    }

    VALUE WindowFactory::consoleWrite(VALUE self, VALUE str)
    {
        CeGuiString text = RubyInterpreter::val2ceguistr(str);

        if (WindowFactory::getSingleton().mConsole != NULL)
        {
            WindowFactory::getSingleton().mConsole->
                write(text + " \n");
        }
        else
        {
            LOG_MESSAGE("Console", text);
        }
        return Qnil;
    }

    void WindowFactory::setActiveCharacter(Creature* character)
    {
        mCharacterStateWindow->setCharacter(character);
        LOG_MESSAGE(Logger::UI, "CharacterStateWindow updated");
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

    void WindowFactory::showLogfiles()
    {
        (new LogWindow())->setVisible(true);
    }

    void WindowFactory::showPopupMessage(int popupTypes)
    {
        if (popupTypes & WindowFactory::ICON_ERROR)
        {
            mInfoPopup->showError();
        }

        if (popupTypes & WindowFactory::ICON_QUEST)
        {
            mInfoPopup->showQuestBookChange();
        }
    }

    void WindowFactory::showObjectDescription(GameObject* object)
    {
        mObjectDescriptionWindow->show(object);
    }

    void WindowFactory::showNextDebugWindowPage()
    {
        mDebugWindow->showNextPage();
    }

    void WindowFactory::showGameSettings()
    {
        // Create the game settings window and show it
        mGameSettings->setVisible(true);
    }

    void WindowFactory::logAllWindows()
    {
        CEGUI::Window* rootWnd = AbstractWindow::getRoot();
        CEGUI::uint count = rootWnd->getChildCount();
        for (CEGUI::uint chIdx =  0; chIdx < count; ++chIdx)
        {
            CEGUI::Window* wnd = rootWnd->getChildAtIdx(chIdx);
            LOG_MESSAGE(
                Logger::UI,
                wnd->getName()
                + (wnd->isVisible() ? " vis" : " nvis")
                + " @"
                + StringConverter::toString(wnd->getPixelRect().getPosition().d_x)
                + ", "
                + StringConverter::toString(wnd->getPixelRect().getPosition().d_y)
                + " "
                + StringConverter::toString(wnd->getPixelSize().d_width)
                + ", "
                + StringConverter::toString(wnd->getPixelSize().d_height));
        }
    }

    CombatWindow* WindowFactory::getCombatWindow()
    {
        if (mCombatWindow == NULL)
        {
            mCombatWindow = new CombatWindow();
        }
        return mCombatWindow;
    }
}
