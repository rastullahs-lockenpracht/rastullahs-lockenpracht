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
        //the moving creature moves from the current position to the landmark
        mMovingCreature = movingCreature;
        mCurrentLandmark = new Landmark(name + "_startup",mMovingCreature->getCreature()->getPosition());
        mNextLandmark = startLandmark;
    }

    CreatureWalkPathJob::~CreatureWalkPathJob()
    {
    }

    bool CreatureWalkPathJob::execute(Ogre::Real time)
    {
        if (mMovingCreature->getCreature()->getActor()->getPosition().x == mNextLandmark->getPosition().x 
            && mMovingCreature->getCreature()->getActor()->getPosition().y == mNextLandmark->getPosition().y)
        {
            if (mLandmarkPath.getPoints().size())
            {
                mCurrentLandmark = mNextLandmark;
                mNextLandmark = mLandmarkPath.getPoints().front();
                mLandmarkPath.getPoints().pop_front();
            }
            else
            {
                mCurrentLandmark = mNextLandmark;
                //@todo
            }
            
        }
        else if (mCurrentLandmark != mNextLandmark)
        {
            updateCreature(time);
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

    MovingCreature* CreatureWalkPathJob::getMovingCreature()
    {
        return mMovingCreature;
    }

    void CreatureWalkPathJob::updateCreature(Ogre::Real time)
    {
        //Max AU ist 0, deswegen erstmal vorläufig:
        if(mMovingCreature->getCreature()->getAu() < 2.0f)
            mMovingCreature->getCreature()->modifyAu(2,true);

        Ogre::Vector3 direction = mNextLandmark->getPosition() - mCurrentLandmark->getPosition();

        Ogre::Vector3 creatureViewVector = mMovingCreature->getCreature()->getActor()->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
        if(creatureViewVector.normalisedCopy().dotProduct(direction.normalisedCopy())  < 0.99)
        {
            //rotate until creature looks into the direction it wants to go to
            //@todo: rotate
            Ogre::Real rotate = Ogre::Math::ACos(creatureViewVector.normalisedCopy().dotProduct(direction.normalisedCopy())).valueRadians();
            mMovingCreature->setMovement(MovingCreature::MT_STEHEN, Ogre::Vector3(0,0,0),Ogre::Vector3(0,rotate,0));
            //bool result = mMovingCreature->setMovement(MovingCreature::MT_STEHEN, Ogre::Vector3(0,0,0), Ogre::Vector3(0,0.5,0));
        }
        else
        {
            //move creature to the target
            mMovingCreature->setMovement(MovingCreature::MT_GEHEN, direction, Ogre::Vector3(0,0,0));
        }

        //make sure that the creature can't fail the landmark
        //Ogre::Vector3 diffTrack = mNextLandmark->getPosition() - mCurrentLandmark->getPosition();
        if(direction.dotProduct(mNextLandmark->getPosition()-mMovingCreature->getCreature()->getActor()->getPosition()) < 0)
            mMovingCreature->getCreature()->getActor()->setPosition(mNextLandmark->getPosition());
    }
}
