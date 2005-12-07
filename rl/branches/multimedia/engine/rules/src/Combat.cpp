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

#include "CombatEvents.h"
#include "Combat.h"
#include "Creature.h"
#include "Exception.h"
#include "DsaManager.h"

using namespace std;

namespace rl {

	Combat::Combat()
		: mEventCaster()
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
		part->initiative = 5+DsaManager::getSingleton().rollD6();
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
		return getGroupMembers(group)[0];
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

	void Combat::setNextAction(Creature* creature, CombatAction* action)
	{
		getParticipant(creature)->nextAction = action;
	}

	void Combat::setNextReaction(Creature* creature, CombatAction* reaction)
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

	void Combat::doAttacke(Creature* creature)
	{
		//TODO: Passende Animationen
		//TODO: Attacke wuerfeln
		//TODO: Parade wuerfeln 
		//TODO: Schaden wuerfeln und machen
	}

	void Combat::run(Ogre::Real elapsedTime)
	{
		//TODO: Zeitliche Steuerung eines Kampfes
	}

	void Combat::addCombatEventListener(CombatEventListener* listener)
	{
		mEventCaster.addEventListener(listener);
	}

	void Combat::removeCombatEventListener(CombatEventListener* listener)
	{
		mEventCaster.addEventListener(listener);
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
}
