/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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
