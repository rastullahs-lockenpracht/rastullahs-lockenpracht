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

#include "Exception.h"
#include "AnimationManager.h"
#include "ActorManager.h"
#include "ScriptWrapper.h"

#include "BaseAnimation.h"

using namespace Ogre;


namespace rl {

BaseAnimation::BaseAnimation( Ogre::Real length, Ogre::Real speed, 
    unsigned int timesToPlay, bool paused ) :
    PlaylistObject(),
    mLength(length),
	mAnimationFrameListener(),
	mAnimationCaster(),
    mPaused(paused),
    mTimesToPlay( timesToPlay ),
    mSpeed( speed ),
    mDelay( 0.0f ),
    mIgnoringGlobalSpeed(false),
    mTimePlayed( 0.0f )
{
    setLoop( mTimesToPlay == 0 ); 
}

BaseAnimation::~BaseAnimation()
{
    removeAllListeners();
}

Ogre::Real BaseAnimation::getLength() const
{
    return mLength;
}

bool BaseAnimation::isPaused() const
{
    return mPaused;
}

void BaseAnimation::setPaused( bool isPaused )
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


bool BaseAnimation::isIgnoringGlobalSpeed() const
{
	return mIgnoringGlobalSpeed;
}

void BaseAnimation::setIgnoringGlobalSpeed( bool isIgnoringGlobalSpeed )
{
	mIgnoringGlobalSpeed = isIgnoringGlobalSpeed;
}

// Regelbare Geschwindigkeit
Ogre::Real BaseAnimation::getSpeed() const
{
	return mSpeed;
}

void BaseAnimation::setSpeed( Ogre::Real speed )
{
	mSpeed = speed;
}

void BaseAnimation::reverseAnimation()
{
	mSpeed = -mSpeed;
}

// Regelbare Wiederholungsanzahl
void BaseAnimation::setTimesToPlay(unsigned int timesToPlay)
{
	mTimesToPlay = timesToPlay;
    setLoop( mTimesToPlay == 0 ); 
}

unsigned int BaseAnimation::getTimesToPlay() const
{
	return mTimesToPlay;
}

void BaseAnimation::resetTimesPlayed()
{
	mTimePlayed = 0;
    setLoop( mTimesToPlay != 1 );
	setPaused( false );
}

Ogre::Real BaseAnimation::getTimePlayed() const
{
	return mTimePlayed;
}

unsigned int BaseAnimation::getTimesPlayed() const
{
	return floor(mTimePlayed/mLength);
}

unsigned int BaseAnimation::getTimesToPlayLeft() const
{
	// Nicht unsigned :)
	int left = mTimesToPlay - getTimesPlayed();
	return max( left ,0);
}

void BaseAnimation::setDelay( Ogre::Real delay )
{
	mDelay = delay;
}

Ogre::Real BaseAnimation::getDelay() const
{
	return mDelay;
}


void BaseAnimation::setLoop( bool loop )
{
}

bool BaseAnimation::isLoop() const
{
    return mTimesToPlay!=1;
}

void BaseAnimation::addAnimationListener(AnimationListener *listener)
{
    if( !mAnimationCaster.containsListener(listener) )
    {    
	    mAnimationCaster.addEventListener(listener);
        ScriptWrapper::getSingleton().owned( listener );
    }
}

void BaseAnimation::removeAnimationListener(AnimationListener *listener)
{
    if( mAnimationCaster.containsListener( listener ) ) 
    {
	    mAnimationCaster.removeEventListener(listener);
        ScriptWrapper::getSingleton().disowned( listener );
    }
}

// @todo - Existenz überprüfen
void BaseAnimation::addAnimationFrameListener( 
	AnimationFrameListener *listener, Ogre::Real frameNumber)
{
	mAnimationFrameListener.insert( 
		std::pair<Ogre::Real,AnimationFrameListener*>(frameNumber,listener) );
    ScriptWrapper::getSingleton().owned( listener );
}

// @todo - Existenz überprüfen
void BaseAnimation::removeAnimationFrameListener( AnimationFrameListener *listener )
{
    AnimationFrameListenerMap::iterator iter = mAnimationFrameListener.begin();

    for (iter; iter != mAnimationFrameListener.end(); ) 
    {
        AnimationFrameListener* afl = iter->second;
        
        if( afl == listener )
        {
            mAnimationFrameListener.erase( iter++ );  
            ScriptWrapper::getSingleton().disowned( listener );
        } else {
            ++iter;
        }
    }
}

void BaseAnimation::removeAnimationFrameListener( 
	AnimationFrameListener *listener, Ogre::Real frameNumber)
{
    AnimationFrameListenerMap::iterator iter = mAnimationFrameListener.begin();

    for (iter; iter != mAnimationFrameListener.end(); ) 
    {
        Real time = iter->first;
        AnimationFrameListener* afl = iter->second;

        if( afl == listener && time == frameNumber )
        {
            mAnimationFrameListener.erase( iter++ );  
            ScriptWrapper::getSingleton().disowned( listener );
        } else {
            ++iter;
        }
    }
}

void BaseAnimation::removeAllListeners()
{
    // Alle AnimationFrameListener
    AnimationFrameListenerMap::iterator iter = mAnimationFrameListener.begin();

    for (iter; iter != mAnimationFrameListener.end(); ) 
    {
        AnimationFrameListener* afl = iter->second; 
        ScriptWrapper::getSingleton().disowned( afl );
        iter++;
    }
    mAnimationFrameListener.clear();
    
    // Alle AnimationListener
    EventCaster<AnimationEvent>::EventSet evSet 
        = mAnimationCaster.getEventSet();
    EventCaster<AnimationEvent>::EventSet::iterator citer 
        = evSet.begin();
    for (citer; citer != evSet.end(); ) 
    {
        EventListener<AnimationEvent>* ev = *citer; 
        AnimationListener* al = dynamic_cast<AnimationListener*>( ev );
        ScriptWrapper::getSingleton().disowned( al );
        citer++;
    }
    mAnimationCaster.removeEventListeners();
}

// Zeit hinzufügen // wird vom AnimationManager aufgerufen
void BaseAnimation::addTime( Ogre::Real timePassed )
{
	if( !mPaused )
	{
		if( mDelay > 0.0 )
		{
			mDelay -= timePassed;
			
			if( mDelay > 0.0 )
				return;
			else
			{
				timePassed = fabs(mDelay);
				mDelay = 0.0;
			}
		}

		timePassed = timePassed * mSpeed;

		if( !mAnimationFrameListener.empty() && timePassed != 0 )
			checkAnimationFrameListeners( timePassed );
        
        doAddTime(timePassed);

		// Begrenzte Abspielanzahl
		if( mTimesToPlay > 0 )
		{
			mTimePlayed += fabs(timePassed);

			if( getTimesToPlayLeft() == 1 )
			{
				setLoop(false);
			}
			else if( getTimesToPlayLeft() == 0 ) 
			{
				setLoop(false);
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
void BaseAnimation::checkAnimationFrameListeners( Ogre::Real timePassed )
{
	if( mAnimationFrameListener.empty() ) 
		return;

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

    Ogre::Real elapsedTime = (mTimePlayed/mLength)*mLength;

	// Vorwärts laufen
	if( mSpeed > 0 )		
	{
        lower = max(elapsedTime, 0.0f);
		upper = min(elapsedTime, mLength );
	}
	// Das ganze rückwärts
	else
	{
		// FIXME für die erste Runde beim Rückwärtsspielen, beginnt leider bei 0, nicht Length
		Ogre::Real timePos = elapsedTime;
		if( timePos == 0.0f )
			timePos = mLength;

		lower = max(timePos-timePassed, 0.0f);
		upper = min(timePos, mLength );
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
	timePassed -= mLength;
	// Ums Wrapping kümmern - wenn Looping - und AbspielZeit
	if( timePassed > 0 && isLoop() && 
		// Falls begrenzte Wiederholungen, müssen mindestens 2(1+die oben abgearbeitete) fehlen
		( ( mTimesToPlay > 0 && getTimesToPlayLeft() > 1 ) || ( mTimesToPlay == 0 ) )
	   )
	{
		// Wie oft passt die Länge in die gesamte fortgeschrittene Zeit		
		unsigned int timesSkipped = floor( timePassed/mLength );
		Ogre::Real timeLeft = timePassed - timesSkipped*mLength;

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
					mLength - timeLeft );
				upperBorder = mAnimationFrameListener.upper_bound(
					mLength );
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

void BaseAnimation::start()
{
	PlaylistObject::start();
    setPaused(false);
}

void BaseAnimation::stop()
{
	PlaylistObject::stop();
    setPaused(true);
}

void BaseAnimation::pause()
{
	PlaylistObject::pause();
    setPaused(true);
}

}
