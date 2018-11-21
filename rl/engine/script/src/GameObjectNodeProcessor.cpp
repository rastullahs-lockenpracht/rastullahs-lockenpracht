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

#include "GameObject.h"
#include "GameObjectManager.h"

using namespace Ogre;

namespace rl
{
    bool GameObjectNodeProcessor::processNode(
        const tinyxml2::XMLElement* nodeElem, const Ogre::String& resourceGroup, bool loadGameObjects)
    {
        if (!hasNodeName(nodeElem, "gameobject"))
        {
            return false;
        }

        LOG_DEBUG(Logger::RULES, "Processing game object node " + getAttributeValueAsStdString(nodeElem, "name"));

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
                LOG_ERROR("MapLoader",
                    "Could not create a game object with classid '" + classname + "' and id '"
                        + StringConverter::toString(goid) + "'");
                return true;
            }

            const tinyxml2::XMLElement* posElem = getChildNamed(nodeElem, "position");
            if (posElem != NULL)
            {
                Vector3 pos = processVector3(posElem);
                go->setPosition(pos);
            }

            const tinyxml2::XMLElement* oriElem = getChildNamed(nodeElem, "rotation");
            if (oriElem != NULL)
            {
                Quaternion ori = processQuaternion(oriElem);
                go->setOrientation(ori);
            }

            for (const tinyxml2::XMLNode* cur = nodeElem->FirstChild(); cur; cur = cur->NextSibling())
            {
                const tinyxml2::XMLElement* elem = cur->ToElement();
                if (elem && hasNodeName(elem, "property"))
                {
                    PropertyEntry propEntry = processProperty(elem);
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
