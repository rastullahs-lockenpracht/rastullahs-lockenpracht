/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#include <boost/bind.hpp>

#include "CommandMapperWindow.h"
#include "UiSubsystem.h"
#include "FixRubyHeaders.h"

using namespace CEGUI;

namespace rl {

CommandMapperWindow::CommandMapperWindow()
	: CeGuiWindow("commandmapper.xml", WND_KEYBOARD_INPUT)
{
	getWindow("CommandMapper/ChangeButton")->
		subscribeEvent(
			PushButton::EventClicked,
			boost::bind(&CommandMapperWindow::handleChangeButton, this));
	getWindow("CommandMapper/CloseButton")->
		subscribeEvent(
			PushButton::EventClicked,
			boost::bind(&CommandMapperWindow::handleCloseButton, this));
	getWindow("CommandMapper/TableMovement")->
		subscribeEvent(
			Window::EventMouseDoubleClick,
			boost::bind(&CommandMapperWindow::handleChangeMovement, this));
	getWindow("CommandMapper/TableInBattle")->
		subscribeEvent(
			Window::EventMouseDoubleClick,
			boost::bind(&CommandMapperWindow::handleChangeInBattle, this));
	getWindow("CommandMapper/TableOffBattle")->
		subscribeEvent(
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
}

bool CommandMapperWindow::handleChangeButton()
{
	muteElements(true);
	muteWindow(false);
	return true;
}

bool CommandMapperWindow::handleCloseButton()
{
	return true;
}

bool CommandMapperWindow::handleChangeInBattle()
{
	return true;
}

bool CommandMapperWindow::handleChangeMovement()
{
	return true;
}

bool CommandMapperWindow::handleChangeOffBattle()
{
	return true;
}

bool CommandMapperWindow::handleKeyDown(const CEGUI::EventArgs& e)
{
	const KeyEventArgs ke = static_cast<const KeyEventArgs&>(e);
	UiSubsystem::getSingleton().log("Key Down "+StringConverter::toString(ke.scancode));

	muteWindow(true);
	muteElements(false);

	return true;
}

bool CommandMapperWindow::handleMouseButton(const CEGUI::EventArgs& e)
{
	return true;
}

}
