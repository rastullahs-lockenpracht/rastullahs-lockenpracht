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

#include "AgentManager.h"

#include "Agent.h"
#include "AgentCombatState.h"
#include "Creature.h"
#include "GameObjectManager.h"

using namespace Ogre;

template<> rl::AgentManager* Singleton<rl::AgentManager>::ms_Singleton = 0;

namespace rl {

AgentManager::AgentManager(void)
    : mBehaviourFactory(NULL), mAgents(), mPlayer(NULL)
{
    // Subscribe as listener to the GameObjectManager, in order to attach Agents to
    // Creatures that are placed into the scene.
    GameObjectManager::getSingleton().registerGameObjectStateListener(this);

    // Register the AgentCombatStateFactory with the CombatManager, so that they can
    // be created when a creature is taking part the combat.
}

AgentManager::~AgentManager(void)
{
    GameObjectManager::getSingleton().unregisterGameObjectStateListener(this);
	removeAllAgents();
}

Agent* AgentManager::createAgent(Creature* character)
{
    Agent* agent = new Agent(character);
	addAgent(agent);
    return agent;
}

void AgentManager::destroyAgent(Agent* agent)
{
    AgentMap::iterator it = mAgents.find(agent->getControlledCreature());
    if (it != mAgents.end())
    {
        delete it->second;
        mAgents.erase(it);
    }
    else
    {
        LOG_ERROR(Logger::AI, "AgentManager::destroyAgent: agent not found.");
    }
}

void AgentManager::addAgent(Agent* agent)
{
    mAgents.insert(std::make_pair(agent->getControlledCreature(), agent));
    LOG_MESSAGE(Logger::AI, 
        "created AI Agent");
}

void AgentManager::run( Ogre::Real elapsedTime ) 
{
    //	update agents
    for(AgentMap::iterator it = mAgents.begin(); it != mAgents.end(); ++it)
    {
	    it->second->update(elapsedTime);
    }
}

void AgentManager::removeAllAgents()
{
    for(AgentMap::iterator it = mAgents.begin(); it != mAgents.end(); ++it)
    {
	    delete it->second;
    }
    mAgents.clear();
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
        AgentMap::iterator it = mAgents.find(creature);
        if (it != mAgents.end())
        {
            destroyAgent(it->second);
        }
    }
    else if (newState == GOS_IN_SCENE)
    {
        // Create an Agent and add the behaviours of the creature to it.
        Property aiProperty = creature->getProperty(Creature::PROPERTY_AI);
        //if (behaviorProperty.isArray())
        //{
        //    PropertyVector behaviors = aiProperty.toArray();
        //    if (!behaviors.empty())
        //    {
        //        Agent* agent = createAgent(creature);
        //        for (PropertyVector::const_iterator it = behaviors.begin(),
        //            end = behaviors.end(); it != end; ++it)
        //        {
        //            if (it->isString())
        //            {
        //                SteeringBehaviour* behavior =
        //                    mBehaviourFactory->createBehaviour(it->toString().c_str());
        //                agent->addSteeringBehaviour(behavior);
        //            }
        //        }
        //    }
        //}
    }
}

void AgentManager::setBehaviourFactory(BehaviourFactory* factory)
{
    mBehaviourFactory = factory;
}

Combatant* AgentManager::createCombatant(Creature* creature)
{
    Agent* agent = NULL;
    // Get agent from the creature.
    AgentMap::iterator it = mAgents.find(creature);
    if (it == mAgents.end())
    {
        // Create agent, since there is none yet.
        agent = createAgent(creature);
    }
    else
    {
        agent = it->second;
    }
    // Put the Agent into combat state and return the state.
    agent->pushState(AST_COMBAT);
    Combatant* combatant = dynamic_cast<AgentCombatState*>(agent->getCurrentState());
    return combatant;
}

void AgentManager::destroyCombatant(Combatant* combatant)
{
    // Get agent from combatant
    AgentCombatState* combatState = dynamic_cast<AgentCombatState*>(combatant);
    if (combatState == NULL)
    {
        Throw(IllegalArgumentException,
            "Given combatant was not created by this Factory.(AgentManager)");
    }
    // Pop state
    combatState->getAgent()->popState();
}

}
