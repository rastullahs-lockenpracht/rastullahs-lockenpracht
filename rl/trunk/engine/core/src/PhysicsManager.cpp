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

#include "PhysicsManager.h"

#include "CoreSubsystem.h"
#include "World.h"
#include "PhysicalThing.h"
#include "Actor.h"
#include "ActorManager.h"
#include "Exception.h"
#include "PhysicsController.h"

using namespace std;

template<> rl::PhysicsManager* Singleton<rl::PhysicsManager>::ms_Singleton = 0;

namespace rl
{
    PhysicsManager& PhysicsManager::getSingleton(void)
    {
        return Singleton<PhysicsManager>::getSingleton();
    }

    PhysicsManager* PhysicsManager::getSingletonPtr(void)
    {
        return Singleton<PhysicsManager>::getSingletonPtr();
    }

    PhysicsManager::PhysicsManager( )
        :   mEnabled(false),
        mWorld(),
        mNewtonDebugger(),
        mPhysicalThings(),
        mControlledThings(),
        mDebugMode(false),
        mGravity(0, -98.0, 0),
        mWorldAABB(Vector3(-100, -100, -100), Vector3(100, 100, 100)),
        mElapsed(0.0f),
        mUpdate(1.0f/120.0f),
        mLevelID(),
        mCharacterID(),
        mCharLevelPair()
    {
        mWorld = new OgreNewt::World();
        mWorld->setSolverModel(0);
        mWorld->setFrictionModel(0);

        // setup materials: default<->default
        OgreNewt::MaterialID* defaultID = mWorld->getDefaultMaterialID();
        OgreNewt::MaterialPair* defaultPair =
            new OgreNewt::MaterialPair(mWorld, defaultID, defaultID);
        defaultPair->setContactCallback(new PhysicsGenericContactCallback);

        // setup materials: character<->level
        mLevelID = new OgreNewt::MaterialID(mWorld);
        mCharacterID = new OgreNewt::MaterialID(mWorld);
        mCharLevelPair = new OgreNewt::MaterialPair(mWorld, mCharacterID, mLevelID);
        mCharLevelPair->setDefaultFriction(0, 0);
    }

    PhysicsManager::~PhysicsManager()
    {
        delete mWorld;
    }

    void PhysicsManager::run(Real elapsedTime)
    {
        mElapsed += elapsedTime;
        int count = 0;

        if ((mElapsed > mUpdate) && (mElapsed < (1.0f)) )
        {
            while (mElapsed > mUpdate)
            {
                mWorld->update(mUpdate);
                mElapsed -= mUpdate;
                count++;
            }
        }
        else
        {
            if (mElapsed < (mUpdate))
            {
                // not enough time has passed this loop, so ignore for now.
            }
            else
            {
                mWorld->update(mElapsed);
                count++;
            }
        }

        //if( mEnabled && elapsedTime > 0.0f )
        //{
        //    mWorld->update(elapsedTime);
        //    if(mDebugMode)
        //    {
        //        mNewtonDebugger->showLines(mWorld);
        //    }
        //}
    }

    void PhysicsManager::setGravity( Real x, Real y, Real z )
    {
        mGravity = Vector3(x, y, z);
    }

    Vector3 PhysicsManager::getGravity() const
    {
        return mGravity;
    }

    PhysicalThing* PhysicsManager::createPhysicalThing(const int geomType,
        const Vector3& size, Real mass, OffsetMode offsetMode)
    {
        PhysicalThing* rval = NULL;

        if (geomType != GT_NONE) {
            OgreNewt::Collision* coll = NULL;

            Vector3 offset(Vector3::ZERO);
            Quaternion orientationBias(Quaternion::IDENTITY);
            Vector3 inertiaCoefficients(Vector3(1.0, 1.0, 1.0));

            if (geomType == GT_BOX)
            {
                coll = new OgreNewt::CollisionPrimitives::Box(mWorld, size);

                if (offsetMode == OM_BOTTOMCENTERED)
                {
                    offset = Vector3(0.0, size.y / 2.0, 0.0);
                }
            }
            else if (geomType == GT_SPHERE)
            {
                double radius = max(size.x, max(size.y, size.z)) / 2.0;

                if (offsetMode == OM_BOTTOMCENTERED)
                {
                    offset = Vector3(0.0, size.y / 2.0, 0.0);
                }

                coll = new OgreNewt::CollisionPrimitives::Ellipsoid(mWorld,
                    Vector3(radius, radius, radius));
            }
            else if (geomType == GT_CAPSULE)
            {
                double radius = max(size.x, size.z) / 2.0;

                if (offsetMode == OM_BOTTOMCENTERED)
                {
                    offset = Vector3(0.0, size.y / 2.0, 0.0);
                }

                orientationBias = Quaternion(Degree(90), Vector3::NEGATIVE_UNIT_Z);

                coll = new OgreNewt::CollisionPrimitives::Capsule(mWorld, radius, size.y);
            }
            else if (geomType == GT_ELLIPSOID)
            {
                if (offsetMode == OM_BOTTOMCENTERED)
                {
                    offset = Vector3(0.0, size.y / 2.0, 0.0);
                }
                // set the size x/z values to the maximum of each for testing
                Vector3 s(size/2.0);
                s.x = max(s.x, s.z);
                s.z = s.x;
                coll = new OgreNewt::CollisionPrimitives::Ellipsoid(mWorld, s);
                inertiaCoefficients = Vector3(s.x*s.x, s.y*s.y, s.z*s.z);
            }

            OgreNewt::Body* body = new OgreNewt::Body(mWorld, coll);

            if (mass > 0.0)
            {
                body->setMassMatrix(mass, mass*inertiaCoefficients);
            }

            body->setCustomForceAndTorqueCallback(genericForceCallback);

            rval = new PhysicalThing(body, offset, orientationBias);
            mPhysicalThings.push_back(rval);        
        }
        return rval;
    }

    void PhysicsManager::removeAndDestroyPhysicalThing(PhysicalThing* thing)
    {
        vector<PhysicalThing*>::iterator it = find(mPhysicalThings.begin(),
            mPhysicalThings.end(), thing);

        if( it != mPhysicalThings.end() )
        {
            PhysicalThing* phys = *it;
            mPhysicalThings.erase(it);
            delete phys;
        }
    }

    void PhysicsManager::setEnabled(bool enabled)
    {
        mEnabled = enabled;
    }

    bool PhysicsManager::isDebugMode() const
    {
        return mDebugMode;
    }

    void PhysicsManager::toggleDebugMode()
    {
        if (mDebugMode)
            mNewtonDebugger->hideLines();
        else
        {
            mNewtonDebugger = &OgreNewt::Debugger::getSingleton();
            mNewtonDebugger->init(
                CoreSubsystem::getSingleton().getWorld()->getSceneManager());
            mNewtonDebugger->showLines(mWorld);
        }
        mDebugMode = !mDebugMode;
    }

    void PhysicsManager::addLevelGeometry( Ogre::Entity* levelEntity )
    {
        RlAssert1(levelEntity);
        RlAssert1(levelEntity->getParentSceneNode());

        SceneNode* node = levelEntity->getParentSceneNode();
        //Level entity has to be attached to a scene node.

        OgreNewt::Collision* collision =
            new OgreNewt::CollisionPrimitives::TreeCollision(mWorld, node, true);
        OgreNewt::Body* body = new OgreNewt::Body(mWorld, collision);

        body->attachToNode(node);
        body->setPositionOrientation(node->getWorldPosition(),
            node->getWorldOrientation());
        body->setMaterialGroupID(mLevelID);

        mLevelBodies.push_back(body);

        // adjust worldAABB
        Vector3 minV(mWorldAABB.getMinimum());
        Vector3 maxV(mWorldAABB.getMaximum());

        AxisAlignedBox entityAABB = levelEntity->getWorldBoundingBox();
        minV.makeFloor(entityAABB.getMinimum());
        maxV.makeCeil(entityAABB.getMaximum());
        mWorldAABB.setMinimum(minV);
        mWorldAABB.setMaximum(maxV);

        mWorld->setWorldSize(mWorldAABB);
    }

    void PhysicsManager::clearLevelGeometry(  )
    {
        for (size_t i = 0; i < mLevelBodies.size(); ++i)
        {
            delete mLevelBodies[i];
        }
        mLevelBodies.clear();
    }

    // adopted from the chararcter demo in the newton sdk
    // copyright 2000-2004
    // By Julio Jerez
    void PhysicsManager::genericForceCallback(OgreNewt::Body* body)
    {
        // apply a simple gravity force.
        Ogre::Real mass;
        Ogre::Vector3 inertia;

        body->getMassMatrix(mass, inertia);

        // apply saved forces in the PhysicalThing
        PhysicalThing* thing =
            static_cast<Actor*>(body->getUserData())->getPhysicalThing();

        thing->addForce(getSingleton().mGravity*mass);
        thing->onApplyForceAndTorque();
    }

    void PhysicsManager::controlledForceCallback(OgreNewt::Body* body)
    {
        PhysicalThing* thing =
            static_cast<Actor*>(body->getUserData())->getPhysicalThing();

        ControllerMap::const_iterator it = getSingleton().mControlledThings.find(thing);
        if (it != getSingleton().mControlledThings.end())
        {
            (*it).second->OnApplyForceAndTorque(thing);
        }
        else
        {
            Throw(IllegalStateException,
                "controlledForceCallback called for uncontrolled PhysicalThing.");
        }
    }

    void PhysicsManager::prepareUserControl(PhysicalThing* thing, OgreNewt::ContactCallback* cb) const
    {
        OgreNewt::Body* body = thing->_getBody();
        body->setMaterialGroupID(mCharacterID);
        body->setAutoFreeze(0);
        body->unFreeze();
        body->setLinearDamping(0.0f);
        Vector3 v(0, 0, 0); // Sonst mag GCC nicht.
        body->setAngularDamping(v);

        body->setCustomForceAndTorqueCallback(controlledForceCallback);

        // Set up-vector, so force application doesn't let the char fall over
        thing->setUpConstraint(Vector3::UNIT_Y);

        ///\todo alles andere als sauber. Es k�nnen mehrere Controllers mit einem Material geben.
        /// in diesem Falle w�rde stets der letzte als Callback f�r alle verwendet.
        mCharLevelPair->setContactCallback(cb);
    }

    void PhysicsManager::unprepareUserControl(PhysicalThing* thing) const
    {
        OgreNewt::Body* body = thing->_getBody();
        body->setMaterialGroupID(mWorld->getDefaultMaterialID());
        body->setCustomForceAndTorqueCallback(genericForceCallback);
        thing->setUpConstraint(Vector3::ZERO);
    }

    PhysicsController* PhysicsManager::getPhysicsController(PhysicalThing* thing) const
    {
        PhysicsController* rval = 0;

        ControllerMap::const_iterator it = mControlledThings.find(thing);
        if (it != mControlledThings.end())
        {
            rval = (*it).second;
        }

        return rval;
    }

    void PhysicsManager::setPhysicsController(PhysicalThing* thing,
        PhysicsController* controller)
    {
        RlAssert1(thing);

        // first see, if the thing has another controller already
        PhysicsController* oldController = getPhysicsController(thing);
        if (oldController)
        {
            // if so, remove it
            unprepareUserControl(thing);
            mControlledThings.erase(thing);
        }

        if(controller)
        {
            // add it to the map and prepare it for control
            mControlledThings[thing] = controller;
            prepareUserControl(thing, controller);
        }
    }

    OgreNewt::World* PhysicsManager::_getNewtonWorld() const
    {
        return mWorld;
    }
}
