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

#include <OgreNoMemoryMacros.h>
#include "FixRubyHeaders.h"		// die Makros sind hier vor Ruby schon definiert
#include <ruby.h>
#include "FixRubyHeaders.h"
#include <OgreMemoryMacros.h>
#undef min

#include <CEGUIVector.h>
#include "GameTask.h"

namespace rl {

	class CeGuiWindow;
	class WindowUpdater;

	class _RlUiExport WindowManager : public Ogre::Singleton<WindowManager>
	{
	public:
		WindowManager();
		~WindowManager();

		static WindowManager& getSingleton();
		static WindowManager* getSingletonPtr();

		void registerWindow(CeGuiWindow* window);
		bool destroyWindow(CeGuiWindow* window);
		void _doDestroyWindow(CeGuiWindow* window);
		void closeTopWindow();
		bool handleMovedToFront(CeGuiWindow* window);
		bool handleMovedToBack(CeGuiWindow* window);

		void _fadeIn(CeGuiWindow* window, Ogre::Real time, float targetAlpha = 1.0);
		void _fadeOut(CeGuiWindow* window, Ogre::Real time, bool destroy);
		void _moveOutLeft(CeGuiWindow* window, Ogre::Real time, bool destroy);

	private:
		std::list<CeGuiWindow*> mWindowList;
		WindowUpdater* mWindowUpdater;
	};

	class WindowUpdateTask {
	public:
		enum WindowUpdateAction
		{
			WND_SHOW = 1,
			WND_HIDE,
			WND_DESTROY
		};
		
		WindowUpdateTask(CeGuiWindow* window, Ogre::Real time, WindowUpdateAction action,
			int targetX, int targetY, Ogre::Real targetAlpha = -1);
		WindowUpdateTask(CeGuiWindow* window, Ogre::Real time, WindowUpdateAction action, 
			Ogre::Real targetAlpha);

		void run(Ogre::Real elapsedTime);
		const CEGUI::Point& getCurrentPosition() const;
		const Ogre::Real& getCurrentAlpha() const;
		const Ogre::Real& getNormalAlpha() const;
		const Ogre::Real& getTimeLeft() const;
		CeGuiWindow* getWindow();
		WindowUpdateAction getAction() const;

	private:
		void initialize();

		CeGuiWindow* mWindow;
		Ogre::Real mTime; 
		CEGUI::Point mTargetPoint;
		CEGUI::Point mRatePoint;
		CEGUI::Point mCurrentPoint;
		Ogre::Real mTargetAlpha;
		Ogre::Real mRateAlpha;
		Ogre::Real mCurrentAlpha;
		Ogre::Real mNormalAlpha;
		bool mCalculatePoint;
		bool mCalculateAlpha;
		WindowUpdateAction mAction;
	};

	class WindowUpdater : public GameTask
	{
	public:
		void run( Ogre::Real elapsedTime );
		void fadeIn(CeGuiWindow* window, Ogre::Real time, float targetAlpha = 1.0);
		void fadeOut(CeGuiWindow* window, Ogre::Real time, bool destroy);
		void moveOutLeft(CeGuiWindow* window, Ogre::Real time, bool destroy);
	private:
		std::set<WindowUpdateTask*> mTasks;
	};


}

#endif
