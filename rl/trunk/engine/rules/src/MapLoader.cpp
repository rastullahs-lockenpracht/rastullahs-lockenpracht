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
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "MapLoader.h"

#include <OgreEntity.h>
#include <OgreMeshManager.h>
#include <OgreResourceManager.h>

#include "Actor.h"
#include "ActorManager.h"
#include "ContentModule.h"
#include "CoreSubsystem.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "GameObjectProxy.h"
#include "PropertyReader.h"
#include "World.h"
#include "XmlHelper.h"
#include "XmlResource.h"
#include "XmlResourceManager.h"

using namespace Ogre;
using namespace XERCES_CPP_NAMESPACE;

namespace rl {

    using XERCES_CPP_NAMESPACE::DOMDocument; //XXX: for VS 2003/2005
    
    MapLoader::MapLoader()
        : mSceneNode(NULL)
    {
    }

    void MapLoader::loadMap(const Ogre::String& mapresource, const Ogre::String& resourceGroup, bool loadGameObjects)
    {
        LOG_MESSAGE(Logger::RULES, "Loading map " + mapresource + " in group " + resourceGroup);
        mResourceGroup = resourceGroup;
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
                CoreSubsystem::getSingleton().getActiveAdventureModule()->getId(), mapresource))
            {
                group = CoreSubsystem::getSingleton().getActiveAdventureModule()->getId();
            }
            res = XmlResourceManager::getSingleton().create(mapresource, group);
        }
                
        if (!res.isNull())
        {
            res->parseBy(parser);
    
            mSceneNode = 
                CoreSubsystem::getSingleton().getWorld()
                    ->getSceneManager()->getRootSceneNode()->createChildSceneNode(mapresource);

            DOMDocument* doc = parser->getDocument();
            DOMElement* dataDocumentContent = doc->getDocumentElement();
            
            mXmlPropertyProcessor = new XmlPropertyReader();
            
            ///@todo process map file DOM ;)        
                        
            LOG_MESSAGE(Logger::RULES, "Processing nodes");
            
            mStatistics[ENTITIES] = 0;
            mStatistics[GAMEOBJECTS] = 0;
            mStatistics[SOUNDS] = 0;
            mStatistics[LIGHTS] = 0;
                    
            DOMElement* nodesElem = XmlHelper::getChildNamed(dataDocumentContent, "nodes");
            DOMNodeList* nodeNodes = nodesElem->getChildNodes();
            for (XMLSize_t idx = 0; idx < nodeNodes->getLength(); idx++)
            {
                DOMNode* cur = nodeNodes->item(idx);
                if (cur->getNodeType() == DOMNode::ELEMENT_NODE
                    && XmlHelper::hasNodeName(cur, "node"))
                {
                    processNode(static_cast<DOMElement*>(cur), loadGameObjects);
                }
            }
            LOG_MESSAGE(Logger::RULES, 
                "Found " 
                + StringConverter::toString(mStatistics[ENTITIES]) + " entities, "
                + StringConverter::toString(mStatistics[SOUNDS]) + " sounds, "
                + StringConverter::toString(mStatistics[LIGHTS]) + " lights, "
                + StringConverter::toString(mStatistics[GAMEOBJECTS]) + " gameobjects.");
            
            LOG_MESSAGE(Logger::RULES, "Map loaded");
    
            delete mXmlPropertyProcessor;
    
            doc->release();
        }
        else
        {
            LOG_ERROR(Logger::RULES, "Map resource '" + mapresource + "' not found");
        }
       
		XMLPlatformUtils::Terminate();
    }

    void MapLoader::processNode(DOMElement* nodeElem, bool loadGameObjects)
    {
        DOMNodeList* childNodes = nodeElem->getChildNodes();
        for (XMLSize_t idx = 0; idx < childNodes->getLength(); idx++)
        {
            DOMNode* cur = childNodes->item(idx);
            if (cur->getNodeType() == DOMNode::ELEMENT_NODE)
            {
                if (XmlHelper::hasNodeName(cur, "entity"))
                {
                    createEntity(nodeElem, static_cast<DOMElement*>(cur));
                    mStatistics[ENTITIES]++;
                    break;
                }
                else if (loadGameObjects && XmlHelper::hasNodeName(cur, "gameobject"))
                {
                    createGameObject(nodeElem, static_cast<DOMElement*>(cur));
                    mStatistics[GAMEOBJECTS]++;
                    break;
                }
                else if (XmlHelper::hasNodeName(cur, "sound"))
                {
                    createSound(nodeElem, static_cast<DOMElement*>(cur));
                    mStatistics[SOUNDS]++;
                    break;
                }
                else if (XmlHelper::hasNodeName(cur, "light"))
                {
                    createLight(nodeElem, static_cast<DOMElement*>(cur));
                    mStatistics[LIGHTS]++;
                    break;
                }
            }
        }
    }

    void MapLoader::createGameObject(DOMElement* nodeElem, DOMElement* gameobjElem) const
    {
        LOG_DEBUG(Logger::RULES, 
            "Processing game object node " 
                + XmlHelper::getAttributeValueAsStdString(nodeElem, "name"));
        
        Ogre::String classname = XmlHelper::getAttributeValueAsStdString(gameobjElem, "class");
        
        unsigned int goid = GameObjectProxy::NO_OBJECT_ID;

        if (XmlHelper::hasAttribute(gameobjElem, "id"))
        {
            goid = XmlHelper::getAttributeValueAsInteger(gameobjElem, "id");
        }

        GameObjectProxy gop = GameObjectManager::getSingleton().createGameObjectProxy(classname, goid);

        DOMElement* posElem = XmlHelper::getChildNamed(nodeElem, "position");
        if (posElem != NULL)
        {
            Vector3 pos = processVector3(posElem);
            gop.getGameObject()->setPosition(pos);
        }

        DOMElement* oriElem = XmlHelper::getChildNamed(nodeElem, "rotation");
        if (oriElem != NULL)
        {
            Quaternion ori = processQuaternion(oriElem);
            gop.getGameObject()->setOrientation(ori);
        }

        DOMNodeList* goElChildNodes = gameobjElem->getChildNodes();
        for (XMLSize_t idx = 0; idx < goElChildNodes->getLength(); idx++)
        {
            DOMNode* cur = goElChildNodes->item(idx);
            if (cur->getNodeType() == DOMNode::ENTITY_NODE 
                && XmlHelper::hasNodeName(cur, "property"))
            {
                PropertyEntry propEntry = mXmlPropertyProcessor->processProperty(static_cast<DOMElement*>(cur));
                if (propEntry.first != "")
                {
                    gop.getGameObject()->setProperty(propEntry.first, propEntry.second);
                }
            }
        }
    }

    void MapLoader::createEntity(DOMElement* nodeElem, DOMElement* entityElem)
    {
        Ogre::String entName = XmlHelper::getAttributeValueAsStdString(entityElem, "name");

        LOG_DEBUG(Logger::RULES, 
            "Processing entity node " 
                + entName);

        if (!XmlHelper::hasAttribute(entityElem, "meshFile"))
        {
            LOG_ERROR(Logger::RULES, "Entity node defines no meshfile");
            return;
        }
        
        Ogre::String meshFile = XmlHelper::getAttributeValueAsStdString(entityElem, "meshFile");
        if (entName == "")
        {
            entName = getRandomName(meshFile);
        }

        //Actor* act = ActorManager::getSingleton().createMeshActor(
        //    XmlHelper::getAttributeValueAsStdString(entityElem, "name"),
        //    meshFile); ///@todo: Collision proxy

        Vector3 position(Vector3::ZERO);
        Quaternion orientation(Quaternion::IDENTITY);

        DOMElement* posElem = XmlHelper::getChildNamed(nodeElem, "position");
        if (posElem != NULL)
        {
            position = processVector3(posElem);
        }
        else 
        {
            LOG_WARNING(Logger::RULES, "No position given for entity, used (0,0,0)");
        }

        DOMElement* oriElem = XmlHelper::getChildNamed(nodeElem, "rotation");
        if (oriElem != NULL)
        {
            orientation = processQuaternion(oriElem);
        }
        else 
        {
            LOG_WARNING(Logger::RULES, "No orientation given for entity, used Identity");
        }

        SceneNode* parentNode = mSceneNode->createChildSceneNode(entName, position, orientation);

        ResourceGroupManager& resGroupMgr = ResourceGroupManager::getSingleton();
        while(parentNode->getCreator()->hasEntity(entName))
        {
            entName = getRandomName(entName);
        }

        Entity* newEnt = NULL;
        try
        {
            // if this mesh exists in our module's resource group: preload it
            if (resGroupMgr.resourceExists(mResourceGroup, meshFile))
            {
                MeshManager::getSingleton().load(meshFile, mResourceGroup);
            }

            // If not, it is now loaded implicitly from the default group
            newEnt = CoreSubsystem::getSingleton().getWorld()
                        ->getSceneManager()->createEntity(entName, meshFile);
        }
        catch(...)
        {
              LOG_ERROR(Logger::CORE, " Entity '"+meshFile+"' mit dem Namen '"+entName+"' konnte nicht geladen werden.");
              return;
        }

        parentNode->attachObject(newEnt);

        DOMElement* scaleElem = XmlHelper::getChildNamed(nodeElem, "scale");
        if (scaleElem != NULL)
        {
            parentNode->scale(processVector3(scaleElem));
        }
    }

    void MapLoader::createSound(DOMElement* nodeElem, DOMElement* gameobjElem) const
    {
        LOG_DEBUG(Logger::RULES, 
            "Processing sound node " 
                + XmlHelper::getAttributeValueAsStdString(nodeElem, "name"));
        
        ///@todo create sound
    }

    void MapLoader::createLight(DOMElement* nodeElem, DOMElement* gameobjElem) const
    {
        LOG_DEBUG(Logger::RULES, 
            "Processing light node " 
                + XmlHelper::getAttributeValueAsStdString(nodeElem, "name"));
        
        ///@todo create light
    }

    Vector3 MapLoader::processVector3(DOMElement* vec3Elem) const
    {
        Vector3 rval(Vector3::ZERO);
        if (vec3Elem != NULL)
        {
            if (XmlHelper::hasAttribute(vec3Elem, "x"))
            {
                rval.x = XmlHelper::getAttributeValueAsReal(vec3Elem, "x");
            }
            if (XmlHelper::hasAttribute(vec3Elem, "y"))
            {
                rval.y = XmlHelper::getAttributeValueAsReal(vec3Elem, "y");
            }
            if (XmlHelper::hasAttribute(vec3Elem, "z"))
            {
                rval.z = XmlHelper::getAttributeValueAsReal(vec3Elem, "z");
            }
        }

        return rval;
    }

    Quaternion MapLoader::processQuaternion(DOMElement* quatElem) const
    {
        Quaternion rval(Quaternion::IDENTITY);
        if (quatElem != NULL)
        {
            if (XmlHelper::hasAttribute(quatElem, "qw"))
            {
                rval.w = XmlHelper::getAttributeValueAsReal(quatElem, "qw");
            }
            if (XmlHelper::hasAttribute(quatElem, "qx"))
            {
                rval.x = XmlHelper::getAttributeValueAsReal(quatElem, "qx");
            }
            if (XmlHelper::hasAttribute(quatElem, "qy"))
            {
                rval.y = XmlHelper::getAttributeValueAsReal(quatElem, "qy");
            }
            if (XmlHelper::hasAttribute(quatElem, "qz"))
            {
                rval.z = XmlHelper::getAttributeValueAsReal(quatElem, "qz");
            }
        }

        return rval;
    }

    Ogre::String MapLoader::getRandomName(const Ogre::String& baseName) const
    {
//        int rnd = rand();
        static int ival = 0;
        std::stringstream rval;
        rval << baseName << "_" << ++ival;
        return rval.str();
    }

    
} // namespace rl
