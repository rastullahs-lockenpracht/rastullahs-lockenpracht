/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */
#include "UiPrerequisites.h"

#include <boost/bind.hpp>

#include "UiSubsystem.h"
#include "GameObject.h"
#include "WindowManager.h"

#include "CommandMapperWindow.h"

using namespace CEGUI;
using namespace Ogre;

namespace rl {

CommandMapperWindow::CommandMapperWindow(GameObject* actionHolder)
	:	CeGuiWindow("commandmapper.xml", WND_ALL_INPUT),
		mActionHolder(actionHolder),
		mInputWindow(new CommandMapperInputWindow())		
{
	getWindow("CommandMapper/ChangeButton")->
		subscribeEvent(
			PushButton::EventClicked,
			boost::bind(&CommandMapperWindow::handleChangeButton, this));
	getWindow("CommandMapper/CloseButton")->
		subscribeEvent(
			PushButton::EventClicked,
			boost::bind(&WindowManager::destroyWindow, WindowManager::getSingletonPtr(), this));
	
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

	getWindow("CommandMapper")->
		subscribeEvent(
			Window::EventKeyDown,
			boost::bind(&CommandMapperWindow::handleKeyDown, this, _1));
	getWindow("CommandMapper")->
		subscribeEvent(
			Window::EventMouseButtonDown,
			boost::bind(&CommandMapperWindow::handleMouseButton, this, _1));
	muteWindow(true);

	centerWindow();
	//TODO: Inhalte anzeigen

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
	UiSubsystem::getSingleton().log("Key Down "+StringConverter::toString(ke.scancode));

	muteWindow(true);
	muteElements(false);

	//TODO: Taste in CommandMapper eintragen

	return true;
}

bool CommandMapperWindow::handleMouseButton(const CEGUI::EventArgs& e)
{
	const MouseEventArgs me = static_cast<const MouseEventArgs&>(e);
	UiSubsystem::getSingleton().log("Mouse Button Down "+StringConverter::toString(me.button));
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

}
