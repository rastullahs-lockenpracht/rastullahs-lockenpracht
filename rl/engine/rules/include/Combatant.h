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

#include "Creature.h"

namespace rl
{
    class Combat;
    class CreatureController;
    class Kampfaktion;
    class JobSet;
    class Weapon;

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

        /// Type name used by factory
        virtual Ogre::String getCombatantTypeName() const = 0;

        CreatureController* getCreatureController() const;

        // convenience getters

        Creature* getCreature() const;
        CeGuiString getName() const;
        Ogre::Vector3 getPosition() const;

        Weapon* getActiveWeapon() const;

        /**
         *  Fuehrt eine Attacke aus. Die Funktion wird von einem Angriffsmanoever
         *  aufgerufen.
         *  @retval RESULT_ERFOLG Eine erfolgreiche Attacke.
         *  @retval RESULT_MISSERFOLG Eine fehlgeschlagene Attacke.
         *  @retval RESULT_GLUECKLICH Eine gute Attacke.
         *  @retval RESULT_PATZER Ein bestaetigter Attackepatzer.
         **/
        int rollAttacke();

        /**
         *  Fuehrt eine Parade aus. Die Funktion wird von einem Parademanoever
         *  aufgerufen.
         *  @param guteParade Versucht eine gute Parade (bei einem gluecklichen Treffer)
         *  @retval RESULT_ERFOLG Eine erfolgreiche Parade.
         *  @retval RESULT_MISSERFOLG Eine fehlgeschlagene Parade.
         *  @retval RESULT_GLUECKLICH Eine gute Parade.
         *  @retval RESULT_PATZER Ein bestaetigter Paradepatzer.
         *  @throws InvalidArgumentException Kampftechnik nicht in
         *   \c mKampftechniken gefunden.
         **/
        int rollParade(bool gluecklich);

        int rollTrefferpunkte(DamageStrength damage) const;
        int applyTrefferpunkte(int tp);

        void doAttacke(JobSet* jobSet, Combatant* target, int attackeResult, bool parade, int paradeResult = 0);
        void doParade(JobSet* jobSet, Combatant* target, int paradeResult);
        void doBewegen(JobSet* jobSet, const Ogre::Vector3& targetPos);
        void doFolgen(JobSet* jobSet, Combatant* target);
        void doGetroffen(JobSet* jobSet);

    protected:
        Combat* mCombat;
        CreatureController* mController;

        /// Converts canonic animation to actual animation name inside the skeleton
        /// According to the mapping defined in Creature gof.
        Ogre::String getMeshAnimationName(const Ogre::String animationName);
    };

    class _RlRulesExport CombatantFactory
    {
    public:
        virtual ~CombatantFactory()
        {
        }

        virtual Combatant* createCombatant(Creature* creature) = 0;
        virtual void destroyCombatant(Combatant*) = 0;
    };
}

#endif
