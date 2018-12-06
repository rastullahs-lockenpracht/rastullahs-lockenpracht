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
		typedef enum {AKTION, REAKTION, FREIE_AKTION, LAENGERFRISTIGE_HANDLUNG} Type;

        Kampfaktion(Type type, const CeGuiString& name, const CeGuiString& description);
        virtual ~Kampfaktion();

        CeGuiString getName() const;
        CeGuiString getDescription() const;

		virtual bool hasTarget() const = 0;
		virtual Ogre::Real getMaximumTargetDistance(Combatant* actor) const = 0;

        virtual bool canDoKampfaktion(Combatant* actor, Combatant* target) const = 0;
        virtual void doKampfaktion(Combatant* actor, Combatant* target) = 0;

	protected:
		Type mType;
		CeGuiString mName;
		CeGuiString mDescription;
    };

	class _RlRulesExport AttackeAktion : public Kampfaktion
	{
	public:
		AttackeAktion();
		virtual ~AttackeAktion();

		virtual bool hasTarget() const;
		virtual Ogre::Real getMaximumTargetDistance(Combatant* actor) const;

		virtual bool canDoKampfaktion(Combatant* actor, Combatant* target) const;
        virtual void doKampfaktion(Combatant* actor, Combatant* target);
	};

	class _RlRulesExport ParadeAktion : public Kampfaktion
	{
	public:
		ParadeAktion();
		virtual ~ParadeAktion();

		virtual bool hasTarget() const;
		virtual Ogre::Real getMaximumTargetDistance(Combatant* actor) const;

		virtual bool canDoKampfaktion(Combatant* actor, Combatant* target) const;
        virtual void doKampfaktion(Combatant* actor, Combatant* target);
	};

	class _RlRulesExport AusweichenAktion : public Kampfaktion
	{
	public:
		AusweichenAktion();
		virtual ~AusweichenAktion();

		virtual bool hasTarget() const;
		virtual Ogre::Real getMaximumTargetDistance(Combatant* actor) const;

		virtual bool canDoKampfaktion(Combatant* actor, Combatant* target) const;
        virtual void doKampfaktion(Combatant* actor, Combatant* target);
	};

	class _RlRulesExport BewegenAktion : public Kampfaktion
	{
	public:
		BewegenAktion();
		virtual ~BewegenAktion();

		virtual bool hasTarget() const;
		virtual Ogre::Real getMaximumTargetDistance(Combatant* actor) const;

		virtual bool canDoKampfaktion(Combatant* actor, Combatant* target) const;
		// Do not call, will throw!
        virtual void doKampfaktion(Combatant* actor, Combatant* target);
		virtual void doKampfaktion(Combatant* actor, const Ogre::Vector3& target);
	};

	class _RlRulesExport FolgenAktion : public Kampfaktion
	{
	public:
		FolgenAktion();
		virtual ~FolgenAktion();

		virtual bool hasTarget() const;
		virtual Ogre::Real getMaximumTargetDistance(Combatant* actor) const;

		virtual bool canDoKampfaktion(Combatant* actor, Combatant* target) const;
        virtual void doKampfaktion(Combatant* actor, Combatant* target);
	};
}

#endif
