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

#include "GameEventManager.h"
#include "GameAreaTypes.h"

template<> rl::GameEventManager* Ogre::Singleton<rl::GameEventManager>::ms_Singleton = 0;

namespace rl {

    GameEventManager& GameEventManager::getSingleton(void)
    {
        return Ogre::Singleton<GameEventManager>::getSingleton();
    }

    GameEventManager* GameEventManager::getSingletonPtr(void)
    {
        return Ogre::Singleton<GameEventManager>::getSingletonPtr();
    }

    GameEventManager::GameEventManager( ) :
        mAreaEventSources()
    {
        
    }

    GameEventManager::~GameEventManager( )
    {
        GameAreaEventSourceList::iterator it;
        for( it = mAreaEventSources.begin(); it != mAreaEventSources.end();++it) 
        {
            GameAreaEventSource* gam = *it;
            delete gam;
        }
        mAreaEventSources.clear();
    }

    void GameEventManager::addSphereAreaListener( Actor* actor,Ogre::Real radius,
        GameAreaListener* list, unsigned long queryMask )
    {
        // Neues Areal erzeugen
        GameAreaType* at = new GameSphereAreaType( actor->getWorldPosition(), radius, queryMask );
        // Event-Quelle erzeugen
        GameAreaEventSource* gam = new GameAreaEventSource( at, actor );
        // In die Menge einfügen
        mAreaEventSources.push_back( gam );
        // Und Listener anhängen
        gam->addAreaListener( list );        
    }

    void GameEventManager::removeAreaListener( GameAreaListener* list )
    {
        GameAreaEventSourceList::iterator it;
        for( it = mAreaEventSources.begin(); it != mAreaEventSources.end();) 
        {
            GameAreaEventSource* gam = *it;
            gam->removeAreaListener( list );

            // Sind alle Listener weggeworfen?
            if( !gam->hasListeners() )
            {            
                // Iterator zurückgeben, da sich dieser nach löschen ändert
                it = mAreaEventSources.erase(it);
                // Die Area-Art löschen
                delete gam->getGameAreaType();
                // Das Objekt löschen
                delete gam;
            }
            else
                // Iterieren
                ++it;
        }
    }

    void GameEventManager::removeAllAreas( Actor* actor )
    {
        GameAreaEventSourceList::iterator it;
        for( it = mAreaEventSources.begin(); it != mAreaEventSources.end();) 
        {
            GameAreaEventSource* gam = *it;

            // Ist das der Actor?
            if( gam->getActor() == actor )
            {      
                // Iterator zurückgeben, da sich dieser nach löschen ändert
                it = mAreaEventSources.erase(it);
                // Die Area-Art löschen
                delete gam->getGameAreaType();
                // Das Objekt löschen
                delete gam;
            }
            else
                // Iterieren
                ++it;
        }
    }

    void GameEventManager::run( Ogre::Real elapsedTime )
    {
        GameAreaEventSourceList::iterator it;
        for( it = mAreaEventSources.begin(); it != mAreaEventSources.end();++it) 
        {
            GameAreaEventSource* gam = *it;
            gam->performQuery( elapsedTime );
        }
    }
}
