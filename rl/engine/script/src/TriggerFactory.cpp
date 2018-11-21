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

#include "TriggerFactory.h"

#include "SaveGameManager.h"
#include "ScriptWrapper.h"
#include "Trigger.h"
#include "Zone.h"
#include "ZoneManager.h"

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

    void TriggerFactory::writeData(SaveGameFileWriter* writer)
    {
        tinyxml2::XMLElement* triggerParentNode
            = writer->appendChildElement(writer->getDocument()->RootElement(), getXmlNodeIdentifier().c_str());

        const ZoneManager::ZoneMap& zoneMap(ZoneManager::getSingleton().getAllZones());
        ZoneManager::ZoneMap::const_iterator zone;

        // look in all zones if there is a trigger
        for (zone = zoneMap.begin(); zone != zoneMap.end(); zone++)
        {
            std::list<Trigger*> allTriggers(zone->second->getTriggers());
            // search for triggers in a zone
            for (std::list<Trigger*>::iterator trigger = allTriggers.begin(); trigger != allTriggers.end(); trigger++)
            {
                if ((*trigger)->needsToBeSaved())
                {
                    tinyxml2::XMLElement* triggerNode = writer->appendChildElement(triggerParentNode, "trigger");
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
        const ZoneManager::ZoneMap& zoneMap(ZoneManager::getSingleton().getAllZones());
        ZoneManager::ZoneMap::const_iterator zone;
        // look in all zones if there is a trigger
        for (zone = zoneMap.begin(); zone != zoneMap.end(); zone++)
        {
            std::list<Trigger*> allTriggers(zone->second->getTriggers());
            // search for triggers in a zone
            for (std::list<Trigger*>::iterator trigger = allTriggers.begin(); trigger != allTriggers.end(); trigger++)
            {
                if ((*trigger)->needsToBeSaved())
                {
                    zone->second->removeTrigger((*trigger));
                    if ((*trigger)->deleteIfZoneDestroyed())
                    {
                        // ScriptWrapper::getSingleton().owned((*trigger));
                        delete (*trigger);
                        ScriptWrapper::getSingleton().deleted((*trigger));
                    }
                }
            }
        }

        XmlElementList rootNodeList
            = reader->getElementsByTagName(reader->getDocument(), getXmlNodeIdentifier().c_str());

        if (!rootNodeList.empty())
        {
            XmlElementList xmlTriggerFactory = reader->getElementsByTagName(rootNodeList[0], "trigger");
            for (XmlElementList::iterator it = xmlTriggerFactory.begin(); it != xmlTriggerFactory.end(); ++it)
            {
                const tinyxml2::XMLNode* xmlTrigger = *it;
                const tinyxml2::XMLElement* xmlTriggerElem = xmlTrigger->ToElement();
                if (xmlTriggerElem)
                {
                    Ogre::String classname = reader->getAttributeValueAsStdString(xmlTriggerElem, "classname");
                    Ogre::String name = reader->getAttributeValueAsStdString(xmlTriggerElem, "name");
                    Ogre::String zoneName = reader->getAttributeValueAsStdString(xmlTriggerElem, "zone");

                    PropertyRecordPtr properties = reader->getPropertiesAsRecord(xmlTriggerElem);

                    Trigger* trigger = createTrigger(classname, name);
                    if (trigger) // if not, there is an error-msg from the script!
                    {
                        trigger->setProperties(properties);
                        Zone* zone = ZoneManager::getSingleton().getZone(zoneName);
                        if (zone == NULL)
                        {
                            LOG_ERROR(Logger::SCRIPT,
                                "Tried to load trigger for zone '" + zoneName + "', but the zone could not be found!");
                            delete trigger;
                        }
                        else
                        {
                            zone->addTrigger(trigger);
                        }
                    }
                }
            }
        }
    }

    int TriggerFactory::getPriority() const
    {
        return 25; // wants to be loaded after zonemanager
    }
}
