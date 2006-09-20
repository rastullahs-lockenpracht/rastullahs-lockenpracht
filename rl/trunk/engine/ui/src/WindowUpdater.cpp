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

const float rl::WindowFadeTask::DEFAULT_RATE = 1.0f / 0.25f; // fade time 250ms

namespace rl {

	WindowUpdateTask::WindowUpdateTask(
		CeGuiWindow* window, 
		WindowUpdateTask::WindowUpdateAction action)
	:	mWindow(window),
		mAction(action)
	{
		mCurrentPoint = mWindow->getWindow()->getAbsolutePosition();
		mCurrentAlpha = mWindow->getWindow()->getAlpha();
	}

	WindowMoveTask::WindowMoveTask(		
		CeGuiWindow* window, Ogre::Real time, 
		WindowUpdateTask::WindowUpdateAction action, 
		int targetX, int targetY)
	:	WindowUpdateTask(window, action),
		mTargetPoint(targetX, targetY)
	{
		CEGUI::Point delta = mTargetPoint - mWindow->getWindow()->getAbsolutePosition();
		mRatePoint = CEGUI::Point(delta.d_x / mTime, delta.d_y / mTime);
	}

	void WindowMoveTask::run(Ogre::Real elapsedTime)
	{
		mTime -= elapsedTime;

		if (mTime > 0)
		{
			mCurrentPoint += CEGUI::Point(mRatePoint.d_x * elapsedTime, mRatePoint.d_y * elapsedTime);
		}
		else if (mTime <= 0)
		{
			mTime = 0;
			mCurrentPoint = mTargetPoint;
			mWindow->setFading(false);
		}
	}

	bool WindowMoveTask::isFinished() const
	{
		return mTime <= 0;
	}

	WindowFadeTask::WindowFadeTask(CeGuiWindow* window,
		WindowUpdateAction action, Ogre::Real targetAlpha)
	:	WindowUpdateTask(window, action),
		mTargetAlpha(targetAlpha),
		mRate(DEFAULT_RATE)
	{
	}

	void WindowFadeTask::run(Ogre::Real elapsedTime)
	{
		if (getAction() == WND_SHOW)
		{
			if (mCurrentAlpha < mTargetAlpha)
			{
				mCurrentAlpha += mRate * elapsedTime;
			}

			if (mCurrentAlpha >= mTargetAlpha)
			{
				mCurrentAlpha = mTargetAlpha;
				mWindow->setFading(false);
			}
		}
		else if (getAction() == WND_HIDE || getAction() == WND_DESTROY)
		{
			if (mCurrentAlpha > mTargetAlpha)
			{
				mCurrentAlpha -= mRate * elapsedTime;
			}
			
			if (mCurrentAlpha <= mTargetAlpha)
			{
				mCurrentAlpha = mTargetAlpha;
				mWindow->setFading(false);
			}
		}

		LOG_TRIVIAL(Logger::UI, 
			mWindow->getName()
			+ " Curr: "
			+ Ogre::StringConverter::toString(mCurrentAlpha)
			+ " Target: "
			+ Ogre::StringConverter::toString(mTargetAlpha));

	}

	bool WindowFadeTask::isFinished() const
	{
		return mCurrentAlpha == mTargetAlpha;
	}

	const CEGUI::Point& WindowUpdateTask::getCurrentPosition() const
	{
		return mCurrentPoint;
	}

	const Ogre::Real& WindowUpdateTask::getCurrentAlpha() const
	{
		return mCurrentAlpha;
	}

	WindowUpdateTask::WindowUpdateAction WindowUpdateTask::getAction() const
	{
		return mAction;
	}

	void WindowUpdateTask::setAction(WindowUpdateTask::WindowUpdateAction action)
	{
		mAction = action;
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
	:	WindowUpdateTask(window, action),
		mTimeFade(timeFade),
		mTimeHold(timeHold),
		mTime(2*timeFade + timeHold)
	{
		mRateAlpha = window->getNormalAlpha() / mTimeFade;
	}

	void WindowFadeInOutTask::run(Ogre::Real elapsedTime)
	{
		mTime -= elapsedTime;

		if (mTime > mTimeFade + mTimeHold)
		{
			mCurrentAlpha += mRateAlpha * elapsedTime;
		}
		else if (mTime > mTimeHold)
		{
			mCurrentAlpha = mWindow->getNormalAlpha();
		}
		else if (mTime > 0)
		{
			mCurrentAlpha -= mRateAlpha * elapsedTime;
		}
		else if (mTime <= 0)
		{
			mTime = 0;
			mCurrentAlpha = 0.0;
			mWindow->setFading(false);
		}
	}

	bool WindowFadeInOutTask::isFinished() const
	{
		return mTime <= 0;
	}

	void WindowUpdater::run(Ogre::Real elapsedTime)
	{
		if (mTasks.empty())
			return;

		vector<CeGuiWindow*> deleteWindows;

		for (std::set<WindowUpdateTask*>::iterator it = mTasks.begin(); it != mTasks.end();)
		{
			WindowUpdateTask* task = *it;
			task->run(elapsedTime);
			CeGuiWindow* wnd = task->getWindow();
			wnd->getWindow()->setPosition(CEGUI::Absolute, task->getCurrentPosition());
			wnd->getWindow()->setAlpha(task->getCurrentAlpha());
			if (task->isFinished())
			{
				mTasks.erase(it++);
				switch (task->getAction())
				{
				case WindowUpdateTask::WND_DESTROY:
					WindowManager::getSingleton()._doDestroyWindow(wnd);
					break;
				case WindowUpdateTask::WND_HIDE:
					wnd->windowHid();
					wnd->getWindow()->hide();
					wnd->getWindow()->setAlpha(wnd->getNormalAlpha());
					break;
				}
				wnd->_setUpdateTask(NULL);
				delete task;
			}
			else
			{
				++it;
			}
		}
	}

	void WindowUpdater::fadeIn(CeGuiWindow* window, Ogre::Real targetAlpha)
	{
		if (window == NULL)
			Throw(NullPointerException, "window  argument is NULL");

		if (window->_getUpdateTask() == NULL)
		{
			WindowUpdateTask* task = new WindowFadeTask(
					window, 
					WindowUpdateTask::WND_SHOW, 
					targetAlpha);
			mTasks.insert(task);
			window->_setUpdateTask(task);
		}
		else
		{
			WindowUpdateTask* task  = window->_getUpdateTask();
			task->setTargetAlpha(targetAlpha);
			task->setAction(WindowUpdateTask::WND_SHOW);			
		}

		window->getWindow()->setVisible(true);
		window->getWindow()->setAlpha(0.0);
		window->setFading(true);
	}

	void WindowUpdater::fadeOut(CeGuiWindow* window, bool destroy)
	{
		if (window == NULL)
			Throw(NullPointerException, "window  argument is NULL");

		if (window->_getUpdateTask() == NULL)
		{
			WindowUpdateTask* task = new WindowFadeTask(
					window, 
					destroy 
						? WindowUpdateTask::WND_DESTROY 
						: WindowUpdateTask::WND_HIDE, 
					0.0);
			mTasks.insert(task);
			window->_setUpdateTask(task);
		}
		else
		{
			WindowUpdateTask* task  = window->_getUpdateTask();
			task->setTargetAlpha(0.0);
			task->setAction(
				destroy ? WindowUpdateTask::WND_DESTROY 
						: WindowUpdateTask::WND_HIDE);
		}
		window->setFading(true);
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

     const Ogre::String& WindowUpdater::getName() const
    {
        static Ogre::String NAME = "WindowUpdater";

        return NAME;
    }


}
