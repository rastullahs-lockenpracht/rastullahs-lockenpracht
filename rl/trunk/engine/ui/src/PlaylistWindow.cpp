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
#include <boost/bind.hpp>

#include "UiPrerequisites.h"

#include "MusicManager.h"
#include "SoundManager.h"
#include "SoundResource.h"

#include "PlaylistWindow.h"

using namespace CEGUI;
using namespace Ogre;


namespace rl {

PlaylistWindow::PlaylistWindow()
	: CeGuiWindow("playlistwindow.xml", WND_MOUSE_INPUT)
{
	mPlaylistTable = getMultiColumnList("PlaylistSheet/PlaylistSheet/Table");
	mPlaylistTable->addColumn((utf8*)"Name", 0, 0.5);
	mPlaylistTable->addColumn((utf8*)"Dauer", 1, 0.3);
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

    centerWindow();
	addToRoot(mWindow);	
}

PlaylistWindow::~PlaylistWindow()
{
}

void PlaylistWindow::update()
{
}

bool PlaylistWindow::handleQuit()
{
    hide();
    delete this;
    return true;
}

bool PlaylistWindow::handlePlay()
{
    MusicManager::getSingleton().playSong();
    return true;
}

}
