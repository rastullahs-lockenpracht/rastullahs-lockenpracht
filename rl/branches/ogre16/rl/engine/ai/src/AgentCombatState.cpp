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
#include "Combat.h"
#include "CombatManager.h"
#include "CreatureControllerManager.h"

namespace rl
{

    AgentCombatState::AgentCombatState(Agent* agent)
        : AgentState(agent),
        Combatant(CombatManager::getSingleton().getCurrentCombat(),
            CreatureControllerManager::getSingleton().getCreatureController(
                agent->getControlledCreature())),
        mState()
    {
    }

    AgentCombatState::~AgentCombatState()
    {
    }

    Ogre::String AgentCombatState::getCombatantTypeName() const
    {
        return "default";
    }

    void AgentCombatState::requestCombatantAction()
    {
        // Think!
		const Combat::CombatantSet& allies = mCombat->getAllAllies();
		if (!allies.empty())
		{
			Combatant* target = *allies.begin();
			// Are we in weapon range to opponent
			if (target)
			{
				// Are we in weapon range to opponent
				if (mCombat->canAttack(this, target))
				{
					// Ok, we can attack
					mCombat->registerAttacke(this, target);
				}
				else
				{
					// We can't attack from here, so go to opponent.
					mCombat->registerFolgen(this, target);
				}
			}
		}
		mCombat->registerCombatantRoundDone(this);
    }

	void AgentCombatState::update(const float elapsedTime)
    {
        mAgent->updateVehicle(0, elapsedTime);
    }
}
