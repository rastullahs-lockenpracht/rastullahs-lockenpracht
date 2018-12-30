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

/**
 * \file MainMenuLoadWindow.h
 *
 * Declaration of the Save/Load dialog
 */

#ifndef __MainMenuLoadWindow_H__
#define __MainMenuLoadWindow_H__

#include "AbstractWindow.h"
#include "MainMenuWindow.h"
#include "UiPrerequisites.h"

namespace rl
{

    /**
     * The Save/Load window class
     *
     * This window is a dialog to save and load the game.
     */
    class _RlUiExport MainMenuLoadWindow : public AbstractWindow
    {
    public:
        /**
         * Constructor of the GameObject window
         */
        MainMenuLoadWindow(MainMenuWindow* win);

        /**
         * Destructor of the GameObject window
         */
        virtual ~MainMenuLoadWindow();

        /**
         * Initializes the window with the specified game object and
         * fills the window with the properties of the GameObject.
         *
         * @param object The GameObject to be displayed
         */
        virtual void initialize();

    private:
        bool handleLoadEvent();
        bool handleDeleteEvent();
        bool handleSelectSaveGame();

        void listSaveGames();

        CEGUI::MultiColumnList* mSaveGameTable; //!< SaveGame table

        MainMenuWindow* mMainMenuWindow;

        CeGuiString mSelectionBrush;
        CeGuiString mSelectionImageset;
        CEGUI::Colour mSelectionColour;
    };
}

#endif
