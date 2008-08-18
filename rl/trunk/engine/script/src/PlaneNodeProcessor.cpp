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

#include <xercesc/dom/DOM.hpp>

#include "CoreSubsystem.h"
#include "PhysicsManager.h"
#include "World.h"

#include <OgreMaterialManager.h>

using namespace Ogre;
using namespace XERCES_CPP_NAMESPACE;

namespace rl
{
	PlaneNodeProcessor::PlaneNodeProcessor()
	{
	}

	bool PlaneNodeProcessor::processNode(DOMElement *nodeElem, bool loadGameObjects)
	{
		if (!hasNodeName(nodeElem, "plane"))
        {
            return false;
        }

		Ogre::String entName = getAttributeValueAsStdString(nodeElem, "name");

		LOG_DEBUG(Logger::RULES,
            "Processing plane node "
                + entName);
		if(entName=="")
		{
			entName = getRandomName("Plane");
		}

		Quaternion orientation(Quaternion::IDENTITY);
		Vector3 position(Vector3::ZERO);
		Vector2 scale(1,1);

		DOMElement* oriElem = getChildNamed(nodeElem, "rotation");
        if (oriElem != NULL)
        {
            orientation = processQuaternion(oriElem);
        }
        else
        {
            LOG_WARNING(Logger::RULES, "No orientation given for plane, used Identity");
        }

		DOMElement* posElem = getChildNamed(nodeElem, "position");
        if (posElem != NULL)
        {
            position = processVector3(posElem);
        }
		else
        {
            LOG_WARNING(Logger::RULES, "No position given for plane, used (0,0,0)");
        }

		DOMElement* scaleElem = getChildNamed(nodeElem, "scale");
        if (posElem != NULL)
        {
            scale = processVector2(scaleElem);
        }
		else
        {
            LOG_WARNING(Logger::RULES, "No scale given for plane, used (0,0)");
        }

		while(!MeshManager::getSingleton().getByName(entName).isNull())
		{
			entName = getRandomName("Plane");
		}

		SceneNode* node = getRootSceneNode()->createChildSceneNode(entName, position, orientation);

		Plane* plane = new MovablePlane(entName);
		plane->d = 0;
		plane->normal = Vector3::UNIT_Y;

		MeshManager::getSingleton().createPlane(entName, "custom", *plane, scale.x, scale.y, 1, 1, true, 1, 1, 1, Vector3::UNIT_Z);

		Entity* ent = CoreSubsystem::getSingleton().getWorld()->getSceneManager()->createEntity(entName, entName);

		LOG_DEBUG(Logger::RULES, " Loaded plane "+entName);

		node->attachObject(ent);
		//node->scale(scale.x,1,scale.y);

		createCollision(ent, getChildNamed(nodeElem, "physicsproxy"));
		
		DOMElement* materialElem = getChildNamed(nodeElem, "material");
		if(materialElem)
		{	
			if(getChildNamed(nodeElem, "renderToTexture"))
			{
				Ogre::String matName = getAttributeValueAsStdString(materialElem, "name");
				MaterialPtr material = static_cast<MaterialPtr>(MaterialManager::getSingleton().getByName(matName))->clone(matName + entName);
				createRenderToTextures(ent, plane, material, getChildNamed(nodeElem, "renderToTexture"));
				ent->setMaterialName(matName + entName);
			}
			else
				ent->setMaterialName(getAttributeValueAsStdString(materialElem, "name"));
		}
		else
        {
            LOG_WARNING(Logger::RULES, "No material given for plane "+entName);
        }
		return true;
	}

	void PlaneNodeProcessor::createCollision(Ogre::Entity *entity, DOMElement *physicsProxyElem)
	{
		bool collisionEnabled = false;
		if (physicsProxyElem == NULL || !hasAttribute(physicsProxyElem, "collision"))
			collisionEnabled = false;
		else if(getAttributeValueAsBool(physicsProxyElem, "collision"))
			collisionEnabled = true;
		if(collisionEnabled)
		{
			std::vector<OgreNewt::CollisionPtr> collisions;
			OgreNewt::CollisionPtr collision = OgreNewt::CollisionPtr();
			OgreNewt::World* world = PhysicsManager::getSingleton()._getNewtonWorld();

			const AxisAlignedBox &aab = entity->getMesh()->getBounds();
			Ogre::Node* parentNode = entity->getParentNode();
			Ogre::Vector3 size = (aab.getMaximum() - aab.getMinimum()) * parentNode->getScale();

			const Quaternion orientation(0,0,0,0);// = parentNode->getOrientation();
			const Ogre::Vector3 pos = aab.getMinimum() * parentNode->getScale() + (size/2.0);

			collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Box(world, size, orientation, pos));

			if (!collision.isNull())
			{
				collisions.push_back(collision);
			}

			// Add to physics of map
			if (collisions.size() > 0)
			{
				PhysicsManager::getSingleton().addLevelGeometry(entity, collisions);
				LOG_WARNING(Logger::RULES, " Plane '"+entity->getName()+"' in levelGeometry geladen");
			}
		}
	}

	void PlaneNodeProcessor::createRenderToTextures(Ogre::Entity* entity, Plane* plane, MaterialPtr material, XERCES_CPP_NAMESPACE::DOMElement* rttElem)
	{
		if(rttElem == NULL)
			return;

		//MeshPtr mesh = entity->getMesh();
		AliasTextureNamePairList aliases;

		if(getAttributeValueAsBool(rttElem, "reflection"))
		{
			TexturePtr texture = Ogre::TextureManager::getSingleton().createManual( "Reflection" + entity->getName(), 
				ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, 
				512, 512, 0, PF_R8G8B8, TU_RENDERTARGET );
			RenderTexture* rttTex = texture->getBuffer()->getRenderTarget();

			CoreSubsystem::getSingleton().getWorld()->getActiveCamera()->setAutoAspectRatio(true);
			Viewport *v = rttTex->addViewport( CoreSubsystem::getSingleton().getWorld()->getActiveCamera() ); //Bleibt die Kamera immer die gleiche?
			v->setOverlaysEnabled(false);
			rttTex->addListener(new PlaneReflectionTextureListener(entity, CoreSubsystem::getSingleton().getWorld()->getActiveCamera(), plane));
			
			aliases["reflection"] = "Reflection" + entity->getName();
			/*int num = mesh->getNumSubMeshes();
			for(int i = 0; i < mesh->getNumSubMeshes(); i++)
			{
				SubMesh* sub = mesh->getSubMesh(i);
				sub->addTextureAlias("reflection", "Reflection" + entity->getName());
			}
			mesh->updateMaterialForAllSubMeshes();*/
		}
		if(getAttributeValueAsBool(rttElem, "refraction"))
		{
			TexturePtr texture = Ogre::TextureManager::getSingleton().createManual( "Refraction" + entity->getName(), 
				ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, 
				512, 512, 0, PF_R8G8B8, TU_RENDERTARGET );
			RenderTexture* rttTex = texture->getBuffer()->getRenderTarget();
			
			Viewport *v = rttTex->addViewport( CoreSubsystem::getSingleton().getWorld()->getActiveCamera());
			v->setOverlaysEnabled(false);
			rttTex->addListener(new PlaneRefractionTextureListener(entity));

			aliases["refraction"] = "Refraction" + entity->getName();
			/*for(int i = 0; i < mesh->getNumSubMeshes(); i++)
			{
				SubMesh* sub = mesh->getSubMesh(i);
				sub->addTextureAlias("refraction", "Refraction" + entity->getName());
			}*/
		}
		if(!material->applyTextureAliases(aliases))
			LOG_ERROR("PLANE", "Texture Aliase konnten nicht angewandt werden");
	}

	PlaneReflectionTextureListener::PlaneReflectionTextureListener(Ogre::Entity *ent, Ogre::Camera* cam, Plane* plane)
	{
		mEntity = ent;
		mCamera = cam;
		mPlane = plane;
	}

	void PlaneReflectionTextureListener::preRenderTargetUpdate(const RenderTargetEvent &evt)
	{
		mEntity->setVisible(false);
		mCamera->enableReflection(*mPlane);
	}

	void PlaneReflectionTextureListener::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
	{
		mEntity->setVisible(true);
		mCamera->disableReflection();
	}

	PlaneRefractionTextureListener::PlaneRefractionTextureListener(Entity* ent)
	{
		mEntity = ent;
	}

	void PlaneRefractionTextureListener::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
	{
		mEntity->setVisible(false);
	}

	void PlaneRefractionTextureListener::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
	{
		mEntity->setVisible(true);
	}
}

