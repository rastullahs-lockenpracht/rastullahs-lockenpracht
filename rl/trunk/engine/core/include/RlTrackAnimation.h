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
	RlTrackAnimation( const String& name, Node *node, Real length );
	virtual ~RlTrackAnimation();
	
    void addKeyFrame( Real timePos );
    void setKeyFrameTranslation( Real timePos, Real xPos, Real yPos, Real zPos );
    void setKeyFrameRotation( Real timePos, Real xRotAxis, Real yRotAxis, Real zRotAxis, Real angleUnits );
    void setKeyFrameScale( Real timePos, Real xScale, Real yScale, Real zScale );

	void setInterpolationMode( AnimationManager::InterpolationMode im );
	AnimationManager::InterpolationMode getInterpolationMode() const;

	void setRotationInterpolationMode( AnimationManager::RotationInterpolationMode im );
	AnimationManager::RotationInterpolationMode getRotationInterpolationMode() const;

	void setUseShortestRotationPath ( bool useShortestPath );
	bool getUseShortestRotationPath () const;

	// TODO - kumulatives Animieren ein/aus
private:
	KeyFrame* getKeyFrameAtTimePos(Real timePos);	

	Animation* mAnimation;
	AnimationTrack* mAnimationTrack;
};

}
#endif
