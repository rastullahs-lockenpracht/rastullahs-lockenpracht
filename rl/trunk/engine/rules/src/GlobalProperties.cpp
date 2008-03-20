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

template<> rl::GlobalProperties* Ogre::Singleton<rl::GlobalProperties>::ms_Singleton = 0;

using namespace XERCES_CPP_NAMESPACE;

namespace rl
{
    GlobalProperties::GlobalProperties()
    {
        SaveGameManager::getSingleton().registerSaveGameData(this);
        mProperties = new PropertyRecord();
    }

    GlobalProperties::~GlobalProperties()
    {
        delete mProperties;
        SaveGameManager::getSingleton().unregisterSaveGameData(this);
    }

    
    /// Overrides from PropertyHolder
    const Property GlobalProperties::getProperty(const CeGuiString& key) const
    {
        return mProperties->getProperty(key);
    }

    void GlobalProperties::setProperty(const CeGuiString& key, const Property& value)
    {
        mProperties->setProperty(key, value);
    }

    PropertyKeys GlobalProperties::getAllPropertyKeys() const
    {
        return mProperties->getAllPropertyKeys();
    }

    bool GlobalProperties::hasProperty(const Ogre::String &key) const
    {
        return mProperties->hasProperty(key);
    }

    /// Overrides from SaveGameData
    /// Manages saving and loading from the SaveGameFile
    CeGuiString GlobalProperties::getXmlNodeIdentifier() const
    {
        return "global";
    }

    void GlobalProperties::writeData(SaveGameFileWriter* writer)
    {
        DOMElement* elem = writer->appendChildElement(writer->getDocument(), writer->getDocument()->getDocumentElement(), getXmlNodeIdentifier().c_str());

        writer->writeEachPropertyToElem(elem, mProperties->toPropertyMap());
    }

    void GlobalProperties::readData(SaveGameFileReader* reader)
    {
        reader->initializeXml();
        
        DOMNodeList* rootNodeList = reader->getDocument()->getDocumentElement()->getElementsByTagName(AutoXMLCh(getXmlNodeIdentifier().c_str()).data());
        if(rootNodeList->getLength())
        {
            DOMElement* elem = static_cast<DOMElement*>(rootNodeList->item(0));

            PropertyRecord properties = reader->getPropertiesAsRecord(elem);

            setProperties(&properties);
        }

        reader->shutdownXml();
    }

    int GlobalProperties::getPriority() const
    {
        return 1001;
    }
}

