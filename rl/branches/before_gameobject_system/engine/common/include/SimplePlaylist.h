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

#ifndef _SIMPLEPLAYLIST_H_
#define _SIMPLEPLAYLIST_H_

#include "CommonPrerequisites.h"
#include "EventListener.h"
#include "Playlist.h"
#include <list>

namespace rl
{

typedef std::list<PlaylistObject*> ObjectList;

class _RlCommonExport SimplePlaylist : public Playlist
{
private:
    ObjectList mQueue;
	ObjectList::iterator mItem;
	bool mLooping;
    
public:
	SimplePlaylist();
	virtual ~SimplePlaylist();
    void add(PlaylistObject *object);
	void remove(PlaylistObject *object);
    virtual void stop();
	virtual void start();
	virtual bool eventRaised(PlaylistEvent *anEvent);
	void setLooping(bool looping);
	bool isLooping() const;
};

}

#endif /* SimplePlaylist_H_*/
