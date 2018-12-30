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
#include "stdinc.h" //precompiled header

#include "UiPrerequisites.h"

#include "CoreSubsystem.h"
#include "MainMenuEngineWindow.h"

using namespace CEGUI;
using namespace Ogre;

namespace rl
{
    MainMenuEngineWindow::MainMenuEngineWindow()
        : AbstractWindow("mainmenuenginewindow.xml", WIT_NONE, false)
    {
        getWindow("MainMenu/EngineVersion")
            ->setText(CoreSubsystem::getSingleton().getEngineVersionString() + " ("
                + StringConverter::toString(CoreSubsystem::getSingleton().getEngineBuildNumber()) + ")");
        mWindow->setAlwaysOnTop(false);
        mWindow->setModalState(false);
        mWindow->moveToBack();
    }
}
