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

#include "EntityNodeProcessor.h"

#include "CoreSubsystem.h"
#include "PhysicsManager.h"
#include "World.h"

using namespace Ogre;

namespace rl
{
    EntityNodeProcessor::EntityNodeProcessor(const Ogre::String& resourceGroup)
        : AbstractMapNodeProcessor()
        , mResourceGroup(resourceGroup)
    {
    }

    bool EntityNodeProcessor::processNode(
        const tinyxml2::XMLElement* nodeElem, const Ogre::String& resourceGroup, bool loadGameObjects)
    {
        if (!hasNodeName(nodeElem, "entity"))
        {
            return false;
        }

        Ogre::String entName = getAttributeValueAsStdString(nodeElem, "name");

        LOG_DEBUG(Logger::SCRIPT, "Processing entity node " + entName);

        if (!hasAttribute(nodeElem, "meshfile"))
        {
            LOG_ERROR(Logger::SCRIPT, "Entity node defines no meshfile attribute");
            return false;
        }

        Ogre::String meshFile = getAttributeValueAsStdString(nodeElem, "meshfile");
        if (entName == "")
        {
            entName = getRandomName(meshFile);
        }

        // Actor* act = ActorManager::getSingleton().createMeshActor(
        //    getAttributeValueAsStdString(entityElem, "name"),
        //    meshFile); ///@todo: Collision proxy

        Vector3 position(Vector3::ZERO);
        Quaternion orientation(Quaternion::IDENTITY);

        const tinyxml2::XMLElement* posElem = getChildNamed(nodeElem, "position");
        if (posElem != NULL)
        {
            position = processVector3(posElem);
        }
        else
        {
            LOG_WARNING(Logger::SCRIPT, "No position given for entity, used (0,0,0)");
        }

        const tinyxml2::XMLElement* oriElem = getChildNamed(nodeElem, "rotation");
        if (oriElem != NULL)
        {
            orientation = processQuaternion(oriElem);
        }
        else
        {
            LOG_WARNING(Logger::SCRIPT, "No orientation given for entity, used Identity");
        }

        ///@todo static geometry groups
        SceneNode* parentNode = getRootSceneNode()->createChildSceneNode(entName, position, orientation);

        ResourceGroupManager& resGroupMgr = ResourceGroupManager::getSingleton();
        while (parentNode->getCreator()->hasEntity(entName))
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
            newEnt = CoreSubsystem::getSingleton().getWorld()->getSceneManager()->createEntity(entName, meshFile);

            LOG_DEBUG(Logger::SCRIPT, " Loaded meshfile " + meshFile);
        }
        catch (...)
        {
            LOG_ERROR(Logger::SCRIPT,
                " Entity '" + meshFile + "' mit dem Namen '" + entName + "' konnte nicht geladen werden.");
            return false;
        }

        parentNode->attachObject(newEnt);

        const tinyxml2::XMLElement* scaleElem = getChildNamed(nodeElem, "scale");
        if (scaleElem != NULL)
        {
            parentNode->scale(processVector3(scaleElem));
        }

        // in order for the scale to work correctly the collision needs to be created after the scale was applied
        createCollision(newEnt, meshFile, getChildNamed(nodeElem, "physicsproxy"));

        XmlElementList list = getElementsByTagName(nodeElem, "animation");
        for (size_t idx = 0; idx < list.size(); idx++)
        {
            const tinyxml2::XMLElement* cur = list[idx];
            processAnimation(newEnt, cur);
        }

        return true;
    }

    void EntityNodeProcessor::createCollision(
        Entity* entity, Ogre::String meshName, const tinyxml2::XMLElement* physicsProxyElem)
    {
        Ogre::String physicsProxyTypeAsString;
        if (physicsProxyElem == NULL || !hasAttribute(physicsProxyElem, "type"))
        {
            physicsProxyTypeAsString = "auto";
        }
        else
        {
            physicsProxyTypeAsString = getAttributeValueAsStdString(physicsProxyElem, "type");
        }

        if (physicsProxyTypeAsString == "custom")
        {
            ///@todo create physics proxy from custom collision primitives which are defined in children elements of
            ///<code>physicsProxyElem<code>
        }
        else if (physicsProxyTypeAsString == "none")
        {
            LOG_DEBUG(Logger::SCRIPT, "No physics proxy for entity '" + entity->getName() + "'.");
        }
        else
        {
            GeometryType physicsProxyType
                = PhysicsManager::getSingleton().convertStringToGeometryType(physicsProxyTypeAsString);
            if (physicsProxyType == GT_NONE)
            {
                // auto -> mesh
                if (physicsProxyTypeAsString == "auto")
                    physicsProxyType = GT_MESH;

                if (physicsProxyTypeAsString == "custom")
                {
                    ///@todo create physics proxy from custom collision primitives which are defined in children
                    ///elements of <code>physicsProxyElem<code>
                    LOG_WARNING(
                        Logger::SCRIPT, "Physics proxy type 'custom' is not yet implemented, using mesh instead");
                    physicsProxyType = GT_MESH;
                }
            }

            if (physicsProxyType != GT_NONE)
            {
                OgreNewt::CollisionPtr collision
                    = PhysicsManager::getSingleton().createCollision(entity, physicsProxyType);
                if (collision)
                {
                    LOG_DEBUG(Logger::SCRIPT,
                        "Created physics proxy type '" + physicsProxyTypeAsString + "' for entity '" + entity->getName()
                            + "'.");
                    std::vector<OgreNewt::CollisionPtr> collisionVector;
                    collisionVector.push_back(collision);
                    PhysicsManager::getSingleton().addLevelGeometry(entity, collisionVector);
                }
            }
            else
            {
                LOG_ERROR(Logger::SCRIPT,
                    "Physics proxy type '" + physicsProxyTypeAsString + "' of entity '" + entity->getName()
                        + "' is unknown.");
                return;
            }
        }
        /*
                std::vector<OgreNewt::CollisionPtr> collisions;

                Ogre::String physicsProxyType;
                if (physicsProxyElem == NULL || !hasAttribute(physicsProxyElem, "type"))
                {
                    physicsProxyType = "auto";
                }
                else
                {
                    physicsProxyType = getAttributeValueAsStdString(physicsProxyElem, "type");
                }

                // ------- Falls noetig automatisch bodyproxy erstellen -------------
                // (wenn physicsProxyType gesetzt wurde)
                OgreNewt::CollisionPtr collision = OgreNewt::CollisionPtr();
                OgreNewt::World *thisWorld = PhysicsManager::getSingleton()._getNewtonWorld();

                if (physicsProxyType == "custom")
                {
                    ///@todo create physics proxy from custom collision primitives which are defined in children
           elements of <code>physicsProxyElem<code>
                }
                else if (physicsProxyType == "none")
                {
                    LOG_DEBUG(Logger::SCRIPT, "No physics proxy for entity '"+entity->getName()+"'.");
                }
                else
                {
                    const AxisAlignedBox &aab = entity->getMesh()->getBounds();
                    Ogre::Node* parentNode = entity->getParentNode();
                    Ogre::Vector3 size = (aab.getMaximum() - aab.getMinimum()) * parentNode->getScale();
                    bool forceBox = false;

                    if ((size.x < PhysicsManager::NEWTON_GRID_WIDTH ||
                         size.y < PhysicsManager::NEWTON_GRID_WIDTH ||
                         size.z < PhysicsManager::NEWTON_GRID_WIDTH) &&
                         physicsProxyType == "convexhull")
                    {
                        if (size.x < PhysicsManager::NEWTON_GRID_WIDTH)
                            size.x = PhysicsManager::NEWTON_GRID_WIDTH;
                        if (size.y < PhysicsManager::NEWTON_GRID_WIDTH)
                            size.y = PhysicsManager::NEWTON_GRID_WIDTH;
                        if (size.z < PhysicsManager::NEWTON_GRID_WIDTH)
                            size.z = PhysicsManager::NEWTON_GRID_WIDTH;

                        LOG_MESSAGE(Logger::SCRIPT, "Entity '"+entity->getName()+"' is planar, using 'box' as instead of
           '"+physicsProxyType+"'."); forceBox = true;
                    }
                    const Quaternion orientation(0,0,0,0);// = parentNode->getOrientation();
                    const Ogre::Vector3 pos = aab.getMinimum() * parentNode->getScale() + (size/2.0);


                    // Pruefen, ob schon ein identischer Proxy erstellt wurde um diesen erneut zu verwenden
                    AlreadyUsedCollision &aucol (mAutoCreatedCollisions[meshName]);
                    if (aucol.Type == physicsProxyType  &&
                        aucol.Scale == parentNode->getScale() &&
                        (!forceBox)) // sicherheitshalber
                    {
                        collision = aucol.ColPtr;
                        LOG_DEBUG(Logger::SCRIPT, " Reused physical body for entity '"+entity->getName()+"'.");
                    }
                    else
                    {
                        if (physicsProxyType == "box" || forceBox)
                        {
                            collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Box(
                                             thisWorld, size, orientation, pos));
                            LOG_DEBUG(Logger::SCRIPT, "Created physics proxy type 'box' for entity
           '"+entity->getName()+"'.");
                        }
                        else if (physicsProxyType == "pyramid")
                        {
                            collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Pyramid(
                                            thisWorld, size, orientation, pos));
                            LOG_DEBUG(Logger::SCRIPT, "Created physics proxy type 'pyramid' for entity
           '"+entity->getName()+"'.");
                        }
                        else if (physicsProxyType == "sphere")
                        {
                            double radius = std::max(size.x, std::max(size.y, size.z)) / 2.0;
                            collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid(
                                            thisWorld, Vector3(radius, radius, radius),
                                            orientation, pos));
                            LOG_DEBUG(Logger::SCRIPT, "Created physics proxy type 'sphere' for entity
           '"+entity->getName()+"'.");
                        }
                        else if (physicsProxyType == "ellipsoid")
                        {
                            // set the size x/z values to the maximum
                            Ogre::Vector3 s(size/2.0);
                            s.x = std::max(s.x, s.z);
                            s.z = s.x;
                            collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid(
                                            thisWorld, s,
                                            orientation, pos));
                            LOG_DEBUG(Logger::SCRIPT, "Created physics proxy type 'ellipsoid' for entity
           '"+entity->getName()+"'.");
                        }
                        else if (physicsProxyType == "capsule")
                        {
                            double radius = std::max(size.x, size.z) / 2.0;
                            double height = size.y;

                            collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Capsule(
                                            thisWorld,
                                            radius,
                                            height,
                                            orientation, pos));
                            LOG_DEBUG(Logger::SCRIPT, "Created physics proxy type 'capsule' for entity
           '"+entity->getName()+"'.");
                        }
                        else if (physicsProxyType == "convexhull")
                        {
                            collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(
                                            thisWorld,
                                            entity));
                            LOG_DEBUG(Logger::SCRIPT, "Created physics proxy type 'convexhull' for entity
           '"+entity->getName()+"'.");
                        }
                        else if (physicsProxyType == "mesh" || physicsProxyType == "auto")
                        {
                            collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::TreeCollision(
                                            thisWorld, entity, true));
                            LOG_DEBUG(Logger::SCRIPT, "Created physics proxy type 'mesh' for entity
           '"+entity->getName()+"'.");
                        }
                        else
                        {
                            LOG_ERROR(Logger::SCRIPT,
                                "Physics proxy type '"+physicsProxyType+"' of entity '"+entity->getName()+"' is
           unknown."); return;
                        }

                        aucol.ColPtr = collision;
                        aucol.Scale = parentNode->getScale();
                        aucol.Type = physicsProxyType;
                    }
                }

                if ( collision != NULL )
                {
                    collisions.push_back(collision);
                }


                // Add to physics of map
                if (collisions.size() > 0)
                {
                    PhysicsManager::getSingleton().addLevelGeometry(entity, collisions);
                    LOG_DEBUG(Logger::SCRIPT, " Entity '"+entity->getName()+"' in levelGeometry geladen");
                }
        */
    }

    void EntityNodeProcessor::processAnimation(Ogre::Entity* entity, const tinyxml2::XMLElement* animationElem)
    {
        ///@todo EntityNodeProcessor::processAnimation
    }
}
