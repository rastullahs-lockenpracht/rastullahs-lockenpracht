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

#include "LieDownMovement.h"

using namespace std;
using namespace Ogre;




namespace rl
{
    // auch wenn Fallen nicht moeglich ist (auf dem Boden und bewusstlos / Tod)
    LieDownMovement::LieDownMovement(CreatureController *creature) :
        AbstractMovement(creature)
    {
        mAnim = mMovingCreature->getCreature()->getAnimation("liegen");
        mAnim1 = mMovingCreature->getCreature()->getAnimation("sterben");
    }

    void LieDownMovement::activate()
    {
        LOG_MESSAGE(Logger::RULES, "Creature '"+mMovingCreature->getCreature()->getName()+"' liegt (evt bewusstlos) auf dem Boden.");
    }

    void LieDownMovement::deactivate()
    {
    }
    
    bool LieDownMovement::calculateBaseVelocity(Real &velocity)
    {
        velocity = 0.0f;
        return isPossible();
    }

    bool LieDownMovement::isPossible() const
    {
        return true;
    }

    void LieDownMovement::calculateForceAndTorque(Vector3 &force, Vector3 &torque, Real timestep)
    {
    }

    bool LieDownMovement::run(Ogre::Real elapsedTime,  Ogre::Vector3 direction, Ogre::Vector3 rotation)
    {
        // todo: wieder aufstehen
        if( mMovingCreature->getCreature()->getLifeState() & (Effect::LS_DEAD | Effect::LS_UNCONSCIOUS) )
            mMovingCreature->setAnimation(mAnim1.first, mAnim1.second, 1);
        else
            mMovingCreature->setAnimation(mAnim.first, mAnim.second, 1);
        return true;
    }

    void LieDownMovement::applyAuChanges(Ogre::Real elapsedTime)
    {
    }

    bool LieDownMovement::isDirectionPossible(Ogre::Vector3 &direction) const
    {
        direction = Vector3::ZERO;
        return false;
    }

    bool LieDownMovement::isRotationPossible(Ogre::Vector3 &rotation) const
    {
        rotation = Vector3::ZERO;
        return false;
    }
}

