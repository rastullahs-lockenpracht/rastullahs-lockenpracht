/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __DEBUGWINDOW_H__
#define __DEBUGWINDOW_H__

#include "UiPrerequisites.h"
#include <OgreSingleton.h>
#include <OgreKeyEvent.h>

#include "CeGuiWindow.h"

namespace rl {

    class _RlUiExport DebugWindow
		: public Ogre::Singleton<DebugWindow>, public Ogre::KeyListener, public Ogre::FrameListener, public CeGuiWindow
    {
    public:
        DebugWindow(void);
        ~DebugWindow();
        static DebugWindow& getSingleton(void);
        static DebugWindow* getSingletonPtr(void);

        void setText(const Ogre::String& output);

        bool isVisible(void) const;
        
        void keyClicked(Ogre::KeyEvent* e) {};
        void keyPressed(Ogre::KeyEvent* e) {};
        void keyReleased(Ogre::KeyEvent* e);

		bool frameStarted(const Ogre::FrameEvent& evt);

    private:
		void updateFps();
        CEGUI::StaticText* mText;		
    };
}

#endif
