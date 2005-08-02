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

#include <OgreSingleton.h>

#include <Gangsta.h>
#include <GaCallbackInterface_Ogre.h>
#include <list>

#include "CorePrerequisites.h"


namespace rl {
    class PhysicalThing;
    class Actor;

    class _RlCoreExport PhysicsManager
        :   protected Ogre::Singleton<PhysicsManager>
    {
    public:
        enum GeometryTypes {
            GT_NONE = -1,
            GT_BOX = 0,
            GT_SPHERE = 1,
            GT_CAPSULE = 2,
            GT_MESH = 3
        };

        static int LEVELGEOMETRY_GROUP;
        
        PhysicsManager();
        virtual ~PhysicsManager();

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
        PhysicalThing* createPhysicalThing( 
            GeometryTypes geomType, 
            const Ogre::Vector3& size,
            const Ogre::Vector3& offsetPosition,
            const Ogre::Quaternion& offsetOrientation,
            Ogre::Real density = 0.0 );

        void setGravity(Ogre::Real x, Ogre::Real y, Ogre::Real z);
        const Ogre::Vector3& getGravity() const;

        void setEnabled(bool enabled);

        void createTestConnection( Actor* actor );

        // Singleton Stuff
        static PhysicsManager & getSingleton(void);
        static PhysicsManager * getSingletonPtr(void);

		/// Levelgeometrie hinzufügen
		// void addLevelGeometry( Ogre::Entity* ent );
		void toggleDebugGeometry();
        
        /// Komplette Levelgeometrie auflösen
        void clearLevelGeometry();

    private:
        typedef std::list< Ga::GaPtr<Ga::Shape> > GaShapeList;
        typedef std::list< Ga::GaPtr<Ga::Shape> >::iterator GaShapeListIterator;

        void initializePhysicsManager( );

        Ga::Manager	m_GaManager;
        Ga::GaPtr<Ga::PhysicsDriver> m_GaDriver;
        Ga::GaPtr<Ga::World> m_GaWorld;
        Ga::GaPtr<Ga::CallbackInterface_Ogre> m_GaCallback;

        GaShapeList m_LevelGeometry;

        bool m_IsEnabled;
    };
}

#endif
