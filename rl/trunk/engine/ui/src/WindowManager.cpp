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

#include "CoreSubsystem.h"
#include "Exception.h"
#include "GameLoop.h"
#include "UiSubsystem.h"
#include "WindowUpdater.h"

template<> rl::WindowManager* Ogre::Singleton<rl::WindowManager>::ms_Singleton = 0;

using namespace CEGUI;

namespace rl {

	WindowManager::WindowManager()
	{
		mWindowUpdater = new WindowUpdater();
		GameLoopManager::getSingleton().addSynchronizedTask(mWindowUpdater, FRAME_STARTED);
		//mWindowUpdater->setPaused(true);
	}

	WindowManager::~WindowManager()
	{
		GameLoopManager::getSingleton().removeSynchronizedTask(mWindowUpdater);
		delete mWindowUpdater;
	}
	
	void WindowManager::registerWindow(CeGuiWindow* window)
	{
		mWindowList.push_back(window);
	}

	bool WindowManager::destroyWindow(CeGuiWindow* window)
	{
		mWindowUpdater->fadeOut(window, 0.25, true);
		return true;
	}

	void WindowManager::_doDestroyWindow(CeGuiWindow* window)
	{
		mWindowList.remove(window);
		window->getWindow()->hide();
		CeGuiWindow::getRoot()->removeChildWindow(window->getWindow());
		CEGUI::WindowManager::getSingleton().destroyWindow(window->getWindow());

		//FIXME: memory leak, aber destroyWindow macht sonst Probleme (heap corruption)
		//delete window;		
	}

	void WindowManager::closeTopWindow()
	{
		for(std::list<CeGuiWindow*>::iterator it = mWindowList.begin(); it != mWindowList.end(); it++)
		{
			CeGuiWindow* cur = *it;
			if (cur->isVisible() && cur->isClosingOnEscape())
			{
				cur->setVisible(false);
				break;
			}
		}
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
		return Ogre::Singleton<WindowManager>::getSingleton();
	}

	WindowManager* WindowManager::getSingletonPtr()
	{
		return Ogre::Singleton<WindowManager>::getSingletonPtr();
	}

	void WindowManager::_fadeIn(CeGuiWindow* window, Ogre::Real time, float targetAlpha)
	{
		mWindowUpdater->fadeIn(window, time, targetAlpha);
	}

	void WindowManager::_fadeOut(CeGuiWindow* window, Ogre::Real time, bool destroy)
	{
		mWindowUpdater->fadeOut(window, time, destroy);
	}

	void WindowManager::_fadeInOut(CeGuiWindow* window, Ogre::Real timeFade, 
		Ogre::Real timeHold, bool destroy)
	{
		mWindowUpdater->fadeInOut(window, timeFade, timeHold, destroy);
	}

	void WindowManager::_moveOutLeft(CeGuiWindow* window, Ogre::Real time, bool destroy)
	{
		mWindowUpdater->moveOutLeft(window, time, destroy);
	}
}
