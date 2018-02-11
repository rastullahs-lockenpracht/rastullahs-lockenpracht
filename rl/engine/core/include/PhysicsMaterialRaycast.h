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

#ifndef __PhysicsMaterialRaycast_H__
#define __PhysicsMaterialRaycast_H__

#include "CorePrerequisites.h"

#ifdef __APPLE__
#   include <OgreNewt/OgreNewt.h>
#else
#   include <OgreNewt.h>
#endif

#include <vector>
#include <utility>

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
        // the collisionID for compound collisions could be added here, but we didn't need them yet
    };

    class _RlCoreExport PhysicsMaterialRaycast : OgreNewt::Raycast
    {
    public:
        typedef std::vector<const OgreNewt::MaterialID*> MaterialVector;

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
        
        /** executes the Raycast
        * @param world OgreNewt-World
        * @param materials a vector with pairs of a material and a boolean value, that describes if this material should be matched or not
        * If this param is NULL, every material will be found
        * @param startpt start-point of the cast
        * @param endpt end-point of the cast
        * @param invertmat if "true", the ray finds all collisions, which do NOT have the material-id "material"
        */
        RaycastInfo execute(OgreNewt::World* world, const MaterialVector* materials,
            const Ogre::Vector3& startpt, const Ogre::Vector3& endpt, bool invertmat = false);

        bool userCallback(OgreNewt::Body* body, Ogre::Real distance, const Ogre::Vector3& normal, int collisionID);


        bool userPreFilterCallback( OgreNewt::Body *body );  // filtering is now done here!
    private:
        RaycastInfo mInfo;
        const MaterialVector *mMaterialVector;
        const OgreNewt::MaterialID* mMaterial;
        bool mInvertMat;
    };



// --------------------------------------------------------------------------------------

    // we can use the info-class from OgreNewt
    class _RlCoreExport ConvexcastInfo : public OgreNewt::BasicConvexcast::ConvexcastContactInfo
    {
        public:
            ConvexcastInfo(const OgreNewt::BasicConvexcast::ConvexcastContactInfo& info);
            Ogre::Real mDistance;
    };

    class _RlCoreExport PhysicsMaterialConvexcast : public OgreNewt::BasicConvexcast
    {
    public:
        typedef std::vector<const OgreNewt::MaterialID*> MaterialVector;

        PhysicsMaterialConvexcast();

        /** executes the Convexcast
        * @param world OgreNewt-World
        * @param material The material, which will be found by the raycast.
        * If this param is NULL, every material will be found
        * @param col the collisions representing a convex shape used for the cast
        * @param startpt start-point of the cast
        * @param ori the orientation of the collision in global space
        * @param endpt end-point of the cast
        * @param invertmat if "true", the ray finds all collisions, which do NOT have the material-id "material"
        */
        ConvexcastInfo execute(OgreNewt::World* world, const OgreNewt::MaterialID* material,
            const OgreNewt::ConvexCollisionPtr& col, const Ogre::Vector3& startpt, const Ogre::Quaternion &ori,
            const Ogre::Vector3& endpt, bool invertmat = false);
        
        /** executes the Convexcast
        * @param world OgreNewt-World
        * @param materials a vector with pairs of a material and a boolean value, that describes if this material should be matched or not
        * If this param is NULL, every material will be found
        * @param col the collisions representing a convex shape used for the cast
        * @param startpt start-point of the cast
        * @param ori the orientation of the collision in global space
        * @param endpt end-point of the cast
        * @param invertmat if "true", the ray finds all collisions, which do NOT have the material-id "material"
        */
        ConvexcastInfo execute(OgreNewt::World* world, const MaterialVector* materials,
            const OgreNewt::ConvexCollisionPtr& col, const Ogre::Vector3& startpt, const Ogre::Quaternion &ori,
            const Ogre::Vector3& endpt, bool invertmat = false);


        bool userPreFilterCallback( OgreNewt::Body *body );
    private:
        const MaterialVector *mMaterialVector;
        const OgreNewt::MaterialID* mMaterial;
        bool mInvertMat;
    };
}

#endif
