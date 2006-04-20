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

#undef max

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
		delete window;		
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

	void WindowManager::_moveOutLeft(CeGuiWindow* window, Ogre::Real time, bool destroy)
	{
		mWindowUpdater->moveOutLeft(window, time, destroy);
	}

	WindowUpdateTask::WindowUpdateTask(
		CeGuiWindow* window, Ogre::Real time, 
		WindowUpdateTask::WindowUpdateAction action,
		int targetX, int targetY, Ogre::Real targetAlpha)
	:	mWindow(window),
		mTime(time), 
		mTargetPoint(targetX, targetY), 
		mTargetAlpha(targetAlpha),
		mAction(action)
	{
		initialize();
	}

	WindowUpdateTask::WindowUpdateTask(
		CeGuiWindow* window, Ogre::Real ftime, 
		WindowUpdateTask::WindowUpdateAction action,
		Ogre::Real targetAlpha)
	:	mWindow(window),
		mTime(ftime), 
		mTargetPoint(-9999999, -9999999), 
		mTargetAlpha(targetAlpha),
		mAction(action)
	{
		initialize();
	}

	void WindowUpdateTask::initialize()
	{
		mCalculatePoint =(mTargetPoint.d_x != -9999999);
		mCalculateAlpha =(mTargetAlpha != -1);
			
		CEGUI::Point delta = mTargetPoint - mWindow->getWindow()->getAbsolutePosition();
		mRatePoint = CEGUI::Point(delta.d_x / mTime, delta.d_y / mTime);
		mRateAlpha = 
			(mTargetAlpha - mWindow->getWindow()->getAlpha()) / mTime;
		mCurrentPoint = mWindow->getWindow()->getAbsolutePosition();
		mCurrentAlpha = mWindow->getWindow()->getAlpha();

		if (mCalculateAlpha)
		{
			mNormalAlpha = std::max(
				mWindow->getWindow()->getAlpha(),
				mTargetAlpha);
		}
		else
		{
			mNormalAlpha = mWindow->getWindow()->getAlpha();
		}
	}

	void WindowUpdateTask::run(Ogre::Real elapsedTime)
	{
		mTime -= elapsedTime;
		if (mTime < 0)
		{
			mTime = 0;
		}

		if (mTime > 0)
		{
			if (mCalculatePoint)
			{
				mCurrentPoint += CEGUI::Point(mRatePoint.d_x * elapsedTime, mRatePoint.d_y * elapsedTime);
			}
			if (mCalculateAlpha)
			{
				mCurrentAlpha += mRateAlpha * elapsedTime;
			}			
		}
		else if (mTime == 0)
		{
			if (mCalculatePoint)
			{
				mCurrentPoint = mTargetPoint;
			}
			if (mCalculateAlpha)
			{
				mCurrentAlpha = mTargetAlpha;
			}
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
		mTargetAlpha = alpha;
	}

	void WindowUpdater::run( Ogre::Real elapsedTime )
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
			Ogre::LML_TRIVIAL, 
			"    WU time "
			 + Ogre::StringConverter::toString(
					Ogre::Real((double)(CoreSubsystem::getSingleton().getClock()-start))));
	}

	void WindowUpdater::fadeIn(CeGuiWindow* window, Ogre::Real time, Ogre::Real targetAlpha)
	{
		if (window == NULL)
			Throw(NullPointerException, "window  argument is NULL");

		WindowUpdateTask* task = new WindowUpdateTask(
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

		WindowUpdateTask* task = new WindowUpdateTask(
				window, 
				time, 
				destroy ? WindowUpdateTask::WND_DESTROY : WindowUpdateTask::WND_HIDE, 
				0.0);
		mTasks.insert(task);
		window->setFading(true);
		window->_setUpdateTask(task);
	}

	void WindowUpdater::moveOutLeft(CeGuiWindow* window, Ogre::Real time, bool destroy)
	{
		if (window == NULL)
			Throw(NullPointerException, "window  argument is NULL");

		CEGUI::Window* wnd = window->getWindow();
		WindowUpdateTask* task = new WindowUpdateTask(
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
