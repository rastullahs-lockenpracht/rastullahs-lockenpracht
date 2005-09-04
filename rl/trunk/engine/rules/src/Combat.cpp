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
		mParticipants.push_back(part);
	}

	void Combat::initialize(Participant* creature)
	{
	}

	Combat::Participant::Participant(Creature* creature, int group)
	{
		this->creature = creature;
		this->group = group;
		this->initiative = NO_INI;
	}
}