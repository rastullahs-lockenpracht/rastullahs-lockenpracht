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

#ifndef __TimerListener_H__
#define __TimerListener_H__

#include "EventListener.h"
#include "TimerEvent.h"

#include "RulesPrerequisites.h"

namespace rl {

/** TimerListener
 *  Eine Basisklasse für Listener die auf TimerEvents lauschen
 *
 *  @see TimerEvent, TimerEventSource, GameObject
 */
class _RlRulesExport TimerListener : public virtual EventListener<TimerEvent>
{
public:
    /// Dekonstruktor
    virtual ~TimerListener();

     /// Kümmert sich um das Aufrufen benannter Methoden aus Reasons
	bool eventRaised(TimerEvent *anEvent);

    /** Diese Methode wird aufgerufen, wenn der Timer ausgelöst wurde
      * @param anEvent Der auslösende Event
      */  
	virtual void timerFired(TimerEvent* anEvent) = 0;
};
}

#endif
