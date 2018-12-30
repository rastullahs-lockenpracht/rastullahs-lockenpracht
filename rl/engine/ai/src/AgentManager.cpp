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
#include "AgentCombatState.h"
#include "AgentManager.h"
#include "AgentSteeringState.h"
#include "AiSubsystem.h"
#include "AiWorld.h"
#include "CombatManager.h"
#include "Creature.h"
#include "GameObjectManager.h"

using namespace Ogre;

template <> rl::AgentManager* Singleton<rl::AgentManager>::msSingleton = 0;

namespace rl
{

    AgentManager::AgentManager(void)
        : mBehaviourFactory(NULL)
        , mAgents()
        , mPlayer(NULL)
    {
        // Subscribe as listener to the GameObjectManager, in order to attach Agents to
        // Creatures that are placed into the scene.
        GameObjectManager::getSingleton().registerGameObjectStateListener(this);

        // Register self as CombatantFactory with the CombatManager, so that State is switched
        // when agent is taking part in a combat.
        CombatManager::getSingleton().registerCombatantFactory("default", this);
    }

    AgentManager::~AgentManager(void)
    {
        GameObjectManager::getSingleton().unregisterGameObjectStateListener(this);
        removeAllAgents();
    }

    Agent* AgentManager::createAgent(Creature* character)
    {
        AgentMap::iterator it = mAgents.find(character);
        if (it != mAgents.end())
        {
            return it->second;
        }

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
        RlAssert(agent, "Agent is NULL");
        // add the agent to the proximity database
        AiSubsystem::getSingleton().getWorld()->addVehicle(agent);
        // add him to the internal agent list
        mAgents.insert(std::make_pair(agent->getControlledCreature(), agent));
        LOG_MESSAGE(Logger::AI, "created AI Agent");
    }

    void AgentManager::run(Ogre::Real elapsedTime)
    {
        //	update agents
        for (AgentMap::iterator it = mAgents.begin(); it != mAgents.end(); ++it)
        {
            it->second->update(elapsedTime);
        }
    }

    void AgentManager::removeAllAgents()
    {
        for (AgentMap::iterator it = mAgents.begin(); it != mAgents.end(); ++it)
        {
            delete it->second;
        }
        mAgents.clear();
        mPlayer = NULL;
    }

    const Ogre::String& AgentManager::getName() const
    {
        static Ogre::String NAME = "AgentManager";

        return NAME;
    }

    void AgentManager::gameObjectStateChanged(GameObject* go, GameObjectState oldState, GameObjectState newState)
    {
        // if the GameObject is not a Creature, we can ignore it
        Creature* creature = dynamic_cast<Creature*>(go);
        if (creature == NULL)
            return;

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
            /// Create an Agent and add the behaviours of the creature to it.
            /// /XXX setting of the actual properties should be delegated to the agent itself
            /// this is one level above what is sensible.
            Property aiProperty = creature->getProperty(Creature::PROPERTY_AI);
            if (aiProperty.isMap())
            {
                PropertyMap aiProps = aiProperty.toMap();
                Property behaviorsProperty = aiProps["behaviours"];
                if (behaviorsProperty.isArray())
                {
                    PropertyArray behaviours = behaviorsProperty.toArray();
                    if (!behaviours.empty())
                    {
                        Agent* agent = createAgent(creature);
                        // Agent is created with AgentSteeringState as default.
                        // So it is save to cast current AgentState to AgentSteeringState.
                        // Nevertheless see above comment for how to improve this situation.
                        AgentSteeringState* ass = dynamic_cast<AgentSteeringState*>(agent->getCurrentState());
                        ass->clearSteeringBehaviours();
                        for (PropertyArray::const_iterator it = behaviours.begin(), end = behaviours.end(); it != end;
                             ++it)
                        {
                            if (it->isString())
                            {
                                SteeringBehaviour* behavior
                                    = mBehaviourFactory->createBehaviour(it->toString().c_str());
                                ass->addSteeringBehaviour(behavior);
                            }
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
            Throw(IllegalArgumentException, "Given combatant was not created by this Factory.(AgentManager)");
        }
        // Pop state
        combatState->getAgent()->popState();
    }
}
