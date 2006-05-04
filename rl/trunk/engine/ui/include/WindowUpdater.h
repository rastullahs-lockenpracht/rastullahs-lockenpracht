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

#ifndef __WindowUpdater_H__
#define __WindowUpdater_H__

#include "UiPrerequisites.h"
#include <OgreSingleton.h>

#include "GameTask.h"

namespace rl {

	class CeGuiWindow;

	class WindowUpdateTask {
	public:
		enum WindowUpdateAction
		{
			WND_SHOW = 1,
			WND_HIDE,
			WND_DESTROY
		};
		
		virtual void run(Ogre::Real elapsedTime) = 0;
		virtual void initialize() = 0;

		const CEGUI::Point& getCurrentPosition() const;
		const Ogre::Real& getCurrentAlpha() const;
		const Ogre::Real& getNormalAlpha() const;
		const Ogre::Real& getTimeLeft() const;
		virtual void setTargetAlpha(const Ogre::Real& alpha);
		CeGuiWindow* getWindow();
		WindowUpdateAction getAction() const;

	protected:
		WindowUpdateTask(CeGuiWindow* window, Ogre::Real time, WindowUpdateAction action);

		CeGuiWindow* mWindow;
		Ogre::Real mTime; 
		Ogre::Real mNormalAlpha;
		CEGUI::Point mCurrentPoint;
		Ogre::Real mCurrentAlpha;

	private:
		WindowUpdateAction mAction;
	};

	class WindowMoveTask : public WindowUpdateTask
	{
	public:
		WindowMoveTask(CeGuiWindow* window, Ogre::Real time, WindowUpdateAction action,
			int targetX, int targetY);

		void run(Ogre::Real elapsedTime);
		void initialize();

	private:
		CEGUI::Point mTargetPoint;
		CEGUI::Point mRatePoint;
	};

	class WindowFadeTask : public WindowUpdateTask
	{
	public:
		WindowFadeTask(CeGuiWindow* window, Ogre::Real time, WindowUpdateAction action, 
			Ogre::Real targetAlpha);

		void initialize();
		void run(Ogre::Real elapsedTime);
		void setTargetAlpha(const Ogre::Real& alpha);

	private:
		Ogre::Real mTargetAlpha;
		Ogre::Real mRateAlpha;
	};

	class WindowFadeInOutTask : public WindowUpdateTask
	{
	public:
		WindowFadeInOutTask(CeGuiWindow* window, Ogre::Real timeFade, Ogre::Real timeHold, 
			WindowUpdateAction action);

		void initialize();
		void run(Ogre::Real elapsedTime);
	private:
		Ogre::Real mTimeFade;
		Ogre::Real mTimeHold;
		Ogre::Real mRateAlpha;
	};

	class WindowUpdater : public GameTask
	{
	public:
		void run(Ogre::Real elapsedTime);

		void fadeIn(
			CeGuiWindow* window, 
			Ogre::Real time, 
			float targetAlpha = 1.0);

		void fadeOut(
			CeGuiWindow* window, 
			Ogre::Real time, 
			bool destroy);

		void fadeInOut(
			CeGuiWindow* window, 
			Ogre::Real timeFade, 
			Ogre::Real timeHold, 
			bool destroy = false);

		void moveOutLeft(
			CeGuiWindow* window, 
			Ogre::Real time, 
			bool destroy);
	private:
		std::set<WindowUpdateTask*> mTasks;
	};


}

#endif
