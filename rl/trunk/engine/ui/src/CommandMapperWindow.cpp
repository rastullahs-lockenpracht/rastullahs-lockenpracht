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

using namespace CEGUI;

namespace rl {

CommandMapperWindow::CommandMapperWindow()
	: CeGuiWindow("commandmapper.xml", WND_KEYBOARD_INPUT)
{
	subscribeEventsToElements();
}

void CommandMapperWindow::subscribeEventsToElements()
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

}

bool CommandMapperWindow::handleChangeButton()
{
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

}
