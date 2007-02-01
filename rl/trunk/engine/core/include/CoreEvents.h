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

#ifndef __CoreEvent_H__
#define __CoreEvent_H__

#include "Actor.h"
#include "EventObject.h"
#include "EventListener.h"

#include "CorePrerequisites.h"

namespace rl {

class _RlCoreExport CoreEvent : public EventObject 
{
public:
    /// Daten wurden geladen
    static const unsigned int DATA_LOADED = 200;

    /** Der Standardkonstruktor
      * @param src Die Eventquelle
      * @param reason Der Grund des Events
      */
    CoreEvent( const unsigned int reason );
    /// Standarddestruktor
    virtual ~CoreEvent() {};
};

class _RlCoreExport DataLoadedEvent : public CoreEvent 
{
public:
    /** Der Standardkonstruktor
      * @param src Die Eventquelle
      * @param reason Der Grund des Events
      */
    DataLoadedEvent(float percent);
    /// Standarddestruktor
    virtual ~DataLoadedEvent() {};

	float getPercentLoaded();

private:
	float mPercentLoaded;
};

class _RlCoreExport CoreEventListener : public EventListener<CoreEvent>
{
public:
	CoreEventListener();

	bool eventRaised(CoreEvent* anEvent);

	virtual bool dataLoadedEventRaised(DataLoadedEvent* anEvent) = 0;
};

}

#endif
