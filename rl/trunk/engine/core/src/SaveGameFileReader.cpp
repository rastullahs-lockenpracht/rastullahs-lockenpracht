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

#include "stdinc.h"

#include <xercesc/dom/DOM.hpp>

#include "SaveGameFileReader.h"

#include "SaveGameManager.h"

using namespace XERCES_CPP_NAMESPACE;

namespace rl
{
    SaveGameFileReader::SaveGameFileReader()
    {
    }

    void SaveGameFileReader::parseSaveGameFile(SaveGameFile* file, const SaveGameDataOrderMap &map)
    {
        initializeXml();

        mDocument = loadDocument(file->getDataStream());

        for(SaveGameDataOrderMap::const_reverse_iterator data_iter = map.rbegin(); data_iter != map.rend(); data_iter++)
        {
            data_iter->second->readData(this);
        }


        mDocument->release();

        file->closeDataStream(); //make the save game writable

        mDocument = NULL;

        shutdownXml();
    }

    void SaveGameFileReader::parseSaveGameFileHeader(Ogre::DataStreamPtr &stream, const Ogre::String &groupName, SaveGameFile* file)
    {
        initializeXml();

        if(stream->size())
        {
            DOMDocument* doc = loadDocument(stream);

             DOMNodeList* headerDefsXml = doc->getDocumentElement()->getElementsByTagName(AutoXMLCh("header").data());
             if(headerDefsXml->getLength())
             {
                 DOMElement* elem = static_cast<DOMElement*>(headerDefsXml->item(0));
                 /*DOMNodeList* headerDefChildren = elem->getChildNodes();
                 for(XMLSize_t childIdx = 0; childIdx < headerDefChildren->getLength(); childIdx++)
                 {
                     DOMNode* curChild = headerDefChildren->item(childIdx);
                     if (curChild->getNodeType() == DOMNode::ELEMENT_NODE)
                     {
                         PropertyEntry entry = processProperty(static_cast<DOMElement*>(curChild));
                         if(entry.first != "")
                         {
                            file->setProperty(entry.first, entry.second);
                         }
                     }
                 }*/
                 PropertyRecord set = getPropertiesAsRecord(elem);
                 file->setProperties(&set);
             }
        }

        shutdownXml();
    }

    PropertyRecord SaveGameFileReader::getAllPropertiesAsRecord(SaveGameData* data)
    {
        PropertyRecord properties;

        initializeXml();

        DOMNodeList* rootNodeList = getDocument()->getDocumentElement()->getElementsByTagName(AutoXMLCh(data->getXmlNodeIdentifier().c_str()).data());

        if (rootNodeList->getLength())
        {
            DOMNode* xmlPropElem = rootNodeList->item(0);
            properties = getPropertiesAsRecord(static_cast<DOMElement*>(xmlPropElem));
        }

        shutdownXml();

        return properties;
    }
}