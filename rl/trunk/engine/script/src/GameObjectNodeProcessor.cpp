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
#include "GameObjectNodeProcessor.h"

#include <xercesc/dom/DOM.hpp>

#include "GameObject.h"
#include "GameObjectManager.h"
#include "PropertyReader.h"
#include "XmlHelper.h"

using namespace Ogre;
using namespace XERCES_CPP_NAMESPACE;

namespace rl
{
    bool GameObjectNodeProcessor::processNode(XERCES_CPP_NAMESPACE::DOMElement* nodeElem, bool loadGameObjects)
    {
        if (!XmlHelper::hasNodeName(nodeElem, "gameobject"))
        {
            return false;
        }
    
        LOG_DEBUG(Logger::RULES, 
            "Processing game object node " 
                + XmlHelper::getAttributeValueAsStdString(nodeElem, "name"));
        
        Ogre::String classname = XmlHelper::getAttributeValueAsStdString(nodeElem, "class");
        
        unsigned int goid = GameObject::NO_OBJECT_ID;

        if (XmlHelper::hasAttribute(nodeElem, "id"))
        {
            goid = XmlHelper::getAttributeValueAsInteger(nodeElem, "id");
        }

        GameObject* go = GameObjectManager::getSingleton().createGameObject(classname, goid);

        DOMElement* posElem = XmlHelper::getChildNamed(nodeElem, "position");
        if (posElem != NULL)
        {
            Vector3 pos = processVector3(posElem);
            go->setPosition(pos);
        }

        DOMElement* oriElem = XmlHelper::getChildNamed(nodeElem, "rotation");
        if (oriElem != NULL)
        {
            Quaternion ori = processQuaternion(oriElem);
            go->setOrientation(ori);
        }

        DOMNodeList* goElChildNodes = nodeElem->getChildNodes();
        for (XMLSize_t idx = 0; idx < goElChildNodes->getLength(); idx++)
        {
            DOMNode* cur = goElChildNodes->item(idx);
            if (cur->getNodeType() == DOMNode::ENTITY_NODE 
                && XmlHelper::hasNodeName(cur, "property"))
            {
                PropertyEntry propEntry = getXmlPropertyReader()->processProperty(static_cast<DOMElement*>(cur));
                if (propEntry.first != "")
                {
                    go->setProperty(propEntry.first, propEntry.second);
                }
            }
        }

		GameObjectState state = GOS_LOADED;
		if (XmlHelper::hasAttribute(nodeElem, "state"))
		{
			Ogre::String stateStr = XmlHelper::getAttributeValueAsStdString(nodeElem, "state");
			if (stateStr == "LOADED")
			{
				state = GOS_LOADED;
			}
			else if (stateStr == "HELD")
			{
				state = GOS_HELD;
			}
			else if (stateStr == "IN_POSSESSION")
			{
				state = GOS_IN_POSSESSION;
			}
			else if (stateStr == "IN_SCENE")
			{
				state = GOS_IN_SCENE;
			}
			else if (stateStr == "READY")
			{
				state = GOS_READY;
			}
		}
		go->setState(state);

        return true;
    }
}
