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

#include "GameAreaEventSource.h"
#include "ScriptObjectRepository.h"

// Für Intersection und so :)
#include <algorithm>

namespace rl {

    GameAreaEventSource::GameAreaEventSource( GameAreaType* areaType, Actor* act ) :
        mAreaType( areaType ),
        mInsideAreaList(),
        mAreaEventCaster(),
        mActor(act)
    {
       
    }

    GameAreaEventSource::~GameAreaEventSource() 
    {
        mInsideAreaList.clear();
        removeAllAreaListeners(  );
    }

    
    void GameAreaEventSource::performQuery( Ogre::Real timePassed )
    {
        // Position übertragen
        mAreaType->setQueryPosition( mActor->getWorldPosition() );       
        ActorMap currInside = mAreaType->performQuery();
        // Der Actor um den herum die Quelle ist, wird vermutlich auch gefunden :)
        // Also rauslöschen
        currInside.erase( mActor->getName() );

        ActorMap enteredMap, leftMap;
        // EinfuegeIteratoren erstellen        
        insert_iterator<ActorMap> enteredInsert(enteredMap, enteredMap.begin());
        insert_iterator<ActorMap> leftInsert(leftMap, leftMap.begin());

        // Alle feststellen die rausgefallen sind
        set_difference( mInsideAreaList.begin(), mInsideAreaList.end(),
                        currInside.begin(), currInside.end(), leftInsert );
        
        // Alle feststellen die neu hinzugekommen sind
        set_difference( currInside.begin(), currInside.end(),
            mInsideAreaList.begin(), mInsideAreaList.end(), enteredInsert );

        // Die Übriggebliebenen in mInsideAreaList speichern
        mInsideAreaList = currInside;

        // Die Neuen und die Rausgefallenen an die Listener dispatchen
        doDispatchEvents( enteredMap, leftMap );
    }

    void GameAreaEventSource::doDispatchEvents( 
        const ActorMap& enteringActors, const ActorMap& leavingActors )
    {
        ActorMap::const_iterator it;
        Actor* actor;

        GameAreaEvent* event = new GameAreaEvent( this, GameAreaEvent::AREA_LEFT );
        // Erst werden alle Listener für jedes verlassende Object einmal benachrichtigt
        for( it = leavingActors.begin(); it != leavingActors.end();++it) 
        {
            actor = it->second;
            event->setProvokingActor( actor );
            mAreaEventCaster.dispatchEvent( event );
        }

        event->setReason( GameAreaEvent::AREA_ENTERED );
        // Dann werden alle Listener für jedes betretende Object einmal benachrichtigt
        for( it = enteringActors.begin(); it != enteringActors.end();++it) 
        {
            actor = it->second;
            event->setProvokingActor( actor );
            mAreaEventCaster.dispatchEvent( event );
        }
    }

    void GameAreaEventSource::addAreaListener( GameAreaListener*  list )
    {
        if( !mAreaEventCaster.containsListener(list) )
        {        
            mAreaEventCaster.addEventListener( list );
            // Owned ;)
            ScriptObjectRepository::getSingleton().own( list );
        }
    }

    void GameAreaEventSource::removeAreaListener( GameAreaListener* list )
    {
        if( mAreaEventCaster.containsListener(list) )
        {        
            mAreaEventCaster.removeEventListener( list );
            // Nicht mehr gebraucht
            ScriptObjectRepository::getSingleton().disown( list );
        }
    }

    void GameAreaEventSource::removeAllAreaListeners(  )
    {
        EventCaster<GameAreaEvent>::EventSet arSet 
            = mAreaEventCaster.getEventSet();
        EventCaster<GameAreaEvent>::EventSet::iterator iter 
            = arSet.begin();
        for (iter; iter != arSet.end(); ) 
        {
            EventListener<GameAreaEvent>* ev = *iter; 
            GameAreaListener* gal = dynamic_cast<GameAreaListener*>( ev );
            ScriptObjectRepository::getSingleton().disown( gal );
            iter++;
        }
        mAreaEventCaster.removeEventListeners();
    }

    bool GameAreaEventSource::hasListeners( ) const
    {
        return mAreaEventCaster.hasEventListeners();
    }
}

