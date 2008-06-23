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

#include "stdinc.h"
#include "AgentDialogState.h"

#include "Agent.h"
#include "AiMessages.h"
#include "Creature.h"
#include "CreatureController.h"
#include "CreatureControllerManager.h"
#include "MessagePump.h"

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

    void AgentDialogState::setDialog(Dialog* dialog)
    {
        mDialog = dialog;
    }

    void AgentDialogState::update(const Ogre::Real elapsedTime)
    {
        CreatureController* ctrl = 
                CreatureControllerManager::getSingleton().getCreatureController(
                    mAgent->getControlledCreature());

        if (mAgent->getPosition().squaredDistance(mPartner->getPosition()) > 1.5
            || !mAgent->isAhead(mPartner, 0.95))
        {		
            mAgent->addForce(mAgent->calcSeek(mPartner->getPosition()));
            mAgent->updateVehicle(0, elapsedTime);
        }
		else
        {
			if (!mTalking)
            {
                mAgent->reset();
                ctrl->setMovement(
                    CreatureController::MT_STEHEN, Vector3::ZERO, Vector3::ZERO);

                ctrl->setAnimation("reden");
				mTalking = true;

                MessagePump::getSingleton().sendMessage<MessageType_DialogStarted>(mDialog);
                
                mAgent->popState();
            }			
        }

    }
} // namespace rl
