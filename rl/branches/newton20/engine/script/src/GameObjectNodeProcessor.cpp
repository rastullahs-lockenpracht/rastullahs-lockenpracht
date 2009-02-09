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

#include "GameObjectNodeProcessor.h"

#include <xercesc/dom/DOM.hpp>

#include "GameObject.h"
#include "GameObjectManager.h"

using namespace Ogre;
using namespace XERCES_CPP_NAMESPACE;

namespace rl
{
    bool GameObjectNodeProcessor::processNode(XERCES_CPP_NAMESPACE::DOMElement* nodeElem, bool loadGameObjects)
    {
        if (!hasNodeName(nodeElem, "gameobject"))
        {
            return false;
        }

        LOG_DEBUG(Logger::RULES,
            "Processing game object node "
                + getAttributeValueAsStdString(nodeElem, "name"));

        Ogre::String classname = getAttributeValueAsStdString(nodeElem, "class");

        unsigned int goid = GameObject::NO_OBJECT_ID;

        if (hasAttribute(nodeElem, "id"))
        {
            goid = getAttributeValueAsInteger(nodeElem, "id");
        }

        GameObject* go = GameObjectManager::getSingleton().getGameObject(goid);
        if (!go)
        {
            go = GameObjectManager::getSingleton().createGameObject(classname, goid);
            
            if (!go) // Error creating the GameObject
            {
                LOG_ERROR(
                    "MapLoader", 
                    "Could not create a game object with classid '" 
                    + classname + "' and id '" + StringConverter::toString(goid) + "'");
                return true;
            }

            DOMElement* posElem = getChildNamed(nodeElem, "position");
            if (posElem != NULL)
            {
                Vector3 pos = processVector3(posElem);
                go->setPosition(pos);
            }

            DOMElement* oriElem = getChildNamed(nodeElem, "rotation");
            if (oriElem != NULL)
            {
                Quaternion ori = processQuaternion(oriElem);
                go->setOrientation(ori);
            }

            DOMNodeList* goElChildNodes = nodeElem->getChildNodes();
            for (XMLSize_t idx = 0; idx < goElChildNodes->getLength(); idx++)
            {
                DOMNode* cur = goElChildNodes->item(idx);
                if (cur->getNodeType() == DOMNode::ELEMENT_NODE
                    && hasNodeName(cur, "property"))
                {
                    PropertyEntry propEntry = processProperty(static_cast<DOMElement*>(cur));
                    if (propEntry.first != "")
                    {
                        go->setProperty(propEntry.first, propEntry.second);
                    }
                }
            } 		    
        }

        GameObjectState state = GOS_IN_SCENE;
        if (hasAttribute(nodeElem, "state"))
	    {
		    Ogre::String stateStr = getAttributeValueAsStdString(nodeElem, "state");
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
