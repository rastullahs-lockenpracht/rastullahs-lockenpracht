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

#include "ActorControlledObject.h"
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
        if (mMovableObject != 0)
        {
            mMovableObject->setUserObject(actor);
        }
    }
    
    Actor* ActorControlledObject::getActor()
    {
        if (mMovableObject != 0)
        {
            UserDefinedObject* m = mMovableObject->getUserObject();
            return m != 0 ? reinterpret_cast<Actor*>(m) : 0;
        }
        return 0;
    }
    
    MovableObject* ActorControlledObject::getMovableObject()
    {
        return mMovableObject;
    }
    
    void ActorControlledObject::_attachSceneNode(Ogre::SceneNode* node)
    {
        if (mMovableObject != 0)
        {
            node->attachObject(mMovableObject);
        }
    }
    
    void ActorControlledObject::_detachSceneNode(Ogre::SceneNode* node)
    {
        if (mMovableObject != 0)
        {
            node->detachObject(mMovableObject);    
        }        
    }

    bool ActorControlledObject::isAttached() const
    {
        if (mMovableObject != 0)
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

	bool ActorControlledObject::isMeshObject()
	{
		return false;
	}

	void ActorControlledObject::_update()
	{
	}
}
