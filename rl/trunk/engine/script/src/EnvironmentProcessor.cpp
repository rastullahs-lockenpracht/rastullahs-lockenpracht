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

#include "EnvironmentProcessor.h"

#include "CoreSubsystem.h"
#include "World.h"
#include "XmlHelper.h"

using namespace XERCES_CPP_NAMESPACE;
using namespace Ogre;


namespace rl
{
	bool EnvironmentProcessor::processNode(XERCES_CPP_NAMESPACE::DOMElement* nodeElem, bool loadGameObjects)
	{
		if (nodeElem == NULL)
		{
			return false;
		}

		processSkySettings(XmlHelper::getChildNamed(nodeElem, "sky"));
		processFogSettings(XmlHelper::getChildNamed(nodeElem, "fog"));

		return true;
	}


	void EnvironmentProcessor::processSkySettings(XERCES_CPP_NAMESPACE::DOMElement* skyElem)
	{
        if (skyElem == NULL)
		{
			return;
		}

		if (!XmlHelper::hasAttribute(skyElem, "material") 
			|| !XmlHelper::hasAttribute(skyElem, "type"))
		{
			LOG_ERROR(Logger::RULES, "<sky> element must have at least attributes 'type' and 'material'.");
		}
		else
		{
			Ogre::String type = XmlHelper::getAttributeValueAsStdString(skyElem, "type");
			Ogre::String material = XmlHelper::getAttributeValueAsStdString(skyElem, "material");

			bool drawFirst = true;
			if (XmlHelper::hasAttribute(skyElem, "drawfirst"))
			{
				drawFirst = XmlHelper::getAttributeValueAsBool(skyElem, "drawfirst");
			}
			
			Ogre::Real distance = 5000;
			if (XmlHelper::hasAttribute(skyElem, "distance"))
			{
				distance = XmlHelper::getAttributeValueAsReal(skyElem, "distance");
			}				

			if (type == "dome")
			{
				Ogre::Real curvature = 10;
				Ogre::Real tiling = 8;

				DOMElement* domeSettings = XmlHelper::getChildNamed(skyElem, "skydomesettings");
				if (domeSettings != NULL)
				{
					if (XmlHelper::hasAttribute(domeSettings, "curvature"))
					{
						curvature = XmlHelper::getAttributeValueAsReal(domeSettings, "curvature");
					}
					if (XmlHelper::hasAttribute(domeSettings, "tiling"))
					{
						curvature = XmlHelper::getAttributeValueAsReal(domeSettings, "tiling");
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


	void EnvironmentProcessor::processFogSettings(XERCES_CPP_NAMESPACE::DOMElement* fogElem)
	{
		///@todo process fog node
	}

}
