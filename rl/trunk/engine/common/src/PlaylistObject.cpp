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

#include "PlaylistObject.h"

namespace rl {
/**
 * @author JoSch
 * @date 12-04-2006
 * @version 1.0
 */
PlaylistObject::PlaylistObject(): EventCaster<PlaylistEvent>(), EventSource()
{
}

/**
 * @author JoSch
 * @date 12-04-2006
 * @version 1.0
 */
PlaylistObject::~PlaylistObject()
{
}

void PlaylistObject::start()
{
	PlaylistEvent event = PlaylistEvent(this, PlaylistEvent::STARTEVENT);
	dispatchEvent(&event);
}

void PlaylistObject::stop()
{
	PlaylistEvent event = PlaylistEvent(this, PlaylistEvent::STOPEVENT);
	dispatchEvent(&event);
}

void PlaylistObject::pause()
{
	PlaylistEvent event = PlaylistEvent(this, PlaylistEvent::PAUSEEVENT);
	dispatchEvent(&event);
}

void PlaylistObject::load()
{
	PlaylistEvent event = PlaylistEvent(this, PlaylistEvent::LOADEVENT);
	dispatchEvent(&event);
}

void PlaylistObject::unload()
{
	PlaylistEvent event = PlaylistEvent(this, PlaylistEvent::UNLOADEVENT);
	dispatchEvent(&event);
}

}
