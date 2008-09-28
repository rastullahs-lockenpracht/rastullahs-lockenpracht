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

#ifndef __StandAroundMovement_H__
#define __StandAroundMovement_H__

#include "AbstractMovement.h"


namespace rl
{
    class StandAroundMovement : public AbstractMovement
    {
    public:
        StandAroundMovement(CreatureController *creature);
        virtual ~StandAroundMovement();
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_STEHEN;}
        virtual CreatureController::MovementType getFallBackMovement() const
        {
            if( mMovingCreature->getAbstractLocation() == CreatureController::AL_FLOOR )
                return CreatureController::MT_LIEGEN;

            return CreatureController::MT_FALLEN;
        }
        virtual void activate();
        virtual void deactivate();
        virtual bool calculateBaseVelocity(Ogre::Real &velocity);
        virtual bool isPossible() const;
        virtual void calculateForceAndTorque(Ogre::Vector3 &force, Ogre::Vector3 &torque, Ogre::Real timestep);
        virtual bool run(Ogre::Real elapsedTime,  Ogre::Vector3 direction, Ogre::Vector3 rotation);
        virtual void setAnimation(Ogre::Real elapsedTime);
        virtual void applyAuChanges(Ogre::Real elapsedTime);
        virtual bool isDirectionPossible(Ogre::Vector3 &direction) const;
        virtual bool isRotationPossible(Ogre::Vector3 &rotation) const;
    protected:
        Ogre::Vector3 mVelocity;
        Creature::AnimationSpeedPair mAnimStehenRechts;
        Creature::AnimationSpeedPair mAnimStehenLinks;
        Creature::AnimationSpeedPair mAnimStehen;
        mutable AbstractMovement *mRotationMovement;
        mutable AbstractMovement *mStepRecognitionMovement;
        virtual AbstractMovement* getRotationMovement() const
        {
            if( mRotationMovement == NULL)
            {
                mRotationMovement = mMovingCreature->getMovementFromId(CreatureController::MT_DREHEN);
            }
            if( mRotationMovement == NULL )
            {
                Throw(NullPointerException, "Konnte Movement mit der Id MT_STEHEN_DREHEN nicht finden.");
            }
            return mRotationMovement;
        }
        virtual AbstractMovement* getStepRecognitionMovement() const
        {
            if( mStepRecognitionMovement == NULL )
            {
                mStepRecognitionMovement = mMovingCreature->getMovementFromId(CreatureController::MT_STUFENERKENNUNG);
            }
            if( mStepRecognitionMovement == NULL )
            {
                Throw(NullPointerException, "Konnte Movement mit der Id MT_STUFENERKENNUNG nicht finden.");
            }
            return mStepRecognitionMovement;
        }
    };
}

#endif

