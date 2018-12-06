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

#include "AStar.h"
#include "CreatureWalkPathJob.h"
#include <CreatureControllerManager.h>
#include <GameObjectManager.h>
#include <JobScheduler.h>

namespace rl
{
    const CeGuiString CreatureWalkPathJob::PROPERTY_CURRENTLANDMARK = "currentlandmark";
    const CeGuiString CreatureWalkPathJob::PROPERTY_NEXTLANDMARKS = "nextlandmarks";
    const CeGuiString CreatureWalkPathJob::PROPERTY_GAMEOBJECTID = "go_id";

    CreatureWalkPathJob::CreatureWalkPathJob(
        const Ogre::String& name, Creature* movingCreature, Landmark* startLandmark)
        : Job(false, true, TimeSource::GAMETIME, Job::PERSISTENT)
        , mLandmarkPath("LandmarkPath" + name)
        , mWayPoints(NULL)
        , mUpdatedDirection(false)
        , mTimeSinceLastRotation(0)
    {
        // the moving creature moves from the current position to the landmark
        mMovingCreature = movingCreature;
        if (movingCreature)
            mCurrentLandmark = new Landmark(name + "_startup", mMovingCreature->getPosition());
        else
            mCurrentLandmark = startLandmark;
        mNextLandmark = startLandmark;

        // JobScheduler::registerJobClass(getClassName().c_str(),
        // (JobScheduler::JobCreateFunction)createSavedCreateWalkPathJob);
    }

    CreatureWalkPathJob::~CreatureWalkPathJob()
    {
    }

    bool CreatureWalkPathJob::execute(Ogre::Real time)
    {
        mTimeSinceLastRotation += time;

        CreatureController* controller
            = CreatureControllerManager::getSingleton().getCreatureController(mMovingCreature);

        Ogre::Vector3 direction = mCurrentLandmark->getPosition() - mMovingCreature->getActor()->getPosition();
        direction.y = 0;
        if (direction.squaredLength() < 0.04)
        {
            controller->setMovement(CreatureController::MT_STEHEN, Ogre::Vector3::ZERO, Ogre::Vector3::ZERO);

            if (!mLandmarkPath.isEmpty())
            {
                mCurrentLandmark = mNextLandmark;
                mNextLandmark = mLandmarkPath.getPoints().front();
                mLandmarkPath.removePoint(mNextLandmark);
                if (mWayPoints)
                {
                    mCurrentWayPath
                        = AStar::search(mWayPoints, mMovingCreature->getPosition(), mCurrentLandmark->getPosition());
                    std::ostringstream ss;
                    ss << "Current Position: " << mMovingCreature->getPosition() << "\n";
                    for (AStar::AStarPath::const_iterator it = mCurrentWayPath.begin(); it != mCurrentWayPath.end();
                         ++it)
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
                mCurrentWayPath
                    = AStar::search(mWayPoints, mMovingCreature->getPosition(), mCurrentLandmark->getPosition());
                std::ostringstream ss;
                ss << "Current Position: " << mMovingCreature->getPosition() << "\n";
                for (AStar::AStarPath::const_iterator it = mCurrentWayPath.begin(); it != mCurrentWayPath.end(); ++it)
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
                + ", current position " + Ogre::StringConverter::toString(mMovingCreature->getPosition()));

        Ogre::Vector3 creatureViewVector
            = mMovingCreature->getActor()->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
        creatureViewVector.y = 0;
        Ogre::Quaternion rotation = creatureViewVector.getRotationTo(direction, Ogre::Vector3::UNIT_Y);
        Ogre::Radian yaw = rotation.getYaw();
        Ogre::Vector3 usedRotation(Ogre::Vector3::ZERO);

        if (!mUpdatedDirection
            || (direction.squaredLength() > 0.04 && mTimeSinceLastRotation > 0.2
                   && direction.normalisedCopy().dotProduct(creatureViewVector.normalisedCopy()) < 0.9))
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

    void CreatureWalkPathJob::setWayPoints(const rl::WayPointGraph* wps)
    {
        mWayPoints = wps;
    }

    /// derived from PropertyHolder
    const Property CreatureWalkPathJob::getProperty(const CeGuiString& key) const
    {
        Property prop;
        if (key == PROPERTY_CURRENTLANDMARK)
        {
            PropertyRecord record;
            record.setProperty("name", Property(mCurrentLandmark->getName()));
            record.setProperty("position", Property(mCurrentLandmark->getPosition()));
            prop.setValue(record.toPropertyMap());
        }
        else if (key == PROPERTY_NEXTLANDMARKS)
        {
            if (mLandmarkPath.getPoints().size())
            {
                PropertyArray vec;
                for (LandmarkPath::LandmarkList::const_iterator it = mLandmarkPath.begin(); it != mLandmarkPath.end();
                     ++it)
                {
                    PropertyRecord rec;
                    rec.setProperty("name", Property((*it)->getName()));
                    rec.setProperty("position", Property((*it)->getPosition()));
                    vec.push_back(rec.toPropertyMap());
                }
                prop.setValue(vec);
            }
        }
        else if (key == PROPERTY_GAMEOBJECTID)
        {
            return GameObjectManager::getSingleton().toProperty(mMovingCreature);
        }
        return prop;
    }

    /// derived from PropertyHolder
    void CreatureWalkPathJob::setProperty(const CeGuiString& key, const Property& value)
    {
        if (key == PROPERTY_CURRENTLANDMARK)
        {
            PropertyMap map(value);
            delete mCurrentLandmark;
            mCurrentLandmark = new Landmark(map["name"].toString().c_str(), map["position"].toVector3());
        }
        else if (key == PROPERTY_NEXTLANDMARKS)
        {
            for (LandmarkPath::LandmarkList::const_iterator it = mLandmarkPath.begin(); it != mLandmarkPath.end(); ++it)
            {
                delete *it;
            }
            mLandmarkPath.getPoints().clear();
            PropertyArray vec(value.toArray());
            for (PropertyArray::const_iterator it = vec.begin(); it != vec.end(); ++it)
            {
                PropertyMap map = *it;
                mLandmarkPath.getPoints().push_back(
                    new Landmark(map["name"].toString().c_str(), map["position"].toVector3()));
            }
        }
        else if (key == PROPERTY_GAMEOBJECTID)
        {
            mMovingCreature
                = static_cast<Creature*>(GameObjectManager::getSingleton().createGameObjectFromProperty(value));
        }
    }

    /// derived from PropertyHolder
    PropertyKeys CreatureWalkPathJob::getAllPropertyKeys() const
    {
        PropertyKeys keys(Job::getAllPropertyKeys());
        keys.insert(PROPERTY_CURRENTLANDMARK);
        keys.insert(PROPERTY_NEXTLANDMARKS);
        keys.insert(PROPERTY_GAMEOBJECTID);
        return keys;
    }

    /*CreatureWalkPathJob* createSavedCreateWalkPathJob()
    {
        return new CreatureWalkPathJob("",NULL,new Landmark("", Ogre::Vector3::ZERO));
    }*/
}
