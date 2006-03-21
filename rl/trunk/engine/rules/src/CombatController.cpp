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

#include "CombatController.h"

namespace rl {

	CombatController::CombatController(Combat* combat, int group)
		: mGroup(group), mCombat(combat), mCurrentCreature(NULL)
	{
	}

	CombatController::~CombatController()
	{
	}

	int CombatController::getGroup()
	{
		return mGroup;
	}

	void CombatController::setActionOption(Combat::ActionOption option)
	{
		mCombat->setActionOption(this, getCurrentCreature(), option);
	}

	void CombatController::setAttackTarget(Creature* creature)
	{
		mCombat->setAttackTarget(this, getCurrentCreature(), creature);
	}

	void CombatController::setPareeTarget(Creature* creature)
	{
		mCombat->setPareeTarget(this, getCurrentCreature(), creature);
	}

	Creature* CombatController::getCurrentCreature()
	{
		return mCurrentCreature;
	}

	void CombatController::setCurrentCreature(Creature* creature)
	{
		mCurrentCreature = creature;
	}

}
