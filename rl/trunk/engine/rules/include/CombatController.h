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
#ifndef __CombatController_H__
#define __CombatController_H__


#include "RulesPrerequisites.h"

namespace rl {

	class Creature;
	class Combat;

	class _RlRulesExport CombatController 
	{
	public:
		CombatController(Combat* combat, int group);

		int getGroup();

		void setActionOption(int option);
		void setAttackTarget(Creature* creature);
		void setPareeTarget(Creature* creature);

		Creature* getCurrentCreature();

	protected:
		Combat* mCombat;
		void setCurrentCreature(Creature* creature);

	private:
		int mGroup;
		Creature* mCurrentCreature;
	};

}

#endif //__CombatController_H__
