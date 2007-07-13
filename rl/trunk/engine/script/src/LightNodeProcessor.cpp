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
#include "stdinc.h" //precompiled header

#include "LightNodeProcessor.h"

#include "XmlHelper.h"

#include "Actor.h"
#include "ActorManager.h"

using namespace Ogre;
using namespace XERCES_CPP_NAMESPACE;

namespace rl
{
    bool LightNodeProcessor::processNode(XERCES_CPP_NAMESPACE::DOMElement* nodeElem, bool loadGameObjects)
    {
        if (!XmlHelper::hasNodeName(nodeElem, "light"))
        {
            return false;
        }

        LOG_DEBUG(Logger::RULES,
            "Processing light node "
                + XmlHelper::getAttributeValueAsStdString(nodeElem, "name"));

        Ogre::String name = XmlHelper::getAttributeValueAsStdString(nodeElem, "name");
        Ogre::String stype = XmlHelper::getAttributeValueAsStdString(nodeElem, "type");
        bool visible = XmlHelper::getAttributeValueAsBool(nodeElem, "visible");
        bool shadowCaster = XmlHelper::getAttributeValueAsBool(nodeElem, "castShadows");

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

        DOMElement* posElem = XmlHelper::getChildNamed(nodeElem, "position");
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
        light->setVisible(visible);

        DOMElement* diffElem = XmlHelper::getChildNamed(nodeElem, "colourDiffuse");
        if (diffElem != NULL)
        {
            light->setDiffuseColour(processColour(diffElem));
        }

        DOMElement* specElem = XmlHelper::getChildNamed(nodeElem, "colourSpecular");
        if (specElem != NULL)
        {
            light->setSpecularColour(processColour(specElem));
        }

        DOMElement* attElem = XmlHelper::getChildNamed(nodeElem, "lightAttenuation");
        if (attElem != NULL)
        {
            if (XmlHelper::hasAttribute(attElem, "range")
                && XmlHelper::hasAttribute(attElem, "constant")
                && XmlHelper::hasAttribute(attElem, "linear")
                && XmlHelper::hasAttribute(attElem, "quadratic"))
            {
                Ogre::Real range = XmlHelper::getAttributeValueAsReal(attElem, "range");
                Ogre::Real constant = XmlHelper::getAttributeValueAsReal(attElem, "constant");
                Ogre::Real linear = XmlHelper::getAttributeValueAsReal(attElem, "linear");
                Ogre::Real quadratic = XmlHelper::getAttributeValueAsReal(attElem, "quadratic");

                light->setAttenuation(range, constant, linear, quadratic);
            }
        }
        ///@todo default attenuation?

        if (stype == "directional")
        {
            DOMElement* dirElem = XmlHelper::getChildNamed(nodeElem, "direction");
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
            DOMElement* rangeElem = XmlHelper::getChildNamed(nodeElem, "spotlightrange");
            if (rangeElem != NULL)
            {
                Ogre::Real innerAngle = XmlHelper::getAttributeValueAsReal(rangeElem, "inner");
                Ogre::Real outerAngle = XmlHelper::getAttributeValueAsReal(rangeElem, "outer");
                if (XmlHelper::hasAttribute(rangeElem, "falloff"))
                {
                    light->setSpotlightRange(
                        innerAngle, outerAngle, XmlHelper::getAttributeValueAsReal(rangeElem, "falloff"));
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
