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
 *
 * The WalkPathBehaviour was realised by using the Pedestrian example from
 * OpenSteer, Copyright (c) 2002-2005, Sony Computer Entertainment America
 * Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
 */
#include "WalkPathBehaviour.h"
#include "AStar.h"
#include "Exception.h"
#include "Landmark.h"
#include "LandmarkPath.h"
#include "OpenSteer/Vec3.h"
#include "SteeringVehicle.h"
#include "stdinc.h"

namespace rl
{
    WalkPathBehaviour::WalkPathBehaviour()
        : mPathDirection(-1)
    {
        init();
    }

    WalkPathBehaviour::~WalkPathBehaviour()
    {
    }

    CeGuiString WalkPathBehaviour::getType()
    {
        return "WalkPath";
    }

    void WalkPathBehaviour::init()
    {
        setActivationLevel(1.0);
    }

    void WalkPathBehaviour::activate()
    {
        setActivationLevel(1.0);
    }

    void WalkPathBehaviour::deactivate()
    {
        setActivationLevel(0.0);
    }

    void WalkPathBehaviour::calcPathToLandmark(Landmark* lm, const WayPointGraph* wps)
    {
        RlAssert(lm != NULL, "Landmark is NULL");
        RlAssert(wps != NULL, "WayPointGraph is NULL");
        mLandmark = lm;
        AStar::AStarPath pathway = AStar::search(wps, mController->getPosition(), mLandmark->getPosition());

        mPathway = OpenSteer::PolylineSegmentedPathwaySingleRadius(pathway,
            2, // radius of points
            false); // cyclic pathway?
    }

    void WalkPathBehaviour::setLandmarkPath(LandmarkPath* lmp)
    {
        std::vector<Vector3> pathway;
        LandmarkPath::LandmarkList list = lmp->getPoints();
        LandmarkPath::LandmarkList::const_iterator itr = list.begin();
        LandmarkPath::LandmarkList::const_iterator end = list.end();
        for (; itr != end; ++itr)
        {
            pathway.push_back((*itr)->getPosition());
        }
        mPathway = OpenSteer::PolylineSegmentedPathwaySingleRadius(pathway,
            2, // radius of points
            false); // cyclic pathway?
    }

    void WalkPathBehaviour::update(const float elapsedTime)
    {
        // compute combined steering force: move forward, avoid obstacles
        // or neighbors if needed, otherwise follow the path

        // move forward
        Vector3 steeringForce = mController->getForward();

        // determine if obstacle avoidance is required
        Vector3 obstacleAvoidance;

        const float minTimeToCollision = 6;
        obstacleAvoidance = mController->calcAvoidObstacles(minTimeToCollision);

        // if obstacle avoidance is needed, do it
        if (obstacleAvoidance != Vector3::ZERO)
        {
            steeringForce += obstacleAvoidance;
        }
        else
        {
            // otherwise consider avoiding collisions with others
            Vector3 collisionAvoidance;
            const float caLeadTime = 10;

            // avoid collision with all neighbors within maxRadius
            // (radius is largest distance between vehicles traveling head-on
            // where a collision is possible within caLeadTime seconds.)
            const float maxRadius = caLeadTime * mController->getMaxSpeed() * 2;
            collisionAvoidance = mController->calcAvoidNeighbors(caLeadTime) * 10;

            // if collision avoidance is needed, do it
            if (collisionAvoidance != Vector3::ZERO)
            {
                steeringForce += collisionAvoidance;
            }
            else
            {
                // add in wander component (according to user switch)
                // if we want wander, we add it in as additional behaviour!
                // if (gWanderSwitch)
                //    steeringForce += mController->steerForWander (elapsedTime);

                const float predictionTime = 3;
                const Vector3 pathFollow = mController->steerToFollowPath(mPathDirection, predictionTime, mPathway);

                // add in to steeringForce
                steeringForce += pathFollow * 0.5;
            }
        }
        float const pathRadius = mPathway.radius();

        if (mController->getPosition().distance(mPathway.point(mPathway.pointCount() - 1)) < pathRadius)
        {
            mPathDirection = -1;
        }
        if (mController->getPosition().distance(mPathway.point(0)) < pathRadius)
        {
            mPathDirection = +1;
        }
        // add steering force constrained to global XZ "ground" plane
        mController->addForce(OpenSteer::Vec3Utils::setYtoZero(steeringForce));
    }

    float WalkPathBehaviour::calculateActivation()
    {
        setActivationLevel(1.0);
        return getActivationLevel();
    }
}
