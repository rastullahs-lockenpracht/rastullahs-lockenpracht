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
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "MapLoader.h"

#include <OgreResourceManager.h>

#include "AbstractMapNodeProcessor.h"
#include "Actor.h"
#include "ActorManager.h"
#include "ContentModule.h"
#include "CoreSubsystem.h"
#include "EntityNodeProcessor.h"
#include "GameObjectConstants.h"
#include "GameObjectNodeProcessor.h"
#include "LightNodeProcessor.h"
#include "PropertyReader.h"
#include "SoundNodeProcessor.h"
#include "World.h"
#include "XmlHelper.h"
#include "XmlResource.h"
#include "XmlResourceManager.h"
#include "Zone.h"
#include "ZoneManager.h"

using namespace Ogre;
using namespace XERCES_CPP_NAMESPACE;
using std::list;

namespace rl {

    using XERCES_CPP_NAMESPACE::DOMDocument; //XXX: for VS 2003/2005
    
    MapLoader::MapLoader(const Ogre::String& resourceGroup)
        : mRootSceneNode(NULL),
          mResourceGroup(resourceGroup)
    {
        mXmlPropertyProcessor = new XmlPropertyReader();

        mNodeProcessors.push_back(new EntityNodeProcessor(resourceGroup));
        mNodeProcessors.push_back(new GameObjectNodeProcessor());
        mNodeProcessors.push_back(new SoundNodeProcessor());
        mNodeProcessors.push_back(new LightNodeProcessor());
    }

    MapLoader::~MapLoader()
    {
        for (list<AbstractMapNodeProcessor*>::const_iterator it = mNodeProcessors.begin(); 
            it != mNodeProcessors.end(); ++it)
        {
            delete *it;
        }
        delete mXmlPropertyProcessor;
    }

    void MapLoader::loadMap(const Ogre::String& mapresource, bool loadGameObjects)
    {
        LOG_MESSAGE(Logger::RULES, "Loading map " + mapresource);
  		XMLPlatformUtils::Initialize();

		XmlHelper::initializeTranscoder();
		XercesDOMParser* parser = new XercesDOMParser();

        parser->setValidationScheme(XercesDOMParser::Val_Auto);    // optional.
        parser->setDoNamespaces(true);    // optional
		
        XmlPtr res = XmlResourceManager::getSingleton().getByName(mapresource);
        if (res.isNull())
        {
            Ogre::String group = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
            if (ResourceGroupManager::getSingleton().resourceExists(
                mResourceGroup, mapresource))
            {
                group = mResourceGroup;
            }
            res = XmlResourceManager::getSingleton().create(mapresource, group);
        }
                
        if (!res.isNull())
        {
            res->parseBy(parser);
    
            setRootSceneNode(CoreSubsystem::getSingleton().getWorld()
                    ->getSceneManager()->getRootSceneNode()->createChildSceneNode(mapresource));

            DOMDocument* doc = parser->getDocument();
            DOMElement* dataDocumentContent = doc->getDocumentElement();
            
            
            ///@todo process map file DOM ;)        
                        
            LOG_MESSAGE(Logger::RULES, "Processing nodes");
            
            processSceneNodes(XmlHelper::getChildNamed(dataDocumentContent, "nodes"), loadGameObjects);
            processZones(XmlHelper::getChildNamed(dataDocumentContent, "zones"));
            processSkySettings(XmlHelper::getChildNamed(dataDocumentContent, "sky"));
            
            LOG_MESSAGE(Logger::RULES, "Map loaded");
    
            doc->release();

			CoreSubsystem::getSingleton().getWorld()->initializeDefaultCamera();
        }
        else
        {
            LOG_ERROR(Logger::RULES, "Map resource '" + mapresource + "' not found");
        }
       
		XMLPlatformUtils::Terminate();
    }

    void MapLoader::processSceneNodes(DOMElement* nodesElem, bool loadGameObjects)
    {
		if (nodesElem == NULL)
		{
			return;
		}

        for (DOMNode* cur = nodesElem->getFirstChild(); cur != NULL; cur = cur->getNextSibling())
        {
            if (cur->getNodeType() == DOMNode::ELEMENT_NODE)
            {
                DOMElement* curElem = static_cast<DOMElement*>(cur);

                list<AbstractMapNodeProcessor*>::iterator it = mNodeProcessors.begin();
                while (it != mNodeProcessors.end() && !(*it)->processNode(curElem, loadGameObjects))
                {
                    ++it;
                }
                
                if (it == mNodeProcessors.end())
                {
                    LOG_WARNING(Logger::RULES,
                        "Node " + XmlHelper::transcodeToString(curElem->getNodeName()) + " could not be processed.");
                }
            }
        }
    }

	void MapLoader::processZones(xercesc_2_7::DOMElement *zonesElem)
	{
		if (zonesElem == NULL)
		{
			return; // no zones
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
	}

	void MapLoader::processSkySettings(XERCES_CPP_NAMESPACE::DOMElement* skyElem)
	{
        if (skyElem == NULL)
		{
			return;
		}

		if (!XmlHelper::hasAttribute(skyElem, "material") 
			|| !XmlHelper::hasAttribute(skyElem, "type"))
		{
			LOG_ERROR(Logger::RULES, "<sky> element must have at least attributes 'type' and 'material'.");
		}
		else
		{
			Ogre::String type = XmlHelper::getAttributeValueAsStdString(skyElem, "type");
			Ogre::String material = XmlHelper::getAttributeValueAsStdString(skyElem, "material");

			bool drawFirst = true;
			if (XmlHelper::hasAttribute(skyElem, "drawfirst"))
			{
				drawFirst = XmlHelper::getAttributeValueAsBool(skyElem, "drawfirst");
			}
			
			Ogre::Real distance = 5000;
			if (XmlHelper::hasAttribute(skyElem, "distance"))
			{
				drawFirst = XmlHelper::getAttributeValueAsBool(skyElem, "distance");
			}				

			if (type == "dome")
			{
				Ogre::Real curvature = 10;
				Ogre::Real tiling = 8;

				DOMElement* domeSettings = XmlHelper::getChildNamed(skyElem, "skydomesettings");
				if (domeSettings != NULL)
				{
					if (XmlHelper::hasAttribute(domeSettings, "curvature"))
					{
						curvature = XmlHelper::getAttributeValueAsReal(domeSettings, "curvature");
					}
					if (XmlHelper::hasAttribute(domeSettings, "tiling"))
					{
						curvature = XmlHelper::getAttributeValueAsReal(domeSettings, "tiling");
					}
				}
				CoreSubsystem::getSingleton().getWorld()->setSkyDome(
					true, material, curvature, tiling, distance, drawFirst);
			}
			else if (type == "box")
			{
				CoreSubsystem::getSingleton().getWorld()->setSkyBox(true, material, distance, drawFirst);
			}
			else if (type == "plane")
			{
				LOG_ERROR(Logger::RULES, "Sky Plane is not implemented yet.");
			}
		}
	}

    void MapLoader::setRootSceneNode(SceneNode* node)
    {
        mRootSceneNode = node;
        for (list<AbstractMapNodeProcessor*>::const_iterator it = mNodeProcessors.begin(); 
            it != mNodeProcessors.end(); ++it)
        {
            (*it)->setRootSceneNode(node);
        }
    }
    
} // namespace rl
