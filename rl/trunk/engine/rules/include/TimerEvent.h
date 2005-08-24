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

#ifndef __TimerEvent_H__
#define __TimerEvent_H__

#include <OgreUserDefinedObject.h>

#include "EventObject.h"

#include "RulesPrerequisites.h"
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
