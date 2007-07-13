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
#include "stdinc.h" //precompiled header


#include "CreatureWalkPathJob.h"

namespace rl
{
    CreatureWalkPathJob::CreatureWalkPathJob(const Ogre::String& name, CreatureController* movingCreature, Landmark* startLandmark)
        : Job(true, true), mLandmarkPath("LandmarkPath" + name), mNextLandmarkRequested(false)
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
        if(mNextLandmarkRequested)
        {
            if (mLandmarkPath.getPoints().size())
            {
                mCurrentLandmark = mNextLandmark;
                mNextLandmark = mLandmarkPath.getPoints().front();
                mLandmarkPath.getPoints().pop_front();
                mNextLandmarkRequested = false;
            }
            else
            {
                mCurrentLandmark = mNextLandmark;
                ///@todo
            }
        }

        updateCreature(true);

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

    CreatureController* CreatureWalkPathJob::getMovingCreature()
    {
        return mMovingCreature;
    }

    void CreatureWalkPathJob::updateCreature(Ogre::Real time)
    {
        static bool updatedDirection(false);
        static Ogre::Real timeSinceLastRotation = 0;

        timeSinceLastRotation += time;

        Ogre::Vector3 direction = mNextLandmark->getPosition() - mMovingCreature->getCreature()->getActor()->getPosition();
        direction.y = 0;
        if( direction.squaredLength() < 0.04 )
        {
            mMovingCreature->setMovement(CreatureController::MT_STEHEN, Ogre::Vector3::ZERO, Ogre::Vector3::ZERO);
            mNextLandmarkRequested = true;
            updatedDirection = false;
            return;
        }

        Ogre::Vector3 creatureViewVector = mMovingCreature->getCreature()->getActor()->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
        creatureViewVector.y = 0;
        Ogre::Quaternion rotation = creatureViewVector.getRotationTo(direction, Ogre::Vector3::UNIT_Y);
        Ogre::Radian yaw = rotation.getYaw();
        Ogre::Vector3 usedRotation(Ogre::Vector3::ZERO);

        if(!updatedDirection || (direction.squaredLength() > 0.04 && timeSinceLastRotation > 1 &&
            direction.normalisedCopy().dotProduct(creatureViewVector.normalisedCopy()) < 0.9))
        {
            usedRotation.y = yaw.valueRadians();
            updatedDirection = true;
            timeSinceLastRotation = 0;
        }

        mMovingCreature->setMovement(CreatureController::MT_GEHEN, Ogre::Vector3::NEGATIVE_UNIT_Z, usedRotation);
    }
}
