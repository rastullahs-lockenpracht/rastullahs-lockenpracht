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

#ifndef __WindowFactory_H__
#define __WindowFactory_H__

#include "UiPrerequisites.h"

#include "FixRubyHeaders.h" // die Makros sind hier vor Ruby schon definiert
#include "FixRubyHeaders.h"
#include <ruby.h>

namespace rl
{

    class Actor;
    class AbstractWindow;
    class CharacterSelectionWindow;
    class CharacterSheetWindow;
    class CharacterStateWindow;
    class CloseConfirmationWindow;
    class CombatWindow;
    class Console;
    class Container;
    class Creature;
    class DebugWindow;
    class DialogCharacter;
    class GameLoggerWindow;
    class GameObject;
    class PropertiesWindow;
    class GameSettings;
    class InfoPopup;
    class InGameMenuWindow;
    class InventoryWindow;
    class JournalWindow;
    class LogWindow;
    class MainMenuWindow;
    class ObjectDescriptionWindow;
    class GameSaveLoadWindow;
    class MainMenuLoadWindow;
    class WindowUpdater;

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

        void setActiveCharacter(Creature* character);

        void showActionChoice(GameObject* obj);
        void showAboutWindow();
        void showCharacterActionChoice();
        void showCharacterSheet();
        void showCharacterSheet(Creature* chara);
        void showContainerContent(Container* container);
        void showDescriptionWindow(GameObject* obj);
        void showPropertiesWindow(GameObject* obj);
        void showExitConfirmation();
        void showJournalWindow();
        void showLogfiles();
        void showMessageWindow(const CeGuiString& message);
        void showMainMenu();
        void showObjectDescription(GameObject* object);
        void showPlaylist();
        void showGameSaveLoadWindow();
        void showMainMenuLoadWindow(MainMenuWindow* win);
        void hideMainMenuLoadWindow();
        void showPopupMessage(int popupTypes);
        void showGameOverWindow();
        /**
         * Creates a GameSettings object and displays its layout file on screen
         */
        void showGameSettings();

        void toggleConsole();
        void toggleDebugWindow();
        void toggleGameLogWindow();
        void toggleCharacterStateWindow();
        void toggleCharacterSelectionWindow();
        void toggleInGameGlobalMenu();
        void toggleInventoryWindow();
        void notifyInventoryWindowDestroyed();
        bool isInventoryWindowDestroyed();

        CombatWindow* getCombatWindow();

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
        MainMenuWindow* mMainMenuWindow;
        MainMenuLoadWindow* mMainMenuLoadWindow;
        CloseConfirmationWindow* mCloseConfirmationWindow;
        GameSettings* mGameSettings;
        CombatWindow* mCombatWindow;
        CharacterSelectionWindow* mCharacterSelectionWindow;
    };
}

#endif
