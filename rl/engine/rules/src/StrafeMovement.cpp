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

#include "StrafeMovement.h"

using namespace std;
using namespace Ogre;




namespace rl
{
    StrafeMovement::StrafeMovement(CreatureController *creature) : WalkMovement(creature), mLeft(true)
    {
        mAnim1 = creature->getCreature()->getAnimation("seitwaerts_rechts");
        mAnim = creature->getCreature()->getAnimation("seitwaerts_links");
    }

    bool StrafeMovement::calculateBaseVelocity(Real &velocity)
    {
        velocity = mMovingCreature->getCurrentGS() / 4.0;
        return isPossible();
    }
    
    bool StrafeMovement::isDirectionPossible(Ogre::Vector3 &direction) const
    {
        Vector3 oldDirection(direction);
        direction.z = direction.y = 0;
        return oldDirection.z == 0 && oldDirection.y == 0;
    }

    bool StrafeMovement::run(Ogre::Real elapsedTime, Ogre::Vector3 direction, Ogre::Vector3 rotation)
    {
        mLeft = direction.x < 0;
        return WalkMovement::run(elapsedTime, direction, rotation);
    }

    void StrafeMovement::setAnimation(Ogre::Real elapsedTime)
    {
        if( mLeft )
            mMovingCreature->setAnimation(mAnim1.first, mAnim1.second * -mMovingCreature->getVelocity().x);
        else
            mMovingCreature->setAnimation(mAnim.first, mAnim.second * -mMovingCreature->getVelocity().x);
    }
}


