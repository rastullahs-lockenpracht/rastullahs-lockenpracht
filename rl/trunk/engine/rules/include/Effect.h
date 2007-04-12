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
	class _RlRulesExport Effect
	{
	public:
      
        typedef unsigned long Status;
        static const Status STATUS_NONE           = 1<<0;
        static const Status STATUS_BLIND          = 1<<1;   ///< The creature can't see anything.
        static const Status STATUS_DEAD           = 1<<2;   ///< The creature is dead.
        static const Status STATUS_DEAF           = 1<<3;   ///< The creature can't hear anything.
        static const Status STATUS_PARALYZED      = 1<<4;   ///< The creature can't move, turn, jump etc.
        static const Status STATUS_INCAPACITATED  = 1<<5;   ///< The creature can't fight anymore due to serious injuries. The creature can barely move.
        static const Status STATUS_INVISIBLE      = 1<<6;   ///< The creature can't be seen.
        static const Status STATUS_INVULNERABLE   = 1<<7;   ///< The creature is immune to any mundane attacks.
        static const Status STATUS_PETRIFIED      = 1<<8;   ///< The creature is coated with stone and has all the effects of paralyzation
        static const Status STATUS_SILENCED       = 1<<9;   ///< The creature doesn't make any noises.
        static const Status STATUS_SLEEPING       = 1<<10;  ///< The creature is asleep.
        static const Status STATUS_UNCONSCIOUS    = 1<<11;  ///< Like SLEEPING, but more difficult to wake.

        // Meta stati
        static const Status STATUS_IMMOBILE = STATUS_PARALYZED | STATUS_SLEEPING | STATUS_UNCONSCIOUS | STATUS_PETRIFIED | STATUS_DEAD;

        typedef int ModType;
        static const ModType MODTYPE_NONE         = 1 << 0;
        static const ModType MODTYPE_SUM      = 1 << 1;   ///< Modifies the actual value.
        static const ModType MODTYPE_PROBENMOD    = 1 << 2;   ///< Modifies the test.
        static const ModType MODTYPE_MULT     = 1 << 3;   ///< Will be multiplied on the value.
        static const ModType MODTYPE_MAXWERTMOD   = 1 << 4;   ///< Modifies only the maximum of the value.

        typedef unsigned long ModTag;
        static const ModTag MODTAG_NONE           = 1 << 0;
        static const ModTag MODTAG_KRANKHEIT      = 1 << 1;
        static const ModTag MODTAG_RECALCULATE    = 1 << 2;
        static const ModTag MODTAG_REGENERATION_LE = 1 << 3;
        static const ModTag MODTAG_REGENERATION_AE = 1 << 4;
        static const ModTag MODTAG_REGENERATION_AU = 1 << 5;
        static const ModTag MODTAG_ERSCHOEPFUNGSSCHWELLE = 1 << 6;


        // other constants
        static const int REMOVE = -1;
        static const int PERMANENT = 0;

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
        /// @see Quantifier
		Quantifier getQuantifier();
        /// @see Quantifier
		void setQuantifier(Quantifier quantifier);
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

		/**
         * runs a check on the effect.
         * @return Returns the time to the next check. If REMOVE is returned, the 
         * effect will be removed, if PERMANENT is returned, the effect lasts permanently.
         **/
		virtual RL_LONGLONG check();
        
        /// Returns the status that is applied to the creature by this effect.
        virtual Status getStatus();
        
        /// Returns the modificator applied on the value specified by the parameters.
        virtual int getMod(CeGuiString target, ModType type, ModTag tag);

	protected:
		CeGuiString mName;
		CeGuiString mDescription;
		/// Dauer desEffekts in Aktionen.
		int mStufe;
		/// Zeitpunkt des Beginns des Effekts.
		RL_LONGLONG mStartTime;
		/// Speichert, ob der Effekt gerade wirksam ist.
		bool mEnabled;
		/// Gibt an, wie oft ein Effekt dieses Namens auf einer Kreatur wirken kann.
		Quantifier mQuantifier;
	};
}

#endif //__EFFECT_H__
