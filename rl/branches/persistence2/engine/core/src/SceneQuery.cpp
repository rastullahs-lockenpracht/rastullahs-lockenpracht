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

#include "SceneQuery.h"

#include "PhysicsManager.h"
#include "CoreSubsystem.h"
#include "World.h"

using namespace Ogre;

namespace rl
{
    SceneQuery::SceneQuery(unsigned long mask)
        : mResult(),
          mDetailMode(DM_COARSE),
          mPosition(Ogre::Vector3::ZERO),
          mOrientation(Ogre::Quaternion::IDENTITY),
          mQueryMask(mask)
    {
    }

    void SceneQuery::clearLastResult()
    {
        mResult.clear();
    }

    const ActorVector& SceneQuery::getLastResult() const
    {
        return mResult;
    }

    void SceneQuery::setDetailMode(SceneQuery::DetailMode mode)
    {
        mDetailMode = mode;
    }

    SceneQuery::DetailMode SceneQuery::getDetailMode() const
    {
        return mDetailMode;
    }

    void SceneQuery::setPosition(const Ogre::Vector3& position)
    {
        mPosition = position;
    }

    Ogre::Vector3 SceneQuery::getPosition() const
    {
        return mPosition;
    }

    void SceneQuery::setOrientation(const Ogre::Quaternion& orientation)
    {
        mOrientation = orientation;
    }

    Ogre::Quaternion SceneQuery::getOrientation() const
    {
        return mOrientation;
    }

    void SceneQuery::setQueryMask(unsigned long mask)
    {
        mQueryMask = mask;
    }

    unsigned long SceneQuery::getQueryMask() const
    {
        return mQueryMask;
    }

    //////////////////////////////////////////////////////////////////////////

    RaySceneQuery::RaySceneQuery(unsigned long mask)
        : SceneQuery(mask),
          mRayStart(Ogre::Vector3::ZERO),
          mRayEnd(Ogre::Vector3::ZERO),
          mLevelOcclusion(true)
    {
    }

    const ActorVector& RaySceneQuery::execute()
    {
        // Clear last results
        mResult.clear();

        // Setup and execute raycast. Set result to be ordered by distance
        OgreNewt::BasicRaycast raycast = OgreNewt::BasicRaycast(
            PhysicsManager::getSingleton()._getNewtonWorld(), mRayStart, mRayEnd, true);

	    const OgreNewt::MaterialID* levelId = PhysicsManager::getSingleton().getMaterialID("level");

        // Collect results
        for (int i = 0, num = raycast.getHitCount(); i < num; ++i)
        {
            OgreNewt::BasicRaycast::BasicRaycastInfo info = raycast.getInfoAt(i);
            OgreNewt::Body* body = info.mBody;
            if (body != NULL)
            {
                // Are we done?
                if (mLevelOcclusion && (body->getMaterialGroupID() == levelId)) break;

                // Add actor to this body to the result
                Actor* actor = NULL;
                if( body->getUserData().getType() == typeid(Actor*) )
                {
                    actor = Ogre::any_cast<Actor*>(body->getUserData());
                }
                if (actor != NULL) mResult.push_back(actor);
            }
        }
        return mResult;
    }

    void RaySceneQuery::setRay(const Ogre::Vector3& start, const Ogre::Vector3& end)
    {
        mRayStart = start;
        mRayEnd = end;
    }

    Ogre::Vector3 RaySceneQuery::getRayStart() const
    {
        return mRayStart;
    }

    Ogre::Vector3 RaySceneQuery::getRayEnd() const
    {
        return mRayEnd;
    }

    /// If set to true, level occlusion is taken into account and only unoccluded
    /// actors are returned by execute(). If set to false, all hit actors are returned.
    /// Default is true.
    void RaySceneQuery::setLevelOcclusion(bool occ)
    {
        mLevelOcclusion = occ;
    }

    SphereSceneQuery::SphereSceneQuery(SceneManager* smgr, unsigned long mask)
        : SceneQuery(mask), mSceneQuery(NULL), mRadius(0.0f)
    {
        mSceneQuery = smgr->createSphereQuery(Sphere());
    }

    const ActorVector& SphereSceneQuery::execute()
    {
        // Discard results from last query
        mResult.clear();

        // Prepare query
        mSceneQuery->setSphere(Sphere(mPosition, mRadius));
        mSceneQuery->setQueryMask(mQueryMask);

        // Determine plane facing of the half-sphere ground plain.
        Vector3 forward = mOrientation * Vector3::NEGATIVE_UNIT_Z;

        const SceneQueryResult& result = mSceneQuery->execute();
        for (SceneQueryResultMovableList::const_iterator it = result.movables.begin(),
            end = result.movables.end(); it != end; ++it)
        {
            Actor* actor = dynamic_cast<Actor*>((*it)->getUserObject());
            if (actor != NULL) mResult.push_back(actor);
        }

        return mResult;
    }

    void SphereSceneQuery::setRadius(Ogre::Real radius)
    {
        mRadius = radius;
    }

    Ogre::Real SphereSceneQuery::getRadius() const
    {
        return mRadius;
    }


    //////////////////////////////////////////////////////////////////////////

    /// function object for sorting Actors in the query result set
    struct MinimalAngleSorter : public std::binary_function<Actor*, Actor*, bool>
    {
        MinimalAngleSorter(const Vector3& pos, const Vector3& direction)
            : mPosition(pos), mDirection(direction)
        {
        }

        bool operator()(Actor* lhs, Actor* rhs) const
        {
            Real dotLhs = mDirection.dotProduct(
                (lhs->getWorldPosition() - mPosition).normalisedCopy());
            Real dotRhs = mDirection.dotProduct(
                (rhs->getWorldPosition() - mPosition).normalisedCopy());

            return dotLhs > dotRhs;
        }

    private:
        Vector3 mPosition;
        Vector3 mDirection;
    };

    HalfSphereSceneQuery::HalfSphereSceneQuery(SceneManager* smgr, unsigned long mask)
        : SceneQuery(mask), mSceneQuery(NULL), mRadius(0.0f), mSceneManager(NULL)
    {
        mSceneQuery = smgr->createSphereQuery(Sphere());
        mSceneManager = smgr;
    }

    HalfSphereSceneQuery::~HalfSphereSceneQuery()
    {
        mSceneManager->destroyQuery(mSceneQuery);
    }

    const ActorVector& HalfSphereSceneQuery::execute()
    {
        // Discard results from last query
        mResult.clear();

        // Prepare query
        mSceneQuery->setSphere(Sphere(mPosition, mRadius));
        mSceneQuery->setQueryMask(mQueryMask);

        // Determine plane facing of the half-sphere ground plain.
        Vector3 forward = mOrientation * Vector3::NEGATIVE_UNIT_Z;

        const SceneQueryResult& result = mSceneQuery->execute();
        for (SceneQueryResultMovableList::const_iterator it = result.movables.begin(),
            end = result.movables.end(); it != end; ++it)
        {
            Vector3 movablePos = (*it)->getParentNode()->_getDerivedPosition();
            if (forward.dotProduct(movablePos - mPosition) > 0.0f)
            {
                Actor* actor = dynamic_cast<Actor*>((*it)->getUserObject());
                if (actor != NULL) mResult.push_back(actor);
            }
        }

        // Sort by angular distance from plane normal
        std::sort(mResult.begin(), mResult.end(), MinimalAngleSorter(mPosition, forward));

        return mResult;
    }

    void HalfSphereSceneQuery::setRadius(Ogre::Real radius)
    {
        mRadius = radius;
    }

    Ogre::Real HalfSphereSceneQuery::getRadius() const
    {
        return mRadius;
    }

    OgreRaySceneQuery::OgreRaySceneQuery(unsigned long mask)
        : RaySceneQuery(mask)
    {
    }

    const ActorVector& OgreRaySceneQuery::execute()
    {
        // Clear last results
        mResult.clear();

		Ray ray(getRayStart(), (getRayEnd() - getRayStart()).normalisedCopy());

		Ogre::RaySceneQuery* query = CoreSubsystem::getSingleton().getWorld()->getSceneManager()
			->createRayQuery(ray, getQueryMask());

		query->execute(this);

        return mResult;
    }

	bool OgreRaySceneQuery::queryResult(
		Ogre::MovableObject* obj, Ogre::Real distance)
	{
        Actor* actor = dynamic_cast<Actor*>(obj->getUserObject());
        if (actor != NULL)
		{
			mResult.push_back(actor);
		}
		return true;
	}

	bool OgreRaySceneQuery::queryResult(
		Ogre::SceneQuery::WorldFragment* fragment, Ogre::Real distance)
	{
		return true;
	}
}
