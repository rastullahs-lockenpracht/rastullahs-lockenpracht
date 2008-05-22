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
#include <boost/bind.hpp>

#include "UiPrerequisites.h"

#include "InputManager.h"
#include "UiSubsystem.h"
#include "CommandMapper.h"
#include "CommandMapperWindow.h"

#include "GameObject.h"
#include "Creature.h"
#include "Action.h"

using namespace CEGUI;
using namespace Ogre;

namespace rl {

CommandMapperWindow::CommandMapperWindow(Creature* actionHolder, CommandMapper* commandMapper)
	:	CeGuiWindow("commandmapper.xml", WND_ALL_INPUT),
		mActionHolder(actionHolder),
		mInputWindow(new CommandMapperInputWindow()),
		mCommandMapper(commandMapper)
{
	getWindow("CommandMapper/ChangeButton")->
		subscribeEvent(
			PushButton::EventClicked,
			boost::bind(&CommandMapperWindow::handleChangeButton, this));
	bindClickToCloseWindow(getWindow("CommandMapper/CloseButton"));
	
	mTabPane = getTabPane("CommandMapper/TabPane");

	mTableMovement = getMultiColumnList("CommandMapper/TableMovement");
	mTableMovement->subscribeEvent(
			Window::EventMouseDoubleClick,
			boost::bind(&CommandMapperWindow::handleChangeMovement, this));
	mTableInCombat = getMultiColumnList("CommandMapper/TableInCombat");
	mTableInCombat->subscribeEvent(
			Window::EventMouseDoubleClick,
			boost::bind(&CommandMapperWindow::handleChangeInCombat, this));
	mTableOffCombat = getMultiColumnList("CommandMapper/TableOffCombat");
	mTableOffCombat->subscribeEvent(
			Window::EventMouseDoubleClick,
			boost::bind(&CommandMapperWindow::handleChangeOffCombat, this));

	mTableMovement->addColumn("Aktion", 0, 0.5);
	mTableMovement->addColumn("Taste", 1, 0.5);
	mTableInCombat->addColumn("Aktion", 0, 0.3);
	mTableInCombat->addColumn("Klasse", 1, 0.4);
	mTableInCombat->addColumn("Taste", 2, 0.3);
	mTableOffCombat->addColumn("Aktion", 0, 0.3);
	mTableOffCombat->addColumn("Klasse", 1, 0.4);
	mTableOffCombat->addColumn("Taste", 2, 0.3);

	getWindow("CommandMapper")->
		subscribeEvent(
			Window::EventKeyDown,
			boost::bind(&CommandMapperWindow::handleKeyDown, this, _1));
	getWindow("CommandMapper")->
		subscribeEvent(
			Window::EventMouseButtonDown,
			boost::bind(&CommandMapperWindow::handleMouseButton, this, _1));
	muteWindow(true);

	bindCloseToCloseButton();

	centerWindow();
	refreshContent();
}

void CommandMapperWindow::muteElements(bool mute)
{
	getWindow("CommandMapper/ChangeButton")->setMutedState(mute);
	getWindow("CommandMapper/CloseButton")->setMutedState(mute);
	getWindow("CommandMapper/TableMovement")->setMutedState(mute);
	getWindow("CommandMapper/TableInCombat")->setMutedState(mute);
	getWindow("CommandMapper/TableOffCombat")->setMutedState(mute);
}

void CommandMapperWindow::muteWindow(bool mute)
{
	getWindow("CommandMapper")->setMutedState(mute);
	mInputWindow->setVisible(!mute);
}

bool CommandMapperWindow::handleChangeButton()
{
	muteElements(true);
	muteWindow(false);

	mSelectedTable = mTabPane->getActiveChild();

	return true;
}

bool CommandMapperWindow::handleCloseButton()
{
	//TODO: CloseButton
	return true;
}

bool CommandMapperWindow::handleChangeInCombat()
{
	//TODO: ChangeInCombat
	return true;
}

bool CommandMapperWindow::handleChangeMovement()
{
	//TODO: ChangeMovement
	return true;
}

bool CommandMapperWindow::handleChangeOffCombat()
{
	//TODO: ChangeOffCombat
	return true;
}

bool CommandMapperWindow::handleKeyDown(const CEGUI::EventArgs& e)
{
	const KeyEventArgs ke = static_cast<const KeyEventArgs&>(e);
	Logger::getSingleton().log(Logger::UI, Logger::LL_MESSAGE, "Key Down "+StringConverter::toString(ke.scancode));

	muteWindow(true);
	muteElements(false);

	//TODO: Taste in CommandMapper eintragen
	if (mSelectedTable == mTableInCombat)
		mCommandMapper->setMapping(
			CMDMAP_KEYMAP_IN_COMBAT, 
			ke.scancode, 
			mTableInCombat->getItemAtGridReference(
				MCLGridRef(mTableInCombat->getNominatedSelectionRow(), 0))->getText()
		);
	else if (mSelectedTable == mTableOffCombat)
		mCommandMapper->setMapping(
			CMDMAP_KEYMAP_OFF_COMBAT, 
			ke.scancode, 
			mTableOffCombat->getItemAtGridReference(
				MCLGridRef(mTableOffCombat->getNominatedSelectionRow(), 0))->getText());
	else if (mSelectedTable == mTableMovement)
		mCommandMapper->setMapping(
			CMDMAP_KEYMAP_MOVEMENT, 
			ke.scancode, 
			mTableMovement->getItemAtGridReference(
				MCLGridRef(mTableMovement->getNominatedSelectionRow(), 0))->getText());
	
	return true;
}

bool CommandMapperWindow::handleMouseButton(const CEGUI::EventArgs& e)
{
	const MouseEventArgs me = static_cast<const MouseEventArgs&>(e);
	Logger::getSingleton().log(Logger::UI, Logger::LL_MESSAGE, "Mouse Button Down "+StringConverter::toString(me.button));
	//TODO: Mausknopf in CommandMapper eintragen

	muteWindow(true);
	muteElements(false);
	return true;
}

CommandMapperInputWindow::CommandMapperInputWindow() :
	CeGuiWindow("commandmapperinput.xml", CeGuiWindow::WND_SHOW)
{
	setVisible(false);
	mWindow->moveToFront();
	centerWindow();
}

void CommandMapperInputWindow::setVisible(bool visible, bool destroyAfterHide)
{
	CeGuiWindow::setVisible(visible, destroyAfterHide);
	if (visible)
		mWindow->moveToFront();
}

void CommandMapperWindow::refreshContent()
{
	unsigned int row;
	//// Alle möglichen Bewegungen aus dem CommandMapper auslesen
	//row = 0;
	//const std::map<CeGuiString, MovementState> movements = 
	//	mCommandMapper->getMovements();
	//for (std::map<CeGuiString, MovementState>::const_iterator move = movements.begin();
	//	move != movements.end(); move++)
	//{
	//	if (mTableMovement->getRowCount() <= row)
	//		mTableMovement->insertRow();

	//	mTableMovement->setItem(new ListboxTextItem(move->first), 0, row);
	//	mTableMovement->setItem(
	//		new ListboxTextItem(InputManager::getSingleton().getKeyName(move->second), 0, row);
	//	row++;
	//} 

	row = 0;
	const ActionVector actions = mActionHolder->getValidActions(mActionHolder);
	for (ActionVector::const_iterator actionIter = actions.begin(); 
		actionIter != actions.end(); actionIter++)
	{
		if (mTableInCombat->getRowCount() <= row)
			mTableInCombat->addRow();
		if (mTableOffCombat->getRowCount() <= row)
			mTableOffCombat->addRow();

		Action* action = *actionIter;
		mTableInCombat->setItem(new ListboxTextItem(action->getName()), 0, row);
		mTableOffCombat->setItem(new ListboxTextItem(action->getName()), 0, row);

		//int keyInCombat = mCommandMapper->getMapping(CMDMAP_KEYMAP_IN_COMBAT, action->getClassName(), action->getName());
		int keyOffCombat = mCommandMapper->getMapping(CMDMAP_KEYMAP_OFF_COMBAT, action->getName());
		//mTableInCombat->setItem(
		//	new ListboxTextItem(
		//		InputManager::getSingleton().getKeyName(keyInCombat)), 2, row);
		mTableOffCombat->setItem(
			new ListboxTextItem(
				InputManager::getSingleton().getKeyName(keyOffCombat)), 2, row);
		row++;
	}
}

}
