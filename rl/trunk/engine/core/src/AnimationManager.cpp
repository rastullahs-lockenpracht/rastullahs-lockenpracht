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

AnimationManager::AnimationManager( ) : mAnimationMap()
{
	mGlobalAnimationSpeed = 1.0;
}
	
AnimationManager::~AnimationManager( )
{
	
}

void AnimationManager::setGlobalAnimationSpeed( Real speed )
{
	mGlobalAnimationSpeed = speed;
}

Real AnimationManager::getGlobalAnimationSpeed( ) const
{
	return mGlobalAnimationSpeed;
}

RlAnimation* AnimationManager::addAnimation(AnimationState* animState, Real speed, unsigned int timesToPlay)
{
	std::map<AnimationState*,RlAnimation*>::iterator iter = mAnimationMap.find(animState);

	RlAnimation* anim;

	// Noch nicht vorhanden
	if( iter == mAnimationMap.end() )
	{
		anim = new RlAnimation(animState,speed,timesToPlay);
		mAnimationMap.insert(std::pair<AnimationState*,RlAnimation*>(animState,anim));
		animState->setEnabled(true);
	}
	// Bereits vorhanden
	else
	{
		anim = iter->second;
		anim->resetTimesPlayed();
		anim->setTimesToPlay(timesToPlay);
		anim->setSpeed(speed);
		anim->setPaused(false);
	}

	return anim;
}

RlAnimation* AnimationManager::getAnimation(AnimationState* animState)
{
	std::map<AnimationState*,RlAnimation*>::iterator iter = mAnimationMap.find(animState);

	if( iter == mAnimationMap.end() )
		return 0;
	else
		return iter->second;
}

void AnimationManager::removeAnimation(AnimationState* animState)
{
	std::map<AnimationState*,RlAnimation*>::iterator iter = mAnimationMap.find(animState);

	if( iter != mAnimationMap.end() )
	{
		RlAnimation* anim = iter->second;

		mAnimationMap.erase(iter);
		delete anim;
	}
}


void AnimationManager::run(Real timePassed)
{
	for (std::map<AnimationState*,RlAnimation*>::iterator it = mAnimationMap.begin(); 
			it != mAnimationMap.end(); it++)
	{
		it->second->addTime(timePassed*mGlobalAnimationSpeed);
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
