/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "CharacterSelectionWindow.h"
#include "CharacterSheetWindow.h"
#include "CharacterStateWindow.h"
#include "CloseConfirmationWindow.h"
#include "CombatWindow.h"
#include "Console.h"
#include "CoreSubsystem.h"
#include "Creature.h"
#include "DebugWindow.h"
#include "DialogControlState.h"
#include "DialogWindow.h"
#include "Exception.h"
#include "GameLoggerWindow.h"
#include "GameObject.h"
#include "GameObjectInfoWindow.h"
#include "GameOverWindow.h"
#include "GameSaveLoadWindow.h"
#include "GameSettings.h"
#include "InGameMenuWindow.h"
#include "InfoPopup.h"
#include "InputManager.h"
#include "InventoryWindow.h"
#include "JournalWindow.h"
#include "LogWindow.h"
#include "MainMenuEngineWindow.h"
#include "MainMenuLoadWindow.h"
#include "MainMenuWindow.h"
#include "MessageWindow.h"
#include "MovableText.h"
#include "ObjectDescriptionWindow.h"
#include "PartyManager.h"
#include "PlaylistWindow.h"
#include "PropertiesWindow.h"
#include "QuestBook.h"
#include "RubyInterpreter.h"
#include "RulesSubsystem.h"
#include "SubtitleWindow.h"
#include "UiSubsystem.h"
#include "WindowManager.h"

#undef max

template <> rl::WindowFactory* Ogre::Singleton<rl::WindowFactory>::msSingleton = 0;

using namespace CEGUI;
using namespace Ogre;

namespace rl
{

    WindowFactory::WindowFactory()
        : mGameLogger(NULL)
        , mCharacterStateWindow(NULL)
        , mInGameMenuWindow(NULL)
        , mCharacterSheet(NULL)
        , mJournalWindow(NULL)
        , mInventoryWindow(NULL)
        , mLogWindow(NULL)
        , mDebugWindow(NULL)
        , mConsole(NULL)
        , mInfoPopup(NULL)
        , mObjectNameText(NULL)
        , mShownObject(NULL)
        , mObjectDescriptionWindow(NULL)
        , mMainMenuWindow(NULL)
        , mMainMenuLoadWindow(NULL)
        , mGameSettings(NULL)
        , mCombatWindow(NULL)
        , mCharacterSelectionWindow(NULL)
    {
    }

    void WindowFactory::initialize()
    {
        mConsole = new Console();
        mDebugWindow = new DebugWindow();
        CoreSubsystem::getSingleton().getRubyInterpreter()->setOutputFunction(
            (VALUE(*)(...)) & WindowFactory::consoleWrite);

        mLogWindow = new LogWindow();

        mGameLogger = new GameLoggerWindow();
        mCharacterStateWindow = new CharacterStateWindow();
        mInGameMenuWindow = new InGameMenuWindow();
        mCharacterSheet = new CharacterSheetWindow();
        mJournalWindow = new JournalWindow();
        // mInfoPopup = new InfoPopup(); // this invisible window blocks the mouse
        mObjectDescriptionWindow = new ObjectDescriptionWindow();
        mGameSettings = new GameSettings();

        RulesSubsystem::getSingleton().getQuestBook()->addQuestListener(mJournalWindow);
        // RulesSubsystem::getSingleton().getQuestBook()->addQuestListener(mInfoPopup);
        mMainMenuWindow = new MainMenuWindow(new MainMenuEngineWindow());

        mCharacterSelectionWindow = new CharacterSelectionWindow();

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
        // delete mInfoPopup;
        delete mDebugWindow;
        delete mConsole;
        delete mMainMenuWindow;
        delete mGameSettings;
        delete mCombatWindow;
    }

    void WindowFactory::showCharacterActionChoice()
    {
        showActionChoice(PartyManager::getSingleton().getActiveCharacter());
    }

    void WindowFactory::showContainerContent(Container* container)
    {
        if (!mInventoryWindow)
            toggleInventoryWindow();

        mInventoryWindow->showContainerContent(container);
    }

    void WindowFactory::showMessageWindow(const CeGuiString& message)
    {
        MessageWindow* w = new MessageWindow("", message, MessageWindow::OK);
        w->setVisible(true);
    }

    void WindowFactory::showMainMenu()
    {
        mMainMenuWindow->setVisible(true);
    }

    void WindowFactory::toggleConsole()
    {
        mConsole->setVisible(!mConsole->isVisible());
        if (mConsole->isVisible())
            mConsole->getWindow()->activate();
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
        if (mMainMenuLoadWindow != NULL && mMainMenuLoadWindow->isVisible())
        {
            mMainMenuLoadWindow->setVisible(false, true);
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

            if (PartyManager::getSingleton().getActiveCharacter() != NULL)
            {
                Creature* creat = PartyManager::getSingleton().getActiveCharacter();

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

    bool WindowFactory::isInventoryWindowDestroyed()
    {
        return mInventoryWindow == NULL;
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
            mCharacterSheet->setCharacter(PartyManager::getSingleton().getActiveCharacter());
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

    void WindowFactory::showCharacterSheet(Creature* chara)
    {
        if (!mCharacterSheet)
        {
            mCharacterSheet = new CharacterSheetWindow();
        }
        mCharacterSheet->setCharacter(chara);
        mCharacterSheet->setVisible(true);
    }

    void WindowFactory::showDescriptionWindow(GameObject* obj)
    {
        (new GameObjectInfoWindow(obj, PartyManager::getSingleton().getActiveCharacter()))->setVisible(true);
    }

    void WindowFactory::toggleCharacterStateWindow()
    {
        mCharacterStateWindow->setVisible(!mCharacterStateWindow->isVisible());
    }

    void WindowFactory::toggleCharacterSelectionWindow()
    {
        mCharacterSelectionWindow->setVisible(!mCharacterSelectionWindow->isVisible());
        mCharacterSelectionWindow->update();
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

        if (WindowFactory::getSingletonPtr() != NULL)
        {
            if (WindowFactory::getSingleton().mConsole != NULL)
            {
                WindowFactory::getSingleton().mConsole->write(text + " \n");
                return Qnil;
            }
        }

        LOG_MESSAGE("Console", text);
        return Qnil;
    }

    void WindowFactory::setActiveCharacter(Creature* character)
    {
        mCharacterStateWindow->setCharacter(character);
        if (mCharacterSheet)
        {
            mCharacterSheet->setCharacter(character);
        }

        if (mInventoryWindow && mInventoryWindow->isVisible())
        {
            toggleInventoryWindow();
        }

        LOG_MESSAGE(Logger::UI, "CharacterStateWindow updated");
    }

    void WindowFactory::showActionChoice(GameObject* obj)
    {
        ActionChoiceWindow* w = new ActionChoiceWindow(PartyManager::getSingleton().getActiveCharacter());
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
            // mInfoPopup->showError();
        }

        if (popupTypes & WindowFactory::ICON_QUEST)
        {
            // mInfoPopup->showQuestBookChange();
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

    void WindowFactory::showGameOverWindow()
    {
        (new GameOverWindow())->setVisible(true);
    }

    void WindowFactory::logAllWindows()
    {
        CEGUI::Window* rootWnd = AbstractWindow::getRoot();
        CEGUI::uint count = rootWnd->getChildCount();
        for (CEGUI::uint chIdx = 0; chIdx < count; ++chIdx)
        {
            CEGUI::Window* wnd = rootWnd->getChildAtIdx(chIdx);
            LOG_MESSAGE(Logger::UI, wnd->getName() + (wnd->isVisible() ? " vis" : " nvis"));
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
