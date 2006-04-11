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

#ifndef _PLAYLIST_H_
#define _PLAYLIST_H_

#include "CorePrerequisites.h"
#include "EventListener.h"
#include "PlaylistObject.h"
#include "SoundEvents.h"
#include <list>

namespace rl
{
//class PlaylistObject;

typedef std::list<PlaylistObject*> ObjectList;

class _RlCoreExport Playlist : public EventListener<EventObject>
{
private:
    ObjectList mQueue;
	ObjectList::iterator mItem;
	bool mLooping;
    
public:
	Playlist();
	virtual ~Playlist();
    void add(PlaylistObject *object);
	void remove(PlaylistObject *object);
    void stop();
	void start();
	virtual bool eventRaised(EventObject *anEvent);
	void setLooping(bool looping);
	bool isLooping() const;
};

}

#endif /*Playlist_H_*/
