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
	TimerManager();
	virtual ~TimerManager();

	void registerTimerEventSource(TimerEventSource* source);
	void unregisterTimerEventSource(TimerEventSource* source);

    virtual const Ogre::String& getName() const;

protected:
	virtual void run( Ogre::Real elapsedTime );

private:
	std::set<TimerEventSource*> mTimers;
};

}

#endif //__TimerManager_H__
