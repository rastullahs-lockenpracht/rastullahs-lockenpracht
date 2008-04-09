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
#include "stdinc.h"

#include "Combat.h"

#include "Combatant.h"
#include "CombatManager.h"
#include "CreatureController.h"
#include "Effect.h"
#include "GameEventLog.h"
#include "Inventory.h"
#include "Job.h"
#include "JobQueue.h"
#include "JobScheduler.h"
#include "MathUtil.h"
#include "MessagePump.h"
#include "RulesMessages.h"
#include "Weapon.h"


using namespace boost;
using namespace boost::tuples;
using namespace Ogre;

namespace rl
{
    struct InitiativeComparator
		: std::binary_function<std::pair<int, Combatant*>, std::pair<int, Combatant*>, bool>
    {
        /// \todo take into account other values of the combatant according to TDE rules,
        /// in case initiative is equal.
        bool operator()(const std::pair<int, Combatant*>& c1,
            const std::pair<int, Combatant*>& c2) const
        {
            return c1.first > c2.first;
        }
    };

	struct FindCombatantByCreature : std::binary_function<Combatant*, Creature*, bool>
    {
        bool operator()(Combatant* combatant, Creature* creature) const
        {
			return combatant->getCreature() == creature;
        }
    };

    Combat::Combat()
        : mOwnedCombatants(),
		  mOpponents(),
          mAllies(),
          mCombatantQueue(),
          mCombatantActions(),
		  mCombatantReactions(),
		  mCancelledActions(),
		  mRemovedCombatants(),
          mCurrentRound(0),
		  mNextActionId(0),
		  mAnimationSequenceTicket(0)
    {
		mLifeStateChangeConnection =
            MessagePump::getSingleton().addMessageHandler<MessageType_GameObjectLifeStateChanged>(
			    boost::bind(&Combat::onGameObjectLifeStateChanged, this, _1, _2, _3));
    }

    Combat::~Combat()
    {
    }

    void Combat::addOpponent(Combatant* combatant)
    {
        mOpponents.insert(combatant);
		GameEventLog::getSingleton().logEvent(
			combatant->getName() + " wurde in einen Kampf verwickelt.", GET_COMBAT);
		MessagePump::getSingleton().sendMessage<MessageType_CombatOpponentEntered>(combatant);
    }

    Combatant* Combat::addOpponent(Creature* creature)
	{
		Combatant* combatant = CombatManager::getSingleton().createCombatant(creature);
		mOwnedCombatants.insert(combatant);
        addOpponent(combatant);
		return combatant;
	}

    void Combat::removeOpponent(Combatant* combatant)
    {
        mOpponents.erase(combatant);
		GameEventLog::getSingleton().logEvent(
			combatant->getName() + " nimmt nicht mehr am Kampf teil.", GET_COMBAT);
		MessagePump::getSingleton().sendMessage<MessageType_CombatOpponentLeft>(combatant);
		checkAndMarkCombatant(combatant);
    }

    void Combat::addAlly(Combatant* combatant)
    {
        mAllies.insert(combatant);
    }

    Combatant* Combat::addAlly(Creature* creature)
	{
		Combatant* combatant = CombatManager::getSingleton().createCombatant(creature);
		mOwnedCombatants.insert(combatant);
        addAlly(combatant);
		return combatant;
	}

    void Combat::removeAlly(Combatant* combatant)
    {
        mAllies.erase(combatant);
		GameEventLog::getSingleton().logEvent(
			combatant->getName() + " nimmt nicht mehr am Kampf teil.", GET_COMBAT);
		checkAndMarkCombatant(combatant);
    }

	void Combat::checkAndMarkCombatant(Combatant* combatant)
	{
		// Cancel any action this combatant takes part in.
		for (CombatantActionsMap::iterator it = mCombatantActions.begin();
			it != mCombatantActions.end(); ++it)
		{
			for (size_t i = 0; i < it->second.size(); ++i)
			{
				const ActionEntry& ae = it->second[i];
				if (ae.actor == combatant || ae.target == combatant)
				{
					mCancelledActions.insert(it->second[i].id);
				}
			}
		}
		mRemovedCombatants.insert(combatant);
	}

	void Combat::clearRemovedCombatantSet()
	{
		for (CombatantSet::iterator i = mRemovedCombatants.begin();
			i != mRemovedCombatants.end(); ++i)
		{
			// Are we owner of this combatant? If so, delete it.
			CombatantSet::iterator it = std::find(
				mOwnedCombatants.begin(), mOwnedCombatants.end(), *i);
			if (it != mOwnedCombatants.end())
			{
				Combatant* combatant = *it;
				mOwnedCombatants.erase(it);
				CombatManager::getSingleton().destroyCombatant(combatant);
			}
		}
		mRemovedCombatants.clear();
	}

    const Combat::CombatantSet& Combat::getAllOpponents() const
    {
        return mOpponents;
    }

    const Combat::CombatantSet& Combat::getAllAllies() const
    {
        return mAllies;
    }

    void Combat::start()
    {
		GameEventLog::getSingleton().logEvent("Kampf beginnt.", GET_COMBAT);

        mCurrentRound = 0;
		mCombatantQueue.clear();

        std::vector<Combatant*> combatants;
        combatants.insert(combatants.end(), mAllies.begin(), mAllies.end());
        combatants.insert(combatants.end(), mOpponents.begin(), mOpponents.end());
        // Calculate initiative for all participiants
        for (std::vector<Combatant*>::const_iterator it = combatants.begin();
            it != combatants.end(); ++it)
        {
            mCombatantQueue.push_back(std::make_pair(
                (*it)->getCreatureController()->getCreature()->doInitiativeWurf(), *it));
        }
        std::sort(mCombatantQueue.begin(), mCombatantQueue.end(), InitiativeComparator());

        beginRound();
    }

    void Combat::stop()
    {
    }

	void Combat::registerAttacke(Combatant* actor, Combatant* target)
	{
		ActionEntry entry;
		entry.id = mNextActionId++;
		entry.aktion = ATTACKE;
		entry.actor = actor;
		entry.target = target;

		mCombatantActions[actor].push_back(entry);
	}

	void Combat::registerParade(Combatant* actor)
	{
		mCombatantReactions[actor] = PARADE;
	}

	void Combat::registerAusweichen(Combatant* actor)
	{
		mCombatantReactions[actor] = AUSWEICHEN;
	}

	void Combat::registerBewegen(Combatant* actor, const Ogre::Vector3& targetPos)
	{
		ActionEntry entry;
		entry.id = mNextActionId++;
		entry.aktion = BEWEGEN;
		entry.actor = actor;
		entry.targetPos = targetPos;

		mCombatantActions[actor].push_back(entry);
	}

	void Combat::registerFolgen(Combatant* actor, Combatant* target)
	{
		ActionEntry entry;
		entry.id = mNextActionId++;
		entry.aktion = FOLGEN;
		entry.actor = actor;
		entry.target = target;

		mCombatantActions[actor].push_back(entry);
	}

	void Combat::registerCombatantRoundDone(Combatant* combatant)
	{
		mFinishedCombatants.insert(combatant);
        // Are all combatants registered now?
        if (mFinishedCombatants.size() == mCombatantQueue.size())
        {
            executeRound();
        }
	}

	bool Combat::canAttack(Combatant* actor, Combatant* target) const
	{
		// Does the actor have a weapon readied?
		if (actor->getActiveWeapon() == NULL)
		{
			return false;
		}
		// Is target in weapon range?
		Real distance = MathUtil::distance(target->getCreature()->getWorldBoundingBox(),
			actor->getCreature()->getWorldBoundingBox());
		if (distance > getMaximumAttackeDistance(actor))
		{
			return false;
		}
		return true;
	}

	Ogre::Real Combat::getMaximumAttackeDistance(Combatant* actor) const
	{
		std::vector<Weapon*> weapons = actor->getCreature()->getInventory()->getReadiedWeapons();
		Real distance = 0.0f;
		for (size_t i = 0; i < weapons.size(); ++i)
		{
			distance = std::max(weapons[i]->getMaximumDistance(), distance);
		}
		return distance;
	}

    void Combat::beginRound()
    {
        // Increment round counter
        mCurrentRound++;

        mCombatantActions.clear();
		mCombatantReactions.clear();
		mCancelledActions.clear();
		mFinishedCombatants.clear();

		GameEventLog::getSingleton().logEvent("Runde " + CeGuiString(StringConverter::toString(mCurrentRound)),
			GET_COMBAT);
		// Show ini for this round.
        for (CombatantQueue::iterator it = mCombatantQueue.begin();
			it != mCombatantQueue.end(); ++it)
        {
			CeGuiString evt = it->second->getName();
			evt += " - Initiative: " + CeGuiString(StringConverter::toString(it->first));
			GameEventLog::getSingleton().logEvent(evt, GET_COMBAT);
        }

        // Request actions from combatants
        for (CombatantQueue::iterator it = mCombatantQueue.begin();
            it != mCombatantQueue.end(); ++it)
        {
            it->second->requestCombatantAction();
        }
    }

    void Combat::executeRound()
	{
		// Auf gehts!

		// Prepare JobQueue for animations.
		JobQueue* jobQueue = new JobQueue();

		for (size_t actionIndex = 0; actionIndex < 3; ++actionIndex)
		{
			JobSet* jobSet = new JobSet();

			for (CombatantQueue::iterator it = mCombatantQueue.begin();
				it != mCombatantQueue.end(); ++it)
			{
				Combatant* combatant = it->second;

			    // Is there an action registed for combatant?
			    CombatantActionsMap::iterator actionsIt = mCombatantActions.find(combatant);
			    if (actionsIt != mCombatantActions.end())
			    {
				    // Do we have an action for this pass ?
				    if (actionIndex < actionsIt->second.size())
				    {
					    // Yes we do.
					    ActionEntry entry = actionsIt->second[actionIndex];

						// Ignore action if cancelled
						if (mCancelledActions.find(entry.id) != mCancelledActions.end())
						{
							continue;
						}

					    if (entry.aktion == ATTACKE)
					    {
						    // Check whether possible
						    if (canAttack(combatant, entry.target))
						    {
							    doAttacke(jobSet, combatant, entry.target);
						    }
					    }
					    else if (entry.aktion == BEWEGEN)
					    {
						    GameEventLog::getSingleton().logEvent(combatant->getName() + " läuft nach "
							    + CeGuiString(StringConverter::toString(entry.targetPos)), GET_COMBAT);
						    combatant->doBewegen(jobSet, entry.targetPos);
					    }
					    else if (entry.aktion == FOLGEN)
					    {
						    GameEventLog::getSingleton().logEvent(combatant->getName() + " läuft zu "
							    + entry.target->getName(), GET_COMBAT);
						    combatant->doFolgen(jobSet, entry.target);
					    }
				    }
				}
			}
			jobQueue->add(jobSet);
		}
		mAnimationSequenceTicket = JobScheduler::getSingleton().addJob(jobQueue,
			JobScheduler::JP_NORMAL, 0.0f, Ogre::Math::POS_INFINITY, this);
	}

    void Combat::endRound()
    {
		clearRemovedCombatantSet();
        // All actions executed. Analyze outcome of this round.
        if (mAllies.empty())
        {
            MessagePump::getSingleton().sendMessage<MessageType_CombatEnded>(false);
        }
        else if (mOpponents.empty())
        {
            MessagePump::getSingleton().sendMessage<MessageType_CombatEnded>(true);
        }
        else 
        {
            // Refill combatant queue with combatants that are still alive.
		    beginRound();
        }
    }

	void Combat::doAttacke(JobSet* jobSet, Combatant* actor, Combatant* target)
	{
        GameEventLog::getSingleton().logEvent(
			actor->getName() + " attackiert " + target->getName() , GET_COMBAT);
        
        bool rollDamage = false;
		// Make an attack roll.
		int aresult = actor->rollAttacke();
		if (aresult >= RESULT_ERFOLG)
		{
			// Ok, succeeded
			// Has target registered a reaction?
			CombatantReactionsMap::iterator it = mCombatantReactions.find(target);
			if (it != mCombatantReactions.end())
			{
				// Yes, there is a reaction
				if (it->second == PARADE)
				{
					int presult = target->rollParade(aresult >= RESULT_GLUECKLICH);
					if (presult >= RESULT_ERFOLG)
					{
						GameEventLog::getSingleton().logEvent("Erfolg, aber pariert.", GET_COMBAT);
					}
					else
					{
						GameEventLog::getSingleton().logEvent("Erfolg, nicht pariert, Treffer!",
							GET_COMBAT);
						rollDamage = true;
					}
					target->doParade(jobSet, actor, presult);
					actor->doAttacke(jobSet, target, aresult, true, presult);
				}
				else if (it->second == AUSWEICHEN)
				{
					// Faellt aus wegen is nich.
				}
			}
			else
			{
				GameEventLog::getSingleton().logEvent("Treffer!", GET_COMBAT);
				actor->doAttacke(jobSet, target, aresult, false);
				rollDamage = true;
			}
		}
		else
		{
			GameEventLog::getSingleton().logEvent("Verfehlt!", GET_COMBAT);
			actor->doAttacke(jobSet, target, aresult, false);
			target->doGetroffen(jobSet);
		}
		
		if (rollDamage)
		{
			int tp = actor->rollTrefferpunkte();
			int sp = target->applyTrefferpunkte(tp);
			CeGuiString msg = actor->getName() + " trifft für "
				+ CeGuiString(StringConverter::toString(tp))
				+ " Trefferpunkte, " + target->getName() + " erleidet "
				+ CeGuiString(StringConverter::toString(sp)) + " Schadenspunkte.";
			GameEventLog::getSingleton().logEvent(msg, GET_COMBAT);
		}
	}

    void Combat::jobFinished(unsigned long ticket)
	{
		if (ticket == mAnimationSequenceTicket)
		{
			endRound();
		}
	}

    bool Combat::onGameObjectLifeStateChanged(GameObject* gameobject, Effect::LifeState oldstate,
		Effect::LifeState newstate)
	{
        if (newstate & Effect::LS_NO_COMBAT)
		{
			// Is a creature in combat affected?
			for (size_t i = 0; i < mCombatantQueue.size(); ++i)
			{
                Creature* curCreature = mCombatantQueue[i].second->getCreature();
				if (gameobject == curCreature)
				{
					// Yes. Log about it and remove it from combat.
					CeGuiString msg = curCreature->getName() + " ist jetzt ";
					if (newstate == Effect::LS_INCAPACITATED)
					{
						msg += "kampfunfähig.";
					}
                    else if (newstate == Effect::LS_UNCONSCIOUS)
					{
						msg += "bewusstlos.";
					}
					else
					{
						msg += "tot.";
					}
					GameEventLog::getSingleton().logEvent(msg, GET_COMBAT);

					CombatantSet::iterator it = std::find_if(mOpponents.begin(), mOpponents.end(),
						std::bind2nd(FindCombatantByCreature(), curCreature));
					if (it != mOpponents.end())
					{
						removeOpponent(mCombatantQueue[i].second);
					}
					else
					{
						CombatantSet::iterator it = std::find_if(mAllies.begin(), mAllies.end(),
							std::bind2nd(FindCombatantByCreature(), curCreature));
						if (it != mOpponents.end())
						{
							removeAlly(mCombatantQueue[i].second);
						}
					}
					break;
				}
			}
		}
		return true;
	}
}
