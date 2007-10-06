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

#include "SaveGameFile.h"
#include "OgreString.h"
#include <xercesc/framework/LocalFileFormatTarget.hpp>

#include <ConfigurationManager.h>
#include <CoreSubsystem.h>
#include <ContentModule.h>

namespace rl
{

    SaveGameFile::SaveGameFile(const CeGuiString &name) : mStream((Ogre::DataStream*)NULL)
    {
        mName = name;

        
    }

    SaveGameFile::~SaveGameFile()
    {
    }

    CeGuiString SaveGameFile::buildFilename()
    {
        return ConfigurationManager::getSingleton().getModulesRootDirectory() + "/saves/" + mName + ".save";
    }

    CeGuiString SaveGameFile::getName()
    {
        return mName;
    }

    Ogre::DataStreamPtr &SaveGameFile::getDataStream()
    {
        Ogre::ResourceGroupManager::getSingleton().createResourceGroup("SaveGame");

        mScriptPatterns.push_back((mName + ".save").c_str());

        Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(ConfigurationManager::getSingleton().getModulesRootDirectory() 
            + "/saves", "FileSystem", "SaveGame");
        Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("SaveGame");

        ///@todo: decryption
        return mStream;
    }

    void SaveGameFile::closeDataStream()
    {
        Ogre::ResourceGroupManager::getSingleton().clearResourceGroup("SaveGame"); //close all resource files -> make them writable
        Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup("SaveGame");

        Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
    }

    XERCES_CPP_NAMESPACE::XMLFormatTarget* SaveGameFile::getFormatTarget()
    {
        ///@todo: encryption
        return new XERCES_CPP_NAMESPACE::LocalFileFormatTarget(this->buildFilename().c_str());
    }

    void SaveGameFile::deleteFileFromStorage()
    {
        if(remove(buildFilename().c_str()))
            LOG_ERROR(Logger::RULES, "SaveGameFile is not removed from filesystem. The file doesn't exists!");
        else 
            LOG_MESSAGE(Logger::RULES,"SaveGameFile successfully removed from filesystem");
    }

    bool SaveGameFile::saveGameExists()
    {
        return !Ogre::DataStreamPtr( new Ogre::FileHandleDataStream(fopen(this->buildFilename().c_str(), "r"))).isNull();
    }

    const Property SaveGameFile::getProperty(const Ogre::String& key) const
    {
        if(key == "ModuleID")
            return Property(mModuleID);
        else if(key == "Time")
            return Property(mLocalTime);
        else
            return Property();
    }

    void SaveGameFile::setProperty(const Ogre::String& key, const Property& value)
    {
        if(key == "ModuleID")
        {
            if(value.isString())
                mModuleID = value.toString();
        }
        else if(key == "Time")
        {
            if(value.isString())
                mLocalTime = value.toString();
        }
    }

    PropertySet* SaveGameFile::getAllProperties() const
    {
        PropertySet* set = new PropertySet();
        set->setProperty("ModuleID", getProperty("ModuleID"));
        set->setProperty("Time", getProperty("Time"));
        return set;
    }

    const Ogre::StringVector& SaveGameFile::getScriptPatterns() const
    {
        return mScriptPatterns;
    }

    void SaveGameFile::parseScript(Ogre::DataStreamPtr &stream, const Ogre::String &groupName)
    {
        mStream = stream;
        LOG_ERROR(Logger::CORE, "Stream Loaded");
    }

    Ogre::Real SaveGameFile::getLoadingOrder() const
    {
        return 1000.0f;
    }
}
