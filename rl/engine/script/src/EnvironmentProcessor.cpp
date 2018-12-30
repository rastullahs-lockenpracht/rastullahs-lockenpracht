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

#include "EnvironmentProcessor.h"

#include "CoreSubsystem.h"
#include "World.h"
#include "XmlProcessor.h"

using namespace Ogre;

namespace rl
{
    bool EnvironmentProcessor::processNode(
        const tinyxml2::XMLElement* nodeElem, const Ogre::String& resourceGroup, bool loadGameObjects)
    {
        if (nodeElem)
        {
            processSkySettings(getChildNamed(nodeElem, "sky"));
            processFogSettings(getChildNamed(nodeElem, "fog"));
        }

        return true;
    }

    void EnvironmentProcessor::processSkySettings(const tinyxml2::XMLElement* skyElem)
    {
        if (skyElem)
        {

            if (!hasAttribute(skyElem, "material") || !hasAttribute(skyElem, "type"))
            {
                LOG_ERROR(Logger::RULES, "<sky> element must have at least attributes 'type' and 'material'.");
            }
            else
            {
                Ogre::String type = getAttributeValueAsStdString(skyElem, "type");
                Ogre::String material = getAttributeValueAsStdString(skyElem, "material");

                bool drawFirst = true;
                if (hasAttribute(skyElem, "drawfirst"))
                {
                    drawFirst = getAttributeValueAsBool(skyElem, "drawfirst");
                }

                Ogre::Real distance = 5000;
                if (hasAttribute(skyElem, "distance"))
                {
                    distance = getAttributeValueAsReal(skyElem, "distance");
                }

                if (type == "dome")
                {
                    Ogre::Real curvature = 10;
                    Ogre::Real tiling = 8;

                    const tinyxml2::XMLElement* domeSettings = getChildNamed(skyElem, "skydomesettings");
                    if (domeSettings != NULL)
                    {
                        if (hasAttribute(domeSettings, "curvature"))
                        {
                            curvature = getAttributeValueAsReal(domeSettings, "curvature");
                        }
                        if (hasAttribute(domeSettings, "tiling"))
                        {
                            curvature = getAttributeValueAsReal(domeSettings, "tiling");
                        }
                    }
                    CoreSubsystem::getSingleton().getWorld()->setSkyDome(
                        true, material, curvature, tiling, distance, drawFirst);
                }
                else if (type == "box")
                {
                    CoreSubsystem::getSingleton().getWorld()->setSkyBox(true, material, distance, drawFirst);
                }
                else if (type == "plane")
                {
                    LOG_ERROR(Logger::RULES, "Sky Plane is not implemented yet.");
                }
            }
        }
    }

    void EnvironmentProcessor::processFogSettings(const tinyxml2::XMLElement* fogElem)
    {
        if (fogElem)
        {
            if (!hasAttribute(fogElem, "type"))
            {
                LOG_ERROR(Logger::RULES, "<fog> element must have at least the attribute 'type'.");
                return;
            }

            const tinyxml2::XMLElement* colourElem = getChildNamed(fogElem, "colour");
            if (colourElem == NULL)
            {
                LOG_ERROR(Logger::RULES, "No fog colour set.");
                return;
            }
            ColourValue fogColour = processColour(colourElem);

            Ogre::String type = getAttributeValueAsStdString(fogElem, "type");
            if (type == "exp" || type == "exp2")
            {
                if (hasAttribute(fogElem, "density"))
                {
                    Ogre::Real density = getAttributeValueAsReal(fogElem, "density");
                    if (type == "exp")
                    {
                        CoreSubsystem::getSingleton().getWorld()->setFog(World::FOG_EXP, fogColour, density);
                    }
                    else if (type == "exp2")
                    {
                        CoreSubsystem::getSingleton().getWorld()->setFog(World::FOG_EXP2, fogColour, density);
                    }
                }
                else
                {
                    LOG_ERROR(Logger::RULES, type + " fog needs attribute 'density'.");
                }
            }
            else if (type == "linear")
            {
                if (hasAttribute(fogElem, "start") && hasAttribute(fogElem, "end"))
                {
                    Ogre::Real start = getAttributeValueAsReal(fogElem, "start");
                    Ogre::Real end = getAttributeValueAsReal(fogElem, "end");
                    CoreSubsystem::getSingleton().getWorld()->setFog(World::FOG_LINEAR, fogColour, 0, start, end);
                }
                else
                {
                    LOG_ERROR(Logger::RULES, "linear fog needs attributes 'start' and 'end'.");
                }
            }
            else
            {
                LOG_ERROR(Logger::RULES, type + " is an unknown fog type.");
            }
        }
    }
}
