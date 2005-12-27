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

#include "CombatActions.h"

namespace rl {

	CombatAction::CombatAction()
		: mSource(NULL)
	{
	}

	CombatAction::CombatAction(rl::Creature* source)
		: mSource(source)
	{
	}

    CombatAction::~CombatAction()
    {
    }


	Creature* CombatAction::getSource()
	{
		return mSource;
	}

	void CombatAction::setSource(rl::Creature* source)
	{
		mSource = source;
	}

	CombatActionMove::CombatActionMove(Creature* source, MoveType moveType, Ogre::Vector3 moveTarget) 
		: CombatAction(source),
		mType(moveType),
		mTarget(moveTarget)
	{
	}

	CombatActionMove::~CombatActionMove()
	{
	}

	Ogre::Vector3 CombatActionMove::getMoveTarget()
	{
		return mTarget;
	}

	MoveType CombatActionMove::getMoveType() 
	{
		return mType;
	}

	CombatActionAttack::CombatActionAttack(Creature* source, Creature* target, const CeGuiString& kampftechnik)
		: CombatAction(source),
		mTarget(target),
		mKampftechnik(kampftechnik)
	{
	}

	Creature* CombatActionAttack::getTarget()
	{
		return mTarget;
	}

	const CeGuiString& CombatActionAttack::getKampftechnik()
	{
		return mKampftechnik;
	}

	CombatActionAttack::~CombatActionAttack()
	{
	}

	void CombatActionAttack::setKampftechnik(const CeGuiString& kampftechnik)
	{
		mKampftechnik = kampftechnik;
	}

	CombatActionParee::CombatActionParee(Creature* source, Creature* target, const CeGuiString& kampftechnik)
		: CombatActionAttack(source, target, kampftechnik)
	{
	}

	CombatActionParee::~CombatActionParee()
	{
	}


	CombatActionNop::CombatActionNop()
		: CombatAction(NULL)
	{
	}

	CombatActionNop::~CombatActionNop()
	{
	}
}