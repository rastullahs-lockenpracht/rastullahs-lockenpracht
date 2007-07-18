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

#include "ZoneProcessor.h"

#include "ActorManager.h"
#include "RulesConstants.h"
#include "ScriptSubsystem.h"
#include "Trigger.h"
#include "TriggerFactory.h"
#include "XmlHelper.h"
#include "Zone.h"
#include "ZoneManager.h"

using namespace XERCES_CPP_NAMESPACE;
using namespace Ogre;

namespace rl
{

	bool ZoneProcessor::processNode(DOMElement* zonesElem, bool loadGameObjects)
	{
		if (zonesElem == NULL)
		{
			return false; // no zones
		}

        for (DOMNode* cur = zonesElem->getFirstChild(); cur != NULL; cur = cur->getNextSibling())
        {
            if (cur->getNodeType() == DOMNode::ELEMENT_NODE
				&& XmlHelper::hasNodeName(cur, "zone"))
            {
				DOMElement* curZoneElem = static_cast<DOMElement*>(cur);
				if (XmlHelper::hasAttribute(curZoneElem, "type"))
				{
					Ogre::String type = XmlHelper::getAttributeValueAsStdString(curZoneElem, "type");
					Zone* zone = NULL;
					if (type == "default")
					{
						zone = ZoneManager::getSingleton().getDefaultZone();
					}
					else if (type == "mesh")
					{
						///@todo: zone = ZoneManager::getSingleton().createZone(...);
					}
					else if (type == "sphere")
					{
						Vector3 center = Vector3::ZERO;
						DOMElement* centerElem = XmlHelper::getChildNamed(curZoneElem, "center");
						if (centerElem != NULL)
						{
							center = XmlHelper::getValueAsVector3(centerElem);
						}

						Real radius = 1;
						DOMElement* radiusElem = XmlHelper::getChildNamed(curZoneElem, "radius");
						if (radiusElem != NULL)
						{
							radius = XmlHelper::getAttributeValueAsReal(radiusElem, "r");
						}

						Ogre::String name = XmlHelper::getAttributeValueAsStdString(curZoneElem, "name");

						zone = ZoneManager::getSingleton().createZone(
							name, center, radius, QUERYFLAG_PLAYER);
					}

					if (zone != NULL)
					{
						for (DOMNode* cur = curZoneElem->getFirstChild(); cur != NULL; cur = cur->getNextSibling())
						{
							if (cur->getNodeType() == DOMNode::ELEMENT_NODE)
							{
								DOMElement* curElem = static_cast<DOMElement*>(cur);
								if (XmlHelper::hasNodeName(curElem, "light"))
								{
									Ogre::String name = XmlHelper::getAttributeValueAsStdString(curElem, "name");
									zone->addLight(ActorManager::getSingleton().getActor(name));
								}
								else if (XmlHelper::hasNodeName(curElem, "sound"))
								{
									Ogre::String name = XmlHelper::getAttributeValueAsStdString(curElem, "name");
									zone->addSound(name);
								}
								else if (XmlHelper::hasNodeName(curElem, "trigger"))
								{
									Ogre::String classname =
										XmlHelper::getAttributeValueAsStdString(curElem, "classname");

									Trigger* trigger = ScriptSubsystem::getSingleton().getTriggerFactory()
										->createTrigger(classname);

									///@todo trigger properties
								}
							}
						}
					}
					else
					{
						LOG_ERROR(Logger::RULES, "Zone of type '"+type+"' could not be processes.");
					}
				}
				else
				{
					LOG_ERROR(Logger::RULES, "<zone> element must have attribute 'type'.");
				}
			}
		}

		return true;
	}

}
