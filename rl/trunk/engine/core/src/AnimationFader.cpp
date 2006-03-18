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

#include "AnimationFader.h"

#include "BaseAnimation.h"
#include "CoreSubsystem.h"
#include "Exception.h"

namespace rl 
{

AnimationFader::AnimationFader() : 
	mIgnoringGlobalSpeed( false ),
	mDeleteOnFinish( true ),
	mSpeed(1.0)
{

}

AnimationFader::~AnimationFader() 
{
}

bool AnimationFader::isIgnoringGlobalSpeed() const
{
	return mIgnoringGlobalSpeed;
}

void AnimationFader::setIgnoringGlobalSpeed( bool isIgnoringGlobalSpeed )
{
	mIgnoringGlobalSpeed = isIgnoringGlobalSpeed;
}

Ogre::Real AnimationFader::getSpeed() const
{
	return mSpeed;
}

void AnimationFader::setSpeed( Ogre::Real speed )
{
	mSpeed = speed;
}

void AnimationFader::reverseAnimation()
{
	mSpeed = -mSpeed;
}

bool AnimationFader::isDeleteOnFinish() const
{
	return mDeleteOnFinish;
}

void AnimationFader::setDeleteOnFinish( bool deleteOnFinish )
{
	mDeleteOnFinish = deleteOnFinish;
}

}
