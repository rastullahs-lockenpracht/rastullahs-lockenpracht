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
#include "ParticleSystemNodeProcessor.h"

#include "XmlHelper.h"

#include "Actor.h"
#include "ActorManager.h"
#include "ParticleSystemObject.h"

using namespace Ogre;
using namespace XERCES_CPP_NAMESPACE;

namespace rl
{
    bool ParticleSystemNodeProcessor::processNode(XERCES_CPP_NAMESPACE::DOMElement* nodeElem, bool loadGameObjects)
    {
		if (!XmlHelper::hasNodeName(nodeElem, "particlesystem"))
		{
			return false;
		}

		LOG_DEBUG(Logger::RULES, 
            "Processing particle system node " 
                + XmlHelper::getAttributeValueAsStdString(nodeElem, "name"));

		if (XmlHelper::hasAttribute(nodeElem, "name") && XmlHelper::hasAttribute(nodeElem, "material"))
		{
			Ogre::String name = XmlHelper::getAttributeValueAsStdString(nodeElem, "name");
			Ogre::String material = XmlHelper::getAttributeValueAsStdString(nodeElem, "material");

			Actor* part = ActorManager::getSingleton().createParticleSystemActor(
				name, material);

			if (XmlHelper::hasAttribute(nodeElem, "active"))
			{
				bool active = XmlHelper::getAttributeValueAsBool(nodeElem, "active");
				static_cast<ParticleSystemObject*>(part->getControlledObject())->setActive(active);
			}
		}
	}
}
