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
			WND_ALL_INPUT,
		};

		CEGUI::Window* getWindow();

		CEGUI::Window* getWindow(const char* name);
		CEGUI::Editbox* getEditbox(const char* name);
		CEGUI::Listbox* getListbox(const char* name);
		CEGUI::StaticText* getStaticText(const char* name);
		CEGUI::StaticImage* getStaticImage(const char* name);
		CEGUI::MultiColumnList* getMultiColumnList(const char* name);
		CEGUI::MultiLineEditbox* getMultiLineEditbox(const char* name);
		CEGUI::TabPane* getTabPane(const char* name);
		CEGUI::ProgressBar* getProgressBar(const char* name);
		CEGUI::MenuBase* getMenu(const char* name);
		CEGUI::MenuItem* getMenuItem(const char* name);

		static void addToRoot(CEGUI::Window* window);
		static CEGUI::Window* getRoot();

		WindowType getWindowType();

        virtual bool isVisible();
        virtual void setVisible(bool visible);

		const CeGuiString& getName() const;

		virtual ~CeGuiWindow();	

	protected:
		CeGuiWindow(const CeGuiString& xmlfile, WindowType type, bool modal = false);
		const CeGuiString& getNamePrefix() const;
		
		void show();
		void hide();

		virtual bool beforeShow();
		virtual bool beforeHide();
	
		void centerWindow();
		bool destroyWindow();

		void bindClickToCloseWindow(CEGUI::Window* button);
		void bindCloseToCloseButton();

		static CEGUI::Window* loadWindow(const CeGuiString& xmlfile);
		static CEGUI::Window* loadWindow(const CeGuiString& xmlfile, CeGuiString& prefix);

		CEGUI::Window* mWindow;
		bool mIsVisible; 

	private:
		
		WindowType mWindowType;
		CeGuiString mNamePrefix;
		CeGuiString mName;

		static int sNumCeGuiWindows;

	};

}

#endif
