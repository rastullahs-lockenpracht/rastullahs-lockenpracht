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

#ifndef __ObjectStateChangeListener_H__
#define __ObjectStateChangeListener_H__

#include "EventListener.h"
#include "ObjectStateChangeEvent.h"

#include "RulesPrerequisites.h"

namespace rl {

/** ObjectStateChangeListener
 *  Eine Basisklasse für Listener die auf ObjectStateChangeEvents lauschen
 *
 *  @see ObjectStateChangeEvent, ObjectStateChangeEventSource, GameObject
 */
class _RlRulesExport ObjectStateChangeListener : public virtual EventListener<ObjectStateChangeEvent>
{
public:
    /// Dekonstruktor
    virtual ~ObjectStateChangeListener();

     /// Kümmert sich um das Aufrufen benannter Methoden aus Reasons
	virtual bool eventRaised(ObjectStateChangeEvent *anEvent);

    /** Diese Methode wird aufgerufen, wenn sich der Status des Objectes geändert hat
      * @param anEvent Der auslösende Event
      */  
	virtual void objectStateChanged(ObjectStateChangeEvent* anEvent) = 0;
};
}

#endif
