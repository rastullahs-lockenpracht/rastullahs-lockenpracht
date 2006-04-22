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

%template(EventObjectEventCaster) EventCaster<rl::EventObject>;
class PlaylistObject : public rl::EventCaster<rl::EventObject>
{
public:
	/// Konstruktor
	PlaylistObject();
	/// Destruktor
	~PlaylistObject();
	/// Objekt laden, einrichten etc.
	virtual void load() = 0;
	/// Objekt entladen, abbauen etc.
	virtual void unload() = 0;
	/// Objektaktion starten
	virtual void start() = 0;
	/// Objectaktion stoppen
	virtual void stop() = 0;
	/// Objektaktion pausieren
	virtual void pause() = 0;
};

class PlaylistEvent : public rl::EventObject
{
};

%template(EventObjectEventListener) EventListener<rl::PlaylistEvent>;
class Playlist : public rl::EventListener<rl::PlaylistEvent>
{
public:
	Playlist();
	virtual ~Playlist();
    virtual void start() = 0;
    virtual void stop() = 0;
};

class SimplePlaylist : public rl::Playlist
{
public:
	SimplePlaylist();
	virtual ~SimplePlaylist();
    virtual void add(rl::PlaylistObject *object);
    virtual void remove(rl::PlaylistObject *object);
    virtual void start();
    virtual void stop();
    void setLooping(bool looping);
    bool isLooping() const;
    virtual bool eventRaised(rl::PlaylistEvent *eventRaised);
};

class rl::LinkItem;
typedef std::list<rl::LinkItem*> LinkedList;
class LinkItem {
public:
	LinkItem(rl::PlaylistObject *item);
	virtual ~LinkItem();
	rl::PlaylistObject* getItem() const;
	void start();
	void stop();
	void add(rl::PlaylistObject *child);
	void remove(rl::PlaylistObject *child);
};


class LinkedPlaylist : public rl::Playlist
{
public:
	LinkedPlaylist();
	virtual ~LinkedPlaylist();
    void add(rl::PlaylistObject *object, LinkItem *parent);
	void remove(rl::PlaylistObject *object, LinkItem *parent);
    virtual void stop();
	virtual void start();
	LinkItem* getList() const;
};

class PauseObject : public rl::PlaylistObject {
	public:
		PauseObject(long unsigned int nsec);
		~PauseObject();
		const long unsigned int getPause() const;
		virtual void start();
		virtual void stop();
		virtual void pause();
		virtual void load();
		virtual void unload();
};


}

