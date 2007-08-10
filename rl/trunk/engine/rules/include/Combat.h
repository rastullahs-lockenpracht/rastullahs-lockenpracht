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

#include <set>
#include <vector>

namespace rl
{
    class Combatant;

    class _RlRulesExport Combat
    {
    public:
        typedef std::set<Combatant*> CombatantSet;

        Combat(Combatant* character);
        ~Combat();

        void addOpponent(Combatant*);
        void removeOpponent(Combatant*);

        void addAlly(Combatant*);
        void removeAlly(Combatant*);

        const CombatantSet& getAllOpponents() const;
        const CombatantSet& getAllAllies() const;

        void start();
        void stop();

    private:
        Combatant* mCharacter;
        CombatantSet mOpponents;
        CombatantSet mAllies;
        typedef std::vector<std::pair<int, Combatant*> > CombatantQueue;
        // Combatants in order of their initiative for the current round.
        CombatantQueue mCombatantQueue;
        // Combatants in order of their initiative for the next round.
        // Will be copied to the current round after current round is done
        // and then it will be emptied.
        CombatantQueue mNextCombatQueue;
    };
}

#endif
