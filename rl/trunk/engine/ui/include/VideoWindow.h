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
#include "VideoEvents.h"
#include "EventListener.h"

namespace CEGUI {
    class String;
    class Texture;
    class StaticImage;
}

namespace rl
{

class _RlUiExport VideoWindow : public CeGuiWindow,
     public Ogre::Singleton<VideoWindow>,
     public EventListener<VideoPlayEvent>,
	 public EventListener<VideoTimingEvent>
{
private:
    /// Textur des Videos
    CEGUI::Texture *mTexture;
    /// Texturname
    CeGuiString mName;
    /// Das Bild
    CEGUI::StaticImage *mImage;
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
    void show(CEGUI::Texture *texture, CeGuiString name);
    /// Reaktion auf Videoevents.
	virtual bool eventRaised(VideoPlayEvent *event);
	virtual bool eventRaised(VideoTimingEvent *event);
    
    
};

}

#endif /*VIDEOWINDOW_H_*/
