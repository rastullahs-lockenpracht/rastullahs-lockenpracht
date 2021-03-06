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

#include "SoundEvents.h"

namespace rl
{

    SoundEvent::SoundEvent(EventSource* source)
        : EventObject(source)
    {
    }

    SoundEvent::SoundEvent(EventSource* source, const unsigned int reason)
        : EventObject(source, reason)
    {
    }

    SoundEvent::~SoundEvent()
    {
    }

    SoundFadeEvent::SoundFadeEvent(EventSource* source)
        : SoundEvent(source)
    {
    }

    SoundFadeEvent::SoundFadeEvent(EventSource* source, const unsigned int reason)
        : SoundEvent(source, reason)
    {
    }

    SoundFadeEvent::~SoundFadeEvent()
    {
    }

    SoundPlayEvent::SoundPlayEvent(EventSource* source)
        : SoundEvent(source)
    {
    }

    SoundPlayEvent::SoundPlayEvent(EventSource* source, const unsigned int reason)
        : SoundEvent(source, reason)
    {
    }

    SoundPlayEvent::~SoundPlayEvent()
    {
    }

    SoundTimingEvent::SoundTimingEvent(EventSource* source)
        : SoundEvent(source)
        , mTime(0.0)
    {
        setReason(TIMEEVENT);
    }

    SoundTimingEvent::SoundTimingEvent(EventSource* source, const unsigned int reason)
        : SoundEvent(source, reason)
    {
    }

    SoundTimingEvent::~SoundTimingEvent()
    {
    }

    SoundVoiceEvent::SoundVoiceEvent(EventSource* source)
        : SoundEvent(source)
    {
    }

    SoundVoiceEvent::SoundVoiceEvent(EventSource* source, const unsigned int reason)
        : SoundEvent(source, reason)
    {
    }

    SoundVoiceEvent::~SoundVoiceEvent()
    {
    }
}
