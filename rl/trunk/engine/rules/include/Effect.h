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

        enum Quantifier
        {
            QUANTIFIER_MULTIPLE,
            QUANTIFIER_UNIQUE,
            QUANTIFIER_UNIQUE_BUT_PROLONGABLE,
            QUANTIFIER_UNIQUE_BUT_CUMULATIVE
        };

        enum EffectTag
        {
            EFFECTTAG_NONE,
            EFFECTTAG_KRANKHEIT
        };

		Effect(int stufe = 1);
		virtual ~Effect();

        const CeGuiString getName() const;
        void setName(CeGuiString name);
        const CeGuiString getDescription() const;
        void setDescription(CeGuiString description);
		Creature* getOwner() const;
		void setOwner(Creature* owner);
		virtual int getDuration();
		virtual void setDuration(int newDuration);
		Quantifier getQuantifier();
		void setQuantifier(Quantifier quantifier);
		/** 
         *  @brief Adds a tag to the effect. 
         *  Tags are describing the type of an Effect, so other Effects can 
         *  identify it. An example would be 'Poison', as well as 'mineralic 
         *  poison'.
         **/
		virtual void addTag(EffectTag tagId);
        /**
         * @brief removes a tag from the effect.
         * @sa addTag
         **/
		virtual void removeTag(EffectTag tagId);
        /**
         * @brief Queries the effect for its tags.
         * @sa addTag
         **/
		virtual bool queryTag(EffectTag tagId);
        /**
         * @brief Retturns the Stufe (level) of the effect.
         **/
		virtual const int getStufe();
        /**
         * @brief Increases the level by one.
         **/
		virtual void increaseStufe();
        /**
         * @brief Decreases the level by one.
         **/
		virtual void decreaseStufe();
		/// Macht den Effekt wirksam.
		virtual void enable();
		/// Macht den Effekt unwirksam.
		virtual void disable();

		/// Ueberprueft die Lebendigkeit des Effekts.
		virtual void check();

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
		int mStufe;
		RL_LONGLONG mDuration;
		/// Zeitpunkt des Beginns des Effekts.
		RL_LONGLONG mStartTime;
		/// Speichert, ob der Effekt gerade wirksam ist.
		bool mEnabled;
		/// Gibt an, wie oft ein Effekt dieses Namens auf einer Kreatur wirken kann.
		Quantifier mQuantifier;

		typedef std::set<EffectTag> Tags;
		Tags mTags;

		Creature* mOwner;
	};
}

#endif //__EFFECT_H__
