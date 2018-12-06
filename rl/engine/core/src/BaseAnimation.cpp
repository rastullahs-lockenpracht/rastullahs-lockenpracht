/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h"

#include "BaseAnimation.h"

#include "ActorManager.h"
#include "AnimationManager.h"
#include "CoreMessages.h"
#include "Exception.h"
#include "MessagePump.h"
#include "ScriptWrapper.h"

using namespace Ogre;


namespace rl {

BaseAnimation::BaseAnimation( Ogre::Real length, Ogre::Real speed, 
    unsigned int timesToPlay, bool paused ) :
    mLength(length),
    mPaused(paused),
    mTimesToPlay(timesToPlay),
    mSpeed(speed),
    mDelay(0.0f),
    mIgnoringGlobalSpeed(false),
    mTimePlayed(0.0f)
{
    setLoop( mTimesToPlay == 0 ); 
}

BaseAnimation::~BaseAnimation()
{
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
        MessagePump::getSingleton().sendMessage<MessageType_AnimationUnpaused>(this);
	}
	else if( !mPaused && isPaused )
	{
        MessagePump::getSingleton().sendMessage<MessageType_AnimationPaused>(this);
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
    return std::max(left, 0);
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

// Zeit hinzufgen // wird vom AnimationManager aufgerufen
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

		timePassed *= mSpeed;

		if (fabs(timePassed) - 0.0001 > 0)
        {
            Real elapsedTime = (mTimePlayed / mLength) * mLength;
            MessagePump::getSingleton().sendMessage<MessageType_AnimationFrameReached>(
                this, elapsedTime);
        }
        
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
			
                MessagePump::getSingleton().sendMessage<MessageType_AnimationFinished>(this);
			}
		}
	}
}

void BaseAnimation::start()
{
    setPaused(false);
}

void BaseAnimation::stop()
{
    setPaused(true);
}

void BaseAnimation::pause()
{
    setPaused(true);
}

}
