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

#include "Combat.h"
#include "CombatActions.h"
#include "CombatEvents.h"
#include "Creature.h"
#include "Exception.h"
#include "DsaManager.h"
#include "Date.h"

using namespace std;


namespace rl {

	int Combat::CombatEvt::sId = 0;

	Combat::Combat(int slowMotionFactor)
		: mEventCasters(),
		mSlowMotionFactor(slowMotionFactor),
		mIsSlowMotion(false),
		mTimeInKampfrunde(0),
		mStarted(false)
	{
	}

	Combat::~Combat()
	{
	}

	void Combat::add(Creature* creature, int group)
	{
		Participant* part = new Participant(creature, group, mParticipants.size() + 1);
		initialize(part);
		mParticipants.insert(make_pair(creature, part));
	}

	void Combat::initialize(Participant* part)
	{
		//TODO: Aktuelle Waffe, INI wuerfeln
		part->initiative = part->creature->doInitiativeWurf();
	}

	Combat::Participant::Participant(Creature* creature, int group, int id)
	{
		this->id = id;
		this->creature = creature;
		this->group = group;
		this->initiative = NO_INI;
		this->attackeTarget = NULL;
		this->paradeTarget = NULL;
		this->nextAction = NULL;
		this->nextReaction = NULL;
	}

	Combat::CombatEvt::CombatEvt(int ini, int iniIdx, bool isAction, Participant* part, CombatAction* action)
	{
		this->ini = ini;
		this->iniIdx = iniIdx;
		this->isAction = isAction;
		this->part = part;
		this->action = action;
		this->id = sId++;
	}

	int Combat::getGroupOf(Creature* creature)
	{
		return getParticipant(creature)->group;
	}

	vector<Creature*> Combat::getGroupMembers(int group)
	{
		vector<Creature*> members;

		for (CombatMap::iterator partIter = mParticipants.begin();
			partIter != mParticipants.end(); partIter++)
		{
			Participant* part = (*partIter).second;
			if (part->group == group)
				members.push_back(part->creature);
		}

		return members;
	}

	Combat::Participant* Combat::getParticipant(Creature* creature)
	{
		CombatMap::iterator iter = mParticipants.find(creature); 
		if (iter == mParticipants.end())
			Throw(InvalidArgumentException, "Wesen nimmt nicht am Kampf teil.");
		return (*iter).second;
	}

	Creature* Combat::getNext()
	{
		return NULL; //TODO
	}

	Creature* Combat::getNext(int group)
	{
		return getGroupMembers(group)[0]; //TODO
	}

	Creature* Combat::getAttackeTarget(Creature* creature)
	{
		return getParticipant(creature)->attackeTarget;
	}

	void Combat::setAttackeTarget(Creature* creature, Creature* target)
	{
		getParticipant(creature)->attackeTarget = target;
	}

	Creature* Combat::getParadeTarget(Creature* creature)
	{
		return getParticipant(creature)->paradeTarget;
	}

	void Combat::setParadeTarget(Creature* creature, Creature* target)
	{
		getParticipant(creature)->paradeTarget = target;
	}

	void Combat::setNextAction(Creature* creature, CombatAction* action, int eventId)
	{
		getParticipant(creature)->nextAction = action;
		if (eventId != -1)
		{
			for (std::vector<CombatEvt*>::iterator it = mEventStack.begin(); it != mEventStack.end(); it++)
			{
				if ((*it)->id == eventId)
					(*it)->action = action;
			}

			for (CombatEventList::iterator it = mEventList.begin(); it != mEventList.end(); it++)
			{
				if ((*it).second->id == eventId)
					(*it).second->action = action;
			}
		}
	}

	void Combat::setNextReaction(Creature* creature, CombatAction* reaction, int eventId)
	{
		getParticipant(creature)->nextReaction = reaction;
	}

	Ogre::Real Combat::getMaxMoveDistance(MoveType action)
	{
		//TODO: Geschwindigkeit
		switch (action)
		{
		case MT_NO_WALK:
			return 0;
		case MT_WALK_IN_AT_PHASE:
		case MT_WALK_IN_PA_PHASE:
			return 300;
		case MT_WALK_IN_AT_PA_PHASE:
			return 600;
		}

		Throw(InvalidArgumentException, "Unbekannter MoveType.");
	}

	void Combat::run(Ogre::Real elapsedTime)
	{
		if (!mStarted)
			return;

		if (mEventList.empty())
			initializeKampfrunde();

		mTimeInKampfrunde += mIsSlowMotion
								? elapsedTime / mSlowMotionFactor
								: elapsedTime;

		bool switchToNext = false;

		if (mEventStack.empty())
			switchToNext = true;
		else if (mTimeInKampfrunde - mCurrentEventStart >= mTimeOfAction)
		{
			sendStopEvent(getLastEvent());
			switchToNext = processEvent(getLastEvent());
		}

		if (switchToNext)
		{
			CombatEventList::const_iterator evtIt = findNextCombatEvent(mEventList);
			CombatEvt* evt = (*evtIt).second;
			mCurrentInitiative = evt->ini;
			mCurrentIniTime = evt->iniIdx;

			mEventStack.push_back(evt);
			mEventCasters[evt->part->group].dispatchEvent(new AskForActionEvent(this, evt->id, evt->part->creature, mTimeOfAction, mSlowMotionFactor));
			mIsSlowMotion = true;
		}
	}

	void Combat::addCombatEventListener(CombatEventListener* listener)
	{
		
		mEventCasters[listener->getGroup()].addEventListener(listener);
	}

	void Combat::removeCombatEventListener(CombatEventListener* listener)
	{
		mEventCasters[listener->getGroup()].removeEventListener(listener);
	}

	bool Combat::isInAttackDistance(Creature* attacker, Creature* target)
	{
		static Ogre::Real strikeDistance = 100; //TODO: (DK, Möglichkeit ohne DK)

		return (attacker->getActor()->getPosition()
				- target->getActor()->getPosition()).length() 
				<= strikeDistance; 
	}

	bool Combat::isActionPhaseDone(Creature* actor)
	{
		return false; //TODO
	}

	void Combat::initializeKampfrunde()
	{
		mTimeOfAction = Date::ONE_KAMPFRUNDE / (mParticipants.size() * 2); //TODO: Es gehen auch Anzahlen != 2 Aktionen pro Teilnehmer
		for (CombatMap::iterator partIter = mParticipants.begin();
				partIter != mParticipants.end(); partIter++)
		{
			Participant* part = (*partIter).second;


			if (part->nextKRAction != NULL)
			{
				part->nextAction = part->nextKRAction;
				part->nextKRAction = NULL;				
			}
			if (part->nextKRReaction != NULL)
			{
				part->nextReaction = part->nextKRReaction;
				part->nextKRReaction = NULL;				
			}

			part->pareesLeft = 1; //TODO: Mehr als 1 PA / KR

			//TODO: Gleiche INIs behandeln
			CombatTime actionTime = make_pair(part->initiative, 0);
			CombatTime reactionTime = make_pair(NO_INI, part->initiative*100);
			mEventList.insert(make_pair(actionTime, new CombatEvt(part->initiative, 0, true, part, part->nextAction)));
			mEventList.insert(make_pair(reactionTime, new CombatEvt(part->initiative, 0, false, part, part->nextReaction)));
		}

		mCurrentInitiative = INI_START;
		mTimeInKampfrunde = 0;
	}

	Combat::CombatEventList::const_iterator Combat::findNextCombatEvent(const CombatEventList& eventList)
	{
		int maxIni = NO_INI;
		long minTIme = INI_START;
		CombatEventList::const_iterator retVal = eventList.end();
		for (CombatEventList::const_iterator iter = eventList.begin(); iter != eventList.end(); iter++)
		{
			CombatTime time = (*iter).first;
			CombatEvt* evt = (*iter).second;
			
			if ( (		time.first > maxIni 
						|| (time.first == maxIni && time.second < minTIme))
				&& ((    time.first < mCurrentInitiative)
						|| (time.first == mCurrentInitiative && time.second > mCurrentIniTime)))
			{
				maxIni = time.first;
				minTIme = time.second;
				retVal = iter;
			}
		}

		return retVal;		
	}
	
	bool Combat::processEvent(CombatEvt* evt)
	{
		switch (evt->action->getType())
		{
		case CombatAction::ATTACK:
			return processAttack(evt->part, dynamic_cast<CombatActionAttack*>(evt->action));
		case CombatAction::MOVE:
			return processMove(evt->part, dynamic_cast<CombatActionMove*>(evt->action));
		case CombatAction::PAREE:
			return processParee(evt->part, dynamic_cast<CombatActionParee*>(evt->action), dynamic_cast<CombatActionAttack*>(getSecondLastEvent()->action));
		}
		return true;
	}

	void Combat::sendStopEvent(CombatEvt* evt)
	{
		if (evt->isAction)
			mEventCasters[evt->part->group].dispatchEvent(new AskForActionEvent(this, evt->id, evt->part->creature, ACTION_TIME_OVER, mSlowMotionFactor));
		else
			mEventCasters[evt->part->group].dispatchEvent(new AskForReactionEvent(this, evt->id, evt->part->creature, ACTION_TIME_OVER, mSlowMotionFactor, NULL)); //TODO: Opponent may be NULL in time over event?
	}

	bool Combat::processAttack(Participant* attacker, CombatActionAttack* attack)
	{
		Participant* defender = getParticipant(attack->getTarget());
		int result = attacker->creature->doAttacke(attack->getKampftechnik(), getAttackeModifier(attacker));

		if (true /*canDefend(defender, attacker, result)*/)
		{
			CombatEvt* evt = new CombatEvt(mCurrentInitiative, 0, false, defender, defender->nextReaction);
			mEventStack.push_back(evt);
			mCurrentEventStart = mTimeInKampfrunde;
			mEventCasters[evt->part->group].dispatchEvent(new AskForReactionEvent(this, evt->id, evt->part->creature, mTimeOfAction, mSlowMotionFactor, attacker->creature));
			return false;
		}
		else
		{
			//executeAttacke(attacker, defender);
			//popEventStack();
			return true;
		}
	}

	bool Combat::processParee(Participant* defender, CombatActionParee* paree, CombatActionAttack* attack)
	{
		if (paree->getTarget() == attack->getSource() 
			&& paree->getSource() == attack->getTarget())
		{
			defender->pareesLeft--;
			int parade = paree->getSource()->doParade(paree->getKampftechnik(), getParadeModifier(defender));
			if (parade != RESULT_ERFOLG)
				executeAttacke(getParticipant(attack->getSource()), defender);

			popEventStack(); // Parade
			popEventStack(); // Attacke
		}

		return true;
	}

	bool Combat::processMove(rl::Combat::Participant *part, rl::CombatActionMove *move)
	{
		//TODO: Implement
		return true;
	}

	void Combat::executeAttacke(Participant* attacker, Participant* defender)
	{
		//TODO Schaden auswürfeln
		int damage = 5;
		defender->creature->applyDamage(damage, NULL); //TODO: Waffe des Angreifers
	}

	bool Combat::canDefend(Participant* defender, Participant* attacker)
	{
		if (defender->pareesLeft == 0)
			return false;

		return true;
	}

	Combat::CombatEvt* Combat::getLastEvent()
	{
		return mEventStack[mEventStack.size() - 1];
	}

	Combat::CombatEvt* Combat::getSecondLastEvent()
	{
		return mEventStack[mEventStack.size() - 2];
	}

	void Combat::popEventStack()
	{
		CombatEvt* evt = getLastEvent();
		mEventStack.pop_back();
		delete evt;
	}

	int Combat::getAttackeModifier(rl::Combat::Participant *attacker)
	{
		return 0; //TODO
	}

	int Combat::getParadeModifier(rl::Combat::Participant *defender)
	{
		return 0; //TODO
	}

	void Combat::start()
	{
		mStarted = true;
	}
}
