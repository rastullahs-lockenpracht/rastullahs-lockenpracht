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

#include "ScriptWrapper.h"
#include "TriggerFactory.h"
#include "Trigger.h"
#include "ZoneManager.h"
#include "Zone.h"


namespace rl
{
    TriggerFactory::TriggerFactory()
    {
        SaveGameManager::getSingleton().registerSaveGameData(this);
    }

    TriggerFactory::~TriggerFactory()
    {
        SaveGameManager::getSingleton().unregisterSaveGameData(this);
    }

    CeGuiString TriggerFactory::getXmlNodeIdentifier() const
    {
        return "triggerfactory";
    }

    using namespace XERCES_CPP_NAMESPACE;

    void TriggerFactory::writeData(SaveGameFileWriter* writer)
    {
        DOMElement* triggerParentNode = writer->appendChildElement(writer->getDocument(), writer->getDocument()->getDocumentElement(), getXmlNodeIdentifier().c_str());

        const ZoneManager::ZoneMap &zoneMap(ZoneManager::getSingleton().getAllZones());
        ZoneManager::ZoneMap::const_iterator zone;

        // look in all zones if there is a trigger
        for(zone = zoneMap.begin(); zone != zoneMap.end(); zone++)
        {
            std::list<Trigger*> allTriggers(zone->second->getTriggers());
            // search for triggers in a zone
            for(std::list<Trigger*>::iterator trigger = allTriggers.begin(); trigger != allTriggers.end(); trigger++)
            {
                if((*trigger)->needsToBeSaved())
                {
                    DOMElement* triggerNode = writer->appendChildElement(writer->getDocument(), triggerParentNode, "trigger");
                    writer->setAttributeValueAsStdString(triggerNode, "name", (*trigger)->getName());
                    writer->setAttributeValueAsStdString(triggerNode, "classname", (*trigger)->getClassName());
                    writer->setAttributeValueAsStdString(triggerNode, "zone", zone->first);

                    PropertyMap map = (*trigger)->getAllProperties()->toPropertyMap();
                    writer->writeEachPropertyToElem(triggerNode, map);
                }
            }
        }
    }

    void TriggerFactory::readData(SaveGameFileReader* reader)
    {
        // delete all triggers, that say that they should be saved.
        const ZoneManager::ZoneMap &zoneMap(ZoneManager::getSingleton().getAllZones());
        ZoneManager::ZoneMap::const_iterator zone;
        // look in all zones if there is a trigger
        for(zone = zoneMap.begin(); zone != zoneMap.end(); zone++)
        {
            std::list<Trigger*> allTriggers(zone->second->getTriggers());
            // search for triggers in a zone
            for(std::list<Trigger*>::iterator trigger = allTriggers.begin(); trigger != allTriggers.end(); trigger++)
            {
                if((*trigger)->needsToBeSaved())
                {
                    zone->second->removeTrigger((*trigger));
                    if((*trigger)->deleteIfZoneDestroyed())
                    {
                        //ScriptWrapper::getSingleton().owned((*trigger));
                        delete (*trigger);
                        ScriptWrapper::getSingleton().deleted((*trigger));
                    }
                }
            }
        }




        // initialize xmlreader
        reader->initializeXml();

        DOMNodeList* rootNodeList = reader->getDocument()->getDocumentElement()->getElementsByTagName(AutoXMLCh(getXmlNodeIdentifier().c_str()).data());

        if(rootNodeList->getLength())
        {
            DOMNodeList* xmlTriggerFactory = static_cast<DOMElement*>(rootNodeList->item(0))->getElementsByTagName(AutoXMLCh("trigger").data());
            if(xmlTriggerFactory->getLength())
            {
                for(XMLSize_t childIdx1 = 0; childIdx1 < xmlTriggerFactory->getLength(); childIdx1++)
                {
                    DOMNode* xmlTrigger = xmlTriggerFactory->item(childIdx1);
                    if(xmlTrigger->getNodeType() == DOMNode::ELEMENT_NODE)
                    {
                        Ogre::String classname = reader->getAttributeValueAsStdString(static_cast<DOMElement*>(xmlTrigger), "classname");
                        Ogre::String name = reader->getAttributeValueAsStdString(static_cast<DOMElement*>(xmlTrigger), "name");
                        Ogre::String zoneName = reader->getAttributeValueAsStdString(static_cast<DOMElement*>(xmlTrigger), "zone");

                        PropertyRecord properties = reader->getPropertiesAsRecord(static_cast<DOMElement*>(xmlTrigger));


                        Trigger *trigger = createTrigger(classname, name);
                        if( trigger ) // if not, there is an error-msg from the script!
                        {
                            trigger->setProperties(&properties);
                            Zone *zone = ZoneManager::getSingleton().getZone(zoneName);
                            if(zone == NULL)
                            {
                                LOG_ERROR(Logger::SCRIPT, "Tried to load trigger for zone '"+zoneName+"', but the zone could not be found!");
                                delete trigger;
                            }
                            else
                                zone->addTrigger(trigger);
                        }

                    }
                }
            }
        }

        // close xmlreader
        reader->shutdownXml();
    }

    int TriggerFactory::getPriority() const
    {
        return 25; // wants to be loaded after zonemanager
    }
}
