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
#include "AgentManager.h"
#include "Agent.h"
#include "PlayerVehicle.h"
#include "Creature.h"

using namespace Ogre;

template<> rl::AgentManager* Singleton<rl::AgentManager>::ms_Singleton = 0;

namespace rl {

AgentManager& AgentManager::getSingleton(void)
{
    return Singleton<AgentManager>::getSingleton();
}

AgentManager* AgentManager::getSingletonPtr(void)
{
    return Singleton<AgentManager>::getSingletonPtr();
}

AgentManager::AgentManager(void) : mAllNeighbors(), mAgents(), mPlayer(NULL)
{

}

AgentManager::~AgentManager(void)
{
	removeAllAgents();
}

Agent* AgentManager::createAgent(AgentType type, Creature* character)
{
	SteeringVehicle* vehicle = NULL;
	if(type == AGENT_PLAYER)
	{
		vehicle = new PlayerVehicle(character->getActor());
	}
    Agent* agent = new Agent(character, vehicle);
	agent->setType(type);
	if(type == AGENT_PLAYER)
	{
		mPlayer = agent;
	}

	addAgent(agent);
    return agent;
}

Agent* AgentManager::createAgent(DialogCharacter* character)
{
	Agent* agent = new Agent(character);
	addAgent(agent);
	return agent;
}

void AgentManager::addAgent(Agent* agent)
{
	mAgents.push_back(agent);
    LOG_MESSAGE(Logger::AI, 
        "created AI Agent");
	mAllNeighbors.push_back(agent->getVehicle());
}

AgentManager::VehicleList AgentManager::getNeighbors(Agent* agent)
{
	return mAllNeighbors;
}
/*
void AgentManager::OnApplyForceAndTorque(PhysicalThing* thing)
{
    //	steerToAvoidNeighbors (10.0, const AVGroup& others);
}
*/

void AgentManager::run( Ogre::Real elapsedTime ) 
{
//	update agents
	if(mPlayer != NULL)
	{
		mPlayer->update(elapsedTime);
	}

  /*  for(AgentList::iterator itr = mAgents.begin(); itr != mAgents.end(); ++itr)
    {
	//  update agents of type "player" only
		if((*itr)->getType() == AGENT_PLAYER)
		{
			(*itr)->update(elapsedTime);
			break;
		}
    }*/
}

void AgentManager::removeAllAgents()
{
    for(AgentList::iterator itr = mAgents.begin(); itr != mAgents.end(); ++itr)
    {
        delete (*itr);
    }
    mAgents.clear();
    mAllNeighbors.clear();
    mPlayer = NULL;
}

const Ogre::String& AgentManager::getName() const
{
    static String NAME = "AgentManager";

    return NAME;
}

}
