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

#include "RlTrackAnimation.h"

#include "Exception.h"

namespace rl {

RlTrackAnimation::RlTrackAnimation( const String& name, Node *node, Real length ) :	
	RlAnimation( new AnimationState( name, 0, length, 1.0, true ), 1.0, 0 ), 
	mAnimation( name, length)
{
	node->setInitialState();
	mAnimationTrack = mAnimation.createTrack(0, node);
	

	// TODO herausfinden wie der normale AnimationState funktioniert
	// klar ControllerFunction/ControllerValue/Controller... 
	// nur wo wird der Mist beim ControllerManager angemeldet???
}

RlTrackAnimation::~RlTrackAnimation()
{
	mAnimState->setEnabled(false);
	mAnimationTrack->getAssociatedNode()->resetToInitialState();
	mAnimation.destroyTrack( 0 );
	delete mAnimState;
}

void RlTrackAnimation::addKeyFrame( Real timePos )
{
	mAnimationTrack->createKeyFrame( timePos );
}

void RlTrackAnimation::setKeyFrameTranslation( Real timePos, Real xPos, Real yPos, Real zPos )
{
	KeyFrame* frame = getKeyFrameAtTimePos( timePos );

	if( frame != 0 )
		frame->setTranslate(Vector3(xPos,yPos,zPos));
}

void RlTrackAnimation::setKeyFrameRotation( Real timePos, Real xRotAxis, Real yRotAxis, Real zRotAxis, Real angleUnits )
{
	KeyFrame* frame = getKeyFrameAtTimePos( timePos );

	if( frame != 0 )
		frame->setRotation( Quaternion(Radian(angleUnits),Vector3(xRotAxis,yRotAxis,zRotAxis)));
}

void RlTrackAnimation::setKeyFrameScale( Real timePos, Real xScale, Real yScale, Real zScale )
{
	KeyFrame* frame = getKeyFrameAtTimePos( timePos );

	if( frame != 0 )
		frame->setScale(Vector3(xScale,yScale,zScale));
}

void RlTrackAnimation::setInterpolationMode( AnimationManager::InterpolationMode im )
{
	mAnimation.setInterpolationMode( 
		Ogre::Animation::InterpolationMode( im ) );
}

AnimationManager::InterpolationMode RlTrackAnimation::getInterpolationMode() const
{
	return AnimationManager::InterpolationMode( 
		mAnimation.getInterpolationMode() );
}

void RlTrackAnimation::setRotationInterpolationMode( AnimationManager::RotationInterpolationMode im )
{
	mAnimation.setRotationInterpolationMode( 
		Ogre::Animation::RotationInterpolationMode( im ) );
}

AnimationManager::RotationInterpolationMode RlTrackAnimation::getRotationInterpolationMode() const
{
	return AnimationManager::RotationInterpolationMode( 
		mAnimation.getRotationInterpolationMode() );
}

void RlTrackAnimation::setUseShortestRotationPath ( bool useShortestPath )
{
	mAnimationTrack->setUseShortestRotationPath( useShortestPath );
}

bool RlTrackAnimation::getUseShortestRotationPath () const
{
	return mAnimationTrack->getUseShortestRotationPath();
}

KeyFrame* RlTrackAnimation::getKeyFrameAtTimePos( Real timePos )
{
	KeyFrame *frame1, *frame2;

	mAnimationTrack->getKeyFramesAtTime(timePos, &frame1, &frame2);

	return frame1;
}

void RlTrackAnimation::applyTime( Real timePassed )
{
	mAnimState->addTime( timePassed );
	// HEUL, animationState ist bloss nen VariablenLager ;_;
	mAnimationTrack->apply( mAnimState->getTimePosition(),mAnimState->getWeight(),false);
}

}
