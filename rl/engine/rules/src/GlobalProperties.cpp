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

#include "GlobalProperties.h"
#include "SaveGameManager.h"

template <> rl::GlobalProperties* Ogre::Singleton<rl::GlobalProperties>::msSingleton = 0;

namespace rl
{
    GlobalProperties::GlobalProperties()
    {
        SaveGameManager::getSingleton().registerSaveGameData(this);
    }

    GlobalProperties::~GlobalProperties()
    {
        SaveGameManager::getSingleton().unregisterSaveGameData(this);
    }

    /// Overrides from PropertyHolder
    const Property GlobalProperties::getProperty(const CeGuiString& key) const
    {
        return mProperties.getProperty(key);
    }

    void GlobalProperties::setProperty(const CeGuiString& key, const Property& value)
    {
        mProperties.setProperty(key, value);
    }

    PropertyKeys GlobalProperties::getAllPropertyKeys() const
    {
        return mProperties.getAllPropertyKeys();
    }

    bool GlobalProperties::hasProperty(const Ogre::String& key) const
    {
        return mProperties.hasProperty(key);
    }

    /// Overrides from SaveGameData
    /// Manages saving and loading from the SaveGameFile
    CeGuiString GlobalProperties::getXmlNodeIdentifier() const
    {
        return "global";
    }

    void GlobalProperties::writeData(SaveGameFileWriter* writer)
    {
        tinyxml2::XMLElement* elem
            = writer->appendChildElement(writer->getDocument()->RootElement(), getXmlNodeIdentifier().c_str());
        writer->writeEachPropertyToElem(elem, mProperties.toPropertyMap());
    }

    void GlobalProperties::readData(SaveGameFileReader* reader)
    {
        XmlElementList rootNodeList
            = reader->getElementsByTagName(reader->getDocument(), getXmlNodeIdentifier().c_str());
        if (!rootNodeList.empty())
        {
            const tinyxml2::XMLElement* elem = rootNodeList[0];

            PropertyRecordPtr properties = reader->getPropertiesAsRecord(elem);

            setProperties(properties);
        }
    }

    int GlobalProperties::getPriority() const
    {
        return 1001;
    }
}
