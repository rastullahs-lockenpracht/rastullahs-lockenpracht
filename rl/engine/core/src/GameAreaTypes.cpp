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

#include "GameAreaTypes.h"

#include <boost/smart_ptr.hpp>

#include "Actor.h"
#include "CoreSubsystem.h"
#include "Exception.h"
#include "PhysicalThing.h"
#include "PhysicsManager.h"
#include "World.h"

using namespace Ogre;

namespace rl
{
    GameAreaType::~GameAreaType()
    {
    }

    void GameAreaType::addQueryFlag(unsigned long flag)
    {
        setQueryMask(getQueryMask() | flag);
    }

    void GameAreaType::removeQueryFlag(unsigned long flag)
    {
        setQueryMask(getQueryMask() & ~flag);
    }
}

namespace rl
{
    /*
        GameSphereAreaType::GameSphereAreaType(Vector3 center, Real radius, unsigned long mask)
        {
            mSphereQuery = CoreSubsystem::getSingleton().getWorld()->
                getSceneManager()->createSphereQuery( Sphere(center,radius), mask );

            // Keine Welt-Geometrie erw¸nscht, nur Movables
            mSphereQuery->setWorldFragmentType( SceneQuery::WFT_NONE );
        }

        GameSphereAreaType::~GameSphereAreaType()
        {
            mSphereQuery->clearResults();
            CoreSubsystem::getSingleton().getWorld()->getSceneManager()->destroyQuery( mSphereQuery );
        }

        ActorMap GameSphereAreaType::performQuery(  )
        {
            SceneQueryResult rs = mSphereQuery->execute();
            SceneQueryResultMovableList movList = rs.movables;

            ActorMap retMap;

            // Durch die Ergebnis Liste iterieren
            SceneQueryResultMovableList::iterator it;
            for (it = movList.begin(); it != movList.end(); ++it)
            {
                MovableObject* mov = *it;
                if (mov->getUserAny())
                {
                    // Zur Zeit sind die einzigen an Movables gekn¸pfte Objekte Actoren
                    Actor* act = any_cast<Actor*>(mov->getUserAny());
                    retMap.insert(ActorPair(act->getName(),act));
                }
            }

            return retMap;
        }

        unsigned long GameSphereAreaType::getQueryMask() const
        {
            return mSphereQuery->getQueryMask();
        }

        void GameSphereAreaType::setQueryMask( unsigned long mask )
        {
            mSphereQuery->setQueryMask( mask );
        }

        void GameSphereAreaType::setQueryPosition( const Ogre::Vector3& vec )
        {
            // Wurde es bewegt?
            if( vec != mSphereQuery->getSphere().getCenter( ) )
                // Kugel muss neu erzeugt werden :(
                mSphereQuery->setSphere( Sphere(vec,mSphereQuery->getSphere().getRadius()) );
        }

        Ogre::Vector3 GameSphereAreaType::getQueryPosition() const
        {
            return mSphereQuery->getSphere().getCenter( );
        }
    */

    GameNewtonBodyAreaType::GameNewtonBodyAreaType()
        : mBody(NULL)
        , mTransitionDistance(0)
    {
    }

    GameNewtonBodyAreaType::~GameNewtonBodyAreaType()
    {
        if (mBody != NULL)
            delete mBody;
        mBody = NULL;
    }

    unsigned long GameNewtonBodyAreaType::getQueryMask() const
    {
        return mQueryMask;
    }

    void GameNewtonBodyAreaType::setQueryMask(unsigned long mask)
    {
        mQueryMask = mask;
    }

    Quaternion GameNewtonBodyAreaType::getQueryOrientation() const
    {
        Vector3 pos;
        Quaternion orient;
        mBody->getPositionOrientation(pos, orient);
        return orient;
    }

    Vector3 GameNewtonBodyAreaType::getQueryPosition() const
    {
        Vector3 pos;
        Quaternion orient;
        mBody->getPositionOrientation(pos, orient);
        return pos;
    }

    void GameNewtonBodyAreaType::setQueryPosition(const Vector3& pos)
    {
        Quaternion orient;
        Vector3 old_pos;
        mBody->getPositionOrientation(old_pos, orient);
        mBody->setPositionOrientation(pos, orient);
    }

    void GameNewtonBodyAreaType::setQueryOrientation(const Quaternion& orient)
    {
        Quaternion old_orient;
        Vector3 pos;
        mBody->getPositionOrientation(pos, old_orient);
        mBody->setPositionOrientation(pos, orient);
    }

    void GameNewtonBodyAreaType::foundCollision(Actor* actor)
    {
        if (actor)
        {
            unsigned long flags = actor->getQueryFlags();
            if (flags & mQueryMask)
                mFoundActors[actor->getName()] = actor;
        }
    }

    void GameNewtonBodyAreaType::resetFoundCollisions()
    {
        mFoundActors.clear();
    }

    bool GameNewtonBodyAreaType::isInside(Actor* actor)
    {
        return getDistance(actor) <= 0;
    }

    ActorMap GameNewtonBodyAreaType::performQuery()
    {
        return mFoundActors;
    }

    Ogre::Real GameNewtonBodyAreaType::getDistance(Actor* actor)
    {
        if (!actor)
            Throw(NullPointerException, "Argument actor cannot be NULL!");
        OgreNewt::World* world = PhysicsManager::getSingleton()._getNewtonWorld();
        Vector3 retA, retB, retNorm;
        Vector3 positionA;
        Quaternion orientationA;
        mBody->getPositionOrientation(positionA, orientationA);
        int intRet = OgreNewt::CollisionTools::CollisionClosestPoint(world, mBody->getCollision(), orientationA,
            positionA, actor->getPhysicalThing()->_getBody()->getCollision(), actor->getOrientation(),
            actor->getPosition(), retA, retB, retNorm, 0); // set threadindex to 0, I hope this is ok!

        if (intRet == 0)
            return 0;

        return (retA - retB).length();
    }

    Ogre::Real GameNewtonBodyAreaType::getTransitionDistance() const
    {
        return mTransitionDistance;
    }

    void GameNewtonBodyAreaType::setTransitionDistance(Ogre::Real dist)
    {
        mTransitionDistance = dist;
    }

    GameMeshAreaType::GameMeshAreaType(
        Ogre::Entity* entity, GeometryType geomType, Ogre::Vector3 offset, Ogre::Quaternion orientation)
    {
        if (geomType == GT_MESH)
        {
            LOG_ERROR(Logger::CORE,
                "Geometrie Typ 'GT_MESH' is not (yet) support by GameMeshAreaType, use GT_CONVEXHULL instead!");
            geomType = GT_CONVEXHULL;
        }
        OgreNewt::CollisionPtr col
            = PhysicsManager::getSingleton().createCollision(entity, geomType, "", offset, orientation);
        mBody = new OgreNewt::Body(PhysicsManager::getSingleton()._getNewtonWorld(), col);
        mBody->setMaterialGroupID(PhysicsManager::getSingleton().getMaterialID("gamearea"));

        std::static_pointer_cast<OgreNewt::ConvexCollision>(col)->setAsTriggerVolume(true);
    }

    GameSimpleCollisionAreaType::GameSimpleCollisionAreaType(
        Ogre::AxisAlignedBox aabb, GeometryType geomType, Ogre::Vector3 offset, Ogre::Quaternion orientation)
    {
        OgreNewt::CollisionPtr col = PhysicsManager::getSingleton().createCollision(
            "", aabb, geomType, offset, orientation, 0, NULL, NULL, true);
        mBody = new OgreNewt::Body(PhysicsManager::getSingleton()._getNewtonWorld(), col);
        mBody->setMaterialGroupID(PhysicsManager::getSingleton().getMaterialID("gamearea"));

        std::static_pointer_cast<OgreNewt::ConvexCollision>(col)->setAsTriggerVolume(true);
    }
}
