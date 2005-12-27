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

#ifndef __CombatActions_H__
#define __CombatActions_H__

#include "RulesPrerequisites.h"
#include "Combat.h"

namespace rl {

	class _RlRulesExport Creature;

	class _RlRulesExport CombatAction {
	public:
		enum CombatActionType {
			NO_ACTION,
			MOVE,
			ATTACK,
			PAREE
		};

		CombatAction();
		CombatAction(Creature* source);
		virtual ~CombatAction() = 0;

		virtual CombatActionType getType() = 0; 

		Creature* getSource();
		void setSource(Creature* source);
	private:
		Creature* mSource;
	};

	class _RlRulesExport CombatActionMove : public CombatAction {
	public:
		CombatActionMove(Creature* source, MoveType moveType, Ogre::Vector3 moveTarget);
		virtual ~CombatActionMove();

		virtual CombatActionType getType() { return MOVE; }

		MoveType getMoveType();
		Ogre::Vector3 getMoveTarget();
	private:
		MoveType mType;
		Ogre::Vector3 mTarget;
	};

	class _RlRulesExport CombatActionAttack : public CombatAction {
	public:
		CombatActionAttack(Creature* source, Creature* target, const CeGuiString& kampftechnik = "");
		virtual ~CombatActionAttack();

		virtual CombatActionType getType() { return ATTACK; }

		Creature* getTarget();
		
		const CeGuiString& getKampftechnik();
		void setKampftechnik(const CeGuiString& kampftechnik);
	private:
		Creature* mTarget;
		CeGuiString mKampftechnik;
	};

	class _RlRulesExport CombatActionParee : public CombatActionAttack {
	public:
		CombatActionParee(Creature* source, Creature* target, const CeGuiString& kampftechnik = "");
		virtual ~CombatActionParee();
		virtual CombatActionType getType() { return PAREE; }
	};

	class _RlRulesExport CombatActionNop : public CombatAction {
	public:
		CombatActionNop();
		virtual ~CombatActionNop();

		virtual CombatActionType getType() { return NO_ACTION; }
	};

} // namespace rl

#endif
