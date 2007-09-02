/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h" //precompiled header

#include "GameAreaEventSource.h"
#include "ScriptWrapper.h"

// Fr Intersection und so :)
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
		// Ohne Listener ist der Query unwichtig...
		if( !mAreaEventCaster.hasEventListeners() )
			return;

        // Position bertragen
        mAreaType->setQueryPosition( mActor->getWorldPosition() );
        ActorMap currInside = mAreaType->performQuery();
        // Der Actor um den herum die Quelle ist, wird vermutlich auch gefunden :)
        // Also rausl�chen
        currInside.erase( mActor->getName() );

        ActorMap enteredMap, leftMap;
        // EinfuegeIteratoren erstellen
        std::insert_iterator<ActorMap> enteredInsert(enteredMap, enteredMap.begin());
        std::insert_iterator<ActorMap> leftInsert(leftMap, leftMap.begin());

        // Alle feststellen die rausgefallen sind
        std::set_difference( mInsideAreaList.begin(), mInsideAreaList.end(),
                        currInside.begin(), currInside.end(), leftInsert );

        // Alle feststellen die neu hinzugekommen sind
        std::set_difference( currInside.begin(), currInside.end(),
            mInsideAreaList.begin(), mInsideAreaList.end(), enteredInsert );

        // diejenigen die rausgefallen sind, aber noch nicht den noetigen abstand haben, wieder hinzufuegen
        ActorMap reallyLeftMap;
        ActorMap notReallyLeftMap;
        ActorMap::iterator it = leftMap.begin();
        for( ; it != leftMap.end(); it++ )
        {
            if( mAreaType->getDistance(it->second) <= mAreaType->getTransitionDistance() )
                notReallyLeftMap.insert( *it );
            else
                reallyLeftMap.insert( *it );
        }

        // Die �riggebliebenen in mInsideAreaList speichern
        //mInsideAreaList = currInside + notReallyLeftMap;
        mInsideAreaList = currInside;
        mInsideAreaList.insert(notReallyLeftMap.begin(), notReallyLeftMap.end());
        

        // Die Neuen und die Rausgefallenen an die Listener dispatchen
		doDispatchEvents( enteredMap, reallyLeftMap );
    }

    void GameAreaEventSource::doDispatchEvents(
        const ActorMap& enteringActors, const ActorMap& leavingActors )
    {
        ActorMap::const_iterator it;
        Actor* actor;

        GameAreaEvent* event = new GameAreaEvent( this, GameAreaEvent::AREA_LEFT );
        // Erst werden alle Listener fr jedes verlassende Object einmal benachrichtigt
        for( it = leavingActors.begin(); it != leavingActors.end();++it)
        {
            actor = it->second;
            event->setProvokingActor( actor );
            mAreaEventCaster.dispatchEvent( event );
        }

        event->setReason( GameAreaEvent::AREA_ENTERED );
        // Dann werden alle Listener fr jedes betretende Object einmal benachrichtigt
        for( it = enteringActors.begin(); it != enteringActors.end();++it)
        {
            actor = it->second;
            event->setProvokingActor( actor );
            mAreaEventCaster.dispatchEvent( event );
        }
        delete event;
    }

    void GameAreaEventSource::addAreaListener( GameAreaListener*  list )
    {
        if( !mAreaEventCaster.containsListener(list) )
        {
            mAreaEventCaster.addEventListener( list );
            // Owned ;)
            ScriptWrapper::getSingleton().owned( list );
        }
    }

    void GameAreaEventSource::removeAreaListener( GameAreaListener* list )
    {
        if( mAreaEventCaster.containsListener(list) )
        {
            mAreaEventCaster.removeEventListener( list );
            // Nicht mehr gebraucht
            ScriptWrapper::getSingleton().disowned( list );
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
            ScriptWrapper::getSingleton().disowned( gal );
            iter++;
        }
        mAreaEventCaster.removeEventListeners();
    }

    bool GameAreaEventSource::hasListeners( ) const
    {
        return mAreaEventCaster.hasEventListeners();
    }
}

