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
	virtual bool eventRaised(TimerEvent *anEvent);

    /** Diese Methode wird aufgerufen, wenn sich der Status des Objectes geändert hat
      * @param anEvent Der auslösende Event
      */  
	virtual void timerFired(TimerEvent* anEvent) = 0;
};
}

#endif
