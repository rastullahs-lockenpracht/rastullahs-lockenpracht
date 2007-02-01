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

        /**
         * Spezifiziert wie oft Effekte des selben Typs gleichzeitig auf dieselbe
         * Kreatur wirken koennen.
         **/
        enum Quantifier
        {
            QUANTIFIER_MULTIPLE,
            QUANTIFIER_UNIQUE,
            QUANTIFIER_UNIQUE_BUT_PROLONGABLE,
            QUANTIFIER_UNIQUE_BUT_CUMULATIVE
        };

        /**
         *  Spezifiziert bestimmte Merkmale eines Effekts und erlaubt damit darauf zu
         *  reagieren.
         *  @warning Das Konzept ist nur ein Prototyp und kann jederzeit verworfen 
         *   werden.
         **/
        enum EffectTag
        {
            EFFECTTAG_NONE,
            EFFECTTAG_KRANKHEIT
        };

        /// Setzt den Quantifier standardmaessig auf QUANTIFIER_MULTIPLE.
		Effect(int stufe = 1);
		virtual ~Effect();

        const CeGuiString getName() const;
        void setName(CeGuiString name);
        const CeGuiString getDescription() const;
        void setDescription(CeGuiString description);
        /// Liefert einen Zeiger auf die Kreatur, auf die dieser Effekt wirkt.
		Creature* getOwner() const;
        /// Weist diesen Effekt einer Kreatur zu.
		void setOwner(Creature* owner);
		virtual int getDuration();
		virtual void setDuration(int newDuration);
        /// @see Quantifier
		Quantifier getQuantifier();
        /// @see Quantifier
		void setQuantifier(Quantifier quantifier);
		/** 
         *  Adds a tag to the effect. 
         *  Tags are describing the type of an Effect, so other Effects can 
         *  identify it. An example would be 'Poison', as well as 'mineralic 
         *  poison'.
         **/
		virtual void addTag(EffectTag tagId);
        /**
         * Removes a tag from the effect.
         * @see addTag
         **/
		virtual void removeTag(EffectTag tagId);
        /**
         * Queries the effect for its tags.
         * @see addTag
         **/
		virtual bool queryTag(EffectTag tagId);
        /**
         * Returns the Stufe (level) of the effect.
         **/
		virtual const int getStufe();
        /**
         * Increases the level by one.
         **/
		virtual void increaseStufe();
        /**
         * Decreases the level by one.
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

        /// Liste aller Merkmale dieses Effekts.
		typedef std::set<EffectTag> Tags;
		Tags mTags;

        /// Zeiger auf die Kreatur, auf die dieser Effekt wirkt.
		Creature* mOwner;
	};
}

#endif //__EFFECT_H__
