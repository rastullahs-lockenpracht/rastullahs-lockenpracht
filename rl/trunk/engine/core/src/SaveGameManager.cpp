/* 
 * (C) 2003-2007. Team Pantheon. www.team-pantheon.de
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

#include <CoreSubsystem.h>
#include <ContentModule.h>
#include <ConfigurationManager.h>

#include <XmlProcessor.h>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

template<> rl::SaveGameManager* Ogre::Singleton<rl::SaveGameManager>::ms_Singleton = 0;

using namespace XERCES_CPP_NAMESPACE;

namespace rl
{
    SaveGameData::SaveGameData()
    {
        SaveGameManager::getSingleton().registerSaveGameData(this);
    }

    SaveGameData::~SaveGameData()
    {
        SaveGameManager::getSingleton().unregisterSaveGameData(this);
    }

    CeGuiString printTimeAsString(tm* time)
    {
        CeGuiString string = ((Ogre::String)(Ogre::StringConverter::toString(time->tm_mday) + "." 
            + Ogre::StringConverter::toString(time->tm_mon) + "."
            + Ogre::StringConverter::toString(time->tm_year + 1900) + " - " 
            + Ogre::StringConverter::toString(time->tm_hour) + ":"
            + Ogre::StringConverter::toString(time->tm_min) + ":" 
            + Ogre::StringConverter::toString(time->tm_sec))).c_str();
        return string;
    }

    SaveGameManager::SaveGameManager()
    { 
        Ogre::ResourceGroupManager::getSingleton().createResourceGroup("SaveGames");

        mScriptPatterns.push_back("*.save");
        Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(ConfigurationManager::getSingleton().getModulesRootDirectory() 
            + "/saves", "FileSystem", "SaveGames");
        Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("SaveGames");
        Ogre::ResourceGroupManager::getSingleton().clearResourceGroup("SaveGames"); //close all resource files -> make them writable
    }

    SaveGameManager::~SaveGameManager()
    {
        freeSaveGameMap();
    }

    SaveGameEntryMap SaveGameManager::listSaveGames()
    {
        return mSaveGames;
    }

    void SaveGameManager::saveSaveGameFile(const CeGuiString &name)
    {
        time_t rawTime;
        tm* localTime; 
        time(&rawTime);
        localTime = localtime(&rawTime); 
        SaveGameFile* file = new SaveGameFile(name);
        file->setProperty("Time", Property(printTimeAsString(localTime)));
        file->setProperty("ModuleID", Property(CoreSubsystem::getSingleton().getActiveAdventureModule()->getName()));

        mSaveGames[name] = file;

        SaveGameFileWriter writer;
        writer.buildSaveGameFile(file, mSaveGameDataSet);

        freeSaveGameMap();

        Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("SaveGames");
        Ogre::ResourceGroupManager::getSingleton().clearResourceGroup("SaveGames"); //close all resource files -> make them writable
    }

    void SaveGameManager::loadSaveGameFile(const CeGuiString &name)
    {
        if(SaveGameFileExists(name))
        {
            SaveGameFile file(name);
            SaveGameFileReader reader;
            reader.parseSaveGameFile(&file, mSaveGameDataSet);
            ///@todo: SaveGameReader
        }
    }

    void SaveGameManager::deleteSaveGameFile(const CeGuiString &name)
    {
        if(SaveGameFileExists(name))
        {
            static_cast<SaveGameFile*>(mSaveGames[name])->deleteFileFromStorage();
            delete mSaveGames[name];
            mSaveGames.erase(name);
        }
    }

    bool SaveGameManager::SaveGameFileExists(const CeGuiString &name)
    {
        if(mSaveGames.find(name) != mSaveGames.end())
            return true;
        return false;
    }
    
    const Ogre::StringVector& SaveGameManager::getScriptPatterns() const
    {
        return mScriptPatterns;
    }

    Ogre::Real SaveGameManager::getLoadingOrder() const
    {
        return 2000.0f;
    }

    void SaveGameManager::parseScript(Ogre::DataStreamPtr &stream, const Ogre::String &groupName)
    {
        Ogre::String name = stream->getName();
        name = name.substr(0, name.length()-5); //delete ".save" at the and of the name

        SaveGameFile* file = new SaveGameFile(name);
        
        LOG_MESSAGE(Logger::RULES, "Parsing header of save game: " + name);
        SaveGameFileReader reader;
        reader.parseSaveGameFileHeader(stream, groupName, file);
        
        mSaveGames[name] = file;
    }

    void SaveGameManager::registerSaveGameData(SaveGameData* data)
    {
        mSaveGameDataSet.insert(data);
    }

    void SaveGameManager::unregisterSaveGameData(SaveGameData* data)
    {
        mSaveGameDataSet.erase(data);
    }

    void SaveGameManager::freeSaveGameMap()
    {
        for(SaveGameEntryMap::const_iterator iter = mSaveGames.begin(); iter != mSaveGames.end(); iter++)
        {
            delete iter->second;
        }
        mSaveGames.clear();
    }
}