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

#include "SoundManager.h"
#include "SoundResource.h"

#include "PlaylistWindow.h"

using namespace CEGUI;
using namespace Ogre;

namespace rl
{
    PlaylistWindow::PlaylistWindow()
        : AbstractWindow("playlistwindow.xml", WIT_MOUSE_INPUT)
    {
        mPlaylistTable = getMultiColumnList("PlaylistSheet/PlaylistSheet/Table");
        mPlaylistTable->addColumn("Name", 0, cegui_reldim(0.5F));
        mPlaylistTable->addColumn("Dauer", 1, cegui_reldim(0.3F));
        mPlaylistTable->setUserSortControlEnabled(false);

        /* getWindow("PlaylistWindow")->subscribeEvent(
                Window::EventMouseClick,
                boost::bind(&MainMenuWindow::handleChooseModules, this)); */

        getWindow("PlaylistSheet/ButtonSheet/Play")
            ->subscribeEvent(Window::EventMouseClick, &PlaylistWindow::handlePlay, this);
        getWindow("PlaylistSheet/ButtonSheet/Quit")
            ->subscribeEvent(Window::EventMouseClick, &PlaylistWindow::handleQuit, this);
        bindDestroyWindowToClick(getWindow("PlaylistSheet"));
        bindDestroyWindowToXButton();

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
