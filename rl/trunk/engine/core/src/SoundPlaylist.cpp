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

#include "SoundPlaylist.h"
#include "SoundObject.h"
#include "SoundChannel.h"
#include "GameLoop.h"

namespace rl
{

SoundPlaylist::SoundPlaylist() :
	mLooping(false)
{
}

SoundPlaylist::~SoundPlaylist()
{
	stop();
	for(mItem = mQueue.begin(); mItem != mQueue.end();)
	{
		remove(*mItem);
	}
}


void SoundPlaylist::add(SoundObject *object)
{
	if (object)
	{
		mQueue.push_back(object);
		object->load();
		object->getSoundChannel()->addEventListener(this);
	}
}

void SoundPlaylist::remove(SoundObject *object)
{
	if (object)
	{
		object->getSoundChannel()->removeEventListener(this);
		object->unload();
		if (*mItem == object)
		{
			mItem++;
		}
		mQueue.remove(object);
	}
}

void SoundPlaylist::start()
{
	stop();
	mItem = mQueue.begin();
	(*mItem)->play();
}

void SoundPlaylist::stop()
{
	SoundObjectList::iterator it;
	for(it = mQueue.begin(); it != mQueue.end(); it++)
	{
		(*it)->stop();
	}
}

bool SoundPlaylist::eventRaised(SoundEvent *anEvent)
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
		(*mItem)->play();
	}
	return true;
}

void SoundPlaylist::setLooping(bool looping)
{
	mLooping = looping;
}

bool SoundPlaylist::isLooping() const
{
	return mLooping;
}

}
