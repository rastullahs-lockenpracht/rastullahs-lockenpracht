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

#include "CombatWindow.h"
#include <boost/bind.hpp>
#include <elements/CEGUIListboxTextItem.h>

#include "Combat.h"
#include "Creature.h"

using namespace CEGUI;

namespace rl {

	CombatWindow::CombatWindow(Combat* combat, int group)
		: CeGuiWindow("combatwindow.xml", WND_MOUSE_INPUT, false, false),
		CombatController(combat, group)
	{
		mCombat->addController(this);
		mActionOptions = getCombobox("CombatWindow/ActionOptions");
		mAttackTargets = getCombobox("CombatWindow/AttackTargets");
		mPareeTargets = getCombobox("CombatWindow/PareeTargets");
		getWindow("CombatWindow/Execute")->subscribeEvent(
			Window::EventMouseClick,
			boost::bind(&CombatWindow::handleExecute, this));

		update();
	}

	void CombatWindow::setVisible(bool visible)
	{
		CeGuiWindow::setVisible(visible);
		//TODO: Change CharacterController
	}

	void CombatWindow::update()
	{
		/// FIXME
		std::vector<Creature*> opponents = mCombat->getGroupMembers(2);
		for (std::vector<Creature*>::iterator it = opponents.begin(); 
			it != opponents.end(); it++)
		{
			Creature* cr = *it;
			ListboxItem* item = new ListboxTextItem(cr->getName(), 0, cr);
			mAttackTargets->addItem(item);
		}
	}

	bool CombatWindow::handleExecute()
	{
		setActionOption(mActionOptions->getSelectedItem()->getID());
		setAttackTarget(
			static_cast<Creature*>(mAttackTargets->getSelectedItem()->getUserData()));
		setPareeTarget(
			static_cast<Creature*>(mPareeTargets->getSelectedItem()->getUserData()));
		mCombat->tick();
		return true;
	}
}
