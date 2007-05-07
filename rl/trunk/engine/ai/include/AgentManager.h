/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#ifndef __RlAI_AgentManager_H__
#define __RlAI_AgentManager_H__

#include <OgreSingleton.h>
#include "AiPrerequisites.h"
//#include "PhysicsController.h"
#include "GameTask.h"
#include "GameObjectStateListener.h"
#include "FuzzyState.h"

namespace rl
{
	class Creature;
	class SteeringVehicle;
	class DialogCharacter;
    class Agent;

    typedef FuzzyState<SteeringVehicle> SteeringBehaviour;

    class _RlAiExport BehaviourFactory
    {
    public:
        BehaviourFactory() {}
        virtual ~BehaviourFactory() {}

        virtual SteeringBehaviour* createBehaviour(const Ogre::String& classname) = 0;
    };

	/** Executes AI during game as a RL::GameTask.
	 * Each registered Agent gets executed once per gametask in order
	 * to do its ai stuff (path finding, movement, decission making).
	 */
	class _RlAiExport AgentManager
		: public Ogre::Singleton<AgentManager>,
		  public GameTask,
          public GameObjectStateListener
  	//	  public PhysicsController,
		  
	{
	public:
		/** Different types of agents.
		 * Implemented in order to recognise different types of agents, because
		 * the AI needs to handle them differently.
		 */
		enum AgentType
		{
			AGENT_NONE = -1,	//!< when the type is unknown/unset
			AGENT_PLAYER = 0,	//!< when the agent represents a player
			AGENT_STD_NPC = 1,	//!< when the agent is a nonplayercharacter
			AGENT_FLOCKING = 2	//!< hmm, possibly for groups behaviour of NPCs
		};

		//! defines a std::vector list of Vehicle objects for OpenSteer
		typedef std::vector<SteeringVehicle*> VehicleList;

		//! default constructor
		AgentManager(void);
		//! explicit virtual destructor
		virtual ~AgentManager(void);

		/** Creates an AI agent with an assigned rl::Actor.
		 * AgentManager is responsible for memory management of the Agent
		 * @param type defines the type of the agent \see AgentType
		 * @param character the associated creature
		 */
		Agent* createAgent(AgentType type, Creature* character);

		//! deprecated - should be removed in the long run.
		//! Hint: it's deprecated
		Agent* createAgent(DialogCharacter* character);

        //! Destroys an Agent and all its behaviours. (if any)
        void destroyAgent(Agent*);

		//! ??? purpose ??? needed for opensteer ?
		VehicleList getNeighbors(Agent* agent);

		/** Interface executed as a GameTask by GameLoop.
		 * is responsible for advancing the ai logik about the time specified.
		 * Each registered Agent is executed can advance the specified time.
		 * @param elapsedTime Ogre::Real specifying 
		 */
		void run( Ogre::Real elapsedTime );
		/** Removes all registered Agents and deletes their objects.
		 * Clears all internal lists and the playercharacter object is also
		 * deallocated.
		 */
		void removeAllAgents();

		/// Override from GameTask
		/// Returns the name of this class - AgentManager for debugging purposes.
        virtual const Ogre::String& getName() const;

        /// Override from GameObjectStateListener.
        /// Used to create/destroy Agents
        virtual void gameObjectStateChanged(GameObject* go, GameObjectState oldState,
            GameObjectState newState);

        /// Set the factory to be used to create behaviours.
        virtual void setBehaviourFactory(BehaviourFactory*);
	private:

		/** Used to register an agent internally.
		 * Adds the given Agent to AgentList and its vehicle to mAllNeighbors.
		 * @param agent Agent to be added
		 */
		void addAgent(Agent* agent);

		//! defines a std::vector of Agents to ease understanding
		typedef std::vector<Agent*> AgentList;

        //! Factory to create behaviours with.
        BehaviourFactory* mBehaviourFactory;
		
		//! List of Vehicle objects from the Agents in mAgents (might be needed for opensteer)
		VehicleList mAllNeighbors;
		//! List of registered agents (includes mPlayer)
		AgentList mAgents;
		//! Agent representing the player
		Agent* mPlayer;
	};
}
#endif
