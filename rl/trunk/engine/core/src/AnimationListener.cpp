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

#include "AnimationListener.h"

#include "Animation.h"

namespace rl {

AnimationEvent::AnimationEvent( Animation* anim, const unsigned int reason ) : 
	EventObject(anim,reason)
{}

Animation* AnimationEvent::getAnimation() const
{
	EventSource* eve =  this->getSource();
	return dynamic_cast<Animation*>( eve );
}


bool AnimationListener::eventRaised( AnimationEvent* anEvent ) const
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





AnimationFrameEvent::AnimationFrameEvent( Animation* anim,  const unsigned int reason, const Ogre::Real& frameNumber ) : 
EventObject(anim,reason)
{
	mFrameNumber = frameNumber;
}

Animation* AnimationFrameEvent::getAnimation() const
{
	EventSource* eve =  this->getSource();
	return dynamic_cast<Animation*>( eve );
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
