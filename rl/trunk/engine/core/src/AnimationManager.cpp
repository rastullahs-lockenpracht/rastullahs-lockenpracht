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

#include "AnimationManager.h"

#include "Exception.h"
#include "Actor.h"
#include "Animation.h"
#include "TrackAnimation.h"
#include "ScriptWrapper.h"


template<> rl::AnimationManager* Ogre::Singleton<rl::AnimationManager>::ms_Singleton = 0;

namespace rl 
{

AnimationManager::AnimationManager( ) : mAnimationMap()
{
	mGlobalAnimationSpeed = 1.0;
}
	
AnimationManager::~AnimationManager( )
{
	removeAllAnimations();
}

void AnimationManager::setGlobalAnimationSpeed( Ogre::Real speed )
{
	mGlobalAnimationSpeed = speed;
}

Ogre::Real AnimationManager::getGlobalAnimationSpeed( ) const
{
	return mGlobalAnimationSpeed;
}

void AnimationManager::setDefaultInterpolationMode( AnimationManager::InterpolationMode im )
{
	Ogre::Animation::setDefaultInterpolationMode( 
		Ogre::Animation::InterpolationMode( im ) );
}

AnimationManager::InterpolationMode 
	AnimationManager::getDefaultInterpolationMode() const
{
	return AnimationManager::InterpolationMode( 
		Ogre::Animation::getDefaultInterpolationMode() );
}

void AnimationManager::setDefaultRotationInterpolationMode(
    AnimationManager::RotationInterpolationMode rim )
{
	Ogre::Animation::setDefaultRotationInterpolationMode( 
		Ogre::Animation::RotationInterpolationMode( rim ) );
}

AnimationManager::RotationInterpolationMode 
	AnimationManager::getDefaultRotationInterpolationMode() const
{
	return AnimationManager::RotationInterpolationMode( 
		Ogre::Animation::getDefaultRotationInterpolationMode() );
}

Animation* AnimationManager::addAnimation(Ogre::AnimationState* animState, MeshObject* mesh,
										  Ogre::Real speed, unsigned int timesToPlay)
{
	AnimMap::iterator iter = 
		mAnimationMap.find(animState);

	Animation* anim = 0;

	// Noch nicht vorhanden
	if( iter == mAnimationMap.end() )
	{
		anim = new Animation(animState,mesh,speed,timesToPlay);
		mAnimationMap.insert(std::pair<Ogre::AnimationState*,Animation*>(animState,anim));
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

TrackAnimation* AnimationManager::createTrackAnimation(Actor* actor, 
	const Ogre::String& name, Ogre::Real length )
{
	if( actor == 0 )
		Throw( NullPointerException, "Actor darf nicht null sein" );

	///@todo Namen abfangen
	TrackAnimation* trackAnim = new TrackAnimation(name,actor,length);
	mAnimationMap.insert(std::pair<Ogre::AnimationState*,Animation*>( 
		trackAnim->getAnimationState(),trackAnim));

	return trackAnim;
}

Animation* AnimationManager::getAnimation(Ogre::AnimationState* animState) const
{
	AnimMap::const_iterator iter = 
		mAnimationMap.find(animState);

	if( iter == mAnimationMap.end() )
		return 0;
	else
		return iter->second;
}

void AnimationManager::removeTrackAnimation( Actor* act, const Ogre::String& name ) 
{
    AnimMap::iterator it;
    for( it = mAnimationMap.begin(); it != mAnimationMap.end();) 
    {
        TrackAnimation* anim = dynamic_cast<TrackAnimation*>( it->second );
		
		if ( anim != 0 && 
			 anim->getActor() == act && 
			 anim->getName() == name ) 
		{
			AnimationManager::stopAnimation(anim);
			ScriptWrapper::getSingleton().deleted( anim );
			delete anim;
			mAnimationMap.erase(it++); // ++i geht nicht
		} 
		else 
		{
		   ++it; // Oder i++
		} 
    }
}

void AnimationManager::removeAllTrackAnimations( Actor* act ) 
{
    AnimMap::iterator it;
    for( it = mAnimationMap.begin(); it != mAnimationMap.end();) 
    {
        TrackAnimation* anim = dynamic_cast<TrackAnimation*>( it->second );
		
		if ( anim != 0 && anim->getActor() == act ) 
		{
			AnimationManager::stopAnimation(anim);
			ScriptWrapper::getSingleton().deleted( anim );
			delete anim;
			mAnimationMap.erase(it++); // ++i geht nicht
		} 
		else 
		{
		   ++it; // Oder i++
		} 
    }
}

void AnimationManager::removeAllAnimations() 
{
    AnimMap::iterator it;
    for( it = mAnimationMap.begin(); it != mAnimationMap.end();) 
    {
        Animation* anim = it->second;
        AnimationManager::stopAnimation(anim);
        ScriptWrapper::getSingleton().deleted( anim );
        delete anim;
		it++;
    }
    mAnimationMap.clear();
}

void AnimationManager::removeAnimation(Ogre::AnimationState* animState)
{
	AnimMap::iterator iter = 
		mAnimationMap.find(animState);

	if( iter != mAnimationMap.end() )
	{
		Animation* anim = iter->second;
        AnimationManager::stopAnimation(anim);
		mAnimationMap.erase(iter);
        ScriptWrapper::getSingleton().deleted( anim );
		delete anim;
	}
}

void AnimationManager::stopAnimation( Animation* anim )
{
    anim->resetTimesPlayed();
    anim->setTimesToPlay(1);
    anim->setSpeed(1.0);
    anim->setPaused(true);    
}

// @todo Check ob das das selbe MeshObject ist
Animation* AnimationManager::replaceAnimation(Animation* oldAnim,  
    Ogre::AnimationState* newAnimState, Ogre::Real speed, unsigned int timesToPlay )
{
    removeAnimation( oldAnim );
    return addAnimation( newAnimState, oldAnim->getMeshObject(), speed, timesToPlay );
}



void AnimationManager::removeAnimation(Animation* anim)
{
	removeAnimation(anim->getAnimationState());
}


void AnimationManager::run(Ogre::Real timePassed)
{
	for (AnimMap::iterator it = mAnimationMap.begin(); 
			it != mAnimationMap.end(); it++)
	{
		it->second->addTime(timePassed*mGlobalAnimationSpeed);
	}
}

AnimationManager& AnimationManager::getSingleton(void)
{
	return Ogre::Singleton<AnimationManager>::getSingleton();
}

AnimationManager* AnimationManager::getSingletonPtr(void)
{
	return Ogre::Singleton<AnimationManager>::getSingletonPtr();
}


}
