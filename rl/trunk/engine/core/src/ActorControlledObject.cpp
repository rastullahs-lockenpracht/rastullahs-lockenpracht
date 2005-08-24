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
    
    ActorControlledObject::~ActorControlledObject()
    {
    }
    
    void ActorControlledObject::_setActor(Actor* actor)
    {
        mMovableObject->setUserObject(actor);
    }
    
    Actor* ActorControlledObject::getActor()
    {
        UserDefinedObject* m = mMovableObject->getUserObject();
        return m != 0 ? reinterpret_cast<Actor*>(m) : 0;
    }
    
    MovableObject* ActorControlledObject::getMovableObject()
    {
        return mMovableObject;
    }
    
    void ActorControlledObject::_attachSceneNode(Ogre::SceneNode* node)
    {
        node->attachObject(mMovableObject);
    }
    
    void ActorControlledObject::_detachSceneNode(Ogre::SceneNode* node)
    {
        node->detachObject(mMovableObject);    
        
    }

    bool ActorControlledObject::isAttached() const
    {
        return mMovableObject->isAttached();
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
