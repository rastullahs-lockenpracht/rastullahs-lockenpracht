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

#include "AnimationListener.h"

namespace rl {

class _RlCoreExport RlAnimation : public virtual EventSource
{
    public:
        RlAnimation(Ogre::AnimationState* animState, Ogre::Real speed=1.0,
            unsigned int timesToPlay=0);
		RlAnimation(Ogre::Real length);
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
		Ogre::Real getSpeed() const;
		void setSpeed( Ogre::Real speed );
		void reverseAnimation();

		// Regelbare Wiederholungsanzahl
		void setTimesToPlay(unsigned int timesToPlay);
		unsigned int getTimesPlayed() const;
		void resetTimesPlayed();
		unsigned int getTimesToPlay() const;
		unsigned int getTimesToPlayLeft() const;
		Ogre::Real getTimePlayed() const;

		// Gewicht (Einfluss) der Animation
		Ogre::Real getWeight(void) const;
        void setWeight(Ogre::Real weight);

		// Zeit hinzufügen // wird vom AnimationManager aufgerufen
		void addTime( Ogre::Real timePassed );

		// AnimationFinishedListener
		void addAnimationListener( AnimationListener *listener);
		void removeAnimationListener( AnimationListener *listener);

		Ogre::AnimationState* getAnimationState() { return mAnimState; };
    protected:
		Ogre::Real mLength;
		Ogre::AnimationState* mAnimState;

        bool mPaused;
		bool mIgnoringGlobalSpeed;
		Ogre::Real mSpeed;
		unsigned int mTimesToPlay;
		Ogre::Real mTimePlayed;

		EventCaster<AnimationEvent> mAnimationCaster;

		void setAnimationState( Ogre::AnimationState* animState );
};

}
#endif
