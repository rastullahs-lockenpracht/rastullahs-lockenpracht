/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the Perl Artistic License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  Perl Artistic License for more details.
*
*  You should have received a copy of the Perl Artistic License
*  along with this program; if not you can get it here
*  http://www.perldoc.com/perl5.6/Artistic.html.
*/

#include "Actor.h"

#include "CoreSubsystem.h"
#include "World.h"
#include "PhysicsManager.h"
#include "PhysicalThing.h"
#include "Exception.h"
#include "ActorControlledObject.h"

using namespace Ogre;

namespace rl {

    Actor::Actor(const String& name,
        ActorControlledObject* aco,
        PhysicalThing* pt,
        UserDefinedObject* go)
        :   mName(name),
        mPhysicalThing(pt),
        mGameObject(go),
        mActorControlledObject(aco),
        mParent(0),
        mChilds(),
        mSceneNode(0)
    {
        mActorControlledObject->_setActor(this);

		if( mPhysicalThing != 0 )
			mPhysicalThing->_setActor(this);
    }

    Actor::~Actor()
    {
        removeFromScene();
        if (mActorControlledObject)
        {
            mActorControlledObject->_setActor(0);
        }
        // Eventuell mal SceneNode löschen.
    }

    void Actor::setGameObject(Ogre::UserDefinedObject* uo)
    {
        mGameObject = uo;
    }

    PhysicalThing* Actor::getPhysicalThing()
    {
        return mPhysicalThing;
    }

    ActorControlledObject* Actor::getControlledObject()
    {
        return mActorControlledObject;
    }
    
    const String& Actor::getName()
    {
        return mName;
    }

    void Actor::setOrientation(const Quaternion& orientation)
    {
        if (mSceneNode)
        {
            mSceneNode->setOrientation(orientation);
        }
        else
        {
            Throw(IllegalStateException, "Actor is not placed into scene.");
        }
    }

    void Actor::setPosition(const Vector3& position)
    {
        if (mSceneNode)
        {
            mSceneNode->setPosition(position);
        }
        else
        {
            Throw(IllegalStateException, "Actor is not placed into scene.");
        }
    }
    
    void Actor::setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z)
    {
        setPosition(Vector3(x, y, z));
    }

    void Actor::translate(const Vector3& translation, Node::TransformSpace ts)
    {
        if (mSceneNode)
        {
            mSceneNode->translate(translation, ts);
        }
        else
        {
            Throw(IllegalStateException, "Actor is not placed into scene.");
        }
    }

    void Actor::roll(Real angle)
    {
        if (mSceneNode)
        {
            mSceneNode->rotate(Vector3::UNIT_Z, Degree(angle));
        }
        else
        {
            Throw(IllegalStateException, "Actor is not placed into scene.");
        }
    }

    void Actor::pitch(Real angle)
    {
        if (mSceneNode)
        {
            mSceneNode->rotate(Vector3::UNIT_X, Degree(angle));
        }
        else
        {
            Throw(IllegalStateException, "Actor is not placed into scene.");
        }
    }

    void Actor::yaw(Real angle)
    {
        if (mSceneNode)
        {
            mSceneNode->rotate(Vector3::UNIT_Y, Degree(angle));
        }
        else
        {
            Throw(IllegalStateException, "Actor is not placed into scene.");
        }
    }

    void Actor::rotate(const Quaternion& orientation)
    {
        if (mSceneNode)
        {
            mSceneNode->setOrientation(orientation);
        }
        else
        {
            Throw(IllegalStateException, "Actor is not placed into scene.");
        }

    }

    const Vector3& Actor::getPosition(void)
    {
        if (mSceneNode)
        {
            return mSceneNode->getPosition();
        }
        else
        {
            Throw(IllegalStateException, "Actor is not placed into scene.");
        }
    }

    const Quaternion& Actor::getOrientation(void)
    {
        if (mSceneNode)
        {
            return mSceneNode->getOrientation();
        }
        else
        {
            Throw(IllegalStateException, "Actor is not placed into scene.");
        }
    }

    void Actor::placeIntoScene(const Vector3& position,
        const Quaternion& orientation)
    {
            SceneManager* mgr = CoreSubsystem::getSingletonPtr()->
                getWorld()->getSceneManager();
            _placeIntoScene(mgr->getRootSceneNode(), position, orientation);
    }        

    void Actor::placeIntoScene(Real px, Real py, Real pz,
        Real ow, Real ox, Real oy, Real oz)
    {
        placeIntoScene(Vector3(px, py, pz), Quaternion(ow, ox, oy, oz));
    }
    
    void Actor::removeFromScene()
    {
        ///@todo child actors berücksichtigen

        if (mActorControlledObject && mSceneNode)
        {
            mSceneNode->detachObject(
                mActorControlledObject->getMovableObject());
            SceneManager* mgr = CoreSubsystem::getSingletonPtr()->
                getWorld()->getSceneManager();
            ///@todo performance
            mgr->destroySceneNode(mSceneNode->getName());
            mSceneNode = 0;
        }
    }

    void Actor::attach(const String& slot, Actor* actor,
        const String& childSlot)
    {
        if (actor->mParent)
        {
            Throw(InvalidArgumentException,
                "Actor already attached to another actor.");
        }
        else
        {
            doAttach(slot, actor, childSlot);
            // Erst danach Parent/Child wirklich zuweisen, falls es ne
            // Exception gibt.
            actor->mParent = this;
            mChilds.insert(actor);
        }
    }

    void Actor::detach(Actor* actor)
    {
        if (mChilds.find(actor) == mChilds.end())
        {
            Throw(InvalidArgumentException, "Actor is not a child of this.");
        }
        else
        {
            doDetach(actor);
            actor->mParent = 0;
            mChilds.erase(actor);
        }
    }

    void Actor::doAttach(const String& slot, Actor* actor,
            const String& childSlot)
    {
    }            

    void Actor::doDetach(Actor* actor)
    {
    }
    
    void Actor::placeChildsIntoScene(const Vector3& position,
            const Quaternion& orientation)
    {
    }
        
    SceneNode* Actor::_getSceneNode()
    {
        return mSceneNode;
    }

    MovableObject* Actor::_getMovableObject()
    {
        return mActorControlledObject ? 
            mActorControlledObject->getMovableObject() : 0;
    }
    
    void Actor::_update()
    {
        if (mSceneNode) mSceneNode->_update(true, true);

		if( mPhysicalThing != 0 )
			mPhysicalThing->_update();
    }
    
    void Actor::_placeIntoScene(SceneNode* parent, const Vector3& position,
        const Quaternion& orientation)
    {
        ///@todo child actors berücksichtigen

        // SceneNode erzeugen, falls nicht schon einer vorhanden
        if (parent && !mSceneNode)
        {
            mSceneNode = parent->createChildSceneNode(mName, position, orientation);
            // Falls ein MovableObject vorhanden, diesen attachen
            if (mActorControlledObject)
            {
                mSceneNode->attachObject(
                    mActorControlledObject->getMovableObject());
            }
        }
        else if (!parent)
        {
            removeFromScene();
        }
    }        
}
