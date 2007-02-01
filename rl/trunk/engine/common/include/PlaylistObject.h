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

#ifndef _PLAYLISTOBJECT_H_
#define _PLAYLISTOBJECT_H_

#include "CommonPrerequisites.h"
#include "EventCaster.h"
#include "PlaylistEvent.h"
#include "EventSource.h"

namespace rl {

class _RlCommonExport PlaylistObject : 
			public EventCaster<PlaylistEvent>, public virtual EventSource
{
public:
	/// Konstruktor
	PlaylistObject();
	/// Destruktor
	virtual ~PlaylistObject();
	/// Objekt laden, einrichten etc.
	virtual void load();
	/// Objekt entladen, abbauen etc.
	virtual void unload();
	/// Objektaktion starten
	virtual void start();
	/// Objectaktion stoppen
	virtual void stop();
	/// Objektaktion pausieren
	virtual void pause();
};

}

#endif
