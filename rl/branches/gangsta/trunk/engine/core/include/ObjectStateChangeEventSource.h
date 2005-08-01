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

#ifndef __ObjectStateChangeEventSource_H__
#define __ObjectStateChangeEventSource_H__

#include "EventSource.h"
#include "EventCaster.h"

#include "ObjectStateChangeListener.h"
#include "ObjectStateChangeEvent.h"

#include "CorePrerequisites.h"

#include <OgreUserDefinedObject.h>

namespace rl {

/** 
 * GameAreaEventSource
 * Die Quelle für Ereignisse die das betreten/verlassen eines Areals betreffen.
 * Hier werden die Actoren die sich zur letzten Abfrage innerhalb des Areals befanden
 * aufbewahrt, um die Differenzen bestimmen zu können, und die Abfragemethode verwaltet
 *
 *  @see GameAreaListener, GameAreaEvent, GameEventManager, GameAreaTypes
 */
class _RlCoreExport ObjectStateChangeEventSource : public virtual EventSource
{
public:
    /** Konstruktor
     *  @param areaType Die Art des abzufragenden Areals 
     *  @param act Der Actor, an den das Zentrum des Areals geknüpft ist
     */
	ObjectStateChangeEventSource( Ogre::UserDefinedObject* obj );
	ObjectStateChangeEventSource( );
	/// Dekonstruktor
    virtual ~ObjectStateChangeEventSource();

    /** Fügt einen ObjectStateChangeListener hinzu, der zukünftig bei ObjectStateChangeEvents benachrichtigt wird 
     *
     * @param list Der hinzuzufügende Listener
     */
    void addObjectStateChangeListener( ObjectStateChangeListener*  list );
    /** Entfernt einen ObjectStateChangeListener
    *
    * @param list Der zu entfernende Listener
    */
    void removeObjectStateChangeListener( ObjectStateChangeListener* list );

    /// Gibt zurück ob sich Listener angemeldet haben
    bool hasListeners( ) const;

    /// Gibt das Objekt zurück, der überwacht wird
    Ogre::UserDefinedObject* getObject() const { return mObject; }

	void fireObjectStateChangeEvent();

protected:
	void setObject(Ogre::UserDefinedObject* obj) { mObject = obj; }

private: 

    /// Der EventCaster der die Verteilung an die Listener übernimmt
    EventCaster<ObjectStateChangeEvent> mObjectStateChangeEventCaster;
    /// Der Aktor der überwacht wird
    Ogre::UserDefinedObject* mObject;
};

}

#endif
