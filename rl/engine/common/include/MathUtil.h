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

#ifndef __MathUtil_H__
#define __MathUtil_H__

#include "CommonPrerequisites.h"

namespace rl
{

    class _RlCommonExport MathUtil
    {
    public:
        static const Ogre::Real EPSILON;

        static Ogre::Vector3 sphericalToCartesian(Ogre::Real r, Ogre::Radian azimuth, Ogre::Radian altitude);

        static void cartesianToSpherical(
            Ogre::Vector3 cartesian, Ogre::Real& r, Ogre::Radian& azimuth, Ogre::Radian& altitude);
        static Ogre::Real normaliseAngle(Ogre::Real angle);

        static Ogre::Real distance(const Ogre::AxisAlignedBox& b1, const Ogre::AxisAlignedBox& b2);
        static Ogre::Real distance(const Ogre::Vector3& v, const Ogre::AxisAlignedBox& b);
        static Ogre::Real distance(const Ogre::AxisAlignedBox& b, const Ogre::Vector3& v);
    };
}
#endif
