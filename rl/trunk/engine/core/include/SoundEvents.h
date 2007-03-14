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
 
#ifndef SOUNDEVENT_H
#define SOUNDEVENT_H

#include "CorePrerequisites.h"
#include "EventObject.h"

namespace rl {

class _RlCoreExport SoundEvent : public EventObject
{

public:
	SoundEvent(EventSource *source);
    SoundEvent(EventSource *source, const unsigned int reason);
	virtual ~SoundEvent();
};

class _RlCoreExport SoundFadeEvent : public SoundEvent {
public:
    SoundFadeEvent(EventSource *source);
    SoundFadeEvent(EventSource *source, const unsigned int reason );
    virtual ~SoundFadeEvent();

    static const unsigned int STARTEVENT = 12;
    static const unsigned int STOPEVENT = 13;
};

class _RlCoreExport SoundPlayEvent : public SoundEvent {
public:

    SoundPlayEvent(EventSource *source);
    SoundPlayEvent(EventSource *source, const unsigned int reason );
    virtual ~SoundPlayEvent();
    
    static const unsigned int STARTEVENT = 10;
    static const unsigned int STOPEVENT = 11;
    static const unsigned int PAUSEEVENT = 15;
};

class _RlCoreExport  SoundTimingEvent : public SoundEvent {

public:
    unsigned int mTime;

    SoundTimingEvent(EventSource *source);
    SoundTimingEvent(EventSource *source, const unsigned int reason );
    virtual ~SoundTimingEvent();
    
    static const unsigned int TIMEEVENT = 14;
};

class _RlCoreExport  SoundVoiceEvent : public SoundEvent {

public:

    SoundVoiceEvent(EventSource *source);
    SoundVoiceEvent(EventSource *source, const unsigned int reason );
    virtual ~SoundVoiceEvent();
    
    static const unsigned int SWAPINEVENT = 16;
    static const unsigned int SWAPOUTEVENT = 17;
};


}
#endif // SOUNDEVENT_H
