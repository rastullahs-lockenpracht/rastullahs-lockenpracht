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

#ifndef __RL_TIMERTASK_H__
#define __RL_TIMERTASK_H__

#include <ScriptPrerequisites.h>

namespace rl {
	
	class Date;
	using Ogre::String;

	class TimerTask
	{
	public:
		TimerTask(const rl::CeGuiString& name);
		virtual ~TimerTask();

		void setActive(bool active);
		bool isActive();

		virtual void run(const rl::CeGuiString& name); // muss in Ruby überschrieben werden, ruft eine Methode anhand ihres Namens auf

		void scheduleRelativeToActivation(const rl::CeGuiString& name, long timeAfterActivation);
		void scheduleRelativeToStart(const rl::CeGuiString& name, long timeAfterStart);
		void scheduleRelativeToGametime(const rl::CeGuiString& name, Date& gameTime);

	private:
		bool mActive;
		rl::CeGuiString mName;
	};
}

#endif
