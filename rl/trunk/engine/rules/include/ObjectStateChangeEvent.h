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

#ifndef __ObjectStateChangeEvent_H__
#define __ObjectStateChangeEvent_H__


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
