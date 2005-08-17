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

#ifndef __RL_TIMERTASK_H__
#define __RL_TIMERTASK_H__

#include "CorePrerequisites.h"

namespace rl {
	
	class Date;
	using Ogre::String;

	class _RlCoreExport TimerTask
	{
	public:
		TimerTask(const Ogre::String& name);
		virtual ~TimerTask();

		void setActive(bool active);
		bool isActive();

		virtual void run(const Ogre::String& name); // muss in Ruby überschrieben werden, ruft eine Methode anhand ihres Namens auf

		void scheduleRelativeToActivation(const Ogre::String& name, long timeAfterActivation);
		void scheduleRelativeToStart(const Ogre::String& name, long timeAfterStart);
		void scheduleRelativeToGametime(const Ogre::String& name, Date& gameTime);

	private:
		bool mActive;
		Ogre::String mName;
	};
}

#endif
