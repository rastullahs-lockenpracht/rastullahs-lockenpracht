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
#ifndef __RlAI_SteeringAgentState_H__
#define __RlAI_SteeringAgentState_H__

#include "AiPrerequisites.h"
#include "AgentState.h"
#include "SteeringVehicle.h"
#include "SteeringMachine.h"

namespace rl
{
    /// AgentState that is used when an Agent is idling or walking from A to B.
    /// This state uses a fuzzy state machine to incorporate one or more behaviours
    /// for decision making.
    class _RlAiExport AgentSteeringState : public AgentState
	{
	public:
        /// @param agent the agent this state works upon.
        AgentSteeringState(Agent* agent);

        virtual ~AgentSteeringState();

		/** Function invoked by Agents to advance the AI.
		 * Decision making is done here.
         * @param elapsedTime time since last update call.
		 */
		void update(const float elapsedTime);

		void addSteeringBehaviour(SteeringBehaviour* behaviour);
		/** Removes all steering behaviours.
		 */
        void clearSteeringBehaviours();

		/** Retrieves the stored SteeringVehicle.
		 */
		SteeringVehicle* getVehicle();

	protected:
		//! stores the behaviour FuzzyStateMachine for behaviour
		SteeringMachine* mBehaviour;
		//! stores the vehicle representation of the creature
		SteeringVehicle* mVehicle;
	};
}
#endif
