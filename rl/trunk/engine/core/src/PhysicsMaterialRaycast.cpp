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

#include "PhysicsMaterialRaycast.h"

using namespace Ogre;
using namespace OgreNewt;

namespace rl {
    PhysicsMaterialRaycast::PhysicsMaterialRaycast()
        : Raycast(), mInfo(), mMaterial(0), mGetNearest(false)
    {
    }

    RaycastInfo PhysicsMaterialRaycast::execute(OgreNewt::World* world, const MaterialID* material,
        const Vector3& start, const Vector3& end, bool invertmat)
    {
        mMaterial = material;
        mInvertMat = invertmat;

        mInfo.mBody = 0;
        mInfo.mDistance = 1.1;
        mInfo.mNormal = Vector3::ZERO;
        
        mGetNearest = false;
        go(world, start, end);
        
        return mInfo;
    }

    bool PhysicsMaterialRaycast::userCallback(Body* body, Ogre::Real distance, const Ogre::Vector3& normal, int collisionID)
    {
        if( mMaterial == NULL )
        {
            mInfo.mBody = body;
            mInfo.mDistance = distance;
            mInfo.mNormal = normal;
            mGetNearest = true;
        }
        else if (body->getMaterialGroupID() && 
            (body->getMaterialGroupID()->getID() == mMaterial->getID() && !mInvertMat ||
             body->getMaterialGroupID()->getID() != mMaterial->getID() && mInvertMat))
        {
            mInfo.mBody = body;
            mInfo.mDistance = distance;
            mInfo.mNormal = normal;
            mGetNearest = true;
        }
        return mGetNearest;
    }

}
