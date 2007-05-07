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
#ifndef __RlAI_Agent_H__
#define __RlAI_Agent_H__

#include "PhysicsController.h"
#include "FuzzyState.h"
#include "AgentManager.h"

namespace rl
{
	class SteeringVehicle;
	class SteeringMachine;
	class DialogCharacter;
	class Creature;
	class Actor;
//	class PerceptionPool;

	/** Represents a single entity that has got AI.
	 * Instantiated objects are normally registered with AgentManager.
	 * This class creates the connection between actively involved
	 * Creatures (rl::Creature), steering (pathfinding, movement - OpenSteer)
	 * and decission making (behaviours)
	 */
	class _RlAiExport Agent 
		: public PhysicsController
	{
	public:
		/** Constructor by Creature object.
		 * @param character Creature object
		 */
		Agent(Creature* character);
		/** Constructor by Creature object.
		 * @param character Creature object
		 * @param vehicle SteeringVehicle object for the specified creature
		 */
		Agent(Creature* character, SteeringVehicle* vehicle);

		/** deprecated
		 */
		Agent(DialogCharacter* character);

		/** explicit virtual destructor
		 */
		virtual ~Agent(void);

		/** Initializes class and registeres with parts exterior to AI system.
		 * Creates a standard Vehicle object for the creature when none was
		 * supplied on object creation. Creates a behaviour state machine and
		 * registers itself as a PhysicsController with PhysicalManager for the
		 * stored creature object.
		 */
		void initialize();

		/** Returns the agent type (Player, NPC, etc.).
		 */
		AgentManager::AgentType getType();
		/** Sets the agent type (Player, NPC, etc. ).
		 * @param type AgentType sets the type to the given one.
		 */
		void setType(AgentManager::AgentType type);

		/** Function invoked by AgentManager to let Agents advance their AI.
		 * Is invoked with time since last invocation. Here the behaviour and
		 * the vehicle are updated.
		 */
		void update(const float elapsedTime);
		/** Adds a steering behaviour to the internal steering machine.
		 * @param behaviour SteeringBehaviour is initialized and registered to mBehaviour.
		 */
		void addSteeringBehaviour(SteeringBehaviour* behaviour);
		/** Removes all steering behaviours.
		 */
        void clearSteeringBehaviours();

		/** Retrieves the stored SteeringVehicle.
		 */
		SteeringVehicle* getVehicle();

		/** Retrieves the Creature object controlled by this agent
		 * @returns Creature object controlled
		 */
        Creature* getControlledCreature() const;

//		PerceptionPool* getPerceptionPool();

		//! deprecated - for DialogBot
		bool isDialogActive();

		/** Function invoked by PhysicsManager whenever force/torque is to be
		 * applied to the creature stored.
		 * @param thing PhysicalThing that is responsible for the creature.
		 */
		virtual void OnApplyForceAndTorque(PhysicalThing* thing);

	protected:
		//! stores the type of the agent (Player, NPC, ...)
		AgentManager::AgentType mType;
		//! stores the behaviour FuzzyStateMachine for behaviour
		SteeringMachine* mBehaviour;
		//! stores the vehicle representation of the creature
		SteeringVehicle* mVehicle;
		//! stores the creature whose AI is simulated
		Creature* mCreature;

		//! deprecated - for DialogBot
		DialogCharacter* mDialogBot;
//		PerceptionPool* mPerceptionPool;
	};

	inline AgentManager::AgentType Agent::getType()
	{
		return mType;
	}

	inline SteeringVehicle* Agent::getVehicle()
	{
		return mVehicle;
	}

	inline void Agent::setType(AgentManager::AgentType type)
	{
		mType = type;
	}

/*	inline PerceptionPool* Agent::getPerceptionPool()
	{
		return mPerceptionPool;
	}
*/
}
#endif
