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

#include "CombatWindowStrategy.h"
#include "CombatActions.h"
#include "Combat.h"
#include "Creature.h"
#include "CombatWindow.h"

namespace rl {

	CombatWindowStrategy::CombatWindowStrategy(Creature* attackTarget, Creature* pareeTarget)
		: mAttackTarget(attackTarget),
		mPareeTarget(pareeTarget)
	{
	}

	void CombatWindowStrategy::setAttackTarget(Creature* target)	
	{
		mAttackTarget = target;
	}

	Creature* CombatWindowStrategy::getAttackTarget()
	{
		return mAttackTarget;
	}

	void CombatWindowStrategy::setPareeTarget(Creature* target)
	{
		mPareeTarget = target;
	}

	Creature* CombatWindowStrategy::getPareeTarget()
	{
		return mPareeTarget;
	}

	CombatAction* createMoveAndAttackAction(Combat* combat, MoveType moveType, Creature* actor, Creature* target)
	{
		if (target != NULL) 
		{
			if (combat->isInAttackDistance(actor, target))
				return new CombatActionAttack(target);
			else
				return new CombatActionMove(moveType, target->getActor()->getPosition());
		}
		else
			return new CombatActionNop();
	}

	CombatAction* createPareeAction(Combat* combat, Creature* pareeTarget, Creature* opponent)
	{
		if (pareeTarget == NULL // Ersten erfolgreichen Angriff parieren, also jetzt
			|| pareeTarget == opponent // oder festgelegter Pariergegner greift erfolgreich an
			|| combat->isActionPhaseDone(pareeTarget)) // oder festgelegter Gegner ist schon dran gewesen
		{
			return new CombatActionParee(opponent);
		}
		else 
		{
			return new CombatActionParee(pareeTarget);
		}
	}


	WalkInActPhaseAttackStrategy::WalkInActPhaseAttackStrategy
		(Creature* attackTarget, Creature* pareeTarget)
		: CombatWindowStrategy(attackTarget, pareeTarget)
	{
	}

	WalkInActPhaseAttackStrategy::~WalkInActPhaseAttackStrategy()
	{
	}

	void WalkInActPhaseAttackStrategy::applyAction(CombatWindow* combatwindow, Combat* combat, AskForActionEvent* anEvent)
	{
		combatwindow->setNextAction(createMoveAndAttackAction(combat, MT_WALK, anEvent->getActor(), getAttackTarget()));
	}

    void WalkInActPhaseAttackStrategy::applyReaction(CombatWindow* combatwindow, Combat* combat, AskForReactionEvent* anEvent)
	{
		combatwindow->setNextReaction(createPareeAction(combat, getPareeTarget(), anEvent->getOpponent()));
	}
	

	WalkInActAndReactPhaseAttackStrategy::WalkInActAndReactPhaseAttackStrategy
		(Creature* attackTarget, Creature* pareeTarget)
		: CombatWindowStrategy(attackTarget, pareeTarget)
	{
	}

	WalkInActAndReactPhaseAttackStrategy::~WalkInActAndReactPhaseAttackStrategy()
	{
	}

	void WalkInActAndReactPhaseAttackStrategy::applyAction(CombatWindow* combatwindow, Combat* combat, AskForActionEvent* anEvent)
	{
	}

	void WalkInActAndReactPhaseAttackStrategy::applyReaction(CombatWindow* combatwindow, Combat* combat, AskForReactionEvent* anEvent)
	{
	}

	RunInActAndReactPhaseAttackStrategy::RunInActAndReactPhaseAttackStrategy
		(Creature* attackTarget, Creature* pareeTarget)
		: CombatWindowStrategy(attackTarget, pareeTarget)
	{
	}

	RunInActAndReactPhaseAttackStrategy::~RunInActAndReactPhaseAttackStrategy()
	{
	}


	void RunInActAndReactPhaseAttackStrategy::applyAction(CombatWindow* combatwindow, Combat* combat, AskForActionEvent* anEvent)
	{
	}

	void RunInActAndReactPhaseAttackStrategy::applyReaction(CombatWindow* combatwindow, Combat* combat, AskForReactionEvent* anEvent)
	{
	}

}