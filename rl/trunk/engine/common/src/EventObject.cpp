/* EventObject.cpp - Das Ereignis, das verschickt wird.
 * (C) 2004. Team Pantheon. www.team-pantheon.de
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

}
