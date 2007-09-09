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

#include "SaveGameFileWriter.h"

#include <XmlProcessor.h>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <Properties.h>

#include "GameObjectManager.h"
#include "QuestBook.h"
#include "RulesSubsystem.h"

#include <OgreResourceManager.h>
#include <CEGUIPropertyHelper.h>
#include <CoreSubsystem.h>
#include <ContentModule.h>
#include <TimeSource.h>

#include <ctime>

using namespace XERCES_CPP_NAMESPACE;
using namespace Ogre;

namespace rl
{
    void SaveGameFileWriter::buildSaveGameFile(SaveGameFile *file)
    {
        //@toto: build
        initializeXml();

        XMLCh tempStr[100];
        XMLString::transcode("LS", tempStr, 99);
        mImplementation = DOMImplementationRegistry::getDOMImplementation(tempStr);
        mWriter = static_cast<DOMImplementationLS*>(mImplementation)->createDOMWriter();
        mTarget = file->getFormatTarget();
        mDocument = static_cast<DOMImplementation*>(mImplementation)->createDocument(0, XMLString::transcode("SaveGameFile"), 0);
        mDocument->setNodeValue(XMLString::transcode("SaveGameFile"));

        if (mWriter->canSetFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true))
            mWriter->setFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true);

        if (mWriter->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
             mWriter->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);

        //Write modul of save game
        DOMElement* header = appendChildElement(mDocument, mDocument->getDocumentElement(), "header");
        setAttributeValueAsString(header, "ModuleID", CoreSubsystem::getSingleton().getActiveAdventureModule()->getId());
        setAttributeValueAsInteger(header, "EngineVersion", CoreSubsystem::getSingleton().getEngineBuildNumber());
        
        //Write date and time
        DOMElement* timeNode = appendChildElement(mDocument, mDocument->getDocumentElement(), "time");
        
        tm* timeinfo = file->getLocalTime();

        setAttributeValueAsInteger(timeNode, "day", timeinfo->tm_mday);
        setAttributeValueAsInteger(timeNode, "month", timeinfo->tm_mon);
        setAttributeValueAsInteger(timeNode, "year", timeinfo->tm_year+1900);

        setAttributeValueAsInteger(timeNode, "hour", timeinfo->tm_hour);
        setAttributeValueAsInteger(timeNode, "minute", timeinfo->tm_min);
        setAttributeValueAsInteger(timeNode, "second", timeinfo->tm_sec);

        //Write globals
        DOMElement* globals = appendChildElement(mDocument, mDocument->getDocumentElement(), "globals");
        DOMElement* gameTime = appendChildElement(mDocument, globals, "gametime");
        TimeSource* gameTimeSource = TimeSourceManager::getSingleton().getTimeSource(TimeSource::GAMETIME);
        setAttributeValueAsInteger(gameTime, "milliseconds", gameTimeSource->getClock());

        //Write Quests
        DOMElement* quests = appendChildElement(mDocument, mDocument->getDocumentElement(), "quests");
        QuestBook* questBook = RulesSubsystem::getSingleton().getQuestBook();
        PropertySet* set = questBook->getAllProperties();
        PropertySetMap::const_iterator it_quests;
        for(it_quests = set->begin(); it_quests != set->end(); it_quests++)
        {
            this->processProperty(quests, PropertyEntry(it_quests->first.c_str(), it_quests->second));
        }

        //Write game objects
        DOMElement* gameobjects = appendChildElement(mDocument, mDocument->getDocumentElement(), "gameobjects");
        
        std::list<const GameObject*>::const_iterator it_gameobjects;
        std::list<const GameObject*> gos;
        gos = GameObjectManager::getSingleton().getAllGameObjects();

        for(it_gameobjects = gos.begin(); it_gameobjects != gos.end(); it_gameobjects++)
        {
            DOMElement* gameobject = appendChildElement(mDocument, gameobjects, "gameobject");
            setAttributeValueAsInteger(gameobject, "ID", (*it_gameobjects)->getId());
            setAttributeValueAsString(gameobject, "ClassID", (*it_gameobjects)->getClassId());

            PropertyMap map = (*it_gameobjects)->getAllProperties()->toPropertyMap();
            PropertyMap::iterator it_properties;
            for(it_properties = map.begin(); it_properties != map.end(); it_properties++)
            {
                this->processProperty(gameobject, PropertyEntry(it_properties->first.c_str(), it_properties->second));
            }
        }        

        //Write Zones?

        //Write scripts
        DOMElement* scripts = appendChildElement(mDocument, mDocument->getDocumentElement(), "scripts");

        mWriter->writeNode(mTarget, *mDocument);


        mWriter->release();

        delete mDocument;
        delete mTarget;

        shutdownXml();
    }
}
