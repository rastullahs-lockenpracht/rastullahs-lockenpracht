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

#ifndef __CeGuiWindow_H__
#define __CeGuiWindow_H__

#include "UiPrerequisites.h"

#include <OgreNoMemoryMacros.h>
#include <CEGUI.h>
#include <OgreMemoryMacros.h>

namespace rl {

	class _RlUiExport CeGuiWindow
	{
	public:

		enum WindowType
		{
			WND_SHOW,
			WND_MOUSE_INPUT,
			WND_KEYBOARD_INPUT,
			WND_ALL_INPUT
		};

		CEGUI::Window* getWindow(const char* name);
		CEGUI::Editbox* getEditbox(const char* name);
		CEGUI::Listbox* getListbox(const char* name);
		CEGUI::StaticText* getStaticText(const char* name);
		CEGUI::StaticImage* getStaticImage(const char* name);
		CEGUI::MultiColumnList* getMultiColumnList(const char* name);
		CEGUI::MultiLineEditbox* getMultiLineEditbox(const char* name);
		CEGUI::TabPane* getTabPane(const char* name);

		static void addToRoot(CEGUI::Window* window);
		static CEGUI::Window* getRoot();

		WindowType getWindowType();

        virtual bool isVisible();
        virtual void setVisible(bool visible);

		const CeGuiString& getName() const;

		virtual ~CeGuiWindow();	

	protected:
		CeGuiWindow(const char* xmlfile, WindowType type);
		
		void show();
		void hide();

		virtual bool beforeShow();
		virtual bool beforeHide();
	
		void centerWindow();
		
		CEGUI::Window* mWindow;
		bool mIsVisible;

	private:
		WindowType mWindowType;
		CeGuiString mNamePrefix;
		CeGuiString mName;

		static int sNumWindows;
		
	};

}

#endif
