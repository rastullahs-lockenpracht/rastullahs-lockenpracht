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

#include "AgentManager.h"
#include "AgentState.h"

#include <stack>

namespace rl
{
	class Actor;
	class Creature;

	/** Represents a single entity that has got AI.
	 * Instantiated objects are normally registered with AgentManager.
	 * This class creates the connection between actively involved
	 * Creatures (rl::Creature), steering (pathfinding, movement - OpenSteer)
	 * and decission making (behaviours)
	 */
	class _RlAiExport Agent 
	{
	public:
		/** Constructor by Creature object.
		 * @param character Creature object
		 */
		Agent(Creature* character);

		/** explicit virtual destructor
		 */
		virtual ~Agent(void);

		/** Initializes class and registeres with parts exterior to AI system.
		 * Creates a standard Vehicle object for the creature when none was
		 * supplied on object creation. Creates a behaviour state machine and
		 * registers itself as a PhysicsController with PhysicalManager for the
		 * stored creature object.
         * Also pushes the SteeringState as the default state onto the state stack.
		 */
		void initialize();

		/** Function invoked by AgentManager to let Agents advance their AI.
		 * Is invoked with time since last invocation. Here the behaviour and
		 * the vehicle are updated.
		 */
		void update(const float elapsedTime);
		/** Adds a steering behaviour to the internal steering machine.
		 * @param behaviour SteeringBehaviour is initialized and registered to mBehaviour.
		 */

		/** Retrieves the Creature object controlled by this agent
		 * @returns Creature object controlled
		 */
        Creature* getControlledCreature() const;

        AgentState* getCurrentState() const;

        void pushState(AgentStateType);
        void popState();

	protected:
        typedef std::deque<AgentState*> AgentStateStack;
		//! stores the creature whose AI is simulated
		Creature* mCreature;
        std::stack<AgentState*> mAgentStates;
	};
}
#endif
