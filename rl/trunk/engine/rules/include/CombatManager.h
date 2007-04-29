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

#ifndef __COMBATMANAGER_H__
#define __COMBATMANAGER_H__

#include "RulesPrerequisites.h"

#include <OgreSingleton.h>

namespace rl
{
	class Combat;
    class Creature;
	
    class _RlRulesExport CombatManager : public Ogre::Singleton<CombatManager>
    {
    public:
        static CombatManager& getSingleton(void);
        static CombatManager* getSingletonPtr(void);

       CombatManager();
        ~CombatManager();

        /// Creates a new combat object and sets it active.
        /// @param character the player character
        /// @param firstOpponent the opponent, that caused the combat.
        /// @throw IllegalStateException if there is already a combat running.
        Combat* startCombat(Creature* character, Creature* firstOpponent);

        /// Return the combat currently running, NULL if there is none.
        Combat* getCurrentCombat() const;

    private:
        Combat* mCurrentCombat;
    };
}

#endif
