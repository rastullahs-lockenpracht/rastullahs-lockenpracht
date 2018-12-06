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
//
// LocalSpace: a local coordinate system for 3d space
//
// Provide functionality such as transforming from local space to global
// space and vice versa.  Also regenerates a valid space from a perturbed
// "forward vector" which is the basis of abnstract vehicle turning.
//
// These are comparable to a 4x4 homogeneous transformation matrix where the
// 3x3 (R) portion is constrained to be a pure rotation (no shear or scale).
// The rows of the 3x3 R matrix are the basis vectors of the space.  They are
// all constrained to be mutually perpendicular and of unit length.  The top
// ("x") row is called "side", the middle ("y") row is called "up" and the
// bottom ("z") row is called forward.  The translation vector is called
// "position".  Finally the "homogeneous column" is always [0 0 0 1].
//
//     [ R R R  0 ]      [ Sx Sy Sz  0 ]
//     [ R R R  0 ]      [ Ux Uy Uz  0 ]
//     [ R R R  0 ]  ->  [ Fx Fy Fz  0 ]
//     [          ]      [             ]
//     [ T T T  1 ]      [ Tx Ty Tz  1 ]
//
// This file defines three classes:
//   AbstractLocalSpace:  pure virtual interface
//   LocalSpaceMixin:     mixin to layer LocalSpace functionality on any base
//   LocalSpace:          a concrete object (can be instantiated)
//
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 06-05-02 cwr: created 
//
//
// ----------------------------------------------------------------------------


#ifndef OPENSTEER_LOCALSPACE_H
#define OPENSTEER_LOCALSPACE_H


#include "OpenSteer/Vec3.h"


// ----------------------------------------------------------------------------


namespace OpenSteer {


    class AbstractLocalSpace
    {
    public:
        virtual ~AbstractLocalSpace() { /* Nothing to do. */ }
        

        // accessors (get and set) for side, up, forward and position
        virtual Vector3 getSide (void) const = 0;
        virtual Vector3 setSide (Vector3 s) = 0;
        virtual Vector3 getUp (void) const = 0;
        virtual Vector3 setUp (Vector3 u) = 0;
        virtual Vector3 getForward (void) const = 0;
        virtual Vector3 setForward (Vector3 f) = 0;
        virtual Vector3 getPosition (void) const = 0;
        virtual Vector3 setPosition (Vector3 p) = 0;

        // use right-(or left-)handed coordinate space
        virtual bool isRightHanded(void) const = 0;

        // reset transform to identity
        virtual void resetLocalSpace (void) = 0;

        // transform a direction in global space to its equivalent in local space
        virtual Vector3 localizeDirection (const Vector3& globalDirection) const = 0;

        // transform a point in global space to its equivalent in local space
        virtual Vector3 localizePosition (const Vector3& globalPosition) const = 0;

        // transform a point in local space to its equivalent in global space
        virtual Vector3 globalizePosition (const Vector3& localPosition) const = 0;

        // transform a direction in local space to its equivalent in global space
        virtual Vector3 globalizeDirection (const Vector3& localDirection) const = 0;

        // set "side" basis vector to normalized cross product of forward and up
        virtual void setUnitSideFromForwardAndUp (void) = 0;

        // regenerate the orthonormal basis vectors given a new forward
        // (which is expected to have unit length)
        virtual void regenerateOrthonormalBasisUF (const Vector3& newUnitForward) = 0;

        // for when the new forward is NOT of unit length
        virtual void regenerateOrthonormalBasis (const Vector3& newForward) = 0;

        // for supplying both a new forward and and new up
        virtual void regenerateOrthonormalBasis (const Vector3& newForward,
                                                 const Vector3& newUp) = 0;

        // rotate 90 degrees in the direction implied by isRightHanded()
        virtual Vector3 localRotateForwardToSide (const Vector3& v) const = 0;
        virtual Vector3 globalRotateForwardToSide (const Vector3& globalForward) const=0;
    };


    // ----------------------------------------------------------------------------
    // LocalSpaceMixin is a mixin layer, a class template with a paramterized base
    // class.  Allows "LocalSpace-ness" to be layered on any class.


    template <class Super>
    class LocalSpaceMixin : public Super
    {
        // transformation as three orthonormal unit basis vectors and the
        // origin of the local space.  These correspond to the "rows" of
        // a 3x4 transformation matrix with [0 0 0 1] as the final column

    private:

        Vector3 _side;     //    side-pointing unit basis vector
        Vector3 _up;       //  upward-pointing unit basis vector
        Vector3 _forward;  // forward-pointing unit basis vector
        Vector3 _position; // origin of local space

    public:

        // accessors (get and set) for side, up, forward and position
        Vector3 getSide     (void) const {return _side;}
        Vector3 getUp       (void) const {return _up;}
        Vector3 getForward  (void) const {return _forward;}
        Vector3 getPosition (void) const {return _position;}
        Vector3 setSide     (Vector3 s) {return _side = s;}
        Vector3 setUp       (Vector3 u) {return _up = u;}
        Vector3 setForward  (Vector3 f) {return _forward = f;}
        Vector3 setPosition (Vector3 p) {return _position = p;}
        Vector3 setSide     (float x, float y, float z){return _side = Vector3(x,y,z);}
        Vector3 setUp       (float x, float y, float z){return _up = Vector3(x,y,z);}
        Vector3 setForward  (float x, float y, float z){return _forward = Vector3(x,y,z);}
        Vector3 setPosition (float x, float y, float z){return _position = Vector3(x,y,z);}


        // ------------------------------------------------------------------------
        // Global compile-time switch to control handedness/chirality: should
        // LocalSpace use a left- or right-handed coordinate system?  This can be
        // overloaded in derived types (e.g. vehicles) to change handedness.

        bool isRightHanded(void) const {return true;}


        // ------------------------------------------------------------------------
        // constructors


        LocalSpaceMixin (void)
        {
            resetLocalSpace ();
        };

        LocalSpaceMixin (const Vector3& Side,
                         const Vector3& Up,
                         const Vector3& Forward,
                         const Vector3& Position)
            : _side( Side ), _up( Up ), _forward( Forward ), _position( Position ) {}


        LocalSpaceMixin (const Vector3& Up,
                         const Vector3& Forward,
                         const Vector3& Position)
            : _side(), _up( Up ), _forward( Forward ), _position( Position )
        {
            setUnitSideFromForwardAndUp();
        }

        
        virtual ~LocalSpaceMixin() { /* Nothing to do. */ }
        

        // ------------------------------------------------------------------------
        // reset transform: set local space to its identity state, equivalent to a
        // 4x4 homogeneous transform like this:
        //
        //     [ X 0 0 0 ]
        //     [ 0 1 0 0 ]
        //     [ 0 0 1 0 ]
        //     [ 0 0 0 1 ]
        //
        // where X is 1 for a left-handed system and -1 for a right-handed system.

        void resetLocalSpace (void)
        {
            _forward = Vector3(0, 0, 1);
            _side = localRotateForwardToSide (_forward);
            _up = Vector3(0, 1, 0);
            _position = Vector3(0, 0, 0);
        };


        // ------------------------------------------------------------------------
        // transform a direction in global space to its equivalent in local space


        Vector3 localizeDirection (const Vector3& globalDirection) const
        {
            // dot offset with local basis vectors to obtain local coordiantes
            return Vector3 (globalDirection.dotProduct(_side),
                         globalDirection.dotProduct(_up),
                         globalDirection.dotProduct(_forward));
        };


        // ------------------------------------------------------------------------
        // transform a point in global space to its equivalent in local space


        Vector3 localizePosition (const Vector3& globalPosition) const
        {
            // global offset from local origin
            Vector3 globalOffset = globalPosition - _position;

            // dot offset with local basis vectors to obtain local coordiantes
            return localizeDirection (globalOffset);
        };


        // ------------------------------------------------------------------------
        // transform a point in local space to its equivalent in global space


        Vector3 globalizePosition (const Vector3& localPosition) const
        {
            return _position + globalizeDirection (localPosition);
        };


        // ------------------------------------------------------------------------
        // transform a direction in local space to its equivalent in global space


        Vector3 globalizeDirection (const Vector3& localDirection) const
        {
            return ((_side    * localDirection.x) +
                    (_up      * localDirection.y) +
                    (_forward * localDirection.z));
        };


        // ------------------------------------------------------------------------
        // set "side" basis vector to normalized cross product of forward and up


        void setUnitSideFromForwardAndUp (void)
        {
            // derive new unit side basis vector from forward and up
            if (isRightHanded())
                _side = _forward.crossProduct(_up);
            else
                _side = _up.crossProduct(_forward);
            _side = _side.normalisedCopy();
        }


        // ------------------------------------------------------------------------
        // regenerate the orthonormal basis vectors given a new forward
        // (which is expected to have unit length)


        void regenerateOrthonormalBasisUF (const Vector3& newUnitForward)
        {
            _forward = newUnitForward;

            // derive new side basis vector from NEW forward and OLD up
            setUnitSideFromForwardAndUp();

            // derive new Up basis vector from new Side and new Forward
            // (should have unit length since Side and Forward are
            // perpendicular and unit length)
            if (isRightHanded())
                _up = _side.crossProduct(_forward);
            else
                _up = _forward.crossProduct(_side);
        }


        // for when the new forward is NOT know to have unit length

        void regenerateOrthonormalBasis (const Vector3& newForward)
        {
            regenerateOrthonormalBasisUF (newForward.normalisedCopy());
        }


        // for supplying both a new forward and and new up

        void regenerateOrthonormalBasis (const Vector3& newForward,
                                         const Vector3& newUp)
        {
            _up = newUp;
            regenerateOrthonormalBasis (newForward.normalisedCopy());
        }


        // ------------------------------------------------------------------------
        // rotate, in the canonical direction, a vector pointing in the
        // "forward" (+Z) direction to the "side" (+/-X) direction


        Vector3 localRotateForwardToSide (const Vector3& v) const
        {
            return Vector3 (isRightHanded() ? -v.z : +v.z,
                         v.y,
                         v.x);
        }

        // not currently used, just added for completeness

        Vector3 globalRotateForwardToSide (const Vector3& globalForward) const
        {
            const Vector3 localForward = localizeDirection (globalForward);
            const Vector3 localSide = localRotateForwardToSide (localForward);
            return globalizeDirection (localSide);
        }
    };


    // ----------------------------------------------------------------------------
    // Concrete LocalSpace class, and a global constant for the identity transform


    typedef LocalSpaceMixin<AbstractLocalSpace> LocalSpace;

    const LocalSpace gGlobalSpace;

} // namespace OpenSteer

// ----------------------------------------------------------------------------
#endif // OPENSTEER_LOCALSPACE_H
