/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include <CreatureControllerManager.h>

namespace rl
{
    CreatureWalkPathJob::CreatureWalkPathJob(const Ogre::String& name, Creature* movingCreature, Landmark* startLandmark)
        : Job(false, true, TimeSource::GAMETIME, Job::FINISH_WHEN_GAME_LOADED), 
        mLandmarkPath("LandmarkPath" + name), 
        mWayPoints(NULL),
        mUpdatedDirection(false),
        mTimeSinceLastRotation(0)
    {
        //the moving creature moves from the current position to the landmark
        mMovingCreature = movingCreature;
        mCurrentLandmark = new Landmark(name + "_startup",mMovingCreature->getPosition());
        mNextLandmark = startLandmark;
    }

    CreatureWalkPathJob::~CreatureWalkPathJob()
    {
    }

    bool CreatureWalkPathJob::execute(Ogre::Real time)
    {
        mTimeSinceLastRotation += time;

        CreatureController* controller = CreatureControllerManager::getSingleton().getCreatureController(mMovingCreature);

        Ogre::Vector3 direction = mCurrentLandmark->getPosition() - mMovingCreature->getActor()->getPosition();
        direction.y = 0;
        if (direction.squaredLength() < 0.04 )
        {
            controller->setMovement(CreatureController::MT_STEHEN, Ogre::Vector3::ZERO, Ogre::Vector3::ZERO);

            if (!mLandmarkPath.isEmpty())
            {
                mCurrentLandmark = mNextLandmark;
                mNextLandmark = mLandmarkPath.getPoints().front();
                mLandmarkPath.removePoint(mNextLandmark);
                if (mWayPoints)
                {
                    mCurrentWayPath = AStar::search(
                        mWayPoints,
                        mMovingCreature->getPosition(), 
                        mCurrentLandmark->getPosition());
                    std::ostringstream ss;
                    ss << "Current Position: " << mMovingCreature->getPosition() << "\n";
                    for (AStar::AStarPath::const_iterator it = mCurrentWayPath.begin();
                        it != mCurrentWayPath.end(); ++it)
                    {
                        ss << *it << " ";
                    }
                    LOG_DEBUG("WalkPathJob", ss.str() + "Next Path");
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

            mUpdatedDirection = false;
            return false;
        }

        if (mWayPoints)
        {

            if (mCurrentWayPath.empty())
            {
                mCurrentWayPath = AStar::search(
                        mWayPoints,
                        mMovingCreature->getPosition(), 
                        mCurrentLandmark->getPosition());
                std::ostringstream ss;
                ss << "Current Position: " << mMovingCreature->getPosition() << "\n";
                for (AStar::AStarPath::const_iterator it = mCurrentWayPath.begin();
                    it != mCurrentWayPath.end(); ++it)
                {
                    ss << *it << " ";
                }
                LOG_DEBUG("WalkPathJob", ss.str() + "Next Path");
            }

            direction = mCurrentWayPath.back() - mMovingCreature->getPosition();
            while (direction.squaredLength() < 0.04)
            {
                mCurrentWayPath.pop_back();
                if (mCurrentWayPath.empty())
                {
                    break;
                }
                direction = mCurrentWayPath.back() - mMovingCreature->getPosition();
            }
            mUpdatedDirection = true;
        }

        LOG_DEBUG("WalkPathJob", 
            "Going to " + Ogre::StringConverter::toString(direction + mMovingCreature->getPosition()) 
            + ", current position "  + Ogre::StringConverter::toString(mMovingCreature->getPosition()));

        Ogre::Vector3 creatureViewVector = mMovingCreature->getActor()->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
        creatureViewVector.y = 0;
        Ogre::Quaternion rotation = creatureViewVector.getRotationTo(direction, Ogre::Vector3::UNIT_Y);
        Ogre::Radian yaw = rotation.getYaw();
        Ogre::Vector3 usedRotation(Ogre::Vector3::ZERO);

        if (!mUpdatedDirection 
            || (direction.squaredLength() > 0.04 
                && mTimeSinceLastRotation > 0.2 
                && direction.normalisedCopy().dotProduct(
                    creatureViewVector.normalisedCopy()) < 0.9))
        {
            usedRotation.y = yaw.valueRadians();
            mUpdatedDirection = true;
            mTimeSinceLastRotation = 0;
        }

        controller->setMovement(CreatureController::MT_GEHEN, Ogre::Vector3::NEGATIVE_UNIT_Z, usedRotation);
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

    Creature* CreatureWalkPathJob::getCreature()
    {
        return mMovingCreature;
    }

    void CreatureWalkPathJob::setWayPoints(const rl::WayPointGraph *wps)
    {
        mWayPoints = wps;
    }
}
