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

#include "VideoWindow.h"
#include "CEGUITexture.h"

template<> rl::VideoWindow* Ogre::Singleton<rl::VideoWindow>::ms_Singleton = 0;

using namespace CEGUI;

namespace rl
{
VideoWindow& VideoWindow::getSingleton()
{
    return Singleton<VideoWindow>::getSingleton();
}
VideoWindow* VideoWindow::getSingletonPtr()
{
    return Singleton<VideoWindow>::getSingletonPtr();
}

VideoWindow::VideoWindow()
    : CeGuiWindow("video.xml", WND_SHOW),
      mTexture(0)
{
}

VideoWindow::~VideoWindow()
{
    if (mTexture != 0)
    {
        delete mTexture;
    }
}

void VideoWindow::play(CEGUI::Texture *texture)
{
    mTexture = texture;
}


}
