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

#ifndef __PhysicsManager_H__
#define __PhysicsManager_H__

#include "CorePrerequisites.h"

#include <map>
#include <vector>

#ifdef __APPLE__
#   include <OgreNewt/OgreNewt.h>
#else
#   include <OgreNewt.h>
#endif

namespace OgreNewt {
    class Body;
    class MaterialID;
    class MaterialPair;
    class World;
}

#include "CorePrerequisites.h"
#include "CoreDefines.h"
#include "GameTask.h"
#include "QuadTree.h"

namespace rl {

    class Actor;
	class PhysicalObject;
    class PhysicalThing;
    class PhysicsController;
    class PhysicsGenericContactCallback;
    class World;
    class PhysicsCollisionFactory;

	class LQTBodies : public TLooseQuadTreeNode<OgreNewt::Body*, LQTBodies>
	{
	public:
		typedef std::vector<OgreNewt::Body*> BodyList;
		LQTBodies(int maxData, int maxDepth, float looseness, const Ogre::Vector2& tlc,
			const Ogre::Vector2& brc, float mWidth);
        LQTBodies(const LQTBodies& LQT);
		~LQTBodies();

		static const Ogre::AxisAlignedBox getAABB(OgreNewt::Body* body);
	};

    /** Management class for the physical properties of game world objects.
     * This class utilizes OgreNewt (and therefore indirectly Newton) for handling
     * the physics of RL. It sets up OgreNewt and realizes the timestepping for Newton.
     * Additionally it manages materials which are needed for handling different kinds
     * of object collisions. It also installs a basic forcefeedback function per object,
     * in a way that is perhaps not the quickest one, but removes the need for a per object
     * registration.
     * So basically this class handles the global settings and things related 
     * to the object world.
     * It also manages a list of physical controllers (that are objects that
     * modify physical properties of a specific object).
     */
    class _RlCoreExport PhysicsManager
        :   public GameTask,
            public Ogre::Singleton<PhysicsManager>
    {
    public:

        static const Ogre::Real NEWTON_GRID_WIDTH;
        
        //! default constructor
        PhysicsManager();
        /** explicit virtual destructor.
         * frees any acquired memory.
         */
        virtual ~PhysicsManager();

        virtual void run( Ogre::Real elapsedTime );

        /**
         * @param geomType Grundform der Geometrie des Objektes
         * @param size Größe des Objektes in cm
         * @param mass die Masse in kg, wenn <= 0.0 ist es ein statisches Objekt
         * @param offsetMode bestimmt, wo der lokale Koordinatenursprung sitzt.
         * @param hullModifer soll ein Collision in NewtonConvexHullModifier gewrapped werden?
         *        Das ist bei sich schnell bewegenden bodies hilfreich um tunneling zu verhindern.
         *
         * @todo Geometry-Kapselung verallgemeinern. z.B. funktioniert Capusle
         *       momentan nur dann gut, wenn die Höhe die Y-Achse ist.
         */
        /*PhysicalThing* createPhysicalThing(const int geomType, PhysicalObject* po,
            Ogre::Real mass, OffsetMode offsetMode = OM_BOTTOMCENTERED, bool hullModifier = false);*/
		PhysicalThing* createPhysicalThing(GeometryType geomType, PhysicalObject* po,
            Ogre::Real mass, bool hullModifier = false);

		/**
		 * Erschafft den entgültigen Physikproxy
		 */
		void createPhysicsProxy(PhysicalThing* pt, Ogre::SceneNode* node);

		/**
		 * Removes the physics proxy
		 * @param pt the physics proxy wrapper; is not deleted
		 */
		void destroyPhysicsProxy(PhysicalThing* pt);

        //PhysicalThing* createConvexHullPhysicalThing(Ogre::Entity*, Ogre::Real mass = 0.0f,
        //    const Ogre::Vector3& inertiaCoefficients = Ogre::Vector3(1.0f, 1.0f, 1.0f));

        void removeAndDestroyPhysicalThing(PhysicalThing* thing);

        // Global Settings
        void setGravity(Ogre::Real x, Ogre::Real y, Ogre::Real z);
        Ogre::Vector3 getGravity() const;
        void setTimeFactor(Ogre::Real factor);

        bool isEnabled() const;
        void setEnabled(bool enabled);

		/// Levelgeometrie hinzufügen
        void addLevelGeometry(Ogre::Entity* ent, const std::vector<OgreNewt::CollisionPtr> &collisions);
		/// Komplette Levelgeometrie auflösen
		void clearLevelGeometry();
		
	void toggleDebugMode();
        int isDebugMode() const;

        // Newton callbacks ...
        /// generic force callback. Gravity is applied and the force,
        /// applied via PhysicalThing interface.
        static void genericForceCallback(OgreNewt::Body* body, float timeStep, int threadIndex);

        /// special force callback for controlled characters (or monsters even)
        /// those bodies have up vector and are controlled using inverse dynamics
        static void controlledForceCallback(OgreNewt::Body* body, float timeStep, int threadIndex);

        OgreNewt::World* _getNewtonWorld() const;

        virtual const Ogre::String& getName() const;

        // gibt die AxisAlignedBox der Welt zurück
        const Ogre::AxisAlignedBox &getWorldAab(void)
        {
            return mWorldAABB;
        }

        Ogre::Real getMinTimestep(void) const
        {
            return mMinTimestep;
        }
        Ogre::Real getMaxTimestep(void) const
        {
            return mMaxTimestep;
        }

        // Material handling (for different types of collision)
        /** retrieves a material by name.
         * @param materialname string identifying the material
         * @returns the specified material object
         */
        const OgreNewt::MaterialID* getMaterialID(const Ogre::String& materialname) const;
        /** creates a material by name.
         * Whenever the material exists already, the existing object is returned.
         * @param materialname string identifying the material
         * @returns the newly created material object (or the already present one)
         */
        const OgreNewt::MaterialID* createMaterialID(const Ogre::String& materialname);

        /** creates a materialpair.
         * Whenever the materialpair exists already, the existing object is returned.
         * @param M1 material id of first material
         * @param M2 material id of second material
         * @returns the created materialpair object (or the already present one)
         */
        OgreNewt::MaterialPair* createMaterialPair(const OgreNewt::MaterialID* M1,
            const OgreNewt::MaterialID* M2);
        /** retrieves a material by name.
         * @param M1 material id of first material
         * @param M2 material id of second material
         * @returns the specified materialpair object
         */
        OgreNewt::MaterialPair* getMaterialPair(const OgreNewt::MaterialID* M1,
            const OgreNewt::MaterialID* M2) const;
        /** reset MaterialPair to default.
         * @param M1 material id of first material
         * @param M2 material id of second material
         */
        void resetMaterialPair( const OgreNewt::MaterialID* M1,
            const OgreNewt::MaterialID* M2);

        /** creates a collision primitive for OgreNewt.
		 * The collision primitive created has got a basic orientation which can be influenced by
		 * offset and orientation parameters. Additionally an initial inertiaCoefficents vector is
		 * calculated according to the size and type of collision primitiv.
         * Whenever any of the parameters is a null pointer, it is ignored.
         * The created collision primitive gets cached for the given mesh,
         * so whenever an other entity with the same mesh tries to fetch
         * a collision primitive here, then it gets back the already existing
         * one (no need to duplicate).
		 * @param entity gives the mesh entity that needs a collision primitive
         * @param geomType defines the geometry of the collision type
         * @param offset Vector3 gives the offset of the coordinate system of the coll. primitiv
		 * @param orientation Quaternion gives an euler rotation for the coordinate system of the coll. primitiv
         * @param mass gives the mass of the collision primitive used for calculating the inertia
		 * @param inertia Vector3 returns the inertia coefficients for the created collision primitiv
		*/
		OgreNewt::CollisionPtr createCollision(
			Ogre::Entity* entity,
            const GeometryType& geomType = GT_NONE,
            const Ogre::String animName = "",
			Ogre::Vector3* offset = NULL,
			Ogre::Quaternion* orientation = NULL,
            const Ogre::Real mass = 0,
            Ogre::Vector3* inertia = NULL);

        /** creates a collision primitive for OgreNewt.
		 * The collision primitive created has got a basic orientation which can be influenced by
		 * offset and orientation parameters. Additionally an initial inertiaCoefficents vector is
		 * calculated according to the size and type of collision primitiv.
         * Whenever any of the parameters is a null pointer, it is ignored.
         * The created collision primitive gets cached for the given aabb,
         * so whenever an other aabb with the same name tries to fetch
         * a collision primitive here, then it gets back the already existing
         * one (no need to duplicate).
		 * @param aabb AxisAlignedBox that contains the extents for the collision primitive to be created
         * @param name gives the name of the AxisAlignedBox
         * @param geomType defines the geometry of the collision type
         * @param offset Vector3 gives the offset of the coordinate system of the coll. primitiv
		 * @param orientation Quaternion gives an euler rotation for the coordinate system of the coll. primitiv
         * @param mass gives the mass of the collision primitive used for calculating the inertia
		 * @param inertia Vector3 returns the inertia coefficients for the created collision primitiv
		*/
        OgreNewt::CollisionPtr createCollision(
            const Ogre::String& name,
            const Ogre::AxisAlignedBox& aabb,
            const GeometryType& geomType = GT_NONE,
            Ogre::Vector3* offset = NULL,
			Ogre::Quaternion* orientation = NULL,
            const Ogre::Real mass = 0,
            Ogre::Vector3* inertia = NULL);

        /** Makes the collision primitive generation available to non mesh objects.
         * Non mesh objects need collision objects too. Therefore they can
         * utilise this function for bypassing the caching mechanism of
         * PhysicalManager.
         * @returns a collision primitive creating factory object.
         */
        PhysicsCollisionFactory* getCollisionFactory();

        /** converts a string identifying a collision property into an enum.
         * Mainly for making string definitions of the collision property
         * possible in .gof files.
         * @param geomTypeString giving the collision primitiv.
         */
		static GeometryType convertStringToGeometryType(const Ogre::String& geomTypeString);

        /** converts an enum into a string identifying a collision property.
         * Mainly for making string definitions of the collision property
         * possible in error messages.
         * @param geomType enum giving the collision primitiv.
         */
        static Ogre::String convertGeometryTypeToString(const GeometryType& geomType);

        /// returns newton debugger
        OgreNewt::Debugger* getNewtonDebugger() {return &mWorld->getDebugger();}

    private:

        /** structure containing further information about the collision primitive.
         * Actually this information should go into either the collisionptr or the
         * object for the primitive ...
         */
        struct CollisionInUse
        {
        public:
            CollisionInUse() : colPtr(NULL), geomType(GT_NONE) {}
            GeometryType geomType;  //! primitive type
            OgreNewt::CollisionPtr colPtr;          //! the collision primitve
        };

        /** shortens definition of a list of collision primitives.
         * currently only one geometry type per entity (mesh) is allowed.
         * if multiple should be possible, the geometry type should be
         * moved from the above struct into the key.
         */
        typedef std::map< std::string, CollisionInUse > CollisionMap;

        bool mEnabled;
        //! the globally used physical representation of the world by Newton
        OgreNewt::World* mWorld;
        //! debug mode: 0 no debugging, 1 show debug lines (freezed state), 2 show debug lines (update every frame), 3 show raycasts from one frame, 4 upda raycasts every frame
        int mDebugMode;

        //! factory for creating new collision primitives
        PhysicsCollisionFactory* mPhysicsCollisionFactory;
        //! a list of collision primitives
        CollisionMap mCollisionPrimitives;

        //! a list of objects of the physical world
        std::vector<PhysicalThing*> mPhysicalThings;
        //! a list of bodies for the static level parts
        //std::vector<OgreNewt::Body*> mLevelBodies;
		//! a quadtree storing a spatial partioning of static level parts
        TLooseQuadTree<OgreNewt::Body*, LQTBodies> mLevelBodiesQuadTree;
        //! the extents of the level
        Ogre::AxisAlignedBox mWorldAABB;
        
        //! the globally known gravity force
        Ogre::Vector3 mGravity;

        Ogre::Real mTimeFactor;
        
        // time stuff
        Ogre::Real mElapsed;
        Ogre::Real mMinTimestep;
        Ogre::Real mMaxTimestep;

        // Material handling needed for different types of collisions
        //! shortens the type definition for maps of materials
        typedef std::map<const Ogre::String, const OgreNewt::MaterialID*> MaterialMap;
        //! shortens the creation of pair types for the stl map
        typedef std::pair<const Ogre::String, const OgreNewt::MaterialID*> MaterialMapPair;

        //! defines a pair of MaterialIDs
        typedef std::pair< const OgreNewt::MaterialID*, const OgreNewt::MaterialID* > PairOfMaterials;

        //! defines a comparison operator for pairs of MaterialIDs
        struct ltPairOfMaterials
        {
            bool operator()(const PairOfMaterials P1, const PairOfMaterials P2) const
            {
                if (P1.first->getID() == P2.first->getID())
                    return (P1.second->getID() < P2.second->getID());

                return (P1.first->getID() < P2.first->getID());
            }
        };

        //! shortens the type definition for maps of materialpairs
        typedef std::multimap< PairOfMaterials, OgreNewt::MaterialPair*, ltPairOfMaterials > MaterialPairMap;
        //! shortens the type definition for pairs for the stl multimap
        typedef std::pair< PairOfMaterials, OgreNewt::MaterialPair* > MaterialPairMapPair;

        //! contains a list materials with string id as a key
        MaterialMap mMaterials;
        //! contains a list of materialpairs (for different collisionhandling)
        MaterialPairMap mMaterialPairs;

        //! generic physics contact callback handler object
        PhysicsGenericContactCallback* mGenericCallback;

#ifdef _DEBUG
        void logBodyProperties( const OgreNewt::Body* body );
#endif

    };

    class PhysicsCollisionFactory
    {
    public:
        /** checks if the specified size is ok for OgreNewt
         * @param size to check
         */
        bool checkSize(const Ogre::Vector3& size) const;
        /** corrects the specified size if it is not ok for OgreNewt
         * @param size to correct
         */
        void correctSize(Ogre::Vector3& size);
        /** calculates the Inertia for the given primitive type
         */
        //Ogre::Vector3 calculateIntertia(const Ogre::Real& Mass, Ogre::Vector3* inertiaCoefficients);

        /** creates a collision primitive for OgreNewt an Ogre::Entity.
		 * The collision primitive created has got a basic orientation which can be influenced by
		 * offset and orientation parameters.
         * Whenever any of the parameters is a null pointer, it is ignored.
         * Scaling should be implemented through attaching to a scene node.
         * if that is not the case, we'll have to fix OgreNewt ...
		 * @param entity Ogre::Entity mesh object
		 * @param geomType specifies the type of collision primitiv to create.
		 * @param offset gives the offset of the coordinate system of the coll. primitiv
		 * @param orientation Quaternion gives an euler rotation for the coordinate system of the coll. primitiv
         * @param mass gives the mass of the collision primitive used for calculating the inertia
		 * @param inertia Vector3 returns the inertia coefficients for the created collision primitiv
		*/
        OgreNewt::CollisionPtr createCollisionFromEntity(Ogre::Entity* entity,
            const GeometryType& geomType,
            Ogre::Vector3* offset = NULL, 
            Ogre::Quaternion* orientation = NULL,
            const Ogre::Real Mass = 0,
            Ogre::Vector3* inertiaCoefficients = NULL);

        /** creates a collision primitive for OgreNewt from an AABB box.
		 * The collision primitive created has got a basic orientation which can be influenced by
		 * offset and orientation parameters.
         * Whenever any of the parameters is a null pointer, it is ignored.
         * Since no entity is given several physical collision primitives are not
         * possible (convexhull, tree, etc.)
         * Scaling should be implemented through attaching to a scene node.
         * if that is not the case, we'll have to fix OgreNewt ...
		 * @param entity Ogre::Entity mesh object
		 * @param geomType specifies the type of collision primitiv to create.
		 * @param offset gives the offset of the coordinate system of the coll. primitiv
		 * @param orientation Quaternion gives an euler rotation for the coordinate system of the coll. primitiv
         * @param mass gives the mass of the collision primitive used for calculating the inertia
		 * @param inertia Vector3 returns the inertia coefficients for the created collision primitiv
		*/
        OgreNewt::CollisionPtr createCollisionFromAABB(const Ogre::AxisAlignedBox aabb,
            const GeometryType& geomType,
            Ogre::Vector3* offset = NULL,
            Ogre::Quaternion* orientation = NULL,
            const Ogre::Real Mass = 0,
            Ogre::Vector3* inertiaCoefficients = NULL);
    protected:
        /** creates a box collision primitive with offset at middle of bottom layer.
         * It's actually a convenience function used by both createCollision functions.
         * @param aabb gives the axis aligned dimension to create the primitive for.
         * @param offset when not null, specifies a different offset than the standard.
         * @param orientation when not null, specifies the euler angle of orientation.
         * @param mass gives the mass for inertia calculation.
         * @param inertia when not null, an inertia is calculated and passed back.
         */
        OgreNewt::CollisionPtr createBox(const Ogre::AxisAlignedBox& aabb,
            Ogre::Vector3* offset = NULL,
            Ogre::Quaternion* orientation = NULL,
            const Ogre::Real mass = 0,
            Ogre::Vector3* inertia = NULL);
        /** creates a pyramid collision primitive with offset at middle of bottom layer.
         * It's actually a convenience function used by both createCollision functions.
         * @param aabb gives the axis aligned dimension to create the primitive for.
         * @param offset when not null, specifies a different offset than the standard.
         * @param orientation when not null, specifies the euler angle of orientation.
         * @param mass gives the mass for inertia calculation.
         * @param inertia when not null, an inertia is calculated and passed back.
         */
        OgreNewt::CollisionPtr createPyramid(const Ogre::AxisAlignedBox& aabb,
            Ogre::Vector3* offset = NULL,
            Ogre::Quaternion* orientation = NULL,
            const Ogre::Real mass = 0,
            Ogre::Vector3* inertia = NULL);
        /** creates a sphere collision primitive with offset at middle of bottom layer.
         * It's actually a convenience function used by both createCollision functions.
         * @param aabb gives the axis aligned dimension to create the primitive for.
         * @param offset when not null, specifies a different offset than the standard.
         * @param orientation when not null, specifies the euler angle of orientation.
         * @param mass gives the mass for inertia calculation.
         * @param inertia when not null, an inertia is calculated and passed back.
         */
        OgreNewt::CollisionPtr createSphere(const Ogre::AxisAlignedBox& aabb,
            Ogre::Vector3* offset = NULL,
            Ogre::Quaternion* orientation = NULL,
            const Ogre::Real mass = 0,
            Ogre::Vector3* inertia = NULL);
        /** creates a ellipsoid collision primitive with offset at middle of bottom layer.
         * It's actually a convenience function used by both createCollision functions.
         * @param aabb gives the axis aligned dimension to create the primitive for.
         * @param offset when not null, specifies a different offset than the standard.
         * @param orientation when not null, specifies the euler angle of orientation.
         * @param mass gives the mass for inertia calculation.
         * @param inertia when not null, an inertia is calculated and passed back.
         */
        OgreNewt::CollisionPtr createEllipsoid(const Ogre::AxisAlignedBox& aabb,
            Ogre::Vector3* offset = NULL,
            Ogre::Quaternion* orientation = NULL,
            const Ogre::Real mass = 0,
            Ogre::Vector3* inertia = NULL);
        /** creates a capsule collision primitive with offset at middle of bottom layer.
         * It's actually a convenience function used by both createCollision functions.
         * @param aabb gives the axis aligned dimension to create the primitive for.
         * @param offset when not null, specifies a different offset than the standard.
         * @param orientation when not null, specifies the euler angle of orientation.
         * @param mass gives the mass for inertia calculation.
         * @param inertia when not null, an inertia is calculated and passed back.
         */
        OgreNewt::CollisionPtr createCapsule(const Ogre::AxisAlignedBox& aabb,
            Ogre::Vector3* offset = NULL,
            Ogre::Quaternion* orientation = NULL,
            const Ogre::Real mass = 0,
            Ogre::Vector3* inertia = NULL);
    };
}

#endif
