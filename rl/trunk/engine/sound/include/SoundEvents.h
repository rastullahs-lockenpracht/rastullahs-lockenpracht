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

#include "SoundPrerequisites.h"
#include "EventObject.h"

namespace rl {

class _RlSoundExport SoundEvent : public EventObject
{
public:
    static const unsigned int START_PLAY_EVENT = 10;
    static const unsigned int STOP_PLAY_EVENT = 11;
	static const unsigned int START_FADE_EVENT = 12;
    static const unsigned int STOP_FADE_EVENT = 13;
    static const unsigned int TIMEEVENT = 14;

	SoundEvent(EventSource *source);
	virtual ~SoundEvent();

private:
	int mEventType;
};

class _RlSoundExport SoundFadeEvent : public SoundEvent {
public:
    SoundFadeEvent(EventSource *source);
    virtual ~SoundFadeEvent();

};

class _RlSoundExport SoundPlayEvent : public SoundEvent {
public:

    SoundPlayEvent(EventSource *source);
    virtual ~SoundPlayEvent();
    
};

class _RlSoundExport  SoundTimingEvent : public SoundEvent {

public:
    double mTime;

    SoundTimingEvent(EventSource *source);
    virtual ~SoundTimingEvent();
    
};


}
#endif // SOUNDEVENT_H
