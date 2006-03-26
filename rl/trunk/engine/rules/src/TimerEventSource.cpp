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

#include "TimerEventSource.h"

#include "TimerEvent.h"
#include "TimerManager.h"
#include "TimerListener.h"
#include "GameObject.h"
#include "DsaManager.h"
#include "CoreSubsystem.h"
#include "ScriptWrapper.h"

using Ogre::Real;

namespace rl {

	const RL_LONGLONG TimerEventSource::NO_REPEAT = 0L;

	TimerEventSource::TimerEventSource( RL_LONGLONG time, RL_LONGLONG repeat ) :
		mTimerEventCaster(),
		mType(TIMER_GAME_TIME),
		mGameTime(time),
		mGameTimeRepeatInterval(repeat),
		mGameTimeLastCall(0L),
		mDsaTime(0L),
		mDsaTimeLastCall(0L),
		mDsaTimeRepeatInterval(0L)
	{ 
		TimerManager::getSingleton().registerTimerEventSource(this);
	}

	TimerEventSource::TimerEventSource( Date time, Date repeat ) :
		mTimerEventCaster(),
		mType(TIMER_DSA_TIME),
		mGameTime(0L),
		mGameTimeRepeatInterval(0L),
		mGameTimeLastCall(0L),
		mDsaTime(time),
		mDsaTimeRepeatInterval(repeat),
		mDsaTimeLastCall(0L)
	{       
		TimerManager::getSingleton().registerTimerEventSource(this);
	}

    TimerEventSource::~TimerEventSource() 
    {
		// Alle TimerListener
		EventCaster<TimerEvent>::EventSet evSet 
			= mTimerEventCaster.getEventSet();
		EventCaster<TimerEvent>::EventSet::iterator citer 
			= evSet.begin();
		for (citer; citer != evSet.end(); ) 
		{
			EventListener<TimerEvent>* ev = *citer; 
			TimerListener* al = dynamic_cast<TimerListener*>( ev );
			ScriptWrapper::getSingleton().disowned( al );
			citer++;
		}
        mTimerEventCaster.removeEventListeners();
    }

    
	void TimerEventSource::fireTimerEvent()
	{
		TimerEvent* evt = new TimerEvent(this);
		if (mType == TIMER_DSA_TIME)
		{
			Date date = DsaManager::getSingleton().getCurrentDate();
			evt->setTime(date);
			mDsaTimeLastCall = date;
		}
		else if (mType == TIMER_GAME_TIME)
		{
			RL_LONGLONG time = CoreSubsystem::getSingleton().getClock();
			evt->setTime(time);
			mGameTimeLastCall = time;
		}

		mTimerEventCaster.dispatchEvent(evt);
	}

    void TimerEventSource::addTimerListener( TimerListener*  list )
    {
		if( !mTimerEventCaster.containsListener( list ) )
		{
			mTimerEventCaster.addEventListener( list );
			ScriptWrapper::getSingleton().owned(list);
		}
    }

    void TimerEventSource::removeTimerListener( TimerListener* list )
    {
		if( mTimerEventCaster.containsListener( list ) )
		{
			mTimerEventCaster.removeEventListener( list );
			ScriptWrapper::getSingleton().disowned(list);
		}
    }

    bool TimerEventSource::hasListeners( ) const
    {
        return mTimerEventCaster.hasEventListeners();
    }

	bool TimerEventSource::injectTimePulse(RL_LONGLONG gameTime, Date dsaTime)
	{
		bool deleteSource = false;

		if (mType == TIMER_DSA_TIME)
		{
			if (mDsaTime < dsaTime && mDsaTimeLastCall + mDsaTimeRepeatInterval <= dsaTime)
			{
				if (mDsaTimeRepeatInterval == NO_REPEAT)
					deleteSource = true;
				fireTimerEvent();
			}
		}
		else if (mType == TIMER_GAME_TIME)
		{
			if (mGameTime < gameTime && mGameTimeLastCall + mGameTimeRepeatInterval <= gameTime)
			{
				if (mGameTimeRepeatInterval == NO_REPEAT)
					deleteSource = true;
				fireTimerEvent();
			}
		}

		return deleteSource;
	}
}

