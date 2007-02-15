/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#include "AboutWindow.h"
#include "CoreSubsystem.h"
#include "ContentModule.h"

using namespace CEGUI;

namespace rl {

AboutWindow::AboutWindow() :
    CeGuiWindow("AboutWindow.xml", WND_MOUSE_INPUT)
{
    getWindow("AboutWindow/Text")->setText("Rastullahs Lockenpracht\n\nCopyright 2003-2006 Team Pantheon\
\n\nBenutzte Bibliotheken: Ogre, fmod, Newton, boost, ...");
    getWindow("AboutWindow/EngineText")->setText(
        CoreSubsystem::getSingleton().getEngineVersionString()
        + "("
        + CoreSubsystem::getSingleton().getEngineVersionName()
        + "), Build "
        + Ogre::StringConverter::toString(
                CoreSubsystem::getSingleton().getEngineBuildNumber()));

    getWindow("AboutWindow/ModuleText")->setText(
        CoreSubsystem::getSingleton().getActiveAdventureModule()->getName());

    bindCloseToCloseButton();
    bindClickToCloseWindow(getWindow("AboutWindow/CloseButton"));

    centerWindow();
}

}
