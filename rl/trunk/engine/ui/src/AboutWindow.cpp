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
#include "AboutWindow.h"
#include "ConfigurationManager.h"
#include "CoreSubsystem.h"

using namespace CEGUI;

namespace rl {

AboutWindow::AboutWindow() :
	CeGuiWindow("AboutWindow.xml", WND_MOUSE_INPUT)
{
	ConfigurationManager confMan = ConfigurationManager::getSingleton();

	getWindow("AboutWindow/Text")->setText(confMan.getAboutText());
	getWindow("AboutWindow/EngineText")->setText(
		confMan.getEngineVersionString()+
		" ("+Ogre::StringConverter::toString(
				confMan.getEngineBuildNumber())+")");

	getWindow("AboutWindow/ModuleText")->setText(
		CoreSubsystem::getSingleton().getActiveAdventureModule());

	bindCloseToCloseButton();
	bindClickToCloseWindow(getWindow("AboutWindow/CloseButton"));

	centerWindow();

	addToRoot(mWindow);
}

}
