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

#ifndef __COMBATMANAGER_H__
#define __COMBATMANAGER_H__

#include "RulesPrerequisites.h"


#include <map>

namespace rl
{
	class Combat;
    class Combatant;
    class CombatantFactory;
    class Creature;
	class Kampfaktion;
	
    class _RlRulesExport CombatManager : public Ogre::Singleton<CombatManager>
    {
    public:
       CombatManager();
        ~CombatManager();

        /// Creates a new combat object and sets it active.
        /// @throw IllegalStateException if there is already a combat running.
        Combat* startCombat();
        void stopCombat();

        /// Return the combat currently running, NULL if there is none.
        Combat* getCurrentCombat() const;

        /// Returns a Combatant instance for the given Creature with the given name.
        /// If no name is given, the default Creature-Combatant is chosen, which is
        /// the default one registered by RlAi.
        /// Caller is resonsible for calling destroyCombatant in order to
        /// All undestroyed instances are destroyed when CombatManager is destroyed.
        /// @param creature Creature to create a Combatant for
        /// @param combatantType Name of the type to be used.
        Combatant* createCombatant(Creature* creature, const Ogre::String& combatantType = "default");
        void destroyCombatant(Combatant* combatant);

        void registerCombatantFactory(const Ogre::String& name, CombatantFactory* factory);
        void unregisterCombatantFactory(CombatantFactory* factory);

		Kampfaktion* getKampfaktion(const CeGuiString& name);

    private:
        typedef std::map<Ogre::String, CombatantFactory*> CombatantFactoryMap;
        CombatantFactoryMap mCombatantFactories;
        Combat* mCurrentCombat;

		std::map<CeGuiString, Kampfaktion*> mKampfaktionen;
    };
}

#endif
