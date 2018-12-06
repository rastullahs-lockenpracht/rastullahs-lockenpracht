/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h"

#include "GameOverWindow.h"

#include <boost/bind.hpp>

#include "CoreSubsystem.h"
#include "UiSubsystem.h"
#include "WindowFactory.h"

namespace rl
{

    GameOverWindow::GameOverWindow()
        : AbstractWindow(CeGuiString("gameoverwindow.xml"), AbstractWindow::WIT_MOUSE_INPUT, false, true)
    {
        getWindow("GameOverWindow/MainMenuButton")
            ->subscribeEvent(CEGUI::Window::EventMouseClick, boost::bind(&GameOverWindow::handleMainMenuClick, this));
        getWindow("GameOverWindow/QuitButton")
            ->subscribeEvent(CEGUI::Window::EventMouseClick, boost::bind(&GameOverWindow::handleQuitClick, this));
        getWindow("GameOverWindow/CheatButton")
            ->subscribeEvent(CEGUI::Window::EventMouseClick, boost::bind(&GameOverWindow::handleCheatClick, this));
    }

    bool GameOverWindow::handleMainMenuClick()
    {
        CoreSubsystem::getSingleton().startAdventureModule(CoreSubsystem::getSingleton().getModule("intro"));
        return true;
    }

    bool GameOverWindow::handleQuitClick()
    {
        WindowFactory::getSingleton().showExitConfirmation();
        return true;
    }

    bool GameOverWindow::handleCheatClick()
    {
        return true;
    }
}
