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
        m_AreaType( areaType ),
        m_InsideAreaList(),
        m_AreaEventCaster(),
        m_Actor(act)
    {
       
    }

    GameAreaEventSource::~GameAreaEventSource() 
    {
        m_InsideAreaList.clear();
        m_AreaEventCaster.removeEventListeners();
    }

    
    void GameAreaEventSource::performQuery( Ogre::Real timePassed )
    {
        // Position übertragen
        m_AreaType->setQueryPosition( m_Actor->getWorldPosition() );       
        ActorMap currInside = m_AreaType->performQuery();

        ActorMap enteredMap, leftMap;
        // EinfuegeIteratoren erstellen        
        insert_iterator<ActorMap> enteredInsert(enteredMap, enteredMap.begin());
        insert_iterator<ActorMap> leftInsert(leftMap, leftMap.begin());

        // Alle feststellen die rausgefallen sind
        set_difference( m_InsideAreaList.begin(), m_InsideAreaList.end(),
                        currInside.begin(), currInside.end(), leftInsert );
        
        // Alle feststellen die neu hinzugekommen sind
        set_difference( currInside.begin(), currInside.end(),
            m_InsideAreaList.begin(), m_InsideAreaList.end(), enteredInsert );

        // Die Übriggebliebenen in m_InsideAreaList speichern
        m_InsideAreaList = currInside;

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
            m_AreaEventCaster.dispatchEvent( event );
        }

        event->setReason( GameAreaEvent::AREA_ENTERED );
        // Dann werden alle Listener für jedes betretende Object einmal benachrichtigt
        for( it = enteringActors.begin(); it != enteringActors.end();++it) 
        {
            actor = it->second;
            event->setProvokingActor( actor );
            m_AreaEventCaster.dispatchEvent( event );
        }
    }

    void GameAreaEventSource::addAreaListener( GameAreaListener*  list )
    {
        m_AreaEventCaster.addEventListener( list );
    }

    void GameAreaEventSource::removeAreaListener( GameAreaListener* list )
    {
        m_AreaEventCaster.removeEventListener( list );
    }

    bool GameAreaEventSource::hasListeners( ) const
    {
        return m_AreaEventCaster.hasEventListeners();
    }
}

