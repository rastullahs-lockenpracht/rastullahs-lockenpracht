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
#include "MeshAnimation.h"
#include "TrackAnimation.h"
#include "ScriptWrapper.h"


template<> rl::AnimationManager* Ogre::Singleton<rl::AnimationManager>::ms_Singleton = 0;

namespace rl 
{

AnimationManager::AnimationManager( ) : 
    mMeshAnimationMap(),
    mGlobalAnimationSpeed( 1.0f )
{
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

MeshAnimation* AnimationManager::addMeshAnimation(Ogre::AnimationState* animState, 
    MeshObject* mesh, Ogre::Real speed, unsigned int timesToPlay, bool paused )
{
	MeshAnimMap::iterator iter = 
		mMeshAnimationMap.find(animState);

	MeshAnimation* anim = 0;

	// Noch nicht vorhanden
	if( iter == mMeshAnimationMap.end() )
	{
		anim = new MeshAnimation(animState,mesh,speed,timesToPlay,paused);
		mMeshAnimationMap.insert(std::pair<Ogre::AnimationState*,MeshAnimation*>(animState,anim));
		animState->setEnabled(true);
	}
	// Bereits vorhanden
	else
	{
		anim = dynamic_cast<MeshAnimation*>(iter->second);
		anim->resetTimesPlayed();
		anim->setTimesToPlay(timesToPlay);
		anim->setSpeed(speed);
		anim->setPaused(paused);
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
	mMeshAnimationMap.insert(std::pair<Ogre::AnimationState*,BaseAnimation*>( 
		trackAnim->getAnimationState(),trackAnim));

	return trackAnim;
}

BaseAnimation* AnimationManager::getAnimation(Ogre::AnimationState* animState) const
{
	MeshAnimMap::const_iterator iter = 
		mMeshAnimationMap.find(animState);

	if( iter == mMeshAnimationMap.end() )
		return NULL;
	else
		return iter->second;
}

void AnimationManager::removeTrackAnimation( Actor* act, const Ogre::String& name ) 
{
    MeshAnimMap::iterator it;
    for( it = mMeshAnimationMap.begin(); it != mMeshAnimationMap.end();) 
    {
        TrackAnimation* anim = dynamic_cast<TrackAnimation*>( it->second );
		
		if ( anim != 0 && 
			 anim->getActor() == act && 
			 anim->getName() == name ) 
		{
			AnimationManager::stopAnimation(anim);
			ScriptWrapper::getSingleton().deleted( anim );
			delete anim;
			mMeshAnimationMap.erase(it++); // ++i geht nicht
		} 
		else 
		{
		   ++it; // Oder i++
		} 
    }
}

void AnimationManager::removeAllTrackAnimations( Actor* act ) 
{
    MeshAnimMap::iterator it;
    for( it = mMeshAnimationMap.begin(); it != mMeshAnimationMap.end();) 
    {
        TrackAnimation* anim = dynamic_cast<TrackAnimation*>( it->second );
		
		if ( anim != 0 && anim->getActor() == act ) 
		{
			AnimationManager::stopAnimation(anim);
			ScriptWrapper::getSingleton().deleted( anim );
			delete anim;
			mMeshAnimationMap.erase(it++); // ++i geht nicht
		} 
		else 
		{
		   ++it; // Oder i++
		} 
    }
}

void AnimationManager::removeAllAnimations() 
{
    MeshAnimMap::iterator it;
    for( it = mMeshAnimationMap.begin(); it != mMeshAnimationMap.end();) 
    {
        BaseAnimation* anim = it->second;
        AnimationManager::stopAnimation(anim);
        ScriptWrapper::getSingleton().deleted( anim );
        delete anim;
		it++;
    }
    mMeshAnimationMap.clear();
}

void AnimationManager::removeAnimation(Ogre::AnimationState* animState)
{
	MeshAnimMap::iterator iter = 
		mMeshAnimationMap.find(animState);

	if( iter != mMeshAnimationMap.end() )
	{
		BaseAnimation* anim = iter->second;
        AnimationManager::stopAnimation(anim);
		mMeshAnimationMap.erase(iter);
        ScriptWrapper::getSingleton().deleted( anim );
		delete anim;
	}
}

void AnimationManager::stopAnimation( BaseAnimation* anim )
{
    anim->resetTimesPlayed();
    anim->setTimesToPlay(1);
    anim->setSpeed(1.0);
    anim->setPaused(true);    
}

// @todo Check ob das das selbe MeshObject ist
MeshAnimation* AnimationManager::replaceAnimation(MeshAnimation* oldAnim,  
    Ogre::AnimationState* newAnimState, Ogre::Real speed, unsigned int timesToPlay )
{
    removeAnimation( oldAnim );
    return addMeshAnimation( newAnimState, oldAnim->getMeshObject(), speed, timesToPlay );
}

void AnimationManager::removeAnimation(MeshAnimation* anim)
{
	removeAnimation(anim->getAnimationState());
}

void AnimationManager::removeAnimation(TrackAnimation* anim)
{
	removeAnimation(anim->getAnimationState());
}

void AnimationManager::run(Ogre::Real timePassed)
{
	for (MeshAnimMap::iterator it = mMeshAnimationMap.begin(); 
			it != mMeshAnimationMap.end(); it++)
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
