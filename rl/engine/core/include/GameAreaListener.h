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

#ifndef __GameAreaListener_H__
#define __GameAreaListener_H__

#include "EventListener.h"
#include "GameAreaEvent.h"

#include "CorePrerequisites.h"

namespace rl {

/** GameAreaListener
 *  Eine Basisklasse fuer Listener die auf GameAreaEvents lauschen
 *
 *  @see GameAreaEvent, GameAreaEventSource, GameEventManager, GameAreaTypes
 */
class _RlCoreExport GameAreaListener : public virtual EventListener<GameAreaEvent>
{
public:
    /// Dekonstruktor
    virtual ~GameAreaListener();

    /** Diese Methode wird aufgerufen wenn ein Aktor das Areal verlassen hat
     *  
     * @param anEvent Das verantwortliche Ereignis
     */
    virtual void areaLeft(GameAreaEvent *anEvent) = 0;
    /** Diese Methode wird aufgerufen wenn ein Aktor das Areal betreten hat
    *  
    * @param anEvent Das verantwortliche Ereignis
    */
    virtual void areaEntered(GameAreaEvent *anEvent) = 0;

    /// Kuemmert sich um das Aufrufen benannter Methoden aus Reasons
    virtual bool eventRaised(GameAreaEvent *anEvent);
};
}

#endif
