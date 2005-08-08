/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#include "TimerManager.h"
#include "OgreSingleton.h"
#include "GameLoop.h"
#include "DsaManager.h"
#include "TimerTask.h"
#include "CoreSubsystem.h"
#include "Date.h"

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
		mTasks.clear();
		mSchedule.clear();
		GameLoopManager::getSingleton().addSynchronizedTask(this);
	}

	TimerManager::~TimerManager()
	{
		GameLoopManager::getSingleton().removeSynchronizedTask(this);
	}

	void TimerManager::registerTask(TimerTask* task)
	{
		mTasks.insert(task);
	}

	void TimerManager::unregisterTask(TimerTask* task)
	{
		std::set<TimerTask*>::iterator iter = mTasks.find(task);

		if (iter != mTasks.end())
			mTasks.erase(iter);
	}

	void TimerManager::registerTaskCallGameTime(RL_LONGLONG time, TimerTask* task, const rl::CeGuiString& name)
	{
		TaskCall* call = new TaskCall();
		call->name = name;
		call->task = task;
		call->time = time;

		mSchedule.insert(call);
	}

	void TimerManager::run(Ogre::Real elapsedTime)
	{
		RL_LONGLONG now = DsaManager::getSingleton().getTimestamp();

		TaskCallSet execute;
		bool run = false;

		for (TaskCallSet::iterator iter = mSchedule.begin(); iter != mSchedule.end(); iter++)
		{
			TaskCall* call = *iter;
			if (call->time < now)
			{
				execute.insert(call);
				run = true;
			}
		}

		if (!run)
			return;
		
		CoreSubsystem::getSingleton().log(StringConverter::toString(mSchedule.size())+" in Schedule");
		for (TaskCallSet::iterator first = execute.begin(); first != execute.end(); first++)
		{			
			TaskCall* call = *first;
			CoreSubsystem::getSingleton().log(("1: Vor run '"+call->name+"'").c_str());
			CoreSubsystem::getSingleton().log("           '"+StringConverter::toString(reinterpret_cast<unsigned int>(call->task))+"'");
			Date d(call->time);
			CoreSubsystem::getSingleton().log("           '"+d.toString()+"'");
			call->task->run(call->name);
			CoreSubsystem::getSingleton().log(("2: Nach run '"+call->name+"'").c_str());
			mSchedule.erase(mSchedule.find(call));
			CoreSubsystem::getSingleton().log("3: Aus Schedule geloescht, noch "+StringConverter::toString(mSchedule.size()));
			delete call;
			CoreSubsystem::getSingleton().log("4: fertig");
		}			
	}

	TaskCall::TaskCall() :
		time(0),
		name(""),
		task(NULL)
	{
	}
}
