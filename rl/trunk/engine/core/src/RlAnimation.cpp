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

#include "RlAnimation.h"

#include "Exception.h"

namespace rl {

RlAnimation::RlAnimation( AnimationState* animState, Real speed, unsigned int timesToPlay )
{
	if( animState == 0 )
		Throw( NullPointerException,"AnimationState darf nicht null sein" );

	mAnimState = animState;
    mPaused = false;
	mIgnoringGlobalSpeed = false;
	mTimesToPlay = timesToPlay;
	mTimePlayed = 0;
	mSpeed = speed;

	mLength = mAnimState->getLength();
	mAnimState->setLoop( true );
	mAnimState->setEnabled(true);
	// TODO evtl. Gewicht zurücksetzen
}
 
RlAnimation::~RlAnimation()
{
	mAnimState->setEnabled(false);
}

bool RlAnimation::isPaused() const
{
    return mPaused;
}

void RlAnimation::setPaused( bool isPaused )
{
    mPaused = isPaused;
}


bool RlAnimation::isIgnoringGlobalSpeed() const
{
	return mIgnoringGlobalSpeed;
}

void RlAnimation::setIgnoringGlobalSpeed( bool isIgnoringGlobalSpeed )
{
	mIgnoringGlobalSpeed = isIgnoringGlobalSpeed;
}

// Regelbare Geschwindigkeit
Real RlAnimation::getSpeed() const
{
	return mSpeed;
}

void RlAnimation::setSpeed( Real speed )
{
	mSpeed = speed;
}

void RlAnimation::reverseAnimation()
{
	mSpeed = -mSpeed;
}

// Regelbare Wiederholungsanzahl
void RlAnimation::setTimesToPlay(unsigned int timesToPlay)
{
	mTimesToPlay = timesToPlay;
}

unsigned int RlAnimation::getTimesToPlay() const
{
	return mTimesToPlay;
}

void RlAnimation::resetTimesPlayed()
{
	mTimePlayed = 0;
	mAnimState->setLoop( true );
}

Real RlAnimation::getTimePlayed() const
{
	return mTimePlayed;
}

unsigned int RlAnimation::getTimesPlayed() const
{
	return floor(mTimePlayed/mLength);
}

unsigned int RlAnimation::getTimesToPlayLeft() const
{
	return max(mTimesToPlay - getTimesPlayed(),0);
}

// Gewicht (Einfluss) der RlAnimation
Real RlAnimation::getWeight(void) const
{
	return mAnimState->getWeight();
}

void RlAnimation::setWeight(Real weight)
{
	mAnimState->setWeight(weight);
}

// Zeit hinzufügen // wird vom AnimationManager aufgerufen
void RlAnimation::addTime( Real timePassed )
{
	if( !mPaused )
	{
		timePassed = timePassed * mSpeed;

		mAnimState->addTime( timePassed );
		
		// Begrenzte Abspielanzahl
		if( mTimesToPlay > 0 )
		{
			if( getTimesToPlayLeft() == 1 )
			{
				mAnimState->setLoop(false);
			}
			else if( getTimesToPlayLeft() == 0 ) 
			{
				setPaused(true);
				// TODO RlAnimation Ended Listener
			}
		}

		// TODO Frame-Listener
	}
}

}
