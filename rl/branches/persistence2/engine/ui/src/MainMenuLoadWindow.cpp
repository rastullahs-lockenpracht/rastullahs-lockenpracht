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

/**
 * \file MainMenuLoadWindow.cpp
 *
 * Implementation of the Save/Load dialog
 */

#include <boost/bind.hpp>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIListboxTextItem.h>

#include "MainMenuLoadWindow.h"

#include "Exception.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Actor.h"
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

        mSaveGameTable->addColumn( (utf8*)"Filename", 0, cegui_reldim(0.35));
        mSaveGameTable->addColumn( (utf8*)"Module", 1, cegui_reldim(0.3));
        mSaveGameTable->addColumn( (utf8*)"Date", 2, cegui_reldim(0.35));

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

        mSelectionColour = CEGUI::ListboxItem::DefaultSelectionColour;
        mSelectionImageset = "RastullahLook-Images";
        mSelectionBrush = "ListboxSelectionBrush";

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
  //      if(mSaveGameTable->getFirstSelectedItem())
		//{
  //          setVisible(false);
		//    mMainMenuWindow->setVisible(false);
		//
  //          CeGuiString moduleId = ((SaveGameFile*)mSaveGameTable->getFirstSelectedItem()->getUserData())->getProperty(SaveGameFile::PROPERTY_MODULEID).toString();
		//	ContentModule* module = NULL;
		//	module = CoreSubsystem::getSingleton().getModule(moduleId.c_str());
		//	assert(module != NULL /*MainMenuLoadWindow::handleLoadEvent()*/);
		//	CoreSubsystem::getSingleton().startAdventureModule(module);
		//}

        return true;
    }

    //------------------------------------------------------- DeleteEvent

    bool MainMenuLoadWindow::handleDeleteEvent()
    {
        LOG_MESSAGE(Logger::UI, "Delete Button pressed");

        if(mSaveGameTable->getFirstSelectedItem())
		{
            mSaveGameTable->getRowWithID(mSaveGameTable->getFirstSelectedItem()->getID());
        }

        return true;
    }

    void MainMenuLoadWindow::listSaveGames()
    {
        mSaveGameTable->clearAllSelections();
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
