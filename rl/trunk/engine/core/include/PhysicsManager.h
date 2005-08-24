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
        
        PhysicsManager();
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
			Real density, OgreOde::Space* space = NULL, OffsetMode offsetMode = OM_BOTTOMCENTERED);

        void removeAndDestroyPhysicalThing(PhysicalThing* thing);

        // Global Settings
        void setGravity(Ogre::Real x, Ogre::Real y, Ogre::Real z);
        Vector3 getGravity();
        void setCFM(Ogre::Real cfm);
        Real getCFM();
        void setERP(Ogre::Real erp);
        Real getERP();

        OgreOde::World* getWorld();

        void setEnabled(bool enabled);

        // Singleton Stuff
        static PhysicsManager & getSingleton(void);
        static PhysicsManager * getSingletonPtr(void);

        void addCollisionListener(OgreOde::CollisionListener*);
        void removeCollisionListener(OgreOde::CollisionListener*);
		/// CollisionListener callback
		virtual bool collision(OgreOde::Contact* contact);


		/// Levelgeometrie hinzufügen
		void addLevelGeometry( Ogre::Entity* ent );
		/// Komplette Levelgeometrie auflösen
		void clearLevelGeometry(  );

		

        /// StepListener callback
        virtual bool preStep(Real time);

		void setActor(OgreOde::Geometry* actor, Ogre::SceneNode* controlNode);
		void setCamera(OgreOde::Geometry* camera, Ogre::SceneNode* cameraNode);
		OgreOde::Geometry* getActor();
		OgreOde::Geometry* getCamera();

		void setFallSpeed(Ogre::Real fallspeed);
		Ogre::Real getFallSpeed();

		void toggleDebugOde();

    private:
		bool collisionWithPlayerActor(OgreOde::Geometry* geometry, OgreOde::Contact* contact);
		bool collisionCameraWithLevel(OgreOde::Contact* contact);

        bool mEnabled;

        std::vector<PhysicalThing*> mPhysicalThings;
        std::vector<OgreOde::CollisionListener*> mCollisionListeners;
        OgreOde::World* mOdeWorld;
        OgreOde::Space* mGlobalSpace;

		OgreOde::Space* mLevelGeomSpace;

        OgreOde::Stepper* mOdeStepper;

		OgreOde::Geometry* mOdeActor;
		OgreOde::Geometry* mOdeCamera;
		Ogre::SceneNode* mControlNode;
		Ogre::SceneNode* mCameraNode;
		Ogre::Real mFallSpeed;
    };
}

#endif
