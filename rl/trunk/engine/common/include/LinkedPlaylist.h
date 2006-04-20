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

#ifndef _LINKEDPLAYLIST_H_
#define _LINKEDPLAYLIST_H_

#include "CommonPrerequisites.h"
#include "EventListener.h"
#include "Playlist.h"
#include <list>

namespace rl
{
class PlaylistObject;
class LinkItem;
typedef std::list<LinkItem*> LinkedList;
class _RlCommonExport LinkItem {
private:
	LinkedList mChildren;
	PlaylistObject *mItem;

public:
	LinkItem(PlaylistObject *item);
	virtual ~LinkItem();
	PlaylistObject* getItem() const;
	void start();
	void stop();
	void add(PlaylistObject *child);
	void remove(PlaylistObject *child);
};


class _RlCommonExport LinkedPlaylist : public Playlist
{
private:
	LinkItem *mList;
	LinkItem *mPlaying;

public:
	LinkedPlaylist();
	virtual ~LinkedPlaylist();
    void add(PlaylistObject *object, LinkItem *parent);
	void remove(PlaylistObject *object, LinkItem *parent);
    virtual void stop();
	virtual void start();
	virtual bool eventRaised(PlaylistEvent *anEvent);
	LinkItem* getList() const;
};

}

#endif /* LinkedPlaylist_H_*/
