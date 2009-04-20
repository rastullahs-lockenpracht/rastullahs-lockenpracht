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

#include "Exception.h"
#include "PhysicsCollisionFactory.h"
#include "PhysicsManager.h"

#ifdef __APPLE__
#   include <Ogre/OgreMesh.h>
#else
#   include <OgreMesh.h>
#endif



using namespace std;
using namespace OgreNewt;
using namespace Ogre;


namespace rl
{
    PhysicsCollisionFactory::~PhysicsCollisionFactory()
    {
        clearCollisionCache();
    }

    void PhysicsCollisionFactory::clearCollisionCache()
    {
        mMeshCollisionsCache.clear();
        mConvexCollisionsCache.clear();
    }

    bool PhysicsCollisionFactory::checkSize(const Ogre::AxisAlignedBox& aabb) const
    {
        Ogre::Vector3 size = aabb.getSize();
        if( size.x < PhysicsManager::NEWTON_GRID_WIDTH ||
            size.y < PhysicsManager::NEWTON_GRID_WIDTH ||
            size.z < PhysicsManager::NEWTON_GRID_WIDTH )
            return false;
        return true;
    }

    void PhysicsCollisionFactory::correctSize(Ogre::AxisAlignedBox& aabb)
    {
        Ogre::Vector3 size = aabb.getSize();
        // correct size, log warning and fail back to box
        if (size.x < PhysicsManager::NEWTON_GRID_WIDTH)
            size.x = PhysicsManager::NEWTON_GRID_WIDTH;
        if (size.y < PhysicsManager::NEWTON_GRID_WIDTH)
            size.y = PhysicsManager::NEWTON_GRID_WIDTH;
        if (size.z < PhysicsManager::NEWTON_GRID_WIDTH)
            size.z = PhysicsManager::NEWTON_GRID_WIDTH;
        LOG_MESSAGE(Logger::CORE, "Correcting collision primitiv size");
        Ogre::Vector3 center = aabb.getCenter();
        aabb.setMaximum(center + 0.5*size);
        aabb.setMinimum(center - 0.5*size);
    }

    OgreNewt::CollisionPtr PhysicsCollisionFactory::createCollisionFromAABB(
        const Ogre::String& name,
        const Ogre::AxisAlignedBox& passedAabb,
        const GeometryType& geomType,
        const Ogre::Vector3 &offset,
        const Ogre::Quaternion &orientation,
        const Ogre::Real mass,
        Ogre::Vector3* inertia,
        Ogre::Vector3* centerOfMass,
        bool nocache)
    {
        Ogre::AxisAlignedBox aabb(passedAabb);
        // type for the collision primitiv (can change internally here)
        bool forceBox (false);

        // result value
        ConvexCollisionPtr rval;
#ifndef OGRENEWT_COLLISION_USE_SHAREDPTR
        rval = NULL;
#endif

        // size check (if object is too small, it falls back to a box primitiv
        if (checkSize(aabb) == false )
        {
            correctSize(aabb);
            LOG_WARNING(Logger::CORE,
                " AABB is too small, using 'box' instead of primitiv '" +
                PhysicsManager::convertGeometryTypeToString(geomType));
            forceBox = true;
        }

        // check if the geometry type is supported for aabb
        if (geomType == GT_CONVEXHULL ||
            geomType == GT_MESH)
        {
            LOG_WARNING(Logger::CORE,
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
			rval = createBox(aabb, offset, orientation);
        }
        else if (geomType == GT_PYRAMID)
        {
            rval = createPyramid(aabb, offset, orientation);
        }
        else if (geomType == GT_SPHERE)
        {
			rval = createSphere(aabb, offset, orientation);
        }
        else if (geomType == GT_ELLIPSOID)
        {
            rval = createEllipsoid(aabb, offset, orientation);
        }
		else if (geomType == GT_CAPSULE)
		{
			rval = createCapsule(aabb, offset, orientation);
		}

        // calculate inertia / centerOfMass if needed
        if (inertia != NULL || centerOfMass != NULL )
        {
            Vector3 temp_inertia, temp_centerOfMass;
            rval->calculateInertialMatrix(temp_inertia, temp_centerOfMass);
            if( inertia != NULL )
                *inertia = temp_inertia*mass;
            if( centerOfMass != NULL )
                *centerOfMass = temp_centerOfMass;
        }

        return rval;
    }

    OgreNewt::CollisionPtr PhysicsCollisionFactory::createCollisionFromEntity(Ogre::Entity* entity,
        const GeometryType& geomType,
        const Ogre::String& animName,
        const Ogre::Vector3 &offset,
        const Ogre::Quaternion &orientation,
        const Ogre::Real mass,
        Ogre::Vector3* inertia,
        Ogre::Vector3* centerOfMass,
        bool nocache)
    {
        // bounding box of the mesh
        Ogre::AxisAlignedBox aabb(entity->getBoundingBox());
        // apply scale if attached to a node (like in OgreNewt for convexhull)
        if( entity->getParentNode() )
            aabb.scale(entity->getParentNode()->getScale());

        // type for the collision primitiv (can change internally here)
        bool forceBox (false);

        // result value
        CollisionPtr rval;
        ConvexCollisionPtr rvalAsConvexCollision; // store pointer to ConvexCollision, so we don't need to cast
                                                  // this also indicates, that it is possible to calculate inertia/centerOfMass
#ifndef OGRENEWT_COLLISION_USE_SHAREDPTR
        rval = NULL;
        rvalAsConvexCollision = NULL;
#endif

        // size check (if object is too small, it falls back to a box primitiv
        if (checkSize(aabb) == false )
        {
            correctSize(aabb);
            LOG_MESSAGE(Logger::CORE, " Entity '"+entity->getName()+
                "' is too small, using 'box' instead of primitiv '"+
                PhysicsManager::convertGeometryTypeToString(geomType));
            forceBox = true;
        }


        MeshCollisionCacheMap::iterator meshCacheIt = mMeshCollisionsCache.end();
        ConvexCollisionCacheMap::iterator convexCacheIt = mConvexCollisionsCache.end();

        // check if we can reuse a cached collision
        if( !nocache )
        {
            // use meshcollisionscache
            if( geomType == GT_MESH && !forceBox )
            {
                Ogre::Vector3 scale = Ogre::Vector3::ZERO;
                if( entity->getParentNode() )
                    scale = entity->getParentNode()->getScale();

                StringVector index(entity->getMesh()->getName() + animName, scale);
                std::pair<MeshCollisionCacheMap::iterator,bool> iterBoolPair = mMeshCollisionsCache.insert(std::make_pair(index, MeshCollisionCacheObject()));
                meshCacheIt = iterBoolPair.first;
                if( !iterBoolPair.second )
                {
                    // there was already an element -- so we can reuse it

                    LOG_DEBUG(Logger::CORE, "Reused collision for entity '" + entity->getName() + "' (mesh: '"
                            + entity->getMesh()->getName() + "', animation: '" + animName + "').");
                    rval = iterBoolPair.first->second.col;
                }
            }
            // use convexcollisionscache
            else
            {
            }
        }

		/* differentiate between the different collision primitives, because they all
		   need different offset and probably different orientation values.
		   Newton SDK is really nifty and helps here, because we can shift the origin
		   of the coordinate system of the primitiv we create into any position we
		   desire. Actually this is the bottom middle of our mesh - as the meshes are
		   always constructed like that.
	    */
        if( !rval )
        {
            if (geomType == GT_BOX || forceBox == true)
            {
                rvalAsConvexCollision = createBox(aabb, offset, orientation);
                rval = rvalAsConvexCollision;
            }
            else if (geomType == GT_PYRAMID)
            {
                rvalAsConvexCollision = createPyramid(aabb, offset, orientation);
                rval = rvalAsConvexCollision;
            }
            else if (geomType == GT_SPHERE)
            {
                rvalAsConvexCollision = createSphere(aabb, offset, orientation);
                rval = rvalAsConvexCollision;
            }
            else if (geomType == GT_ELLIPSOID)
            {
                rvalAsConvexCollision = createEllipsoid(aabb, offset, orientation);
                rval = rvalAsConvexCollision;
            }
            else if (geomType == GT_CAPSULE)
            {
                rvalAsConvexCollision = createCapsule(aabb, offset, orientation);
                rval = rvalAsConvexCollision;
            }
            else if (geomType == GT_CONVEXHULL)
            {
                rvalAsConvexCollision = createConvexHull(entity, offset, orientation);
                rval = rvalAsConvexCollision;
            }
            else if (geomType == GT_MESH)
            {
                rval = CollisionPtr(new OgreNewt::CollisionPrimitives::TreeCollision(
                    PhysicsManager::getSingleton()._getNewtonWorld(),
                    entity, true ));

                if( meshCacheIt != mMeshCollisionsCache.end() )
                    meshCacheIt->second.col = rval;
            }
            else
            {
                Throw(IllegalArgumentException, "unknown geometry type.");
            }
        }

        if ( rval == NULL )
        {
            LOG_WARNING(Logger::CORE, " creating collision primitiv '"+
                PhysicsManager::convertGeometryTypeToString(geomType)+"' for Entity '"+
                entity->getName()+"' failed.");

            // delete cacheobject
            if( meshCacheIt != mMeshCollisionsCache.end() )
            {
                mMeshCollisionsCache.erase( meshCacheIt );
            }
            if( convexCacheIt != mConvexCollisionsCache.end() )
            {
                mConvexCollisionsCache.erase( convexCacheIt );
            }
        }
        else
        {
            LOG_DEBUG(Logger::CORE, " collision primitiv '"+
                PhysicsManager::convertGeometryTypeToString(geomType)+"' created for Entity '"+
                entity->getName()+"'");
        }

        // calculate inertia / centerOfMass if requested
        if( rvalAsConvexCollision )
        {
            if (inertia != NULL || centerOfMass != NULL )
            {
                Vector3 temp_inertia, temp_centerOfMass;
                rvalAsConvexCollision->calculateInertialMatrix(temp_inertia, temp_centerOfMass);
                if( inertia != NULL )
                    *inertia = temp_inertia*mass;
                if( centerOfMass != NULL )
                    *centerOfMass = temp_centerOfMass;
            }
        }
        else
        {
            if (inertia != NULL)
                *inertia = Ogre::Vector3::ZERO;
            if (centerOfMass != NULL)
                *centerOfMass = Ogre::Vector3::ZERO;
        }

        return rval;
    }

    OgreNewt::ConvexCollisionPtr PhysicsCollisionFactory::createBox(const Ogre::AxisAlignedBox& aabb,
            const Ogre::Vector3 &offset,
            const Ogre::Quaternion &orientation)
    {
        // offset relative to center
        Ogre::Vector3 offsetInGlobalSpace = offset + aabb.getCenter();

        // a box collision primitiv has got it's coordinate system at it's center, so we need to shift it
        OgreNewt::ConvexCollisionPtr rval(new OgreNewt::CollisionPrimitives::Box(
            PhysicsManager::getSingleton()._getNewtonWorld(),
            aabb.getSize(), orientation, offsetInGlobalSpace));

       return rval;
    }

    OgreNewt::ConvexCollisionPtr PhysicsCollisionFactory::createPyramid(const Ogre::AxisAlignedBox& aabb,
            const Ogre::Vector3 &offset,
            const Ogre::Quaternion &orientation)
    {
        // offset relative to center
        Ogre::Vector3 offsetInGlobalSpace = offset + aabb.getCenter();

        OgreNewt::ConvexCollisionPtr rval(new OgreNewt::CollisionPrimitives::Pyramid(
            PhysicsManager::getSingleton()._getNewtonWorld(),
            aabb.getSize(), orientation, offsetInGlobalSpace));

       return rval;
    }

    OgreNewt::ConvexCollisionPtr PhysicsCollisionFactory::createSphere(const Ogre::AxisAlignedBox& aabb,
            const Ogre::Vector3 &offset,
            const Ogre::Quaternion &orientation)
    {
        // offset relative to center
        Ogre::Vector3 offsetInGlobalSpace = offset + aabb.getCenter();

        Ogre::Vector3 size = aabb.getSize();
        // calculate the maximum radius needed to include 'everything'
        double radius = std::max(size.x, std::max(size.y, size.z)) / 2.0;
 
        // a sphere primitiv has got its coordinate system at its center, so shift it with radius
        OgreNewt::ConvexCollisionPtr rval(new OgreNewt::CollisionPrimitives::Ellipsoid(
            PhysicsManager::getSingleton()._getNewtonWorld(),
            Vector3(radius, radius, radius), orientation, offsetInGlobalSpace));
 
       return rval;
    }

    OgreNewt::ConvexCollisionPtr PhysicsCollisionFactory::createEllipsoid(const Ogre::AxisAlignedBox& aabb,
            const Ogre::Vector3 &offset,
            const Ogre::Quaternion &orientation)
    {
        // offset relative to center
        Ogre::Vector3 offsetInGlobalSpace = offset + aabb.getCenter();

        Ogre::Vector3 size = aabb.getSize();
        // set the size x/z values to the maximum
        Vector3 s(size/2.0);
        s.x = std::max(s.x, s.z);
        s.z = s.x;

        // an ellipsoid primitiv has got its coordinate system at its center, so shift it with radius
        OgreNewt::ConvexCollisionPtr rval(new OgreNewt::CollisionPrimitives::Ellipsoid(
            PhysicsManager::getSingleton()._getNewtonWorld(),
            s, orientation, offsetInGlobalSpace));

       return rval;
    }

    OgreNewt::ConvexCollisionPtr PhysicsCollisionFactory::createCapsule(const Ogre::AxisAlignedBox& aabb,
            const Ogre::Vector3 &offset,
            const Ogre::Quaternion &orientation)
    {
        // offset relative to center
        Ogre::Vector3 offsetInGlobalSpace = offset + aabb.getCenter();
        // @todo: fixme: this is a semi ugly fix - because the orientation should be set by the loader
        //object_orientation.FromAngleAxis(Degree(90), Vector3::UNIT_Z);

        Ogre::Vector3 size = aabb.getSize();

        double radius = std::max(size.x, size.z) / 2.0;
		double height = size.y;

        // an capsule primitiv has got its coordinate system at its center, so shift it with radius
        // additionally it is x axis aligned, so rotate it 90 degrees around z axis
        OgreNewt::ConvexCollisionPtr rval(new OgreNewt::CollisionPrimitives::Capsule(
            PhysicsManager::getSingleton()._getNewtonWorld(),
            radius, height, orientation, offsetInGlobalSpace));

        return rval;
    }



    OgreNewt::ConvexCollisionPtr PhysicsCollisionFactory::createConvexHull(
            Ogre::Entity* entity,
            const Ogre::Vector3 &offset,
            const Ogre::Quaternion &orientation)
    {
        Ogre::Vector3 offsetInGlobalSpace = offset;

        // calculate the convex hull of the animated mesh
        OgreNewt::ConvexCollisionPtr rval(new OgreNewt::CollisionPrimitives::ConvexHull(
                    PhysicsManager::getSingleton()._getNewtonWorld(),
                    entity, orientation, offsetInGlobalSpace));

        return rval;
    }


}

