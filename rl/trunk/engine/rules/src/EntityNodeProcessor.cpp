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
#include "EntityNodeProcessor.h"

#include <OgreEntity.h>
#include <OgreMeshManager.h>

#include "CoreSubsystem.h"
#include "World.h"
#include "XmlHelper.h"

using namespace Ogre;
using namespace XERCES_CPP_NAMESPACE;

namespace rl
{
    EntityNodeProcessor::EntityNodeProcessor(const Ogre::String& resourceGroup)
    :   AbstractMapNodeProcessor(),
        mResourceGroup(resourceGroup)
    {
    }

    bool EntityNodeProcessor::processNode(XERCES_CPP_NAMESPACE::DOMElement* nodeElem, bool loadGameObjects)
    {
        if (!XmlHelper::hasNodeName(nodeElem, "entity"))
        {
            return false;
        }
    
        Ogre::String entName = XmlHelper::getAttributeValueAsStdString(nodeElem, "name");

        LOG_DEBUG(Logger::RULES, 
            "Processing entity node " 
                + entName);

        if (!XmlHelper::hasAttribute(nodeElem, "meshFile"))
        {
            LOG_ERROR(Logger::RULES, "Entity node defines no meshfile");
            return false;
        }
        
        Ogre::String meshFile = XmlHelper::getAttributeValueAsStdString(nodeElem, "meshFile");
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

        ///@todo static geometry groups
        SceneNode* parentNode = getRootSceneNode()->createChildSceneNode(entName, position, orientation);

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
            return false;
        }

        parentNode->attachObject(newEnt);

        DOMElement* scaleElem = XmlHelper::getChildNamed(nodeElem, "scale");
        if (scaleElem != NULL)
        {
            parentNode->scale(processVector3(scaleElem));
        }
        
        return true;
    }

    	//void EntityNodeProcessor::createCollision()
	//{
 //       // ------- Falls n�tig automatisch bodyproxy erstellen -------------
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

 //               LOG_MESSAGE(Logger::CORE, " Die Entity '"+entName+"' liegt in einer Ebene, verwende 'box' f�r physical_body '"+physical_body+"' ");
 //               forceBox = true;
 //           }
 //           const Quaternion orientation(0,0,0,0);// = parentNode->getOrientation();
 //           const Ogre::Vector3 pos = aab.getMinimum()* parentNode->getScale() + (size/2.0);


 //           // Pr�fen, ob schon ein identischer Proxy erstellt wurde um diesen erneut zu verwenden
 //           AlreadyUsedCollision &aucol (mAutoCreatedCollisions[meshName]);
 //           if (aucol.Type.compare(physical_body) == 0  &&
 //               aucol.Scale == parentNode->getScale() &&
 //               (!forceBox)) // sicherheitshalber
 //           {
 //               collision = aucol.ColPtr;
 //               LOG_DEBUG(Logger::CORE, " Schon fr�her erstellten physical_body f�r Entity '"+entName+"' wieder verwendet. ");
 //           }
 //           else
 //           {

 //               if (physical_body.compare("box") == 0 || forceBox)
 //               {
 //                   collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Box(
 //                                    thisWorld, size, orientation, pos));
 //                   LOG_DEBUG(Logger::CORE, " physical_body 'box' f�r Entity '"+entName+"' erstellt. ");
 //               }
 //               else if (physical_body.compare("pyramid") == 0)
 //               {
 //                   collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Pyramid(
 //                                   thisWorld, size, orientation, pos));
 //                   LOG_DEBUG(Logger::CORE, " physical_body 'pyramid' f�r Entity '"+entName+"' erstellt. ");
 //               }
 //               else if (physical_body.compare("sphere") == 0)
 //               {
 //                   double radius = std::max(size.x, std::max(size.y, size.z)) / 2.0;
 //                   collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid(
 //                                   thisWorld, Vector3(radius, radius, radius),
 //                                   orientation, pos));
 //                   LOG_DEBUG(Logger::CORE, " physical_body 'sphere' f�r Entity '"+entName+"' erstellt. ");
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
 //                   LOG_DEBUG(Logger::CORE, " physical_body 'ellipsoid' f�r Entity '"+entName+"' erstellt. ");
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
 //                   LOG_DEBUG(Logger::CORE, " physical_body 'capsule' f�r Entity '"+entName+"' erstellt. ");
 //               }
 //               else if (physical_body.compare("convexhull") == 0)
 //               {
 //                   collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(
 //                                   thisWorld,
 //                                   newEnt,
 //                                   false));
 //                   //orientation, pos));
 //                   LOG_DEBUG(Logger::CORE, " physical_body 'convexhull' f�r Entity '"+entName+"' erstellt. ");
 //               }
 //               else if (physical_body.compare("mesh") == 0 || physical_body.compare("auto"))
 //               {
 //                   collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::TreeCollision(
 //                                   thisWorld, newEnt, false));
 //                   LOG_DEBUG(Logger::CORE, " physical_body 'mesh' f�r Entity '"+entName+"' erstellt. ");
 //               }
 //               else
 //                   LOG_MESSAGE(Logger::CORE,
 //                       " Der bodyproxy_type '"+physical_body+"'(aus userData) der Entity '"+meshName+"' ist ung�ltig.");

 //               // proxy in die liste der schon erstellten proxies hinzuf�gen
 //               aucol.ColPtr = collision;
 //               aucol.Scale = parentNode->getScale();
 //               aucol.Type = physical_body;
 //           }
 //       }

 //       // zur liste hinzuf�gen
 //       if (!collision.isNull())
 //       {
 //           mCollisions.push_back(collision);
 //       }


 //       // Zur Physik des Levels hinzuf�gen
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

}
