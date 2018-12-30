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
 * \file GameSaveLoadWindow.h
 *
 * Declaration of the Save/Load dialog
 */

#ifndef __GameSaveLoadWindow_H__
#define __GameSaveLoadWindow_H__

#include "AbstractWindow.h"
#include "UiPrerequisites.h"

namespace rl
{

    /**
     * The Save/Load window class
     *
     * This window is a dialog to save and load the game.
     */
    class _RlUiExport GameSaveLoadWindow : public AbstractWindow
    {
    public:
        /**
         * Constructor of the GameObject window
         */
        GameSaveLoadWindow();

        /**
         * Destructor of the GameObject window
         */
        virtual ~GameSaveLoadWindow();

        /**
         * Initializes the window with the specified game object and
         * fills the window with the properties of the GameObject.
         *
         * @param object The GameObject to be displayed
         */
        virtual void initialize();

        // derived from AbstractWindow
        bool wantsKeyToRepeat(const int& key);

    private:
        bool handleLoadEvent();
        bool handleSaveEvent();
        bool handleDeleteEvent();
        bool handleSelectSaveGame();

        void listSaveGames();

        CEGUI::MultiColumnList* mSaveGameTable; //!< SaveGame table
        CEGUI::Editbox* mFilename; //!< Filename for the savegame

        CeGuiString mSelectionBrush;
        CeGuiString mSelectionImageset;
        CEGUI::Colour mSelectionColour;
    };
}

#endif
