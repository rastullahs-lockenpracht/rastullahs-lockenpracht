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
				&& hasNodeName(cur, "zone"))
            {
				DOMElement* curZoneElem = static_cast<DOMElement*>(cur);
				if (hasAttribute(curZoneElem, "name"))
				{
					Ogre::String name = getAttributeValueAsStdString(curZoneElem, "name");
                    Zone* zone = NULL;
                    if( name == "default" )
                        zone = ZoneManager::getSingleton().getDefaultZone();
                    else
                    {
                        // get the zone with that name or create one
                        zone = ZoneManager::getSingleton().getZone(name);
                        if( !zone )
                            zone = ZoneManager::getSingleton().createZone(name);

                        // multiple areas
                        for(DOMNode* curArea = cur->getFirstChild(); curArea != NULL; curArea = curArea->getNextSibling())
                        {
                            if( curArea->getNodeType() == DOMNode::ELEMENT_NODE
                                && hasNodeName(curArea, "area") )
                            {
                                DOMElement *curAreaElem = static_cast<DOMElement*>(curArea);
                                if( hasAttribute(curAreaElem, "type") )
                                {
                                    // type
                                    Ogre::String type;
                                    type = getAttributeValueAsStdString(curAreaElem, "type");

                                    // add or subtract?
                                    bool subtract = false;
                                    if( hasAttribute(curAreaElem, "subtract") )
                                        subtract = getAttributeValueAsBool(curAreaElem, "subtract");

                                    // position
                                    Vector3 position = Vector3::ZERO;
                                    DOMElement* positionElem = getChildNamed(curAreaElem, "position");
                                    if( positionElem != NULL )
                                        position = getValueAsVector3(positionElem);

                                    //scale, rotation, offset
                                    Vector3 scale = Vector3::UNIT_SCALE;
                                    DOMElement* scaleElem = getChildNamed(curAreaElem, "scale");
                                    if( scaleElem != NULL )
                                        scale = getValueAsVector3(scaleElem);

                                    Vector3 offset = Vector3::ZERO;
                                    DOMElement* offsetElem = getChildNamed(curAreaElem, "offset");
                                    if( offsetElem != NULL )
                                        offset = getValueAsVector3(offsetElem);

                                    Quaternion rotation = Quaternion::IDENTITY;
                                    DOMElement* rotationElem = getChildNamed(curAreaElem, "rotation");
                                    if( rotationElem != NULL )
                                        rotation = getValueAsQuaternion(rotationElem);

                                    //transition distance
                                    Real transitionDistance = 0;
                                    DOMElement* transitionElem = getChildNamed(curAreaElem, "transition_distance");
                                    if( transitionElem != NULL )
                                        transitionDistance = getValueAsReal(transitionElem);

					            
                                    if (type == "mesh")
    					            {
                                        Ogre::String meshName;
                                        if( hasAttribute(curAreaElem, "meshfile") )
                                        {
                                            meshName = getAttributeValueAsStdString(curAreaElem, "meshfile");
                                            if( subtract )
                                                ZoneManager::getSingleton().subtractMeshAreaFromZone(name,
                                                    meshName, GT_CONVEXHULL, position, scale, offset, rotation, transitionDistance, QUERYFLAG_PLAYER);
                                            else
                                                ZoneManager::getSingleton().addMeshAreaToZone(name,
                                                    meshName, GT_CONVEXHULL, position, scale, offset, rotation, transitionDistance, QUERYFLAG_PLAYER);
                                        }
                                        else
                                            LOG_ERROR(Logger::SCRIPT, "an <area> element with type=\"mesh\" must have attribute 'meshfile'");
                                    }
    					            else
                                    {
                                        GeometryType geom = GT_NONE;
                                        if (type == "sphere" )
                                            geom = GT_SPHERE;
                                        else if (type == "box" )
                                            geom = GT_BOX;
                                        else if (type == "ellipsoid" )
                                            geom = GT_ELLIPSOID;
                                        else if (type == "pyramid" )
                                            geom = GT_PYRAMID;
                                        else if (type == "capsule" )
                                            geom = GT_CAPSULE;
                                        else
                                        {
                                            LOG_ERROR(Logger::SCRIPT, "Unknown area type '" + type + "' !");
                                        }
                                        if ( geom != GT_NONE )
                                        {
                                            Ogre::AxisAlignedBox aabb;
                                            aabb.setMinimum( - scale / 2.0f);
                                            aabb.setMaximum( + scale / 2.0f);

                                            if( subtract )
                                                ZoneManager::getSingleton().subtractAreaFromZone(name,
                                                    aabb, geom, position, offset, rotation, transitionDistance, QUERYFLAG_PLAYER);
                                            else
                                                ZoneManager::getSingleton().addAreaToZone(name,
                                                    aabb, geom, position, offset, rotation, transitionDistance, QUERYFLAG_PLAYER);
                                        }
                                    }
                                }
                                else
                                {
                                    LOG_ERROR(Logger::SCRIPT, "<area> elemt must have attribute 'type'");
                                }
                            }
                        }
                    }

					if (zone != NULL)
					{
						for (DOMNode* cur = curZoneElem->getFirstChild(); cur != NULL; cur = cur->getNextSibling())
						{
							if (cur->getNodeType() == DOMNode::ELEMENT_NODE)
							{
								DOMElement* curElem = static_cast<DOMElement*>(cur);
								if (hasNodeName(curElem, "light"))
								{
									Ogre::String name = getAttributeValueAsStdString(curElem, "name");
									zone->addLight(ActorManager::getSingleton().getActor(name));
								}
								else if (hasNodeName(curElem, "sound"))
								{
									Ogre::String name = getAttributeValueAsStdString(curElem, "name");
									zone->addSound(name);
								}
								else if (hasNodeName(curElem, "trigger"))
								{
									Ogre::String classname =
										getAttributeValueAsStdString(curElem, "classname");

									Ogre::String name =
										getAttributeValueAsStdString(curElem, "name");

									Trigger* trigger = ScriptSubsystem::getSingleton().getTriggerFactory()
										->createTrigger(classname, name);

                                    // add trigger properties
                                    for( DOMNode* curProperty = cur->getFirstChild(); curProperty != NULL; curProperty = curProperty->getNextSibling() )
                                    {
                                        if( hasNodeName(curProperty, "property") )
                                        {
                                            PropertyEntry propEntry = processProperty(static_cast<DOMElement*>(curProperty));
                                            if (propEntry.first != "")
                                            {
                                                trigger->setProperty(propEntry.first, propEntry.second);
                                            }
                                        }
                                    }

                                    zone->addTrigger(trigger);
								}
                                else if (hasNodeName(curElem, "eaxpreset"))
                                {
									Ogre::String name = getAttributeValueAsStdString(curElem, "name");
                                    zone->setEaxPreset(name);
                                }
							}
						}
					}
					else
					{
						LOG_ERROR(Logger::SCRIPT, "Zone named '"+name+"' could not be processes.");
					}
				}
				else
				{
					LOG_ERROR(Logger::SCRIPT, "<zone> element must have attribute 'name'.");
				}
			}
		}

        ZoneManager::getSingleton().update();
		return true;
	}

}
