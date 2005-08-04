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

#include "GameAreaEventSource.h"

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
        mAreaEventCaster.removeEventListeners();
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
        mAreaEventCaster.addEventListener( list );
    }

    void GameAreaEventSource::removeAreaListener( GameAreaListener* list )
    {
        mAreaEventCaster.removeEventListener( list );
    }

    bool GameAreaEventSource::hasListeners( ) const
    {
        return mAreaEventCaster.hasEventListeners();
    }
}

