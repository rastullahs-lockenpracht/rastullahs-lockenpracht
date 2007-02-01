/*
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

#include "PauseObject.h"
#include <boost/thread.hpp>

using namespace boost;

namespace rl
{
	PauseObject::PauseObject(unsigned long nsec)
		: PlaylistObject(),
		mNSec(nsec),
		mThread(0),
		mThreadFunctor()
	{
		mThreadFunctor.that = this;
	}

	PauseObject::~PauseObject()
	{
		stop();
	}

	const long unsigned int PauseObject::getPause() const
	{
		return mNSec;
	}

	void PauseObject::start()
	{
		PlaylistObject::start();
		if (mThread != 0)
		{
			mThread = new thread(mThreadFunctor);
		}
	}

	void PauseObject::stop()
	{
		PlaylistObject::stop();
		if (mThread != 0)
		{
			delete mThread;
			mThread = 0;
		}
	}

	void PauseObject::pause()
	{
		PlaylistObject::pause();
		// Big TODO, Warten unterbrechen
	}

	void PauseObject::ThreadFunctor::operator()()
	{
		xtime timeToSleep;
		xtime_get(&timeToSleep, TIME_UTC);
		timeToSleep.nsec += that->mNSec;          
		thread::sleep(timeToSleep);
	}

}
