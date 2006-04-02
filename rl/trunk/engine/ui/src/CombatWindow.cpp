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
#include <CEGUIPropertyHelper.h>
#include <elements/CEGUIListboxTextItem.h>

#include "Combat.h"
#include "Creature.h"

using namespace CEGUI;

namespace rl {

	CombatWindow::CombatWindow(Combat* combat, int group)
		: CeGuiWindow("combatwindow.xml", WND_MOUSE_INPUT, false, false),
		CombatController(combat, group)
	{
		combat->addController(this);
		mActionOptions = getListbox("CombatWindow/ActionOptions");
		mAttackTargets = getListbox("CombatWindow/AttackTargets");
		mPareeTargets = getListbox("CombatWindow/PareeTargets");
		mOwnGroup = combat->getGroupMembers(getGroup());

		getWindow("CombatWindow/Confirm")->subscribeEvent(
			Window::EventMouseClick,
			boost::bind(&CombatWindow::handleExecute, this));
		
		initialize();
	}

	void CombatWindow::setVisible(bool visible, bool destroyAfterHide)
	{
		CeGuiWindow::setVisible(visible, destroyAfterHide);
		//TODO: Change CharacterController
	}

	void CombatWindow::initialize()
	{
		ColourRect acColors(PropertyHelper::stringToColour(mActionOptions->getProperty("DefaultSelectionColour")));
		
		ListboxTextItem* item = new ListboxTextItem("Abwarten", Combat::ACTION_NONE);
		item->setSelectionColours(acColors);
		mActionOptions->addItem(item);

		item = new ListboxTextItem("Attackieren", Combat::ACTION_ATTACK);
		item->setSelectionColours(acColors);
		mActionOptions->addItem(item);

		item = new ListboxTextItem("Bewegen", Combat::ACTION_MOVE);
		item->setSelectionColours(acColors);
		mActionOptions->addItem(item);

		setCurrentCreature(*mOwnGroup.begin());

		update();
	}

	void CombatWindow::update()
	{
		colour atColor = PropertyHelper::stringToColour(mAttackTargets->getProperty("DefaultSelectionColour"));
		colour paColor = PropertyHelper::stringToColour(mPareeTargets->getProperty("DefaultSelectionColour"));

		/// FIXME
		std::vector<Creature*> opponents = getCombat()->getGroupMembers(2);
		int idx = 1;
		for (std::vector<Creature*>::iterator it = opponents.begin(); 
			it != opponents.end(); it++)
		{
			Creature* cr = *it;

			ListboxItem* itemAt = new ListboxTextItem("Bla", idx++, cr);
			itemAt->setSelectionColours(atColor);
			mAttackTargets->addItem(itemAt);

			ListboxItem* itemPa = new ListboxTextItem("Bla", idx++, cr);
			itemPa->setSelectionColours(paColor);
			mPareeTargets->addItem(itemPa);
		}
	}

	bool CombatWindow::handleExecute()
	{
		setActionOption(static_cast<Combat::ActionOption>(mActionOptions->getFirstSelectedItem()->getID()));
		if (mAttackTargets->getFirstSelectedItem() != NULL)
		{
			setAttackTarget(
				static_cast<Creature*>(mAttackTargets->getFirstSelectedItem()->getUserData()));
		}
		else
		{
			setAttackTarget(NULL);
		}
		
		if (mPareeTargets->getFirstSelectedItem() != NULL)
		{
			setPareeTarget(
				static_cast<Creature*>(mPareeTargets->getFirstSelectedItem()->getUserData()));
		}
		else
		{
			setPareeTarget(NULL);
		}
		getCombat()->tick();
		return true;
	}

	void CombatWindow::notifyActionStart()
	{
	}
}
