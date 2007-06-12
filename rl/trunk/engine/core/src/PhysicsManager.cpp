/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#include "Actor.h"
#include "ActorManager.h"
#include "CoreSubsystem.h"
#include "Exception.h"
#include "PhysicsController.h"
#include "PhysicsGenericContactCallback.h"
#include "PhysicalObject.h"
#include "PhysicalThing.h"
#include "World.h"

using namespace std;
using namespace OgreNewt;
using namespace Ogre;

template<> rl::PhysicsManager* Singleton<rl::PhysicsManager>::ms_Singleton = 0;

namespace rl
{
    const Ogre::Real PhysicsManager::NEWTON_GRID_WIDTH = 0.01;

	LQTBodies::LQTBodies(int maxData, int maxDepth, float looseness,
		const Ogre::Vector2& tlc, const Ogre::Vector2& brc, float mWidth)
		: TLooseQuadTreeNode<OgreNewt::Body*, LQTBodies>( maxData, maxDepth, looseness, tlc, brc, mWidth)
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

    PhysicsManager::PhysicsManager( )
        : mEnabled(false),
        mNewtonDebugger(),
        mPhysicalThings(),
        mDebugMode(false),
        mGravity(0, -9.81, 0),
        mWorldAABB(Vector3(-100, -100, -100), Vector3(100, 100, 100)),
        mElapsed(0.0f),
        mMinTimestep(1.0f/600.0f),
        mMaxTimestep(1.0f/30.0f),
        mGenericCallback(),
        mPhysicsCollisionFactory(new PhysicsCollisionFactory())
    {
		mWorld = new OgreNewt::World();
        mWorld->setFrictionModel(OgreNewt::World::FM_ADAPTIVE);
		mWorld->setSolverModel(OgreNewt::World::SM_ADAPTIVE);
		//mWorld->setSolverModel(4);

        // setup materials: default<->default
        const OgreNewt::MaterialID* defaultID = mWorld->getDefaultMaterialID();
        mMaterials.insert(MaterialMapPair("default", defaultID));

        OgreNewt::MaterialPair* mDefaultPair = createMaterialPair( defaultID, defaultID );
        mGenericCallback = new PhysicsGenericContactCallback();
        mDefaultPair->setContactCallback(mGenericCallback);
        mDefaultPair->setDefaultFriction(0.85f, 0.80f);

        // setup level material
        createMaterialID("level");

        // below here starts 'old' stale fix code that should be removed

        // setup character material
        // actually this is needed here, because the actor is created in advance before the
        // character controller who actually does create 'character' material too.
        createMaterialID("character");

        // setup level quadtree extents
        mLevelBodiesQuadTree.setMaxData(20);
        mLevelBodiesQuadTree.setMaxDepth(10);
        mLevelBodiesQuadTree.setLoosenessFactor(0.5f);
        mLevelBodiesQuadTree.setExtents(Ogre::Vector2(-100,-100), Ogre::Vector2(100,100));
    }

    PhysicsManager::~PhysicsManager()
    {
        // simply remove all collision ptrs (is this really ok?)
        mCollisionPrimitives.clear();

        // remove all materials
        for (MaterialMap::iterator it = mMaterials.begin(); it != mMaterials.end(); it++)
        {
            // default material has been provided by OgreNewt, therefore it
            // it must not be deleted here
            if ((*it).first != "default") 
                delete (*it).second;
        }
        mMaterials.clear();

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
        //NewtonSetMinimumFrameRate(mWorld->getNewtonWorld(), 1./mMaxTimestep);


        // Newton kann timesteps zwischen 1/20 und 1/600!
        mElapsed += elapsedTime;
        while( mElapsed >= mMaxTimestep)
        {
            mWorld->update(mMaxTimestep);
            mElapsed-=mMaxTimestep;
        }
        if( mElapsed > mMinTimestep)
        {
            mWorld->update(mElapsed);
            mElapsed = 0;
        }
    }

    void PhysicsManager::setGravity( Real x, Real y, Real z )
    {
        mGravity = Vector3(x, y, z);
    }

    Vector3 PhysicsManager::getGravity() const
    {
        return mGravity;
    }

    PhysicalThing* PhysicsManager::createPhysicalThing(GeometryType geomType,
        PhysicalObject* po, Real mass, bool hullModifier)
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
        vector<PhysicalThing*>::iterator it = find(mPhysicalThings.begin(),
            mPhysicalThings.end(), thing);

        if( it != mPhysicalThings.end() )
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

    bool PhysicsManager::isDebugMode() const
    {
        return mDebugMode;
    }

    void PhysicsManager::toggleDebugMode()
    {
        if (mDebugMode)
		{
            mNewtonDebugger->hideLines();
		}
        else
        {
            mNewtonDebugger = &OgreNewt::Debugger::getSingleton();

			try
			{
				mNewtonDebugger->init(
					CoreSubsystem::getSingleton().getWorld()->getSceneManager());
			}
			catch(Ogre::Exception) {}

            mNewtonDebugger->showLines(mWorld);
        }
        mDebugMode = !mDebugMode;
    }

    void PhysicsManager::addLevelGeometry( Ogre::Entity* levelEntity, const std::vector<OgreNewt::CollisionPtr> &collisions)
    {
        RlAssert1(levelEntity);
        RlAssert1(levelEntity->getParentSceneNode());

        SceneNode* node = levelEntity->getParentSceneNode();
        //Level entity has to be attached to a scene node.

        for( size_t i = 0; i < collisions.size(); i++)
        {
		    if( collisions[i].isNull() )
                continue;

            OgreNewt::CollisionPtr collision = collisions[i];
            
		    OgreNewt::Body* body = new OgreNewt::Body(mWorld, collision );
            

            body->attachToNode(node);
            body->setPositionOrientation(node->getWorldPosition(),
                node->getWorldOrientation());
            body->setMaterialGroupID(getMaterialID("level"));
			
			mLevelBodiesQuadTree.add(body);
            //mLevelBodies.push_back(body);
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

	void PhysicsManager::createPhysicsProxy(PhysicalThing* pt, SceneNode* node)
	{
		pt->createPhysicsProxy(node);
		mPhysicalThings.push_back(pt);
	}

    void PhysicsManager::clearLevelGeometry(  )
    {
		mLevelBodiesQuadTree.removeAll();
        /*for (size_t i = 0; i < mLevelBodies.size(); i++ )
        {
            delete mLevelBodies[i];
        }
        mLevelBodies.clear();
		*/
    }

    // adopted from the chararcter demo in the newton sdk
    // copyright 2000-2004
    // By Julio Jerez
    void PhysicsManager::genericForceCallback(OgreNewt::Body* body)
    {
        // apply saved forces in the PhysicalThing
        PhysicalThing* thing =
            static_cast<Actor*>(body->getUserData())->getPhysicalThing();

        thing->onApplyForceAndTorque();
    }

    void PhysicsManager::controlledForceCallback(OgreNewt::Body* body)
    {
        PhysicalThing* thing =
            static_cast<Actor*>(body->getUserData())->getPhysicalThing();

        if (thing->getPhysicsController())
        {
            thing->getPhysicsController()->OnApplyForceAndTorque(thing);
        }
        else
        {
            Throw(IllegalStateException,
                "controlledForceCallback called for uncontrolled PhysicalThing.");
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
            mMaterials.insert( MaterialMapPair(materialname, materialid) );
        }
        return materialid;
    }

    OgreNewt::MaterialPair* PhysicsManager::createMaterialPair(const OgreNewt::MaterialID* M1,
        const OgreNewt::MaterialID* M2)
    {
        OgreNewt::MaterialPair* materialpair = NULL;

        if ((materialpair = getMaterialPair(M1, M2)) == NULL)
        {
            materialpair = new OgreNewt::MaterialPair(mWorld, M1, M2);
            mMaterialPairs.insert(
                MaterialPairMapPair( PairOfMaterials(M1,M2), materialpair ));
        }
        return materialpair;
    }

    OgreNewt::MaterialPair* PhysicsManager::getMaterialPair(const OgreNewt::MaterialID* M1,
            const OgreNewt::MaterialID* M2) const
    {
        MaterialPairMap::const_iterator it;

        it = mMaterialPairs.find(PairOfMaterials(M1,M2));
        if (it != mMaterialPairs.end())
        {
            return (*it).second;
        }

        return NULL;
    }

    void PhysicsManager::resetMaterialPair(const OgreNewt::MaterialID* M1,
            const OgreNewt::MaterialID* M2)
    {
        getMaterialPair(M1,M2)->setContactCallback(mGenericCallback);
    }

    OgreNewt::CollisionPtr PhysicsManager::createCollision(
        Ogre::Entity* entity, const GeometryType& geomType, 
		const Ogre::String animName, Ogre::Vector3* offset,
        Ogre::Quaternion* orientation, const Ogre::Real mass, Ogre::Vector3* inertia)
    {
        // problem here is that a mesh can have different animations with
        // different extents. Therefore we add the animName to the meshname
        // to compute a unique name for the collision primitiv.
        Ogre::String collisionName (entity->getMesh()->getName() + animName);

        // result value
        CollisionPtr rval;
		
        // check if there is a collision primitiv for the specified mesh object
        CollisionInUse &usedcol (mCollisionPrimitives[collisionName]);
        // log some performance warning if collisionname is equal, but geomtype different ?

        if (! usedcol.colPtr.isNull() )
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
                

                if( inertia )
                {
                    // we must set inertia here, the calling function doesn't know we are not creating a new collision
                    Ogre::AxisAlignedBox aabb(entity->getBoundingBox());
                    Vector3 size( aabb.getSize() );
                    switch(usedcol.geomType)
                    {
                    case GT_BOX: // from createBox
                        *inertia = OgreNewt::MomentOfInertia::CalcBoxSolid(mass, aabb.getSize());
                        break;
                    case GT_CAPSULE: // from createCapsule
                        {
                            double radius = std::max(size.x, size.z) / 2.0;
                            double sradius = radius*radius;
                            *inertia = Vector3(sradius, size.y*size.y, sradius) * mass;
                        }
                        break;
                    case GT_CONVEXHULL: // from createCollisionFromEntity
				        *inertia = Vector3(
				        size.x*size.x/6.0f,
				        size.y*size.y/6.0f,
			            size.z*size.z/6.0f) * mass;
                        break;
                    case GT_ELLIPSOID: // from createEllipsoid
                        {
                            Vector3 s(size/2.0);
                            s.x = std::max(s.x, s.z);
                            s.z = s.x;
                            *inertia = Vector3(s.x*s.x, s.y*s.y, s.z*s.z) * mass;
                        }
                        break;
                    case GT_MESH:
                    case GT_NONE:
                        *inertia = Ogre::Vector3::ZERO;
                        break;
                    case GT_PYRAMID: // createPyramid
                        *inertia = Ogre::Vector3(size.x,size.y/2.0f, size.z) * mass;
                        break;
                    case GT_SPHERE:
                        {
                            double radius = std::max(size.x, std::max(size.y, size.z)) / 2.0;
                            //*inertia = OgreNewt::MomentOfInertia::CalcSphereSolid(Mass,radius);
                            *inertia = mass * Vector3(radius*radius, radius*radius, radius*radius);
                        }
                        break;
                    }
                }
            }
        }
        
        if (rval.isNull())
        {
            // if there is none, then create a new collision object
            rval = mPhysicsCollisionFactory->createCollisionFromEntity(
                entity, geomType, offset, orientation, mass, inertia );

            usedcol.geomType = geomType;
            usedcol.colPtr = rval;
        }

        return rval;
    }

    OgreNewt::CollisionPtr PhysicsManager::createCollision(
        const Ogre::String& name, const Ogre::AxisAlignedBox& aabb,
        const GeometryType& geomType, Ogre::Vector3* offset,
        Ogre::Quaternion* orientation, const Ogre::Real mass, Ogre::Vector3* inertia)
    {
        // result value
        CollisionPtr rval;
		
        // check if there is a collision primitiv for the specified mesh object
        CollisionInUse &usedcol (mCollisionPrimitives[name]);
        // log some performance warning if collisionname is equal, but geomtype different ?

        if (! usedcol.colPtr.isNull() )
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
        
        if (rval.isNull())
        {
            // if there is none, then create a new collision object
            rval = mPhysicsCollisionFactory->createCollisionFromAABB(
                aabb, geomType, offset, orientation, mass, inertia );

            usedcol.geomType = geomType;
            usedcol.colPtr = rval;
        }

        return rval;
    }

    PhysicsCollisionFactory* PhysicsManager::getCollisionFactory()
    {
        return mPhysicsCollisionFactory;
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

    bool PhysicsCollisionFactory::checkSize(const Ogre::Vector3& size) const
    {
        if( size.x < PhysicsManager::NEWTON_GRID_WIDTH ||
            size.y < PhysicsManager::NEWTON_GRID_WIDTH ||
            size.z < PhysicsManager::NEWTON_GRID_WIDTH )
            return false;
        return true;            
    }

    void PhysicsCollisionFactory::correctSize(Ogre::Vector3& size)
    {
        // correct size, log warning and fail back to box
        if (size.x < PhysicsManager::NEWTON_GRID_WIDTH)
            size.x = PhysicsManager::NEWTON_GRID_WIDTH;
        if (size.y < PhysicsManager::NEWTON_GRID_WIDTH)
            size.y = PhysicsManager::NEWTON_GRID_WIDTH;
        if (size.z < PhysicsManager::NEWTON_GRID_WIDTH)
            size.z = PhysicsManager::NEWTON_GRID_WIDTH;
        LOG_MESSAGE(Logger::CORE, "Correcting collision primitiv size");
    }

    /*
    Ogre::Vector3 PhysicsCollisionFactory::calculateIntertia(const Ogre::Real& Mass, Ogre::Vector3* inertiaCoefficients)
    {
        return Ogre::Vector3(0,0,0);
    }
    */

    OgreNewt::CollisionPtr PhysicsCollisionFactory::createCollisionFromAABB(const Ogre::AxisAlignedBox aabb,
        const GeometryType& geomType,
        Ogre::Vector3* offset,
        Ogre::Quaternion* orientation,
        const Ogre::Real Mass,
        Ogre::Vector3* inertia) 
    {
        // size of the mesh
        Vector3 size( aabb.getSize() );
        // type for the collision primitiv (can change internally here)
        bool forceBox (false);
		
        // result value
        CollisionPtr rval;

        // size check (if object is too small, it falls back to a box primitiv
        if (checkSize(size) == false )
        {
            correctSize(size);
            LOG_MESSAGE(Logger::CORE, 
                " AABB is too small, using 'box' instead of primitiv '" +
                PhysicsManager::convertGeometryTypeToString(geomType));
            forceBox = true;
        }

        // check if the geometry type is supported for aabb
        if (geomType == GT_CONVEXHULL || 
            geomType == GT_MESH)
        {
            LOG_MESSAGE(Logger::CORE, 
                " the geometry type '"+
                PhysicsManager::convertGeometryTypeToString(geomType)+
                "' is not supported for aabb, fail back to box");
            forceBox = true;
        }

        /* differentiate between the different collision primitives, because they all
		   need different offset and probably different orientation values.
		   Newton SDK is really nifty and helps here, because we can shift the origin
		   of the coordinate system of the primitiv we create into any position we
		   desire. Actually this is the bottom middle of our mesh - as the meshes are
		   always constructed like that.
	    */
		if (geomType == GT_BOX || forceBox == true)
        {
			rval = createBox(aabb, offset, orientation, Mass, inertia);
        }
        else if (geomType == GT_PYRAMID)
        {
            rval = createPyramid(aabb, offset, orientation, Mass, inertia);
        }
        else if (geomType == GT_SPHERE)
        {
			rval = createSphere(aabb, offset, orientation, Mass, inertia);
        }
        else if (geomType == GT_ELLIPSOID)
        {
            rval = createEllipsoid(aabb, offset, orientation, Mass, inertia);
        }
		else if (geomType == GT_CAPSULE)
		{
			rval = createCapsule(aabb, offset, orientation, Mass, inertia);
		}
        return rval;
    }

    OgreNewt::CollisionPtr PhysicsCollisionFactory::createCollisionFromEntity(Ogre::Entity* entity,
        const GeometryType& geomType,
        Ogre::Vector3* offset,
        Ogre::Quaternion* orientation,
        const Ogre::Real Mass,
        Ogre::Vector3* inertia) 
    {
        // bounding box of the mesh
        const Ogre::AxisAlignedBox aabb(entity->getBoundingBox());
        // size of the mesh
        Vector3 size( aabb.getSize() );
        // type for the collision primitiv (can change internally here)
        bool forceBox (false);
		
        // result value
        CollisionPtr rval;

        // size check (if object is too small, it falls back to a box primitiv
        if (checkSize(size) == false )
        {
            correctSize(size);
            LOG_MESSAGE(Logger::CORE, " Entity '"+entity->getName()+
                "' is too small, using 'box' instead of primitiv '"+
                PhysicsManager::convertGeometryTypeToString(geomType));
            forceBox = true;
        }

		/* differentiate between the different collision primitives, because they all
		   need different offset and probably different orientation values.
		   Newton SDK is really nifty and helps here, because we can shift the origin
		   of the coordinate system of the primitiv we create into any position we
		   desire. Actually this is the bottom middle of our mesh - as the meshes are
		   always constructed like that.
	    */
		if (geomType == GT_BOX || forceBox == true)
        {
			rval = createBox(aabb, offset, orientation, Mass, inertia);
        }
        else if (geomType == GT_PYRAMID)
        {
            rval = createPyramid(aabb, offset, orientation, Mass, inertia);
        }
        else if (geomType == GT_SPHERE)
        {
			rval = createSphere(aabb, offset, orientation, Mass, inertia);
        }
        else if (geomType == GT_ELLIPSOID)
        {
            rval = createEllipsoid(aabb, offset, orientation, Mass, inertia);
        }
		else if (geomType == GT_CAPSULE)
		{
			rval = createCapsule(aabb, offset, orientation, Mass, inertia);
		}
        else if (geomType == GT_CONVEXHULL)
        {
            // offset of the collision primitiv
	    	Ogre::Vector3 object_offset( Ogre::Vector3::ZERO );
            // orientation of the collision primitiv
		    Ogre::Quaternion object_orientation = Ogre::Quaternion::IDENTITY;
				
            // set offset/orientation when they are null
			if (! offset)
				offset = &object_offset;
			if (! orientation)
				orientation = &object_orientation;

			// calculate the convex hull of the animated mesh
			rval = CollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(
                PhysicsManager::getSingleton()._getNewtonWorld(),
				entity, entity->hasSkeleton(), *orientation, *offset));

			if (inertia != NULL)
			{
				*inertia = Vector3(
				size.x*size.x/6.0f,
				size.y*size.y/6.0f,
				size.z*size.z/6.0f) * Mass;
			}
        }
        else if (geomType == GT_MESH)
        {
            if (inertia != NULL)
                *inertia = Ogre::Vector3::ZERO;

            rval = CollisionPtr(new OgreNewt::CollisionPrimitives::TreeCollision(
                PhysicsManager::getSingleton()._getNewtonWorld(),
                entity, false, true));
        }
        else
        {
            Throw(IllegalArgumentException, "unknown geometry type.");
        }

        if (rval.isNull())
        {
            LOG_DEBUG(Logger::CORE, " creating collision primitiv '"+
                PhysicsManager::convertGeometryTypeToString(geomType)+"' for Entity '"+
                entity->getName()+"' failed.");
        }
        else
        {
            LOG_DEBUG(Logger::CORE, " collision primitiv '"+
                PhysicsManager::convertGeometryTypeToString(geomType)+"' created for Entity '"+
                entity->getName()+"'");
        }

        return rval;
    }

    OgreNewt::CollisionPtr PhysicsCollisionFactory::createBox(const Ogre::AxisAlignedBox& aabb,
            Ogre::Vector3* offset,
            Ogre::Quaternion* orientation,
            const Ogre::Real mass,
            Ogre::Vector3* inertia)
    {
        // offset of the collision primitiv
		Ogre::Vector3 object_offset( aabb.getCenter() );
        // orientation of the collision primitiv
		Ogre::Quaternion object_orientation = Ogre::Quaternion::IDENTITY;

        // set offset/orientation when they are null
		if (! offset)
			offset = &object_offset;
		if (! orientation)
			orientation = &object_orientation;
        if (inertia)
            *inertia = OgreNewt::MomentOfInertia::CalcBoxSolid(mass, aabb.getSize());

		// a box collision primitiv has got it's coordinate system at it's center, so we need to shift it
		return CollisionPtr(new OgreNewt::CollisionPrimitives::Box(
            PhysicsManager::getSingleton()._getNewtonWorld(),
            aabb.getSize(), *orientation, *offset));
    }

    OgreNewt::CollisionPtr PhysicsCollisionFactory::createPyramid(const Ogre::AxisAlignedBox& aabb,
            Ogre::Vector3* offset,
            Ogre::Quaternion* orientation,
            const Ogre::Real mass,
            Ogre::Vector3* inertia)
    {
        Ogre::Vector3 size = aabb.getSize();
        // positional offset of the collision primitiv
        Ogre::Vector3 object_offset(aabb.getCenter());
        // orientation of the collision primitiv
		Ogre::Quaternion object_orientation = Ogre::Quaternion::IDENTITY;

        // set offset/orientation when they are null
		if (! offset)
			offset = &object_offset;
		if (! orientation)
			orientation = &object_orientation;
        if (inertia)
            *inertia = Ogre::Vector3(size.x,size.y/2.0f, size.z) * mass;

        return CollisionPtr(new OgreNewt::CollisionPrimitives::Pyramid(
            PhysicsManager::getSingleton()._getNewtonWorld(),
            size, *orientation, *offset));
    }

    OgreNewt::CollisionPtr PhysicsCollisionFactory::createSphere(const Ogre::AxisAlignedBox& aabb,
            Ogre::Vector3* offset,
            Ogre::Quaternion* orientation,
            const Ogre::Real mass,
            Ogre::Vector3* inertia)
    {
        Ogre::Vector3 size = aabb.getSize();
        // calculate the maximum radius needed to include 'everything'
        double radius = std::max(size.x, std::max(size.y, size.z)) / 2.0;
        // positional offset of the collision primitiv
		Ogre::Vector3 object_offset(0,radius,0);
        // orientation of the collision primitiv
		Ogre::Quaternion object_orientation = Ogre::Quaternion::IDENTITY;
        
		// set offset/orientation when they are null
		if (! offset)
			offset = &object_offset;
		if (! orientation)
			orientation = &object_orientation;
        if (inertia)
            //*inertia = OgreNewt::MomentOfInertia::CalcSphereSolid(Mass,radius);
            *inertia = mass * Vector3(radius*radius, radius*radius, radius*radius);

		// a sphere primitiv has got its coordinate system at its center, so shift it with radius
        return CollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid(
    		PhysicsManager::getSingleton()._getNewtonWorld(),
            Vector3(radius, radius, radius), *orientation, *offset));
    }

    OgreNewt::CollisionPtr PhysicsCollisionFactory::createEllipsoid(const Ogre::AxisAlignedBox& aabb,
            Ogre::Vector3* offset,
            Ogre::Quaternion* orientation,
            const Ogre::Real mass,
            Ogre::Vector3* inertia)
    {
        Ogre::Vector3 size = aabb.getSize();
        // set the size x/z values to the maximum
        Vector3 s(size/2.0);
        s.x = std::max(s.x, s.z);
        s.z = s.x;
        // positional offset of the collision primitiv
		Ogre::Vector3 object_offset(0,s.y,0);
        // orientation of the collision primitiv
		Ogre::Quaternion object_orientation = Ogre::Quaternion::IDENTITY;

		// set offset/orientation when they are null
		if (! offset) 
			offset = &object_offset;
		if (! orientation)
			orientation = &object_orientation;
        if (inertia)
            *inertia = Vector3(s.x*s.x, s.y*s.y, s.z*s.z) * mass;

        // an ellipsoid primitiv has got its coordinate system at its center, so shift it with radius
        return CollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid(
            PhysicsManager::getSingleton()._getNewtonWorld(),
            s, *orientation, *offset));
    }

    OgreNewt::CollisionPtr PhysicsCollisionFactory::createCapsule(const Ogre::AxisAlignedBox& aabb,
            Ogre::Vector3* offset,
            Ogre::Quaternion* orientation,
            const Ogre::Real mass,
            Ogre::Vector3* inertia)
    {
        Ogre::Vector3 size = aabb.getSize();
        // positional offset of the collision primitiv
		Ogre::Vector3 object_offset(0, size.y/2, 0);
        // orientation of the collision primitiv
		Ogre::Quaternion object_orientation = Ogre::Quaternion::IDENTITY;
        double radius = std::max(size.x, size.z) / 2.0;
		double height = size.y;
        // fixme: this is a semi ugly fix - because the orientation should be set by the loader
        object_orientation.FromAngleAxis(Degree(90), Vector3::UNIT_Z);

		// set offset/orientation when they are null
		if (! offset)
    		offset = &object_offset;
		if (! orientation) 
			orientation = &object_orientation;
        if (inertia) {
            double sradius = radius*radius;
			*inertia = Vector3(sradius, size.y*size.y, sradius) * mass;
        }

		// an capsule primitiv has got its coordinate system at its center, so shift it with radius
		// additionally it is x axis aligned, so rotate it 90 degrees around z axis
		return CollisionPtr(new OgreNewt::CollisionPrimitives::Capsule(
            PhysicsManager::getSingleton()._getNewtonWorld(),
            radius, height, *orientation, *offset));
    }

}
