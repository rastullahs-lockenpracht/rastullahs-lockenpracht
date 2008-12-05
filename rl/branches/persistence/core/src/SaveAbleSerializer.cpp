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
		mSaveGameFiles[Ogre::StringConverter::parseInt(stream->getName())] = 
			new SaveGameFile(Ogre::StringConverter::parseInt(stream->getName()),
			WriteableDataStreamPtr(dynamic_cast<WriteableDataStream*>(stream.get())));
	}

	const Ogre::StringVector& SaveAbleSerializer::getScriptPatterns() const
    {
        return mScriptPatterns;
    }

    Ogre::Real SaveAbleSerializer::getLoadingOrder() const
    {
        return 2000.0f;
    }
	
	SaveAbleSerializer::SaveGameEntryMap SaveAbleSerializer::listSaveGames()
	{
		return mSaveGameFiles;
	}

	SaveAbleSerializer::SaveGameEntryMap SaveAbleSerializer::listSaveGames(const CeGuiString &moduleId)
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
}
