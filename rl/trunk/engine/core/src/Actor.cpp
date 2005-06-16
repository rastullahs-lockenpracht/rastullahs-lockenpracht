/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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
#include "GameEventManager.h"
#include "PhysicalThing.h"
#include "Exception.h"
#include "ActorControlledObject.h"
#include "MeshObject.h"

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
        mSceneNode(0),
		mHighlighted(false),
        mAttachedToBone(false)
	{
        if( mActorControlledObject != NULL )
            mActorControlledObject->_setActor(this);

		if( mPhysicalThing != NULL )
			mPhysicalThing->_setActor(this);

        setQueryMask( QGF_DEFAULT );
    }

    Ogre::String Actor::DEFAULT_SLOT_NAME = "SLOT_DEFAULT";

    Actor::~Actor()
    {
        // Alle möglichen Area-Verknüpfungen entfernen
        GameEventManager::getSingleton().removeAllAreas( this );

        removeFromScene();
        
        if (mActorControlledObject)
        {
            mActorControlledObject->_setActor(0);
        }

        /// @todo Physikverknüpfung entfernen
        /// @todo Highlightmaterial entfernen
    }

    void Actor::setGameObject(Ogre::UserDefinedObject* uo)
    {
        mGameObject = uo;
    }

	Ogre::UserDefinedObject* Actor::getGameObject() const
	{
		return mGameObject;
	}

    PhysicalThing* Actor::getPhysicalThing() const
    {
        return mPhysicalThing;
    }

    void Actor::setPhysicalThing( PhysicalThing* pt ) 
    {
       
    }

    ActorControlledObject* Actor::getControlledObject() const
    {
        return mActorControlledObject;
    }

    void Actor::setControlledObject( ActorControlledObject* act ) 
    {
       if( act->getActor() != NULL )
           Throw(IllegalStateException, "Das anzufügende Objekt ist bereits an einem Aktor befestigt.");
       if( this->getControlledObject() != NULL )
           Throw(IllegalStateException, "Es ist bereits ein Objekt an diesem Aktor befestigt.");

       mActorControlledObject = act;
    }

    const String& Actor::getName() const
    {
        return mName;
    }

    unsigned long Actor::getQueryMask() const
    {
        return mActorControlledObject ?
            mActorControlledObject->getMovableObject()->getQueryFlags() : QGF_NONE;
    }

    void Actor::setQueryMask( unsigned long mask )
    {
        if( mActorControlledObject )
            mActorControlledObject->getMovableObject()->setQueryFlags( mask );
    }

    void Actor::addQueryFlag( unsigned long flag  )
    {
        setQueryMask(  getQueryMask() | flag );
    }

    void Actor::removeQueryFlag( unsigned long flag )
    {
        setQueryMask(  getQueryMask() &~ flag );
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
        _update();
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
        _update();
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
        _update();
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
        _update();
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
        _update();
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
        _update();
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
        _update();
    }

    const Vector3& Actor::getPosition(void) const
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

    const Quaternion& Actor::getOrientation(void) const
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

    const Vector3& Actor::getWorldPosition(void) const
    {
        if (mSceneNode)
        {
            return mSceneNode->getWorldPosition();
        }
        else
        {
            Throw(IllegalStateException, "Actor is not placed into scene.");
        }
    }

    const Quaternion& Actor::getWorldOrientation(void) const
    {
        if (mSceneNode)
        {
            return mSceneNode->getWorldOrientation();
        }
        else
        {
            Throw(IllegalStateException, "Actor is not placed into scene.");
        }
    }

    void Actor::placeIntoScene(const Vector3& position,
		const Quaternion& orientation, const std::string& odeBone)
    {
            SceneManager* mgr = CoreSubsystem::getSingletonPtr()->
                getWorld()->getSceneManager();
            _placeIntoScene(mgr->getRootSceneNode(), position, orientation, odeBone);
    }        

    void Actor::placeIntoScene(Real px, Real py, Real pz,
        Real ow, Real ox, Real oy, Real oz, const std::string& odeBone)
    {
        placeIntoScene(Vector3(px, py, pz), Quaternion(ow, ox, oy, oz), odeBone);
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
            mSceneNode = NULL;
        }
    }

    void Actor::attachToSlot( 
        Actor* actor,
        const Ogre::String& slot,
        const Ogre::String& childSlot,
        const Ogre::Vector3& offsetPosition,
        const Ogre::Quaternion& offsetOrientation
        )
    {
        doAttach(actor, slot, childSlot, offsetPosition, offsetOrientation );
        // Erst danach Parent/Child wirklich zuweisen, falls es eine Exception gibt.
        actor->mParent = this;
        mChilds.insert(actor);
    }

    void Actor::attachToSlot( 
        Actor* actor,
        const Ogre::String& slot,
        const Ogre::String& childSlot,            
        const Ogre::Vector3& offsetPosition,
        const Ogre::Vector3& offsetAxis,
        const Ogre::Radian& offsetRotation )
    {
        attachToSlot( actor, slot, childSlot, 
            offsetPosition, Quaternion(offsetRotation,offsetAxis) );
    }

    void Actor::attach( 
        Actor* actor,
        const Ogre::String& childSlot,
        const Ogre::Vector3& offsetPosition,
        const Ogre::Quaternion& offsetOrientation
        )
    {
        attachToSlot( actor, DEFAULT_SLOT_NAME, childSlot, 
            offsetPosition, offsetOrientation );
    }

    void Actor::attach( 
        Actor* actor,
        const Ogre::String& childSlot,            
        const Ogre::Vector3& offsetPosition,
        const Ogre::Vector3& offsetAxis,
        const Ogre::Radian& offsetRotation)
    {
        attachToSlot( actor, DEFAULT_SLOT_NAME, childSlot, 
            offsetPosition, Quaternion(offsetRotation,offsetAxis) );
    }

    void Actor::detach(Actor* actor)
    {
        if( mChilds.find(actor) == mChilds.end() )
        {
            Throw(InvalidArgumentException, "Actor is not a child of this.");
        }
        else
        {
            doDetach(actor);
            actor->mParent = NULL;
            mChilds.erase(actor);
        }
    }

    void Actor::doAttach(
        Actor* actor,
        const Ogre::String& slot, 
        const Ogre::String& childSlot, 
        const Ogre::Vector3& offsetPosition,
        const Ogre::Quaternion& offsetOrientation ) 
    {
        if( actor == NULL )
            Throw(NullPointerException, "Der anzufügende Aktor darf nicht NULL sein." );

        // Verschiebung durch den Child-Slot berechnen
        // Ist es ein nicht Standard-Slot && Kontrolliert der Aktor ein Objekt && Ist dieses ein Mesh
        if( childSlot.compare(DEFAULT_SLOT_NAME) != 0 &&
            actor->getControlledObject() != NULL && 
            actor->getControlledObject()->isMeshObject() )
        {
            Entity* ent = dynamic_cast<MeshObject*>(actor->getControlledObject())->getEntity();

            // Braucht ein Skelett
            if( !ent->hasSkeleton() )
                Throw(InvalidArgumentException, 
                    "Das kontrollierte MeshObject des ChildAktor hat kein Skeleton." );

            // Der Slot muss existieren
            try
            {
                Bone* bone = ent->getSkeleton()->getBone( childSlot );
                // @todo Berechnen des extra-Offsets
            }
            catch (Ogre::Exception) {
                Throw(InvalidArgumentException, 
                    "Der geforderte Slot '"+childSlot+"' am ChildAktor existiert nicht." );
            }
        }

        // Das wirkliche Anfügen
        // Ist es ein nicht Standard-Slot && Kontrolliert der Aktor ein Objekt && Ist dieses ein Mesh
		if( slot.compare(DEFAULT_SLOT_NAME) != 0 && 
            getControlledObject() != NULL && 
            getControlledObject()->isMeshObject() )
		{
			MovableObject* movObj = actor->getControlledObject()->getMovableObject();
            Entity* ent = dynamic_cast<MeshObject*>(getControlledObject())->getEntity();

            // Braucht ein Skelett
            if( !ent->hasSkeleton() )
                Throw(InvalidArgumentException, "Das kontrollierte MeshObject hat kein Skeleton." );

            // Der Slot muss existieren
            try
            {
                ent->getSkeleton()->getBone( slot );
            }
            catch (Ogre::Exception) {
                Throw(InvalidArgumentException, "Der geforderte Slot '"+slot+"' existiert nicht." );
            }
                
            // Am Bone befestigen
			ent->attachObjectToBone( slot, movObj, offsetOrientation, offsetPosition );

			return;
		}
        else
		{
			return; 
            // @todo: wenn this kein MeshObjekt ist, trotzdem irgendwie zusammenfügen
		}

    }         

    void Actor::setScale( Ogre::Real sx, Ogre::Real sy, Ogre::Real sz )
    {
        Node* node = getControlledObject()->getMovableObject()->getParentNode();

        if( node )
        {
            Vector3 vec = Vector3(sx,sy,sz);
            node->setScale( vec );

            // Falls es sich um ein Mesh handelt ...
            if( getControlledObject()->isMeshObject() )
            {
                MeshObject* meshObj = dynamic_cast<MeshObject*>( getControlledObject() );

                // ... und größer/kleiner als normal skaliert wird ...
                if( vec != Vector3(1,1,1) )
                    // ... müssen die Normalen neu berechnet werden.
                    meshObj->getEntity()->setNormaliseNormals( true );
                else
                    meshObj->getEntity()->setNormaliseNormals( false );
            }
        }

        
    }

    void Actor::doDetach(Actor* actor)
    {
		if( getControlledObject()->isMeshObject() )
		{
			MovableObject* mo = actor->getControlledObject()->getMovableObject();
			dynamic_cast<MeshObject*>(getControlledObject())->getEntity()->detachObjectFromBone(mo);
			return;
		}
	}
    
    void Actor::placeChildsIntoScene(const Vector3& position,
            const Quaternion& orientation)
    {
    }
        
    SceneNode* Actor::_getSceneNode() const
    {
        return mSceneNode;
    }

    MovableObject* Actor::_getMovableObject() const
    {
        return mActorControlledObject ? 
            mActorControlledObject->getMovableObject() : 0;
    }
    
    void Actor::_update()
    {
        if (mSceneNode)
            mSceneNode->_update(true, true);

		if (mPhysicalThing)
			mPhysicalThing->_update();
			
		if (mActorControlledObject)
		    mActorControlledObject->_update();
    }
    
    void Actor::_placeIntoScene(SceneNode* parent, const Vector3& position,
		const Quaternion& orientation, const std::string& odeBone)
    {
        ///@todo child actors berücksichtigen

        // SceneNode erzeugen, falls nicht schon einer vorhanden
        if (parent && !mSceneNode)
        {
            mSceneNode = parent->createChildSceneNode(mName, position, orientation);
            // Falls ein MovableObject vorhanden, diesen attachen
            if (mActorControlledObject)
            {
                mActorControlledObject->_attachSceneNode(mSceneNode);
            }
            if (mPhysicalThing && mActorControlledObject )
            {
				if (odeBone.compare("") == 0 || !mActorControlledObject->isMeshObject())
					mPhysicalThing->_attachToSceneNode(mSceneNode);
				else
					mPhysicalThing->_attachToBone(
						dynamic_cast<MeshObject*>(mActorControlledObject), 
						odeBone);
            }
        }
        else if (!parent)
        {
            removeFromScene();
        }
        
        _update();
    }

	void Actor::setHighlighted(bool highlight)
	{
		if (highlight == mHighlighted)
			return;

        getControlledObject()->setHighlighted( highlight );		

		mHighlighted = highlight;
	}
	
    bool Actor::isHighlighted()const
    {
        return mHighlighted;
    }

    Actor* Actor::getChildByName(const String& name ) const
    {
        std::set<Actor*>::const_iterator iter =  mChilds.begin();

        for( iter; iter != mChilds.end(); iter++ )
        {
            Actor* actor = *iter;
            
            if( actor->getName().compare( name ) == 0 )
                return actor;
            if( ( actor = actor->getChildByName(name) ) != NULL )
                return actor;
        }

        return NULL;
    }
}
