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

#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include <functional>

namespace rl {
/**
 * Diese Klasse ist die Basisklasse fuer Objekte, die an Ereignisse interessiert
 * sind.
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
template <typename Event>
class EventListener {
public:
    /// Der Konstruktor.
	EventListener();
    /// Der Destruktor.
	virtual ~EventListener();
    /* Hier wird das Objekt benachrichtigt.
	 *
	 * @return bool consumed Wurde der Event aufgebracht (defunct)
	 */
    virtual bool eventRaised(Event *anEvent) = 0;
};

/**
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
template <typename Event>
EventListener<Event>::EventListener()
{}

/**
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
template <typename Event>
EventListener<Event>::~EventListener()
{}

/**
 * Diese Klasse ist ein Funktor fuer EventListener, der "dispatchEvent"
 * aufruft.
 * @author JoSch
 * @date 10-30-2004
 * @version 1.0
 */
template <typename Event>
class DispatchFunctor:
    public std::binary_function<EventListener<Event>*, Event*, bool> {
public:
    /// Ein Functor, der einen EventListener und einen Event entgegennimmt
    inline bool operator() (EventListener<Event> *listener, Event *event) const;
};

/**
 * @author JoSch
 * @date 10-30-2004
 * @version 1.0
 */
template <typename Event>
bool DispatchFunctor<Event>::operator() 
        (EventListener<Event> *listener, Event *event) const
{
    return listener->eventRaised(event);
}


}
#endif // EVENTLISTENER_H
