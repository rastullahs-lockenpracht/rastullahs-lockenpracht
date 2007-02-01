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

#ifndef __TimerEvent_H__
#define __TimerEvent_H__

#include "RulesPrerequisites.h"

#include "EventObject.h"

#include "Date.h"

namespace rl {

class TimerEventSource;
class GameObject;

/** 
 *  TimerEvent
 *  Ein Event der ausgelöst wird, wenn sich eine Eigenschaft eines GameObjects verändert hat 
 *
 *  @see ObjectStateChangeListener, TimerEventSource
 *  @todo Feldgenauigkeit, also auch noch im Ereignis zu kodieren, was verändert wurde
 */
class _RlRulesExport TimerEvent : public virtual EventObject 
{
public:
	/** Der Standardkonstruktor
	* @param src Die Eventquelle
	*/
	TimerEvent( TimerEventSource* src );
	/// Standarddestruktor
	virtual ~TimerEvent() {};

	/// Setzt den Zeitpunkt des Events
	void setTime(Ogre::Real time) { mTime = time; }
	void setTime(Date gameTime) { mGameTime = gameTime; }

	/// Gibt den Zeitpunkt des Events zurück
	Ogre::Real getTime() { return mTime; }
	Date getGameTime() { return mGameTime; }

	/// Gibt die Eventquelle zurück
	TimerEventSource* getSource() const;
private:
	/// Das verursachende Object
	Ogre::Real mTime;
	Date mGameTime;
};
}

#endif
