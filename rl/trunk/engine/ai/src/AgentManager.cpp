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
#include "GameObjectManager.h"

using namespace Ogre;

template<> rl::AgentManager* Singleton<rl::AgentManager>::ms_Singleton = 0;

namespace rl {

struct FindAgentByControlledCreature : public std::binary_function<Agent*, Creature*, bool>
{
    bool operator()(Agent* agent, Creature* creature) const
    {
        return agent->getControlledCreature() == creature;
    }
};

AgentManager::AgentManager(void)
    : mBehaviourFactory(NULL), mAllNeighbors(), mAgents(), mPlayer(NULL)
{
    // Subscribe as listener to the GameObjectManager, in order to attach Agents to
    // Creatures that are placed into the scene.
    GameObjectManager::getSingleton().registerGameObjectStateListener(this);
}

AgentManager::~AgentManager(void)
{
    GameObjectManager::getSingleton().unregisterGameObjectStateListener(this);
	removeAllAgents();
}

Agent* AgentManager::createAgent(AgentType type, Creature* character)
{
	SteeringVehicle* vehicle = NULL;
	if(type == AGENT_PLAYER)
	{
		vehicle = new PlayerVehicle(character);
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

void AgentManager::destroyAgent(Agent* agent)
{
    AgentList::iterator it = std::find(mAgents.begin(), mAgents.end(), agent);
    if (it != mAgents.end())
    {
        delete *it;
        mAgents.erase(it);
    }
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

void AgentManager::run( Ogre::Real elapsedTime ) 
{
    //	update agents
    for(AgentList::iterator itr = mAgents.begin(); itr != mAgents.end(); ++itr)
    {
	    (*itr)->update(elapsedTime);
    }
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

void AgentManager::gameObjectStateChanged(GameObject* go, GameObjectState oldState,
                                          GameObjectState newState)
{
    // if the GameObject is not a Creature, we can ignore it
    Creature* creature = dynamic_cast<Creature*>(go);
    if (creature == NULL) return;

    if (oldState == GOS_IN_SCENE && newState != GOS_IN_SCENE)
    {
        // Remove the Agent and destroy it. Later we should pool them...
        AgentList::iterator it = std::find_if(mAgents.begin(), mAgents.end(),
            std::bind2nd(FindAgentByControlledCreature(), creature));
        if (it != mAgents.end())
        {
            destroyAgent(*it);
        }
    }
    else if (newState == GOS_IN_SCENE)
    {
        // Create an Agent and add the behaviours of the creature to it.
        Property behaviorProperty = creature->getProperty(Creature::PROPERTY_BEHAVIOURS);
        if (behaviorProperty.isArray())
        {
            PropertyVector behaviors = behaviorProperty.toArray();
            if (!behaviors.empty())
            {
                Agent* agent = createAgent(AGENT_STD_NPC, creature);
                for (PropertyVector::const_iterator it = behaviors.begin(),
                    end = behaviors.end(); it != end; ++it)
                {
                    if (it->isString())
                    {
                        SteeringBehaviour* behavior =
                            mBehaviourFactory->createBehaviour(it->toString().c_str());
                        agent->addSteeringBehaviour(behavior);
                    }
                }
            }
        }
    }
}

void AgentManager::setBehaviourFactory(BehaviourFactory* factory)
{
    mBehaviourFactory = factory;
}

}
