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

#ifndef _PLAYLISTOBJECT_H_
#define _PLAYLISTOBJECT_H_

#include "CommonPrerequisites.h"
#include "EventCaster.h"
#include "EventObject.h"

namespace rl {

class _RlCommonExport PlaylistObject : public EventCaster<EventObject>
{
public:
	/// Konstruktor
	PlaylistObject();
	/// Destruktor
	~PlaylistObject();
	/// Objekt laden, einrichten etc.
	virtual void load() = 0;
	/// Objekt entladen, abbauen etc.
	virtual void unload() = 0;
	/// Objektaktion starten
	virtual void start() = 0;
	/// Objectaktion stoppen
	virtual void stop() = 0;
	/// Objektaktion pausieren
	virtual void pause() = 0;
};

}

#endif
