/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#include "CreatureWalkPathJob.h"

namespace rl
{
    CreatureWalkPathJob::CreatureWalkPathJob(const Ogre::String& name, MovingCreature* movingCreature, Landmark* startLandmark) : Job(true, true), 
        mLandmarkPath("LandmarkPath" + name)
    {
        mMovingCreature = movingCreature;
        //addLandmark(startLandmark);
        mCurrentLandmark = new Landmark(name + "_startup",mMovingCreature->getCreature()->getPosition());
        mNextLandmark = startLandmark;
        //mMovingCreature->getCreature()->setPosition(startLandmark->getPosition());
    }

    CreatureWalkPathJob::~CreatureWalkPathJob()
    {
    }

    bool CreatureWalkPathJob::execute(Ogre::Real time)
    {
        if (mLandmarkPath.getPoints().size() > 0 && mMovingCreature->getCreature()->getPosition() == mNextLandmark->getPosition())
        {
            mCurrentLandmark = mNextLandmark;
            mNextLandmark = mLandmarkPath.getPoints().front();
            mLandmarkPath.getPoints().pop_front();
        }
        else if (mMovingCreature->getCreature()->getPosition() != mNextLandmark->getPosition())
        {
            Ogre::Vector3 creatureViewVector = mMovingCreature->getCreature()->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
            Ogre::Vector3 diffTrack = mNextLandmark->getPosition() - mCurrentLandmark->getPosition();
            /*if (diffTrack.dotProduct(creatureViewVector) < 0.5)
            {

            }*/
            mMovingCreature->setMovement(MovingCreature::MT_GEHEN,diffTrack,Ogre::Vector3(0,0,0));
            if(diffTrack.dotProduct(mNextLandmark->getPosition()-mMovingCreature->getCreature()->getPosition()))
                mMovingCreature->getCreature()->setPosition(mNextLandmark->getPosition());        
        }
        return false;
    }

    void CreatureWalkPathJob::addLandmark(const Ogre::String& name, const Ogre::Vector3& position)
    {
        mLandmarkPath.addPoint(name, position);
    }

    void CreatureWalkPathJob::addLandmark(Landmark* lm)
    {
        mLandmarkPath.addPoint(lm);
    }
}