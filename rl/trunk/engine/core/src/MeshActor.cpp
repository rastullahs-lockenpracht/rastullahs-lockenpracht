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

#include "MeshActor.h"

#include <OgreAnimation.h>
#include <OgreBone.h>
#include <OgreTagPoint.h>
#include <OgreException.h>

#include "CoreSubsystem.h"
#include "AnimationManager.h"
#include "World.h"

namespace rl {

    const String MeshActor::TYPENAME="MeshActor";

    MeshActor::MeshActor(const String& name, Entity* pEntity): Actor(name)
    {
        mEntity = pEntity;
        mSceneNode->attachObject(mEntity);
        mEntity->setUserObject(this);
    }

    MeshActor::MeshActor(const String& name, SceneNode* pParentSceneNode, Entity* pEntity)
        : Actor(name,pParentSceneNode)
    {
        mEntity = pEntity;
        mSceneNode->attachObject(mEntity);
        mEntity->setUserObject(this);
    }

    MeshActor::~MeshActor()
    {
        mSceneNode->detachObject(mEntity->getName()); 
        mSceneNode->getCreator()->removeEntity(mEntity);
    }

    void MeshActor::setSceneNode(SceneNode* node)
    {
        SceneNode* old = getSceneNode();
        if (old != 0 && mEntity != 0)
        {
            old->detachObject(mEntity);
        }
        Actor::setSceneNode(node);
        if (node != 0 && mEntity != 0)
        {
            node->attachObject(mEntity);
        }
    }

    void MeshActor::setScale( Real x, Real y, Real z )
    {
        mSceneNode->scale(x, y, z);
        mEntity->setNormaliseNormals(true);
    }



	RlAnimation* MeshActor::getAnimation(const String& animName)
	{
		try
		{
			AnimationState* animState = getEntity()->getAnimationState(animName);
			return AnimationManager::getSingleton().getAnimation(animState);
		}
		catch(Ogre::Exception&) 
		{
		}

		return 0;
	}



	RlAnimation* MeshActor::startAnimation(const String& animName, Real speed, unsigned int timesToPlay)
    {
		try
		{
			AnimationState* animState = getEntity()->getAnimationState(animName);
			return AnimationManager::getSingleton().addAnimation(animState);
		}
		catch(Ogre::Exception&) 
		{
		}

		return 0;
    }

    void MeshActor::stopAnimation(const String& animName)
    {
        try
		{
			AnimationState* animState = getEntity()->getAnimationState(animName);
			AnimationManager::getSingleton().removeAnimation(animState);
		}
		catch(Ogre::Exception&) 
		{ 
		}
    }

    void MeshActor::setCastShadows (bool enabled)
    {
        mEntity->setCastShadows(enabled);
    }

    bool MeshActor::getCastShadows (void) const
    {
        return mEntity->getCastShadows();
    }

	
    void MeshActor::attachActorToBone(Actor* actor, String name)
    {
        if( mEntity->hasSkeleton() )
        {
            if( actor->getSceneNode()->getAttachedObject(0) != 0 )
            {
                try
                {   
                    Bone* bone = mEntity->getSkeleton()->getBone( name );
                    /*TagPoint* tp =  mEntity->getSkeleton()->createTagPointOnBone( bone );
                    actor->getSceneNode()->getParent()->removeChild(actor->getName());
                    tp->addChild( actor->getSceneNode() );
                    tp->setParentEntity(mEntity); 
                    tp->addChild( actor->getSceneNode() );*/
                }
                catch( Ogre::Exception )
                {
                    // TODO Eigene Fehlerbehandlung
                }
            }
            else
            {
                CoreSubsystem::log(String(TYPENAME)
                    + " - " + this->getName()
                    + " : Befestigen am Knochen mit dem Name " + name
                    + " gescheitert. Kein Ogre-Movable gefunden.");
            }
        }
        else
        {
            CoreSubsystem::log(String(TYPENAME) + " - "
                + this->getName()
                + " : Befestigen am Knochen mit dem Namen " + name +
                " gescheitert. Kein Skelett vorhanden" );
        }
    }

    void MeshActor::attachActorToBone(Actor* actor, unsigned short id)
    {
        if( mEntity->hasSkeleton() )
        {
            if( actor->getSceneNode()->getAttachedObject(0) != 0 )
            {
                if( id < mEntity->getSkeleton()->getNumBones() )
                {
                    Bone* bone = mEntity->getSkeleton()->getBone( id );
                    /*TagPoint* tp =  mEntity->getSkeleton()->createTagPointOnBone( bone );
                    actor->getSceneNode()->getParent()->removeChild(actor->getName());
                    tp->addChild( actor->getSceneNode() );
                    tp->setParentEntity(mEntity); 
                    tp->addChild(  actor->getSceneNode() );*/
                }
                else
                {
                    CoreSubsystem::log(String(TYPENAME) + " - "
                        + this->getName()
                        + " : Befestigen am Knochen mit der Id "
                        + StringConverter::toString(id)
                        + " gescheitert. Bone nicht gefunden.");
                }
            }
            else
            {
                CoreSubsystem::log(String(TYPENAME) + " - "
                    + this->getName()
                    + " : Befestigen am Knochen mit der Id "
                    + StringConverter::toString(id)
                    + " gescheitert. Kein Ogre-Movable gefunden.");
            }
        }
        else
        {
            CoreSubsystem::log(String(TYPENAME) + " - "
                + this->getName()
                + " : Befestigen am Knochen mit der Id "
                + StringConverter::toString(id)
                + " gescheitert. Kein Skelett vorhanden.");
        }
    }

    void MeshActor::detachActorFromBone(Actor* actor )
    {
        if( mEntity->hasSkeleton() )
        {
            if( actor->getSceneNode()->getAttachedObject(0) != 0 )
            {
                try
                {
                    mEntity->detachObjectFromBone(
                        actor->getSceneNode()->getAttachedObject(0)->getName());
                    SceneManager* sceneMgr
                        = CoreSubsystem::getSingleton().getWorld()->getSceneManager();
                    sceneMgr->getRootSceneNode()->addChild(actor->getSceneNode());
                }
                catch( Ogre::Exception )
                {
                    // TODO Eigene Fehlerbehandlung
                }
            }
        }
    }

    Entity* MeshActor::getEntity(void)
    {
        return mEntity;
    }

    const String& MeshActor::getTypeName(){
        return TYPENAME;
    }

    Vector3 MeshActor::getExtent()
    {
        Vector3 rval = Vector3::ZERO;

        const AxisAlignedBox &aab = mEntity->getBoundingBox();
        Vector3 x = aab.getMaximum() - Vector3::UNIT_SCALE;
        Vector3 n = aab.getMinimum() + Vector3::UNIT_SCALE;
        Vector3 s = getSceneNode()->getScale();
        rval.x = (x.x - n.x) * s.x;
        rval.y = (x.y - n.y) * s.y;
        rval.z = (x.z - n.z) * s.z;

        return rval;
    }

    Vector3 MeshActor::getCenter()
    {
        return Vector3(0, 0, getHeight() / 2.0);
    }
    
    Real MeshActor::getRadius()
    {
        Vector3 extent = getExtent();
        return std::max(extent.x, extent.z) / 2.0;
    }
    
    Real MeshActor::getHeight()
    {
        return getExtent().y;
    }

}
