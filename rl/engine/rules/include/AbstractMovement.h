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


#ifndef __AbstractMovement_H__
#define __AbstractMovement_H__

#include "RulesPrerequisites.h"
#include "CreatureController.h"



namespace rl
{
    /// this is the base class of all movements and provides a general interface for movements
    class AbstractMovement // : public PhysicsGenericContactCallback
    {
    public:
        AbstractMovement(CreatureController *movingCreature) : mMovingCreature(movingCreature), mActive(false) {}
        virtual ~AbstractMovement() {}

        /// returns the id of this movement
        virtual CreatureController::MovementType getId() const = 0;

        /// returns the id of the movement that is used, if this movement isn't possible (any more)
        virtual CreatureController::MovementType getFallBackMovement() const = 0;

        /**
         * this method calculates the basis velocity (without any changes due to Talentproben etc)
         * @param velocity this parameter returns the calculated "velocity" ( m/s, rpm, or width/height of a jump)
         * @retval false indicates, that this movement isn't possible, so the calculated values can be incorrect
         */
        virtual bool calculateBaseVelocity(Ogre::Real &velocity) = 0;

        /**
         * this method specifies, if the movement is possible
         */
        virtual bool isPossible() const = 0;

        /**
         * this method is called, when this movement is currently used to enable 
         * an individual procession of collisions contacts for each movement
         */
        virtual void userProcess(OgreNewt::ContactJoint &contactJoint, Ogre::Real timestep, int threadid) {}

        /**
         * this method is called by OnApplyTorqueAndForceCallback of the CreatureController
         * the PhysicalThing can be acquired via the Creature
         */
        virtual void calculateForceAndTorque(Ogre::Vector3 &force, Ogre::Vector3 &torque, Ogre::Real timestep) = 0;

        /**
         * this method indicates if it is possible to change to the specified movement at this moment
         */
        virtual bool canChangeToMovement(CreatureController::MovementType id) {return true;}

        /**
         * this signifies that someone tried to change to another movement, but this was not possible (due to canChangeToMovement returning false)
         */
        virtual void requestChangeToMovement(CreatureController::MovementType id) {}

        /**
         * here all the stuff not (directly) relating the physics should be done
         * this function is called every frame if this movement is activated and the
         * Creature is in an active moving state. It is important to handle all 
         * "Talentproben" etc here and not in the physics function!
         * The parameter direction an rotation don't need to obey the rules from isDirectionPossible and isRotationPossible
         * @retval true indicates that the creature should remain active (in order to activate it use CreatureControllerManager::setActive)
         */
        virtual bool run(Ogre::Real elapsedTime,  Ogre::Vector3 direction, Ogre::Vector3 rotation) = 0;

        /**
         * this function is called if this movement is activated.
         * it can be used to determine if a new "Talentprobe" is needed
         * if you ovveride it, please make sure to call the base class method
         */
        virtual void activate() {mActive = true;}

        /**
         * this function is called every time this movement is deactivated.
         * if you ovveride it, please make sure to call the base class method
         */
        virtual void deactivate() {mActive = false;}

        /**
         * this method is used to get to know if a movement can be used with a certain direction
         * @retval true if this movement can be used with the direction, false otherwise
         * @param direction (in local axes); if this function returns false, this variable contains another valid direction (constructed from the given direction)
         */
        virtual bool isDirectionPossible(Ogre::Vector3 &direction) const = 0;

        /**
         * this method is used to get to know if the given rotation can be used with this movement
         * @retval true if the creature can perform this rotation with this movement
         * @param rotation (in local axes and rpm); if the function returns false, this variable contains another valid rotation (constructed from the given rotation)
         */
        virtual bool isRotationPossible(Ogre::Vector3 &rotation) const = 0;


        /// this can return true even though movingcreature doesn't call this movement, but another that is using this movement
        bool isActive() const {return mActive;}

        /// this method is used to return the yaw value of rotation movemenrts etc
        virtual Ogre::Real getMovementDefinedValue() {return 0;}
    protected:
        CreatureController *mMovingCreature;
        bool mActive;
    };
}
#endif

