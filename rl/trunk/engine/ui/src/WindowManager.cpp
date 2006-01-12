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

#include "CeGuiWindow.h"
#include "WindowManager.h"
#include <CEGUIWindowManager.h>

using namespace CEGUI;

template<> rl::WindowManager* Ogre::Singleton<rl::WindowManager>::ms_Singleton = 0;

namespace rl {

	WindowManager::WindowManager()
	{
	}
	
	void WindowManager::registerWindow(CeGuiWindow* window)
	{
		mWindowList.push_back(window);
	}

	bool WindowManager::destroyWindow(CeGuiWindow* window)
	{
		mWindowList.remove(window);
		CeGuiWindow::getRoot()->removeChildWindow(window->getWindow());
		window->setVisible(false);
		CEGUI::WindowManager::getSingleton().destroyWindow(window->getWindow());
		delete window;
		return true;
	}

	CeGuiWindow* WindowManager::getTopWindow()
	{
		for(std::list<CeGuiWindow*>::iterator it = mWindowList.begin(); it != mWindowList.end(); it++)
		{
			CeGuiWindow* cur = *it;
			if (cur->isVisible())
				return cur;
		}
		return mWindowList.front();
	}

	bool WindowManager::handleMovedToFront(CeGuiWindow* window)
	{
		mWindowList.remove(window);
		mWindowList.push_front(window);
		return true;
	}
	
	bool WindowManager::handleMovedToBack(CeGuiWindow* window)
	{
		mWindowList.remove(window);
		mWindowList.push_back(window);
		return true;
	}

	WindowManager& WindowManager::getSingleton()
	{
		return Singleton<WindowManager>::getSingleton();
	}

	WindowManager* WindowManager::getSingletonPtr()
	{
		return Singleton<WindowManager>::getSingletonPtr();
	}
}
