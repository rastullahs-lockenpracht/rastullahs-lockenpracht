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

namespace Ogre {

template<class T> class SharedPtr {
 public:
    SharedPtr();
    SharedPtr(T* rep);
    SharedPtr(const SharedPtr& r);
    virtual ~SharedPtr(); 
    T* get() const;
};

}

namespace rl {

class GameTask
{
    public:
        GameTask();
        virtual ~GameTask( ) {};
        bool isPaused() const;
        void setPaused( bool isPaused );
		virtual void run(float) = 0;
};


template <typename Event> class EventListener {
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

template <typename Event> class EventCaster {
public:
    typedef rl::EventListener<Event> ListenerToEvent;
    typedef std::set<rl::ListenerToEvent*> EventSet;
    typedef typename EventSet::iterator EventSetIterator;

    /// Der Konstruktor
	EventCaster();
    /// Der Destruktor
    virtual ~EventCaster();
    /// Fuege einen EventListener hinzu.
    void addEventListener(rl::EventListener<Event> *newListener);
    /// Loesche einen EventListener.
    void removeEventListener(rl::EventListener<Event> *aListener);
    /// Loescht alle EventListener.
    void removeEventListeners();
    /// Gibt es diesen Listener bereits?
    bool containsListener(rl::EventListener<Event> *aListener) const;
    /// Sind Listener vorhanden?
    bool hasEventListeners() const;
    /// Ein Ereignis verteilen.
    void dispatchEvent(Event *anEvent);
    /// EventSet
    EventSet getEventSet() const;

};

class EventObject {
public:
    /// Der Konstruktor.
	EventObject(rl::EventSource *source);
	/// Der Konstruktor.
	EventObject(rl::EventSource *source, const unsigned int reason );
    /// Der Destruktor.
	virtual ~EventObject();
   
   /// Die Ereignisquelle zurueckgeben
   rl::EventSource *getSource() const;
   /// Die ID des Grundes für den Event.
   const unsigned int getReason() const;
   /// Die ID setzen
   void setReason(const unsigned int reason);
};

}

