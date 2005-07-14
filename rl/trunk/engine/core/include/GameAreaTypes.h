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

#ifndef __GameAreaTypes_H__
#define __GameAreaTypes_H__

#include <map>

#include "Actor.h"
#include "CorePrerequisites.h"

namespace rl {

typedef std::map<const Ogre::String,Actor*> ActorMap;
typedef std::pair<const Ogre::String,Actor*> ActorPair;

/** GameAreaType
 * Verwaltet verschiedene Flaechenarten und Methoden die Aktoren innerhalb dieser Flächen zu
 * sammeln. Dabei lassen sich die gefundenen Aktoren über Masken beschränken.
 *
 *  @see GameAreaListener, GameAreaEventSource, GameEventManager, GameAreaEvent
 */
class _RlCoreExport GameAreaType
{
public:
    /** Führt die Anfrage durch und sammelt die Aktoren innerhalb einer std::map 
     *  Key Wert ist der einzigartige Name des Actors
     *
     *  Die genaue Implementation wird in den Unterklassen durchgeführt.
     */
    virtual ActorMap performQuery(  ) = 0;

    /// Gibt die Anfrage-Maske zurück, wird in Unterklassen implementiert
    virtual unsigned long getQueryMask() const = 0;
    /// Setzt die Anfrage-Maske, wird in Unterklassen implementiert
    virtual void setQueryMask( unsigned long mask = 0xFFFFFFFF ) = 0;

    /// Fügt der Anfrage-Maske ein Flag hinzu
    void addQueryFlag( unsigned long flag  );
    /// Entfernt ein Flag aus der Anfrage-Maske
    void removeQueryFlag( unsigned long flag );

    /// Setzt die genaue Position der Anfrage, wird in Unterklassen implementiert
    virtual void setQueryPosition( const Ogre::Vector3& vec ) = 0;
    /// Gibt die Position der Anfrage zurück, wird in Unterklassen implementiert
    virtual const Ogre::Vector3& getQueryPosition() const = 0;
    /// Blendet ein Debug-Objekt ein, um das Areal zu verdeutlichen
    //virtual setShowDebug() = 0;
    /// Gibt zurück ob ein Debug-Objekt eingeblendet ist
    //virtual bool getShowDebug() const = 0;
private:
};

/** GameSphereAreaType
* Implementierung einer Kugelanfrage
*/
class _RlCoreExport GameSphereAreaType : public GameAreaType
{
public:
    /** Konstruktor für eine Kugelanfrage 
     *  
     * @param center Das Zentrum der Kugel
     * @param radius Der Radius der Kugel
     * @param mask Die Anfrage Maske
     */
    GameSphereAreaType(Ogre::Vector3 center, Ogre::Real radius, unsigned long mask = 0xFFFFFFFF );
    /// Dekonstruktor
    virtual ~GameSphereAreaType();

    /// Implementierung der Anfrage über einen SphereSceneQuery
    virtual ActorMap performQuery(  );

    /// Gibt die Anfrage-Maske zurück
    virtual unsigned long getQueryMask() const;
    /// Setzt die Anfrage-Maske
    virtual void setQueryMask( unsigned long mask = 0xFFFFFFFF );

     /// Setzt die genaue Position der Anfrage
    virtual void setQueryPosition( const Ogre::Vector3& vec );
    /// Gibt die Position der Anfrage zurück
    virtual const Ogre::Vector3& getQueryPosition() const;    
private:
    /// Die Kugel-Anfrage
    Ogre::SphereSceneQuery* m_SphereQuery;
};

}

#endif
