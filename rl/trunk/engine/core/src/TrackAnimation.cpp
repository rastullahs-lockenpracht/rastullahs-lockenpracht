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

#include "TrackAnimation.h"

#include "Exception.h"

#include "CoreSubsystem.h"
#include "World.h"


namespace rl {

TrackAnimation::TrackAnimation( const Ogre::String& name, Ogre::Node *node, Ogre::Real length ) :	
	Animation( )
{
	Ogre::SceneManager* mgr =  CoreSubsystem::getSingleton().getWorld()->getSceneManager();

	node->setInitialState();
	mAnimation = mgr->createAnimation(name, length );
	mAnimationTrack = mAnimation->createTrack(0, node);
	this->setAnimationState( mgr->createAnimationState(name) );
}

TrackAnimation::~TrackAnimation()
{
	Ogre::SceneManager* mgr =  CoreSubsystem::getSingleton().getWorld()->getSceneManager();

	mAnimState->setEnabled(false);
	mAnimationTrack->getAssociatedNode()->resetToInitialState();
	mAnimation->destroyTrack( 0 );
	mgr->destroyAnimationState( mAnimation->getName() );
	mgr->destroyAnimation( mAnimation->getName() );
}

void TrackAnimation::addKeyFrame( Ogre::Real timePos )
{
	mAnimationTrack->createKeyFrame( timePos );
}

void TrackAnimation::setKeyFrameTranslation( Ogre::Real timePos, Ogre::Real xPos, Ogre::Real yPos, Ogre::Real zPos )
{
	Ogre::KeyFrame* frame = getKeyFrameAtTimePos( timePos );

	if( frame != 0 )
		frame->setTranslate(Ogre::Vector3(xPos,yPos,zPos));
}

void TrackAnimation::setKeyFrameRotationQuaternion( Ogre::Real timePos,
        Ogre::Real w, Ogre::Real x, Ogre::Real y, Ogre::Real z )
{
    Ogre::KeyFrame* frame = getKeyFrameAtTimePos( timePos );

	if( frame != 0 )
		frame->setRotation( Ogre::Quaternion( w, x, y, z ) );
}

void TrackAnimation::setKeyFrameRotation( Ogre::Real timePos, Ogre::Real xRotAxis, Ogre::Real yRotAxis, Ogre::Real zRotAxis, Ogre::Real angleUnits )
{
	Ogre::KeyFrame* frame = getKeyFrameAtTimePos( timePos );

	if( frame != 0 )
		frame->setRotation( Ogre::Quaternion(  Ogre::Radian( Ogre::Degree(angleUnits)), 
				Ogre::Vector3(xRotAxis,yRotAxis,zRotAxis) ) );
}

void TrackAnimation::setKeyFrameScale( Ogre::Real timePos, Ogre::Real xScale, Ogre::Real yScale, Ogre::Real zScale )
{
	Ogre::KeyFrame* frame = getKeyFrameAtTimePos( timePos );

	if( frame != 0 )
		frame->setScale(Ogre::Vector3(xScale,yScale,zScale));
}

void TrackAnimation::setInterpolationMode( AnimationManager::InterpolationMode im )
{
	mAnimation->setInterpolationMode( 
		Ogre::Animation::InterpolationMode( im ) );
}

AnimationManager::InterpolationMode TrackAnimation::getInterpolationMode() const
{
	return AnimationManager::InterpolationMode( 
		mAnimation->getInterpolationMode() );
}

void TrackAnimation::setRotationInterpolationMode( AnimationManager::RotationInterpolationMode im )
{
	mAnimation->setRotationInterpolationMode( 
		Ogre::Animation::RotationInterpolationMode( im ) );
}

AnimationManager::RotationInterpolationMode TrackAnimation::getRotationInterpolationMode() const
{
	return AnimationManager::RotationInterpolationMode( 
		mAnimation->getRotationInterpolationMode() );
}

void TrackAnimation::setUseShortestRotationPath ( bool useShortestPath )
{
	mAnimationTrack->setUseShortestRotationPath( useShortestPath );
}

bool TrackAnimation::getUseShortestRotationPath () const
{
	return mAnimationTrack->getUseShortestRotationPath();
}

Ogre::KeyFrame* TrackAnimation::getKeyFrameAtTimePos( Ogre::Real timePos )
{
	Ogre::KeyFrame *frame1;
	Ogre::KeyFrame *frame2;

	mAnimationTrack->getKeyFramesAtTime(timePos, &frame1, &frame2);

	return frame1;
}

// CoreSubsystem::getSingleton().log( StringConverter::toString( mAnimState->getTimePosition() ) );


}
