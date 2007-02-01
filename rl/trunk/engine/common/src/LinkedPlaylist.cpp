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

#include "LinkedPlaylist.h"

namespace rl
{

LinkItem::LinkItem(PlaylistObject *object)
	: mItem(object)
{
	mItem->addEventListener(this);
}

LinkItem::~LinkItem()
{
	mItem->stop();
	mItem->removeEventListener(this);

	LinkedList::iterator it;
	for(it = mChildren.begin(); it != mChildren.end();)
	{
		delete *(it++);
	} 
}

void LinkItem::add(PlaylistObject *child)
{
	if (child != 0)
	{
		LinkItem *item = new LinkItem(child);
		mChildren.push_back(item);
	}
}

bool LinkItem::eventRaised(PlaylistEvent *anEvent)
{
	switch(anEvent->getReason())
	{
/*	case PlaylistEvent::STARTEVENT:
		break;
	case PlaylistEvent::NOPEVENT:
		break;
	case PlaylistEvent::PAUSEEVENT:
		break; */
	case PlaylistEvent::STOPEVENT:
		{
			LinkedList::iterator it;
			for(it = mChildren.begin(); it != mChildren.end();it++)
			{
				(*it)->start();
			}
		}
		break;
	default:
		break;
	}
	return true;
}

PlaylistObject *LinkItem::getItem() const
{
	return mItem;
}

void LinkItem::remove(PlaylistObject *child)
{
	LinkedList::iterator it;
	for(it = mChildren.begin(); it != mChildren.end();)
	{
		if ((*it)->getItem() == child)
		{
			mChildren.erase(it++);
		} else {
			++it;
		}
	}
}

void LinkItem::start()
{

	mItem->start();
}

void LinkItem::stop()
{
}

LinkedPlaylist::LinkedPlaylist() : Playlist(),
	mList(0)
{
}

LinkedPlaylist::~LinkedPlaylist()
{
	stop();
	delete mList;
}


void LinkedPlaylist::add(PlaylistObject *object, LinkItem *parent)
{
	if (object)
	{
		if (parent == 0)
		{
			LinkItem *item = new LinkItem(object);
			mList = item;
		} else {
			parent->add(object);
		}
		object->load();
		object->addEventListener(this);
	}
}

void LinkedPlaylist::remove(PlaylistObject *object, LinkItem *parent)
{
	if (object && parent)
	{
		object->removeEventListener(this);
		object->unload();
		parent->remove(object);
	}
}

void LinkedPlaylist::start()
{
	stop();
	if (mList != 0)
	{
		mPlaying = mList;
		mPlaying->start();
	}
}

void LinkedPlaylist::stop()
{
	if (mList != 0)
	{
		mList->stop();
	}
}

bool LinkedPlaylist::eventRaised(PlaylistEvent *anEvent)
{
	mPlaying->stop();
	return true;
}

LinkItem *LinkedPlaylist::getList() const
{
	return mList;
}

}
