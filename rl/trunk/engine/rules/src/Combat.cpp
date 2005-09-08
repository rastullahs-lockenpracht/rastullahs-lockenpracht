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
#include "Creature.h"
#include "Exception.h"
#include "DsaManager.h"

using namespace std;

namespace rl {

	Combat::Combat()
	{
	}

	Combat::~Combat()
	{
	}

	void Combat::add(Creature* creature, int group)
	{
		Participant* part = new Participant(creature, group);
		initialize(part);
		mParticipants.insert(make_pair(creature, part));
	}

	void Combat::initialize(Participant* part)
	{
		//TODO: Aktuelle Waffe, INI würfeln
		part->initiative = 5+DsaManager::getSingleton().rollD6();
	}

	Combat::Participant::Participant(Creature* creature, int group)
	{
		this->creature = creature;
		this->group = group;
		this->initiative = NO_INI;
		this->attackeTarget = NULL;
		this->paradeTarget = NULL;
		this->nextMoveAction = Combat::AT_NO_WALK;
	}

	int Combat::getGroupOf(Creature* creature)
	{
		return getParticipant(creature)->group;
	}

	vector<Creature*> Combat::getGroupMembers(int group)
	{
		vector<Creature*> members;

		//TODO

		return members;
	}

	Combat::Participant* Combat::getParticipant(Creature* creature)
	{
		CombatMap::iterator iter = mParticipants.find(creature); 
		if (iter == mParticipants.end())
			Throw(InvalidArgumentException, "Wesen nimmt nicht am Kampf teil.");
		return (*iter).second;
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

	Ogre::Real Combat::getMaxMoveDistance(MoveType action)
	{
		//TODO: Geschwindigkeit
		switch (action)
		{
		case AT_NO_WALK:
			return 0;
		case AT_WALK_IN_AT_PHASE:
		case AT_WALK_IN_PA_PHASE:
			return 300;
		case AT_WALK_IN_AT_PA_PHASE:
			return 600;
		}

		Throw(InvalidArgumentException, "Unbekannter MoveType.");
	}

	void Combat::doAttacke(Creature* creature)
	{
		//TODO: Passende Animationen
		//TODO: Attacke würfeln
		//TODO: Parade würfeln 
		//TODO: Schaden würfeln und machen
	}
}