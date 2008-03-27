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

#ifndef __EFFECTMANAGER_H__
#define __EFFECTMANAGER_H__

#include "RulesPrerequisites.h"

#include <set>

#include "Date.h"
#include "Effect.h"

namespace rl
{
    class GameObject;

    /**
     * @brief Manages the effects affecting a game object
     * 
     * This class manages all effects applied to a game object.
     * Each game object has a seperate EffectManager. The method checkEffects()
     * should be called every Aktion (the dark eye time unit)
     **/

	class _RlRulesExport EffectManager
	{
	public:
		EffectManager(GameObject* gameobject);
		~EffectManager();

        /**
         *  Checks effects for changes.
         **/
		void checkEffects();
        /** 
         *  Mit dieser Funktion kann ein Effekt dem Manager mitteilen,
         *  in welcher Zeit er wieder ueberprueft werden muss.
         *  @param time Die Zeitspanne wann die naechste Ueberpruefung faellig ist.
         *  @param effect Der zu ueberpruefende Effekt.
         *  @throws IllegalArgumentException time parameter is <= 0
         *  @throws IllegalArgumentException effect pointer is NULL
         *  @see addCheckDate Date
         **/
        void addTimeCheck(RL_LONGLONG timeUntilCheck, Effect* effect);
        /**
         *  Mit dieser Funktion kann ein Effekt dem Manager mitteilen,
         *  zu welchem Zeitpunkt er wieder ueberprueft werden muss.
         *  @param date Das Datum der naechsten Ueberpruefung.
         *  @param effect Der zu ueberpruefende Effekt.
         *  @throws IllegalArgumentException date lies in the past
         *  @throws IllegalArgumentException effect pointer is NULL
         *  @see addCheck, Date
         **/
        void addDateCheck(RL_LONGLONG date, Effect* effect);
        /**
         * Fuegt einen neuen Effekt in die Liste ein.
         * @param effect Ein Zeiger auf den einzufuegenden Effekt.
         **/
		void addEffect(Effect* effect);
        
        /// Removes effects by pointer.
        void removeEffect(Effect* effect);

        /// Removes effects by name.
        void removeEffect(CeGuiString name);
        
        /**
         * Checks the effects for the given status.
         **/
        Effect::LifeState getLifeState();
        
        /**
         * Returns the specified modificator.
         * @param target specifies what modificator you want to get (e.g. "Athletik")
         * @param type specifies what type of modificator you want (e.g. modificator of the TaW or the Probenzuschlag)
         * @param tag add some additional restrictions (e.g. KO Probe for sicknesses or Athletik for balance acts)
         **/
        int getMod(CeGuiString target, Effect::ModType type, Effect::ModTag tag = Effect::MODTAG_NONE);

        /**
         * Returns a printable list of all effects
         **/
        CeGuiString getEffects();

        void checkStateChange(Effect::LifeState oldState);

	private:

        GameObject* mGameObject;

        /// Eine einfache Menge von Effekten
		typedef std::set<Effect*> Effects;
		Effects mEffects;
        /**
         * Eine Liste die wiedergibt, wann welcher Effekt
         * ueberprueft werden muss.
         **/
        typedef std::map<RL_LONGLONG, Effects> Checklist;
        Checklist mChecklist;
	};
}

#endif //__EFFECTMANAGER_H__
		
