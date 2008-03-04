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

#ifndef __RL_COMBAT_H__
#define __RL_COMBAT_H__

#include "RulesPrerequisites.h"
#include "CombatAction.h"

#include <set>
#include <vector>
#include <boost/tuple/tuple.hpp>

namespace rl
{
    class Combatant;

    class _RlRulesExport Combat
    {
    public:
        typedef std::set<Combatant*> CombatantSet;

        Combat();
        ~Combat();

        void addOpponent(Combatant*);
        void removeOpponent(Combatant*);

        void addAlly(Combatant*);
        void removeAlly(Combatant*);

        const CombatantSet& getAllOpponents() const;
        const CombatantSet& getAllAllies() const;

        void start();
        void stop();

        // Called by combatants in response to a request by the Combat object.
        // With calling this function combatants register their actions for this round.
        void registerCombatantAction(Combatant*, CombatAction*, CombatAction*, CombatAction*);
        void actionExecuted(Combatant*, CombatAction*);

    private:
        typedef std::vector<std::pair<int, Combatant*> > CombatantQueue;
        /// Stores the max three actions a combatant can do per round.
        /// Probably has to be replaced with a more sophisticated container later on,
        /// but for now it will do what it needs to.
        typedef boost::tuples::tuple<CombatAction*, CombatAction*, CombatAction*> ActionTuple;
        typedef std::map<Combatant*, ActionTuple> CombatantActionMap;

        CombatantSet mOpponents;
        CombatantSet mAllies;
        /// Combatants in order of their initiative for the current round.
        CombatantQueue mCombatantQueue;
        CombatantActionMap mCombatantActions;

        unsigned short mCurrentRound;

        void beginRound();
        void executeRound();
        void endRound();
    };
}

#endif
