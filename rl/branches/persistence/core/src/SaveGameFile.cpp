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

#include "SaveGameFile.h"
#include "SaveGameManager.h"
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include "WriteableDataStreamFormatTarget.h"

#include <ConfigurationManager.h>
#include <CoreSubsystem.h>
#include <ContentModule.h>

namespace rl
{
    const Ogre::String SaveGameFile::PROPERTY_MODULEID = "moduleid";
    const Ogre::String SaveGameFile::PROPERTY_TIME = "time";
    const Ogre::String SaveGameFile::PROPERTY_NAME = "name";
    const Ogre::String SaveGameFile::PROPERTY_MODULENAME = "modulename";


    SaveGameFile::SaveGameFile(int id, WriteableDataStreamPtr stream) : mStream(stream)
    {
		mSaveGameId = id;  
    }

    SaveGameFile::~SaveGameFile()
    {
    }

    CeGuiString SaveGameFile::getName() const
    {
        return getProperty(PROPERTY_NAME);
    }

	CeGuiString SaveGameFile::getModuleId() const
	{
		return mModuleID;
	}

    int SaveGameFile::getId() const
    {
        return mSaveGameId;
    }

    WriteableDataStreamPtr SaveGameFile::getDataStream() const
    {
		return mStream;
    }

    XERCES_CPP_NAMESPACE::XMLFormatTarget* SaveGameFile::getFormatTarget()
    {
        ///@todo: encryption
		return new WriteableDataStreamFormatTarget(mStream);
    }

    void SaveGameFile::deleteFileFromStorage()
    {
        /*if(remove(buildFilename().c_str()))
            LOG_ERROR(Logger::RULES, "SaveGameFile is not removed from filesystem. The file doesn't exists!");
        else 
            LOG_MESSAGE(Logger::RULES,"SaveGameFile successfully removed from filesystem");*/
    }

    const Property SaveGameFile::getProperty(const CeGuiString& key) const
    {
        if(key == PROPERTY_MODULEID)
            return Property(mModuleID);
        else if(key == PROPERTY_TIME)
            return Property(mLocalTime);
        else if(key == PROPERTY_MODULENAME)
            return Property(mModuleName);
        else if(key == PROPERTY_NAME)
            return Property(mName);
        else
            return Property();
    }

    void SaveGameFile::setProperty(const CeGuiString& key, const Property& value)
    {
        if(key == PROPERTY_MODULEID)
        {
            if(value.isString())
                mModuleID = value.toString();
        }
        else if(key == PROPERTY_TIME)
        {
            if(value.isString())
                mLocalTime = value.toString();
        }
        else if(key == PROPERTY_NAME)
        {
            if(value.isString())
                mName = value.toString();
        }
        else if(key == PROPERTY_MODULENAME)
        {
            if(value.isString())
                mModuleName = value.toString();
        }
    }

    PropertyKeys SaveGameFile::getAllPropertyKeys() const
    {
        PropertyKeys set;
        set.insert(PROPERTY_MODULEID);
        set.insert(PROPERTY_TIME);
        set.insert(PROPERTY_NAME);
        set.insert(PROPERTY_MODULENAME);
        return set;
    }
}
