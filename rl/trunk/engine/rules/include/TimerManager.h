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

#ifndef __TimerManager_H__
#define __TimerManager_H__

#include "RulesPrerequisites.h"
#include "GameTask.h"

#include <OgreSingleton.h>

namespace rl {

class TimerEventSource;

/** 
 *  TimerEvent
 *  Ein Event der ausgelöst wird, wenn ein Zeitpunkt erreicht wurde
 *
 *  @see TimerListener, TimerEventSource
 */
class _RlRulesExport TimerManager : public GameTask, public Ogre::Singleton<TimerManager>
{
public:
	static TimerManager& getSingleton();
	static TimerManager* getSingletonPtr();

	TimerManager();
	~TimerManager();

	void registerTimerEventSource(TimerEventSource* source);
	void unregisterTimerEventSource(TimerEventSource* source);

protected:
	virtual void run( Ogre::Real elapsedTime );

private:
	std::list<TimerEventSource*> mTimers;
};

}

#endif //__TimerManager_H__
