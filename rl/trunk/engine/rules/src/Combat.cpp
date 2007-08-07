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

namespace rl
{
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
}
