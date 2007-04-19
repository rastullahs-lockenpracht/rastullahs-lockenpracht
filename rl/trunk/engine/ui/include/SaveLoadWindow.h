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

/**
 * \file SaveLoadWindow.h
 *
 * Declaration of the Save/Load dialog
 */

#ifndef __SaveLoadWindow_H__
#define __SaveLoadWindow_H__

#include "UiPrerequisites.h"
#include "AbstractWindow.h"

namespace rl {

    /**
     * The Save/Load window class
     *
     * This window is a dialog to save and load the game.
     */
    class _RlUiExport SaveLoadWindow : public AbstractWindow
    {
    public:

        /**
         * Constructor of the GameObject window
         */
        SaveLoadWindow();
        
        /**
         * Destructor of the GameObject window
         */
        virtual ~SaveLoadWindow();

        /**
         * Initializes the window with the specified game object and
         * fills the window with the properties of the GameObject.
         *
         * @param object The GameObject to be displayed
         */
        virtual void initialize();
        
    private:
        CEGUI::MultiColumnList     *mSaveGameTable;     //!< SaveGame table
        CEGUI::Editbox             *mFilename;         //!< Filename for the savegame 
    };
}

#endif
