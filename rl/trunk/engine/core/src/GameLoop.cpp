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

#include <OgreRoot.h>

#include "GameLoop.h"
#include "GameTask.h"
#include "CoreSubsystem.h"
#include "FixRubyHeaders.h"
//#include "Sleep.h"
#include <boost/thread.hpp>

using Ogre::Singleton;
using Ogre::Real;

template<> rl::GameLoopManager* Singleton<rl::GameLoopManager>::ms_Singleton = 0;
template<> rl::AsynchronousGameLoop* Singleton<rl::AsynchronousGameLoop>::ms_Singleton = 0;
unsigned long rl::AsynchronousGameLoop::sTimeTickInMillis = 999;

namespace rl {

	GameLoopManager* GameLoopManager::getSingletonPtr(void)
	{
		return Singleton<GameLoopManager>::getSingletonPtr();
	}
	GameLoopManager& GameLoopManager::getSingleton(void)
	{
		return Singleton<GameLoopManager>::getSingleton();
	}
using namespace Ogre;

	GameLoopManager::GameLoopManager(unsigned long timePerAsyncTick)
	{
		AsynchronousGameLoop::sTimeTickInMillis = timePerAsyncTick;
		mAsynchronousGameLoop = new AsynchronousGameLoop(timePerAsyncTick);	
		//mAsynchronousGameLoop->setSchedulePriority(OpenThreads::Thread::PRIORITY_LOW);
		
		mSynchronizedGameLoop = new SynchronizedGameLoop();
		Ogre::Root::getSingleton().addFrameListener(mSynchronizedGameLoop);
		setPaused(true);
	}

	GameLoopManager::~GameLoopManager()
	{
		//mAsynchronousGameLoop->cancel();
		delete mAsynchronousGameLoop;

		Ogre::Root::getSingleton().removeFrameListener(mSynchronizedGameLoop);
		delete mSynchronizedGameLoop;
	}

	void GameLoopManager::addSynchronizedTask(GameTask* newTask)
	{
		mSynchronizedGameLoop->add(newTask);
	}

	void GameLoopManager::addAsynchronousTask(GameTask* newTask)
	{
		mAsynchronousGameLoop->add(newTask);
	}

	void GameLoopManager::removeSynchronizedTask(GameTask* oldTask)
	{
		mSynchronizedGameLoop->remove(oldTask);
	}

	void GameLoopManager::removeAsynchronousTask(GameTask* oldTask)
	{
		mAsynchronousGameLoop->remove(oldTask);
	}

	bool GameLoopManager::isPaused()
	{
		return mAsynchronousGameLoop->isPaused() && mSynchronizedGameLoop->isPaused();
	}

	void GameLoopManager::setPaused(bool pause)
	{
		mSynchronizedGameLoop->setPaused(pause);
		mAsynchronousGameLoop->setPaused(pause);
	}

	void GameLoopManager::quitGame()
	{
		mSynchronizedGameLoop->quitGame();
	}

	GameLoop::GameLoop() 
		: mPaused(false)
	{    
		mTaskList.clear();
	}

	GameLoop::~GameLoop()
	{
		mTaskList.clear();    
	}

	void GameLoop::loop(Real timeSinceLastCall)
	{
		if( !mPaused ) 
		{
			std::list<GameTask*>::iterator i;

			for(i = mTaskList.begin(); i != mTaskList.end(); i++)
			{
				if( ! (*i)->isPaused() )
					(*i)->run(timeSinceLastCall);
			}
		}
	}

	bool GameLoop::isPaused()
	{
		return mPaused;
	}

	void GameLoop::setPaused(bool pause)
	{
		mPaused = pause;
	}

	void GameLoop::add(GameTask* task)
	{
		mTaskList.push_back(task);
	}

	void GameLoop::remove(GameTask* task)
	{
		mTaskList.remove(task);
	}

	bool SynchronizedGameLoop::frameStarted(const Ogre::FrameEvent & evt)
	{
		Real timeSinceLastFrame = evt.timeSinceLastEvent;
		loop(timeSinceLastFrame);
		return true;
	}

	bool SynchronizedGameLoop::frameEnded(const Ogre::FrameEvent & evt)
	{
		return mRunning;
	}

	void SynchronizedGameLoop::quitGame()
	{
		mRunning = false;
	}

	SynchronizedGameLoop::SynchronizedGameLoop()
		: mRunning(true), GameLoop()
	{
	}

	AsynchronousGameLoop::AsynchronousGameLoop(unsigned long timeTickInMillis)
	{
		mTimer = PlatformManager::getSingleton().createTimer();
		mTimer->reset();		
		mThread = new boost::thread(&AsynchronousGameLoop::runStatic);
	}

	void AsynchronousGameLoop::run()
	{
		boost::xtime timeToSleep;
		timeToSleep.sec = 0;
		timeToSleep.nsec = sTimeTickInMillis * 1000000L;			

		while(true)
		{
			unsigned long timeSinceLastCall = mTimer->getMilliseconds();
			if (timeSinceLastCall >= sTimeTickInMillis)
			{
				mTimer->reset();
				loop((double)timeSinceLastCall/1000.0);
				boost::thread::sleep(timeToSleep);
			}
            boost::thread::yield();
		}
	}

	void AsynchronousGameLoop::runStatic()
	{
		AsynchronousGameLoop::getSingletonPtr()->run();		
	}

	AsynchronousGameLoop* AsynchronousGameLoop::getSingletonPtr(void)
	{
		return Singleton<AsynchronousGameLoop>::getSingletonPtr();
	}

	AsynchronousGameLoop& AsynchronousGameLoop::getSingleton(void)
	{
		return Singleton<AsynchronousGameLoop>::getSingleton();
	}


}
