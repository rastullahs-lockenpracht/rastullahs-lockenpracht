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

#include "CEGUI.h"

namespace rl {

	class _RlUiExport CeGuiWindow
	{
	public:
		CEGUI::Window* getWindow(const char* name);
		CEGUI::Editbox* getEditbox(const char* name);
		CEGUI::Listbox* getListbox(const char* name);
		CEGUI::StaticText* getStaticText(const char* name);
		CEGUI::StaticImage* getStaticImage(const char* name);
		CEGUI::MultiColumnList* getMultiColumnList(const char* name);
		CEGUI::MultiLineEditbox* getMultiLineEditbox(const char* name);

		static void addToRoot(CEGUI::Window* window);

		bool isInputWindow();

        virtual bool isVisible();
        virtual void setVisible(bool visible);

		const CeGuiString& getName() const;

		virtual ~CeGuiWindow();

	protected:
		CeGuiWindow(const char* xmlfile, bool input = false);
		
		void show();
		void hide();

		virtual bool beforeShow();
		virtual bool beforeHide();
	
		void centerWindow();
		
		CEGUI::Window* mWindow;
		bool mIsVisible;

	private:
		bool mWithInput;
		CeGuiString mNamePrefix;
		CeGuiString mName;

		static int sNumWindows;
		
	};

}

#endif
