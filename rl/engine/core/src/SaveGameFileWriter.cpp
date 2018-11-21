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

#include "SaveGameFileWriter.h"

#include "SaveGameManager.h"

#include <Properties.h>
#include <XmlProcessor.h>

#ifdef __APPLE__
#include <CEGUI/CEGUIPropertyHelper.h>
#else
#include <CEGUI/PropertyHelper.h>
#endif

#include <ContentModule.h>
#include <CoreSubsystem.h>
#include <TimeSource.h>

#include <cstdio>
#include <ctime>

using namespace Ogre;

namespace rl
{
    void SaveGameFileWriter::buildSaveGameFile(SaveGameFile* file, const SaveGameDataOrderMap& map)
    {
        //@toto: build

        mDocument = new tinyxml2::XMLDocument();
        mDocument->SetValue("SaveGameFile");

        tinyxml2::XMLElement* rootElem = mDocument->RootElement();
        // Write SaveGameVersion
        setAttributeValueAsString(rootElem, "SaveGameFormatVersion", "0.5");
        setAttributeValueAsInteger(rootElem, "Engineversion", CoreSubsystem::getSingleton().getEngineBuildNumber());

        // Write modul of save game
        tinyxml2::XMLElement* header = appendChildElement(rootElem, "header");

        PropertyRecordPtr headerSet = file->getAllProperties();
        for (PropertyRecord::PropertyRecordMap::const_iterator it_header = headerSet->begin();
             it_header != headerSet->end(); it_header++)
        {
            processProperty(header, PropertyEntry(it_header->first.c_str(), it_header->second));
        }

        ////Write globals
        // tinyxml2::XMLElement* globals = appendChildElement(mDocument, mDocument->getDocumentElement(), "globals");
        // tinyxml2::XMLElement* gameTime = appendChildElement(mDocument, globals, "gametime");
        // TimeSource* gameTimeSource = TimeSourceManager::getSingleton().getTimeSource(TimeSource::GAMETIME);
        // setAttributeValueAsInteger(gameTime, "milliseconds", gameTimeSource->getClock());

        for (SaveGameDataOrderMap::const_iterator data_iter = map.begin(); data_iter != map.end(); data_iter++)
        {
            data_iter->second->writeData(this);
        }

        std::FILE* fileHandle = std::fopen(file->buildFilename().c_str(), "w");
        if (fileHandle)
        {
            tinyxml2::XMLPrinter printer(fileHandle);
            mDocument->Print(&printer);
            std::fclose(fileHandle);
        }

        delete mDocument;
        mDocument = NULL;
    }

    void SaveGameFileWriter::writeEachProperty(SaveGameData* data, const rl::PropertyMap& map)
    {
        tinyxml2::XMLElement* saveElem
            = appendChildElement(mDocument->RootElement(), data->getXmlNodeIdentifier().c_str());

        XmlPropertyWriter::writeEachPropertyToElem(saveElem, map);
    }
}
