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
#include <CEGUITexture.h>

template<> rl::VideoWindow* Ogre::Singleton<rl::VideoWindow>::ms_Singleton = 0;

using namespace CEGUI;

namespace rl
{
VideoWindow& VideoWindow::getSingleton()
{
    return Ogre::Singleton<VideoWindow>::getSingleton();
}
VideoWindow* VideoWindow::getSingletonPtr()
{
    return Ogre::Singleton<VideoWindow>::getSingletonPtr();
}

VideoWindow::VideoWindow()
    : CeGuiWindow("video.xml", WND_SHOW),
      mTexture(0)
{
    addToRoot(mWindow);
    setVisible(false);
}

VideoWindow::~VideoWindow()
{
    if (mTexture != 0)
    {
        delete mTexture;
    }
}

void VideoWindow::show(Texture *texture, CeGuiString name)
{
    mTexture = texture;
    mName = name;
    
    mImage = (StaticImage*)WindowManager::getSingleton().createWindow("RastullahLook/StaticImage", "Video/image");
    mImage->setMetricsMode(Relative);
    mImage->setPosition(Relative, Vector2(0.02, 0.02));
    mImage->setHeight(0.95);
    mImage->setWidth(0.95);
    mWindow->addChildWindow(mImage);

    CeGuiWindow::show();

   //Now attach Texture to
    if(mTexture)
    {
        CeGuiString temp = "MyImages";
        Imageset *img = ImagesetManager::getSingleton().createImageset( 
                temp, mTexture );
        img->defineImage( mName, Point(0.0f,0.0f), mImage->getSize(), Point(0.0f,0.0f));
        mImage->setImage(temp, mName);
    } 
}

/// Reaktion auf Videoevents.
bool VideoWindow::eventRaised(VideoPlayEvent *event)
{
    if (event->getReason() == VideoPlayEvent::ENDOFSTREAM)
    {
        hide();
    }
}

}
