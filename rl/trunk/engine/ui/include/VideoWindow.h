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

#ifndef VIDEOWINDOW_H_
#define VIDEOWINDOW_H_

#include "UiPrerequisites.h"

#include <OgreSingleton.h>
#include "CeGuiWindow.h"

namespace CEGUI {
    class String;
}

namespace rl
{

class Video;

class _RlUiExport VideoWindow : public CeGuiWindow, public Ogre::Singleton<VideoWindow>
{
private:
    /// Video-Objekt
    Video *mVideo;
    /// Der Name des Videos
    CEGUI::String mVideoName;
public:
    /// Singleton-Object
    static VideoWindow& getSingleton();
    /// Singleton-Zeiger
    static VideoWindow* getSingletonPtr();

    ///Standardkonstruktor
	VideoWindow();
    /// Destruktor
	virtual ~VideoWindow();
    /// Spiel's nochmal, Sam
    void play(const CEGUI::String &videoName);
    
};

}

#endif /*VIDEOWINDOW_H_*/
