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

#ifndef __DEBUGWINDOW_H__
#define __DEBUGWINDOW_H__

#include "UiPrerequisites.h"
#include <OgreSingleton.h>

#include "CeGuiWindow.h"
#include "GameTask.h"

namespace rl {

    class _RlUiExport DebugWindow
		: public Ogre::Singleton<DebugWindow>, public GameTask, public CeGuiWindow
    {
    public:
        DebugWindow(void);
        ~DebugWindow();
        static DebugWindow& getSingleton(void);
        static DebugWindow* getSingletonPtr(void);

		void setVisible(bool visible);
        void setText(const Ogre::String& output);
		void run(Ogre::Real elapsedTime);

    private:
		void updateFps();
        CEGUI::StaticText* mText;		
    };
}

#endif
