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

#ifndef __RL_SCRIPTTIMER_H__
#define __RL_SCRIPTTIMER_H__

#include <set>
#include <string>
#include <vector>

#include "ScriptPrerequisites.h"
#include "SynchronizedTask.h"

namespace rl {

	class SynchronizedTask;
	class TimerTask;

	class TaskCall
	{
	public:
		RL_LONGLONG time;
		TimerTask* task;
		rl::CeGuiString name;

		TaskCall();
	};

	typedef std::set<TaskCall*> TaskCallSet;

	class ScriptTimer : public Ogre::Singleton<ScriptTimer>, public SynchronizedTask
	{
	public:
		ScriptTimer();
		~ScriptTimer();

		void registerTask(TimerTask* task);
		void unregisterTask(TimerTask* task);
		void registerTaskCallGameTime(RL_LONGLONG time, TimerTask* task, const rl::CeGuiString& name);

		void run(Real elapsedTime);

		static ScriptTimer& getSingleton();
		static ScriptTimer* getSingletonPtr();

	private:
		std::set<TimerTask*> mTasks;
		TaskCallSet mSchedule;
	};

}

#endif
