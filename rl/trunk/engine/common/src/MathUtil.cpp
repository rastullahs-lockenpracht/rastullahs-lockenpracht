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

#include "MathUtil.h"


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

	Real MathUtil::distance(const AxisAlignedBox& b1, const AxisAlignedBox& b2)
	{
		if (b1.intersects(b2))
		{
			return 0.0f;
		}
		else
		{
			Vector3 dv;

			const Vector3& min1 = b1.getMinimum();
			const Vector3& min2 = b2.getMinimum();
			const Vector3& max1 = b1.getMaximum();
			const Vector3& max2 = b2.getMaximum();

			dv.x = min1.x > max2.x ? min1.x - max2.x : min2.x > max1.x ? min2.x - max1.x : 0.0f;
			dv.y = min1.y > max2.y ? min1.y - max2.y : min2.y > max1.y ? min2.y - max1.y : 0.0f;
			dv.z = min1.z > max2.z ? min1.z - max2.z : min2.z > max1.z ? min2.z - max1.z : 0.0f;

			return dv.length();
		}
	}
}
