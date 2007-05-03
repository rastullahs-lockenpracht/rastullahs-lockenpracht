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

#include "WaypointProcessor.h"

#include "AiSubsystem.h"
#include "Landmark.h"
#include "LandmarkPath.h"
#include "XmlHelper.h"

using namespace XERCES_CPP_NAMESPACE;
using namespace Ogre;

namespace rl
{
	bool WaypointProcessor::processNode(DOMElement* nodeElem, bool loadGameObjects)
	{
		if (nodeElem == NULL)
		{
			return false;
		}

		for (DOMNode* cur = nodeElem->getFirstChild(); cur != NULL; cur = cur->getNextSibling())
		{
			if (cur->getNodeType() == DOMNode::ELEMENT_NODE && XmlHelper::hasNodeName(cur, "landmark"))
			{
				DOMElement* elem = static_cast<DOMElement*>(cur);
				Ogre::String name = XmlHelper::getAttributeValueAsStdString(elem, "name");
				DOMElement* posChild = XmlHelper::getChildNamed(elem, "position");
				if (name != "" && posChild != NULL)
				{
					AiSubsystem::getSingleton().createLandmark(
						name, processVector3(posChild));
				}
			}
		}

		for (DOMNode* cur = nodeElem->getFirstChild(); cur != NULL; cur = cur->getNextSibling())
		{
			if (cur->getNodeType() == DOMNode::ELEMENT_NODE && XmlHelper::hasNodeName(cur, "landmarkgraph"))
			{
				DOMElement* elem = static_cast<DOMElement*>(cur);
				///@todo process waypointgraph
				Ogre::String name;

				LandmarkPath* path = AiSubsystem::getSingleton().createLandmarkPath(name);
				
				for (DOMNode* curPathChild = nodeElem->getFirstChild(); 
					curPathChild != NULL; curPathChild = curPathChild->getNextSibling())
				{
					if (curPathChild->getNodeType() == DOMNode::ELEMENT_NODE 
						&& XmlHelper::hasNodeName(curPathChild, "landmark"))
					{
						DOMElement* curLmElem = static_cast<DOMElement*>(curPathChild);
						Landmark* lm = AiSubsystem::getSingleton().getLandmark(
							XmlHelper::getAttributeValueAsStdString(curLmElem, "name"));

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
