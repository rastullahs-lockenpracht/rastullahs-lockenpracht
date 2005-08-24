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

CommandMapperWindow::CommandMapperWindow(Creature* actionHolder)
	:	CeGuiWindow("commandmapper.xml", WND_ALL_INPUT),
		mActionHolder(actionHolder),
		mInputWindow(new CommandMapperInputWindow())		
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
	mTableInBattle = getMultiColumnList("CommandMapper/TableInBattle");
	mTableInBattle->subscribeEvent(
			Window::EventMouseDoubleClick,
			boost::bind(&CommandMapperWindow::handleChangeInBattle, this));
	mTableOffBattle = getMultiColumnList("CommandMapper/TableOffBattle");
	mTableOffBattle->subscribeEvent(
			Window::EventMouseDoubleClick,
			boost::bind(&CommandMapperWindow::handleChangeOffBattle, this));

	mTableMovement->addColumn("Aktion", 0, 0.5);
	mTableMovement->addColumn("Taste", 1, 0.5);
	mTableInBattle->addColumn("Aktion", 0, 0.3);
	mTableInBattle->addColumn("Klasse", 1, 0.4);
	mTableInBattle->addColumn("Taste", 2, 0.3);
	mTableOffBattle->addColumn("Aktion", 0, 0.3);
	mTableOffBattle->addColumn("Klasse", 1, 0.4);
	mTableOffBattle->addColumn("Taste", 2, 0.3);

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

	addToRoot(mWindow);
}

void CommandMapperWindow::muteElements(bool mute)
{
	getWindow("CommandMapper/ChangeButton")->setMutedState(mute);
	getWindow("CommandMapper/CloseButton")->setMutedState(mute);
	getWindow("CommandMapper/TableMovement")->setMutedState(mute);
	getWindow("CommandMapper/TableInBattle")->setMutedState(mute);
	getWindow("CommandMapper/TableOffBattle")->setMutedState(mute);
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

bool CommandMapperWindow::handleChangeInBattle()
{
	//TODO: ChangeInBattle
	return true;
}

bool CommandMapperWindow::handleChangeMovement()
{
	//TODO: ChangeMovement
	return true;
}

bool CommandMapperWindow::handleChangeOffBattle()
{
	//TODO: ChangeOffBattle
	return true;
}

bool CommandMapperWindow::handleKeyDown(const CEGUI::EventArgs& e)
{
	const KeyEventArgs ke = static_cast<const KeyEventArgs&>(e);
	UiSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "Key Down "+StringConverter::toString(ke.scancode));

	muteWindow(true);
	muteElements(false);

	//TODO: Taste in CommandMapper eintragen
	if (mSelectedTable == mTableInBattle)
		CommandMapper::getSingleton().setMapping(
			CMDMAP_KEYMAP_IN_BATTLE, 
			ke.scancode, 
			mTableInBattle->getItemAtGridReference(
				MCLGridRef(mTableInBattle->getNominatedSelectionRow(), 0))->getText(),
			mTableInBattle->getItemAtGridReference(
				MCLGridRef(mTableInBattle->getNominatedSelectionRow(), 1))->getText()
		);
	else if (mSelectedTable == mTableOffBattle)
		CommandMapper::getSingleton().setMapping(
			CMDMAP_KEYMAP_OFF_BATTLE, 
			ke.scancode, 
			mTableOffBattle->getItemAtGridReference(
				MCLGridRef(mTableOffBattle->getNominatedSelectionRow(), 0))->getText(),
			mTableOffBattle->getItemAtGridReference(
				MCLGridRef(mTableOffBattle->getNominatedSelectionRow(), 1))->getText()
		);
	else if (mSelectedTable == mTableMovement)
		CommandMapper::getSingleton().setMapping(
			CMDMAP_KEYMAP_MOVEMENT, 
			ke.scancode, 
			mTableMovement->getItemAtGridReference(
				MCLGridRef(mTableMovement->getNominatedSelectionRow(), 0))->getText(),
			mTableMovement->getItemAtGridReference(
				MCLGridRef(mTableMovement->getNominatedSelectionRow(), 1))->getText()
		);
	
	return true;
}

bool CommandMapperWindow::handleMouseButton(const CEGUI::EventArgs& e)
{
	const MouseEventArgs me = static_cast<const MouseEventArgs&>(e);
	UiSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "Mouse Button Down "+StringConverter::toString(me.button));
	//TODO: Mausknopf in CommandMapper eintragen

	muteWindow(true);
	muteElements(false);
	return true;
}

CommandMapperInputWindow::CommandMapperInputWindow() :
	CeGuiWindow("commandmapperinput.xml", CeGuiWindow::WND_SHOW)
{
	addToRoot(mWindow);
	setVisible(false);
	mWindow->moveToFront();
	centerWindow();
}

void CommandMapperInputWindow::setVisible(bool visible)
{
	CeGuiWindow::setVisible(visible);
	if (visible)
		mWindow->moveToFront();
}

void CommandMapperWindow::refreshContent()
{
	unsigned int row;
	//// Alle möglichen Bewegungen aus dem CommandMapper auslesen
	//row = 0;
	//const std::map<CeGuiString, MovementState> movements = 
	//	CommandMapper::getSingleton().getMovements();
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
		if (mTableInBattle->getRowCount() <= row)
			mTableInBattle->addRow();
		if (mTableOffBattle->getRowCount() <= row)
			mTableOffBattle->addRow();

		Action* action = *actionIter;
		mTableInBattle->setItem(new ListboxTextItem(action->getName()), 0, row);
		mTableInBattle->setItem(new ListboxTextItem(action->getClassName()), 1, row);
		mTableOffBattle->setItem(new ListboxTextItem(action->getName()), 0, row);
		mTableOffBattle->setItem(new ListboxTextItem(action->getClassName()), 1, row);

		//int keyInBattle = CommandMapper::getSingleton().getMapping(CMDMAP_KEYMAP_IN_BATTLE, action->getClassName(), action->getName());
		int keyOffBattle = CommandMapper::getSingleton().getMapping(CMDMAP_KEYMAP_OFF_BATTLE, action->getClassName(), action->getName());
		//mTableInBattle->setItem(
		//	new ListboxTextItem(
		//		InputManager::getSingleton().getKeyName(keyInBattle)), 2, row);
		mTableOffBattle->setItem(
			new ListboxTextItem(
				InputManager::getSingleton().getKeyName(keyOffBattle)), 2, row);
		row++;
	}
}

}
