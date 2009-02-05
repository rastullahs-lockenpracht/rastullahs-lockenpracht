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
#include "SaveAbleSerializer.h"
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
		listSaveGames();
    }

    //------------------------------------------------------- LoadEvent

    bool GameSaveLoadWindow::handleLoadEvent()
    {
        LOG_MESSAGE(Logger::UI, "Load Button pressed");

		/*if(!WindowFactory::getSingleton().isInventoryWindowDestroyed())
			WindowFactory::getSingleton().toggleInventoryWindow();
	
        CeGuiString filename = mFilename->getText();

        if(filename != "")
        {

        }*/

        return true;
    }

    //------------------------------------------------------- SaveEvent

    bool GameSaveLoadWindow::handleSaveEvent()
    {
        LOG_MESSAGE(Logger::UI, "Save Button pressed");

        CeGuiString filename = mFilename->getText();

        if(filename != "")
        {
			if(SaveAbleSerializer::getSingleton().SaveGameExists(filename, CoreSubsystem::getSingleton().getActiveAdventureModule()->getId()))
			{
				SaveAbleSerializer::getSingleton().writeSaveGame(SaveAbleSerializer::getSingleton().getSaveGame(filename, CoreSubsystem::getSingleton().getActiveAdventureModule()->getId()));
			}
			else
			{
			}
			SaveAbleSerializer::getSingleton().updateFileList();
			listSaveGames();
        }
        else
        {
            WindowFactory::getSingleton().showMessageWindow("Bitte einen Namen für den Spielstand eingeben");
        }


        return true;
    }

    //------------------------------------------------------- DeleteEvent

    bool GameSaveLoadWindow::handleDeleteEvent()
    {
        LOG_MESSAGE(Logger::UI, "Delete Button pressed");

        return true;
    }

    void GameSaveLoadWindow::listSaveGames()
    {
		mSaveGameTable->clearAllSelections();

		while(mSaveGameTable->getRowCount() > 0)
			mSaveGameTable->removeRow(0);

		SaveGameEntryMap saveGames = SaveAbleSerializer::getSingleton().listSaveGames(CoreSubsystem::getSingleton().getActiveAdventureModule()->getId());
		SaveGameEntryMap::const_iterator iter = saveGames.begin();

		for(int i = 0; iter != saveGames.end(); i++, iter++)
		{
			mSaveGameTable->addRow(i);
			mSaveGameTable->setItem(new ListboxTextItem(iter->second->getName(),0,iter->second),0,i); 
			mSaveGameTable->setItem(new ListboxTextItem(iter->second->getProperty(SaveGameFile::PROPERTY_TIME).toString()),1,i);
		}

        mSaveGameTable->autoSizeColumnHeader(0);
        mSaveGameTable->autoSizeColumnHeader(1);
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
