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

#ifndef __RL_COMBATANT_H__
#define __RL_COMBATANT_H__

#include "RulesPrerequisites.h"

namespace rl
{
    class Combat;
    class Creature;
    class CreatureController;
	class Kampfaktion;

    /// Base class of combat participiants. This defines the interface Combat uses
	/// to communicate with Combatants. Be it AI bots or actual users.
	class _RlRulesExport Combatant
    {
    public:
        Combatant(Combat* combat, CreatureController* controller);
        virtual ~Combatant();

        /// This function is called by the Combat the Combatantant is taking part in,
        /// in order to request it to register actions for the next round.
        /// In response to this call Combatant is supposed to call Combat#registerCombatantAction.
        /// The call is not required to be done immediatly,
        /// it can be done synchronously or asynchronously.
        virtual void requestCombatantAction() = 0;

        /// This function is called by the Combat the Combatantant is taking part in,
        /// in order to request it to execute the registered action.
		/// When done, Combatant is supposed to call Combat#actionExecuted.
		virtual void executeAction(Kampfaktion* aktion) = 0;

        /// Type name used by factory
		virtual Ogre::String getCombatantTypeName() const = 0;

        CreatureController* getCreatureController() const;

		// convenience getters

        Creature* getCreature() const;
		CeGuiString getName() const;
		Ogre::Vector3 getPosition() const;

    protected:
        Combat* mCombat;
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
