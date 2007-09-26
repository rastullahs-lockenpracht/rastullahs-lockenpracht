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

#ifndef __WindowManager_H__
#define __WindowManager_H__

#include "UiPrerequisites.h"
#include <OgreSingleton.h>

#include <OgreNoMemoryMacros.h>
#include "FixRubyHeaders.h"		// die Makros sind hier vor Ruby schon definiert
#include <ruby.h>
#include "FixRubyHeaders.h"
#include <OgreMemoryMacros.h>

#include <CEGUIVector.h>

namespace rl {

	class AbstractWindow;
	class WindowUpdater;

	class _RlUiExport WindowManager : public Ogre::Singleton<WindowManager>
	{
	public:
		WindowManager();
		~WindowManager();

		void registerWindow(AbstractWindow* window);
		void unregisterWindow(AbstractWindow* window);
		void destroyWindow(AbstractWindow* window);
		void closeTopWindow();
		bool handleMovedToFront(AbstractWindow* window);
		bool handleMovedToBack(AbstractWindow* window);

        /// Returns the combined input mask of all open windows
        int getWindowInputMask() const;

        /// Callback for Windows, called when there visibility is changed.
        void _visiblityChanged(AbstractWindow* window, bool isVisible);

        AbstractWindow* getActiveWindow();

	private:
		std::list<AbstractWindow*> mWindowList;

        unsigned short mNumActiveWindowsMouseInput;
        unsigned short mNumActiveWindowsKeyboardInput;
	};
}

#endif
