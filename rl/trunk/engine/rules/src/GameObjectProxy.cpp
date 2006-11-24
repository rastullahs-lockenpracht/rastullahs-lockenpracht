/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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
#include "GameObjectProxy.h"

#include "Actor.h"
#include "ActorManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"

namespace rl
{
    const Ogre::String GameObjectProxy::CLASS_ID = "classid";
    const Ogre::String GameObjectProxy::OBJECT_ID = "objectid";
    const Ogre::String GameObjectProxy::BASE_CLASS = "baseclass";

    GameObjectProxy::GameObjectProxy(const Ogre::String& classId, unsigned int goId)
        : mId(goId),
          mClassId(classId),
          mProperties(NULL)
    {
    }

    GameObject* GameObjectProxy::getGameObject()
    {
        if (mGameObject == NULL)
        {
            load();
        }
        return mGameObject;
    }

    const Ogre::String& GameObjectProxy::getClassId() const
    {
        return mClassId;
    }

    unsigned int GameObjectProxy::getId() const
    {
        return mId;
    }

    GameObjectState GameObjectProxy::getState() const
    {
        if (mGameObject != NULL)
        {
            return mGameObject->getState();
        }

        return GOS_UNLOADED;
    }

    void GameObjectProxy::load()
    {
        if (mGameObject == NULL)
        {
            mGameObject = GameObjectManager::getSingleton().createGameObject(this);
            if (mProperties != NULL)
            {
                mGameObject->setProperties(mProperties);
            }
        }
    }

    void GameObjectProxy::unload()
    {
        mProperties = mGameObject->getAllProperties();
        delete mGameObject;
        mGameObject = NULL;
    }
}

