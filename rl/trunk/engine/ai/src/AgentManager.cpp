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

using namespace Ogre;

template<> rl::AgentManager* Singleton<rl::AgentManager>::ms_Singleton = 0;

using namespace rl;

AgentManager& AgentManager::getSingleton(void)
{
    return Singleton<AgentManager>::getSingleton();
}

AgentManager* AgentManager::getSingletonPtr(void)
{
    return Singleton<AgentManager>::getSingletonPtr();
}

AgentManager::AgentManager(void) : mAllNeighbors(), mAgents()
{

}

AgentManager::~AgentManager(void)
{
    //	free all agents
    for(AgentList::iterator itr = mAgents.begin(); itr != mAgents.end(); ++itr)
    {
        delete (*itr);
    }
}

Agent* AgentManager::createAgent(Creature* character)
{
    Agent* agent = new Agent(character);
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
	Logger::getSingleton().log(
        Logger::AI,
        Logger::LL_MESSAGE,
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


void AgentManager::run( Ogre::Real elapsedTime ) 
{
    //	update all agents
    for(AgentList::iterator itr = mAgents.begin(); itr != mAgents.end(); ++itr)
    {
        (*itr)->update(elapsedTime);
    }
}*/