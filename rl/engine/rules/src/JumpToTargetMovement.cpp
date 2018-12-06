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

#include "JumpToTargetMovement.h"

using namespace std;
using namespace Ogre;



namespace rl
{
    JumpToTargetMovement::JumpToTargetMovement(CreatureController *creature) : 
        JumpLongMovement(creature) 
    {
    }

    bool JumpToTargetMovement::calculateBaseVelocity(Real &velocity)
    {
        // Code from Weitsprung but do not jump as far
        velocity = (mMovingCreature->getCreature()->getEigenschaft("GE") +
                mMovingCreature->getCreature()->getEigenschaft("KK")) / 5.0;
        velocity*=0.8f;

        // steht nicht in den Regeln aber finde ich sinnvoll
        // velocityBase *= (1 - getrageneLast/KK);
        // steht in den Regeln: pro Ersch�pfung ein KK abziehen
        //if( mErschoepfung > getEigenschaft("KO") )
        //    velocity -= (mErschoepfung - getEigenschaft("KO")) / 5.0;
        // steht nicht in den Regeln, aber finde ich sinnvoll
        //if( getAu() < getAuBasis() / 3.0 )
        //    velocity -= (getAu() / getAuBasis() * 3.0) * getEigenschaft("GE") / 5.0;
        return isPossible();
    }

    void JumpToTargetMovement::activate()
    {
        // Code from Weitsprung without Talentprobe
        AbstractMovement::activate();
        mState = DOWNTOUP;
        mMovingCreature->setAnimation(mAnimationAbsprung.first,mAnimationAbsprung.second,1,mAnimationForCollision.first);
        mTimer = 0;
        calculateBaseVelocity(mWidth);


        // the person will only achieve this width if it is running
        // retrieve run movement
        AbstractMovement *run = mMovingCreature->getMovementFromId(CreatureController::MT_RENNEN);
        if( run != NULL )
        {
            Real vel(0);
            run->calculateBaseVelocity(vel);
            Real factor = -mMovingCreature->getVelocity().z / vel;
            factor = std::max(Real(0),factor);
            // without moving before, the width will be 1/3
            mWidth = mWidth/3. + mWidth * 2./3. * factor;
        }

        mMovingCreature->getCreature()->damageAu(1./3); // not as much AU as Weitsprung
    }

    bool JumpToTargetMovement::run(Ogre::Real elapsedTime,  Ogre::Vector3 direction, Ogre::Vector3 rotation)
    {
        // this is the main difference to Weitsprung
        // use direction as target in local coordinates (only in -z - direction!), so direction.z is -mWidth
        // TODO: Talentprobe
        LOG_MESSAGE(Logger::RULES, "Zielsprung aktiviert!");
        std::ostringstream oss;
        oss << "Alte Sprungweite: " << mWidth << "Zielweite: " << -direction.z;
        if( direction.z < 0 && direction.z > -mWidth )
        {
            mWidth = - direction.z;
            oss << "Zielweite: " << mWidth;
        }
        LOG_MESSAGE(Logger::RULES, oss.str());

        return JumpLongMovement::run(elapsedTime, direction, rotation);
    }
}

