#include "MeshActor.h"

#include <OgreAnimation.h>
#include <OgreBone.h>
#include <OgreTagPoint.h>

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

    void MeshActor::startAnimation(const String& anim)
    {
        AnimationState* animState = getEntity()->getAnimationState(anim);
        AnimationManager::getSingleton().addAnimation(animState);
    }

    void MeshActor::stopAnimation(const String& anim)
    {
        AnimationState* animState = getEntity()->getAnimationState(anim);
        AnimationManager::getSingleton().removeAnimation(animState);
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

}
