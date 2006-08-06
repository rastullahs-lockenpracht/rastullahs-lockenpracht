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
 
#ifndef SOUNDEVENT_H
#define SOUNDEVENT_H

#include "MultimediaPrerequisites.h"
#include "EventObject.h"

namespace rl {

class _RlMultimediaExport SoundEvent : public EventObject
{

public:
	SoundEvent(EventSource *source);
    SoundEvent(EventSource *source, const unsigned int reason);
	virtual ~SoundEvent();
};

class _RlMultimediaExport SoundFadeEvent : public SoundEvent {
public:
    SoundFadeEvent(EventSource *source);
    SoundFadeEvent(EventSource *source, const unsigned int reason );
    virtual ~SoundFadeEvent();

    static const unsigned int STARTEVENT = 12;
    static const unsigned int STOPEVENT = 13;
};

class _RlMultimediaExport SoundPlayEvent : public SoundEvent {
public:

    SoundPlayEvent(EventSource *source);
    SoundPlayEvent(EventSource *source, const unsigned int reason );
    virtual ~SoundPlayEvent();
    
    static const unsigned int STARTEVENT = 10;
    static const unsigned int STOPEVENT = 11;
    static const unsigned int PAUSEEVENT = 15;
};

class _RlMultimediaExport  SoundTimingEvent : public SoundEvent {

public:
    double mTime;

    SoundTimingEvent(EventSource *source);
    SoundTimingEvent(EventSource *source, const unsigned int reason );
    virtual ~SoundTimingEvent();
    
    static const unsigned int TIMEEVENT = 14;
};


}
#endif // SOUNDEVENT_H
