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

#ifndef __RL_KAMPFAKTION_H__
#define __RL_KAMPFAKTION_H__

#include "RulesPrerequisites.h"

namespace rl
{
	class Combatant;

    /**
    * Abstract base class for combat actions.
	* Each combatant has a set of these Kampfaktionen, which
	* define valid actions during combat like Attacke, Parade.
    */
    class _RlRulesExport Kampfaktion
    {
    public:

        /**
        * @param name Name, mit der die Aktion dem Benutzer
        *             gegenüber dargestellt wird.
        *             Die gleiche, die intern verwendet wird.
        * @param descritpion Eine naehere Beschreibung.
        */
        Kampfaktion(const CeGuiString& name, const CeGuiString& description);
        virtual ~Kampfaktion();

        CeGuiString getName() const;
        CeGuiString getDescription() const;

        virtual bool canDoKampfaktion(Combatant* actor, Combatant* target) const = 0;
        virtual void doKampfaktion(Combatant* actor, Combatant* target) = 0;

	protected:
		CeGuiString mName;
		CeGuiString mDescription;
    };

	class _RlRulesExport Attacke : public Kampfaktion
	{
	public:
		Attacke();
		virtual ~Attacke();

        virtual bool canDoKampfaktion(Combatant* actor, Combatant* target) const;
        virtual void doKampfaktion(Combatant* actor, Combatant* target);
	};
}

#endif
