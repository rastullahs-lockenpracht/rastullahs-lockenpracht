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
#include <OgreNewt/OgreNewt.h>
#else
#include <OgreNewt.h>
#endif

#include "CoreDefines.h"
#include "CorePrerequisites.h"
#include "GameTask.h"
#include "QuadTree.h"

namespace rl
{

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
        LQTBodies(int maxData, int maxDepth, float looseness, const Ogre::Vector2& tlc, const Ogre::Vector2& brc,
            float mWidth);
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
    class _RlCoreExport PhysicsManager : public GameTask, public Ogre::Singleton<PhysicsManager>
    {
    public:
        static const Ogre::Real NEWTON_GRID_WIDTH;

        //! default constructor
        PhysicsManager();
        /** explicit virtual destructor.
         * frees any acquired memory.
         */
        virtual ~PhysicsManager();

        virtual void run(Ogre::Real elapsedTime);

        /**
         * @param geomType Grundform der Geometrie des Objektes
         * @param size Groesse des Objektes in cm
         * @param mass die Masse in kg, wenn <= 0.0 ist es ein statisches Objekt
         * @param offsetMode bestimmt, wo der lokale Koordinatenursprung sitzt.
         * @param hullModifer soll ein Collision in NewtonConvexHullModifier gewrapped werden?
         *        Das ist bei sich schnell bewegenden bodies hilfreich um tunneling zu verhindern.
         *
         * @todo Geometry-Kapselung verallgemeinern. z.B. funktioniert Capusle
         *       momentan nur dann gut, wenn die Hoehe die Y-Achse ist.
         */
        /*PhysicalThing* createPhysicalThing(const int geomType, PhysicalObject* po,
            Ogre::Real mass, OffsetMode offsetMode = OM_BOTTOMCENTERED, bool hullModifier = false);*/
        PhysicalThing* createPhysicalThing(
            GeometryType geomType, PhysicalObject* po, Ogre::Real mass, bool hullModifier = false);

        /**
         * Erschafft den entgueltigen Physikproxy
         */
        void createPhysicsProxy(PhysicalThing* pt);

        /**
         * Creates a ragdoll as physics proxy
         */
        void createPhysicsProxy_RagDoll(PhysicalThing* pt);

        /**
         * Removes the physics proxy
         * @param pt the physics proxy wrapper; is not deleted
         */
        void destroyPhysicsProxy(PhysicalThing* pt);

        // PhysicalThing* createConvexHullPhysicalThing(Ogre::Entity*, Ogre::Real mass = 0.0f,
        //    const Ogre::Vector3& inertiaCoefficients = Ogre::Vector3(1.0f, 1.0f, 1.0f));

        void removeAndDestroyPhysicalThing(PhysicalThing* thing);

        // Global Settings
        void setGravity(Ogre::Real x, Ogre::Real y, Ogre::Real z);
        Ogre::Vector3 getGravity() const;
        void setTimeFactor(Ogre::Real factor);

        bool isEnabled() const;
        void setEnabled(bool enabled);

        /// Levelgeometrie hinzufuegen
        void addLevelGeometry(Ogre::Entity* ent, const std::vector<OgreNewt::CollisionPtr>& collisions);
        /// Komplette Levelgeometrie aufloesen
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

        // gibt die AxisAlignedBox der Welt zurueck
        const Ogre::AxisAlignedBox& getWorldAab(void)
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
        OgreNewt::MaterialPair* createMaterialPair(const OgreNewt::MaterialID* M1, const OgreNewt::MaterialID* M2);
        /** retrieves a material by name.
         * @param M1 material id of first material
         * @param M2 material id of second material
         * @returns the specified materialpair object
         */
        OgreNewt::MaterialPair* getMaterialPair(const OgreNewt::MaterialID* M1, const OgreNewt::MaterialID* M2) const;
        /** reset MaterialPair to default.
         * @param M1 material id of first material
         * @param M2 material id of second material
         */
        void resetMaterialPair(const OgreNewt::MaterialID* M1, const OgreNewt::MaterialID* M2);

        /// calls PhysicsCollisionFactory::createCollisionFromEntity
        OgreNewt::CollisionPtr createCollision(Ogre::Entity* entity, const GeometryType& geomType = GT_NONE,
            const Ogre::String& animName = "", const Ogre::Vector3& offset = Ogre::Vector3::ZERO,
            const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY, const Ogre::Real mass = 0,
            Ogre::Vector3* inertia = nullptr, Ogre::Vector3* centerOfMass = nullptr, bool nocache = false);

        /// calls PhysicsCollisionFactory::createCollisionFromAABB
        OgreNewt::CollisionPtr createCollision(const Ogre::String& name, const Ogre::AxisAlignedBox& aabb,
            const GeometryType& geomType = GT_NONE, const Ogre::Vector3& offset = Ogre::Vector3::ZERO,
            const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY, const Ogre::Real mass = 0,
            Ogre::Vector3* inertia = nullptr, Ogre::Vector3* centerOfMass = nullptr, bool nocache = false);

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
        OgreNewt::Debugger* getNewtonDebugger()
        {
            return &mWorld->getDebugger();
        }

    private:
        bool mEnabled;
        //! the globally used physical representation of the world by Newton
        OgreNewt::World* mWorld;
        //! debug mode: 0 no debugging, 1 show debug lines (freezed state), 2 show debug lines (update every frame), 3
        //! show raycasts from one frame, 4 upda raycasts every frame
        int mDebugMode;

        //! factory for creating new collision primitives
        PhysicsCollisionFactory* mPhysicsCollisionFactory;

        //! a list of objects of the physical world
        std::vector<PhysicalThing*> mPhysicalThings;
        //! a list of bodies for the static level parts
        // std::vector<OgreNewt::Body*> mLevelBodies;
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
        typedef std::pair<const OgreNewt::MaterialID*, const OgreNewt::MaterialID*> PairOfMaterials;

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
        typedef std::multimap<PairOfMaterials, OgreNewt::MaterialPair*, ltPairOfMaterials> MaterialPairMap;
        //! shortens the type definition for pairs for the stl multimap
        typedef std::pair<PairOfMaterials, OgreNewt::MaterialPair*> MaterialPairMapPair;

        //! contains a list materials with string id as a key
        MaterialMap mMaterials;
        //! contains a list of materialpairs (for different collisionhandling)
        MaterialPairMap mMaterialPairs;

        //! generic physics contact callback handler object
        PhysicsGenericContactCallback* mGenericCallback;

#ifdef _DEBUG
        void logBodyProperties(const OgreNewt::Body* body);
#endif
    };
}

#endif
