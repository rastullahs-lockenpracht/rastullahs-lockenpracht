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

#ifndef __PhysicsManager_H__
#define __PhysicsManager_H__

#include <map>
#include <vector>
#include <OgreSingleton.h>

#include <OgreNewt_Collision.h>

namespace OgreNewt {
    class Body;
    class Debugger;
    class MaterialID;
    class MaterialPair;
    class World;
}


#include "GameTask.h"
#include "CorePrerequisites.h"

namespace rl {

    class Actor;
	class PhysicalObject;
    class PhysicalThing;
    class PhysicsController;
    class PhysicsGenericContactCallback;
    class World;

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
            protected Ogre::Singleton<PhysicsManager>
    {
    public:

        //! differentiates between the different collision primitives
        enum GeometryType {
            GT_NONE = -1,
            GT_BOX = 0,
            GT_SPHERE = 1,
            GT_CAPSULE = 2,
            GT_MESH = 3,
            GT_ELLIPSOID = 4,
            GT_CONVEXHULL = 5
        };

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

        //PhysicalThing* createConvexHullPhysicalThing(Ogre::Entity*, Ogre::Real mass = 0.0f,
        //    const Ogre::Vector3& inertiaCoefficients = Ogre::Vector3(1.0f, 1.0f, 1.0f));

        void removeAndDestroyPhysicalThing(PhysicalThing* thing);

        // Global Settings
        void setGravity(Ogre::Real x, Ogre::Real y, Ogre::Real z);
        Ogre::Vector3 getGravity() const;

        bool isEnabled() const;
        void setEnabled(bool enabled);

        // Singleton Stuff
        static PhysicsManager & getSingleton(void);
        static PhysicsManager * getSingletonPtr(void);

		/// Levelgeometrie hinzufügen
        void addLevelGeometry(Ogre::Entity* ent, const std::vector<OgreNewt::CollisionPtr> &collisions);
		/// Komplette Levelgeometrie auflösen
		void clearLevelGeometry();
		
		void toggleDebugMode();
        bool isDebugMode() const;

        // Newton callbacks ...
        /// generic force callback. Gravity is applied and the force,
        /// applied via PhysicalThing interface.
        static void genericForceCallback(OgreNewt::Body* body);

        /// special force callback for controlled characters (or monsters even)
        /// those bodies have up vector and are controlled using inverse dynamics
        static void controlledForceCallback(OgreNewt::Body* body);

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

        /** converts a string identifying a collision property into an enum.
         * Mainly for making string definitions of the collision property
         * possible in .gof files.
         * @param geomTypeString giving the collision primitiv.
         */
		static GeometryType convertStringToGeometryType(const Ogre::String& geomTypeString);

    private:
        //typedef std::map<PhysicalThing*, PhysicsController*> ControllerMap;
        //ControllerMap mControlledThings;

        bool mEnabled;
        OgreNewt::World* mWorld;
        OgreNewt::Debugger* mNewtonDebugger;
        std::vector<PhysicalThing*> mPhysicalThings;
        std::vector<OgreNewt::Body*> mLevelBodies;
        bool mDebugMode;
        Ogre::Vector3 mGravity;
        Ogre::AxisAlignedBox mWorldAABB;
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

        /*
        OgreNewt::MaterialID* mLevelID;
        OgreNewt::MaterialID* mCharacterID;
        OgreNewt::MaterialPair* mDefaultPair;
        OgreNewt::MaterialPair* mCharLevelPair;
        OgreNewt::MaterialPair* mCharCharPair;
        OgreNewt::MaterialPair* mCharDefaultPair;
        */

        PhysicsGenericContactCallback* mGenericCallback;
    };
}

#endif
