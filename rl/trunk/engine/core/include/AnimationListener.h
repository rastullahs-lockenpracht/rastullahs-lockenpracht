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

#ifndef __AnimationListener_H__
#define __AnimationListener_H__

#include "CorePrerequisites.h"

#include "EventCaster.h"
#include "EventSource.h"
#include "EventObject.h"

namespace rl {

class Animation;

class _RlCoreExport AnimationEvent : public virtual EventObject
{
	public:
		static const unsigned int ANIMATION_PAUSED = 360;
		static const unsigned int ANIMATION_UNPAUSED = 361;
		static const unsigned int ANIMATION_FINISHED = 362;

		AnimationEvent( Animation* anim,  const unsigned int reason );
		virtual ~AnimationEvent() {};

		Animation* getAnimation() const;
};


class _RlCoreExport AnimationListener : public virtual EventListener<AnimationEvent>
{
	public:
		virtual ~AnimationListener();

		virtual void animationFinished(AnimationEvent *anEvent) const = 0;
		virtual void animationPaused(AnimationEvent *anEvent) const = 0;
 		virtual void animationUnpaused(AnimationEvent *anEvent) const = 0;

		virtual bool eventRaised(AnimationEvent *anEvent);
};

class _RlCoreExport AnimationFrameEvent : public virtual EventObject
{
public:
	static const unsigned int ANIMATION_FRAMEREACHED = 363;

	AnimationFrameEvent( Animation* anim,  const unsigned int reason, const Ogre::Real& frameNumber );
	virtual ~AnimationFrameEvent() {};

	Animation* getAnimation() const;
	Ogre::Real getFrameNumber() const;
	void setFrameNumber(const Ogre::Real& frameNumber);
private:
	Ogre::Real mFrameNumber;
};

class _RlCoreExport AnimationFrameListener
{
	public:
		virtual ~AnimationFrameListener() {};
		virtual void animationFrameReached( AnimationFrameEvent *anEvent ) const = 0;

        virtual bool eventRaised(AnimationFrameEvent *anEvent);
};

}
#endif
