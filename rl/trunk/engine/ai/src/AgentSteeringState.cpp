/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */
#include "stdinc.h"

#include "Agent.h"
#include "AgentSteeringState.h"

using namespace Ogre;
using namespace OpenSteer;

namespace rl {

    AgentSteeringState::AgentSteeringState(Agent* agent)
        : AgentState(agent),
        mBehaviour(NULL),
        mVehicle(NULL)
    {
        mVehicle = new SteeringVehicle(agent);

        LOG_MESSAGE(Logger::AI, 
            "created SteeringVehicle for Agent");

        mBehaviour = new SteeringMachine(NULL, mVehicle);
        LOG_MESSAGE(Logger::AI, 
            "created SteeringMachine for Agent");
    }

    AgentSteeringState::~AgentSteeringState(void)
    {
        delete mVehicle;
        delete mBehaviour;
    }

    void AgentSteeringState::addSteeringBehaviour(SteeringBehaviour* behaviour)
    {
        behaviour->setParent(mBehaviour);
        behaviour->setController(mVehicle);
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
        mVehicle->update(0.0f, elapsedTime);
    }

    SteeringVehicle* AgentSteeringState::getVehicle()
    {
        return mVehicle;
    }

}
