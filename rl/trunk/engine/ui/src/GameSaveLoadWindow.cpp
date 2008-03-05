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
 * \file GameSaveLoadWindow.cpp
 *
 * Implementation of the Save/Load dialog
 */

#include <boost/bind.hpp>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIListboxTextItem.h>
#include <Exception.h>
#include <InputManager.h>
#include <GameObjectManager.h>
#include <GameObject.h>
#include <Actor.h>

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

        mSaveGameTable->addColumn( (utf8*)"Filename", 0, cegui_reldim(0.65));
        mSaveGameTable->addColumn( (utf8*)"Date", 1, cegui_reldim(0.35));

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

        mSelectionColour = CEGUI::ListboxItem::DefaultSelectionColour;
        mSelectionImageset = "RastullahLook-Images";
        mSelectionBrush = "ListboxSelectionBrush";
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

		if(!WindowFactory::getSingleton().isInventoryWindowDestroyed())
			WindowFactory::getSingleton().toggleInventoryWindow();
	
        CeGuiString filename = mFilename->getText();

        if(filename != "")
        {

            if(SaveGameManager::getSingleton().SaveGameFileExists(filename, CoreSubsystem::getSingleton().getActiveAdventureModule()->getId()))
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
        if(SaveGameManager::getSingleton().SaveGameFileExists(mFilename->getText(), CoreSubsystem::getSingleton().getActiveAdventureModule()->getId()))
        {
            SaveGameManager::getSingleton().deleteSaveGameFile(mFilename->getText(), CoreSubsystem::getSingleton().getActiveAdventureModule()->getId());
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
            ListboxTextItem* item = new CEGUI::ListboxTextItem(it->second->getName());
            item->setSelectionBrushImage(mSelectionImageset, mSelectionBrush);
            mSaveGameTable->setItem(item, 0, saveGameNum);
            item = new CEGUI::ListboxTextItem(it->second->getProperty(SaveGameFile::PROPERTY_TIME));
            item->setSelectionBrushImage(mSelectionImageset, mSelectionBrush);
            mSaveGameTable->setItem(item, 1, saveGameNum);
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

    //------------------------------------------------------- repeat some keys
    bool GameSaveLoadWindow::wantsKeyToRepeat(const int &key)
    {
        InputManager* im = InputManager::getSingletonPtr();
        static const CEGUI::utf8 NO_CHAR = 0;
        if( im->getKeyChar(key, im->getModifierCode()) != NO_CHAR || // keys that should be repeated
            key == CEGUI::Key::ArrowLeft ||
            key == CEGUI::Key::ArrowRight ||
            key == CEGUI::Key::Backspace ||
            key == CEGUI::Key::Delete )
            return true;

        return false;
    }
} // namespace rl
