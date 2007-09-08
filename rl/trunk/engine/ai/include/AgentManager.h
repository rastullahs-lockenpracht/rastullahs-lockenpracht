/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#ifndef __RlAI_AgentManager_H__
#define __RlAI_AgentManager_H__

#include <OgreSingleton.h>
#include "AiPrerequisites.h"
#include "Combatant.h"
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
	 * to do its ai stuff (path finding, movement, decision making).
	 */
	class _RlAiExport AgentManager
		: public Ogre::Singleton<AgentManager>,
		  public GameTask,
          public GameObjectStateListener,
          public CombatantFactory
		  
	{
	public:
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
		Agent* createAgent(Creature* character);

        //! Destroys an Agent and all its behaviours. (if any)
        void destroyAgent(Agent*);

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

        // overrides from CombatantFactory
        virtual Combatant* createCombatant(Creature* creature);
        virtual void destroyCombatant(Combatant*);

	private:

		/** Used to register an agent internally.
		 * Adds the given Agent to AgentList and its vehicle to mAllNeighbors.
		 * @param agent Agent to be added
		 */
		void addAgent(Agent* agent);

		//! defines a std::vector of Agents to ease understanding
		typedef std::map<Creature*, Agent*> AgentMap;

        //! Factory to create behaviours with.
        BehaviourFactory* mBehaviourFactory;
		
		//! List of registered agents (includes mPlayer)
		AgentMap mAgents;
		//! Agent representing the player
		Agent* mPlayer;
	};
}
#endif
