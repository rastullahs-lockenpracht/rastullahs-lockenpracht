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

#ifndef __RotateMovement_H__
#define __RotateMovement_H__

#include "AbstractMovement.h"


namespace rl
{
    class RotateMovement : public AbstractMovement
    {
    public:
        RotateMovement(CreatureController *creature);
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_DREHEN;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_NONE;}
        virtual bool calculateBaseVelocity(Ogre::Real &velocity);
        virtual bool isPossible() const;
        virtual void activate();
        virtual void deactivate();
        virtual void calculateForceAndTorque(Ogre::Vector3 &force, Ogre::Vector3 &torque, Ogre::Real timestep);
        virtual bool run(Ogre::Real elapsedTime,  Ogre::Vector3 direction, Ogre::Vector3 rotation);
        virtual bool isDirectionPossible(Ogre::Vector3 &direction) const;
        virtual bool isRotationPossible(Ogre::Vector3 &rotation) const;
    protected:
        Ogre::Radian mYaw;
        Ogre::Real mRotLinearDampingK, mRotLinearSpringK;
        virtual Ogre::Real getMovementDefinedValue() {return mYaw.valueRadians();}
    };
}

#endif

