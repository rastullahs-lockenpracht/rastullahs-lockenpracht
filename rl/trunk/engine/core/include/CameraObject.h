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

#ifndef __CameraObject_H__
#define __CameraObject_H__

#include "CorePrerequisites.h"
#include "PhysicalObject.h"

#include <OgreCamera.h>

namespace rl {

    class _RlCoreExport CameraObject : public PhysicalObject
    {
    public:
        CameraObject(const Ogre::String& name);
        ~CameraObject();

        /** 
		 * Returns the Ogre camera
		 * @return the camera
		 */
        Ogre::Camera* getCamera() const;

		/**
		 * Returns the size of the bounding box
		 * Note: an Ogre camera has no size, this method returns the size of
		 * the physics proxy to prevent camera vs wall/object collisions
		 */
		Ogre::AxisAlignedBox getDefaultSize() const;

        virtual Ogre::String getObjectType() const;

		/** 
		 * Transforms a point in world space into the (2D) screen space
		 * @param worldCoords point in world space
		 * @return the corresponding position on the screen. x,y are in [0,1].
		 * The z component is the z value from the eye space, this allows checking
		 * whether a point was behind the camera (z>0 in this case)
		 */
		Ogre::Vector3 getPointOnScreen(const Ogre::Vector3& worldCoords) const;
    };
}
#endif
