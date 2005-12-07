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
#include "CombatWindowStrategy.h"
#include <boost/bind.hpp>
#include <CEGUIEventArgs.h>

#include "InputManager.h"


#include "Combat.h"
#include "GameObject.h"
#include "Creature.h"

using namespace CEGUI;

namespace rl {

	CombatWindow::CombatWindow(Combat* combat, int group)
		: mCombat(combat),
		  mPlannedStrategy(NULL),
		CeGuiWindow("combatwindow.xml", WND_MOUSE_INPUT),
		CombatEventListener(group)
	{
        mCurrentCreature = mCombat->getNext(group);

		mOpponentContextMenu = getMenu("CombatWindow/OpponentMenu");
		mOpponentContextMenu->setAutoResizeEnabled(true);

		mMenuItemAttackWalkAt = getMenuItem("CombatWindow/OpponentMenu/AttackWalkAt");
		mMenuItemAttackWalkAt->subscribeEvent(
			Window::EventMouseClick,
			boost::bind(&CombatWindow::handleContextMenu, this, CTX_AT_WALK_AT));
		
		mMenuItemAttackWalkAtPa = getMenuItem("CombatWindow/OpponentMenu/AttackWalkAtPa");
		mMenuItemAttackWalkAtPa->subscribeEvent(
			Window::EventMouseClick,
			boost::bind(&CombatWindow::handleContextMenu, this, CTX_AT_WALK_ATPA));

		mMenuItemAttackRunAtPa = getMenuItem("CombatWindow/OpponentMenu/AttackRunAtPa");
		mMenuItemAttackRunAtPa->subscribeEvent(
			Window::EventMouseClick,
			boost::bind(&CombatWindow::handleContextMenu, this, CTX_AT_RUN_ATPA));

		mMenuItemParadeThis = getMenuItem("CombatWindow/OpponentMenu/ParadeThis");
		mMenuItemParadeThis->subscribeEvent(
			Window::EventMouseClick,
			boost::bind(&CombatWindow::handleContextMenu, this, CTX_PA));

		mMenuItemInfo = getMenuItem("CombatWindow/OpponentMenu/Info");
		mMenuItemInfo->setEnabled(false);

		getWindow("CombatWindow/NextKR")->subscribeEvent(
			Window::EventMouseClick,
			boost::bind(&CombatWindow::handleNextKR, this));

		//Contextmenue mit Rechtsklick oeffnen
		getWindow("CombatWindow")->subscribeEvent(
			Window::EventMouseClick,
			boost::bind(&CombatWindow::handleRightClick, this, _1));

		addToRoot(mWindow);
		mCombat->addCombatEventListener(this);
	}

	bool CombatWindow::handleRightClick(const EventArgs& evt)
	{
		const MouseEventArgs mouseEvt = static_cast<const MouseEventArgs&>(evt);
		if (mouseEvt.button == RightButton)
		{
			GameObject* pickedObject = InputManager::getSingleton().getPickedObject();
			Creature* creature = NULL;
			try {
				creature = dynamic_cast<Creature*>(pickedObject);
			}
			catch (bad_cast)
			{
			}

			if (creature != NULL && mCombat->getGroupOf(creature) != getGroup())
			{
				mMenuItemAttackRunAtPa->setText("Hinrennen (XX Aktionen) und angreifen, keine Parade");
				mMenuItemAttackWalkAtPa->setText("Hinlaufen (XX Aktionen) und angreifen, keine Parade");
				mMenuItemAttackWalkAt->setText("Hinlaufen (XX Aktionen) und angreifen");
				mMenuItemParadeThis->setText("Dieses parieren");
				mMenuItemInfo->setText(pickedObject->getDescription());
				mOpponentContextMenu->setPosition(Absolute, mouseEvt.position);
				mOpponentContextMenu->show();
				mOpponentContextMenu->setUserData(creature);
				return true;
			}
		}
	
		return false; // Ereignis nicht loeschen
	}

	bool CombatWindow::handleContextMenu(ContextMenuAction action)
	{
		Creature* target = static_cast<Creature*>(mOpponentContextMenu->getUserData());

		switch (action) {
			case CTX_AT_RUN_ATPA:
				mPlannedStrategy = 
					new RunInActAndReactPhaseAttackStrategy(target);
				break;
			case CTX_AT_WALK_AT:
				mPlannedStrategy = 
					new WalkInActPhaseAttackStrategy(target);
				break;
			case CTX_AT_WALK_ATPA:
				mPlannedStrategy = 
					new WalkInActAndReactPhaseAttackStrategy(target);
				break;
			case CTX_PA:
				mPlannedStrategy->setPareeTarget(target);
				break;

			default:
				return false;
		}
		
		if (mPlannedStrategy != NULL)
		{
			mPlannedStrategy->applyAction(this, mCombat);
			mPlannedStrategy->applyReaction(this, mCombat);
		}
		return true;
	}

	bool CombatWindow::handleContextMenuClose()
	{
		mOpponentContextMenu->hide();
		return true;
	}

	bool CombatWindow::handleNextKR()
	{
		if (mPlannedStrategy != NULL)
		{
			mPlannedStrategy->applyAction(this, mCombat);
			mPlannedStrategy->applyReaction(this, mCombat);
		}
		return true;
	}

	bool CombatWindow::eventRaised(AskForReactionEvent* anEvent)
	{
		if (anEvent->getTimeLeftToAct() == 0.0) // Entscheidung wird abgefragt
		{
			mCombat->setNextReaction(mCurrentCreature, mNextReaction);
		}
		else
		{
			if (mPlannedStrategy != NULL)
				mPlannedStrategy->applyReaction(this, mCombat, anEvent);
		}
		return true;
	}

	bool CombatWindow::eventRaised(AskForActionEvent* anEvent)
	{
		if (anEvent->getTimeLeftToAct() == 0.0) // Entscheidung wird abgefragt
		{
			mCombat->setNextAction(mCurrentCreature, mNextAction);
		}
		else
		{
			if (mPlannedStrategy != NULL)
				mPlannedStrategy->applyAction(this, mCombat, anEvent);
		}
		return true;
	}

	bool CombatWindow::eventRaised(CombatFinishEvent* anEvent)
	{
		destroyWindow();
		return true;
	}

	void CombatWindow::setNextAction(CombatAction* action)
	{
		mNextAction = action;
	}

	void CombatWindow::setNextReaction(CombatAction* reaction)
	{
		mNextReaction = reaction;
	}

}