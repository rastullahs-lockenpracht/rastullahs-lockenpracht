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
	bool WaypointProcessor::processNode(const TiXmlElement* nodeElem, const Ogre::String& resourceGroup, bool loadGameObjects)
	{
		if (nodeElem == NULL)
		{
			return false;
		}

		for (const TiXmlNode* cur = nodeElem->FirstChild(); cur != NULL; cur = cur->NextSibling())
		{
			if (cur->Type() == TiXmlNode::ELEMENT && hasNodeName(cur, "landmark"))
			{
				const TiXmlElement* elem = cur->ToElement();
				Ogre::String name = getAttributeValueAsStdString(elem, "name");
				const TiXmlElement* posChild = getChildNamed(elem, "position");
				if (name != "" && posChild != NULL)
				{
					AiSubsystem::getSingleton().createLandmark(
						name, processVector3(posChild));
				}
			}
		}

		for (const TiXmlNode* cur = nodeElem->FirstChild(); cur != NULL; cur = cur->NextSibling())
		{
			if (cur->Type() == TiXmlNode::ELEMENT && hasNodeName(cur, "landmarkgraph"))
			{
				const TiXmlElement* elem = cur->ToElement();
				///@todo process waypointgraph
				Ogre::String name;

				LandmarkPath* path = AiSubsystem::getSingleton().createLandmarkPath(name);

				for (const TiXmlNode* curPathChild = nodeElem->FirstChild();
					curPathChild != NULL; curPathChild = curPathChild->NextSibling())
				{
					if (curPathChild->Type() == TiXmlNode::ELEMENT
						&& hasNodeName(curPathChild, "landmark"))
					{
						const TiXmlElement* curLmElem = curPathChild->ToElement();
						Landmark* lm = AiSubsystem::getSingleton().getLandmark(
							getAttributeValueAsStdString(curLmElem, "name"));

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
