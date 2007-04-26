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
#include "GameObjectNodeProcessor.h"
#include "LightNodeProcessor.h"
#include "PropertyReader.h"
#include "SoundNodeProcessor.h"
#include "World.h"
#include "XmlHelper.h"
#include "XmlResource.h"
#include "XmlResourceManager.h"

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
            
            processNodes(dataDocumentContent, loadGameObjects);
            ///@todo process zones
            ///@todo skies
            
            
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

    void MapLoader::processNodes(DOMElement* dataDocumentContent, bool loadGameObjects)
    {
        DOMElement* nodesElem = XmlHelper::getChildNamed(dataDocumentContent, "nodes");
        DOMNodeList* nodeNodes = nodesElem->getChildNodes();
        for (XMLSize_t idx = 0; idx < nodeNodes->getLength(); idx++)
        {
            DOMNode* cur = nodeNodes->item(idx);
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
