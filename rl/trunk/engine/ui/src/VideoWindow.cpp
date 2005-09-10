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
#include "DebugWindow.h"

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
	mImage = getStaticImage( "VideoWindow/Video" );
	mWindow->addChildWindow(mImage);
	bindCloseToCloseButton();
	
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
    
 
   //Now attach Texture to
    if(mTexture)
    {
        CeGuiString temp = "MyImages";
        Imageset *img = ImagesetManager::getSingleton().createImageset( 
                temp, mTexture );
        img->defineImage( mName, Point(0.0f,0.0f), mImage->getAbsoluteSize(), Point(0.0f,0.0f));
        mImage->setImage(temp, mName);
    } 

	CeGuiWindow::show();
}

/// Reaktion auf Videoevents.
bool VideoWindow::eventRaised(VideoPlayEvent *event)
{
    if (event->getReason() == VideoPlayEvent::ENDOFSTREAM)
    {
		hide();
        destroyWindow();
    }
	return true;
}

bool VideoWindow::eventRaised(VideoTimingEvent* event)
{
	DebugWindow::getSingleton().setText(
		"Frame: "+Ogre::StringConverter::toString(event->mFrameNumber)+
		"   Dropped: "+Ogre::StringConverter::toString(event->mFramesDropped));
	return true;
}

}
