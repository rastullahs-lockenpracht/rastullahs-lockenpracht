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

#ifndef __TimerEventSource_H__
#define __TimerEventSource_H__

#include "EventSource.h"
#include "EventCaster.h"

#include "TimerListener.h"
#include "TimerEvent.h"

#include "RulesPrerequisites.h"

#include <OgreUserDefinedObject.h>

namespace rl {

class GameObject;
/** 
 * TimerEventSource
 *
 *  @see TimerEvent, TimerListener, GameObject
 */
class _RlRulesExport TimerEventSource : public virtual EventSource
{
public:
	static const RL_LONGLONG NO_REPEAT;

    /** Konstruktor
     */
	TimerEventSource( const RL_LONGLONG gameTime, const RL_LONGLONG interval = NO_REPEAT );
	TimerEventSource( const Date dsaTime, const Date interval = NO_REPEAT );

	/// Dekonstruktor
    virtual ~TimerEventSource();

    /** Fügt einen TimerListener hinzu, der zukünftig bei TimerEvents benachrichtigt wird 
     *
     * @param list Der hinzuzufügende Listener
     */
    void addTimerListener( TimerListener*  list );
    /** Entfernt einen TimerListener
    *
    * @param list Der zu entfernende Listener
    */
    void removeTimerListener( TimerListener* list );

	/**
	 * Regt eine Timerüberprüfung an
	 *
	 * @param gameTime die abgelaufene Spielzeit
	 * @param dsaTime die aktuelle DSA-Zeit
	 * @return true, falls das Ereignis nie wieder auftreten kann und somit gelöscht werden soll
	 */
	bool injectTimePulse(RL_LONGLONG gameTime, Date dsaTime);

    /// Gibt zurück ob sich Listener angemeldet haben
    bool hasListeners( ) const;

    void fireTimerEvent();

private: 

	enum TimerType {
		TIMER_GAME_TIME,
		TIMER_DSA_TIME
	};

    /// Der EventCaster der die Verteilung an die Listener übernimmt
    EventCaster<TimerEvent> mTimerEventCaster;
    /// Das Object der überwacht wird
	RL_LONGLONG mGameTime;
	RL_LONGLONG mGameTimeLastCall;
	RL_LONGLONG mGameTimeRepeatInterval;

	Date mDsaTime;
    Date mDsaTimeLastCall;
	Date mDsaTimeRepeatInterval;

	TimerType mType;
};

}

#endif
