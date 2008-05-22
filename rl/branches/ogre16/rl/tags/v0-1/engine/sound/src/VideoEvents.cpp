/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#include "VideoEvents.h"

namespace rl {

VideoEvent::VideoEvent(EventSource *source) :
    EventObject(source)
{}

VideoEvent::~VideoEvent()
{}


VideoPlayEvent::VideoPlayEvent(EventSource *source) :
    VideoEvent(source)
{
}

VideoPlayEvent::~VideoPlayEvent()
{
}

VideoTimingEvent::VideoTimingEvent(EventSource *source) :
    VideoEvent(source),
    mVTime(0.0f),
    mATime(0.0f),
    mFrameNumber(0),
    mFramesDropped(0)
{
    setReason(VIDEOTIMEEVENT);
}

VideoTimingEvent::~VideoTimingEvent()
{
}

}
