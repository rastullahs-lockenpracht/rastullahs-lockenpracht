/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#include "CeGuiWindow.h"
#include "WindowManager.h"

using namespace Ogre;

template<> rl::WindowManager* Ogre::Singleton<rl::WindowManager>::ms_Singleton = 0;

namespace rl {

	WindowManager::WindowManager()
	  : mWindowsToDelete(),
	    mActiveWindows(),
	    mNumCeGuiWindows(0)
	{
	}
	
	CEGUI::Window* WindowManager::loadWindow(const CeGuiString& xmlfile, CeGuiString* prefix)    
	{
		CeGuiString namePrefix = StringConverter::toString(mNumCeGuiWindows);
		if (prefix != NULL)
			prefix->assign(namePrefix);
		mNumCeGuiWindows++;
		
		CEGUI::Window* window = NULL;		
		try 
		{
			window = CEGUI::WindowManager::getSingleton().loadWindowLayout(xmlfile, 
				namePrefix);
		}
		catch(...)
		{
		}
			
		return window;
	}	
 
	bool WindowManager::destroyWindow(CeGuiWindow* window)
	{
		pruneWindows();
		
		WindowSet::iterator iter = mActiveWindows.find(window);
		if (iter == mActiveWindows.end())
			return false;

		window->setVisible(false);
		mActiveWindows.erase(iter);
		mWindowsToDelete.insert(window);

		return true;
	}

	void WindowManager::registerWindow(CeGuiWindow* window)
	{
		pruneWindows();
		mActiveWindows.insert(window);
	}

	WindowManager& WindowManager::getSingleton()
	{
		return Singleton<WindowManager>::getSingleton();
	}

	WindowManager* WindowManager::getSingletonPtr()
	{
		return Singleton<WindowManager>::getSingletonPtr();
	}
	
	void WindowManager::pruneWindows()
	{
		while(!mWindowsToDelete.empty())
		{
			CeGuiWindow* wnd = *mWindowsToDelete.begin();
			mWindowsToDelete.erase(mWindowsToDelete.begin());
			delete wnd;
		}
	}
}
