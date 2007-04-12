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

namespace rl
{
    class Creature;

    class _RlRulesExport Combat
    {
    public:
        Combat(Creature* character);
        ~Combat();

        void addOpponent(Creature*);
        void removeOpponent(Creature*);

        void addAlly(Creature*);
        void removeAlly(Creature*);

    private:
        typedef std::set<Creature*> CreatureSet;
        Creature* mCharacter;
        CreatureSet mOpponents;
        CreatureSet mAllies;
    };
}

#endif
