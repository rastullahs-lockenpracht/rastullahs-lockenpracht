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
 * \file SaveLoadWindow.cpp
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

#include "SaveLoadWindow.h"
#include "SaveGameFile.h"
#include "SaveGameFileWriter.h"
#include "CoreSubsystem.h"
#include "ContentModule.h"
#include "ConfigurationManager.h"

using namespace CEGUI;

namespace rl {

    //------------------------------------------------------- Constructor

    SaveLoadWindow::SaveLoadWindow() :
        AbstractWindow("saveloadwindow.xml", WIT_MOUSE_INPUT | WIT_KEYBOARD_INPUT)
    {
        // Get a access to the filename edit box
        mFilename = getEditbox("SaveLoadWindow/FileSheet/Filename");
        RlAssert(mFilename != NULL, "SaveLoadWindow/FileSheet/Filename is null");

        mFilename->activate();

        // Get a access to the savegame table
        mSaveGameTable = getMultiColumnList("SaveLoadWindow/FileSheet/SaveGameTable");
        RlAssert(mSaveGameTable != NULL, "SaveLoadWindow/FileSheet/SaveGameTable is null");

        mSaveGameTable->addColumn( (utf8*)"Filename", 0, cegui_reldim(0.7));
        mSaveGameTable->addColumn( (utf8*)"Date", 1, cegui_reldim(0.3));

        centerWindow();

        getPushButton("SaveLoadWindow/ButtonSheet/LoadButton")->subscribeEvent(
        CEGUI::Window::EventMouseClick,
        boost::bind(
            &SaveLoadWindow::handleLoadEvent,
            this
        ));

        getPushButton("SaveLoadWindow/ButtonSheet/SaveButton")->subscribeEvent(
        CEGUI::Window::EventMouseClick,
        boost::bind(
            &SaveLoadWindow::handleSaveEvent,
            this
        ));

        getPushButton("SaveLoadWindow/ButtonSheet/DeleteButton")->subscribeEvent(
        CEGUI::Window::EventMouseClick,
        boost::bind(
            &SaveLoadWindow::handleDeleteEvent,
            this
        ));

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
        listSaveGames();
    }

    //------------------------------------------------------- LoadEvent

    bool SaveLoadWindow::handleLoadEvent()
    {
        LOG_MESSAGE(Logger::UI, "Load Button pressed");
        return true;
    }

    //------------------------------------------------------- SaveEvent

    bool SaveLoadWindow::handleSaveEvent()
    {
        LOG_MESSAGE(Logger::UI, "Save Button pressed");

        LOG_MESSAGE(Logger::UI, "Create a SaveGameFile");

        SaveGameFile saveGameFile(mFilename->getText());
        saveGameFile.setModulName(CoreSubsystem::getSingleton().getActiveAdventureModule()->getName());

        /*std::list<const GameObject*>::const_iterator it;
        std::list<const GameObject*> gos;
        gos = GameObjectManager::getSingleton().getAllGameObjects();

        std::vector<PropertySet*> sets;
        for( it=gos.begin(); it!=gos.end(); ++it )
        {
            sets.push_back((*it)->getAllProperties());
        }*/
        SaveGameFileWriter saveGameFileWriter;
        //saveGameFileWriter.setPropertySets(sets);
        saveGameFileWriter.buildSaveGameFile(&saveGameFile);

        LOG_MESSAGE(Logger::UI, "Created save game");

        listSaveGames();

        return true;
    }

    //------------------------------------------------------- DeleteEvent

    bool SaveLoadWindow::handleDeleteEvent()
    {
        LOG_MESSAGE(Logger::UI, "Delete Button pressed");
        return true;
    }

    void SaveLoadWindow::listSaveGames()
    {
        Ogre::ResourceGroupManager::getSingleton().createResourceGroup("SaveGames");
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(ConfigurationManager::getSingleton().getModulesRootDirectory() + Ogre::String("/")
            + Ogre::String(CoreSubsystem::getSingleton().getActiveAdventureModule()->getName().c_str()) + Ogre::String("/saves"), "FileSystem", "SaveGames");
        Ogre::StringVectorPtr saveGames = Ogre::ResourceGroupManager::getSingleton().listResourceNames("SaveGames");
        Ogre::StringVector::iterator it;
        
        while(mSaveGameTable->getRowCount() > saveGames->size())
		    mSaveGameTable->removeRow(mSaveGameTable->getRowCount()-1);
        while(mSaveGameTable->getRowCount() < saveGames->size())
		    mSaveGameTable->addRow();
        
        int saveGameNum = 0;

        for(it = saveGames->begin(); it != saveGames->end(); it++)
        {
            mSaveGameTable->setItem(new CEGUI::ListboxTextItem(it->data()), 0, saveGameNum);
            saveGameNum++;
        }

        Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup("SaveGames");
    }

} // namespace rl
