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

#ifndef __DEBUGWINDOW_H__
#define __DEBUGWINDOW_H__

#include "UiPrerequisites.h"

#include "AbstractWindow.h"
#include "GameTask.h"

#include <map>

namespace rl {

    class _RlUiExport DebugWindow
		: public Ogre::Singleton<DebugWindow>, public GameTask, public AbstractWindow
    {
    public:
        DebugWindow(void);
        ~DebugWindow();

		void setVisible(bool visible, bool destroyAfterHide = false);

        /** Registers a page on the debug window.
         *  Debug Window allows multiple pages of informations to be registered
         *  This allows components to show debug informations without meddling with
         *  other components debug text.
         *  Only one page at a time is shown.
         *  @param name the key and caption of the registered page.
         *  @throw IllegalArgumentException if there already is a page with this name registered.
         *  @see unregisterPage, setPageText, showNextPage
         */
        void registerPage(const Ogre::String& name);

        /** Unregisters a page on the debug window.
         *  This page is removed from the DebugWindow and setting its text is no longer allowed.
         *  @param name the key of the to be unregistered page.
         *  @throw IllegalArgumentException if there is no page with this name.
         *  @see registerPage, setPageText, showNextPage
         */
        void unregisterPage(const Ogre::String& name);

        /** Unregisters a page on the debug window.
         *  This page is removed from the DebugWindow and setting its text is no longer allowed.
         *  @param page the key of the page, whose text shall be set.
         *  @param text the text to be shown on the page. Use '\n' to break it into multiple lines.
         *  @throw IllegalArgumentException if there is no page with this name.
         */
        void setPageText(const Ogre::String& page, const Ogre::String& text);

        /** Sets the text for the one line Window field at the bottom of the window.
         *  This is used for situational short term info.
         */
        void setMessageText(const Ogre::String& text);
		void run(Ogre::Real elapsedTime);

        /// Changes the shown page to the next page in the list.
        void showNextPage();

        virtual const Ogre::String& getName() const;

    private:
        CEGUI::Window* mMessageText;
        CEGUI::Window* mPageCaption;
        CEGUI::MultiLineEditbox* mPageText;
        typedef std::map<Ogre::String, Ogre::String> PageTextMap;
        PageTextMap mPageTexts;
        Ogre::String mCurrentPage;
        /// Name of the default debug page with FPS statistics
        Ogre::String mDebugPageName;

		void updateFps();
        void updatePageText();
    };
}

#endif
