/*
 * (C) 2003-2008. Team Pantheon. www.team-pantheon.de
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
 *  along with this program; if not you can get it here:
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm
 */

#include "stdinc.h"

#include "SaveGameManager.h"

#include "ConfigurationManager.h"
#include "ContentModule.h"
#include "CoreMessages.h"
#include "CoreSubsystem.h"

#include "XmlProcessor.h"

template <> rl::SaveGameManager* Ogre::Singleton<rl::SaveGameManager>::msSingleton = 0;

namespace rl
{
    SaveGameData::SaveGameData()
    {
    }

    SaveGameData::~SaveGameData()
    {
    }

    CeGuiString printTimeAsString(tm* time)
    {
        CeGuiString string = ((Ogre::String)(Ogre::StringConverter::toString(time->tm_mday) + "."
                                  + Ogre::StringConverter::toString(time->tm_mon) + "."
                                  + Ogre::StringConverter::toString(time->tm_year + 1900) + " - "
                                  + Ogre::StringConverter::toString(time->tm_hour) + ":"
                                  + Ogre::StringConverter::toString(time->tm_min) + ":"
                                  + Ogre::StringConverter::toString(time->tm_sec)))
                                 .c_str();
        return string;
    }

    SaveGameManager::SaveGameManager()
        : mHighestSaveGameNumber(0)
    {
        Ogre::ResourceGroupManager::getSingleton().createResourceGroup("SaveGames");

        mScriptPatterns.push_back("*.save");
        Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
            ConfigurationManager::getSingleton().getModulesRootDirectory() + "/saves", "FileSystem", "SaveGames");
        Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("SaveGames");
        Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(
            "SaveGames"); // close all resource files -> make them writable
    }

    SaveGameManager::~SaveGameManager()
    {
        freeSaveGameMap();
    }

    SaveGameEntryMap SaveGameManager::listSaveGames()
    {
        return mSaveGames;
    }

    SaveGameEntryMap SaveGameManager::listSaveGames(const CeGuiString& moduleId)
    {
        SaveGameEntryMap entries;
        for (SaveGameEntryMap::const_iterator iter = mSaveGames.begin(); iter != mSaveGames.end(); iter++)
        {
            if (iter->second->getProperty(SaveGameFile::PROPERTY_MODULEID).toString() == moduleId)
            {
                entries[iter->first] = iter->second;
            }
        }
        return entries;
    }

    void SaveGameManager::saveSaveGameFile(const CeGuiString& name)
    {
        MessagePump::getSingleton().sendMessage<MessageType_SaveGameSaving>();

        time_t rawTime;
        tm* localTime;
        time(&rawTime);
        localTime = localtime(&rawTime);

        SaveGameFile* file = NULL;

        if (SaveGameFileExists(name, CoreSubsystem::getSingleton().getActiveAdventureModule()->getId()))
        {
            file = getSaveGameFile(name, CoreSubsystem::getSingleton().getActiveAdventureModule()->getId());
            // new SaveGameFile(name,getSaveGameFile(name,
            // CoreSubsystem::getSingleton().getActiveAdventureModule()->getId())->getId());
        }
        else
        {
            mHighestSaveGameNumber++;
            file = new SaveGameFile(name, mHighestSaveGameNumber);
        }

        file->setProperty(SaveGameFile::PROPERTY_TIME, Property(printTimeAsString(localTime)));
        file->setProperty(SaveGameFile::PROPERTY_MODULEID,
            Property(CoreSubsystem::getSingleton().getActiveAdventureModule()->getId()));
        file->setProperty(SaveGameFile::PROPERTY_MODULENAME,
            Property(CoreSubsystem::getSingleton().getActiveAdventureModule()->getName()));

        mSaveGames[file->getId()] = file;

        SaveGameFileWriter writer;
        writer.buildSaveGameFile(file, mSaveGameDataOrderMap);

        // freeSaveGameMap();

        // Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("SaveGames");
        // Ogre::ResourceGroupManager::getSingleton().clearResourceGroup("SaveGames"); //close all resource files ->
        // make them writable

        MessagePump::getSingleton().sendMessage<MessageType_SaveGameSaved>();
    }

    void SaveGameManager::loadSaveGameFile(const CeGuiString& name, const CeGuiString& moduleId)
    {
        if (SaveGameFileExists(name, moduleId))
        {
            MessagePump::getSingleton().sendMessage<MessageType_SaveGameLoading>();

            SaveGameFile* file = getSaveGameFile(name, moduleId);
            SaveGameFileReader reader;

            CoreSubsystem::getSingleton().getActiveAdventureModule()->unload();

            reader.parseSaveGameFile(file, mSaveGameDataOrderMap);
            ///@todo: SaveGameReader

            MessagePump::getSingleton().sendMessage<MessageType_SaveGameLoaded>();
        }
    }

    void SaveGameManager::loadSaveGameFile(int id)
    {
        if (mSaveGames.find(id) != mSaveGames.end())
        {
            MessagePump::getSingleton().sendMessage<MessageType_SaveGameLoading>();

            SaveGameFile* file = getSaveGameFile(id);
            SaveGameFileReader reader;

            CoreSubsystem::getSingleton().getActiveAdventureModule()->unload();

            reader.parseSaveGameFile(file, mSaveGameDataOrderMap);
            ///@todo: SaveGameReader

            MessagePump::getSingleton().sendMessage<MessageType_SaveGameLoaded>();
        }
    }

    void SaveGameManager::deleteSaveGameFile(const CeGuiString& name, const CeGuiString& moduleId)
    {
        if (SaveGameFileExists(name, moduleId))
        {
            int id = getSaveGameId(name, moduleId);
            static_cast<SaveGameFile*>(mSaveGames[id])->deleteFileFromStorage();
            delete mSaveGames[id];
            mSaveGames.erase(id);
        }
    }

    void SaveGameManager::deleteSaveGameFile(int id)
    {
        if (mSaveGames.find(id) == mSaveGames.end())
        {
            static_cast<SaveGameFile*>(mSaveGames[id])->deleteFileFromStorage();
            delete mSaveGames[id];
            mSaveGames.erase(id);
        }
    }

    bool SaveGameManager::SaveGameFileExists(const CeGuiString& name, const CeGuiString& moduleId)
    {
        bool saveGameFileExists = false;
        for (SaveGameEntryMap::const_iterator it = mSaveGames.begin(); it != mSaveGames.end() && !saveGameFileExists;
             ++it)
        {
            if (it->second->getProperty(SaveGameFile::PROPERTY_NAME).toString() == name
                && it->second->getProperty(SaveGameFile::PROPERTY_MODULEID).toString() == moduleId)
                saveGameFileExists = true;
        }
        return saveGameFileExists;
    }

    SaveGameFile* SaveGameManager::getSaveGameFile(const CeGuiString& name, const CeGuiString& moduleId)
    {
        if (SaveGameFileExists(name, moduleId))
            return mSaveGames[getSaveGameId(name, moduleId)];
        return NULL;
    }

    SaveGameFile* SaveGameManager::getSaveGameFile(int id)
    {
        if (mSaveGames.find(id) != mSaveGames.end())
            return mSaveGames[id];
        return NULL;
    }

    int SaveGameManager::getSaveGameId(const CeGuiString& name, const CeGuiString& moduleId)
    {
        for (SaveGameEntryMap::const_iterator it = mSaveGames.begin(); it != mSaveGames.end(); ++it)
        {
            if (it->second->getProperty(SaveGameFile::PROPERTY_NAME).toString() == name
                && it->second->getProperty(SaveGameFile::PROPERTY_MODULEID).toString() == moduleId)
                return it->first;
        }
        return -1;
    }

    const Ogre::StringVector& SaveGameManager::getScriptPatterns() const
    {
        return mScriptPatterns;
    }

    Ogre::Real SaveGameManager::getLoadingOrder() const
    {
        return 2000.0f;
    }

    void SaveGameManager::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName)
    {
        Ogre::String name = stream->getName();
        name = name.substr(0, name.length() - 5); // delete ".save" at the and of the name
        int pointpos = name.find_last_of(".");
        name = name.substr(0, pointpos);

        if (Ogre::StringConverter::isNumber(name))
        {
            mHighestSaveGameNumber = std::max(mHighestSaveGameNumber, Ogre::StringConverter::parseInt(name));

            SaveGameFile* file = new SaveGameFile("", Ogre::StringConverter::parseInt(name));

            LOG_MESSAGE(Logger::RULES, "Parsing header of save game: " + name + ".save");
            SaveGameFileReader reader;
            reader.parseSaveGameFileHeader(stream, groupName, file);

            if (file->getProperty(SaveGameFile::PROPERTY_MODULEID) != "") // broken save game
                mSaveGames[Ogre::StringConverter::parseInt(name)] = file;
        }
    }

    void SaveGameManager::registerSaveGameData(SaveGameData* data)
    {
        mSaveGameDataOrderMap.insert(std::make_pair(data->getPriority(), data));
    }

    void SaveGameManager::unregisterSaveGameData(SaveGameData* data)
    {
        int order = data->getPriority();
        SaveGameDataOrderMap::iterator oi = mSaveGameDataOrderMap.find(order);
        while (oi != mSaveGameDataOrderMap.end() && oi->first == order)
        {
            if (oi->second == data)
            {
                // erase does not invalidate on multimap, except current
                SaveGameDataOrderMap::iterator del = oi++;
                mSaveGameDataOrderMap.erase(del);
            }
            else
            {
                ++oi;
            }
        }
    }

    void SaveGameManager::freeSaveGameMap()
    {
        for (SaveGameEntryMap::const_iterator iter = mSaveGames.begin(); iter != mSaveGames.end(); iter++)
        {
            delete iter->second;
        }
        mSaveGames.clear();
    }

    int SaveGameManager::getHighestSaveGameNumber()
    {
        return mHighestSaveGameNumber;
    }
}
