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
#ifndef __RlAI_AgentCombatState_H__
#define __RlAI_AgentCombatState_H__

#include "AiPrerequisites.h"
#include "AgentState.h"
#include "Combatant.h"

namespace rl
{
	/** Represents a discrete state an Agent can be in.
     * The Agent delegates the decision making to the currently active state, wich can
     * further deligate decision making.
	 */
    class _RlAiExport AgentCombatState : public AgentState, public Combatant
	{
	public:
        /// @param agent the agent this state works upon.
        AgentCombatState(Agent* agent);

        virtual ~AgentCombatState();

        virtual void requestCombatantAction();
		virtual void executeAction(Kampfaktion* action);

        virtual Ogre::String getCombatantTypeName() const;

		/** Function invoked by Agents to advance the AI.
		 * Decision making is done here.
         * @param elapsedTime time since last update call.
		 */
		virtual void update(const float elapsedTime);
    protected:
        typedef enum {} CombatState;
        CombatState mState;
	};
}
#endif
