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

#ifndef __WindowManager_H__
#define __WindowManager_H__

#include "UiPrerequisites.h"
#include <OgreSingleton.h>
#include <set>

namespace rl {

	class CeGuiWindow;

	class _RlUiExport WindowManager : public Ogre::Singleton<WindowManager>
	{
	public:
		WindowManager();

		static WindowManager& getSingleton();
		static WindowManager* getSingletonPtr();

		typedef std::set<CeGuiWindow*> WindowSet;

		CEGUI::Window* loadWindow(const CeGuiString& xmlfile, CeGuiString* prefix = NULL);
		void registerWindow(CeGuiWindow* window);
		bool destroyWindow(CeGuiWindow* window);
		void pruneWindows();

	private:
		WindowSet mActiveWindows;
		WindowSet mWindowsToDelete;
	
		int mNumCeGuiWindows;
	};

}

#endif
