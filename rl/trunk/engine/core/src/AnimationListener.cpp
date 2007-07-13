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
#include "stdinc.h" //precompiled header

#include "AnimationListener.h"

#include "BaseAnimation.h"
#include "Exception.h"

namespace rl {

AnimationEvent::AnimationEvent( BaseAnimation* anim, const unsigned int reason ) :
	EventObject(anim,reason)
{}

BaseAnimation* AnimationEvent::getAnimation() const
{
	EventSource* eve =  this->getSource();
	return dynamic_cast<BaseAnimation*>( eve );
}

AnimationListener::~AnimationListener()
{

}

bool AnimationListener::eventRaised( AnimationEvent* anEvent )
{
    switch( anEvent->getReason() )
    {
    case AnimationEvent::ANIMATION_FINISHED:
        this->animationFinished( anEvent );
        break;
    case AnimationEvent::ANIMATION_UNPAUSED:
        this->animationUnpaused( anEvent );
        break;
    case AnimationEvent::ANIMATION_PAUSED:
        this->animationPaused( anEvent );
        break;
    }

	// consumed or not ;)
	return false;
}

 bool AnimationFrameListener::eventRaised(AnimationFrameEvent *anEvent)
{
    this->animationFrameReached( anEvent );

    // consumed or not ;)
    return false;
}


AnimationFrameEvent::AnimationFrameEvent( BaseAnimation* anim,  const unsigned int reason, const Ogre::Real& frameNumber ) :
EventObject(anim,reason)
{
	mFrameNumber = frameNumber;
}

BaseAnimation* AnimationFrameEvent::getAnimation() const
{
	EventSource* eve =  this->getSource();
	return dynamic_cast<BaseAnimation*>( eve );
}

Ogre::Real AnimationFrameEvent::getFrameNumber() const
{
	return mFrameNumber;
}

void AnimationFrameEvent::setFrameNumber(const Ogre::Real& frameNumber)
{
	mFrameNumber = frameNumber;
}


}
