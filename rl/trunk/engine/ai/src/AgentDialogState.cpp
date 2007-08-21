/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#include "stdinc.h"
#include "AgentDialogState.h"

#include "Agent.h"
#include "Creature.h"
#include "CreatureController.h"
#include "CreatureControllerManager.h"

using namespace Ogre;

namespace rl
{

    AgentDialogState::AgentDialogState(Agent* agent)
        : AgentState(agent),
        mPartner(NULL),
        mTalking(false)
    {
    }

    AgentDialogState::~AgentDialogState()
    {
    }

    void AgentDialogState::setDialogPartner(Agent* partner)
    {
        mPartner = partner;
    }

    void AgentDialogState::update(const Ogre::Real elapsedTime)
    {
        static CreatureController* ctrl = 
                CreatureControllerManager::getSingleton().getCreatureController(
                    mAgent->getControlledCreature());

        if (mAgent->getPosition().squaredDistance(mPartner->getPosition()) > 1.5)
        {		
            mAgent->addForce(mAgent->calcPursuit(mPartner));
            mAgent->updateVehicle(0, elapsedTime);
        }
        //else if (!mAgent->isAhead(mPartner, 0.95)) 
        //{		
        //    mAgent->addForce(mAgent->calcPursuit(mPartner));
        //    mAgent->updateVehicle(0, elapsedTime);
        //}
        ///@todo: Turn if near but not correctly oriented
		else
        {
			if (!mTalking)
            {
                mAgent->reset();
                ctrl->setMovement(CreatureController::MT_NONE, Vector3::ZERO, Vector3::ZERO);
		        //$IM.pushControlState(CST_DIALOG);

                ctrl->setAnimation("reden");
				mTalking = true;
            }			
        }


    }
} // namespace rl
