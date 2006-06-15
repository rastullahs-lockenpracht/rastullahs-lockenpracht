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

	class WindowUpdateTask;

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
		virtual ~CeGuiWindow();	

		CEGUI::Window* getWindow();

		CEGUI::Window* getWindow(const char* name, const char* requiredClass = NULL);
		CEGUI::Editbox* getEditbox(const char* name);
		CEGUI::Listbox* getListbox(const char* name);
		CEGUI::StaticText* getStaticText(const char* name);
		CEGUI::StaticImage* getStaticImage(const char* name);
		CEGUI::MultiColumnList* getMultiColumnList(const char* name);
		CEGUI::MultiLineEditbox* getMultiLineEditbox(const char* name);
		CEGUI::TabPane* getTabPane(const char* name);
		CEGUI::TabControl* getTabControl(const char* name);
		CEGUI::ProgressBar* getProgressBar(const char* name);
		CEGUI::MenuBase* getMenu(const char* name);
		CEGUI::MenuItem* getMenuItem(const char* name);
		CEGUI::PushButton* getPushButton(const char* name);
		CEGUI::Combobox* getCombobox(const char* name);
		CEGUI::ComboDropList* getComboDropList(const char* name);
		CEGUI::Slider* getSlider(const char* name);

		static CEGUI::Window* getRoot();

		WindowType getWindowType();

        virtual bool isVisible();
        virtual void setVisible(bool visible, bool destroyAfterHide = false);

		bool isModal();
		bool isClosingOnEscape();
		bool isFading();
		void setFading(bool fading);

		const CeGuiString& getName() const;
		const Ogre::Real& getNormalAlpha() const;

		virtual void windowHid();		

		void _setUpdateTask(WindowUpdateTask* updateTask);
		WindowUpdateTask* _getUpdateTask();

	protected:
		CeGuiWindow(
			const CeGuiString& xmlfile, 
			WindowType type, 
			bool closeOnEscape = true,
			bool modal = false);

		const CeGuiString& getNamePrefix() const;
		
		void centerWindow();
		bool destroyWindow();

		void bindClickToCloseWindow(CEGUI::Window* button);
		void bindCloseToCloseButton();

		static CEGUI::Window* loadWindow(const CeGuiString& xmlfile);
		static CEGUI::Window* loadWindow(const CeGuiString& xmlfile, CeGuiString& prefix);

		CEGUI::Window* mWindow;
		bool mVisible; 

	private:
		
		WindowType mWindowType;
		CeGuiString mNamePrefix;
		CeGuiString mName;
		bool mModal;
		bool mCloseOnEscape;
		bool mFading;
		WindowUpdateTask* mUpdateTask;
		Ogre::Real mNormalAlpha;

		static int sNumCeGuiWindows;

	};

}

#endif
