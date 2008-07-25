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

using namespace Ogre;
using namespace XERCES_CPP_NAMESPACE;

namespace rl
{
	PlaneNodeProcessor::PlaneNodeProcessor(const Ogre::String &resourcegroup)
	{
	}

	bool PlaneNodeProcessor::processNode(xercesc_2_7::DOMElement *nodeElem, bool loadGameObjects)
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
            scale = processVector2(posElem);
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

		MeshManager::getSingleton().createPlane(entName, mResourceGroup, *plane, 1, 1, 1, 1, true, 1, 1, 1, Vector3::UNIT_Z);

		Entity* ent = CoreSubsystem::getSingleton().getWorld()->getSceneManager()->createEntity(entName, entName);

		LOG_DEBUG(Logger::RULES, " Loaded plane "+entName);

		node->attachObject(ent);

		createCollision(ent, getChildNamed(nodeElem, "physicsproxy"));

		DOMElement* materialElem = getChildNamed(nodeElem, "material");
		if(materialElem)
		{
			ent->setMaterialName(getAttributeValueAsStdString(materialElem, "name"));
		}
		else
        {
            LOG_WARNING(Logger::RULES, "No material given for plane "+entName);
        }

		node->scale(scale.x,1,scale.y);

		return true;
	}

	void PlaneNodeProcessor::createCollision(Ogre::Entity *entity, xercesc_2_7::DOMElement *physicsProxyElem)
	{
		bool collisionEnabled = false;
		if (physicsProxyElem == NULL || !hasAttribute(physicsProxyElem, "type"))
			collisionEnabled = true;
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
				LOG_DEBUG(Logger::RULES, " Plane '"+entity->getName()+"' in levelGeometry geladen");
			}
		}
	}
}