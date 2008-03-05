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
#include "stdinc.h"

#include "Combat.h"

#include "Combatant.h"
#include "CreatureController.h"
#include "GameEventLog.h"
#include "MessagePump.h"
#include "RulesMessages.h"

#include <OgreStringConverter.h>

using namespace boost;
using namespace boost::tuples;
using namespace Ogre;

namespace rl
{
    struct InitiativeComparator
    {
        /// \todo take into account other values of the combatant according to TDE rules,
        /// in case initiative is equal.
        bool operator()(const std::pair<int, Combatant*>& c1,
            const std::pair<int, Combatant*>& c2) const
        {
            return c1.first < c2.first;
        }
    };

    Combat::Combat()
        : mOpponents(),
          mAllies(),
          mCombatantQueue(),
          mCombatantActions(),
          mCurrentRound(0)
    {
    }

    Combat::~Combat()
    {
    }

    void Combat::addOpponent(Combatant* combatant)
    {
        mOpponents.insert(combatant);
		MessagePump::getSingleton().sendMessage<MessageType_CombatOpponentEntered>(combatant);
    }

    void Combat::removeOpponent(Combatant* combatant)
    {
        mOpponents.erase(combatant);
		MessagePump::getSingleton().sendMessage<MessageType_CombatOpponentLeft>(combatant);
    }

    void Combat::addAlly(Combatant* combatant)
    {
        mAllies.insert(combatant);
    }

    void Combat::removeAlly(Combatant* combatant)
    {
        mAllies.erase(combatant);
    }

    const Combat::CombatantSet& Combat::getAllOpponents() const
    {
        return mOpponents;
    }

    const Combat::CombatantSet& Combat::getAllAllies() const
    {
        return mAllies;
    }

    void Combat::start()
    {
		GameEventLog::getSingleton().logEvent("Kampf beginnt.", GET_COMBAT);

        mCurrentRound = 0;
		mCombatantQueue.clear();

        std::vector<Combatant*> combatants;
        combatants.insert(combatants.end(), mAllies.begin(), mAllies.end());
        combatants.insert(combatants.end(), mOpponents.begin(), mOpponents.end());
        // Calculate initiative for all participiants
        for (std::vector<Combatant*>::const_iterator it = combatants.begin();
            it != combatants.end(); ++it)
        {
            mCombatantQueue.push_back(std::make_pair(
                (*it)->getCreatureController()->getCreature()->getInitiativeBasis(), *it));
        }
        std::sort(mCombatantQueue.begin(), mCombatantQueue.end(), InitiativeComparator());

        beginRound();
    }

    void Combat::stop()
    {
    }

    void Combat::beginRound()
    {
        // Increment round counter
        mCurrentRound++;
        mCombatantActions.clear();

		GameEventLog::getSingleton().logEvent("Runde " + StringConverter::toString(mCurrentRound),
			GET_COMBAT);
		// Show ini for this round.
        for (CombatantQueue::iterator it = mCombatantQueue.begin();
			it != mCombatantQueue.end(); ++it)
        {
			String evt = it->second->getCreatureController()->getCreature()->getName().c_str();
			evt += " - Initiative: " + StringConverter::toString(it->first);
			GameEventLog::getSingleton().logEvent(evt, GET_COMBAT);
        }


        // Request actions from combatants
        for (CombatantQueue::iterator it = mCombatantQueue.begin();
            it != mCombatantQueue.end(); ++it)
        {
            it->second->requestCombatantAction();
        }
    }

    void Combat::registerCombatantAction(Combatant* combatant,
        CombatAction* action1, CombatAction* action2, CombatAction* action3)
    {
        mCombatantActions[combatant] = ActionTuple(action1, action2, action3);
        // Are all combatants registered now?
        if (mCombatantActions.size() == mCombatantQueue.size())
        {
            executeRound();
        }
    }

    void Combat::actionExecuted(Combatant*, CombatAction*)
	{
	}

    void Combat::executeRound()
	{
	}

    void Combat::endRound()
    {
        // All actions executed. Analyze outcome of this round.
        // 

        // Refill combatant queue with combatants that are still alive.
    }
}
