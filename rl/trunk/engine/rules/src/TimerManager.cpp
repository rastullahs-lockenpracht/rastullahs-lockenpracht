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
#include "stdinc.h" //precompiled header

#include "TimerManager.h"

#include "GameLoop.h"
#include "CoreSubsystem.h"
#include "DsaManager.h"
#include "TimerEventSource.h"
#include "ScriptWrapper.h"
#include "Exception.h"


using Ogre::Singleton;
using Ogre::StringConverter;

template<> rl::TimerManager* Singleton<rl::TimerManager>::ms_Singleton = 0;

namespace rl {

	TimerManager::TimerManager() :
		mTimers()
	{
        GameLoop::getSingleton().addTask(this, GameLoop::TG_GRAPHICS);
	}

	TimerManager::~TimerManager()
	{
		for (std::set<TimerEventSource*>::iterator timerIter = mTimers.begin();
			timerIter != mTimers.end();timerIter++)
		{
			TimerEventSource* currTimer = *timerIter;
			ScriptWrapper::getSingleton().disowned( currTimer );
		}
		mTimers.clear();
		GameLoop::getSingleton().removeTask(this);
	}

	void TimerManager::run(Ogre::Real elapsedTime)
	{
		RL_LONGLONG nowDsa = DsaManager::getSingleton().getTimestamp();
		RL_LONGLONG now = GameLoop::getSingleton().getClock();

		for (std::set<TimerEventSource*>::iterator timerIter = mTimers.begin();
			timerIter != mTimers.end();)
		{
			TimerEventSource* currTimer = *timerIter;
			bool toDelete = currTimer->injectTimePulse(now, nowDsa);

			if (toDelete)
			{
				mTimers.erase( timerIter++ );
				ScriptWrapper::getSingleton().disowned( currTimer );
			}
			else
				++timerIter;
		}
	}

	void TimerManager::registerTimerEventSource(TimerEventSource* source)
	{
		if (mTimers.find(source) != mTimers.end())
		{
            Throw(IllegalArgumentException, "TimerEventSource already registered.");
        }
		mTimers.insert(source);
		ScriptWrapper::getSingleton().owned( source );
	}

	void TimerManager::unregisterTimerEventSource(TimerEventSource* source)
    {
		if (mTimers.find(source) == mTimers.end())
		{
            Throw(IllegalArgumentException, "TimerEventSource not registered.");
        }
		mTimers.erase(source);
		ScriptWrapper::getSingleton().disowned(source);
    }

    const Ogre::String& TimerManager::getName() const
    {
        static Ogre::String NAME = "TimerManager";

        return NAME;
    }
}
