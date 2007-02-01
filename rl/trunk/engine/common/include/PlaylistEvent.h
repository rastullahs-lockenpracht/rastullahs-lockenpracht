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
 
#ifndef PLAYLISTEVENT_H
#define PLAYLISTEVENT_H

#include "CommonPrerequisites.h"
#include "EventObject.h"

namespace rl {

class _RlCommonExport PlaylistEvent : public EventObject
{

public:
	PlaylistEvent(EventSource *source);
    PlaylistEvent(EventSource *source, const unsigned int reason);
	virtual ~PlaylistEvent();

	static const unsigned int NOPEVENT = 0;
	static const unsigned int STARTEVENT = 1;
    static const unsigned int STOPEVENT = 2;
    static const unsigned int PAUSEEVENT = 3;
	static const unsigned int LOADEVENT = 4;
	static const unsigned int UNLOADEVENT = 5;
};

}
#endif // PLAYLISTEVENT_H
