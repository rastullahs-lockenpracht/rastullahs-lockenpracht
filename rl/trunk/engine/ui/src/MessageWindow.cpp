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
#include "UiPrerequisites.h"

#include <boost/bind.hpp>

#include "WindowManager.h"
#include "MessageWindow.h"

namespace rl {

MessageWindow::MessageWindow() :
	CeGuiWindow("messagewindow.xml", WND_MOUSE_INPUT)
{
	mTextField = getWindow("MessageWindow/Text");
	getWindow("MessageWindow/CloseButton")->subscribeEvent(
			CEGUI::Window::EventMouseClick, 
			boost::bind(&WindowManager::destroyWindow, WindowManager::getSingletonPtr(), this));

	centerWindow();
	addToRoot(mWindow);
}

void MessageWindow::setText(const CeGuiString& message)
{
	mTextField->setText(message);
}

}
