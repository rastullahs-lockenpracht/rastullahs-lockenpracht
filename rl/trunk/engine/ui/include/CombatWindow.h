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

#ifndef __CombatWindow_H__
#define __CombatWindow_H__

#include "UiPrerequisites.h"

#include "CeGuiWindow.h"
#include "CombatEvents.h"

namespace rl {

	class Combat;
	class Creature;

	class _RlUiExport CombatWindow : public CeGuiWindow, public CombatEventListener
	{
	public:
		CombatWindow(Combat* combat, int group);

	protected:
		bool eventRaised(AskForReactionEvent* anEvent);
		bool eventRaised(AskForActionEvent* anEvent);
		bool eventRaised(CombatFinishEvent* anEvent);

	private:
		enum ContextMenuAction {
			CTX_PA,
			CTX_AT_RUN_ATPA,
			CTX_AT_WALK_ATPA,
			CTX_AT_WALK_AT
		};

		bool handleContextMenu(ContextMenuAction action);
		bool handleRightClick(const CEGUI::EventArgs& evt);
		bool handleNextKR();
		bool handleContextMenuClose();

		Creature* mCurrentCreature;
		Combat* mCombat;
		int mGroup;

		CEGUI::MenuBase* mOpponentContextMenu;
		CEGUI::MenuItem* mMenuItemAttackWalkAt;
		CEGUI::MenuItem* mMenuItemAttackWalkAtPa;
		CEGUI::MenuItem* mMenuItemAttackRunAtPa;
		CEGUI::MenuItem* mMenuItemParadeThis;
		CEGUI::MenuItem* mMenuItemInfo;
		CEGUI::PushButton* mButtonNextRound;

		Creature* mAttackTarget;
		Creature* mPareeTarget;

	};
}

#endif //__CombatWindow_H__