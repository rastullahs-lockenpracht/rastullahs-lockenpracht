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

#ifndef EVENTCASTER_H
#define EVENTCASTER_H

#include <set>
#include "EventListener.h"

using namespace std;

namespace rl {
/**
 * Diese Klasse ist die Basisklasse fuer Objekte, die Ereignisse verschicken.
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
template <typename Event>
class EventCaster {
private:
    typedef EventListener<Event> ListenerToEvent;
    typedef set<ListenerToEvent*> EventSet;
    typedef typename EventSet::iterator EventSetIterator;
    EventSet mListeners; 
public:
    /// Der Konstruktor
	EventCaster();
    /// Der Destruktor
	virtual ~EventCaster();
   /// Fuege einen EventListener hinzu.
   void addEventListener(EventListener<Event> *newListener);
   /// Loesche einen EventListener.
   void removeEventListener(EventListener<Event> *aListener);
   /// Loescht alle EventListener.
   void removeEventListeners();
   /// Ein Ereignis verteilen.
   void dispatchEvent(Event *anEvent);
};

/**
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
template <typename Event>
EventCaster<Event>::EventCaster()
{
}

/**
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
template <typename Event> 
EventCaster<Event>::~EventCaster()
{
}

/**
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
template <typename Event>
void EventCaster<Event>::addEventListener(ListenerToEvent *newListener)
{
    mListeners.insert(newListener);
}

/**
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
template <typename Event>
void EventCaster<Event>::removeEventListener(ListenerToEvent *aListener)
{
    mListeners.erase(mListeners.find(aListener));
}

/**
* @author JoSch
* @date 10-05-2004
* @version 1.0
*/
template <typename Event>
void EventCaster<Event>::removeEventListeners()
{
    mListeners.clear();
}

/**
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
template <typename Event>
void EventCaster<Event>::dispatchEvent(Event *anEvent)
{
    for_each(mListeners.begin(), mListeners.end(),
        bind2nd(DispatchFunctor<Event>(), anEvent));
}

     
}
#endif // EVENTCASTER_H
