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
#include "RTCombatController.h"

#include "Creature.h"

namespace rl {

	RTCombatController::RTCombatController()
	{
	}

	RTCombatController::~RTCombatController()
	{
	}

	int RTCombatController::getGroup()
	{
		return mGroup;
	}

	void RTCombatController::setGroup(int group)
	{
		mGroup = group;
	}

	Creature* RTCombatController::getCurrentCreature()
	{
		return mCurrentCreature;
	}
	
	void RTCombatController::setCurrentCreature(Creature* creature)
	{
		mCurrentCreature = creature;
	}
	
	RTCombat* RTCombatController::getCombat()
	{
		return mCombat;
	}
	
	void RTCombatController::setCombat(RTCombat* combat)
	{
		mCombat = combat;
		mCombat->addController(this);
	}

	void RTCombatController::attack(Creature* target)
	{
		Creature* atTarget = target;
		if (!isInAtRange(atTarget))
		{
			atTarget = findAtTarget();
		}

		if (atTarget != NULL)
		{
			mCombat->attack(this, mCurrentCreature, atTarget);
		}
	}

	void RTCombatController::block(Creature* target)
	{
		mCombat->paree(this, mCurrentCreature, target);
	}

	bool RTCombatController::isInAtRange(Creature* target)
	{
		Weapon* weapon = mCurrentCreature->getActiveWeapon();
		return true;
	}

	Creature* RTCombatController::findAtTarget()
	{
		return NULL;
	}
}
