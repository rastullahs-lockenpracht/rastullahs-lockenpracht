/* EventCaster.cpp - Ein Objekt, das Ereignisse verschickt.
 * (C) 2004. Team Pantheon. www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License as published by
 *  the .
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not
 */
#include "EventCaster.h"


namespace rl {
/**
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
EventCaster<class Event>::EventCaster()
{
}

/**
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
EventCaster<class Event>::~EventCaster()
{
}

/**
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
void EventCaster<class Event>::addEventListener(EventListener<Event> *newListener)
{
    mListeners.insert(newListener);
}

/**
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
void EventCaster<class Event>::removeEventListener(EventListener<Event> *aListener)
{
    mListeners.erase(mListeners.find(aListener));
}

/**
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
void EventCaster<class Event>::dispatchEvent(Event *anEvent)
{
    set<EventListener<Event>*>::iterator it;
    for(it = mListeners.begin(); it != mListeners.end(); it++)
    {
        try {
            (*it)->eventRaised(anEvent);
        } catch(...)
        {
        }
    }
}

}
