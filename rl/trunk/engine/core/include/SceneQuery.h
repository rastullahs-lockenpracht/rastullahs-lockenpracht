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

#ifndef __RL_SCENE_QUERY_H__
#define __RL_SCENE_QUERY_H__

#include "CorePrerequisites.h"
#include "DebugVisualisable.h"
#include "Actor.h"

#include <OgreNewt_RayCast.h>

namespace rl
{
    /// This is a super class for all kinds of scene queries.
    /// Those queries can utilise Newton, Ogre or both.
    class _RlCoreExport SceneQuery
    {
    public:
        typedef enum {DM_COARSE, DM_FINE} DetailMode;

        SceneQuery(unsigned long mask = 0xffffffff);

        /// Executes the query anew. Last result set is
        /// overwritten by the new one.
        virtual const ActorVector& execute() = 0;

        /// Clear results of the last query execution
        void clearLastResult();

        /// Get the results of the last execution.
        /// Empty, if this query has not yet been executed.
        const ActorVector& getLastResult() const;

        /// Sets the detail mode for the query. How and whether at all this
        /// flag is interpreted is decided by the subclasses.
        void setDetailMode(DetailMode mode);

        /// Gets the detail mode for the query.
        DetailMode getDetailMode() const;

        void setPosition(const Ogre::Vector3& position);
        Ogre::Vector3 getPosition() const;

        void setOrientation(const Ogre::Quaternion& orientation);
        Ogre::Quaternion getOrientation() const;

        void setQueryMask(unsigned long mask);
        unsigned long getQueryMask() const;

    protected:
        ActorVector mResult;
        DetailMode mDetailMode;
        Ogre::Vector3 mPosition;
        Ogre::Quaternion mOrientation;
        unsigned long mQueryMask;
    };

    /// This query uses Newton's raycast mechanism, which works better with trimeshes
    /// than Ogre's AABB based approach.
    class _RlCoreExport RaySceneQuery : public SceneQuery
    {
    public:
        RaySceneQuery(unsigned long mask = 0xffffffff);

        virtual const ActorVector& execute();

        void setRay(const Ogre::Vector3& start, const Ogre::Vector3& end);

        Ogre::Vector3 getRayStart() const;

        Ogre::Vector3 getRayEnd() const;

        /// If set to true, level occlusion is taken into account and only unoccluded
        /// actors are returned by execute(). If set to false, all hit actors are returned.
        /// Default is true.
        void setLevelOcclusion(bool occ);

    private:
        Ogre::Vector3 mRayStart;
        Ogre::Vector3 mRayEnd;
        bool mLevelOcclusion;
    };

    /// This query queries for objects within a sphere.
    class _RlCoreExport SphereSceneQuery : public SceneQuery
    {
    public:
        SphereSceneQuery(Ogre::SceneManager* smgr, unsigned long mask = 0xffffffff);

        virtual const ActorVector& execute();

        /// Set the radius of the sphere. Value has to be non-negative.
        void setRadius(Ogre::Real radius);
        Ogre::Real getRadius() const;

    protected:
        Ogre::SphereSceneQuery* mSceneQuery;
        Ogre::Real mRadius;
    };

    /// This query queries for objects within a half-sphere.
    /// The half-sphere's ground plate is on the local XY-plane, with its center
    /// in the origin. The half-sphere expands in the local -Z direction.
    /// Use SceneQuery::setOrientation to orient it to your needs.
    class _RlCoreExport HalfSphereSceneQuery : public SceneQuery
    {
    public:
        HalfSphereSceneQuery(Ogre::SceneManager* smgr, unsigned long mask = 0xffffffff);

        virtual const ActorVector& execute();

        /// Set the radius of the sphere. Value has to be non-negative.
        void setRadius(Ogre::Real radius);
        Ogre::Real getRadius() const;

    protected:
        Ogre::SphereSceneQuery* mSceneQuery;
        Ogre::Real mRadius;
    };
}

#endif
