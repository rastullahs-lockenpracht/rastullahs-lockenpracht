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

/**
 * \file MainMenuLoadWindow.cpp
 *
 * Implementation of the Save/Load dialog
 */

#include <boost/bind.hpp>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIListboxTextItem.h>
#include <Exception.h>
#include <GameObjectManager.h>
#include <GameObject.h>
#include <Actor.h>
#include <OgreResourceGroupManager.h>

#include "MainMenuLoadWindow.h"
#include "MessageWindow.h"
#include "WindowFactory.h"
#include "ConfigurationManager.h"
#include "SaveGameManager.h"
#include "CoreSubsystem.h"
#include "ContentModule.h"

using namespace CEGUI;

namespace rl {

    //------------------------------------------------------- Constructor

    MainMenuLoadWindow::MainMenuLoadWindow(MainMenuWindow* win) :
        AbstractWindow("mainmenuloadwindow.xml", WIT_MOUSE_INPUT | WIT_KEYBOARD_INPUT)
    {
        // Get a access to the savegame table
        mSaveGameTable = getMultiColumnList("MainMenuLoadWindow/FileSheet/SaveGameTable");
        RlAssert(mSaveGameTable != NULL, "MainMenuLoadWindow/FileSheet/SaveGameTable is null");

        mSaveGameTable->addColumn( (utf8*)"Filename", 0, cegui_reldim(0.6));
        mSaveGameTable->addColumn( (utf8*)"Module", 1, cegui_reldim(0.2));
        mSaveGameTable->addColumn( (utf8*)"Date", 2, cegui_reldim(0.2));

        mSaveGameTable->setSelectionMode(MultiColumnList::RowSingle);
        mSaveGameTable->subscribeEvent(MultiColumnList::EventSelectionChanged, boost::bind(&MainMenuLoadWindow::handleSelectSaveGame, this));

        centerWindow();

        getPushButton("MainMenuLoadWindow/ButtonSheet/LoadButton")->subscribeEvent(
        CEGUI::Window::EventMouseClick,
        boost::bind(
            &MainMenuLoadWindow::handleLoadEvent,
            this
        ));

        getPushButton("MainMenuLoadWindow/ButtonSheet/DeleteButton")->subscribeEvent(
        CEGUI::Window::EventMouseClick,
        boost::bind(
            &MainMenuLoadWindow::handleDeleteEvent,
            this
        ));

        bindDestroyWindowToXButton();
        bindDestroyWindowToClick(getWindow("MainMenuLoadWindow/ButtonSheet/CancelButton"));

		mMainMenuWindow = win;
    }

    //------------------------------------------------------- Destructor

    MainMenuLoadWindow::~MainMenuLoadWindow()
    {
    }

    //------------------------------------------------------- initialize

    void MainMenuLoadWindow::initialize()
    {
        //mSaveGameTable->autoSizeColumnHeader(0);
        //mSaveGameTable->autoSizeColumnHeader(1);
        listSaveGames();
    }

    //------------------------------------------------------- LoadEvent

    bool MainMenuLoadWindow::handleLoadEvent()
    {
        LOG_MESSAGE(Logger::UI, "Load Button pressed");

		setVisible(false);
		mMainMenuWindow->setVisible(false);

		if(mSaveGameTable->getFirstSelectedItem())
		{
			uint SelectedRow = mSaveGameTable->getRowWithID(mSaveGameTable->getFirstSelectedItem()->getID());
			CeGuiString moduleName = mSaveGameTable->getItemAtGridReference(CEGUI::MCLGridRef(SelectedRow,1))->getText();
			ContentModule* module = NULL;
			module = CoreSubsystem::getSingleton().getModule(moduleName.c_str());
			assert(module != NULL /*MainMenuLoadWindow::handleLoadEvent()*/);
			CoreSubsystem::getSingleton().startAdventureModule(module);
			CeGuiString saveGameName = mSaveGameTable->getItemAtGridReference(CEGUI::MCLGridRef(SelectedRow,0))->getText();
			SaveGameManager::getSingleton().loadSaveGameFile(saveGameName, moduleName);
		}

        return true;
    }

    //------------------------------------------------------- DeleteEvent

    bool MainMenuLoadWindow::handleDeleteEvent()
    {
        LOG_MESSAGE(Logger::UI, "Delete Button pressed");

        return true;
    }

    void MainMenuLoadWindow::listSaveGames()
    {
        SaveGameEntryMap saveGames = SaveGameManager::getSingleton().listSaveGames();

        while(mSaveGameTable->getRowCount() > saveGames.size())
		    mSaveGameTable->removeRow(mSaveGameTable->getRowCount()-1);
        while(mSaveGameTable->getRowCount() < saveGames.size())
		    mSaveGameTable->addRow();
        
        int saveGameNum = 0;

        for(SaveGameEntryMap::iterator it = saveGames.begin(); it != saveGames.end(); it++)
        {
            mSaveGameTable->setItem(new CEGUI::ListboxTextItem(it->first), 0, saveGameNum);
            mSaveGameTable->setItem(new CEGUI::ListboxTextItem(it->second->getProperty(SaveGameFile::PROPERTY_MODULEID).toString()), 1, saveGameNum);
            LOG_MESSAGE(Logger::UI, "Module ID: " + it->second->getProperty(SaveGameFile::PROPERTY_MODULEID).toString());
            mSaveGameTable->setItem(new CEGUI::ListboxTextItem(it->second->getProperty(SaveGameFile::PROPERTY_TIME).toString()), 2, saveGameNum);
            saveGameNum++;
        }
        /*mSaveGameTable->autoSizeColumnHeader(0);
        mSaveGameTable->autoSizeColumnHeader(1);
        mSaveGameTable->autoSizeColumnHeader(2);*/
    }

    bool MainMenuLoadWindow::handleSelectSaveGame()
    {
        mSaveGameTable->getFirstSelectedItem()->setSelectionColours(CEGUI::colour(0.0f,0.0f,1.0f));
        /*if(mSaveGameTable->getFirstSelectedItem())
            mFilename->setText(mSaveGameTable->getFirstSelectedItem()->getText());*/
        return true;
    }

} // namespace rl
