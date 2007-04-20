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

#include "ActorControlledObject.h"

#include <OgreSceneNode.h>

#include "Actor.h"

using namespace Ogre;

namespace rl {
    ActorControlledObject::ActorControlledObject()
        :   mMovableObject(0)
    {
    }

    ActorControlledObject::ActorControlledObject(Ogre::MovableObject* mo)
        :   mMovableObject(mo)
    {
    }
    
    ActorControlledObject::~ActorControlledObject()
    {
    }
    
    void ActorControlledObject::_setActor(Actor* actor)
    {
        if (mMovableObject != NULL)
        {
            mMovableObject->setUserObject(actor);
        }
    }
    
    Actor* ActorControlledObject::getActor() const
    {
        if (mMovableObject != NULL)
        {
            return static_cast<Actor*>(mMovableObject->getUserObject());
        }
        return NULL;
    }
    
    MovableObject* ActorControlledObject::getMovableObject() const
    {
        return mMovableObject;
    }
    
    void ActorControlledObject::_attachSceneNode(Ogre::SceneNode* node)
    {
        if (mMovableObject != NULL)
        {
            node->attachObject(mMovableObject);
        }
    }
    
    void ActorControlledObject::_detachSceneNode(Ogre::SceneNode* node)
    {
        if (mMovableObject != NULL)
        {
            node->detachObject(mMovableObject);    
        }        
    }

    bool ActorControlledObject::isAttached() const
    {
        if (mMovableObject != NULL)
        {
            return mMovableObject->isAttached();
        }
        return false;
    }

    bool ActorControlledObject::isAttachedToNode(Ogre::SceneNode* node) const
    {
        try
        {
            node->getAttachedObject( mMovableObject->getName() );
            return true;
        }
        catch(...) { return false; }
    }

	bool ActorControlledObject::isMeshObject() const
	{
		return false;
	}

	void ActorControlledObject::_update()
	{
	}
}
