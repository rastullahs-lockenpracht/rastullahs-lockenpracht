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

#include "AgentDialogState.h"
#include "stdinc.h"

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
        : AgentState(agent)
        , mTalking(false)
        , mDialog(NULL)
    {
    }

    AgentDialogState::~AgentDialogState()
    {
    }

    void AgentDialogState::addDialogPartner(Agent* partner)
    {
        mPartners.push_back(partner);
    }

    void AgentDialogState::setDialog(Dialog* dialog)
    {
        mDialog = dialog;
    }

    void AgentDialogState::update(const Ogre::Real elapsedTime)
    {
        CreatureController* ctrl
            = CreatureControllerManager::getSingleton().getCreatureController(mAgent->getControlledCreature());

        Vector3 partnerPos(Vector3::ZERO);
        if (!mPartners.empty())
        {
            for (std::list<Agent*>::iterator it = mPartners.begin(), end = mPartners.end(); it != end; ++it)
            {
                partnerPos += (*it)->getPosition();
            }
            partnerPos /= mPartners.size();
        }

        if (!mPartners.empty()
            && (mAgent->getPosition().squaredDistance(partnerPos) > 1.5 || !mAgent->isAhead(partnerPos, 0.95)))
        {
            mAgent->addForce(mAgent->calcSeek(partnerPos));
            mAgent->updateVehicle(0, elapsedTime);
        }
        else
        {
            if (!mTalking && mDialog)
            {
                mAgent->reset();
                ctrl->setMovement(CreatureController::MT_STEHEN, Vector3::ZERO, Vector3::ZERO);

                ctrl->setAnimation("reden");
                mTalking = true;

                MessagePump::getSingleton().sendMessage<MessageType_DialogStarted>(mDialog);

                mAgent->popState();
            }
        }
    }
} // namespace rl
