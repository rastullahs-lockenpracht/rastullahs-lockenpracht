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
#include "CoreSubsystem.h"
#include <OgreStringConverter.h>

using namespace Ogre;
namespace rl {

RlAnimation::RlAnimation( AnimationState* animState, Real speed, unsigned int timesToPlay ) :
	EventSource(), 
	mAnimationFrameListener(),
	mAnimationCaster()
{
    mPaused = false;
	mIgnoringGlobalSpeed = false;
	mTimesToPlay = timesToPlay;
	mTimePlayed = 0;
	mSpeed = speed;

	this->setAnimationState(animState);
}

RlAnimation::RlAnimation( ) :
	EventSource(),
	mAnimationFrameListener(),
	mAnimationCaster()
{
	mPaused = true;
	mIgnoringGlobalSpeed = false;
	mTimesToPlay = 0;
	mTimePlayed = 0;
	mSpeed = 1.0;
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
	if( mPaused && !isPaused )
	{
		AnimationEvent* animEve = new AnimationEvent(this,AnimationEvent::ANIMATION_UNPAUSED);
		mAnimationCaster.dispatchEvent( animEve );
		delete animEve;
	}
	else if( !mPaused && isPaused )
	{
		AnimationEvent* animEve = new AnimationEvent(this,AnimationEvent::ANIMATION_PAUSED);
		mAnimationCaster.dispatchEvent( animEve );
		delete animEve;
	}

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
	// Eventuell auf Anfang/Ende setzen
	if( speed < 0 && mAnimState->getTimePosition() == 0 )
		mAnimState->setTimePosition( mAnimState->getLength() );
	else if( speed > 0 && mAnimState->getTimePosition() == mAnimState->getLength() )
		mAnimState->setTimePosition( 0 );

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
	// Zurückspulen
	if( mSpeed < 0 )
		mAnimState->setTimePosition( mAnimState->getLength() );
	else if( mSpeed > 0 )
		mAnimState->setTimePosition( 0 );

	mTimePlayed = 0;
	mAnimState->setLoop( true );
	setPaused( false );
}

Real RlAnimation::getTimePlayed() const
{
	return mTimePlayed;
}

unsigned int RlAnimation::getTimesPlayed() const
{
	return floor(mTimePlayed/mAnimState->getLength());
}

unsigned int RlAnimation::getTimesToPlayLeft() const
{
	// Nicht unsigned :)
	int left = mTimesToPlay - getTimesPlayed();
	return max( left ,0);
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

void RlAnimation::addAnimationListener(AnimationListener *listener)
{
	mAnimationCaster.addEventListener(listener);
}

void RlAnimation::removeAnimationListener(AnimationListener *listener)
{
	mAnimationCaster.removeEventListener(listener);
}

void RlAnimation::addAnimationFrameListener( 
	AnimationFrameListener *listener, Ogre::Real frameNumber)
{
	mAnimationFrameListener.insert( 
		std::pair<Ogre::Real,AnimationFrameListener*>(frameNumber,listener) );
}

void RlAnimation::removeAnimationFrameListener( AnimationFrameListener *listener)
{
	///@todo Alle zugehörigen löschen
}

void RlAnimation::removeAnimationFrameListener( 
	AnimationFrameListener *listener, Ogre::Real frameNumber)
{
	///@todo Zugehöriges löschen
}

// Zeit hinzufügen // wird vom AnimationManager aufgerufen
void RlAnimation::addTime( Real timePassed )
{
	if( !mPaused )
	{
		timePassed = timePassed * mSpeed;

		if( !mAnimationFrameListener.empty() && timePassed != 0 )
			checkAnimationFrameListeners( timePassed );

		mAnimState->addTime( timePassed );

		// Begrenzte Abspielanzahl
		if( mTimesToPlay > 0 )
		{
			mTimePlayed += fabs(timePassed);

			if( getTimesToPlayLeft() == 1 )
			{
				mAnimState->setLoop(false);
			}
			else if( getTimesToPlayLeft() == 0 ) 
			{
				mAnimState->setLoop(false);
				mPaused = true;
			
				AnimationEvent* animEve = new AnimationEvent(this,AnimationEvent::ANIMATION_FINISHED);
				mAnimationCaster.dispatchEvent( animEve );
				delete animEve;			
			}
		}

		// TODO Frame-Listener
	}
}
/**
	Benachrichtigt die AnimationFrameListener

	Erfolgt in drei Schritten 
	 * Aktueller Durchlauf
	 * Wenn Überlauf, dann wird die Anzahl weiterer Durchläufe bestimmt
	 * Für den Rest im letzten Durchlauf wird erneut geprüft
*/
void RlAnimation::checkAnimationFrameListeners( Ogre::Real timePassed )
{
	// Iteratoren
	std::multimap<Ogre::Real,AnimationFrameListener*>::iterator 
		lowerBorder;
	std::multimap<Ogre::Real,AnimationFrameListener*>::iterator 
		upperBorder;
	std::multimap<Ogre::Real,AnimationFrameListener*>::iterator 
		iter;
	float lower, upper;
	// Event erzeugen
	AnimationFrameEvent* animEve = new AnimationFrameEvent(this,
		AnimationFrameEvent::ANIMATION_FRAMEREACHED,0);

	// Eventuelles Vorzeichen weg
	timePassed = fabs( timePassed );

	// Vorwärts laufen
	if( mSpeed > 0 )		
	{
		lower = max(mAnimState->getTimePosition(),0);
		upper = min(mAnimState->getTimePosition()+timePassed, mAnimState->getLength() );
	}
	// Das ganze rückwärts
	else
	{
		// FIX für die erste Runde beim Rückwärtsspielen, beginnt leider bei 0, nicht Length
		Ogre::Real timePos = mAnimState->getTimePosition();
		if( timePos == 0 )
			timePos = mAnimState->getLength();

		lower = max(timePos-timePassed,0);
		upper = min(timePos, mAnimState->getLength() );
	}

	// Iteratoren für die Grenzen holen
	lowerBorder = mAnimationFrameListener.lower_bound(
		lower);
	upperBorder = mAnimationFrameListener.upper_bound(
		upper);

	// Alle Listener innerhalb der Grenzen benachrichtigen
	for (iter = lowerBorder; iter != upperBorder; iter++) 
	{
		animEve->setFrameNumber( iter->first );
		iter->second->animationFrameReached(animEve);
	}

	// Einmal abspielen abziehen
	timePassed -= mAnimState->getLength();
	// Ums Wrapping kümmern - wenn Looping - und AbspielZeit
	if( timePassed > 0 && mAnimState->getLoop() && 
		// Falls begrenzte Wiederholungen, müssen mindestens 2(1+die oben abgearbeitete) fehlen
		( ( mTimesToPlay > 0 && getTimesToPlayLeft() > 1 ) || ( mTimesToPlay == 0 ) )
	   )
	{
		// Wie oft passt der Event in die gesamte fortgeschrittene Zeit		
		unsigned int timesSkipped = floor( timePassed/mAnimState->getLength() );
		Ogre::Real timeLeft = timePassed - timesSkipped*mAnimState->getLength();

		// Falls die Abspielanzahlbegrenzt ist, nicht häufiger als verbliebene Anzahl abspielen
		if( mTimesToPlay > 0 && timesSkipped >= getTimesToPlayLeft()-1)
		{
			timesSkipped = getTimesToPlayLeft()-1;
			// Restzeit unwichtig, letzen Abspielvorgänge waren komplette
			timeLeft = 0;
		}

		lowerBorder = mAnimationFrameListener.begin();
		upperBorder = mAnimationFrameListener.end();

		// Geskippte Events
		for( unsigned int i = 0; i < timesSkipped; i++)
		{
			for (iter = lowerBorder; iter != upperBorder; iter++) 
			{
				animEve->setFrameNumber( iter->first );
				iter->second->animationFrameReached(animEve);
			}
		}		

		// Für letzten Event prüfen, wenn noch Restzeit vorhanden ist
		if( timeLeft > 0 )
		{
			// Wrapping nach oben
			if( mSpeed > 0 )
			{
				lowerBorder = mAnimationFrameListener.lower_bound(
					0);
				upperBorder = mAnimationFrameListener.upper_bound(
					timeLeft);
			}
			// Wrapping nach unten
			else if( mSpeed < 0 )
			{
				lowerBorder = mAnimationFrameListener.lower_bound(
					mAnimState->getLength() - timeLeft );
				upperBorder = mAnimationFrameListener.upper_bound(
					mAnimState->getLength() );
			}

			// Ein letztes Mal durchlaufen ^^
			for (iter = lowerBorder; iter != upperBorder; iter++) 
			{
				animEve->setFrameNumber( iter->first );
				iter->second->animationFrameReached(animEve);
			}
		}
	}

	delete animEve;
}


void RlAnimation::setAnimationState( AnimationState* animState )
{
	if( animState == 0 )
		Throw( NullPointerException,"AnimationState darf nicht null sein" );

	mAnimState = animState;
	
	if( mTimesToPlay != 1 )
		mAnimState->setLoop( true );
	
	// Wenn die Zeit negativ ist, beginnen wir am Ende
	if( mSpeed < 0 )
		mAnimState->setTimePosition( mAnimState->getLength() );

	mAnimState->setEnabled( true );


}

}
