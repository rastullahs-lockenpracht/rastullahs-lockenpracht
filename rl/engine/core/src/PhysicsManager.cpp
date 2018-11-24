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

#include "PhysicsManager.h"

#include "Actor.h"
#include "ActorManager.h"
#include "CoreSubsystem.h"
#include "Exception.h"
#include "GameEventManager.h"
#include "PhysicalObject.h"
#include "PhysicalThing.h"
#include "PhysicsCollisionFactory.h"
#include "PhysicsController.h"
#include "PhysicsGenericContactCallback.h"
#include "World.h"

using namespace std;
using namespace OgreNewt;
using namespace Ogre;

template <> rl::PhysicsManager* Singleton<rl::PhysicsManager>::msSingleton = 0;

namespace rl
{
    const Ogre::Real PhysicsManager::NEWTON_GRID_WIDTH = 0.02;

    LQTBodies::LQTBodies(
        int maxData, int maxDepth, float looseness, const Ogre::Vector2& tlc, const Ogre::Vector2& brc, float mWidth)
        : TLooseQuadTreeNode<OgreNewt::Body*, LQTBodies>(maxData, maxDepth, looseness, tlc, brc, mWidth)
    {
    }

    LQTBodies::LQTBodies(const LQTBodies& LQT)
        : TLooseQuadTreeNode<OgreNewt::Body*, LQTBodies>(LQT)
    {
        for (BodyList::const_iterator it = LQT.mData.begin(); it != LQT.mData.end(); it++)
        {
            mData.push_back(*it);
        }
    }

    LQTBodies::~LQTBodies()
    {
        for (BodyList::iterator it = mData.begin(); it != mData.end(); it++)
        {
            delete (*it);
        }
        mData.clear();
    }

    const Ogre::AxisAlignedBox LQTBodies::getAABB(OgreNewt::Body* body)
    {
        return body->getCollision()->getAABB();
    }

    PhysicsManager::PhysicsManager()
        : mEnabled(false)
        , mPhysicalThings()
        , mDebugMode(0)
        , mGravity(0, -9.81, 0)
        , mTimeFactor(1.0f)
        , mWorldAABB(Vector3(-100, -100, -100), Vector3(100, 100, 100))
        , mElapsed(0.0f)
        , mMinTimestep(1.0f / 600.0f)
        , mMaxTimestep(1.0f / 30.0f)
        , mGenericCallback()
        , mPhysicsCollisionFactory(new PhysicsCollisionFactory())
    {
        mWorld = new OgreNewt::World();
        mWorld->setFrictionModel(OgreNewt::World::FM_ADAPTIVE);
        mWorld->setSolverModel(OgreNewt::World::SM_ADAPTIVE);
        // mWorld->setSolverModel(4);

        // setup materials: default<->default
        const OgreNewt::MaterialID* defaultID = mWorld->getDefaultMaterialID();
        mMaterials.insert(MaterialMapPair("default", defaultID));

        OgreNewt::MaterialPair* mDefaultPair = createMaterialPair(defaultID, defaultID);
        mGenericCallback = new PhysicsGenericContactCallback();
        mDefaultPair->setContactCallback(mGenericCallback);
        mDefaultPair->setDefaultFriction(0.85f, 0.80f);

        // setup level material
        createMaterialID("level");

        mWorld->getDebugger().setMaterialColor(getMaterialID("level"), Ogre::ColourValue::Blue);

        // below here starts 'old' stale fix code that should be removed

        // setup level quadtree extents
        mLevelBodiesQuadTree.setMaxData(20);
        mLevelBodiesQuadTree.setMaxDepth(10);

        mLevelBodiesQuadTree.setLoosenessFactor(0.5f);
        mLevelBodiesQuadTree.setExtents(Ogre::Vector2(-100, -100), Ogre::Vector2(100, 100));
    }

    PhysicsManager::~PhysicsManager()
    {
        // remove all bodies
        clearLevelGeometry();

        // remove all physical things
        for (std::vector<PhysicalThing*>::iterator it = mPhysicalThings.begin(); it != mPhysicalThings.end(); it++)
        {
            delete (*it);
        }

        // remove all material-pairs
        for (MaterialPairMap::iterator it = mMaterialPairs.begin(); it != mMaterialPairs.end(); it++)
        {
            delete it->second;
        }

        // remove all materials
        for (MaterialMap::iterator it = mMaterials.begin(); it != mMaterials.end(); it++)
        {
            // default material has been provided by OgreNewt, therefore it
            // it must not be deleted here
            if ((*it).first != "default")
                delete (*it).second;
        }
        mMaterials.clear();

        mWorld->getDebugger().deInit();

        delete mPhysicsCollisionFactory;
        delete mGenericCallback;
        delete mWorld;
    }

    void PhysicsManager::run(Real elapsedTime)
    {
        // do nothing, if not enabled
        if (!mEnabled)
            return;

        // does not need to be executed each frame!
        // this is only here for testing
        // NewtonSetMinimumFrameRate(mWorld->getNewtonWorld(), 1./mMaxTimestep);

        if (mDebugMode == 4)
        {
            mWorld->getDebugger().clearRaycastsRecorded();
        }

        // Newton kann timesteps zwischen 1/20 und 1/600!
        mElapsed += elapsedTime * mTimeFactor;
        while (mElapsed >= mMaxTimestep)
        {
            // perhaps we should add a newtonupdate listener, but i don't
            // know if it's really neccessary
            GameEventManager::getSingleton().notifyNewtonWorldUpdate();

            mWorld->update(mMaxTimestep);
            mElapsed -= mMaxTimestep;
#ifdef _DEBUG
            if (mDebugMode)
            {
                LOG_DEBUG(Logger::CORE,
                    "\tNewtonBodyLog: &Body  Position  Orientation  Velocity  "
                    "Omega  Force  Torque NewtonBodyGetContinuousCollisionMode mass inertia");
                if (Logger::getSingleton().getLogDetail() <= Logger::LL_DEBUG)
                    for (OgreNewt::Body* body = mWorld->getFirstBody(); body != NULL; body = body->getNext())
                        logBodyProperties(body);
            }
#endif
        }
        if (mElapsed > mMinTimestep)
        {
            // perhaps we should add a newtonupdate listener, but i don't
            // know if it's really neccessary
            GameEventManager::getSingleton().notifyNewtonWorldUpdate();

            mWorld->update(mElapsed);
            mElapsed = 0;
#ifdef _DEBUG
            if (mDebugMode)
            {
                LOG_DEBUG(Logger::CORE,
                    "\tNewtonBodyLog: &Body  Position  Orientation  Velocity  "
                    "Omega  Force  Torque NewtonBodyGetContinuousCollisionMode mass inertia");
                for (OgreNewt::Body* body = mWorld->getFirstBody(); body != NULL; body = body->getNext())
                    logBodyProperties(body);
            }
#endif
        }

        if (mDebugMode == 2)
        {
            mWorld->getDebugger().showDebugInformation();
        }
        else if (mDebugMode == 3)
        {
            mWorld->getDebugger().stopRaycastRecording();
        }
    }

#ifdef _DEBUG
    void PhysicsManager::logBodyProperties(const OgreNewt::Body* body)
    {
        std::ostringstream oss;
        Quaternion orient;
        Vector3 pos;
        body->getPositionOrientation(pos, orient);
        Vector3 force = body->getForce();
        Vector3 torque = body->getTorque();
        Vector3 omega = body->getOmega();
        Vector3 velocity = body->getVelocity();
        Vector3 inertia;
        Real mass;
        body->getMassMatrix(mass, inertia);
        oss << "\tNewtonBodyLog: " << body << "  " << pos << "  " << orient << "  " << velocity << "  " << omega << "  "
            << force << "  " << torque << " " << body->getContinuousCollisionMode() << mass << "  " << inertia;
        LOG_DEBUG(Logger::CORE, oss.str());
    }
#endif

    void PhysicsManager::setGravity(Real x, Real y, Real z)
    {
        mGravity = Vector3(x, y, z);
    }

    Vector3 PhysicsManager::getGravity() const
    {
        return mGravity;
    }

    void PhysicsManager::setTimeFactor(Ogre::Real factor)
    {
        mTimeFactor = factor;
    }

    PhysicalThing* PhysicsManager::createPhysicalThing(
        GeometryType geomType, PhysicalObject* po, Real mass, bool hullModifier)
    {
        PhysicalThing* rval = NULL;
        if (geomType != GT_NONE)
        {
            rval = new PhysicalThing(geomType, po, mass, hullModifier);
        }

        return rval;
    }

    void PhysicsManager::removeAndDestroyPhysicalThing(PhysicalThing* thing)
    {
        std::vector<PhysicalThing*>::iterator it = find(mPhysicalThings.begin(), mPhysicalThings.end(), thing);

        if (it != mPhysicalThings.end())
        {
            PhysicalThing* phys = *it;
            mPhysicalThings.erase(it);
            delete phys;
        }
    }

    bool PhysicsManager::isEnabled() const
    {
        return mEnabled;
    }

    void PhysicsManager::setEnabled(bool enabled)
    {
        mEnabled = enabled;
    }

    int PhysicsManager::isDebugMode() const
    {
        return mDebugMode;
    }

    void PhysicsManager::toggleDebugMode()
    {
        mWorld->getDebugger().init(CoreSubsystem::getSingleton().getWorld()->getSceneManager());
        mDebugMode = (mDebugMode + 1) % 5;
        switch (mDebugMode)
        {
        case 0:
            mWorld->getDebugger().stopRaycastRecording();
            mWorld->getDebugger().clearRaycastsRecorded();
            mWorld->getDebugger().hideDebugInformation();
            break;
        case 1:
            mWorld->getDebugger().showDebugInformation();
            break;
        case 2:
            break;
        case 3:
            mWorld->getDebugger().startRaycastRecording(true);
            mWorld->getDebugger().hideDebugInformation();
            break;
        case 4:
            mWorld->getDebugger().startRaycastRecording(true);
            mWorld->getDebugger().hideDebugInformation();
            break;
        default:
            break;
        }
    }

    void PhysicsManager::addLevelGeometry(
        Ogre::Entity* levelEntity, const std::vector<OgreNewt::CollisionPtr>& collisions)
    {
        RlAssert1(levelEntity);
        RlAssert1(levelEntity->getParentSceneNode());

        SceneNode* node = levelEntity->getParentSceneNode();
        // Level entity has to be attached to a scene node.

        // try one compound collision for the entity if there are several collisions
        OgreNewt::CollisionPtr collision;
        switch (collisions.size())
        {
        case 0:
            break;
        case 1:
            collision = collisions[0];
            break;
        default:
            collision
                = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::CompoundCollision(mWorld, collisions, 0));
            break;
        }

        if (collision)
        {
            OgreNewt::Body* body = new OgreNewt::Body(mWorld, collision);

            body->attachNode(node);
            body->setPositionOrientation(node->_getDerivedPosition(), node->_getDerivedOrientation());
            body->setMaterialGroupID(getMaterialID("level"));

            mLevelBodiesQuadTree.add(body);
            // mLevelBodies.push_back(body);
        }

        // adjust worldAABB
        Vector3 minV(mWorldAABB.getMinimum());
        Vector3 maxV(mWorldAABB.getMaximum());

        AxisAlignedBox entityAABB = levelEntity->getWorldBoundingBox(true);
        minV.makeFloor(entityAABB.getMinimum());
        maxV.makeCeil(entityAABB.getMaximum());
        mWorldAABB.setMinimum(minV - Vector3(50, 50, 50));
        mWorldAABB.setMaximum(maxV + Vector3(50, 50, 50));

        mWorld->setWorldSize(mWorldAABB);
    }

    void PhysicsManager::createPhysicsProxy(PhysicalThing* pt)
    {
        pt->createPhysicsProxy();
        mPhysicalThings.push_back(pt);
    }

    void PhysicsManager::createPhysicsProxy_RagDoll(PhysicalThing* pt)
    {
        pt->createPhysicsProxy_RagDoll();
        mPhysicalThings.push_back(pt);
    }

    void PhysicsManager::destroyPhysicsProxy(PhysicalThing* pt)
    {
        std::vector<PhysicalThing*>::iterator it = find(mPhysicalThings.begin(), mPhysicalThings.end(), pt);
        if (it != mPhysicalThings.end())
        {
            mPhysicalThings.erase(it);
        }
        pt->destroyPhysicsProxy();
    }

    void PhysicsManager::clearLevelGeometry()
    {
        mLevelBodiesQuadTree.removeAll();
        /*for (size_t i = 0; i < mLevelBodies.size(); i++ )
        {
            delete mLevelBodies[i];
        }
        mLevelBodies.clear();
        */
    }

    void PhysicsManager::genericForceCallback(OgreNewt::Body* body, float timestep, int)
    {
        // apply saved forces in the PhysicalThing
        PhysicalThing* thing;

        thing = Ogre::any_cast<Actor*>(body->getUserData())->getPhysicalThing();

        thing->onApplyForceAndTorque(timestep);
    }

    void PhysicsManager::controlledForceCallback(OgreNewt::Body* body, float timestep, int)
    {
        PhysicalThing* thing;

        thing = Ogre::any_cast<Actor*>(body->getUserData())->getPhysicalThing();

        if (thing->getPhysicsController())
        {
            thing->getPhysicsController()->OnApplyForceAndTorque(thing, timestep);
        }
        else
        {
            Throw(IllegalStateException, "controlledForceCallback called for uncontrolled PhysicalThing.");
        }
    }

    OgreNewt::World* PhysicsManager::_getNewtonWorld() const
    {
        return mWorld;
    }

    const Ogre::String& PhysicsManager::getName() const
    {
        static String NAME = "PhysicsManager";

        return NAME;
    }

    const OgreNewt::MaterialID* PhysicsManager::getMaterialID(const Ogre::String& materialname) const
    {
        MaterialMap::const_iterator it;

        it = mMaterials.find(materialname);
        if (it != mMaterials.end())
        {
            return (*it).second;
        }

        return NULL;
    }

    const OgreNewt::MaterialID* PhysicsManager::createMaterialID(const Ogre::String& materialname)
    {
        const OgreNewt::MaterialID* materialid = NULL;

        if ((materialid = getMaterialID(materialname)) == NULL)
        {
            materialid = new OgreNewt::MaterialID(mWorld);
            mMaterials.insert(MaterialMapPair(materialname, materialid));
        }
        return materialid;
    }

    OgreNewt::MaterialPair* PhysicsManager::createMaterialPair(
        const OgreNewt::MaterialID* M1, const OgreNewt::MaterialID* M2)
    {
        OgreNewt::MaterialPair* materialpair = NULL;

        if ((materialpair = getMaterialPair(M1, M2)) == NULL)
        {
            materialpair = new OgreNewt::MaterialPair(mWorld, M1, M2);
            mMaterialPairs.insert(MaterialPairMapPair(PairOfMaterials(M1, M2), materialpair));
        }
        return materialpair;
    }

    OgreNewt::MaterialPair* PhysicsManager::getMaterialPair(
        const OgreNewt::MaterialID* M1, const OgreNewt::MaterialID* M2) const
    {
        MaterialPairMap::const_iterator it;

        it = mMaterialPairs.find(PairOfMaterials(M1, M2));
        if (it != mMaterialPairs.end())
        {
            return (*it).second;
        }

        return NULL;
    }

    void PhysicsManager::resetMaterialPair(const OgreNewt::MaterialID* M1, const OgreNewt::MaterialID* M2)
    {
        OgreNewt::MaterialPair* mat_pair = getMaterialPair(M1, M2);
        mat_pair->setContactCallback(mGenericCallback);
        mat_pair->setContinuousCollisionMode(0);
        mat_pair->setDefaultCollidable(1);
        mat_pair->setDefaultElasticity(0.4f);
        mat_pair->setDefaultSoftness(0.1f);
        mat_pair->setDefaultFriction(0.9f, 0.5f);
        //! TODO find a good value here, until now I hope nobody is changing the default from newton :-P
        // mat_pair->setDefaultSurfaceThickness();
    }

    OgreNewt::CollisionPtr PhysicsManager::createCollision(Ogre::Entity* entity, const GeometryType& geomType,
        const Ogre::String& animName, const Ogre::Vector3& offset, const Ogre::Quaternion& orientation,
        const Ogre::Real mass, Ogre::Vector3* inertia, Ogre::Vector3* centerOfMass, bool nocache)
    {
        return mPhysicsCollisionFactory->createCollisionFromEntity(
            entity, geomType, animName, offset, orientation, mass, inertia, centerOfMass, nocache);
        /*
                // problem here is that a mesh can have different animations with
                // different extents. Therefore we add the animName to the meshname
                // to compute a unique name for the collision primitiv.
                Ogre::String collisionName (entity->getMesh()->getName() + animName);

                // result value
                CollisionPtr rval;

                // check if there is a collision primitiv for the specified mesh object
                CollisionInUse &usedcol (mCollisionPrimitives[collisionName]);
                // log some performance warning if collisionname is equal, but geomtype different ?

                if( false )
        //        if ( usedcol.colPtr != NULL )
                {
                    if ( usedcol.geomType != geomType )
                    {   // when geometry types mismatch, issue performance warning
                        LOG_DEBUG(Logger::CORE, " performance warning creating an other collision primitives '"+
                        PhysicsManager::convertGeometryTypeToString(geomType)+ "' for Entity '"+
                        entity->getName()+"' that already has got '"+
                        PhysicsManager::convertGeometryTypeToString(usedcol.geomType)+ "' as a primitive.");
                    } else {
                        // found it
                        rval = usedcol.colPtr;

                    }
                }

        //        if ( rval == NULL )
                {
                    // if there is none, then create a new collision object
                    rval = mPhysicsCollisionFactory->createCollisionFromEntity(
                        entity, geomType, offset, orientation, mass, inertia, centerOfMass );

                    usedcol.geomType = geomType;
                    usedcol.colPtr = rval;
                }

                return rval;
        */
    }

    OgreNewt::CollisionPtr PhysicsManager::createCollision(const Ogre::String& name, const Ogre::AxisAlignedBox& aabb,
        const GeometryType& geomType, const Ogre::Vector3& offset, const Ogre::Quaternion& orientation,
        const Ogre::Real mass, Ogre::Vector3* inertia, Ogre::Vector3* centerOfMass, bool nocache)
    {
        return mPhysicsCollisionFactory->createCollisionFromAABB(
            name, aabb, geomType, offset, orientation, mass, inertia, centerOfMass, nocache);
        /*
                // result value
                CollisionPtr rval;

                // check if there is a collision primitiv for the specified mesh object
                CollisionInUse &usedcol (mCollisionPrimitives[name]);
                // log some performance warning if collisionname is equal, but geomtype different ?

                if( false )
        //        if ( usedcol.colPtr != NULL )
                {
                    if ( usedcol.geomType != geomType )
                    {   // when geometry types mismatch, issue performance warning
                        LOG_DEBUG(Logger::CORE, " performance warning creating an other collision primitives '"+
                        PhysicsManager::convertGeometryTypeToString(geomType)+ "' for AABB '"+
                        name +"' that already has got '"+
                        PhysicsManager::convertGeometryTypeToString(usedcol.geomType)+ "' as a primitive.");
                    } else {
                        // found it
                        rval = usedcol.colPtr;
                    }
                }

        //        if ( rval == NULL )
                {
                    // if there is none, then create a new collision object
                    rval = mPhysicsCollisionFactory->createCollisionFromAABB(
                        aabb, geomType, offset, orientation, mass, inertia, centerOfMass );

                    usedcol.geomType = geomType;
                    usedcol.colPtr = rval;
                }

                return rval;
        */
    }

    GeometryType PhysicsManager::convertStringToGeometryType(const Ogre::String& geomTypeString)
    {
        if (geomTypeString == "box")
            return GT_BOX;
        if (geomTypeString == "pyramid")
            return GT_PYRAMID;
        if (geomTypeString == "sphere")
            return GT_SPHERE;
        if (geomTypeString == "capsule")
            return GT_CAPSULE;
        if (geomTypeString == "mesh")
            return GT_MESH;
        if (geomTypeString == "ellipsoid")
            return GT_ELLIPSOID;
        if (geomTypeString == "convexhull")
            return GT_CONVEXHULL;
        return GT_NONE;
    }

    Ogre::String PhysicsManager::convertGeometryTypeToString(const GeometryType& geomType)
    {
        Ogre::String typestr("none");

        if (geomType == GT_BOX)
            typestr = "box";
        if (geomType == GT_PYRAMID)
            typestr = "pyramid";
        if (geomType == GT_SPHERE)
            typestr = "sphere";
        if (geomType == GT_CAPSULE)
            typestr = "capsule";
        if (geomType == GT_MESH)
            typestr = "mesh";
        if (geomType == GT_ELLIPSOID)
            typestr = "ellipsoid";
        if (geomType == GT_CONVEXHULL)
            typestr = "convexhull";
        return typestr;
    }
}
