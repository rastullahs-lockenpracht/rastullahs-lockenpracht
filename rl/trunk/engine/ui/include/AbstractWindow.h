/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __AbstractWindow_H__
#define __AbstractWindow_H__

#include "UiPrerequisites.h"


#include <CEGUIWindow.h>
#include <elements/CEGUIEditbox.h>
#include <elements/CEGUIListbox.h>
#include <elements/CEGUIMultiColumnList.h>
#include <elements/CEGUIMultiLineEditbox.h>
#include <elements/CEGUIProgressBar.h>
#include <elements/CEGUIMenuBase.h>
#include <elements/CEGUIMenuItem.h>
#include <elements/CEGUIPushButton.h>
#include <elements/CEGUICombobox.h>
#include <elements/CEGUIComboDropList.h>
#include <elements/CEGUIScrollablePane.h>
#include <elements/CEGUISlider.h>
#include <elements/CEGUITabControl.h>

namespace rl {

	class WindowUpdateTask;

	/**
	 * This is the base class for all UI windows
	 */
	class _RlUiExport AbstractWindow
	{
	public:

		enum WindowInputType
		{
			WIT_NONE = 0x00,
			WIT_MOUSE_INPUT = 0x01,
			WIT_KEYBOARD_INPUT = 0x02
		};

		virtual ~AbstractWindow();	

		/// creates a CEGUI window from an XML file, should only be used to load window parts (e.g. buttons)
		static CEGUI::Window* loadWindow(const CeGuiString& xmlfile);
		
		/// gets the CEGUI root
		static CEGUI::Window* getRoot();

		/// gets the wrapped CEGUI window
		CEGUI::Window* getWindow();

		CEGUI::Window* getWindow(const char* name, const char* requiredClass = NULL);
		CEGUI::Editbox* getEditbox(const char* name);
		CEGUI::Listbox* getListbox(const char* name);
		CEGUI::MultiColumnList* getMultiColumnList(const char* name);
		CEGUI::MultiLineEditbox* getMultiLineEditbox(const char* name);
		CEGUI::ProgressBar* getProgressBar(const char* name);
		CEGUI::MenuBase* getMenu(const char* name);
		CEGUI::MenuItem* getMenuItem(const char* name);
		CEGUI::PushButton* getPushButton(const char* name);
		CEGUI::Combobox* getCombobox(const char* name);
		CEGUI::ComboDropList* getComboDropList(const char* name);
        CEGUI::ScrollablePane* getScrollablePane(const char* name);
		CEGUI::Slider* getSlider(const char* name);
		CEGUI::TabControl* getTabControl(const char* name);

		int getWindowInputType();

        virtual bool isVisible();
        virtual void setVisible(bool visible, bool destroyAfterHide = false);

		bool isModal();
		bool isClosingOnEscape();

		const CeGuiString& getName() const;
		const Ogre::Real& getNormalAlpha() const;

		virtual void windowHid();

	protected:
		AbstractWindow(
			const CeGuiString& xmlfile, 
			int inputType, 
			bool closeOnEscape = true,
			bool modal = false);

		const CeGuiString& getNamePrefix() const;
		
		void centerWindow();
		bool destroyWindow();		
		bool hideWindow();

		void bindDestroyWindowToClick(CEGUI::Window* button);
		void bindDestroyWindowToXButton();
		void bindHideWindowToXButton();

		static CEGUI::Window* loadWindow(const CeGuiString& xmlfile, CeGuiString& prefix);

		const CeGuiString& getUserDataType(CEGUI::Window* window) const;
		void setUserDataType(CEGUI::Window* window, const CeGuiString& typeDescription) const;

		CEGUI::Window* mWindow;
		bool mVisible;

	private:
		
		int mWindowInputType;
		CeGuiString mNamePrefix;
		CeGuiString mName;
		bool mModal;
		bool mCloseOnEscape;
		Ogre::Real mNormalAlpha;

		static int sNumAbstractWindows;

	};

}

#endif
