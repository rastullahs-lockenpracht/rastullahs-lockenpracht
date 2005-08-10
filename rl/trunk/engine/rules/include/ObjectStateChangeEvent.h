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

#ifndef __ObjectStateChangeEvent_H__
#define __ObjectStateChangeEvent_H__

#include <OgreUserDefinedObject.h>

#include "EventObject.h"

#include "RulesPrerequisites.h"

namespace rl {

class ObjectStateChangeEventSource;
class GameObject;

/** 
 *  ObjectStateChangeEvent
 *  Ein Event der ausgelöst wird, wenn sich eine Eigenschaft eines GameObjects verändert hat 
 *
 *  @see ObjectStateChangeListener, ObjectStateChangeEventSource
 *  @todo Feldgenauigkeit, also auch noch im Ereignis zu kodieren, was verändert wurde
 */
class _RlRulesExport ObjectStateChangeEvent : public virtual EventObject 
{
public:
     /** Der Standardkonstruktor
      * @param src Die Eventquelle
      */
    ObjectStateChangeEvent( ObjectStateChangeEventSource* src );
    /// Standarddestruktor
    virtual ~ObjectStateChangeEvent() {};

    /// Setzt das verursachenden Object des Events
    void setProvokingObject(GameObject* obj ) {  mObject = obj; };
    /// Gibt das verursachende Object des Events zurück
    GameObject* getProvokingObject() const { return mObject; };
    /// Gibt die Eventquelle zurück
    ObjectStateChangeEventSource* getSource() const;
private:
    /// Das verursachende Object
    GameObject* mObject;
};
}

#endif
