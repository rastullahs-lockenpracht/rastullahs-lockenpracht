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

#include "Actor.h"
#include "RlAnimation.h"
#include "RlTrackAnimation.h"

template<> rl::AnimationManager* Singleton<rl::AnimationManager>::ms_Singleton = 0;

namespace rl 
{

AnimationManager::AnimationManager( ) : mAnimationMap()
{
	mGlobalAnimationSpeed = 1.0;
}
	
AnimationManager::~AnimationManager( )
{
	// TODO Alle Animationen löschen...
}

void AnimationManager::setGlobalAnimationSpeed( Real speed )
{
	mGlobalAnimationSpeed = speed;
}

Real AnimationManager::getGlobalAnimationSpeed( ) const
{
	return mGlobalAnimationSpeed;
}

void AnimationManager::setDefaultInterpolationMode( AnimationManager::InterpolationMode im )
{
	Animation::setDefaultInterpolationMode( 
		Ogre::Animation::InterpolationMode( im ) );
}

AnimationManager::InterpolationMode AnimationManager::getDefaultInterpolationMode() const
{
	return AnimationManager::InterpolationMode( 
		Animation::getDefaultInterpolationMode() );
}

void AnimationManager::setDefaultRotationInterpolationMode(
    AnimationManager::RotationInterpolationMode rim )
{
	Animation::setDefaultRotationInterpolationMode( 
		Ogre::Animation::RotationInterpolationMode( rim ) );
}

AnimationManager::RotationInterpolationMode AnimationManager::getDefaultRotationInterpolationMode() const
{
	return AnimationManager::RotationInterpolationMode( 
		Animation::getDefaultRotationInterpolationMode() );
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

RlTrackAnimation* AnimationManager::createTrackAnimation(Actor* actor, const String& name, Real length )
{
	RlTrackAnimation* trackAnim = new RlTrackAnimation(name,actor->_getSceneNode(),length);
	mAnimationMap.insert(std::pair<AnimationState*,RlAnimation*>(trackAnim->getAnimationState(),trackAnim));

	return trackAnim;
}

RlAnimation* AnimationManager::getAnimation(AnimationState* animState) const
{
	std::map<AnimationState*,RlAnimation*>::const_iterator iter = mAnimationMap.find(animState);

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

void AnimationManager::removeAnimation(RlAnimation* anim)
{
	removeAnimation(anim->getAnimationState());
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
