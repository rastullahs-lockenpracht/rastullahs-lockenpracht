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
 
#ifndef VIDEOEVENT_H
#define VIDEOEVENT_H

#include "SoundPrerequisites.h"
#include "EventObject.h"
#include <TheoraMovieClip.h>

namespace rl {

class _RlSoundExport VideoEvent : public EventObject
{

public:
	VideoEvent(EventSource *source);
	virtual ~VideoEvent();
};

class _RlSoundExport VideoPlayEvent : public VideoEvent {
public:

    VideoPlayEvent(EventSource *source);
    virtual ~VideoPlayEvent();
    
    static const unsigned int ENDOFSTREAM = Ogre::TheoraMovieMessage::TH_OggStreamDone;
    static const unsigned int ENDOFAUDIOSTREAM = Ogre::TheoraMovieMessage::TH_VorbisStreamDone;
    static const unsigned int ENDOFVIDEOSTREAM = Ogre::TheoraMovieMessage::TH_TheoraStreamDone;
};

class _RlSoundExport  VideoTimingEvent : public VideoEvent {

public:
    float mVTime;
    float mATime;
    int mFrameNumber;
    int mFramesDropped;

    VideoTimingEvent(EventSource *source);
    virtual ~VideoTimingEvent();
    
    static const unsigned int VIDEOTIMEEVENT = 4;
};


}
#endif // VIDEOEVENT_H
