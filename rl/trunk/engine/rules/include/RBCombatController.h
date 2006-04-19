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
#ifndef __RBCombatController_H__
#define __RBCombatController_H__


#include "RulesPrerequisites.h"

#include "RBCombat.h"

namespace rl {

	class Creature;	

	class _RlRulesExport RBCombatController 
	{
	public:
		RBCombatController(RBCombat* combat, int group);
		virtual ~RBCombatController();

		int getGroup();

		void setActionOption(RBCombat::ActionOption option);
		void setAttackTarget(Creature* creature);
		void setPareeTarget(Creature* creature);

		Creature* getCurrentCreature();

		virtual void notifyActionStart() = 0;
		virtual void notifyCombatEnd() = 0;

		void setCurrentCreature(Creature* creature);
		RBCombat* getCombat();

	private:
		int mGroup;
		Creature* mCurrentCreature;
		RBCombat* mCombat;
	};

}

#endif //__RBCombatController_H__
