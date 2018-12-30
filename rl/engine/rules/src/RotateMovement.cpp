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

#include "PhysicalThing.h"
#include "RotateMovement.h"

using namespace std;
using namespace Ogre;

namespace rl
{
    RotateMovement::RotateMovement(CreatureController* creature)
        : AbstractMovement(creature)
        , mYaw(0)
    {
        mRotLinearSpringK = 600.0f;
        Real relationCoefficient = 1.0f;
        mRotLinearDampingK = relationCoefficient * 2.0f * Math::Sqrt(mRotLinearSpringK);
    }

    bool RotateMovement::calculateBaseVelocity(Real& velocity)
    {
        velocity = 0.3f;
        return isPossible();
    }

    bool RotateMovement::isPossible() const
    {
        return mMovingCreature->getAbstractLocation() == CreatureController::AL_FLOOR
            && mMovingCreature->getCreature()->getAu() > 1
            && !(mMovingCreature->getCreature()->getLifeState() & (Effect::LS_IMMOBILE));
    }

    void RotateMovement::activate()
    {
        AbstractMovement::activate();
        mYaw = mMovingCreature->getCreature()->getActor()->getWorldOrientation().getYaw();
    }

    void RotateMovement::deactivate()
    {
        AbstractMovement::deactivate();
    }

    void RotateMovement::calculateForceAndTorque(Vector3& force, Vector3& torque, Real timestep)
    {
        Real mass;
        Vector3 inertia;
        OgreNewt::Body* body = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody();
        body->getMassMatrix(mass, inertia);

        Quaternion orientation;
        Vector3 position;
        body->getPositionOrientation(position, orientation);
        // Calculate angular velocity
        // We first need the yaw rotation from actual yaw to desired yaw
        Vector3 src = orientation * Vector3::UNIT_Z;
        src.y = 0;
        Vector3 dst = Quaternion(mYaw, Vector3::UNIT_Y) * Vector3::UNIT_Z;
        dst.y = 0;
        Quaternion quatDiff = src.getRotationTo(dst, Vector3::UNIT_Y);
        quatDiff.normalise();
        Radian yaw = quatDiff.getYaw();

        // using a spring system to apply the rotation
        Vector3 diff = Vector3(0, yaw.valueRadians(), 0);
        Vector3 omega = body->getOmega();
        omega.x = omega.z = 0;
        Vector3 springAcc = mRotLinearSpringK * diff - mRotLinearDampingK * omega;

        torque = springAcc * inertia;
    }

    bool RotateMovement::run(Ogre::Real elapsedTime, Ogre::Vector3 direction, Ogre::Vector3 rotation)
    {
        mYaw += Radian(rotation.y);
        while (mYaw.valueDegrees() > 360.0f)
            mYaw -= Degree(360.0f);
        while (mYaw.valueDegrees() < -360.0f)
            mYaw += Degree(360.0f);

        OgreNewt::Body* body = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody();
        Quaternion orientation;
        Vector3 position;
        body->getPositionOrientation(position, orientation);

        Real diff = (orientation.getYaw() - mYaw).valueDegrees();
        return diff <= 0.5f && diff >= -0.5f;
    }

    bool RotateMovement::isDirectionPossible(Ogre::Vector3& direction) const
    {
        Vector3 oldDirection(direction);
        direction = Vector3::ZERO;
        return oldDirection == Vector3::ZERO;
    }

    bool RotateMovement::isRotationPossible(Ogre::Vector3& rotation) const
    {
        Vector3 oldRotation(rotation);
        rotation.x = rotation.z = 0;
        return oldRotation.x == 0 && oldRotation.z == 0;
    }
}
