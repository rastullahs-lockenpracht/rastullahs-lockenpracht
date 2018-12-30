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

#include "SaveGameData.h"
#include "SaveGameFileReader.h"
#include "SaveGameManager.h"

#include "CoreSubsystem.h"

namespace rl
{
    SaveGameFileReader::SaveGameFileReader()
    {
    }

    void SaveGameFileReader::parseSaveGameFile(SaveGameFile* file, const SaveGameDataOrderMap& map)
    {
        mDocument = loadDocument(file->getDataStream());

        int version = getAttributeValueAsInteger(mDocument->RootElement(), "Engineversion");
        if (version >= CoreSubsystem::getSingleton().getEngineBuildNumber())
            LOG_MESSAGE(Logger::CORE, "Loading save game: Engine version is ok");
        else
            LOG_ERROR(Logger::CORE,
                "Loading save game: Save game version is newer then engine version! Loading save game could crash");

        for (SaveGameDataOrderMap::const_reverse_iterator data_iter = map.rbegin(); data_iter != map.rend();
             data_iter++)
        {
            data_iter->second->readData(this);
        }

        file->closeDataStream(); // make the save game writable

        mDocument = NULL;
        delete mDocument;
    }

    void SaveGameFileReader::parseSaveGameFileHeader(
        Ogre::DataStreamPtr& stream, const Ogre::String& groupName, SaveGameFile* file)
    {
        if (stream->size())
        {
            tinyxml2::XMLDocument* doc = loadDocument(stream);

            XmlElementList headerDefsXml = getElementsByTagName(mDocument->RootElement(), "header");
            if (!headerDefsXml.empty())
            {
                const tinyxml2::XMLElement* elem = headerDefsXml[0];
                /*tinyxml2::XMLNodeList* headerDefChildren = elem->getChildNodes();
                for(XMLSize_t childIdx = 0; childIdx < headerDefChildren->getLength(); childIdx++)
                {
                    tinyxml2::XMLNode* curChild = headerDefChildren->item(childIdx);
                    if (curChild->getNodeType() == tinyxml2::XMLNode::ELEMENT_NODE)
                    {
                        PropertyEntry entry = processProperty(static_cast<tinyxml2::XMLElement*>(curChild));
                        if (entry.first != "")
                        {
                           file->setProperty(entry.first, entry.second);
                        }
                    }
                }*/
                PropertyRecordPtr set = getPropertiesAsRecord(elem);
                file->setProperties(set);
            }
            delete doc;
        }
    }

    PropertyRecordPtr SaveGameFileReader::getAllPropertiesAsRecord(SaveGameData* data)
    {
        PropertyRecordPtr properties(new PropertyRecord());

        XmlElementList rootNodeList
            = getElementsByTagName(mDocument->RootElement(), data->getXmlNodeIdentifier().c_str());

        if (!rootNodeList.empty())
        {
            properties = getPropertiesAsRecord(rootNodeList[0]);
        }

        return properties;
    }
}
