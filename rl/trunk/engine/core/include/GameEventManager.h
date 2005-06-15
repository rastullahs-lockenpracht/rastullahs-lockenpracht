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

#ifndef __GameEventManager_H__
#define __GameEventManager_H__

#include <list>
#include <OgreSingleton.h>

#include "Actor.h"
#include "GameTask.h"
#include "GameAreaEventSource.h"
#include "GameAreaListener.h"
#include "CorePrerequisites.h"

namespace rl {

typedef std::list<GameAreaEventSource*> GameAreaEventSourceList;

/** GameEventManager
 *  
 *  @see GameAreaListener, GameAreaEventSource, GameAreaEvent, GameAreaTypes
 */
class _RlCoreExport GameEventManager
  : public GameTask,
    protected Ogre::Singleton<GameEventManager>
{
public:
    /** Default Constructor */
    GameEventManager( );
    /** Default Deconstructor */
    virtual ~GameEventManager();

    /** Wird vom Gameloop aufgerufen, wenn nicht pausiert
     * Führt die Anfragen der eingetragenen GameAreaEventSource durch
     * @param elapsedTime Die vergangene Zeit
     */
    virtual void run( Ogre::Real elapsedTime );

    /** Fügt eine Sphere Area hinzu, und hängt einen Listener an
    *
    * @param actor Der Actor um den herum die Kugel aufgespannt werden soll
    * @param queryMask Die Maske um die SzenenAnfrage zu beschleunigen
    * @param radius Der Radius der Kugel
    * @param list Der neu hinzuzufügende Listener    
    */
    void addSphereAreaListener( Actor* actor, Ogre::Real radius,
        GameAreaListener* list, unsigned long queryMask = 0xFFFFFFFF );

    /** Entfernt an allen Areas diesen Listener
    *
    * @param list Der Listener an den GameAreaEventSource
    * @note Dabei werden alle GameAreaEventSource, an denen keine
            Listener befestigt sind, entfernt und gelöscht.
    */
    void removeAreaListener( GameAreaListener* list );

    /** Entfernt alle Areas die an einen Actor geknüpft sind
      *
      * @param actor Der Actor der GameAreaEventSource
      */
    void removeAllAreas( Actor* actor );

    /// Singleton
    static GameEventManager & getSingleton(void);
    /// Singleton
    static GameEventManager * getSingletonPtr(void);
private:
    GameAreaEventSourceList m_AreaEventSources;
};

}

#endif
