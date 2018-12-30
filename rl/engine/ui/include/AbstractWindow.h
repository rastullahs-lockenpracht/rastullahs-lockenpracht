/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

#include <CEGUI/Window.h>
#include <CEGUI/widgets/ComboDropList.h>
#include <CEGUI/widgets/Combobox.h>
#include <CEGUI/widgets/Editbox.h>
#include <CEGUI/widgets/Listbox.h>
#include <CEGUI/widgets/MenuBase.h>
#include <CEGUI/widgets/MenuItem.h>
#include <CEGUI/widgets/MultiColumnList.h>
#include <CEGUI/widgets/MultiLineEditbox.h>
#include <CEGUI/widgets/ProgressBar.h>
#include <CEGUI/widgets/PushButton.h>
#include <CEGUI/widgets/RadioButton.h>
#include <CEGUI/widgets/ScrollablePane.h>
#include <CEGUI/widgets/Slider.h>
#include <CEGUI/widgets/TabControl.h>
#include <CEGUI/widgets/ToggleButton.h>

namespace rl
{
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

        /// creates a CEGUI window from an XML file, should only be used to load window parts (e.g. buttons)
        static CEGUI::Window* loadWindow(const CeGuiString& xmlfile, CeGuiString& prefix);

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
        CEGUI::RadioButton* getRadioButton(const char* name);
        CEGUI::Combobox* getCombobox(const char* name);
        CEGUI::ComboDropList* getComboDropList(const char* name);
        CEGUI::ScrollablePane* getScrollablePane(const char* name);
        CEGUI::Slider* getSlider(const char* name);
        CEGUI::TabControl* getTabControl(const char* name);
        CEGUI::ToggleButton* getCheckbox(const char* name);

        int getWindowInputType();

        virtual bool isVisible();
        virtual void setVisible(bool visible, bool destroyAfterHide = false);

        bool isModal();
        bool isClosingOnEscape();

        const CeGuiString& getName() const;
        const Ogre::Real& getNormalAlpha() const;

        virtual void windowHid();

        // return true, if the specified key should be repeated if pressed down
        virtual bool wantsKeyToRepeat(const int& key)
        {
            return false;
        }

        bool hideWindow();

    protected:
        AbstractWindow(const CeGuiString& xmlfile, int inputType, bool closeOnEscape = true, bool modal = false);

        const CeGuiString& getNamePrefix() const;

        void centerWindow();
        bool destroyWindow();

        void bindDestroyWindowToClick(CEGUI::Window* button);
        void bindHideWindowToClick(CEGUI::Window* button);
        void bindDestroyWindowToXButton();
        void bindHideWindowToXButton();

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
