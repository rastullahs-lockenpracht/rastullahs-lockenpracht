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
 * \file SaveLoadWindow.cpp
 *
 * Implementation of the Save/Load dialog
 */

#include <boost/bind.hpp>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIListboxTextItem.h>
#include <Exception.h>

#include "SaveLoadWindow.h"

using namespace CEGUI;

namespace rl {

    //------------------------------------------------------- Constructor

    SaveLoadWindow::SaveLoadWindow() :
        AbstractWindow("saveloadwindow.xml", WND_MOUSE_INPUT)
    {
        // Get a access to the filename edit box
        mFilename = getEditbox("SaveLoadWindow/FileSheet/Filename");
        RlAssert(mFilename != NULL, "SaveLoadWindow/FileSheet/File is null");
        
        // Get a access to the savegame table
        mSaveGameTable = getMultiColumnList("SaveLoadWindow/FileSheet/SaveGameTable");
        RlAssert(mSaveGameTable != NULL, "SaveLoadWindow/FileSheet/SaveGameTable is null");

        mSaveGameTable->addColumn( (utf8*)"Filename", 0, cegui_reldim(0.7));
        mSaveGameTable->addColumn( (utf8*)"Date", 1, cegui_reldim(0.3));
        
        centerWindow();

        bindDestroyWindowToXButton();
        bindDestroyWindowToClick(getWindow("SaveLoadWindow/ButtonSheet/CancelButton"));
        
    }

    //------------------------------------------------------- Destructor

    SaveLoadWindow::~SaveLoadWindow() 
    {        
    }

    //------------------------------------------------------- initialize

    void SaveLoadWindow::initialize()
    {
        //mSaveGameTable->autoSizeColumnHeader(0);
        //mSaveGameTable->autoSizeColumnHeader(1);
    }

} // namespace rl
