/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#include "AnimationManager.h"


template<> rl::AnimationManager* Singleton<rl::AnimationManager>::ms_Singleton = 0;

namespace rl 
{

AnimationManager::AnimationManager( ) : mAnimationList()
{

}
	
AnimationManager::~AnimationManager( )
{

}

void AnimationManager::addAnimation(AnimationState* newAnimState)
{
	bool found = false;

	for (std::list<AnimationState*>::iterator it = mAnimationList.begin(); 
			it != mAnimationList.end(); it++)
	{
		if (*it == newAnimState)
		{
			(*it)->setEnabled(true);
			found = true;
		}
	}

	if (!found)
	{
		mAnimationList.push_back(newAnimState);
		newAnimState->setEnabled(true);
	}
}

void AnimationManager::removeAnimation(AnimationState* oldAnimState)
{
	for (std::list<AnimationState*>::iterator it = mAnimationList.begin(); 
			it != mAnimationList.end(); it++)
	{
		if (*it == oldAnimState)
		{
			(*it)->setEnabled(false);
			mAnimationList.remove(*it);
			return;
		}
	}
}


void AnimationManager::run(Real timePassed)
{
	for (std::list<AnimationState*>::iterator it = mAnimationList.begin(); 
			it != mAnimationList.end(); it++)
	{
		if ((*it)->getEnabled())
			(*it)->addTime(timePassed);
	}
}

AnimationManager& AnimationManager::getSingleton(void)
{
	return Singleton<AnimationManager>::getSingleton();
}

AnimationManager* AnimationManager::getSingletonPtr(void)
{
	return Singleton<AnimationManager>::getSingletonPtr();
}


}
