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

#include "PhysicsCollisionFactory.h"

#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>

#include "ConfigurationManager.h"
#include "Exception.h"
#include "PhysicsManager.h"

namespace fs = boost::filesystem;

#ifdef __APPLE__
#include <Ogre/OgreMesh.h>
#else
#include <OgreMesh.h>
#endif

using namespace std;
using namespace OgreNewt;
using namespace Ogre;

namespace rl
{

    PhysicsCollisionFactory::PhysicsCollisionFactory()
        : mCollisionSerializer()
    {

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
        mCachePathName = Ogre::String(Ogre::String(::getenv("HOME")) + "/.rastullah/cache/");
#else
        mCachePathName = Ogre::String(ConfigurationManager::getSingleton().getModulesRootDirectory() + "/cache/");
#endif

        fs::path cachePath(mCachePathName);

        if (!fs::exists(cachePath))
            fs::create_directory(cachePath);
    }

    PhysicsCollisionFactory::~PhysicsCollisionFactory()
    {
        clearCollisionCache();
    }

    void PhysicsCollisionFactory::clearCollisionCache()
    {
        mMeshCollisionsCache.clear();
        mConvexCollisionsCache.clear();
        mConvexAABBCollisionsCache.clear();
    }

    bool PhysicsCollisionFactory::checkSize(const Ogre::AxisAlignedBox& aabb) const
    {
        Ogre::Vector3 size = aabb.getSize();
        if (size.x < PhysicsManager::NEWTON_GRID_WIDTH || size.y < PhysicsManager::NEWTON_GRID_WIDTH
            || size.z < PhysicsManager::NEWTON_GRID_WIDTH)
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
        aabb.setMaximum(center + 0.5 * size);
        aabb.setMinimum(center - 0.5 * size);
    }

    OgreNewt::CollisionPtr PhysicsCollisionFactory::createCollisionFromAABB(const Ogre::String& name,
        const Ogre::AxisAlignedBox& passedAabb, const GeometryType& geomType, const Ogre::Vector3& offset,
        const Ogre::Quaternion& orientation, const Ogre::Real mass, Ogre::Vector3* inertia, Ogre::Vector3* centerOfMass,
        bool nocache)
    {

        // don't cache if the name is ""
        if (name == "")
            nocache = true;

        Ogre::AxisAlignedBox aabb(passedAabb);
        // type for the collision primitiv (can change internally here)
        bool forceBox(false);

        // result value
        ConvexCollisionPtr rval;

        // size check (if object is too small, it falls back to a box primitiv
        if (checkSize(aabb) == false)
        {
            correctSize(aabb);
            LOG_WARNING(Logger::CORE,
                " AABB is too small, using 'box' instead of primitiv '"
                    + PhysicsManager::convertGeometryTypeToString(geomType));
            forceBox = true;
        }

        // check if the geometry type is supported for aabb
        if (geomType == GT_CONVEXHULL || geomType == GT_MESH)
        {
            LOG_WARNING(Logger::CORE,
                " the geometry type '" + PhysicsManager::convertGeometryTypeToString(geomType)
                    + "' is not supported for aabb, fail back to box");
            forceBox = true;
        }

        ConvexCollisionCacheMap::iterator convexCacheIt = mConvexAABBCollisionsCache.end();

        // check if we can reuse a cached collision
        if (!nocache)
        {
            if (!forceBox)
            {
                Ogre::String index(name);
                std::pair<ConvexCollisionCacheMap::iterator, bool> iterBoolPair
                    = mConvexAABBCollisionsCache.insert(std::make_pair(index, ConvexCollisionCacheObject()));
                convexCacheIt = iterBoolPair.first;
                if (!iterBoolPair.second)
                {
                    // there was already an element, check type, the size, offset etc if we can use it
                    if (geomType != convexCacheIt->second.type)
                    {
                        LOG_MESSAGE(Logger::CORE,
                            "Performance warning: trying to create a collision for aabb '" + name
                                + "' with geometry-type '" + PhysicsManager::convertGeometryTypeToString(geomType)
                                + "'. There's already a collision with type '"
                                + PhysicsManager::convertGeometryTypeToString(convexCacheIt->second.type) + "'!");
                    }
                    else if (aabb.getSize() == convexCacheIt->second.scale && offset == convexCacheIt->second.offset
                        && orientation == convexCacheIt->second.orientation)
                    {
                        // everything fine, reuse it
                        rval = convexCacheIt->second.col;

                        if (inertia != NULL)
                            *inertia = convexCacheIt->second.inertia;
                        if (centerOfMass != NULL)
                            *centerOfMass = convexCacheIt->second.centerOfMass;

                        LOG_DEBUG(Logger::CORE, "Reused collision for aabb '" + name + "'.");

                        return rval;
                    }
                }
            }
        }

        /* differentiate between the different collision primitives, because they all
           need different offset and probably different orientation values.
           Newton SDK is really nifty and helps here, because we can shift the origin
           of the coordinate system of the primitiv we create into any position we
           desire. Actually this is the bottom middle of our mesh - as the meshes are
           always constructed like that.
        */
        if (!rval)
        {
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
        }

        if (rval == NULL)
        {
            LOG_WARNING(Logger::CORE,
                " creating collision primitiv '" + PhysicsManager::convertGeometryTypeToString(geomType)
                    + "' for AABB '" + name + "' failed.");

            // delete cacheobject
            if (convexCacheIt != mConvexAABBCollisionsCache.end())
            {
                mConvexAABBCollisionsCache.erase(convexCacheIt);
            }
        }
        else
        {
            LOG_DEBUG(Logger::CORE,
                " collision primitiv '" + PhysicsManager::convertGeometryTypeToString(geomType) + "' created for AABB '"
                    + name + "'");
        }

        // calculate inertia / centerOfMass if requested
        // and save collision in cache
        if (rval)
        {
            if (convexCacheIt != mConvexAABBCollisionsCache.end())
            {
                // save collision in cache
                Vector3 temp_inertia, temp_centerOfMass;
                rval->calculateInertialMatrix(temp_inertia, temp_centerOfMass);

                convexCacheIt->second.col = rval;
                convexCacheIt->second.scale = aabb.getSize();
                convexCacheIt->second.offset = offset;
                convexCacheIt->second.orientation = orientation;
                convexCacheIt->second.inertia = temp_inertia;
                convexCacheIt->second.centerOfMass = temp_centerOfMass;
                convexCacheIt->second.type = geomType;

                if (inertia != NULL)
                    *inertia = temp_inertia * mass;
                if (centerOfMass != NULL)
                    *centerOfMass = temp_centerOfMass;
            }
            else if (inertia != NULL || centerOfMass != NULL)
            {
                Vector3 temp_inertia, temp_centerOfMass;
                rval->calculateInertialMatrix(temp_inertia, temp_centerOfMass);
                if (inertia != NULL)
                    *inertia = temp_inertia * mass;
                if (centerOfMass != NULL)
                    *centerOfMass = temp_centerOfMass;
            }
        }

        return rval;
    }

    OgreNewt::CollisionPtr PhysicsCollisionFactory::createCollisionFromEntity(Ogre::Entity* entity,
        const GeometryType& geomType, const Ogre::String& animName, const Ogre::Vector3& offset,
        const Ogre::Quaternion& orientation, const Ogre::Real mass, Ogre::Vector3* inertia, Ogre::Vector3* centerOfMass,
        bool nocache)
    {
        // bounding box of the mesh
        Ogre::AxisAlignedBox aabb(entity->getBoundingBox());
        // apply scale if attached to a node (like in OgreNewt for convexhull)
        Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
        if (entity->getParentNode())
        {
            scale = entity->getParentNode()->getScale();
            aabb.scale(scale);
        }

        // type for the collision primitiv (can change internally here)
        bool forceBox(false);

        // result value
        CollisionPtr rval;
        ConvexCollisionPtr
            rvalAsConvexCollision; // store pointer to ConvexCollision, so we don't need to cast
                                   // this also indicates, that it is possible to calculate inertia/centerOfMass

        // size check (if object is too small, it falls back to a box primitiv
        if (checkSize(aabb) == false)
        {
            correctSize(aabb);
            LOG_MESSAGE(Logger::CORE,
                " Entity '" + entity->getName() + "' is too small, using 'box' instead of primitiv '"
                    + PhysicsManager::convertGeometryTypeToString(geomType));
            forceBox = true;
        }

        if (geomType == GT_MESH)
        {
            if (offset != Ogre::Vector3::ZERO || orientation != Ogre::Quaternion::IDENTITY)
            {
                LOG_WARNING(Logger::CORE,
                    " Cannot set collision offset or orientation when using mesh-collision (entity: '"
                        + entity->getName() + "')!");
            }
            if (inertia || centerOfMass)
            {
                LOG_WARNING(Logger::CORE,
                    " Cannot calculate inertia or center of mass when using mesh-collision (entity: '"
                        + entity->getName() + "')!");
            }
        }

        MeshCollisionCacheMap::iterator meshCacheIt = mMeshCollisionsCache.end();
        ConvexCollisionCacheMap::iterator convexCacheIt = mConvexCollisionsCache.end();

        // check if we can reuse a cached collision
        if (!nocache)
        {
            // use meshcollisionscache
            if (geomType == GT_MESH && !forceBox)
            {
                StringVector index(entity->getMesh()->getName() + animName, scale);
                std::pair<MeshCollisionCacheMap::iterator, bool> iterBoolPair
                    = mMeshCollisionsCache.insert(std::make_pair(index, MeshCollisionCacheObject()));
                meshCacheIt = iterBoolPair.first;
                if (!iterBoolPair.second)
                {
                    // there was already an element -- so we can reuse it

                    LOG_DEBUG(Logger::CORE,
                        "Reused collision for entity '" + entity->getName() + "' (mesh: '"
                            + entity->getMesh()->getName() + "', animation: '" + animName + "').");
                    rval = iterBoolPair.first->second.col;

                    return rval;
                }
            }
            // use convexcollisionscache
            else if (!forceBox)
            {
                Ogre::String index(entity->getMesh()->getName() + animName);
                std::pair<ConvexCollisionCacheMap::iterator, bool> iterBoolPair
                    = mConvexCollisionsCache.insert(std::make_pair(index, ConvexCollisionCacheObject()));
                convexCacheIt = iterBoolPair.first;
                if (!iterBoolPair.second)
                {
                    // there was already an element, check type, the size, offset etc if we can use it
                    if (geomType != convexCacheIt->second.type)
                    {
                        LOG_MESSAGE(Logger::CORE,
                            "Performance warning: trying to create a collision for entity '" + entity->getName()
                                + "' with animation '" + animName + "' with geometry-type '"
                                + PhysicsManager::convertGeometryTypeToString(geomType)
                                + "'. There's already a collision with type '"
                                + PhysicsManager::convertGeometryTypeToString(convexCacheIt->second.type) + "'!");
                    }
                    else if (scale == convexCacheIt->second.scale && offset == convexCacheIt->second.offset
                        && orientation == convexCacheIt->second.orientation)
                    {
                        // everything fine, reuse it
                        rval = convexCacheIt->second.col;

                        if (inertia != NULL)
                            *inertia = convexCacheIt->second.inertia;
                        if (centerOfMass != NULL)
                            *centerOfMass = convexCacheIt->second.centerOfMass;

                        LOG_DEBUG(Logger::CORE,
                            "Reused collision for entity '" + entity->getName() + "' (mesh: '"
                                + entity->getMesh()->getName() + "', animation: '" + animName + "').");

                        return rval;
                    }
                    else if (geomType == GT_CONVEXHULL) // for simple types just use create a new collision
                    {
                        // we need to rescale (or change offset or orientation)
                        // the old transformation
                        Ogre::Matrix4 oldInvTransformation;
                        oldInvTransformation.makeInverseTransform(convexCacheIt->second.offset,
                            convexCacheIt->second.scale, convexCacheIt->second.orientation);
                        // the new transformation
                        Ogre::Matrix4 newTransformation;
                        newTransformation.makeTransform(offset, scale, orientation);

                        // the needed transformation from old to new:
                        Ogre::Matrix4 transformation = oldInvTransformation * newTransformation;

                        OgreNewt::ConvexModifierCollision* convexModCol = new OgreNewt::ConvexModifierCollision(
                            PhysicsManager::getSingleton()._getNewtonWorld(), convexCacheIt->second.col, 0);
                        convexModCol->setScalarMatrix(transformation);

                        // set the convexcollision-ptr, so centerOfMass and inertia can be calculated
                        rval = OgreNewt::ConvexCollisionPtr(convexModCol);

                        LOG_DEBUG(Logger::CORE,
                            "Reused collision for entity '" + entity->getName() + "' (mesh: '"
                                + entity->getMesh()->getName() + "', animation: '" + animName
                                + "') with a ConvexModifierCollision.");

                        // calculate inertia / centerOfMass if needed
                        if (inertia != NULL || centerOfMass != NULL)
                        {
                            Vector3 temp_inertia, temp_centerOfMass;
                            rvalAsConvexCollision->calculateInertialMatrix(temp_inertia, temp_centerOfMass);
                            if (inertia != NULL)
                                *inertia = temp_inertia * mass;
                            if (centerOfMass != NULL)
                                *centerOfMass = temp_centerOfMass;
                        }

                        return rval;
                    }
                }
            }
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
            rval = createMesh(entity, animName, nocache);

            if (meshCacheIt != mMeshCollisionsCache.end())
                meshCacheIt->second.col = rval;
        }
        else
        {
            Throw(IllegalArgumentException, "unknown geometry type.");
        }

        if (rval == NULL)
        {
            LOG_WARNING(Logger::CORE,
                " creating collision primitiv '" + PhysicsManager::convertGeometryTypeToString(geomType)
                    + "' for Entity '" + entity->getName() + "' failed.");

            // delete cacheobject
            if (meshCacheIt != mMeshCollisionsCache.end())
            {
                mMeshCollisionsCache.erase(meshCacheIt);
            }
            if (convexCacheIt != mConvexCollisionsCache.end())
            {
                mConvexCollisionsCache.erase(convexCacheIt);
            }
        }
        else
        {
            LOG_DEBUG(Logger::CORE,
                " collision primitiv '" + PhysicsManager::convertGeometryTypeToString(geomType)
                    + "' created for Entity '" + entity->getName() + "'");
        }

        // calculate inertia / centerOfMass if requested
        // and save collision in cache
        if (rvalAsConvexCollision)
        {
            if (convexCacheIt != mConvexCollisionsCache.end())
            {
                // save collision in cache
                Vector3 temp_inertia, temp_centerOfMass;
                rvalAsConvexCollision->calculateInertialMatrix(temp_inertia, temp_centerOfMass);

                convexCacheIt->second.col = rvalAsConvexCollision;
                convexCacheIt->second.scale = scale;
                convexCacheIt->second.offset = offset;
                convexCacheIt->second.orientation = orientation;
                convexCacheIt->second.inertia = temp_inertia;
                convexCacheIt->second.centerOfMass = temp_centerOfMass;
                convexCacheIt->second.type = geomType;

                if (inertia != NULL)
                    *inertia = temp_inertia * mass;
                if (centerOfMass != NULL)
                    *centerOfMass = temp_centerOfMass;
            }
            else if (inertia != NULL || centerOfMass != NULL)
            {
                Vector3 temp_inertia, temp_centerOfMass;
                rvalAsConvexCollision->calculateInertialMatrix(temp_inertia, temp_centerOfMass);
                if (inertia != NULL)
                    *inertia = temp_inertia * mass;
                if (centerOfMass != NULL)
                    *centerOfMass = temp_centerOfMass;
            }
        }

        return rval;
    }

    OgreNewt::ConvexCollisionPtr PhysicsCollisionFactory::createBox(
        const Ogre::AxisAlignedBox& aabb, const Ogre::Vector3& offset, const Ogre::Quaternion& orientation)
    {
        // offset relative to center
        Ogre::Vector3 offsetInGlobalSpace = offset + aabb.getCenter();

        // a box collision primitiv has got it's coordinate system at it's center, so we need to shift it
        OgreNewt::ConvexCollisionPtr rval(new OgreNewt::CollisionPrimitives::Box(
            PhysicsManager::getSingleton()._getNewtonWorld(), aabb.getSize(), 0, orientation, offsetInGlobalSpace));

        return rval;
    }

    OgreNewt::ConvexCollisionPtr PhysicsCollisionFactory::createPyramid(
        const Ogre::AxisAlignedBox& aabb, const Ogre::Vector3& offset, const Ogre::Quaternion& orientation)
    {
        // offset relative to center
        Ogre::Vector3 offsetInGlobalSpace = offset + aabb.getCenter();

        OgreNewt::ConvexCollisionPtr rval(new OgreNewt::CollisionPrimitives::Pyramid(
            PhysicsManager::getSingleton()._getNewtonWorld(), aabb.getSize(), 0, orientation, offsetInGlobalSpace));

        return rval;
    }

    OgreNewt::ConvexCollisionPtr PhysicsCollisionFactory::createSphere(
        const Ogre::AxisAlignedBox& aabb, const Ogre::Vector3& offset, const Ogre::Quaternion& orientation)
    {
        // offset relative to center
        Ogre::Vector3 offsetInGlobalSpace = offset + aabb.getCenter();

        Ogre::Vector3 size = aabb.getSize();
        // calculate the maximum radius needed to include 'everything'
        double radius = std::max(size.x, std::max(size.y, size.z)) / 2.0;

        // a sphere primitiv has got its coordinate system at its center, so shift it with radius
        OgreNewt::ConvexCollisionPtr rval(
            new OgreNewt::CollisionPrimitives::Ellipsoid(PhysicsManager::getSingleton()._getNewtonWorld(),
                Vector3(radius, radius, radius), 0, orientation, offsetInGlobalSpace));

        return rval;
    }

    OgreNewt::ConvexCollisionPtr PhysicsCollisionFactory::createEllipsoid(
        const Ogre::AxisAlignedBox& aabb, const Ogre::Vector3& offset, const Ogre::Quaternion& orientation)
    {
        // offset relative to center
        Ogre::Vector3 offsetInGlobalSpace = offset + aabb.getCenter();

        Ogre::Vector3 size = aabb.getSize();
        // set the size x/z values to the maximum
        Vector3 s(size / 2.0);
        s.x = std::max(s.x, s.z);
        s.z = s.x;

        // an ellipsoid primitiv has got its coordinate system at its center, so shift it with radius
        OgreNewt::ConvexCollisionPtr rval(new OgreNewt::CollisionPrimitives::Ellipsoid(
            PhysicsManager::getSingleton()._getNewtonWorld(), s, 0, orientation, offsetInGlobalSpace));

        return rval;
    }

    OgreNewt::ConvexCollisionPtr PhysicsCollisionFactory::createCapsule(
        const Ogre::AxisAlignedBox& aabb, const Ogre::Vector3& offset, const Ogre::Quaternion& orientation)
    {
        // offset relative to center
        Ogre::Vector3 offsetInGlobalSpace = offset + aabb.getCenter();
        // @todo: fixme: this is a semi ugly fix - because the orientation should be set by the loader
        // object_orientation.FromAngleAxis(Degree(90), Vector3::UNIT_Z);

        Ogre::Vector3 size = aabb.getSize();

        double radius = std::max(size.x, size.z) / 2.0;
        double height = size.y;

        // an capsule primitiv has got its coordinate system at its center, so shift it with radius
        // additionally it is x axis aligned, so rotate it 90 degrees around z axis
        OgreNewt::ConvexCollisionPtr rval(new OgreNewt::CollisionPrimitives::Capsule(
            PhysicsManager::getSingleton()._getNewtonWorld(), radius, height, 0, orientation, offsetInGlobalSpace));

        return rval;
    }

    OgreNewt::ConvexCollisionPtr PhysicsCollisionFactory::createConvexHull(
        Ogre::Entity* entity, const Ogre::Vector3& offset, const Ogre::Quaternion& orientation)
    {
        Ogre::Vector3 offsetInGlobalSpace = offset;

        // calculate the convex hull of the animated mesh
        OgreNewt::ConvexCollisionPtr rval(new OgreNewt::CollisionPrimitives::ConvexHull(
            PhysicsManager::getSingleton()._getNewtonWorld(), entity, 0, orientation, offsetInGlobalSpace));

        return rval;
    }

    OgreNewt::CollisionPtr PhysicsCollisionFactory::loadFromFile(const Ogre::String& filename)
    {
        OgreNewt::CollisionPtr rval;
        Ogre::String fullfilename = mCachePathName + filename;
        FILE* pFile = fopen(fullfilename.c_str(), "r");
        if (pFile)
        {
            Ogre::DataStreamPtr streamPtr(new Ogre::FileHandleDataStream(pFile));
            rval = mCollisionSerializer.importCollision(streamPtr, PhysicsManager::getSingleton()._getNewtonWorld());

            if (rval)
            {
                LOG_DEBUG(Logger::CORE, "Loaded collision from file '" + filename + "'.");
            }
        }

        return rval;
    }

    void PhysicsCollisionFactory::saveToFile(OgreNewt::CollisionPtr col, const Ogre::String& filename)
    {
        mCollisionSerializer.exportCollision(col, mCachePathName + filename);
    }

    OgreNewt::CollisionPtr PhysicsCollisionFactory::createMesh(
        Ogre::Entity* entity, const Ogre::String& animName, bool nocache)
    {
        OgreNewt::CollisionPtr rval;
        Ogre::String filename;

        if (!nocache)
        {
            Ogre::Vector3 scale(Ogre::Vector3::UNIT_SCALE);
            if (entity->getParentNode())
                scale = entity->getParentNode()->getScale();

            filename = entity->getMesh()->getName() + animName + Ogre::StringConverter::toString(scale) + ".collision";

            // check date of cache file and mesh
            time_t meshFileModified = Ogre::ResourceGroupManager::getSingleton().resourceModifiedTime(
                entity->getMesh()->getGroup(), entity->getMesh()->getName());

            fs::path cacheFilePath(mCachePathName + filename);

            time_t cacheFileModified = 0;
            if (fs::exists(cacheFilePath))
                cacheFileModified = fs::last_write_time(cacheFilePath);

            if (cacheFileModified > meshFileModified)
                rval = loadFromFile(filename);
        }

        if (!rval)
        {
            rval = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::TreeCollision(
                PhysicsManager::getSingleton()._getNewtonWorld(), entity, true, 0));

            if (!nocache)
            {
                saveToFile(rval, filename);
            }
        }

        return rval;
    }

    PhysicsCollisionFactory::StringVector::StringVector(const Ogre::String& str, const Ogre::Vector3& vec)
    {
        setString(str);
        setVector(vec);
    }

    void PhysicsCollisionFactory::StringVector::setString(const Ogre::String& str)
    {
        mStr = str;
    }

    void PhysicsCollisionFactory::StringVector::setVector(const Ogre::Vector3& vec)
    {
        mVec = vec;
    }

    const Ogre::String& PhysicsCollisionFactory::StringVector::getString() const
    {
        return mStr;
    }

    const Ogre::Vector3& PhysicsCollisionFactory::StringVector::getVector() const
    {
        return mVec;
    }

    bool PhysicsCollisionFactory::StringVector::operator==(const PhysicsCollisionFactory::StringVector& strVec) const
    {
        if (mStr != strVec.mStr)
            return false;

        Ogre::Vector3 diff = mVec - strVec.mVec;
        if (abs(diff.x) > 0.01)
            return false;
        if (abs(diff.y) > 0.01)
            return false;
        if (abs(diff.z) > 0.01)
            return false;

        return true;
    }

    bool PhysicsCollisionFactory::StringVector::operator<(const PhysicsCollisionFactory::StringVector& strVec) const
    {
        int strCompare = mStr.compare(strVec.mStr);
        // @todo: use a hash or so
        if (strCompare == 0) // strings are the same
        {
            // compare vectors
            Ogre::Vector3 diff = mVec - strVec.mVec;
            if (diff.x < -0.01)
                return true;
            if (diff.x > 0.01)
                return false;
            if (diff.y < -0.01)
                return true;
            if (diff.y > 0.01)
                return false;
            if (diff.y < -0.01)
                return true;

            return false;
        }

        return strCompare < 0;
    }
}
