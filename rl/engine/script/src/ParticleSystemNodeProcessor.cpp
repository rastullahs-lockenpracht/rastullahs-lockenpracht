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

#include "ParticleSystemNodeProcessor.h"

#include "Actor.h"
#include "ActorManager.h"
#include "ParticleSystemObject.h"

using namespace Ogre;

namespace rl
{
    bool ParticleSystemNodeProcessor::processNode(
        const tinyxml2::XMLElement* nodeElem, const Ogre::String& resourceGroup, bool loadGameObjects)
    {
        if (!hasNodeName(nodeElem, "particlesystem"))
        {
            return false;
        }

        if (!hasAttribute(nodeElem, "material"))
        {
            LOG_WARNING(Logger::SCRIPT, "Particle system nodes must at least have the attribute 'material'.");
            return false;
        }

        Ogre::String name = getAttributeValueAsStdString(nodeElem, "name");

        LOG_DEBUG(Logger::SCRIPT, "Processing particle system node " + name);

        Ogre::String material = getAttributeValueAsStdString(nodeElem, "material");

        Actor* part = ActorManager::getSingleton().createParticleSystemActor(name, material);

        if (hasAttribute(nodeElem, "active"))
        {
            bool active = getAttributeValueAsBool(nodeElem, "active");
            static_cast<ParticleSystemObject*>(part->getControlledObject())->setActive(active);
        }

        return true;
    }
}
