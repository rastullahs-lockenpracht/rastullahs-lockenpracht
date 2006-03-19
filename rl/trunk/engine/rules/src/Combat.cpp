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
		CreatureData* part = new CreatureData(creature, group);
		initialize(part);
		mCreatureData.insert(make_pair(creature, part));
	}

	void Combat::initialize(CreatureData* part)
	{
		//TODO: Aktuelle Waffe, INI wuerfeln
		part->initiative = part->creature->doInitiativeWurf();
	}

	Combat::CreatureData::CreatureData(Creature* creature, int group)
	{
		this->creature = creature;
		this->group = group;
		this->initiative = NO_INI;
	}

	vector<Creature*> Combat::getGroupMembers(int group)
	{
		vector<Creature*> members;

		for (CreatureDataMap::iterator partIter = mCreatureData.begin();
			partIter != mCreatureData.end(); partIter++)
		{
			CreatureData* part = (*partIter).second;
			if (part->group == group)
				members.push_back(part->creature);
		}

		return members;
	}

	Combat::CreatureData* Combat::getData(Creature* creature)
	{
		vector<Creature*> members;

		for (CreatureDataMap::iterator partIter = mCreatureData.begin();
			partIter != mCreatureData.end(); partIter++)
		{
			CreatureData* part = (*partIter).second;
			if (part->creature == creature)
				return part;
		}

		Throw(InvalidArgumentException, (creature->getName()+" nimmt nicht am Kampf teil.").c_str());
	}

	int Combat::getGroupOf(Creature* creature)
	{
		return getData(creature)->group;
	}

	void Combat::addController(rl::CombatController *controller)
	{
		mControllers.push_back(controller);
	}

	void Combat::start()
	{
	}

	void Combat::tick()
	{
	}

	void Combat::setActionOption(CombatController* controller, Creature* actor, int option)
	{
	}

	void Combat::setAttackTarget(CombatController* controller, Creature* actor, Creature* target)
	{
	}

	void Combat::setPareeTarget(CombatController* controller, Creature* actor, Creature* target)
	{
	}

}
