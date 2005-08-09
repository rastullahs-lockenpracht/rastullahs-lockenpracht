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

#include "Animation.h"

#include "Exception.h"
#include "AnimationManager.h"

namespace rl {

Animation::Animation( Ogre::AnimationState* animState, Ogre::Real speed, unsigned int timesToPlay ) :
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

Animation::Animation( ) :
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

Animation::~Animation()
{
	mAnimState->setEnabled(false);
}

bool Animation::isPaused() const
{
    return mPaused;
}

void Animation::setPaused( bool isPaused )
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


bool Animation::isIgnoringGlobalSpeed() const
{
	return mIgnoringGlobalSpeed;
}

void Animation::setIgnoringGlobalSpeed( bool isIgnoringGlobalSpeed )
{
	mIgnoringGlobalSpeed = isIgnoringGlobalSpeed;
}

// Regelbare Geschwindigkeit
Ogre::Real Animation::getSpeed() const
{
	return mSpeed;
}

void Animation::setSpeed( Ogre::Real speed )
{
	// Eventuell auf Anfang/Ende setzen
	if( speed < 0 && mAnimState->getTimePosition() == 0 )
		mAnimState->setTimePosition( mAnimState->getLength() );
	else if( speed > 0 && mAnimState->getTimePosition() == mAnimState->getLength() )
		mAnimState->setTimePosition( 0 );

	mSpeed = speed;
}

void Animation::reverseAnimation()
{
	mSpeed = -mSpeed;
}

// Regelbare Wiederholungsanzahl
void Animation::setTimesToPlay(unsigned int timesToPlay)
{
	mTimesToPlay = timesToPlay;
}

unsigned int Animation::getTimesToPlay() const
{
	return mTimesToPlay;
}

void Animation::resetTimesPlayed()
{
	// Zurückspulen
	if( mSpeed < 0 )
		mAnimState->setTimePosition( mAnimState->getLength() );
	else if( mSpeed > 0 )
		mAnimState->setTimePosition( 0 );

	mTimePlayed = 0;
	
	if( mTimesToPlay != 1 )
		mAnimState->setLoop( true );

	setPaused( false );
}

Ogre::Real Animation::getTimePlayed() const
{
	return mTimePlayed;
}

unsigned int Animation::getTimesPlayed() const
{
	return floor(mTimePlayed/mAnimState->getLength());
}

unsigned int Animation::getTimesToPlayLeft() const
{
	// Nicht unsigned :)
	int left = mTimesToPlay - getTimesPlayed();
	return max( left ,0);
}

// Gewicht (Einfluss) der Animation
Ogre::Real Animation::getWeight(void) const
{
	return mAnimState->getWeight();
}

void Animation::setWeight(Ogre::Real weight)
{
	mAnimState->setWeight(weight);
}

void Animation::addAnimationListener(AnimationListener *listener)
{
	mAnimationCaster.addEventListener(listener);
}

void Animation::removeAnimationListener(AnimationListener *listener)
{
	mAnimationCaster.removeEventListener(listener);
}

void Animation::addAnimationFrameListener( 
	AnimationFrameListener *listener, Ogre::Real frameNumber)
{
	mAnimationFrameListener.insert( 
		std::pair<Ogre::Real,AnimationFrameListener*>(frameNumber,listener) );
}

void Animation::removeAnimationFrameListener( AnimationFrameListener *listener )
{
	///@todo Alle zugehörigen löschen
}

void Animation::removeAnimationFrameListener( 
	AnimationFrameListener *listener, Ogre::Real frameNumber)
{
	///@todo Zugehöriges löschen
}

// Zeit hinzufügen // wird vom AnimationManager aufgerufen
void Animation::addTime( Ogre::Real timePassed )
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
	}
}

/**
	Benachrichtigt die AnimationFrameListener

	Erfolgt in drei Schritten 
	 * Aktueller Durchlauf
	 * Wenn Überlauf, dann wird die Anzahl weiterer Durchläufe bestimmt
	 * Für den Rest im letzten Durchlauf wird erneut geprüft
*/
void Animation::checkAnimationFrameListeners( Ogre::Real timePassed )
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
		iter->second->eventRaised(animEve);
	}

	// Einmal abspielen abziehen
	timePassed -= mAnimState->getLength();
	// Ums Wrapping kümmern - wenn Looping - und AbspielZeit
	if( timePassed > 0 && mAnimState->getLoop() && 
		// Falls begrenzte Wiederholungen, müssen mindestens 2(1+die oben abgearbeitete) fehlen
		( ( mTimesToPlay > 0 && getTimesToPlayLeft() > 1 ) || ( mTimesToPlay == 0 ) )
	   )
	{
		// Wie oft passt die Länge in die gesamte fortgeschrittene Zeit		
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
				iter->second->eventRaised(animEve);
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
				iter->second->eventRaised(animEve);
			}
		}
	}

	delete animEve;
}


void Animation::setAnimationState( Ogre::AnimationState* animState )
{
	if( animState == 0 )
		Throw( NullPointerException,"Ogre::AnimationState darf nicht null sein" );

	mAnimState = animState;
	
	if( mTimesToPlay != 1 )
		mAnimState->setLoop( true );
	
	// Wenn die Zeit negativ ist, beginnen wir am Ende
	if( mSpeed < 0 )
		mAnimState->setTimePosition( mAnimState->getLength() );

	mAnimState->setEnabled( true );


}

}
