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

#ifndef EVENTCASTER_H
#define EVENTCASTER_H

#include "EventListener.h"
#include <set>

namespace rl
{
    /**
     * Diese Klasse ist die Basisklasse fuer Objekte, die Ereignisse verschicken.
     * @author JoSch
     * @date 10-05-2004
     * @version 1.0
     */

    template <typename Event> class EventCaster
    {
    public:
        typedef EventListener<Event> ListenerToEvent;
        typedef std::set<ListenerToEvent*> EventSet;
        typedef typename EventSet::iterator EventSetIterator;

        /// Der Konstruktor
        EventCaster();
        /// Der Destruktor
        virtual ~EventCaster();
        /// Fuege einen EventListener hinzu.
        void addEventListener(EventListener<Event>* newListener);
        /// Loesche einen EventListener.
        void removeEventListener(EventListener<Event>* aListener);
        /// Loescht alle EventListener.
        void removeEventListeners();
        /// Gibt es diesen Listener bereits?
        bool containsListener(EventListener<Event>* aListener) const;
        /// Sind Listener vorhanden?
        bool hasEventListeners() const;
        /// Ein Ereignis verteilen.
        void dispatchEvent(Event* anEvent);
        /// EventSet
        EventSet getEventSet() const;

    private:
        EventSet mListeners;
        EventSet mRemovedListeners;
    };

    /**
     * @author JoSch
     * @date 10-05-2004
     * @version 1.0
     */
    template <typename Event> EventCaster<Event>::EventCaster()
    {
    }

    /**
     * @author JoSch
     * @date 10-05-2004
     * @version 1.0
     */
    template <typename Event> EventCaster<Event>::~EventCaster()
    {
    }

    /**
     * @author JoSch
     * @date 10-05-2004
     * @version 1.0
     */
    template <typename Event> void EventCaster<Event>::addEventListener(ListenerToEvent* newListener)
    {
        typename EventSet::const_iterator it = mRemovedListeners.find(newListener);

        // Listener sollte zuvor entfernt werden, aus mRemovedListeners nehmen
        if (it != mRemovedListeners.end())
            mRemovedListeners.erase(*it);

        mListeners.insert(newListener);
    }

    /**
     * @author JoSch
     * @date 10-05-2004
     * @version 1.0
     */
    template <typename Event> void EventCaster<Event>::removeEventListener(ListenerToEvent* aListener)
    {
        typename EventSet::const_iterator it = mListeners.find(aListener);
        if (it != mListeners.end())
        {
            mRemovedListeners.insert(*it);
        }
    }

    /**
     * @author JoSch
     * @date 10-05-2004
     * @version 1.0
     */
    template <typename Event> void EventCaster<Event>::removeEventListeners()
    {
        mRemovedListeners.insert(mListeners.begin(), mListeners.end());
    }

    /**
     * @author Harlequin
     * @date 11-08-2005
     * @version 1.0
     */
    template <typename Event> bool EventCaster<Event>::containsListener(ListenerToEvent* aListener) const
    {
        return (mListeners.end() != mListeners.find(aListener))
            && (mRemovedListeners.end() == mRemovedListeners.find(aListener));
    }

    /**
     * @author Harlequin
     * @date 30-09-2004
     * @version 1.0
     */
    template <typename Event> bool EventCaster<Event>::hasEventListeners() const
    {
        return mListeners.size() > mRemovedListeners.size();
    }

    /**
     * @author JoSch
     * @date 10-05-2004
     * @version 1.0
     */
    template <typename Event> void EventCaster<Event>::dispatchEvent(Event* anEvent)
    {
        while (!mRemovedListeners.empty())
        {
            mListeners.erase(mListeners.find(*mRemovedListeners.begin()));
            mRemovedListeners.erase(mRemovedListeners.begin());
        }

        if (mListeners.empty())
            return;

        for_each(mListeners.begin(), mListeners.end(), bind2nd(DispatchFunctor<Event>(), anEvent));
    }

    template <typename Event> std::set<EventListener<Event>*> EventCaster<Event>::getEventSet() const
    {
        return mListeners;
    }
}
#endif // EVENTCASTER_H
