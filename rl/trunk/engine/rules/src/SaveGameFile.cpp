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

    SaveGameFile::SaveGameFile(const CeGuiString &name)
    {
        mName = name;
    }

    SaveGameFile::~SaveGameFile()
    {
    }

    CeGuiString SaveGameFile::buildFilename()
    {
        return ConfigurationManager::getSingleton().getModulesRootDirectory() + /*"/" 
            + CoreSubsystem::getSingleton().getActiveAdventureModule()->getId()*/ + "/saves/" + mName + ".save";
    }

    CeGuiString SaveGameFile::getName()
    {
        return mName;
    }

    Ogre::DataStreamPtr SaveGameFile::getDataStream()
    {
        ///@todo: decryption
        return Ogre::DataStreamPtr( new Ogre::FileHandleDataStream(fopen(this->buildFilename().c_str(), "r")));
    }

    XERCES_CPP_NAMESPACE::XMLFormatTarget* SaveGameFile::getFormatTarget()
    {
        ///@todo: encryption
        return new XERCES_CPP_NAMESPACE::LocalFileFormatTarget(this->buildFilename().c_str());
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
}
