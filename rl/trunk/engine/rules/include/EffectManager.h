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

#ifndef __EFFECTMANAGER_H__
#define __EFFECTMANAGER_H__

#include "RulesPrerequisites.h"
#include "Date.h"
#include "Effect.h"
#include <set>


namespace rl
{
    /**
     * @brief Verwaltet die Effekte die auf einer Kreatur wirken
     *
     * Diese Klasse verwaltet alle Effekte, die auf einer Kreatur liegen.
     * Jede Kreatur hat ihren eigenen EffectManager. Die Funktion checkeffects
     * sollte während jeder Aktion (gemeint ist die DSA-Zeiteinheit) aufgerufen
     * werden.
     **/

	class _RlRulesExport EffectManager
	{
	public:
		EffectManager();
		~EffectManager();

        /**
         * @brief Ruft fuer jeden Effekt in der Liste die isAlive() Funktion auf.
         **/
		void checkEffects();
        /** 
         * @brief Mit dieser Funktion kann ein Effekt dem Manager mitteilen,
         * wann er wieder ueberprueft werden muss.
         * @param time Die Zeit wann die naechste Ueberpruefung faellig ist.
         * @param effect Der zu ueberpruefende Effekt.
         * @sa Die Date Klasse.
         **/
        void addCheck(RL_LONGLONG time, Effect* effect);
        /**
         * @brief Fuegt einen neuen Effekt in die Liste ein.
         * @param effect Ein Zeiger auf den einzufuegenden Effekt.
         **/
		void addEffect(Effect* effect);

	private:
		typedef std::set<Effect*> Effects;
        typedef std::map<Effect::EffectTag, Effects> Watchlist;
        typedef std::map<RL_LONGLONG, Effects> Checklist;
        /// @brief Eine einfache Menge von Effekten
		Effects mEffects;
        /**
         * @brief Eine Liste von Tags und Effekten die auf das hinzufuegen
         * eines Effekts mit diesem Tag reagieren.
         **/
		Watchlist mWatchlist;
        /**
         * @brief Eine Liste die wiedergibt, wann welcher Effekt
         * ueberprueft werden muss.
         **/
        Checklist mChecklist;
	};
}

#endif //__EFFECTMANAGER_H__
		
