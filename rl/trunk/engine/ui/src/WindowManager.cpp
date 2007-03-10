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
#include "WindowManager.h"
#include <CEGUIWindowManager.h>

#include "CoreSubsystem.h"
#include "AbstractWindow.h"
#include "Exception.h"
#include "GameLoop.h"
#include "UiSubsystem.h"

template<> rl::WindowManager* Ogre::Singleton<rl::WindowManager>::ms_Singleton = 0;

using namespace CEGUI;

namespace rl {

	WindowManager::WindowManager()
	{
	}

	WindowManager::~WindowManager()
	{
	}
	
	void WindowManager::registerWindow(AbstractWindow* window)
	{
		mWindowList.push_back(window);
	}

	void WindowManager::destroyWindow(AbstractWindow* window)
	{
		mWindowList.remove(window);
		window->getWindow()->hide();
		AbstractWindow::getRoot()->removeChildWindow(window->getWindow());
		CEGUI::WindowManager::getSingleton().destroyWindow(window->getWindow());

		///@todo memory leak, aber destroyWindow macht sonst Probleme (heap corruption)
		//delete window;		
	}

	void WindowManager::closeTopWindow()
	{
		for(std::list<AbstractWindow*>::iterator it = mWindowList.begin(); it != mWindowList.end(); it++)
		{
			AbstractWindow* cur = *it;
			if (cur->isVisible() && cur->isClosingOnEscape())
			{
				cur->setVisible(false);
				break;
			}
		}
	}

	bool WindowManager::handleMovedToFront(AbstractWindow* window)
	{
		mWindowList.remove(window);
		mWindowList.push_front(window);
		return true;
	}
	
	bool WindowManager::handleMovedToBack(AbstractWindow* window)
	{
		mWindowList.remove(window);
		mWindowList.push_back(window);
		return true;
	}

	WindowManager& WindowManager::getSingleton()
	{
		return Ogre::Singleton<WindowManager>::getSingleton();
	}

	WindowManager* WindowManager::getSingletonPtr()
	{
		return Ogre::Singleton<WindowManager>::getSingletonPtr();
	}
}
