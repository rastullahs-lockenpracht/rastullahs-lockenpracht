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

#ifndef __GameEventManager_H__
#define __GameEventManager_H__

#include <list>

#include "Actor.h"
#include "CorePrerequisites.h"
#include "GameAreaEventSource.h"
#include "GameAreaListener.h"
#include "GameTask.h"
#include "PhysicsGenericContactCallback.h"

namespace rl
{

    class PhysicalThing;
    class GameNewtonBodyAreaType;
    class Zone;

    typedef std::set<GameAreaEventSource*> GameAreaEventSourceList;

    /** GameEventManager
     *
     *  @see GameAreaListener, GameAreaEventSource, GameAreaEvent, GameAreaTypes
     */
    class _RlCoreExport GameEventManager : public GameTask,
                                           public Ogre::Singleton<GameEventManager>,
                                           public PhysicsGenericContactCallback
    {
    public:
        /** Default Constructor */
        GameEventManager();
        /** Default Deconstructor */
        virtual ~GameEventManager();

        /** Wird vom Gameloop aufgerufen, wenn nicht pausiert
         * Fuehrt die Anfragen der eingetragenen GameAreaEventSource durch
         * @param elapsedTime Die vergangene Zeit
         */
        virtual void run(Ogre::Real elapsedTime);

        /** DEPRECATED! Fuegt eine Sphere Area hinzu, und haengt einen Listener an
         *
         * @param actor Der Actor um den herum die Kugel aufgespannt werden soll
         * @param queryMask Die Maske um die SzenenAnfrage zu beschleunigen
         * @param radius Der Radius der Kugel
         * @param list Der neu hinzuzufuegende Listener
         * @param forceNew erstellt auf jeden Fall eine neue GameAreaEventSource (z.B. fuer den ZoneManager)
         * @retval Die GameAreaEventSource, der der Listener hinzugefuegt wurde
         */
        GameAreaEventSource* addSphereAreaListener(Actor* actor, Ogre::Real radius, GameAreaListener* list,
            unsigned long queryMask = 0xFFFFFFFF, bool forceNew = false);

        /** Fuegt eine neue Mesh-Area hinzu und haengt einen Listener an
         *
         * @param actor der actor um den herum die area gelegt werden soll, kann NULL sein
         * @param ent die Entity mit dem Mesh, das fuer die Area verwendet werden soll, falls NULL wird das mesh des
         * actors verwendet
         * @param geom der Geometrietype, sinnvoll waere GT_CONVEXHULL (eventuell GT_MESH)
         * @param list Der neu hinzuzufpgende Listener
         * @param queryMask Die Querymaske, welche Aktoren von der Area betroffen sind
         * @param forceNew erstellt auf jeden Fall eine neue GameAreaEventSource (z.B. fuer den ZoneManager)
         * @retval Die GameAreaEventSource, der der Listener hinzugefuegt wurde
         */
        GameAreaEventSource* addMeshAreaListener(Actor* actor, Ogre::Entity* ent, GeometryType geom,
            GameAreaListener* list, unsigned long queryMask = 0xFFFFFFFF, Ogre::Vector3 offset = Ogre::Vector3::ZERO,
            Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY, bool forceNew = false);

        GameAreaEventSource* addMeshAreaListener(Actor* actor, GeometryType geom, GameAreaListener* list,
            unsigned long queryMask = 0xFFFFFFFF, Ogre::Vector3 offset = Ogre::Vector3::ZERO,
            Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY, bool forceNew = false);

        /** Fuegt eine neue Area hinzu und haengt einen Listener an
         *
         * @param actor Der Actor um den herum die Area aufgespannt werden soll, kann NULL sein
         * @param aabb Die Groesse der Area
         * @param geom der Geometrietype, moeglich sind GT_BOX, GT_ELLIPSOID, GT_SPHERE, GT_PYRAMID
         * @param list Der neu hinzuzufpgende Listener
         * @param queryMask Die Querymaske, welche Aktoren von der Area betroffen sind
         * @param forceNew erstellt auf jeden Fall eine neue GameAreaEventSource (z.B. fuer den ZoneManager)
         * @retval Die GameAreaEventSource, der der Listener hinzugefuegt wurde
         */
        GameAreaEventSource* addAreaListener(Actor* actor, Ogre::AxisAlignedBox aabb, GeometryType geom,
            GameAreaListener* list, unsigned long queryMask = 0xFFFFFFFF, Ogre::Vector3 offset = Ogre::Vector3::ZERO,
            Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY, bool forceNew = false);

        /** Entfernt an allen Areas diesen Listener
        *
        * @param list Der Listener an den GameAreaEventSource
        * @note Dabei werden alle GameAreaEventSource, an denen keine
                Listener befestigt sind, entfernt und geloescht.
        */
        void removeAreaListener(GameAreaListener* list);

        /** Entfernt eine bestimmte GameAreaEventSource
         *
         */
        void removeAreaEventSource(GameAreaEventSource* gam);

        /** Entfernt alle Areas die an einen Actor geknuepft sind
         *
         * @param actor Der Actor der GameAreaEventSource
         */
        void removeAllAreas(Actor* actor);

        virtual const Ogre::String& getName() const;

        void removeQueuedDeletionSources();

        /// newton collision callback functions
        void userProcess(OgreNewt::ContactJoint& contactJoint, Ogre::Real timestep, int threadid);

        /// notify about newton world update
        void notifyNewtonWorldUpdate();

    private:
        GameAreaEventSourceList mAreaEventSources;
        GameAreaEventSourceList mQueuedDeletionSources;
        typedef std::map<OgreNewt::Body*, GameNewtonBodyAreaType*> NewtonBodyGameAreaMap;
        NewtonBodyGameAreaMap mBodyGameAreaMap;

        Actor* bodyToActor(OgreNewt::Body* body);
    };
}

#endif
