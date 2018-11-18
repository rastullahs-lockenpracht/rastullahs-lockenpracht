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

#ifndef __GameAreaTypes_H__
#define __GameAreaTypes_H__

#include "Actor.h"
#include "CorePrerequisites.h"
#include "CoreDefines.h"

namespace OgreNewt {
	class Body;
}
namespace rl {

	class PhysicalThing;

/** GameAreaType
 * Verwaltet verschiedene Flaechenarten und Methoden die Aktoren innerhalb dieser Flaechen zu
 * sammeln. Dabei lassen sich die gefundenen Aktoren ueber Masken beschraenken.
 *
 *  @see GameAreaListener, GameAreaEventSource, GameEventManager, GameAreaEvent
 */
class _RlCoreExport GameAreaType
{
public:
    virtual ~GameAreaType();
    /** F�hrt die Anfrage durch und sammelt die Aktoren innerhalb einer std::map 
     *  Key Wert ist der einzigartige Name des Actors
     *
     *  Die genaue Implementation wird in den Unterklassen durchgef�hrt.
     */
    virtual ActorMap performQuery(  ) = 0;

    /// Gibt die Anfrage-Maske zur�ck, wird in Unterklassen implementiert
    virtual unsigned long getQueryMask() const = 0;
    /// Setzt die Anfrage-Maske, wird in Unterklassen implementiert
    virtual void setQueryMask( unsigned long mask = 0xFFFFFFFF ) = 0;

    /// F�gt der Anfrage-Maske ein Flag hinzu
    void addQueryFlag( unsigned long flag  );
    /// Entfernt ein Flag aus der Anfrage-Maske
    void removeQueryFlag( unsigned long flag );

    /// Setzt die genaue Position der Anfrage, wird in Unterklassen implementiert
    virtual void setQueryPosition( const Ogre::Vector3& vec ) = 0;
    /// Gibt die Position der Anfrage zur�ck, wird in Unterklassen implementiert
    virtual Ogre::Vector3 getQueryPosition() const = 0;
    /// Setzt die genaue Orientierung der Anfrage, wird in Unterklassen implementiert
    virtual void setQueryOrientation( const Ogre::Quaternion& ori ) = 0;
    /// Gibt die Orientierung der Anfrage zur�ck, wird in Unterklassen implementiert
    virtual Ogre::Quaternion getQueryOrientation() const = 0;

    /// Blendet ein Debug-Objekt ein, um das Areal zu verdeutlichen
    //virtual setShowDebug() = 0;
    /// Gibt zur�ck ob ein Debug-Objekt eingeblendet ist
    //virtual bool getShowDebug() const = 0;

    virtual OgreNewt::Body* getBody() { return NULL;}

    virtual Ogre::Real getDistance(Actor* actor) = 0;

    /// Der minimale abstand, den ein aktor von der zone haben muss, um die zone zu verlassen (um st�ndige wechsel in und aus der zone zu vermeiden)
    virtual Ogre::Real getTransitionDistance() const = 0;
    virtual void setTransitionDistance(Ogre::Real dist) = 0;

    /// Gibt true zurueck, wenn sich der Actor in der Area befindet
    virtual bool isInside(Actor *actor) = 0;
};

/** GameSphereAreaType
* Implementierung einer Kugelanfrage
*/
/*
class _RlCoreExport GameSphereAreaType : public GameAreaType
{
public:
    /** Konstruktor f�r eine Kugelanfrage 
     *  
     * @param center Das Zentrum der Kugel
     * @param radius Der Radius der Kugel
     * @param mask Die Anfrage Maske
    
    GameSphereAreaType(Ogre::Vector3 center, Ogre::Real radius, unsigned long mask = 0xFFFFFFFF );
    /// Dekonstruktor
    virtual ~GameSphereAreaType();

    /// Implementierung der Anfrage �ber einen SphereSceneQuery
    virtual ActorMap performQuery(  );

    /// Gibt die Anfrage-Maske zur�ck
    virtual unsigned long getQueryMask() const;
    /// Setzt die Anfrage-Maske
    virtual void setQueryMask( unsigned long mask = 0xFFFFFFFF );

     /// Setzt die genaue Position der Anfrage
    virtual void setQueryPosition( const Ogre::Vector3& vec );
    /// Gibt die Position der Anfrage zur�ck
    virtual Ogre::Vector3 getQueryPosition() const;
    /// Setzt die genaue Orientierung der Anfrage, wird in Unterklassen implementiert
    virtual void setQueryOrientation( const Ogre::Quaternion& ori ) {}
    /// Gibt die Orientierung der Anfrage zur�ck, wird in Unterklassen implementiert
    virtual Ogre::Quaternion getQueryOrientation() const {return Ogre::Quaternion::IDENTITY;}
private:
    /// Die Kugel-Anfrage
    Ogre::SphereSceneQuery* mSphereQuery;
};
*/

/// ein Problem k�nnte die zeitliche Verschiebung um eine Framedauer sein, wenn position und orientation
/// neu gesetzt werden, muss erst newton wieder upgedated werden!
/// Von dieser Klasse muss abgleitet werden, sie definiert keinen eigenen body!
class _RlCoreExport GameNewtonBodyAreaType : 
    public GameAreaType
{
public:
    /// Konstruktor
    GameNewtonBodyAreaType();

    /// Destruktor
    virtual ~GameNewtonBodyAreaType();

    /// Implementierung der Anfrage �ber einen SphereSceneQuery
    virtual ActorMap performQuery(  );

    /// Gibt die Anfrage-Maske zur�ck
    virtual unsigned long getQueryMask() const;
    /// Setzt die Anfrage-Maske
    virtual void setQueryMask( unsigned long mask = 0xFFFFFFFF );

     /// Setzt die genaue Position der Anfrage
    virtual void setQueryPosition( const Ogre::Vector3& vec );
    /// Gibt die Position der Anfrage zur�ck
    virtual Ogre::Vector3 getQueryPosition() const;
    /// Setzt die genaue Orientierung der Anfrage, wird in Unterklassen implementiert
    virtual void setQueryOrientation( const Ogre::Quaternion& ori );
    /// Gibt die Orientierung der Anfrage zur�ck, wird in Unterklassen implementiert
    virtual Ogre::Quaternion getQueryOrientation() const;

    virtual void foundCollision(Actor* actor);

    virtual void resetFoundCollisions();

    virtual OgreNewt::Body* getBody() { return mBody;}

    Ogre::Real getDistance(Actor* actor);

    Ogre::Real getTransitionDistance() const;
    void setTransitionDistance(Ogre::Real dist);

    /// Gibt true zurueck, wenn sich der Actor in der Area befindet
    virtual bool isInside(Actor *actor);
protected:
    OgreNewt::Body* mBody;
    ActorMap mFoundActors;
    unsigned long mQueryMask;
    Ogre::Real mTransitionDistance;
};


class _RlCoreExport GameMeshAreaType :
    public GameNewtonBodyAreaType
{
public:
    GameMeshAreaType(
            Ogre::Entity* entity,
            GeometryType geomType = GT_CONVEXHULL,
            Ogre::Vector3 offset = Ogre::Vector3::ZERO,
            Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY);
};

class _RlCoreExport GameSimpleCollisionAreaType :
    public GameNewtonBodyAreaType
{
public:
    GameSimpleCollisionAreaType(
            Ogre::AxisAlignedBox aabb,
            GeometryType geomType = GT_BOX,
            Ogre::Vector3 offset = Ogre::Vector3::ZERO,
            Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY);
};


}

#endif
