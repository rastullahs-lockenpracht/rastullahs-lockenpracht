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

#include <set>
#include <vector>
#include <boost/tuple/tuple.hpp>

#include "JobListener.h"
#include "Kampfaktion.h"
#include "MessagePump.h"
#include "RulesConstants.h"
#include "Effect.h"

namespace rl
{
    class Combatant;
	class Creature;
	class GameObject;
	class JobSet;

	class _RlRulesExport Combat : public JobListener
    {
    public:
        typedef std::set<Combatant*> CombatantSet;

        Combat();
        ~Combat();

        void addOpponent(Combatant*);
        Combatant* addOpponent(Creature*);

        void removeOpponent(Combatant*);

        void addAlly(Combatant*);
        Combatant* addAlly(Creature*);

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


    private:
		typedef enum {ATTACKE, BEWEGEN, FOLGEN} Aktion;
		typedef enum {PARADE, AUSWEICHEN} Reaktion;
		struct ActionEntry
		{
			int id;
			Aktion aktion;
			Combatant* actor;
			Combatant* target;
			Ogre::Vector3 targetPos;
		};
		typedef std::vector<ActionEntry> ActionEntryVector;
		typedef std::map<Combatant*, ActionEntryVector> CombatantActionsMap;
		typedef std::map<Combatant*, Reaktion> CombatantReactionsMap;
        typedef std::vector<std::pair<int, Combatant*> > CombatantQueue;

        /// Combatants owned by this Combat are also stored here.
		/// This is needed, in order to destroy removed combatant instances properly.
		CombatantSet mOwnedCombatants;

		CombatantSet mOpponents;
        CombatantSet mAllies;
        /// Combatants in order of their initiative for the current round.
        CombatantQueue mCombatantQueue;
        CombatantActionsMap mCombatantActions;
		CombatantReactionsMap mCombatantReactions;
		/// If a combatant is removed from combat it becomes invalid.
		/// And thus can't be neither actors nor targets of actions.
		/// This set stores such action ids.
		std::set<int> mCancelledActions;
		/// Store combatants that are removed in current round.
		/// Instead of destroying them the moment they are out of combat,
		/// we destroy them at the end of the current combat round.
		/// This prevents all kinds of problems that dangling pointers would cause else.
		CombatantSet mRemovedCombatants;
		// Combatants who have registered all their actions for this round
		CombatantSet mFinishedCombatants;
		unsigned long mAnimationSequenceTicket;

        unsigned short mCurrentRound;
		unsigned short mNextActionId;

		MessagePump::ScopedConnection mLifeStateChangeConnection;

        void beginRound();
        void executeRound();
        void endRound();

		void doAttacke(JobSet* jobSet, Combatant* actor, Combatant* target);

		Ogre::Real getMaximumAttackeDistance(Combatant* actor) const;

		/// Mark any action regarding this combatant as invalid.
		/// Mark combatant as removed.
		void checkAndMarkCombatant(Combatant* combatant);

		/// Clear combatant list of removed combatants,
		/// destroy combatants owned by this combat instance.
		void clearRemovedCombatantSet();

        // Message handlers

        bool onGameObjectLifeStateChanged(GameObject*, Effect::LifeState, Effect::LifeState);
    };
}

#endif
