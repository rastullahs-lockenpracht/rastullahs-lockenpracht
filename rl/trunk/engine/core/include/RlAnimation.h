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

#ifndef __RlAnimation_H__
#define __RlAnimation_H__

#include "CorePrerequisites.h"

#include "EventCaster.h"
#include "EventSource.h"
#include "EventObject.h"

namespace rl {

class _RlCoreExport RlAnimation : public virtual EventSource
{
    public:
        RlAnimation( AnimationState* animState, Real speed=1.0, unsigned int timesToPlay=0 );
		virtual ~RlAnimation( );
        
		// Pausieren und fortsetzen
        bool isPaused() const;
        void setPaused( bool isPaused );

		// Möglichkeit die globale SlowMotion zu umgehen
		// Nützlich für Statusanzeigen, oder ähnliche konstante Animationen
		// TODO Eventuell das ganze auf Flags erweitern
		bool isIgnoringGlobalSpeed() const;
		void setIgnoringGlobalSpeed( bool isIgnoringGlobalSpeed );

		// Regelbare Geschwindigkeit
		Real getSpeed() const;
		void setSpeed( Real speed );
		void reverseAnimation();

		// Regelbare Wiederholungsanzahl
		void setTimesToPlay(unsigned int timesToPlay);
		unsigned int getTimesPlayed() const;
		void resetTimesPlayed();
		unsigned int getTimesToPlay() const;
		unsigned int getTimesToPlayLeft() const;
		Real getTimePlayed() const;

		// Gewicht (Einfluss) der Animation
		Real getWeight(void) const;
        void setWeight(Real weight);

		// Zeit hinzufügen // wird vom AnimationManager aufgerufen
		void addTime( Real timePassed );

		// AnimationFinishedListener
		void addAnimationFinishedListener(EventListener<EventObject> *listener);
		void removeAnimationFinishedListener(EventListener<EventObject> *listener);

		AnimationState* getAnimationState() { return mAnimState; };
    protected:
		virtual void applyTime( Real timePassed );

		Real mLength;
		AnimationState* mAnimState;

        bool mPaused;
		bool mIgnoringGlobalSpeed;
		Real mSpeed;
		unsigned int mTimesToPlay;
		Real mTimePlayed;

		EventCaster<EventObject> mAnimationFinishedCaster;
};

}
#endif
