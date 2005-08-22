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

#ifndef __TARGETSELECTIONWINDOW_H__
#define __TARGETSELECTIONWINDOW_H__

#include "UiPrerequisites.h"
#include <OgreSingleton.h>

#include "CeGuiWindow.h"

namespace rl {

	class Action;
	class GameObject;

    class _RlUiExport TargetSelectionWindow
		: public Ogre::Singleton<TargetSelectionWindow>, public CeGuiWindow
    {
    public:
        TargetSelectionWindow();
        ~TargetSelectionWindow();
        static TargetSelectionWindow& getSingleton();
        static TargetSelectionWindow* getSingletonPtr();

		void setVisible(bool visible);
		void setAction(Action* action);
		bool showObjectDescription(const CEGUI::EventArgs& e);
		bool showObjectActionsWindow(const CEGUI::EventArgs& e);

    private:
		GameObject* getTargetedObject(float x, float y);
		void setText(const CeGuiString& text);

        CEGUI::StaticText* mText;		
		Action* mAction;
		CEGUI::Window* mTooltip;
    };
}

#endif //__TARGETSELECTIONWINDOW_H__
