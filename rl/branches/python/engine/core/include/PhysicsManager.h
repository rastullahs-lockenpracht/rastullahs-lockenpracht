/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#ifndef __PhysicsManager_H__
#define __PhysicsManager_H__

#include <map>
#include <vector>
#include <OgreSingleton.h>
#include <OgreOde_Core.h>
#include "GameTask.h"

#include "CorePrerequisites.h"


namespace rl {

    class PhysicalThing;
    class Actor;
    class World;

    class _RlCoreExport PhysicsManager
        :   public GameTask,
            public OgreOde::CollisionListener,
            public OgreOde::StepListener,
            protected Singleton<PhysicsManager>
    {
    public:

        enum GeometryTypes {
            GT_NONE = -1,
            GT_BOX = 0,
            GT_SPHERE = 1,
            GT_CAPSULE = 2,
            GT_MESH = 3
        };
        
        /// Typ bestimmt, wo der Usrprung (0/0/0) des Objektes liegt.
        enum OffsetMode {
            /// Ursprung in Objektmitte
            OM_CENTERED = 0,
            /// Ursprung Unten-Mitte
            OM_BOTTOMCENTERED
        };
        
        PhysicsManager(rl::World* world);
        virtual ~PhysicsManager();

        virtual void run( Real elapsedTime );

        /**
         * @param geomType Grundform der Geometrie des Objektes
         * @param size Größe des Objektes in cm
         * @param density die Dichte, wenn <= 0.0, dann wird keine Physik
         *        dafür erstellt, sondern nur Geometry für die Kollision.
         * @param offsetMode bestimmt, wo der lokale Koordinatenursprung sitzt.
         *
         * @todo Geometry-Kapselung verallgemeinern. z.B. funktioniert Capusle
         *       momentan nur dann gut, wenn die Höhe die Y-Achse ist.
         */
        PhysicalThing* createPhysicalThing(const int geomType, const Ogre::Vector3& size,
            Real density, OffsetMode offsetMode = OM_BOTTOMCENTERED);

        void removeAndDestroyPhysicalThing(PhysicalThing* thing);

        // Global Settings
        void setGravity(Ogre::Real x, Ogre::Real y, Ogre::Real z);
        Vector3 getGravity();
        void setCFM(Ogre::Real cfm);
        Real getCFM();
        void setERP(Ogre::Real erp);
        Real getERP();

        OgreOde::World* getWorld();

        void createLevelGeometry(Ogre::SceneNode* levelNode);
        OgreOde::Geometry* getLevelGeometry();

        void setEnabled(bool enabled);

        // Singleton Stuff
        static PhysicsManager & getSingleton(void);
        static PhysicsManager * getSingletonPtr(void);

        void addCollisionListener(OgreOde::CollisionListener*);
        void removeCollisionListener(OgreOde::CollisionListener*);

		/// CollisionListener callback
		virtual bool collision(OgreOde::Contact* contact);

        /// StepListener callback
        virtual bool preStep(Real time);
    private:
        bool mEnabled;

        std::vector<PhysicalThing*> mPhysicalThings;
        std::vector<OgreOde::CollisionListener*> mCollisionListeners;
        OgreOde::World* mOdeWorld;
        OgreOde::Space* mGlobalSpace;
        OgreOde::Stepper* mOdeStepper;
        OgreOde::Geometry* mOdeLevel;
        World* mWorld;
    };
}

#endif
