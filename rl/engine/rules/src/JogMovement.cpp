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

#include "JogMovement.h"
#include "MeshAnimation.h"


using namespace std;
using namespace Ogre;


const Real minSquaredSpeed = 0.6f;


namespace rl
{
    JogMovement::JogMovement(CreatureController *creature) : WalkMovement(creature)
    {
        mAnim = creature->getCreature()->getAnimation("joggen");
    }
        
    bool JogMovement::calculateBaseVelocity(Real &velocity)
    {
        velocity = mMovingCreature->getCurrentGS() / 2.0f;
        return isPossible();
    }
    
    bool JogMovement::isPossible() const
    {
        return WalkMovement::isPossible() &&
            mMovingCreature->getCreature()->getAu() > 6;
    }

    void JogMovement::applyAuChanges(Ogre::Real elapsedTime)
    {
        if( mMovingCreature->getVelocity().squaredLength() < minSquaredSpeed )
            StandAroundMovement::applyAuChanges(elapsedTime);
    }
    
    void JogMovement::setAnimation(Ogre::Real elapsedTime)
    {
        if( mMovingCreature->getVelocity().squaredLength() < minSquaredSpeed)
        {
            StandAroundMovement::setAnimation(elapsedTime);
            return;
        }

        Real length = 5./3.;
        Real weight = 1;
        Real relTimeOffset = 0;


        switch( mMovingCreature->getLastMovementType() )
        {
            case CreatureController::MT_NONE:
                break;
            case CreatureController::MT_STEHEN:
                relTimeOffset = 0.25;
                break;
            case CreatureController::MT_GEHEN:
                relTimeOffset = mMovingCreature->getAnimationTimePlayed();
                if( mMovingCreature->getLastMovementChange() < 0.5f )
                    weight = mMovingCreature->getLastMovementChange() / 0.5f;
                break;
            default:
                break;
        }

        // apply relTimeOffset only once
        if( mMovingCreature->getLastMovementChange() > elapsedTime )
            relTimeOffset = 0;

        MeshAnimation *meshAnim = mMovingCreature->setAnimation(mAnim.first, -mMovingCreature->getVelocity().z * mAnim.second, 0, mAnim.first, weight );
        if( meshAnim != NULL && relTimeOffset != 0)
            meshAnim->doAddTime(relTimeOffset*meshAnim->getLength());
    }

}

