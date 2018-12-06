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

#include "Agent.h"

#include "Actor.h"
#include "AgentCombatState.h"
#include "AgentDialogState.h"
#include "AgentSteeringState.h"
#include "Creature.h"
#include "Exception.h"
#include "ScriptWrapper.h"
#include "SteeringMachine.h"
#include "SteeringVehicle.h"

using namespace Ogre;
using namespace OpenSteer;

namespace rl
{

    Agent::Agent(Creature* character)
        : SteeringVehicle(character)
        , mAgentStates()
    {
        initialize();
        ScriptWrapper::getSingleton().owned(character);
    }

    Agent::~Agent(void)
    {
        if (mCreature != NULL)
        {
            ScriptWrapper::getSingleton().disowned(mCreature);
        }
    }

    void Agent::initialize()
    {
        //  an agent needs a creature it refers to
        if (mCreature == NULL)
        {
            Throw(NullPointerException, "Agent has no creature");
        }
        // We need a state to start with. Per default the Agent is steering.
        pushState(AST_STEERING);
    }

    void Agent::update(const float elapsedTime)
    {
        if (!mAgentStates.empty())
        {
            mAgentStates.top()->update(elapsedTime);
        }
    }

    Creature* Agent::getControlledCreature() const
    {
        return mCreature;
    }

    void Agent::pushState(AgentStateType stateType)
    {
        AgentState* state = NULL;
        if (stateType == AST_STEERING)
        {
            state = new AgentSteeringState(this);
        }
        else if (stateType == AST_COMBAT)
        {
            state = new AgentCombatState(this);
        }
        else if (stateType == AST_DIALOG)
        {
            state = new AgentDialogState(this);
        }
        else
        {
            Throw(IllegalArgumentException, "Unknown AgentStateType");
        }

        mAgentStates.push(state);
    }

    void Agent::popState()
    {
        AgentState* cur = mAgentStates.top();
        mAgentStates.pop();
        delete cur;
    }

    AgentState* Agent::getCurrentState() const
    {
        if (mAgentStates.empty())
        {
            return NULL;
        }
        else
        {
            return mAgentStates.top();
        }
    }

    void Agent::updateVehicle(const float currentTime, const float elapsedTime)
    {
        SteeringVehicle::update(currentTime, elapsedTime);
    }
}
