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

			CoreSubsystem::getSingleton().getWorld()->initializeDefaultCamera();
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

		GameObjectState state = GOS_LOADED;
		if (XmlHelper::hasAttribute(gameobjElem, "state"))
		{
			Ogre::String stateStr = XmlHelper::getAttributeValueAsStdString(gameobjElem, "state");
			if (stateStr == "LOADED")
			{
				state = GOS_LOADED;
			}
			else if (stateStr == "HELD")
			{
				state = GOS_HELD;
			}
			else if (stateStr == "IN_POSSESSION")
			{
				state = GOS_IN_POSSESSION;
			}
			else if (stateStr == "IN_SCENE")
			{
				state = GOS_IN_SCENE;
			}
			else if (stateStr == "READY")
			{
				state = GOS_READY;
			}
		}
		gop.getGameObject()->setState(state);
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
	        LOG_ERROR(Logger::RULES, " Entity '"+meshFile+"' mit dem Namen '"+entName+"' konnte nicht geladen werden.");
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

	//void MapLoader::createCollision()
	//{
 //       // ------- Falls nötig automatisch bodyproxy erstellen -------------
 //       // (wenn physical_body gesetzt wurde)
 //       OgreNewt::CollisionPtr collision = OgreNewt::CollisionPtr();
 //       OgreNewt::World *thisWorld = PhysicsManager::getSingleton()._getNewtonWorld();

 //       if (physical_body.compare("none") != 0)
 //       {
 //           const AxisAlignedBox &aab = newEnt->getMesh()->getBounds();
 //           Ogre::Vector3 size = (aab.getMaximum() - aab.getMinimum()) * parentNode->getScale();
 //           bool forceBox = false;

 //           if ((size.x < PhysicsManager::NEWTON_GRID_WIDTH ||
 //                size.y < PhysicsManager::NEWTON_GRID_WIDTH ||
 //                size.z < PhysicsManager::NEWTON_GRID_WIDTH) &&
 //                physical_body.compare("convexhull") == 0)
 //           {
 //               if (size.x < PhysicsManager::NEWTON_GRID_WIDTH)
 //                   size.x = PhysicsManager::NEWTON_GRID_WIDTH;
 //               if (size.y < PhysicsManager::NEWTON_GRID_WIDTH)
 //                   size.y = PhysicsManager::NEWTON_GRID_WIDTH;
 //               if (size.z < PhysicsManager::NEWTON_GRID_WIDTH)
 //                   size.z = PhysicsManager::NEWTON_GRID_WIDTH;

 //               LOG_MESSAGE(Logger::CORE, " Die Entity '"+entName+"' liegt in einer Ebene, verwende 'box' für physical_body '"+physical_body+"' ");
 //               forceBox = true;
 //           }
 //           const Quaternion orientation(0,0,0,0);// = parentNode->getOrientation();
 //           const Ogre::Vector3 pos = aab.getMinimum()* parentNode->getScale() + (size/2.0);


 //           // Prüfen, ob schon ein identischer Proxy erstellt wurde um diesen erneut zu verwenden
 //           AlreadyUsedCollision &aucol (mAutoCreatedCollisions[meshName]);
 //           if (aucol.Type.compare(physical_body) == 0  &&
 //               aucol.Scale == parentNode->getScale() &&
 //               (!forceBox)) // sicherheitshalber
 //           {
 //               collision = aucol.ColPtr;
 //               LOG_DEBUG(Logger::CORE, " Schon früher erstellten physical_body für Entity '"+entName+"' wieder verwendet. ");
 //           }
 //           else
 //           {

 //               if (physical_body.compare("box") == 0 || forceBox)
 //               {
 //                   collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Box(
 //                                    thisWorld, size, orientation, pos));
 //                   LOG_DEBUG(Logger::CORE, " physical_body 'box' für Entity '"+entName+"' erstellt. ");
 //               }
 //               else if (physical_body.compare("pyramid") == 0)
 //               {
 //                   collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Pyramid(
 //                                   thisWorld, size, orientation, pos));
 //                   LOG_DEBUG(Logger::CORE, " physical_body 'pyramid' für Entity '"+entName+"' erstellt. ");
 //               }
 //               else if (physical_body.compare("sphere") == 0)
 //               {
 //                   double radius = std::max(size.x, std::max(size.y, size.z)) / 2.0;
 //                   collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid(
 //                                   thisWorld, Vector3(radius, radius, radius),
 //                                   orientation, pos));
 //                   LOG_DEBUG(Logger::CORE, " physical_body 'sphere' für Entity '"+entName+"' erstellt. ");
 //               }
 //               else if (physical_body.compare("ellipsoid") == 0)
 //               {
 //                   // set the size x/z values to the maximum
 //                   Ogre::Vector3 s(size/2.0);
 //                   s.x = std::max(s.x, s.z);
 //                   s.z = s.x;
 //                   collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid(
 //                                   thisWorld, s,
 //                                   orientation, pos));
 //                   LOG_DEBUG(Logger::CORE, " physical_body 'ellipsoid' für Entity '"+entName+"' erstellt. ");
 //               }
 //               else if (physical_body.compare("capsule") == 0)
 //               {
 //                   double radius = std::max(size.x, size.z) / 2.0;
 //                   double height = size.y;

 //                   collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Capsule(
 //                                   thisWorld,
 //                                   radius,
 //                                   height,
 //                                   orientation, pos));
 //                   LOG_DEBUG(Logger::CORE, " physical_body 'capsule' für Entity '"+entName+"' erstellt. ");
 //               }
 //               else if (physical_body.compare("convexhull") == 0)
 //               {
 //                   collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(
 //                                   thisWorld,
 //                                   newEnt,
 //                                   false));
 //                   //orientation, pos));
 //                   LOG_DEBUG(Logger::CORE, " physical_body 'convexhull' für Entity '"+entName+"' erstellt. ");
 //               }
 //               else if (physical_body.compare("mesh") == 0 || physical_body.compare("auto"))
 //               {
 //                   collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::TreeCollision(
 //                                   thisWorld, newEnt, false));
 //                   LOG_DEBUG(Logger::CORE, " physical_body 'mesh' für Entity '"+entName+"' erstellt. ");
 //               }
 //               else
 //                   LOG_MESSAGE(Logger::CORE,
 //                       " Der bodyproxy_type '"+physical_body+"'(aus userData) der Entity '"+meshName+"' ist ungültig.");

 //               // proxy in die liste der schon erstellten proxies hinzufügen
 //               aucol.ColPtr = collision;
 //               aucol.Scale = parentNode->getScale();
 //               aucol.Type = physical_body;
 //           }
 //       }

 //       // zur liste hinzufügen
 //       if (!collision.isNull())
 //       {
 //           mCollisions.push_back(collision);
 //       }


 //       // Zur Physik des Levels hinzufügen
 //       if (mCollisions.size() > 0)
 //       {
 //           PhysicsManager::getSingleton().addLevelGeometry(newEnt, mCollisions);
 //           LOG_DEBUG(Logger::CORE, " Entity '"+entName+"' in levelGeometry geladen");
 //       }


 //       // wieder aus der liste entfernen, falls mehrere entities hier definiert werden
 //       if (!collision.isNull())
 //       {
 //           mCollisions.pop_back();
 //       }
	//}

    Ogre::String MapLoader::getRandomName(const Ogre::String& baseName) const
    {
//        int rnd = rand();
        static int ival = 0;
        std::stringstream rval;
        rval << baseName << "_" << ++ival;
        return rval.str();
    }

    
} // namespace rl
