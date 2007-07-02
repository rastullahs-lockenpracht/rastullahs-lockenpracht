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
#include "Agent.h"

#include "Actor.h"
#include "Creature.h"
#include "DialogCharacter.h"
#include "Exception.h"
#include "ScriptWrapper.h"
#include "SteeringVehicle.h"
#include "SteeringMachine.h"

using namespace Ogre;
using namespace OpenSteer;

namespace rl {

Agent::Agent(Creature* character)
	: mType(AgentManager::AGENT_NONE),
      mBehaviour(NULL),
      mVehicle(NULL),
	  mCreature(character)
{
	initialize();
	ScriptWrapper::getSingleton().owned(character);
}

Agent::Agent(Creature* character, SteeringVehicle* vehicle)
	: mType(AgentManager::AGENT_NONE),
      mBehaviour(NULL),
      mVehicle(vehicle), 
	  mCreature(character)
{
	initialize();
}

Agent::~Agent(void)
{
    delete mVehicle;
    delete mBehaviour;
	if (mCreature != NULL)
		ScriptWrapper::getSingleton().disowned(mCreature);
}

void Agent::initialize()
{
    //  an agent needs a creature it refers to
	if(mCreature == NULL)
	{
		Throw(NullPointerException, "Agent has no creature");
	}

    //  if there is no vehicle, create a standard vehicle
	if(mVehicle == NULL)
	{
		mType = AgentManager::AGENT_STD_NPC;
		mVehicle = new SteeringVehicle(this, mCreature);
	}
    LOG_MESSAGE(Logger::AI, 
        "created SteeringVehicle for Agent");
    
	mBehaviour = new SteeringMachine(NULL, mVehicle);
    LOG_MESSAGE(Logger::AI, 
        "created SteeringMachine for Agent");
}

void Agent::addSteeringBehaviour(SteeringBehaviour* behaviour)
{
    behaviour->setParent(mBehaviour);
    behaviour->setController(mVehicle);
    mBehaviour->addState(behaviour);
    LOG_MESSAGE(Logger::AI, 
        "added SteeringBehaviour for Agent");
}

void Agent::clearSteeringBehaviours()
{
    mBehaviour->clearStates();
    LOG_MESSAGE(Logger::AI, 
        "Cleared all SteeringBehaviours for Agent");
}

void Agent::update(const float elapsedTime)
{
	mBehaviour->update(elapsedTime);
    //  currentTime not needed yet, only elapsedTime
    mVehicle->update(0.0f, elapsedTime);
}

Creature* Agent::getControlledCreature() const
{
    return mCreature;
}

}
