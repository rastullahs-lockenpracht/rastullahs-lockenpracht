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


#include "JogFastMovement.h"
#include "DsaManager.h"
#include "MeshAnimation.h"


using namespace std;
using namespace Ogre;


const Real minSquaredSpeed = 0.6f;

namespace rl
{
    JogFastMovement::JogFastMovement(CreatureController *creature) : WalkMovement(creature), mTimePerAu(1L), mLastProbe(0)
    {
        mAnim = creature->getCreature()->getAnimation("laufen");
    }
    
    
    
    bool JogFastMovement::calculateBaseVelocity(Real &velocity)
    {
        velocity = mMovingCreature->getCurrentGS() / 1.6f; // was 2.0
        return isPossible();
    }
    
    bool JogFastMovement::isPossible() const
    {
        if( mMovingCreature->getMovement() == this )
            return WalkMovement::isPossible() &&
                mMovingCreature->getCreature()->getAu() > 6;
        else
            return WalkMovement::isPossible() &&
                mMovingCreature->getCreature()->getAu() > 8;
    }


    
    bool JogFastMovement::run(Ogre::Real elapsedTime, Ogre::Vector3 direction, Ogre::Vector3 rotation)
    {
        doTalentProbeIfNecessary();
        return WalkMovement::run(elapsedTime, direction, rotation);
    }
        
    void JogFastMovement::applyAuChanges(Ogre::Real elapsedTime)
    {
        if( mMovingCreature->getVelocity().squaredLength()< minSquaredSpeed )
            StandAroundMovement::applyAuChanges(elapsedTime);
        else
            mMovingCreature->getCreature()->damageAu(elapsedTime/mTimePerAu);
    }

    
    void JogFastMovement::setAnimation(Ogre::Real elapsedTime)
    {
        if( mMovingCreature->getVelocity().squaredLength() < minSquaredSpeed )
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

    
    void JogFastMovement::activate()
    {
        WalkMovement::activate();
        doTalentProbeIfNecessary();
    }
    
    void JogFastMovement::doTalentProbeIfNecessary()
    {
        Date now = DsaManager::getSingleton().getCurrentDate();
        if( mLastProbe + Date::ONE_SPIELRUNDE >= now || mLastProbe == 0 )
        {
            mTimePerAu = 120; // was 180
            if( mMovingCreature->getCreature()->hasTalent("Athletik") )
            {
                mLastProbe = now;
                int taw = mMovingCreature->getCreature()->doTalentprobe("Athletik",0);
                if( taw == RESULT_AUTOERFOLG )
                {
                    mTimePerAu += mMovingCreature->getCreature()->getTalent("Athletik") * 5;
                }
                else if( taw == RESULT_SPEKT_AUTOERFOLG )
                {
                    mTimePerAu += mMovingCreature->getCreature()->getTalent("Athletik") * 10;
                }
                else
                {
                    mTimePerAu += taw * 5;
                }
            }
            else
            {
                mTimePerAu = 120; // was 180
            }
        }
    }
}

