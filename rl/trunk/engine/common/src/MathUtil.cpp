/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#include "MathUtil.h"

#include <OgreMath.h>

using namespace Ogre;

namespace rl
{
    const Ogre::Real MathUtil::EPSILON = 0.00001f;

    Ogre::Vector3 MathUtil::sphericalToCartesian(Ogre::Real r,
        Ogre::Radian azimuth, Ogre::Radian altitude)
    {
        Vector3 rval;
        rval.x = r*Math::Sin(azimuth)*Math::Sin(altitude + Radian(Math::HALF_PI));
        rval.y = r*Math::Cos(altitude + Radian(Math::HALF_PI));
        rval.z = r*Math::Cos(azimuth)*Math::Sin(altitude + Radian(Math::HALF_PI));
        return rval;
    }

    void MathUtil::cartesianToSpherical(Ogre::Vector3 cartesian, Ogre::Real& r,
        Ogre::Radian& azimuth, Ogre::Radian& altitude)
    {
        r = Math::Sqrt(Math::Sqr(cartesian.x)*Math::Sqr(cartesian.y)*Math::Sqr(cartesian.z));
        azimuth = Math::ATan2(cartesian.x, cartesian.z);
        altitude = Math::ACos(cartesian.y/r) - Radian(Math::HALF_PI);
    }

    Ogre::Real MathUtil::normaliseAngle(Ogre::Real angle)
	{
        Ogre::Real ang = angle;

		while (ang < 0) ang += 360;
		while (ang > 360) ang -= 360;

		return ang;
	}	
}
