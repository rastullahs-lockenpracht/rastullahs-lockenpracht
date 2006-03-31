/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "RulesPrerequisites.h"
#include "Date.h"
#include <set>


namespace rl
{
	class Creature;

	class _RlRulesExport Effect
	{
	public:

		static const int EFFECT_MULTIPLE = 0;
		static const int EFFECT_UNIQUE = 1;
		static const int EFFECT_UNIQUE_BUT_PROLONGABLE = 2;
		static const int EFFECT_UNIQUE_BUT_CUMULATIVE = 3;

		Effect();
		virtual ~Effect();

        const CeGuiString& getName() const;
        void setName(CeGuiString& name);
        const CeGuiString& getDescription() const;
        void setDescription(CeGuiString& description);
		Creature* getOwner() const;
		void setOwner(Creature* owner);
		virtual int getDuration();
		virtual void setDuration(int newDuration);
		int getQuantifier();
		void setQuantifier(int quantifier);
		//** @brief Adds a tag to the Effect. Tags are describing the type of
		//*  an Effect, so other Effects can identify it. An example would be
		//** 'Poison', as well as 'mineralic poison'.
		virtual void addTag(int tagId);
		virtual void removeTag(int tagId);
		virtual bool queryTag(int tagId);
		/// Macht den Effekt wirksam.
		virtual void enable();
		/// Macht den Effekt unwirksam.
		virtual void disable();

		/// Überprüft die Lebendigkeit des Effekts.
		virtual bool isAlive() = 0;

	protected:
		/// Bringt die Aenderungen ein.
		virtual void apply() = 0;
		/// Entfernt die Aenderungen.
		virtual void remove() = 0;
		/// Gibt die Zeit seit Beginn des Effekts zurueck.
		virtual RL_LONGLONG getTimePassed();

		CeGuiString mName;
		CeGuiString mDescription;
		/// Dauer desEffekts in Aktionen.
		RL_LONGLONG mDuration;
		/// Zeitpunkt des Beginns des Effekts.
		RL_LONGLONG mStartTime;
		/// Speichert, ob der Effekt gerade wirksam ist.
		bool mEnabled;
		/// Gibt an, wie oft ein Effekt dieses Namens auf einer Kreatur wirken kann.
		int mQuantifier;

		typedef std::set<int> Tags;
		Tags mTags;

		Creature* mOwner;
	};
}

#endif //__EFFECT_H__
