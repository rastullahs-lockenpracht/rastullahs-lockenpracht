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
#include "AgentSteeringState.h"

using namespace Ogre;
using namespace OpenSteer;

namespace rl {


    AgentSteeringState::AgentSteeringState(Agent* agent)
        : AgentState(agent),
        mBehaviour(NULL)
    {
        mBehaviour = new SteeringMachine(NULL, mAgent);
        LOG_MESSAGE(Logger::AI, 
            "created SteeringMachine for Agent");
    }

    AgentSteeringState::~AgentSteeringState(void)
    {
        delete mBehaviour;
    }

    void AgentSteeringState::addSteeringBehaviour(SteeringBehaviour* behaviour)
    {
        behaviour->setParent(mBehaviour);
        behaviour->setController(mAgent);
        mBehaviour->addState(behaviour);
        LOG_MESSAGE(Logger::AI, 
            "added SteeringBehaviour for Agent");
    }

    void AgentSteeringState::clearSteeringBehaviours()
    {
        mBehaviour->clearStates();
        LOG_MESSAGE(Logger::AI, 
            "Cleared all SteeringBehaviours for Agent");
    }

    void AgentSteeringState::update(const float elapsedTime)
    {
        mBehaviour->update(elapsedTime);
        //  currentTime not needed yet, only elapsedTime
        mAgent->updateVehicle(0.0f, elapsedTime);
    }

    SteeringVehicle* AgentSteeringState::getVehicle()
    {
        return mAgent;
    }

}
