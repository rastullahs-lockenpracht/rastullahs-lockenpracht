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

#include "WaypointProcessor.h"

#include "AiSubsystem.h"
#include "Landmark.h"
#include "LandmarkPath.h"

using namespace Ogre;

namespace rl
{
    bool WaypointProcessor::processNode(
        const tinyxml2::XMLElement* nodeElem, const Ogre::String& resourceGroup, bool loadGameObjects)
    {
        if (nodeElem == NULL)
        {
            return false;
        }

        for (const tinyxml2::XMLNode* cur = nodeElem->FirstChild(); cur != NULL; cur = cur->NextSibling())
        {
            const tinyxml2::XMLElement* elem = cur->ToElement();
            if (elem && hasNodeName(elem, "landmark"))
            {
                Ogre::String name = getAttributeValueAsStdString(elem, "name");
                const tinyxml2::XMLElement* posChild = getChildNamed(elem, "position");
                if (name != "" && posChild != NULL)
                {
                    AiSubsystem::getSingleton().createLandmark(name, processVector3(posChild));
                }
            }
        }

        for (const tinyxml2::XMLNode* cur = nodeElem->FirstChild(); cur != NULL; cur = cur->NextSibling())
        {
            const tinyxml2::XMLElement* elem = cur->ToElement();
            if (elem && hasNodeName(elem, "landmarkgraph"))
            {
                ///@todo process waypointgraph
                Ogre::String name;

                LandmarkPath* path = AiSubsystem::getSingleton().createLandmarkPath(name);

                for (const tinyxml2::XMLNode* curPathChild = nodeElem->FirstChild(); curPathChild != NULL;
                     curPathChild = curPathChild->NextSibling())
                {
                    const tinyxml2::XMLElement* curLmElem = curPathChild->ToElement();
                    if (curLmElem && hasNodeName(curLmElem, "landmark"))
                    {
                        Landmark* lm
                            = AiSubsystem::getSingleton().getLandmark(getAttributeValueAsStdString(curLmElem, "name"));

                        if (lm != NULL)
                        {
                            path->addPoint(lm);
                        }
                    }
                }
            }
        }

        return true;
    }
}
