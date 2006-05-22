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

#ifndef __DEBUGWINDOW_H__
#define __DEBUGWINDOW_H__

#include "UiPrerequisites.h"
#include <OgreSingleton.h>

#include "CeGuiWindow.h"
#include "GameTask.h"

#include <map>

namespace rl {

    class _RlUiExport DebugWindow
		: public Ogre::Singleton<DebugWindow>, public GameTask, public CeGuiWindow
    {
    public:
        DebugWindow(void);
        ~DebugWindow();
        static DebugWindow& getSingleton(void);
        static DebugWindow* getSingletonPtr(void);

		void setVisible(bool visible, bool destroyAfterHide = false);
        void registerPage(const Ogre::String& page);
        void unregisterPage(const Ogre::String& page);

        void setPageText(const Ogre::String& page, const Ogre::String& text);
		void run(Ogre::Real elapsedTime);

        void showNextPage();

    private:
        CEGUI::StaticText* mText;
        typedef std::map<Ogre::String, Ogre::String> PageTextMap;
        PageTextMap mPageTexts;
        Ogre::String mCurrentPage;        

		void updateFps();
        void updatePageText();
    };
}

#endif
