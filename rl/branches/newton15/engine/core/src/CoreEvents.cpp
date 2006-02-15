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

#ifndef __GameAreaEvent_H__
#define __GameAreaEvent_H__

#include "CoreEvents.h"
#include "CoreSubsystem.h"

namespace rl {

	CoreEvent::CoreEvent(const unsigned int reason)
		: EventObject(CoreSubsystem::getSingletonPtr(), reason)
	{
	}

	DataLoadedEvent::DataLoadedEvent(float percent)
		: CoreEvent(CoreEvent::DATA_LOADED),
		mPercentLoaded(percent)
	{
		
	}

	float DataLoadedEvent::getPercentLoaded()
	{
		return mPercentLoaded;
	}

	CoreEventListener::CoreEventListener()
		: EventListener<CoreEvent>()
	{
	}

	bool CoreEventListener::eventRaised(rl::CoreEvent *anEvent)
	{
		if (anEvent->getReason() == CoreEvent::DATA_LOADED)
			return dataLoadedEventRaised(dynamic_cast<DataLoadedEvent*>(anEvent));

		return false;
	}

}

#endif
