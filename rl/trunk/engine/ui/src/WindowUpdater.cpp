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
#include "CoreSubsystem.h"
#include "Exception.h"
#include "WindowManager.h"
#include "WindowUpdater.h"

using namespace CEGUI;

namespace rl {

	WindowUpdateTask::WindowUpdateTask(
		CeGuiWindow* window, Ogre::Real time, 
		WindowUpdateTask::WindowUpdateAction action)
	:	mWindow(window),
		mTime(time), 
		mAction(action)
	{
		mCurrentPoint = mWindow->getWindow()->getAbsolutePosition();
		mCurrentAlpha = mWindow->getWindow()->getAlpha();
	}

	WindowMoveTask::WindowMoveTask(		
		CeGuiWindow* window, Ogre::Real time, 
		WindowUpdateTask::WindowUpdateAction action, 
		int targetX, int targetY)
	:	WindowUpdateTask(window, time, action),
		mTargetPoint(targetX, targetY)
	{
		initialize();
	}

	void WindowMoveTask::initialize()
	{
		CEGUI::Point delta = mTargetPoint - mWindow->getWindow()->getAbsolutePosition();
		mRatePoint = CEGUI::Point(delta.d_x / mTime, delta.d_y / mTime);
		mNormalAlpha = mWindow->getWindow()->getAlpha();
	}

	void WindowMoveTask::run(Ogre::Real elapsedTime)
	{
		mTime -= elapsedTime;
		if (mTime < 0)
		{
			mTime = 0;
		}

		if (mTime > 0)
		{
			mCurrentPoint += CEGUI::Point(mRatePoint.d_x * elapsedTime, mRatePoint.d_y * elapsedTime);
		}
		else if (mTime == 0)
		{
			mCurrentPoint = mTargetPoint;
			mWindow->setFading(false);
		}
	}

	WindowFadeTask::WindowFadeTask(CeGuiWindow* window, Ogre::Real time, 
		WindowUpdateAction action, Ogre::Real targetAlpha)
	:	WindowUpdateTask(window, time, action),
		mTargetAlpha(targetAlpha)
	{
		initialize();
	}

	void WindowFadeTask::initialize()
	{
		mRateAlpha = 
			(mTargetAlpha - mWindow->getWindow()->getAlpha()) / mTime;
		mNormalAlpha = std::max(
			mWindow->getWindow()->getAlpha(),
			mTargetAlpha);
	}


	void WindowFadeTask::run(Ogre::Real elapsedTime)
	{
		mTime -= elapsedTime;
		if (mTime < 0)
		{
			mTime = 0;
		}

		if (mTime > 0)
		{
			mCurrentAlpha += mRateAlpha * elapsedTime;
		}
		else if (mTime == 0)
		{
			mCurrentAlpha = mTargetAlpha;
			mWindow->setFading(false);
		}
	}

	const CEGUI::Point& WindowUpdateTask::getCurrentPosition() const
	{
		return mCurrentPoint;
	}

	const Ogre::Real& WindowUpdateTask::getCurrentAlpha() const
	{
		return mCurrentAlpha;
	}

	const Ogre::Real& WindowUpdateTask::getNormalAlpha() const
	{
		return mNormalAlpha;
	}

	const Ogre::Real& WindowUpdateTask::getTimeLeft() const
	{
		return mTime;
	}

	WindowUpdateTask::WindowUpdateAction WindowUpdateTask::getAction() const
	{
		return mAction;
	}

	CeGuiWindow* WindowUpdateTask::getWindow()
	{
		return mWindow;
	}

	void WindowUpdateTask::setTargetAlpha(const Ogre::Real& alpha)
	{
	}

	void WindowFadeTask::setTargetAlpha(const Ogre::Real& alpha)
	{
		mTargetAlpha = alpha;
	}

	WindowFadeInOutTask::WindowFadeInOutTask(
		CeGuiWindow* window, Ogre::Real timeFade, Ogre::Real timeHold, 
		WindowUpdateAction action)
	:	WindowUpdateTask(window, 2*timeFade + timeHold, action),
		mTimeFade(timeFade),
		mTimeHold(timeHold)
	{
		initialize();
	}

	void WindowFadeInOutTask::initialize()
	{
		mNormalAlpha = mWindow->getWindow()->getAlpha();
		mRateAlpha = mNormalAlpha / mTimeFade;
	}

	void WindowFadeInOutTask::run(Ogre::Real elapsedTime)
	{
		mTime -= elapsedTime;
		if (mTime < 0)
		{
			mTime = 0;
		}

		if (mTime > mTimeFade + mTimeHold)
		{
			mCurrentAlpha += mRateAlpha * elapsedTime;
		}
		else if (mTime > mTimeHold)
		{
			mCurrentAlpha = mNormalAlpha;
		}
		else if (mTime > 0)
		{
			mCurrentAlpha -= mRateAlpha * elapsedTime;
		}
		else if (mTime == 0)
		{
			mCurrentAlpha = mNormalAlpha;
			mWindow->setFading(false);
		}
	}

	void WindowUpdater::run(Ogre::Real elapsedTime)
	{
		RL_LONGLONG start = CoreSubsystem::getSingleton().getClock();

		if (mTasks.empty())
			return;

		vector<CeGuiWindow*> deleteWindows;

		for (std::set<WindowUpdateTask*>::iterator it = mTasks.begin(); it != mTasks.end();)
		{
			WindowUpdateTask* task = *it;
			task->run(elapsedTime);
			task->getWindow()->getWindow()->setPosition(CEGUI::Absolute, task->getCurrentPosition());
			task->getWindow()->getWindow()->setAlpha(task->getCurrentAlpha());
			if (task->getTimeLeft() <= 0)
			{
				mTasks.erase(it++);
				switch (task->getAction())
				{
				case WindowUpdateTask::WND_DESTROY:
					WindowManager::getSingleton()._doDestroyWindow(task->getWindow());
					break;
				case WindowUpdateTask::WND_HIDE:
					task->getWindow()->windowHid();
					task->getWindow()->getWindow()->hide();
					task->getWindow()->getWindow()->setAlpha(task->getNormalAlpha());
					break;
				}
				task->getWindow()->_setUpdateTask(NULL);
				delete task;
			}
			else
			{
				++it;
			}
		}

		Logger::getSingleton().log(
			Logger::CORE, 
			Logger::LL_TRIVIAL, 
			"    WU time "
			 + Ogre::StringConverter::toString(
					Ogre::Real((double)(CoreSubsystem::getSingleton().getClock()-start))));
	}

	void WindowUpdater::fadeIn(CeGuiWindow* window, Ogre::Real time, Ogre::Real targetAlpha)
	{
		if (window == NULL)
			Throw(NullPointerException, "window  argument is NULL");

		WindowUpdateTask* task = new WindowFadeTask(
				window, 
				time, 
				WindowUpdateTask::WND_SHOW, 
				targetAlpha);
		mTasks.insert(task);
		window->setFading(true);
		window->_setUpdateTask(task);
	}

	void WindowUpdater::fadeOut(CeGuiWindow* window, Ogre::Real time, bool destroy)
	{
		if (window == NULL)
			Throw(NullPointerException, "window  argument is NULL");

		WindowUpdateTask* task = new WindowFadeTask(
				window, 
				time, 
				destroy ? WindowUpdateTask::WND_DESTROY : WindowUpdateTask::WND_HIDE, 
				0.0);
		mTasks.insert(task);
		window->setFading(true);
		window->_setUpdateTask(task);
	}

	void WindowUpdater::fadeInOut(rl::CeGuiWindow *window, Ogre::Real timeFade, 
		Ogre::Real timeHold, bool destroy)
	{
		if (window == NULL)
			Throw(NullPointerException, "window  argument is NULL");

		WindowUpdateTask* task = new WindowFadeInOutTask(
				window, 
				timeFade, timeHold, 
				destroy ? WindowUpdateTask::WND_DESTROY : WindowUpdateTask::WND_HIDE);
		mTasks.insert(task);
		window->setFading(true);
		window->_setUpdateTask(task);
	}

	void WindowUpdater::moveOutLeft(CeGuiWindow* window, Ogre::Real time, bool destroy)
	{
		if (window == NULL)
			Throw(NullPointerException, "window  argument is NULL");

		CEGUI::Window* wnd = window->getWindow();
		WindowUpdateTask* task = new WindowMoveTask(
				window, 
				time,
				destroy ? WindowUpdateTask::WND_DESTROY : WindowUpdateTask::WND_HIDE, 
				-wnd->getAbsoluteWidth(), 
				wnd->getAbsoluteYPosition());
		mTasks.insert(task);
		window->setFading(true);
		window->_setUpdateTask(task);
	}

}
