/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#include "Exception.h"
#include "TimerTask.h"
#include "ScriptTimer.h"
#include "Date.h"

namespace rl {

	TimerTask::TimerTask(const rl::CeGuiString& name) :
		mName(name),
		mActive(false)
	{
		
	}

	TimerTask::~TimerTask()
	{
		setActive(false);
	}

	void TimerTask::setActive(bool active)
	{
		if (active && !mActive)
		{
			ScriptTimer::getSingleton().registerTask(this);
			mActive = true;
		}
		else if (!active && mActive)
		{
			ScriptTimer::getSingleton().unregisterTask(this);
			mActive = false;
		}
	}

	bool TimerTask::isActive()
	{
		return mActive;
	}

	void TimerTask::run(const rl::CeGuiString& name)
	{
		RlFail("Not implemented. Must be overwritten.");
	}

	void TimerTask::scheduleRelativeToActivation(const rl::CeGuiString& name, long timeAfterActivation)
	{
	}

	void TimerTask::scheduleRelativeToStart(const rl::CeGuiString& name, long timeAfterStart)
	{
	}

	void TimerTask::scheduleRelativeToGametime(const rl::CeGuiString& name, Date& gameTime)
	{
		ScriptTimer::getSingleton().registerTaskCallGameTime(gameTime.getTimestamp(), this, name);
	}
}
