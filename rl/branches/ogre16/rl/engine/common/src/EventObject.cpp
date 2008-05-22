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

#include "stdinc.h" //precompiled header

#include "EventObject.h"

namespace rl {
/**
 * @param source. Die Ereignisquelle.
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
EventObject::EventObject(EventSource *source):
    mSource(source)
{
}

/**
* @param source. Die Ereignisquelle.
* @param reason. Der Grund des Events.
* @author Harlequin
* @date 28-01-2005
* @version 1.0
*/
EventObject::EventObject(EventSource *source, const unsigned int reason):
mSource(source), mReason( reason )
{
}

/**
 * @author JoSch
 * @date 10-05-2004
 * @version 1.0
 */
EventObject::~EventObject()
{}

/**
 * @author JoSch
 * @date 10-14-2004
 * @version 1.0
 */
EventSource *EventObject::getSource() const
{
    return mSource;
}

/**
 * @return Gibt den Grund des Events zurueck.
 * @author JoSch
 * @date 10-14-2004
 * @version 1.0
 */
const unsigned int EventObject::getReason() const
{
    return mReason;
}

/**
 * @param reason Der Grund fuer das Ereignis.
 * @author JoSch
 * @date 10-14-2004
 * @version 1.0
 */
void EventObject::setReason(const unsigned int reason)
{
    mReason = reason;
}


}
