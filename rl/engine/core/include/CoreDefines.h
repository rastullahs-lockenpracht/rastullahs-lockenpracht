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
#ifndef __CoreConstants_H__
#define __CoreConstants_H__

#include <map>

#include <OgreString.h>

namespace rl
{

    //! differentiates between the different collision primitives
    enum GeometryType
    {
        GT_NONE = -1,
        GT_BOX = 0,
        GT_SPHERE = 1,
        GT_CAPSULE = 2,
        GT_MESH = 3,
        GT_ELLIPSOID = 4,
        GT_CONVEXHULL = 5,
        GT_PYRAMID = 6
    };

    typedef std::map<Ogre::String, Ogre::String> MeshPartMap;
}

#endif // __CoreConstants_H__
