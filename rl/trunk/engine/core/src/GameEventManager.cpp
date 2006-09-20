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

#include "GameEventManager.h"
#include "GameAreaTypes.h"
#include "ScriptWrapper.h"
#include "CoreSubsystem.h"


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
        mAreaEventSources(),
		mQueuedDeletionSources()
    {
        
    }

    GameEventManager::~GameEventManager( )
    {
        GameAreaEventSourceList::iterator it;
        for( it = mAreaEventSources.begin(); it != mAreaEventSources.end();++it) 
        {
            GameAreaEventSource* gam = *it;
			ScriptWrapper::getSingleton().deleted( gam );
			delete gam->getGameAreaType();
            delete gam;
        }
        mAreaEventSources.clear();
		mQueuedDeletionSources.clear();
    }

	/// @todo  Doppelte Aktoren nachnutzen??
    void GameEventManager::addSphereAreaListener( Actor* actor, Ogre::Real radius,
        GameAreaListener* list, unsigned long queryMask )
    {
        // Neues Areal erzeugen
        GameAreaType* at = new GameSphereAreaType( actor->getWorldPosition(), radius, queryMask );
        // Event-Quelle erzeugen
        GameAreaEventSource* gam = new GameAreaEventSource( at, actor );
        // In die Menge einfügen
        mAreaEventSources.insert( gam );
		ScriptWrapper::getSingleton().owned( gam );
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
				// Später löschen
				mQueuedDeletionSources.insert( gam );
            }

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
				// Später löschen
                mQueuedDeletionSources.insert( gam );
            }
            
            // Iterieren
            ++it;
        }
    }

	void GameEventManager::removeQueuedDeletionSources()
	{
		while(!mQueuedDeletionSources.empty())
		{
			GameAreaEventSource* gam = *mQueuedDeletionSources.begin();

			mAreaEventSources.erase(mAreaEventSources.find(gam));
			ScriptWrapper::getSingleton().deleted( gam );
			mQueuedDeletionSources.erase(mQueuedDeletionSources.begin());

			// Die Area-Art löschen
			delete gam->getGameAreaType();
			// Das Objekt löschen
			delete gam;
		}
		
	}

    void GameEventManager::run( Ogre::Real elapsedTime )
    {
        removeQueuedDeletionSources();

        GameAreaEventSourceList::iterator it;
        for( it = mAreaEventSources.begin(); it != mAreaEventSources.end();++it) 
        {
            GameAreaEventSource* gam = *it;
            gam->performQuery( elapsedTime );
        }
	}

    const Ogre::String& GameEventManager::getName() const
    {
        static Ogre::String NAME = "GameEventManager";

        return NAME;
    }
}
