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

#ifndef __AnimationManager_H__
#define __AnimationManager_H__

#include <map>
#include <OgreSingleton.h>

#include "GameTask.h"
#include "CorePrerequisites.h"

namespace rl {

class Actor;
class Animation;
class TrackAnimation;

class _RlCoreExport AnimationManager
  : public GameTask,
    protected Ogre::Singleton<AnimationManager>
{
public:
	enum InterpolationMode { IM_LINEAR, IM_SPLINE };
	enum RotationInterpolationMode { RIM_LINEAR, RIM_SPHERICAL };

    /** Default Constructor */
	AnimationManager( );
	/** Default Deconstructor */
    virtual ~AnimationManager();

    Animation* addAnimation(Ogre::AnimationState* animState,
        Ogre::Real speed=1.0, unsigned int timesToPlay=0 );
        
	Animation* getAnimation(Ogre::AnimationState* animState) const;
    void removeAnimation(Ogre::AnimationState* animState);
	TrackAnimation* createTrackAnimation(Actor* actor,
	    const Ogre::String& name, Ogre::Real length );
	void removeAnimation(Animation* anim);
	
	// Für globale SlowMotion oder anderes
	void setGlobalAnimationSpeed( Ogre::Real speed );
	Ogre::Real getGlobalAnimationSpeed( ) const;
	
	void setDefaultInterpolationMode( AnimationManager::InterpolationMode im );
	AnimationManager::InterpolationMode getDefaultInterpolationMode() const;

	void setDefaultRotationInterpolationMode( AnimationManager::RotationInterpolationMode rim );
	AnimationManager::RotationInterpolationMode getDefaultRotationInterpolationMode() const;

    virtual void run(Ogre::Real timePassed);

    static AnimationManager & getSingleton(void);
	static AnimationManager * getSingletonPtr(void);
private:
    std::map<Ogre::AnimationState*,Animation*> mAnimationMap;

	Ogre::Real mGlobalAnimationSpeed;
};
}
#endif
