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

#include "LightNodeProcessor.h"

#include "Actor.h"
#include "ActorManager.h"

using namespace Ogre;

namespace rl
{
    bool LightNodeProcessor::processNode(
        const tinyxml2::XMLElement* nodeElem, const Ogre::String& resourceGroup, bool loadGameObjects)
    {
        if (!hasNodeName(nodeElem, "light"))
        {
            return false;
        }

        LOG_DEBUG(Logger::RULES, "Processing light node " + getAttributeValueAsStdString(nodeElem, "name"));

        Ogre::String name = getAttributeValueAsStdString(nodeElem, "name");
        Ogre::String stype = getAttributeValueAsStdString(nodeElem, "type");
        bool visible = getAttributeValueAsBool(nodeElem, "visible");
        bool shadowCaster = getAttributeValueAsBool(nodeElem, "castShadows");

        LightObject::LightTypes type = LightObject::LT_DIRECTIONAL;
        if (stype == "point")
        {
            type = LightObject::LT_POINT;
        }
        else if (stype == "directional")
        {
            type = LightObject::LT_DIRECTIONAL;
        }
        else if (stype == "spot")
        {
            type = LightObject::LT_SPOTLIGHT;
        }
        else
        {
            LOG_ERROR(Logger::RULES, "Could not create light.");
            return false;
        }
        Actor* lightActor = ActorManager::getSingleton().createLightActor(name, type);

        const tinyxml2::XMLElement* posElem = getChildNamed(nodeElem, "position");
        if (posElem != NULL)
        {
            lightActor->placeIntoScene(processVector3(posElem));
        }
        else
        {
            lightActor->placeIntoScene(Vector3::ZERO);
        }

        LightObject* light = static_cast<LightObject*>(lightActor->getControlledObject());

        light->setCastShadows(shadowCaster);
        light->setActive(visible);

        const tinyxml2::XMLElement* diffElem = getChildNamed(nodeElem, "colourDiffuse");
        if (diffElem != NULL)
        {
            light->setDiffuseColour(processColour(diffElem));
        }

        const tinyxml2::XMLElement* specElem = getChildNamed(nodeElem, "colourSpecular");
        if (specElem != NULL)
        {
            light->setSpecularColour(processColour(specElem));
        }

        const tinyxml2::XMLElement* attElem = getChildNamed(nodeElem, "lightAttenuation");
        if (attElem != NULL)
        {
            if (hasAttribute(attElem, "range") && hasAttribute(attElem, "constant") && hasAttribute(attElem, "linear")
                && hasAttribute(attElem, "quadratic"))
            {
                Ogre::Real range = getAttributeValueAsReal(attElem, "range");
                Ogre::Real constant = getAttributeValueAsReal(attElem, "constant");
                Ogre::Real linear = getAttributeValueAsReal(attElem, "linear");
                Ogre::Real quadratic = getAttributeValueAsReal(attElem, "quadratic");

                light->setAttenuation(range, constant, linear, quadratic);
            }
        }
        ///@todo default attenuation?

        if (stype == "directional")
        {
            const tinyxml2::XMLElement* dirElem = getChildNamed(nodeElem, "direction");
            if (dirElem != NULL)
            {
                light->setDirection(processVector3(dirElem));
            }
            else
            {
                light->setDirection(Vector3::NEGATIVE_UNIT_Y);
            }
        }
        else if (stype == "spot")
        {
            const tinyxml2::XMLElement* rangeElem = getChildNamed(nodeElem, "spotlightrange");
            if (rangeElem != NULL)
            {
                Ogre::Real innerAngle = getAttributeValueAsReal(rangeElem, "inner");
                Ogre::Real outerAngle = getAttributeValueAsReal(rangeElem, "outer");
                if (hasAttribute(rangeElem, "falloff"))
                {
                    light->setSpotlightRange(innerAngle, outerAngle, getAttributeValueAsReal(rangeElem, "falloff"));
                }
                else
                {
                    light->setSpotlightRange(innerAngle, outerAngle);
                }
            }
        }

        ///@todo create light

        return true;
    }
}
