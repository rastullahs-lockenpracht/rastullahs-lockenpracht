/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __PhysicsMaterialRaycast_H__
#define __PhysicsMaterialRaycast_H__

#include "CorePrerequisites.h"
#include <OgreNewt.h>

namespace rl {
    struct _RlCoreExport RaycastInfo
    {
        RaycastInfo() : mDistance(), mBody(), mNormal() {}
        /// dist from point1 of the raycast, in range [0,1].
        Ogre::Real mDistance;	
        /// pointer to body intersected with
        OgreNewt::Body* mBody;
        /// normal of intersection.
        Ogre::Vector3 mNormal;
    };

    class _RlCoreExport PhysicsMaterialRaycast : OgreNewt::Raycast
    {
    public:
        PhysicsMaterialRaycast();

        /** executes the Raycast
        * @param world OgreNewt-World
        * @param material The material, which will be found by the raycast.
        * If this param is NULL, every material will be found
        * @param startpt start-point of the cast
        * @param endpt end-point of the cast
        * @param invertmat if "true", the ray finds all collisions, which do NOT have the material-id "material"
        */
        RaycastInfo execute(OgreNewt::World* world, const OgreNewt::MaterialID* material,
            const Ogre::Vector3& startpt, const Ogre::Vector3& endpt, bool invertmat = false);

        bool userCallback(OgreNewt::Body* body, Ogre::Real distance, const Ogre::Vector3& normal, int collisionID);
    private:
        RaycastInfo mInfo;
        const OgreNewt::MaterialID* mMaterial;
        bool mGetNearest;
        bool mInvertMat;
    };
}

#endif
