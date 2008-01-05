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
 * \file GameSaveLoadWindow.cpp
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

#include "GameSaveLoadWindow.h"
#include "MessageWindow.h"
#include "WindowFactory.h"
#include "ConfigurationManager.h"
#include "SaveGameManager.h"
#include "CoreSubsystem.h"
#include "ContentModule.h"

using namespace CEGUI;

namespace rl {

    //------------------------------------------------------- Constructor

    GameSaveLoadWindow::GameSaveLoadWindow() :
        AbstractWindow("gamesaveloadwindow.xml", WIT_MOUSE_INPUT | WIT_KEYBOARD_INPUT)
    {
        // Get a access to the filename edit box
        mFilename = getEditbox("GameSaveLoadWindow/FileSheet/Filename");
        RlAssert(mFilename != NULL, "GameSaveLoadWindow/FileSheet/Filename is null");

        mFilename->activate();

        // Get a access to the savegame table
        mSaveGameTable = getMultiColumnList("GameSaveLoadWindow/FileSheet/SaveGameTable");
        RlAssert(mSaveGameTable != NULL, "GameSaveLoadWindow/FileSheet/SaveGameTable is null");

        mSaveGameTable->addColumn( (utf8*)"Filename", 0, cegui_reldim(0.7));
        mSaveGameTable->addColumn( (utf8*)"Date", 1, cegui_reldim(0.3));

        mSaveGameTable->setSelectionMode(MultiColumnList::RowSingle);
        mSaveGameTable->subscribeEvent(MultiColumnList::EventSelectionChanged, boost::bind(&GameSaveLoadWindow::handleSelectSaveGame, this));

        centerWindow();

        getPushButton("GameSaveLoadWindow/ButtonSheet/LoadButton")->subscribeEvent(
        CEGUI::Window::EventMouseClick,
        boost::bind(
            &GameSaveLoadWindow::handleLoadEvent,
            this
        ));

        getPushButton("GameSaveLoadWindow/ButtonSheet/SaveButton")->subscribeEvent(
        CEGUI::Window::EventMouseClick,
        boost::bind(
            &GameSaveLoadWindow::handleSaveEvent,
            this
        ));

        getPushButton("GameSaveLoadWindow/ButtonSheet/DeleteButton")->subscribeEvent(
        CEGUI::Window::EventMouseClick,
        boost::bind(
            &GameSaveLoadWindow::handleDeleteEvent,
            this
        ));

        bindDestroyWindowToXButton();
        bindDestroyWindowToClick(getWindow("GameSaveLoadWindow/ButtonSheet/CancelButton"));

    }

    //------------------------------------------------------- Destructor

    GameSaveLoadWindow::~GameSaveLoadWindow()
    {
    }

    //------------------------------------------------------- initialize

    void GameSaveLoadWindow::initialize()
    {
        //mSaveGameTable->autoSizeColumnHeader(0);
        //mSaveGameTable->autoSizeColumnHeader(1);
        listSaveGames();
    }

    //------------------------------------------------------- LoadEvent

    bool GameSaveLoadWindow::handleLoadEvent()
    {
        LOG_MESSAGE(Logger::UI, "Load Button pressed");

        CeGuiString filename = mFilename->getText();

        if(filename != "")
        {

            if(SaveGameManager::getSingleton().SaveGameFileExists(filename))
                SaveGameManager::getSingleton().loadSaveGameFile(filename, CoreSubsystem::getSingleton().getActiveAdventureModule()->getId());
            else
            {
                LOG_ERROR(Logger::UI, "Save Game " + filename + " doesn't exist!");
                WindowFactory::getSingleton().showMessageWindow("Der Spielstand existiert nicht");
            }
        }

        return true;
    }

    //------------------------------------------------------- SaveEvent

    bool GameSaveLoadWindow::handleSaveEvent()
    {
        LOG_MESSAGE(Logger::UI, "Save Button pressed");

        CeGuiString filename = mFilename->getText();

        if(filename != "")
        {
            LOG_MESSAGE(Logger::UI, "Create a SaveGameFile");

            SaveGameManager::getSingleton().saveSaveGameFile(filename);

            LOG_MESSAGE(Logger::UI, "Created save game");
        }
        else
        {
            WindowFactory::getSingleton().showMessageWindow("Bitte einen Namen für den Spielstand eingeben");
        }

        listSaveGames();

        return true;
    }

    //------------------------------------------------------- DeleteEvent

    bool GameSaveLoadWindow::handleDeleteEvent()
    {
        LOG_MESSAGE(Logger::UI, "Delete Button pressed");
        if(SaveGameManager::getSingleton().SaveGameFileExists(mFilename->getText()))
        {
            SaveGameManager::getSingleton().deleteSaveGameFile(mFilename->getText());
            listSaveGames();
        }
        return true;
    }

    void GameSaveLoadWindow::listSaveGames()
    {
        SaveGameEntryMap saveGames = SaveGameManager::getSingleton().listSaveGames(CoreSubsystem::getSingleton().getActiveAdventureModule()->getId());
    
        while(mSaveGameTable->getRowCount() > saveGames.size())
		    mSaveGameTable->removeRow(mSaveGameTable->getRowCount()-1);
        while(mSaveGameTable->getRowCount() < saveGames.size())
		    mSaveGameTable->addRow();
        
        int saveGameNum = 0;

        for(SaveGameEntryMap::iterator it = saveGames.begin(); it != saveGames.end(); it++)
        {
            mSaveGameTable->setItem(new CEGUI::ListboxTextItem(it->first), 0, saveGameNum);
            mSaveGameTable->setItem(new CEGUI::ListboxTextItem(it->second->getProperty(SaveGameFile::PROPERTY_TIME)), 1, saveGameNum);
            saveGameNum++;
        }
        //mSaveGameTable->autoSizeColumnHeader(0);
        //mSaveGameTable->autoSizeColumnHeader(1);
    }

    bool GameSaveLoadWindow::handleSelectSaveGame()
    {
        if(mSaveGameTable->getFirstSelectedItem())
            mFilename->setText(mSaveGameTable->getFirstSelectedItem()->getText());
        return true;
    }

} // namespace rl
