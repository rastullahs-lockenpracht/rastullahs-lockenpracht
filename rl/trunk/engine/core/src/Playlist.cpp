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

#include "Playlist.h"
#include "SoundObject.h"
#include "SoundChannel.h"
#include "GameLoop.h"
#include "PlaylistObject.h"

namespace rl
{

Playlist::Playlist() : EventListener<EventObject>(),
	mLooping(false)
{
}

Playlist::~Playlist()
{
	stop();
	for(mItem = mQueue.begin(); mItem != mQueue.end();)
	{
		remove(*mItem);
	}
}


void Playlist::add(PlaylistObject *object)
{
	if (object)
	{
		mQueue.push_back(object);
		object->load();
		object->addEventListener(this);
	}
}

void Playlist::remove(PlaylistObject *object)
{
	if (object)
	{
		object->removeEventListener(this);
		object->unload();
		if (*mItem == object)
		{
			mItem++;
		}
		mQueue.remove(object);
	}
}

void Playlist::start()
{
	stop();
	mItem = mQueue.begin();
	(*mItem)->start();
}

void Playlist::stop()
{
	ObjectList::iterator it;
	for(it = mQueue.begin(); it != mQueue.end(); it++)
	{
		(*it)->stop();
	}
}

bool Playlist::eventRaised(EventObject *anEvent)
{
	(*mItem)->stop();
	if (mItem != mQueue.end())
	{
		mItem++;
	} else {
		if (mLooping)
		{
			mItem = mQueue.begin();
		}
	}
	if (mItem != mQueue.end())
	{
		(*mItem)->start();
	}
	return true;
}

void Playlist::setLooping(bool looping)
{
	mLooping = looping;
}

bool Playlist::isLooping() const
{
	return mLooping;
}

}
