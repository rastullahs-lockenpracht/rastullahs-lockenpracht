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

#include <XmlHelper.h>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <Properties.h>

#include "GameObjectManager.h"
#include "QuestBook.h"
#include "RulesSubsystem.h"

#include <OgreResourceManager.h>
#include <CEGUIPropertyHelper.h>

#include <ctime>

using namespace XERCES_CPP_NAMESPACE;
using namespace Ogre;

namespace rl
{
    void SaveGameFileWriter::buildSaveGameFile(SaveGameFile *file)
    {
        //@toto: build
        XMLPlatformUtils::Initialize();

        XmlHelper::initializeTranscoder();

        XMLCh tempStr[100];
        XMLString::transcode("LS", tempStr, 99);
        mImplementation = DOMImplementationRegistry::getDOMImplementation(tempStr);
        mWriter = ((DOMImplementationLS*)mImplementation)->createDOMWriter();
        mTarget = file->getFormatTarget();
        mDocument = ((DOMImplementation*)mImplementation)->createDocument(0, XMLString::transcode("SaveGameFile"), 0);
        mDocument->setNodeValue(XMLString::transcode("SaveGameFile"));

        if (mWriter->canSetFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true))
            mWriter->setFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true);

        if (mWriter->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
             mWriter->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);

        //Write date and time
        DOMElement* timeNode = XmlHelper::appendChildElement(mDocument, mDocument->getDocumentElement(), "time");
        
        time_t rawtime;
        tm* timeinfo;

        time ( &rawtime );
        timeinfo = localtime ( &rawtime );

        XmlHelper::setAttributeValueAsInteger(timeNode, "day", timeinfo->tm_mday);
        XmlHelper::setAttributeValueAsInteger(timeNode, "month", timeinfo->tm_mon);
        XmlHelper::setAttributeValueAsInteger(timeNode, "year", timeinfo->tm_year+1900);

        XmlHelper::setAttributeValueAsInteger(timeNode, "hour", timeinfo->tm_hour);
        XmlHelper::setAttributeValueAsInteger(timeNode, "minute", timeinfo->tm_min);
        XmlHelper::setAttributeValueAsInteger(timeNode, "second", timeinfo->tm_sec);

        //Write globals
        DOMElement* globals = XmlHelper::appendChildElement(mDocument, mDocument->getDocumentElement(), "globals");

        //Write Quests
        DOMElement* quests = XmlHelper::appendChildElement(mDocument, mDocument->getDocumentElement(), "quests");
        QuestBook* questBook = RulesSubsystem::getSingleton().getQuestBook();
        PropertySet* set = questBook->getAllProperties();
        PropertySetMap::const_iterator it_quests;
        for(it_quests = set->begin(); it_quests != set->end(); it_quests++)
        {
            this->processProperty(quests, PropertyEntry(it_quests->first.c_str(), it_quests->second));
        }

        //Write game objects
        DOMElement* gameobjects = XmlHelper::appendChildElement(mDocument, mDocument->getDocumentElement(), "gameobjects");
        
        std::list<const GameObject*>::const_iterator it_gameobjects;
        std::list<const GameObject*> gos;
        gos = GameObjectManager::getSingleton().getAllGameObjects();

        for(it_gameobjects = gos.begin(); it_gameobjects != gos.end(); it_gameobjects++)
        {
            DOMElement* gameobject = XmlHelper::appendChildElement(mDocument, gameobjects, "gameobject");
            XmlHelper::setAttributeValueAsInteger(gameobject, "ID", (*it_gameobjects)->getId());
            XmlHelper::setAttributeValueAsString(gameobject, "ClassID", (*it_gameobjects)->getClassId());

            PropertyMap map = (*it_gameobjects)->getAllProperties()->toPropertyMap();
            PropertyMap::iterator it_properties;
            for(it_properties = map.begin(); it_properties != map.end(); it_properties++)
            {
                this->processProperty(gameobject, PropertyEntry(it_properties->first.c_str(), it_properties->second));
            }
        }        

        //Write Zones?

        //Write scripts
        DOMElement* scripts = XmlHelper::appendChildElement(mDocument, mDocument->getDocumentElement(), "scripts");

        mWriter->writeNode(mTarget, *mDocument);

        mWriter->release();
        delete mTarget;
    }
}
