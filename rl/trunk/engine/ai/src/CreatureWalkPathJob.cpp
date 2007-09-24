/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h" //precompiled header


#include "CreatureWalkPathJob.h"
#include "AStar.h"

namespace rl
{
    CreatureWalkPathJob::CreatureWalkPathJob(const Ogre::String& name, CreatureController* movingCreature, Landmark* startLandmark)
        : Job(true, true), 
        mLandmarkPath("LandmarkPath" + name), 
        mNextLandmarkRequested(false),
        mWayPoints(NULL)
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
        static bool updatedDirection(false);
        static Ogre::Real timeSinceLastRotation = 0;

        timeSinceLastRotation += time;

        Ogre::Vector3 direction = mCurrentLandmark->getPosition() - mMovingCreature->getCreature()->getActor()->getPosition();
        direction.y = 0;
        if (direction.squaredLength() < 0.04 )
        {
            mMovingCreature->setMovement(CreatureController::MT_STEHEN, Ogre::Vector3::ZERO, Ogre::Vector3::ZERO);

            if (!mLandmarkPath.isEmpty())
            {
                mCurrentLandmark = mNextLandmark;
                mNextLandmark = mLandmarkPath.getPoints().front();
                mLandmarkPath.removePoint(mNextLandmark);
                mNextLandmarkRequested = false;
                if (mWayPoints)
                {
                    mCurrentWayPath = AStar::search(
                        mWayPoints,
                        mMovingCreature->getCreature()->getPosition(), 
                        mCurrentLandmark->getPosition());
                    std::ostringstream ss;
                    ss << "Current Position: " << mMovingCreature->getCreature()->getPosition() << "\n";
                    for (AStar::AStarPath::const_iterator it = mCurrentWayPath.begin();
                        it != mCurrentWayPath.end(); ++it)
                    {
                        ss << *it << " ";
                    }
                    LOG_MESSAGE2("WalkPathJob", ss.str(), "Next Path");
                }
            }
            else if (mNextLandmark)
            {
                mCurrentLandmark = mNextLandmark;
                mNextLandmark = NULL;
            }
            else
            {
                return true;
            }

            updatedDirection = false;
            return false;
        }

        if (mWayPoints)
        {

            if (mCurrentWayPath.empty())
            {
                mCurrentWayPath = AStar::search(
                        mWayPoints,
                        mMovingCreature->getCreature()->getPosition(), 
                        mCurrentLandmark->getPosition());
                std::ostringstream ss;
                ss << "Current Position: " << mMovingCreature->getCreature()->getPosition() << "\n";
                for (AStar::AStarPath::const_iterator it = mCurrentWayPath.begin();
                    it != mCurrentWayPath.end(); ++it)
                {
                    ss << *it << " ";
                }
                LOG_MESSAGE2("WalkPathJob", ss.str(), "Next Path");
            }

            direction = mCurrentWayPath.back() - mMovingCreature->getCreature()->getPosition();
            while (direction.squaredLength() < 0.04)
            {
                mCurrentWayPath.pop_back();
                if (mCurrentWayPath.empty())
                {
                    break;
                }
                direction = mCurrentWayPath.back() - mMovingCreature->getCreature()->getPosition();
            }
            updatedDirection = true;
        }

        LOG_MESSAGE("WalkPathJob", 
            "Going to " + Ogre::StringConverter::toString(direction + mMovingCreature->getCreature()->getPosition()) 
            + ", current position "  + Ogre::StringConverter::toString(mMovingCreature->getCreature()->getPosition()));

        Ogre::Vector3 creatureViewVector = mMovingCreature->getCreature()->getActor()->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
        creatureViewVector.y = 0;
        Ogre::Quaternion rotation = creatureViewVector.getRotationTo(direction, Ogre::Vector3::UNIT_Y);
        Ogre::Radian yaw = rotation.getYaw();
        Ogre::Vector3 usedRotation(Ogre::Vector3::ZERO);

        if (!updatedDirection 
            || (direction.squaredLength() > 0.04 
                && timeSinceLastRotation > 0.2 
                && direction.normalisedCopy().dotProduct(
                    creatureViewVector.normalisedCopy()) < 0.9))
        {
            usedRotation.y = yaw.valueRadians();
            updatedDirection = true;
            timeSinceLastRotation = 0;
        }

        mMovingCreature->setMovement(CreatureController::MT_GEHEN, Ogre::Vector3::NEGATIVE_UNIT_Z, usedRotation);
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

    void CreatureWalkPathJob::setWayPoints(const rl::WayPointGraph *wps)
    {
        mWayPoints = wps;
    }
}
