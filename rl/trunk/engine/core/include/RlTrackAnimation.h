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

#ifndef __RlTrackAnimation_H__
#define __RlTrackAnimation_H__

#include "CorePrerequisites.h"

#include "OgreAnimation.h"

#include "RlAnimation.h"
#include "AnimationManager.h"

namespace rl {

class _RlCoreExport RlTrackAnimation : public RlAnimation
{
public:
	RlTrackAnimation(const Ogre::String& name, Ogre::Node *node,
	    Ogre::Real length);
	virtual ~RlTrackAnimation();
	
    void addKeyFrame( Ogre::Real timePos );
    void setKeyFrameTranslation( Ogre::Real timePos,
        Ogre::Real xPos, Ogre::Real yPos, Ogre::Real zPos );
    void setKeyFrameRotation( Ogre::Real timePos,
        Ogre::Real xRotAxis, Ogre::Real yRotAxis, Ogre::Real zRotAxis,
        Ogre::Real angleUnits );
    void setKeyFrameScale( Ogre::Real timePos,
        Ogre::Real xScale, Ogre::Real yScale, Ogre::Real zScale);

	void setInterpolationMode( AnimationManager::InterpolationMode im );
	AnimationManager::InterpolationMode getInterpolationMode() const;

	void setRotationInterpolationMode(
	    AnimationManager::RotationInterpolationMode im );
	AnimationManager::RotationInterpolationMode getRotationInterpolationMode() const;

	void setUseShortestRotationPath ( bool useShortestPath );
	bool getUseShortestRotationPath () const;

	// TODO - kumulatives Animieren ein/aus
private:
	Ogre::KeyFrame* getKeyFrameAtTimePos(Ogre::Real timePos);	

	Ogre::Animation* mAnimation;
	Ogre::AnimationTrack* mAnimationTrack;
};

}
#endif
