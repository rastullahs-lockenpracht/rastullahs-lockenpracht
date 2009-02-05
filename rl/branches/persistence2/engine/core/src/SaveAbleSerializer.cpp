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

#include "stdinc.h"

#include "SaveAbleSerializer.h"
#include "ConfigurationManager.h"

template<> rl::SaveAbleSerializer* Ogre::Singleton<rl::SaveAbleSerializer>::ms_Singleton = 0;



namespace rl
{
	CeGuiString convertTimeToString(tm* time)
    {
        CeGuiString string = ((Ogre::String)(Ogre::StringConverter::toString(time->tm_mday) + "." 
            + Ogre::StringConverter::toString(time->tm_mon) + "."
            + Ogre::StringConverter::toString(time->tm_year + 1900) + " - " 
            + Ogre::StringConverter::toString(time->tm_hour) + ":"
            + Ogre::StringConverter::toString(time->tm_min) + ":" 
            + Ogre::StringConverter::toString(time->tm_sec))).c_str();
        return string;
    }

	SaveAbleSerializer::SaveAbleSerializer()
	{
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup("SaveGameFiles");

        mScriptPatterns.push_back("*.save");
        Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(ConfigurationManager::getSingleton().getModulesRootDirectory() 
            + "/saves", "WriteableFileSystem", "SaveGameFiles");
        Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("SaveGameFiles");
	}

	void SaveAbleSerializer::readFromSaveGameFile(rl::SaveGameFile *file)
	{
	}

	void SaveAbleSerializer::writeToSaveGameFile(rl::SaveGameFile *file)
	{
		if(file)
		{
			mWriter.buildSaveGameFile(file);
		}
		else
			LOG_ERROR("CORE", "Error: could not save game, ne file given!!");
	}

	void SaveAbleSerializer::parseScript(Ogre::DataStreamPtr &stream, const Ogre::String &groupName)
	{
		LOG_MESSAGE(Logger::CORE, "Parsing headers of save game file: " + stream->getName() + " Resource group: " + groupName);
		SaveGameFile* file = new SaveGameFile(Ogre::StringConverter::parseInt(stream->getName()),(WriteableDataStreamPtr)static_cast<WriteableDataStream*>(stream.get()));
		mReader.parseSaveGameFileHeader(file);
		mHighestSaveGameNumber = Ogre::StringConverter::parseInt(stream->getName()) > mHighestSaveGameNumber ? Ogre::StringConverter::parseInt(stream->getName()) : mHighestSaveGameNumber;
		mSaveGameFiles[Ogre::StringConverter::parseInt(stream->getName())] = file;
	}

	const Ogre::StringVector& SaveAbleSerializer::getScriptPatterns() const
    {
        return mScriptPatterns;
    }

    Ogre::Real SaveAbleSerializer::getLoadingOrder() const
    {
        return 2000.0f;
    }
	
	SaveAbleSerializer::SaveGameEntryMap SaveAbleSerializer::listSaveGames() const
	{
		return mSaveGameFiles;
	}

	SaveAbleSerializer::SaveGameEntryMap SaveAbleSerializer::listSaveGames(const CeGuiString &moduleId) const
	{
		SaveGameEntryMap files;
		SaveGameEntryMap::const_iterator iter;
		for(iter = mSaveGameFiles.begin(); iter != mSaveGameFiles.end(); iter++)
		{
			if(iter->second->getProperty(SaveGameFile::PROPERTY_MODULEID).toString() == moduleId)
				files[iter->first] = iter->second;
		}
		return files;
	}

	bool SaveAbleSerializer::SaveGameExists(const CeGuiString &name, const CeGuiString &moduleId)
	{
		bool exists = false;
		SaveGameEntryMap::const_iterator iter;
		for(iter = mSaveGameFiles.begin(); iter != mSaveGameFiles.end() && !exists; iter++)
		{
			if(iter->second->getProperty(SaveGameFile::PROPERTY_MODULEID).toString() == moduleId)
			{
				if(iter->second->getName() == name)
					exists = true;
			}
		}
		return exists;
	}

	SaveGameFile* SaveAbleSerializer::getSaveGame(const CeGuiString &name, const CeGuiString &moduleId)
	{
		SaveGameEntryMap::const_iterator iter;
		for(iter = mSaveGameFiles.begin(); iter != mSaveGameFiles.end(); iter++)
		{
			if(iter->second->getProperty(SaveGameFile::PROPERTY_MODULEID).toString() == moduleId)
			{
				if(iter->second->getName() == name)
					return iter->second;
			}
		}
		return NULL;
	}

	void SaveAbleSerializer::updateFileList()
	{
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup("SaveGameFiles");

		SaveGameEntryMap::iterator iter;
		for(iter = mSaveGameFiles.begin(); iter != mSaveGameFiles.end(); iter++)
		{
			delete iter->second;
		}

		mSaveGameFiles.clear();

		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("SaveGameFiles");
	}


///---------------------------------------------------------------


	SaveGameFile* SaveAbleSerializer::createNewSaveGame()
	{
		//SaveGameFile* file = new SaveGameFile(NULL);
		updateFileList();
		return NULL;
	}

	void SaveAbleSerializer::deleteSaveGame(int fileId)
	{
	}

	void SaveAbleSerializer::deleteSaveGame(SaveGameFile* save)
	{
		//TODO!!!!!!111elf
	}

	void SaveAbleSerializer::writeSaveGame(int fileId)
	{
		if(mSaveGameFiles.find(fileId) != mSaveGameFiles.end())
			writeSaveGame(mSaveGameFiles[fileId]);
	}

	void SaveAbleSerializer::writeSaveGame(SaveGameFile* save)
	{
		if(save != NULL)
			writeToSaveGameFile(save);
	}

	void SaveAbleSerializer::readSaveGame(int fileId)
	{
		if(mSaveGameFiles.find(fileId) != mSaveGameFiles.end())
			readSaveGame(mSaveGameFiles[fileId]);
	}

	void SaveAbleSerializer::readSaveGame(SaveGameFile* save)
	{
		if(save != NULL)
			readFromSaveGameFile(save);
	}

	int SaveAbleSerializer::getHighestSaveGameNumber()
	{
		return mHighestSaveGameNumber;
	}
}
