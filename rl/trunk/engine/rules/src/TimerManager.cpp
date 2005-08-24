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

#include "TimerManager.h"

#include "GameLoop.h"
#include "CoreSubsystem.h"
#include "DsaManager.h"
#include "TimerEventSource.h"

using Ogre::Singleton;
using Ogre::StringConverter;

template<> rl::TimerManager* Singleton<rl::TimerManager>::ms_Singleton = 0;

namespace rl {

	TimerManager& TimerManager::getSingleton()
	{
		return Singleton<TimerManager>::getSingleton();
	}

	TimerManager* TimerManager::getSingletonPtr()
	{
		return Singleton<TimerManager>::getSingletonPtr();
	}

	TimerManager::TimerManager()
	{
		mTimers.clear();
		GameLoopManager::getSingleton().addSynchronizedTask(this, FRAME_ENDED);
	}

	TimerManager::~TimerManager()
	{
		GameLoopManager::getSingleton().removeSynchronizedTask(this);
	}

	void TimerManager::run(Ogre::Real elapsedTime)
	{
		RL_LONGLONG nowDsa = DsaManager::getSingleton().getTimestamp();
		RL_LONGLONG now = CoreSubsystem::getSingleton().getClock();

		for (std::list<TimerEventSource*>::iterator timerIter = mTimers.begin(); timerIter != mTimers.end(); timerIter++)
		{
			TimerEventSource* currTimer = *timerIter;
			bool toDelete = currTimer->injectTimePulse(now, nowDsa);
			if (toDelete)
				timerIter = mTimers.erase(timerIter);
		}
	}

	void TimerManager::registerTimerEventSource(TimerEventSource* source)
	{
		mTimers.push_back(source);
	}

}

