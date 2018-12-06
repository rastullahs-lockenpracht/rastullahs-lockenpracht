// ----------------------------------------------------------------------------
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2005, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
// ----------------------------------------------------------------------------
//
// Vector3: OpenSteer's generic type for 3d vectors
//
// This file defines the class Vector3, which is used throughout OpenSteer to
// manipulate 3d geometric data.  It includes standard vector operations (like
// vector addition, subtraction, scale, dot, cross...) and more idiosyncratic
// utility functions.
//
// When integrating OpenSteer into a preexisting 3d application, it may be
// important to use the 3d vector type of that application.  In that case Vector3
// can be changed to inherit from the preexisting application' vector type and
// to match the interface used by OpenSteer to the interface provided by the
// preexisting 3d vector type.
//
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 03-26-03 cwr: created to replace for Hiranabe-san's execellent but larger
//               vecmath package (http://objectclub.esm.co.jp/vecmath/)
//
// ----------------------------------------------------------------------------

#ifndef OPENSTEER_Vector3_H
#define OPENSTEER_Vector3_H

#include "OpenSteer/Utilities.h" // for interpolate, etc.
#ifdef __APPLE__
#include <Carbon/Carbon.h>
#include <Ogre/Ogre.h>
#else
#include <Ogre.h>
#endif

using Ogre::Vector3;

namespace OpenSteer
{

    // ----------------------------------------------------------------------------
    class Vec3Utils
    {
    public:
        // return component of vector parallel to a unit basis vector
        // (IMPORTANT NOTE: assumes "basis" has unit magnitude (length==1))

        static Vector3 parallelComponent(const Vector3& vector, const Vector3& unitBasis)
        {
            const float projection = vector.dotProduct(unitBasis);
            return unitBasis * projection;
        }

        // return component of vector perpendicular to a unit basis vector
        // (IMPORTANT NOTE: assumes "basis" has unit magnitude (length==1))

        static Vector3 perpendicularComponent(const Vector3& vector, const Vector3& unitBasis)
        {
            return vector - parallelComponent(vector, unitBasis);
        }

        // clamps the length of a given vector to maxLength.  If the vector is
        // shorter its value is returned unaltered, if the vector is longer
        // the value returned has length of maxLength and is paralle to the
        // original input.

        static Vector3 truncateLength(const Vector3& vector, const float maxLength)
        {
            const float vecLength = vector.length();
            if (vecLength <= maxLength)
                return vector;
            else
                return vector * (maxLength / vecLength);
        }

        // forces a 3d position onto the XZ (aka y=0) plane

        static Vector3 setYtoZero(const Vector3& vector)
        {
            return Vector3(vector.x, 0, vector.z);
        }

        // rotate this vector about the global Y (up) axis by the given angle

        static Vector3 rotateAboutGlobalY(const Vector3& vector, float angle)
        {
            const float s = sinXXX(angle);
            const float c = cosXXX(angle);
            return Vector3((vector.x * c) + (vector.z * s), (vector.y), (vector.z * c) - (vector.x * s));
        }

        // version for caching sin/cos computation
        static Vector3 rotateAboutGlobalY(const Vector3& vector, float angle, float& sin, float& cos)
        {
            // is both are zero, they have not be initialized yet
            if (sin == 0 && cos == 0)
            {
                sin = sinXXX(angle);
                cos = cosXXX(angle);
            }
            return Vector3((vector.x * cos) + (vector.z * sin), (vector.y), (vector.z * cos) - (vector.x * sin));
        }

        // if this position is outside sphere, push it back in by one diameter

        static Vector3 sphericalWrapAround(const Vector3& vector, const Vector3& center, float radius)
        {
            const Vector3 offset = vector - center;
            const float r = offset.length();
            if (r > radius)
                return vector + ((offset / r) * radius * -2);
            else
                return vector;
        }
    };

    // ----------------------------------------------------------------------------
    // scalar times vector product ("float * Vector3")

    inline Vector3 operator*(float s, const Vector3& v)
    {
        return v * s;
    }

    // return cross product a x b
    inline Vector3 crossProduct(const Vector3& a, const Vector3& b)
    {
        Vector3 result((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
        return result;
    }

    // ----------------------------------------------------------------------------
    // Returns a position randomly distributed inside a sphere of unit radius
    // centered at the origin.  Orientation will be random and length will range
    // between 0 and 1

    Vector3 RandomVectorInUnitRadiusSphere(void);

    // ----------------------------------------------------------------------------
    // Returns a position randomly distributed on a disk of unit radius
    // on the XZ (Y=0) plane, centered at the origin.  Orientation will be
    // random and length will range between 0 and 1

    Vector3 randomVectorOnUnitRadiusXZDisk(void);

    // ----------------------------------------------------------------------------
    // Returns a position randomly distributed on the surface of a sphere
    // of unit radius centered at the origin.  Orientation will be random
    // and length will be 1

    inline Vector3 RandomUnitVector(void)
    {
        return RandomVectorInUnitRadiusSphere().normalisedCopy();
    }

    // ----------------------------------------------------------------------------
    // Returns a position randomly distributed on a circle of unit radius
    // on the XZ (Y=0) plane, centered at the origin.  Orientation will be
    // random and length will be 1

    inline Vector3 RandomUnitVectorOnXZPlane(void)
    {
        return Vec3Utils::setYtoZero(RandomVectorInUnitRadiusSphere()).normalisedCopy();
    }

    // ----------------------------------------------------------------------------
    // used by limitMaxDeviationAngle / limitMinDeviationAngle below

    Vector3 vecLimitDeviationAngleUtility(
        const bool insideOrOutside, const Vector3& source, const float cosineOfConeAngle, const Vector3& basis);

    // ----------------------------------------------------------------------------
    // Enforce an upper bound on the angle by which a given arbitrary vector
    // diviates from a given reference direction (specified by a unit basis
    // vector).  The effect is to clip the "source" vector to be inside a cone
    // defined by the basis and an angle.

    inline Vector3 limitMaxDeviationAngle(const Vector3& source, const float cosineOfConeAngle, const Vector3& basis)
    {
        return vecLimitDeviationAngleUtility(true, // force source INSIDE cone
            source, cosineOfConeAngle, basis);
    }

    // ----------------------------------------------------------------------------
    // Enforce a lower bound on the angle by which a given arbitrary vector
    // diviates from a given reference direction (specified by a unit basis
    // vector).  The effect is to clip the "source" vector to be outside a cone
    // defined by the basis and an angle.

    inline Vector3 limitMinDeviationAngle(const Vector3& source, const float cosineOfConeAngle, const Vector3& basis)
    {
        return vecLimitDeviationAngleUtility(false, // force source OUTSIDE cone
            source, cosineOfConeAngle, basis);
    }

    // ----------------------------------------------------------------------------
    // Returns the distance between a point and a line.  The line is defined in
    // terms of a point on the line ("lineOrigin") and a UNIT vector parallel to
    // the line ("lineUnitTangent")

    inline float distanceFromLine(const Vector3& point, const Vector3& lineOrigin, const Vector3& lineUnitTangent)
    {
        const Vector3 offset = point - lineOrigin;
        const Vector3 perp = Vec3Utils::perpendicularComponent(offset, lineUnitTangent);
        return perp.length();
    }

    // ----------------------------------------------------------------------------
    // given a vector, return a vector perpendicular to it (note that this
    // arbitrarily selects one of the infinitude of perpendicular vectors)

    Vector3 findPerpendicularIn3d(const Vector3& direction);

    // ----------------------------------------------------------------------------
    // candidates for global utility functions
    //
    // dot
    // cross
    // length
    // distance
    // normalized

} // namespace OpenSteer

// ----------------------------------------------------------------------------
#endif // OPENSTEER_Vector3_H
