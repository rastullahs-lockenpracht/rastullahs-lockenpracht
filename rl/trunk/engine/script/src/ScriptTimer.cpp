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

#include "ScriptTimer.h"
#include "OgreSingleton.h"
#include "GameLoop.h"
#include "DsaManager.h"
#include "TimerTask.h"

using namespace Ogre;

template<> rl::ScriptTimer* Singleton<rl::ScriptTimer>::ms_Singleton = 0;

namespace rl {

	ScriptTimer& ScriptTimer::getSingleton()
	{
		return Singleton<ScriptTimer>::getSingleton();
	}

	ScriptTimer* ScriptTimer::getSingletonPtr()
	{
		return Singleton<ScriptTimer>::getSingletonPtr();
	}

	ScriptTimer::ScriptTimer()
	{
		mTasks.clear();
		mSchedule.clear();
		GameLoop::getSingleton().addSynchronizedTask(this);
	}

	ScriptTimer::~ScriptTimer()
	{
		GameLoop::getSingleton().removeSynchronizedTask(this);
	}

	void ScriptTimer::registerTask(TimerTask* task)
	{
		mTasks.insert(task);
	}

	void ScriptTimer::unregisterTask(TimerTask* task)
	{
		std::set<TimerTask*>::iterator iter = mTasks.find(task);

		if (iter != mTasks.end())
			mTasks.erase(iter);
	}

	void ScriptTimer::registerTaskCallGameTime(RL_LONGLONG time, TimerTask* task, const rl::CeGuiString& name)
	{
		TaskCall* call = new TaskCall();
		call->name = name;
		call->task = task;
		call->time = time;

		mSchedule.insert(call);
	}

	void ScriptTimer::run(Real elapsedTime)
	{
		RL_LONGLONG now = DsaManager::getSingleton().getTimestamp();

		TaskCallSet execute;

		for (TaskCallSet::iterator iter = mSchedule.begin(); iter != mSchedule.end(); iter++)
		{
			TaskCall* call = *iter;
			if (call->time < now)
				execute.insert(call);
		}

		while (execute.size() > 0)
		{
			TaskCallSet::iterator first = execute.begin();
			TaskCall* call = *first;
			call->task->run(call->name);
			execute.erase(first);
			mSchedule.erase(mSchedule.find(call));
		}	
	}

	TaskCall::TaskCall() :
		time(0),
		name(""),
		task(NULL)
	{
	}
}