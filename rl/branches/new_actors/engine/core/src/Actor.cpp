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
        mActorControlledObject->setActor(this);
    }

    Actor::~Actor()
    {
        removeFromScene();
        if (mActorControlledObject)
        {
            mActorControlledObject->setActor(0);
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
        ///@todo child actors berücksichtigen

        // SceneNode erzeugen, falls nicht schon einer vorhanden
        if (!mSceneNode)
        {
            SceneManager* mgr = CoreSubsystem::getSingletonPtr()->
                getWorld()->getSceneManager();
            mSceneNode = mgr->createSceneNode();
            mgr->getRootSceneNode()->addChild(mSceneNode);
        }
        else
        {
            removeFromScene();
        }

        // Falls ein MovableObject vorhanden, diesen an den SceneNode bappen
        if (mActorControlledObject)
        {
            mSceneNode->attachObject(
                mActorControlledObject->getMovableObject());
        }

        // SceneNode platzieren
        mSceneNode->translate(position, Node::TS_PARENT);
        mSceneNode->rotate(orientation, Node::TS_PARENT);
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
        }
        // SceneNode wird erstmal nicht gelöscht.
        // Laut Ogre-API nicht empfohlen. Lieber beim
        // Map-Wechsel abräumen.
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

}
