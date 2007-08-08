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

#ifndef __RL_COMBATANT_H__
#define __RL_COMBATANT_H__

#include "RulesPrerequisites.h"

namespace rl
{
    class Creature;
    class CreatureController;

    class _RlRulesExport Combatant
    {
    public:
        Combatant(CreatureController* controller);
        virtual ~Combatant();

        virtual Ogre::String getCombatantTypeName() const = 0;

        CreatureController* getCreatureController() const;

    protected:
        CreatureController* mController;
    };

    class _RlRulesExport CombatantFactory
    {
    public:
        virtual ~CombatantFactory() {}

        virtual Combatant* createCombatant(Creature* creature) = 0;
        virtual void destroyCombatant(Combatant*) = 0;
    };
}

#endif
