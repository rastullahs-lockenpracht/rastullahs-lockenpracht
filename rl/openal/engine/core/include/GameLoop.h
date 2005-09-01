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

#ifndef __GameLoop_H__
#define __GameLoop_H__

#include <list>
#include <OgreSingleton.h>
//#include <OpenThreads/Thread>
#include <boost/thread.hpp>

#include "CorePrerequisites.h"

namespace rl {

class GameTask;
class GameLoop;
class SynchronizedGameLoop;
class AsynchronousGameLoop;

typedef std::list<GameTask*> GameTaskList;
enum GameLoopSyncTime {
	FRAME_STARTED,
	FRAME_ENDED
};

class _RlCoreExport GameLoopManager : protected Ogre::Singleton<GameLoopManager>
{
public:
	GameLoopManager(unsigned long millisPerAsyncTick);
	virtual ~GameLoopManager();

    void addSynchronizedTask(GameTask* newTask, GameLoopSyncTime syncTime);
	void addAsynchronousTask(GameTask* newTask);
	void removeSynchronizedTask(GameTask* oldTask);
	void removeAsynchronousTask(GameTask* oldTask);	
      
    bool isPaused();
    void setPaused(bool pause);
	void quitGame();

    static GameLoopManager & getSingleton(void);
	static GameLoopManager * getSingletonPtr(void);

private:
    AsynchronousGameLoop* mAsynchronousGameLoop;    
	SynchronizedGameLoop* mSynchronizedFrameStartedGameLoop;
	SynchronizedGameLoop* mSynchronizedFrameEndedGameLoop;
};

class GameLoop
{
public:
	GameLoop();
	virtual ~GameLoop();

	void add(GameTask* task);
	void remove(GameTask* task);
	bool isPaused();
	void setPaused(bool pause);

protected:
	void loop( Ogre::Real timeSinceLastCall );
	
private:	
	GameTaskList mTaskList;
	bool mPaused;
};

class SynchronizedGameLoop : public GameLoop, public Ogre::FrameListener
{
private:
	bool mRunning;
	GameLoopSyncTime mSyncTime;

public:
	SynchronizedGameLoop(GameLoopSyncTime syncTime);
    
	void quitGame();

	bool frameStarted(const Ogre::FrameEvent & evt);
	bool frameEnded(const Ogre::FrameEvent & evt);
};

class AsynchronousGameLoop
	: public GameLoop, protected Ogre::Singleton<AsynchronousGameLoop>
{
public:
	AsynchronousGameLoop(unsigned long timeTickInMillis);
	void run();
	static void runStatic();

	static AsynchronousGameLoop & getSingleton(void);
	static AsynchronousGameLoop * getSingletonPtr(void);

	static unsigned long sTimeTickInMillis;

private: 
	Ogre::Timer* mTimer;
	
	boost::thread* mThread;
};

}
#endif

