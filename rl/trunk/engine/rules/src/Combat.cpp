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

    Combat::Combat(Combatant* character) : mCharacter(character)
    {
    }

    Combat::~Combat()
    {
    }

    void Combat::addOpponent(Combatant* Combatant)
    {
        mOpponents.insert(Combatant);
    }

    void Combat::removeOpponent(Combatant* Combatant)
    {
        mOpponents.erase(Combatant);
    }

    void Combat::addAlly(Combatant* Combatant)
    {
        mAllies.insert(Combatant);
    }

    void Combat::removeAlly(Combatant* Combatant)
    {
        mAllies.erase(Combatant);
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
    }

    void Combat::stop()
    {
    }
}
