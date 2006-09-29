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

#include <OgreRoot.h>

#include "GameLoop.h"
#include "GameTask.h"
#include "CoreSubsystem.h"
#include "FixRubyHeaders.h"

using Ogre::Singleton;
using Ogre::Real;

template<> rl::GameLoopManager* Singleton<rl::GameLoopManager>::ms_Singleton = 0;

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
		mSynchronizedFrameStartedGameLoop = new SynchronizedGameLoop(FRAME_STARTED);
		mSynchronizedFrameEndedGameLoop = new SynchronizedGameLoop(FRAME_ENDED);
		Ogre::Root::getSingleton().addFrameListener(mSynchronizedFrameStartedGameLoop);
		Ogre::Root::getSingleton().addFrameListener(mSynchronizedFrameEndedGameLoop);
		setPaused(true);
	}

	GameLoopManager::~GameLoopManager()
	{
		Ogre::Root::getSingleton().removeFrameListener(mSynchronizedFrameStartedGameLoop);
		Ogre::Root::getSingleton().removeFrameListener(mSynchronizedFrameEndedGameLoop);

		delete mSynchronizedFrameStartedGameLoop;
		delete mSynchronizedFrameEndedGameLoop;
	}

	void GameLoopManager::addSynchronizedTask(GameTask* newTask, GameLoopSyncTime syncTime)
	{
		if (syncTime == FRAME_STARTED)
			mSynchronizedFrameStartedGameLoop->add(newTask);
		else if (syncTime == FRAME_ENDED)
			mSynchronizedFrameEndedGameLoop->add(newTask);
	}

	void GameLoopManager::removeSynchronizedTask(GameTask* oldTask)
	{
		mSynchronizedFrameStartedGameLoop->remove(oldTask);
		mSynchronizedFrameEndedGameLoop->remove(oldTask);
	}

	bool GameLoopManager::isPaused()
	{
		return mSynchronizedFrameStartedGameLoop->isPaused()
			&& mSynchronizedFrameEndedGameLoop->isPaused();
	}

	void GameLoopManager::setPaused(bool pause)
	{
		mSynchronizedFrameStartedGameLoop->setPaused(pause);
		mSynchronizedFrameEndedGameLoop->setPaused(pause);
	}

	void GameLoopManager::quitGame()
	{
		mSynchronizedFrameEndedGameLoop->quitGame();
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
			GameTaskList::iterator i;

			for(i = mTaskList.begin(); i != mTaskList.end(); ++i)
			{
                GameTask* curTask = *i;
				if( !curTask->isPaused() )
                {

			        LOG_DEBUG(Logger::CORE, curTask->getName() + " start ");
			        double time = (double)CoreSubsystem::getSingleton().getClock();
			    	(*i)->run(timeSinceLastCall);
                    time = (double)CoreSubsystem::getSingleton().getClock() - time;
			        LOG_DEBUG(Logger::CORE, curTask->getName() +  " end "
                        + Ogre::StringConverter::toString(Ogre::Real(time)));
                }
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
		if (mSyncTime == FRAME_STARTED)
		{
			LOG_DEBUG(Logger::CORE, "Sync frame-start start ");
			double time = (double)CoreSubsystem::getSingleton().getClock();
			loop(evt.timeSinceLastFrame);
            time = (double)CoreSubsystem::getSingleton().getClock() - time;
			LOG_DEBUG(Logger::CORE, "Sync frame-start end "
                + Ogre::StringConverter::toString(Ogre::Real(time)));
		}

		return true;
	}

	bool SynchronizedGameLoop::frameEnded(const Ogre::FrameEvent & evt)
	{
		if (mSyncTime == FRAME_ENDED)
		{
			LOG_MESSAGE(Logger::CORE, "Sync frame-end start ");
			double time = (double)CoreSubsystem::getSingleton().getClock();
			loop(evt.timeSinceLastFrame);
            time = (double)CoreSubsystem::getSingleton().getClock() - time;
			LOG_MESSAGE(Logger::CORE, "Sync frame-end end "
				+ Ogre::StringConverter::toString(Ogre::Real(time)));
		}

		return mRunning;
	}

	void SynchronizedGameLoop::quitGame()
	{
		mRunning = false;
	}

	SynchronizedGameLoop::SynchronizedGameLoop(GameLoopSyncTime syncTime)
		: mRunning(true), mSyncTime(syncTime), GameLoop()
	{
	}

}
