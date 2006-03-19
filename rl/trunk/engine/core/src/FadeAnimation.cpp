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

#include "FadeAnimation.h"

#include "CoreSubsystem.h"
#include "Exception.h"

#include "ScriptWrapper.h"
#include "AnimationManager.h"

namespace rl 
{

static Ogre::Real s_Epsilon = 0.0000001;

AnimationFadeOptions::AnimationFadeOptions( MeshAnimation* anim, Ogre::Real timeStart, 
            Ogre::Real timeEnd, Ogre::Real weightStart, Ogre::Real weightEnd )
{
    mAnimation = anim;
    mTimeStart = timeStart; 
    mTimeEnd = timeEnd;
    mWeightStart = weightStart;
    mWeightEnd = weightEnd;
}

FadeAnimation::FadeAnimation() : 
    BaseAnimation(0.0, 1.0, 1, true ),
	mDeleteOnFinish( true ),
    mDeleteAnimationsOnFinish( false ),
    mFadeOptions(),
    mFadeSoft( false )
{
}

FadeAnimation::~FadeAnimation() 
{
    for (FadeOptionsSet::iterator it = mFadeOptions.begin();
            it != mFadeOptions.end(); ) 
    {
        AnimationFadeOptions* afo = *it;

        if( mDeleteAnimationsOnFinish && 
            afo->getAnimation()->getWeight() == 0.0 )
            AnimationManager::getSingleton().removeAnimation( afo->getAnimation() );

        ScriptWrapper::getSingleton().deleted( afo );
        mFadeOptions.erase(it++);
        delete afo;
    }
}

void FadeAnimation::addAnimation( MeshAnimation* anim, Ogre::Real timeStart, 
    Ogre::Real timeEnd, Ogre::Real weightStart, Ogre::Real weightEnd )
{
    anim->doAddTime(0.0);
    mFadeOptions.insert( new AnimationFadeOptions(anim, timeStart, timeEnd, weightStart, weightEnd ) );

    if( timeStart < s_Epsilon )
        anim->setWeight(weightStart);
    else
        anim->setWeight(0.0);

    mLength = max(timeEnd,mLength);
}

void FadeAnimation::removeAnimation( MeshAnimation* anim )
{
    for (FadeOptionsSet::iterator it = mFadeOptions.begin();
            it != mFadeOptions.end(); ) 
    {
        if( (*it)->getAnimation() == anim ) 
        {
            AnimationFadeOptions* afo = *it;
            ScriptWrapper::getSingleton().deleted( afo );
            mFadeOptions.erase(it++);
            delete afo;
        } 
        else 
           ++it;
    }
}

bool FadeAnimation::isDeleteOnFinish() const
{
	return mDeleteOnFinish;
}

void FadeAnimation::setDeleteOnFinish( bool deleteOnFinish )
{
	mDeleteOnFinish = deleteOnFinish;
}

/// FIXME - was macht Rückwärts?
void FadeAnimation::doAddTime( Ogre::Real timePassed )
{        
    for( FadeOptionsSet::iterator it = mFadeOptions.begin();
            it != mFadeOptions.end(); it++ ) 
    {
        AnimationFadeOptions* afo = *it;
        Ogre::Real localElapsed = mTimePlayed + timePassed - afo ->getTimeStart();
        
        // Nach der Anfangszeit
        if( localElapsed >= 0.0 )
        {
            Ogre::Real localLength = afo->getTimeEnd() - afo->getTimeStart();

            // Unter Endzeit
            if( mFadeSoft && localLength > 0.0f && localElapsed <= localLength )
            {
                Ogre::Real fadeFactor = (localElapsed/localLength);
                Ogre::Real finalWeight = afo->getWeightEnd()*(1-fadeFactor) +
                    afo->getWeightStart() * (fadeFactor); 

                afo->getAnimation()->setWeight(finalWeight);                
            }
            else if( !mFadeSoft && localElapsed >= localLength )
            {
                afo->getAnimation()->setWeight(afo->getWeightEnd());  
            }
            else if( !mFadeSoft && localElapsed < localLength )
            {
                afo->getAnimation()->setWeight(afo->getWeightStart());
            }
        }
    }
}

}
