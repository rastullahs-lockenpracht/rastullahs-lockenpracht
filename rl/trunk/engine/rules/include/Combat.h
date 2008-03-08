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

#ifndef __RL_COMBAT_H__
#define __RL_COMBAT_H__

#include "RulesPrerequisites.h"

#include "JobListener.h"
#include "Kampfaktion.h"
#include "MessagePump.h"
#include "RulesConstants.h"

#include <set>
#include <vector>
#include <boost/tuple/tuple.hpp>

namespace rl
{
    class Combatant;
	class Creature;
	class JobSet;

	class _RlRulesExport Combat : public JobListener
    {
    public:
        typedef std::set<Combatant*> CombatantSet;

        Combat();
        ~Combat();

        void addOpponent(Combatant*);
        void removeOpponent(Combatant*);

        void addAlly(Combatant*);
        void removeAlly(Combatant*);

        const CombatantSet& getAllOpponents() const;
        const CombatantSet& getAllAllies() const;

        void start();
        void stop();

        // Called by combatants in response to a request by the Combat object.
        // With calling one of these functions combatants register their actions for this round.

		void registerAttacke(Combatant* actor, Combatant* target);
        void registerParade(Combatant* actor);
        void registerAusweichen(Combatant* actor);
		void registerBewegen(Combatant* actor, const Ogre::Vector3& targetPos);
		void registerFolgen(Combatant* actor, Combatant* target);
		void registerCombatantRoundDone(Combatant* actor);

		bool canAttack(Combatant* actor, Combatant* target) const;

		// JobListener overrides

        virtual void jobFinished(unsigned long ticket);

		// Message handlers

		bool creatureLifeStateChanged(Creature*, LifeState);

    private:
		typedef enum {ATTACKE, BEWEGEN, FOLGEN} Aktion;
		typedef enum {PARADE, AUSWEICHEN} Reaktion;
		struct ActionEntry
		{
			Aktion aktion;
			Combatant* actor;
			Combatant* target;
			Ogre::Vector3 targetPos;
		};
		typedef std::vector<ActionEntry> ActionEntryVector;
		typedef std::map<Combatant*, ActionEntryVector> CombatantActionsMap;
		typedef std::map<Combatant*, Reaktion> CombatantReactionsMap;
        typedef std::vector<std::pair<int, Combatant*> > CombatantQueue;

        CombatantSet mOpponents;
        CombatantSet mAllies;
        /// Combatants in order of their initiative for the current round.
        CombatantQueue mCombatantQueue;
        CombatantActionsMap mCombatantActions;
		CombatantReactionsMap mCombatantReactions;
		// Combatants who have registered all their actions for this round
		CombatantSet mFinishedCombatants;
		unsigned long mAnimationSequenceTicket;

        unsigned short mCurrentRound;

		MessagePump::ScopedConnection mLifeStateChangeConnection;

        void beginRound();
        void executeRound();
        void endRound();

		void doAttacke(JobSet* jobSet, Combatant* actor, Combatant* target);

		Ogre::Real getMaximumAttackeDistance(Combatant* actor) const;
    };
}

#endif
