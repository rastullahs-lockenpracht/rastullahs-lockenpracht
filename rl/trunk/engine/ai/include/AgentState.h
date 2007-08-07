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
#ifndef __RlAI_AgentState_H__
#define __RlAI_AgentState_H__

#include "AiPrerequisites.h"

namespace rl
{
	class Agent;

    typedef enum {AST_STEERING, AST_COMBAT, AST_DIALOG} AgentStateType;

	/** Represents a discrete state an Agent can be in.
     * The Agent delegates the decision making to the currently active state, wich can
     * further deligate decision making.
	 */
	class _RlAiExport AgentState
	{
	public:
        /// @param agent the agent this state works upon.
        AgentState(Agent* agent) : mAgent(agent) {}

        virtual ~AgentState() {}

		/** Function invoked by Agents to advance the AI.
		 * Decision making is done here.
         * @param elapsedTime time since last update call.
		 */
		virtual void update(const float elapsedTime) = 0;

        Agent* getAgent() const { return mAgent; }

	protected:
		Agent* mAgent;
	};
}
#endif
