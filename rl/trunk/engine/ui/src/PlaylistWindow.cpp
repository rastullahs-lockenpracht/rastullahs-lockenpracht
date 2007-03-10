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
#include <boost/bind.hpp>

#include "UiPrerequisites.h"

#include "SoundManager.h"
#include "SoundResource.h"

#include "PlaylistWindow.h"

using namespace CEGUI;
using namespace Ogre;


namespace rl {

PlaylistWindow::PlaylistWindow()
	: AbstractWindow("playlistwindow.xml", WND_MOUSE_INPUT)
{
	mPlaylistTable = getMultiColumnList("PlaylistSheet/PlaylistSheet/Table");
	mPlaylistTable->addColumn((utf8*)"Name", 0, cegui_reldim(0.5));
	mPlaylistTable->addColumn((utf8*)"Dauer", 1, cegui_reldim(0.3));
	mPlaylistTable->setUserSortControlEnabled(false);
	
    /* getWindow("PlaylistWindow")->subscribeEvent(
            Window::EventMouseClick, 
            boost::bind(&MainMenuWindow::handleChooseModules, this)); */

    getWindow("PlaylistSheet/ButtonSheet/Play")->subscribeEvent(
            Window::EventMouseClick, 
            boost::bind(&PlaylistWindow::handlePlay, this));
    getWindow("PlaylistSheet/ButtonSheet/Quit")->subscribeEvent(
            Window::EventMouseClick, 
            boost::bind(&PlaylistWindow::handleQuit, this));
    bindClickToCloseWindow(getWindow("PlaylistSheet"));
	bindCloseToCloseButton();

    centerWindow();
}

PlaylistWindow::~PlaylistWindow()
{
}

void PlaylistWindow::update()
{
}

bool PlaylistWindow::handleQuit()
{
    setVisible(false, true);
	return true;
}

bool PlaylistWindow::handlePlay()
{
	return true;
}

}
