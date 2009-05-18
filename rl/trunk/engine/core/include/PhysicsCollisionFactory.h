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

#ifndef __PhysicsCollisionFactory_H__
#define __PhysicsCollisionFactory_H__

#include "CorePrerequisites.h"

#include <map>

#ifdef __APPLE__
#   include <OgreNewt/OgreNewt.h>
#else
#   include <OgreNewt.h>
#endif

#include "CorePrerequisites.h"
#include "CoreDefines.h"

namespace rl {

    /** Helper class for the PhysicsManager for creating collision primitives. Don't use this class directly to create
     * collision primitives, use the PhysicsManager::createCollision functions instead.
     * Additionally this class takes care about caching of collisions, loading collisions from files and saving them to
     * a file. You don't have to care about loading or saving collisions, this is only done for caching purposes.
    */
    class PhysicsCollisionFactory
    {
    public:
        //! constructor
        PhysicsCollisionFactory();


        //! destructor, releases the collisions from the collision cache
        ~PhysicsCollisionFactory();

        /** creates a collision primitive for OgreNewt from an Ogre::Entity.
		 * The collision primitive created has got a basic orientation which can be influenced by
		 * offset and orientation parameters. Additionally an initial inertiaCoefficents vector is
		 * calculated according to the size and type of collision primitiv.
         * Whenever any of the parameters is a null pointer, it is ignored.
         * The created collision primitive gets cached for the given mesh,
         * so whenever an other entity with the same mesh tries to fetch
         * a collision primitive here, then it gets back the already existing
         * one (no need to duplicate).
         * 
         * Scaling should be implemented through attaching to a scene node.
         * if that is not the case, we'll have to fix OgreNewt ...
		 * @param entity gives the mesh entity that needs a collision primitive
		 * @param geomType specifies the type of collision primitiv to create.
		 * @param animName gives the name of the currently set animation (this is used in the collisionscache)
		 * @param offset gives the offset of the coordinate system of the coll. primitiv,
         *        it is relative to the standard offset (center of the aabb of an entity for simple collision types)
		 * @param orientation Quaternion gives an euler rotation for the coordinate system of the coll. primitiv
         * @param mass gives the mass of the collision primitive used for calculating the inertia
		 * @param inertia Vector3 returns the inertia coefficients for the created collision primitiv
		 * @param centerOfMass Vector3 returns the centerOfMass coefficients for the created collision primitiv
         * @param nocache if set to true a new collision will be created (without looking for an existing one in the cache);
         *                the new collision is not added to the cache
		*/
        OgreNewt::CollisionPtr createCollisionFromEntity(
            Ogre::Entity* entity,
            const GeometryType& geomType,
            const Ogre::String& animName,
			const Ogre::Vector3 &offset,
			const Ogre::Quaternion &orientation,
            const Ogre::Real Mass,
            Ogre::Vector3* inertia,
            Ogre::Vector3* centerOfMass,
            bool nocache);

        /** creates a collision primitive for OgreNewt.
		 * The collision primitive created has got a basic orientation which can be influenced by
		 * offset and orientation parameters. Additionally an initial inertiaCoefficents vector is
		 * calculated according to the size and type of collision primitiv.
         * Whenever any of the parameters is a null pointer, it is ignored.
         * The created collision primitive gets cached for the given aabb,
         * so whenever an other aabb with the same name tries to fetch
         * a collision primitive here, then it gets back the already existing
         * one (no need to duplicate). There's no difference between Collisions created through an aabb or through
         * the entity in the cache. In order to supply a support for 'scaling' the size of the cached collision is
         * checked.
         * Since no entity is given several physical collision primitives are not
         * possible (convexhull, tree, etc.)
         * // no scaling at the moment, the correct scale must be given (OgreNewt cannot scale a collision, this is a limitation
         * // of newton. It is only possible to 'scale' a convex collision by created a ConvexHullModifier (a new collision,
         * // that uses the old one and applies transformation through a matrix), so for aabbs (-> convex collisions) a scale
         * // that udpates with the scale of the node could be implemented, but as this would be different for treecollisions,
         * // it is better to create a collision with the correct scale...
         * // "Scaling should be implemented through attaching to a scene node.
         * // if that is not the case, we'll have to fix OgreNewt ..."
         * @param name gives the name of the AxisAlignedBox
		 * @param aabb AxisAlignedBox that contains the extents for the collision primitive to be created
		 * @param geomType specifies the type of collision primitiv to create.
		 * @param offset gives the offset of the coordinate system of the coll. primitiv, it is relative to the standard offset (center of the aabb)
		 * @param orientation Quaternion gives an euler rotation for the coordinate system of the coll. primitiv
         * @param mass gives the mass of the collision primitive used for calculating the inertia
		 * @param inertia Vector3 returns the inertia coefficients for the created collision primitiv
		 * @param centerOfMass Vector3 returns the centerOfMass coefficients for the created collision primitiv
         * @param nocache if set to true a new collision will be created (without looking for an existing one in the cache);
         *                the new collision is not added to the cache
		*/
        OgreNewt::CollisionPtr createCollisionFromAABB(
            const Ogre::String& name,
            const Ogre::AxisAlignedBox& aabb,
            const GeometryType& geomType,
			const Ogre::Vector3 &offset,
			const Ogre::Quaternion &orientation,
            const Ogre::Real Mass,
            Ogre::Vector3* inertia,
            Ogre::Vector3* centerOfMass,
            bool nocache);

        /// clear the collision cache
        void clearCollisionCache();

    protected:
        /** checks if the specified size is ok for OgreNewt
         * @param aabb check the aabb's size
         */
        bool checkSize(const Ogre::AxisAlignedBox& aabb) const;

        /** corrects the specified size if it is not ok for OgreNewt
         * @param aabb correct the aabb's size
         */
        void correctSize(Ogre::AxisAlignedBox& aabb);

        /** creates a box collision primitive with offset at middle of bottom layer.
         * It's actually a convenience function used by both createCollision functions.
         * @param aabb gives the axis aligned dimension to create the primitive for.
         * @param offset when not null, specifies a different offset than the standard.
         * @param orientation when not null, specifies the euler angle of orientation.
         */
        OgreNewt::ConvexCollisionPtr createBox(const Ogre::AxisAlignedBox& aabb,
            const Ogre::Vector3 &offset,
            const Ogre::Quaternion &orientation);

        /** creates a pyramid collision primitive with offset at middle of bottom layer.
         * It's actually a convenience function used by both createCollision functions.
         * @param aabb gives the axis aligned dimension to create the primitive for.
         * @param offset when not null, specifies a different offset than the standard.
         * @param orientation when not null, specifies the euler angle of orientation.
         */
        OgreNewt::ConvexCollisionPtr createPyramid(const Ogre::AxisAlignedBox& aabb,
            const Ogre::Vector3 &offset,
            const Ogre::Quaternion &orientation);

        /** creates a sphere collision primitive with offset at middle of bottom layer.
         * It's actually a convenience function used by both createCollision functions.
         * @param aabb gives the axis aligned dimension to create the primitive for.
         * @param offset when not null, specifies a different offset than the standard.
         * @param orientation when not null, specifies the euler angle of orientation.
         */
        OgreNewt::ConvexCollisionPtr createSphere(const Ogre::AxisAlignedBox& aabb,
            const Ogre::Vector3 &offset,
            const Ogre::Quaternion &orientation);

        /** creates a ellipsoid collision primitive with offset at middle of bottom layer.
         * It's actually a convenience function used by both createCollision functions.
         * @param aabb gives the axis aligned dimension to create the primitive for.
         * @param offset when not null, specifies a different offset than the standard.
         * @param orientation when not null, specifies the euler angle of orientation.
         */
        OgreNewt::ConvexCollisionPtr createEllipsoid(const Ogre::AxisAlignedBox& aabb,
            const Ogre::Vector3 &offset,
            const Ogre::Quaternion &orientation);

        /** creates a capsule collision primitive with offset at middle of bottom layer.
         * It's actually a convenience function used by both createCollision functions.
         * @param aabb gives the axis aligned dimension to create the primitive for.
         * @param offset when not null, specifies a different offset than the standard.
         * @param orientation when not null, specifies the euler angle of orientation.
         */
        OgreNewt::ConvexCollisionPtr createCapsule(const Ogre::AxisAlignedBox& aabb,
            const Ogre::Vector3 &offset,
            const Ogre::Quaternion &orientation);

        /** creates a convex-hull collision primitive with offset at middle of bottom layer.
         * It's actually a convenience function used by createCollisionFromEntity
         * @param entity the entity for which a convexhull should be created, if it is attached to
         *               a node the scale of the node is used (see OgreNewt)
         * @param offset when not null, specifies a different offset than the standard.
         * @param orientation when not null, specifies the euler angle of orientation.
         */
        OgreNewt::ConvexCollisionPtr createConvexHull(
            Ogre::Entity* entity,
            const Ogre::Vector3 &offset,
            const Ogre::Quaternion &orientation);

        /** creates a mesh collision primitive from aabb
         * It's actually a convenience function used by createCollisionFromEntity
         * @param entity the entity for which a convexhull should be created, if it is attached to
         *               a node the scale of the node is used (see OgreNewt)
		 * @param animName gives the name of the currently set animation (this is used in the collisionscache)
         * @param nocache if true, the function doesn't try to load the collision from a cache file
         */
        OgreNewt::CollisionPtr createMesh( Ogre::Entity* entity, const Ogre::String & animName, bool nocache );


        //! class for caching convex collisions (box...convexhull), saves additionally the scale, so it can be rescaled if necessary
        class ConvexCollisionCacheObject
        {
            public:
                ConvexCollisionCacheObject() :
                    scale(Ogre::Vector3::ZERO),
                    offset(Ogre::Vector3::ZERO),
                    orientation(Ogre::Quaternion::IDENTITY),
                    inertia(Ogre::Vector3::ZERO),
                    centerOfMass(Ogre::Vector3::ZERO),
                    type(GT_NONE)
                    {}
                OgreNewt::ConvexCollisionPtr col;
                Ogre::Vector3 scale;
                Ogre::Vector3 offset;
                Ogre::Quaternion orientation;
                Ogre::Vector3 inertia, centerOfMass; // cached values
                GeometryType type;
        };

        typedef std::map<Ogre::String, ConvexCollisionCacheObject> ConvexCollisionCacheMap;

        //! cache for convex collisions created with the 'FromEntity'-function
        ConvexCollisionCacheMap mConvexCollisionsCache;

        //! cache for convex collisions created with the 'FromAABB'-function
        ConvexCollisionCacheMap mConvexAABBCollisionsCache;

        //! for using a string and a vector as key in a map... we should implement here a hash-function
        class StringVector
        {
            public:
                StringVector(const Ogre::String& str, const Ogre::Vector3& vec);
                void setString(const Ogre::String& str);
                void setVector(const Ogre::Vector3& vec);
                const Ogre::String& getString() const;
                const Ogre::Vector3& getVector() const;
                bool operator==(const StringVector& strVec) const;
                bool operator<(const StringVector& strVec) const;
            private:
                Ogre::String mStr;
                Ogre::Vector3 mVec;
        };

        //! class for caching mesh collisions
        class MeshCollisionCacheObject
        {
            public:
                MeshCollisionCacheObject()
                    {}
                OgreNewt::CollisionPtr col;
        };


        typedef std::map<StringVector, MeshCollisionCacheObject> MeshCollisionCacheMap;

        //! cache for mesh collisions
        MeshCollisionCacheMap mMeshCollisionsCache;


        //! OgreNewt collision serializer
        OgreNewt::CollisionSerializer mCollisionSerializer;

        /** load a collision from a file
         * It's actually a convenience function used internally
        */
        OgreNewt::CollisionPtr loadFromFile(const Ogre::String& filename);

        /** save a collision to a file
         * It's actually a convenience function used internally
        */
        void saveToFile(OgreNewt::CollisionPtr col, const Ogre::String& filename);

        //! the path for cached collision files
        Ogre::String mCachePathName;
    };
}

#endif

