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

#ifndef __WindowManager_H__
#define __WindowManager_H__

#include "UiPrerequisites.h"
#include <OgreSingleton.h>

namespace rl {

	class CeGuiWindow;

	class _RlUiExport WindowManager : public Ogre::Singleton<WindowManager>
	{
	public:
		WindowManager();

		static WindowManager& getSingleton();
		static WindowManager* getSingletonPtr();

		void registerWindow(CeGuiWindow* window);
		bool destroyWindow(CeGuiWindow* window);
		void closeTopWindow();
		bool handleMovedToFront(CeGuiWindow* window);
		bool handleMovedToBack(CeGuiWindow* window);
		
	private:
		std::list<CeGuiWindow*> mWindowList;
	};

}

#endif
