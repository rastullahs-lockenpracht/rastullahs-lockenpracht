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

#include "PlaneNodeProcessor.h"

#include "CoreSubsystem.h"
#include "PhysicsManager.h"
#include "World.h"

//#include <OgreMaterialManager.h>

using namespace Ogre;

namespace rl
{
    PlaneNodeProcessor::PlaneNodeProcessor()
    {
    }

    bool PlaneNodeProcessor::processNode(
        const tinyxml2::XMLElement* nodeElem, const Ogre::String& resourceGroup, bool loadGameObjects)
    {
        if (!hasNodeName(nodeElem, "plane"))
        {
            return false;
        }

        Ogre::String entName = getAttributeValueAsStdString(nodeElem, "name");

        LOG_DEBUG(Logger::RULES, "Processing plane node " + entName);
        if (entName == "")
        {
            entName = getRandomName("Plane");
        }

        Quaternion orientation(Quaternion::IDENTITY);
        Vector3 position(Vector3::ZERO);
        Vector2 scale(1, 1);

        const tinyxml2::XMLElement* oriElem = getChildNamed(nodeElem, "rotation");
        if (oriElem != NULL)
        {
            orientation = processQuaternion(oriElem);
        }
        else
        {
            LOG_WARNING(Logger::RULES, "No orientation given for plane, used Identity");
        }

        const tinyxml2::XMLElement* posElem = getChildNamed(nodeElem, "position");
        if (posElem != NULL)
        {
            position = processVector3(posElem);
        }
        else
        {
            LOG_WARNING(Logger::RULES, "No position given for plane, used (0,0,0)");
        }

        const tinyxml2::XMLElement* scaleElem = getChildNamed(nodeElem, "scale");
        if (posElem != NULL)
        {
            scale = processVector2(scaleElem);
        }
        else
        {
            LOG_WARNING(Logger::RULES, "No scale given for plane, used (0,0)");
        }

        while (MeshManager::getSingleton().getByName(entName))
        {
            entName = getRandomName("Plane");
        }

        SceneNode* node = getRootSceneNode()->createChildSceneNode(entName + "Node", position, orientation);

        MovablePlane* plane = new MovablePlane(entName + "Plane");
        plane->d = 0;
        plane->normal = Vector3::UNIT_Y;

        MeshManager::getSingleton().createPlane(
            entName + "Mesh", resourceGroup, *plane, scale.x, scale.y, 10, 10, true, 1, 1, 1, Vector3::UNIT_Z);

        Entity* ent
            = CoreSubsystem::getSingleton().getWorld()->getSceneManager()->createEntity(entName, entName + "Mesh");

        LOG_DEBUG(Logger::RULES, " Loaded plane " + entName);

        node->attachObject(ent);
        node->attachObject(plane);
        // node->scale(scale.x,1,scale.y);

        createCollision(ent, getChildNamed(nodeElem, "physicsproxy"));

        const tinyxml2::XMLElement* materialElem = getChildNamed(nodeElem, "material");
        if (materialElem)
        {
            if (getChildNamed(nodeElem, "renderToTexture"))
            {
                Ogre::String matName = getAttributeValueAsStdString(materialElem, "name");
                MaterialPtr material = static_cast<MaterialPtr>(MaterialManager::getSingleton().getByName(matName))
                                           ->clone(matName + entName);
                createRenderToTextures(ent, plane, material, getChildNamed(nodeElem, "renderToTexture"));
                ent->setMaterialName(matName + entName);
            }
            else
                ent->setMaterialName(getAttributeValueAsStdString(materialElem, "name"));
        }
        else
        {
            LOG_WARNING(Logger::RULES, "No material given for plane " + entName);
        }
        return true;
    }

    void PlaneNodeProcessor::createCollision(Ogre::Entity* entity, const tinyxml2::XMLElement* physicsProxyElem)
    {
        bool collisionEnabled = false;
        if (physicsProxyElem == NULL || !hasAttribute(physicsProxyElem, "collision"))
            collisionEnabled = false;
        else if (getAttributeValueAsBool(physicsProxyElem, "collision"))
            collisionEnabled = true;
        if (collisionEnabled)
        {
            std::vector<OgreNewt::CollisionPtr> collisions;
            OgreNewt::CollisionPtr collision = OgreNewt::CollisionPtr();
            OgreNewt::World* world = PhysicsManager::getSingleton()._getNewtonWorld();

            const AxisAlignedBox& aab = entity->getMesh()->getBounds();
            Ogre::Node* parentNode = entity->getParentNode();
            Ogre::Vector3 size = (aab.getMaximum() - aab.getMinimum()) * parentNode->getScale();

            const Quaternion orientation(0, 0, 0, 0); // = parentNode->getOrientation();
            const Ogre::Vector3 pos = aab.getMinimum() * parentNode->getScale() + (size / 2.0);

            collision = PhysicsManager::getSingleton().createCollision(
                entity, GT_BOX, "", Vector3::ZERO, Quaternion::IDENTITY, 0, NULL, NULL, true);

            if (collision != NULL)
            {
                collisions.push_back(collision);
            }

            // Add to physics of map
            if (collisions.size() > 0)
            {
                PhysicsManager::getSingleton().addLevelGeometry(entity, collisions);
                LOG_WARNING(Logger::RULES, " Plane '" + entity->getName() + "' in levelGeometry geladen");
            }
        }
    }

    void PlaneNodeProcessor::createRenderToTextures(
        Ogre::Entity* entity, Plane* plane, MaterialPtr material, const tinyxml2::XMLElement* rttElem)
    {
        if (rttElem == NULL)
            return;

        Camera* cam
            = CoreSubsystem::getSingleton().getWorld()->getSceneManager()->createCamera("Cam" + entity->getName());
        cam->setNearClipDistance(CoreSubsystem::getSingleton().getWorld()->getActiveCamera()->getNearClipDistance());
        cam->setFarClipDistance(CoreSubsystem::getSingleton().getWorld()->getActiveCamera()->getFarClipDistance());
        // cam->setFarClipDistance(1000000);
        cam->setAspectRatio(CoreSubsystem::getSingleton().getWorld()->getActiveCamera()->getAspectRatio());
        cam->setFOVy(CoreSubsystem::getSingleton().getWorld()->getActiveCamera()->getFOVy());

        AliasTextureNamePairList aliases;

        if (getAttributeValueAsBool(rttElem, "reflection"))
        {
            TexturePtr texture = Ogre::TextureManager::getSingleton().createManual("Reflection" + entity->getName(),
                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, 512, 512, 0, PF_R8G8B8,
                TU_RENDERTARGET);
            RenderTexture* rttTex = texture->getBuffer()->getRenderTarget();

            Viewport* v = rttTex->addViewport(cam);
            v->setOverlaysEnabled(false);
            rttTex->addListener(new PlaneReflectionTextureListener(entity, cam, plane));

            aliases["reflection"] = "Reflection" + entity->getName();

            cam->enableCustomNearClipPlane((MovablePlane*)plane);
        }
        if (getAttributeValueAsBool(rttElem, "refraction"))
        {
            TexturePtr texture = Ogre::TextureManager::getSingleton().createManual("Refraction" + entity->getName(),
                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, 512, 512, 0, PF_R8G8B8,
                TU_RENDERTARGET);
            RenderTexture* rttTex = texture->getBuffer()->getRenderTarget();

            Viewport* v = rttTex->addViewport(cam);
            v->setOverlaysEnabled(false);
            rttTex->addListener(new PlaneRefractionTextureListener(entity, cam));

            aliases["refraction"] = "Refraction" + entity->getName();

            plane->normal = Vector3::NEGATIVE_UNIT_Y;
            cam->enableCustomNearClipPlane((MovablePlane*)plane);
        }
        if (!material->applyTextureAliases(aliases))
            LOG_ERROR("PLANE", "Texture Aliase konnten nicht angewandt werden");
    }

    PlaneReflectionTextureListener::PlaneReflectionTextureListener(Ogre::Entity* ent, Ogre::Camera* cam, Plane* plane)
    {
        mEntity = ent;
        mCamera = cam;
        mPlane = plane;
    }

    void PlaneReflectionTextureListener::preRenderTargetUpdate(const RenderTargetEvent& evt)
    {
        mCamera->setPosition(CoreSubsystem::getSingleton().getWorld()->getActiveCamera()->getDerivedPosition());
        mCamera->setOrientation(CoreSubsystem::getSingleton().getWorld()->getActiveCamera()->getDerivedOrientation());
        mEntity->setVisible(false);
        mCamera->enableReflection((MovablePlane*)mPlane);
    }

    void PlaneReflectionTextureListener::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
    {
        mEntity->setVisible(true);
        mCamera->disableReflection();
    }

    PlaneRefractionTextureListener::PlaneRefractionTextureListener(Entity* ent, Camera* cam)
    {
        mEntity = ent;
        mCamera = cam;
    }

    void PlaneRefractionTextureListener::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
    {
        mCamera->setPosition(CoreSubsystem::getSingleton().getWorld()->getActiveCamera()->getDerivedPosition());
        mCamera->setOrientation(CoreSubsystem::getSingleton().getWorld()->getActiveCamera()->getDerivedOrientation());
        mEntity->setVisible(false);
    }

    void PlaneRefractionTextureListener::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
    {
        mEntity->setVisible(true);
    }
}
