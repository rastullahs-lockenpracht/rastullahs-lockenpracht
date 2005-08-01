/* SoundEvent.h - Diese Klassse repräsentiert ein Event das in RlSound auftritt.
 * (C) 2003-2005. Team Pantheon. www.team-pantheon.de
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
 
#ifndef SOUNDEVENT_H
#define SOUNDEVENT_H

#include "SoundPrerequisites.h"
#include "EventObject.h"

namespace rl {

class SoundEvent : public EventObject{

public:
	SoundEvent(EventSource *source);
	virtual ~SoundEvent();
};

class SoundFadeEvent : public SoundEvent {
public:
    SoundFadeEvent(EventSource *source);
    virtual ~SoundFadeEvent();

    static const unsigned int STARTEVENT = 12;
    static const unsigned int STOPEVENT = 13;
};

class SoundPlayEvent : public SoundEvent {
public:

    SoundPlayEvent(EventSource *source);
    virtual ~SoundPlayEvent();
    
    static const unsigned int STARTEVENT = 10;
    static const unsigned int STOPEVENT = 11;
};

class SoundTimingEvent : public SoundEvent {

public:
    double mTime;

    SoundTimingEvent(EventSource *source);
    virtual ~SoundTimingEvent();
    
    static const unsigned int TIMEEVENT = 14;
};


}
#endif // SOUNDEVENT_H
