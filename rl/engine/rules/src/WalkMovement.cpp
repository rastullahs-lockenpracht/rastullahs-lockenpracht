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

#include "WalkMovement.h"
#include "MeshAnimation.h"


using namespace std;
using namespace Ogre;




namespace rl
{
    WalkMovement::WalkMovement(CreatureController *creature) : StandAroundMovement(creature)
    {
        mAnim = creature->getCreature()->getAnimation("gehen");
    }

    bool WalkMovement::calculateBaseVelocity(Real &velocity)
    {
        velocity = mMovingCreature->getCurrentGS() / 3.6f * 0.7f;
        return isPossible();
    }

    bool WalkMovement::isPossible() const
    {
        return
            mMovingCreature->getAbstractLocation() == CreatureController::AL_FLOOR &&
            mMovingCreature->getCreature()->getAu() > 1 &&
            !(mMovingCreature->getCreature()->getLifeState() & Effect::LS_IMMOBILE);
    }

    bool WalkMovement::run(Ogre::Real elapsedTime, Ogre::Vector3 direction, Ogre::Vector3 rotation)
    {
        StandAroundMovement::run(elapsedTime, direction, rotation);
        return true;
    }

    bool WalkMovement::isDirectionPossible(Ogre::Vector3 &direction) const
    {
        Vector3 oldDirection(direction);
        direction.x = direction.y = 0;
        if( direction.z > 0 )
            direction.z = 0;
        return oldDirection.x == 0 && oldDirection.y == 0 && oldDirection.z > 0;
    }

    void WalkMovement::applyAuChanges(Ogre::Real elapsedTime)
    {
        mMovingCreature->getCreature()->regenerateAu(2, 0.5, elapsedTime);
    }

    void WalkMovement::setAnimation(Ogre::Real elapsedTime)
    {
        Real speed =  -mMovingCreature->getVelocity().z;
        if( fabs(speed) < 0.1 )
        {
            StandAroundMovement::setAnimation(elapsedTime);
            return;
        }

        Real weight = 1;
        Real relTimeOffset = 0;

        switch( mMovingCreature->getLastMovementType() )
        {
            case CreatureController::MT_NONE:
                break;
            case CreatureController::MT_STEHEN:
                relTimeOffset = 0.25;
                break;
            case CreatureController::MT_JOGGEN:
            case CreatureController::MT_LAUFEN:
            case CreatureController::MT_RENNEN:
                relTimeOffset = mMovingCreature->getAnimationTimePlayed();
                if( mMovingCreature->getLastMovementChange() < 1.0f )
                    weight = mMovingCreature->getLastMovementChange() / 1.0f;
                break;
            default:
                break;
        }


        // apply relTimeOffset only once
        if( mMovingCreature->getLastMovementChange() > elapsedTime )
            relTimeOffset = 0;

        MeshAnimation *meshAnim = mMovingCreature->setAnimation(mAnim.first, speed * mAnim.second, 0, mAnim.first, weight );
        if( meshAnim != NULL && relTimeOffset != 0 )
            meshAnim->doAddTime(relTimeOffset*meshAnim->getLength());
    }
}

