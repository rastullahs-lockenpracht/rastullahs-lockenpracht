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

#ifndef __CombatWindowStrategy_H__
#define __CombatWindowStrategy_H__

#include "UiPrerequisites.h"

namespace rl {

	class Combat;
	class Creature;
	class CombatWindow;
	class AskForActionEvent;
	class AskForReactionEvent;

	class CombatWindowStrategy {
	public:
		virtual void applyAction(CombatWindow* combatwindow, Combat* combat, AskForActionEvent* anEvent = NULL) = 0;
		virtual void applyReaction(CombatWindow* combatwindow, Combat* combat, AskForReactionEvent* anEvent = NULL) = 0;

		void setAttackTarget(Creature* target);
		Creature* getAttackTarget();
		void setPareeTarget(Creature* target);
		Creature* getPareeTarget();

	protected:
		CombatWindowStrategy(Creature* attackTarget, Creature* pareeTarget);
		virtual ~CombatWindowStrategy() {};

	private:

		Creature* mAttackTarget;
		Creature* mPareeTarget;
	};

	class WalkInActPhaseAttackStrategy : public CombatWindowStrategy {
	public:
		WalkInActPhaseAttackStrategy(Creature* attackTarget, Creature* pareeTarget = NULL);
		virtual ~WalkInActPhaseAttackStrategy();

		virtual void applyAction(CombatWindow* combatwindow, Combat* combat, AskForActionEvent* anEvent = NULL);
		virtual void applyReaction(CombatWindow* combatwindow, Combat* combat, AskForReactionEvent* anEvent = NULL);
	};

	class WalkInActAndReactPhaseAttackStrategy : public CombatWindowStrategy {
	public:
		WalkInActAndReactPhaseAttackStrategy(Creature* attackTarget, Creature* pareeTarget = NULL);
		virtual ~WalkInActAndReactPhaseAttackStrategy();

		virtual void applyAction(CombatWindow* combatwindow, Combat* combat, AskForActionEvent* anEvent = NULL);
		virtual void applyReaction(CombatWindow* combatwindow, Combat* combat, AskForReactionEvent* anEvent = NULL);
	};

	class RunInActAndReactPhaseAttackStrategy : public CombatWindowStrategy {
	public:
		RunInActAndReactPhaseAttackStrategy(Creature* attackTarget, Creature* pareeTarget = NULL);
		virtual ~RunInActAndReactPhaseAttackStrategy();

		virtual void applyAction(CombatWindow* combatwindow, Combat* combat, AskForActionEvent* anEvent = NULL);
		virtual void applyReaction(CombatWindow* combatwindow, Combat* combat, AskForReactionEvent* anEvent = NULL);
	};
}

#endif //__CombatWindow_H__