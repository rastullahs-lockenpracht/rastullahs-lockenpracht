#include "GameLoop.h"

#include <OgreRoot.h>

#include "SynchronizedTask.h"

#include "AnimationManager.h"

template<> rl::GameLoop* Singleton<rl::GameLoop>::ms_Singleton = 0;

namespace rl {


GameLoop::GameLoop() : mSynchronizedTaskList()
{
    mPaused = false;
    Ogre::Root::getSingleton().addFrameListener(this);
    addSynchronizedTask( new AnimationManager() );
}

GameLoop::~GameLoop()
{
    Ogre::Root::getSingleton().removeFrameListener(this);
    mSynchronizedTaskList.clear();
    delete AnimationManager::getSingletonPtr();
}

void GameLoop::loop(Real timeSinceLastCall)
{
    if( !mPaused ) 
    {
        std::list<SynchronizedTask*>::iterator i;

	    for(i=mSynchronizedTaskList.begin(); i!=mSynchronizedTaskList.end(); i++)
        {
            if( ! (*i)->isPaused() )
		        (*i)->run(timeSinceLastCall);
        }
    }
}

bool GameLoop::frameStarted(const Ogre::FrameEvent & evt)
{
	Real timeSinceLastFrame = evt.timeSinceLastEvent;
	
	loop(timeSinceLastFrame);

	return true;
}

bool GameLoop::frameEnded(const Ogre::FrameEvent & evt)
{
	return true;
}

bool GameLoop::isPaused()
{
    return mPaused;
}

void GameLoop::setPaused(bool pause)
{
    mPaused = pause;
}

void GameLoop::addSynchronizedTask(SynchronizedTask* newTask)
{
    mSynchronizedTaskList.push_back(newTask);
}

void GameLoop::removeSynchronizedTask(SynchronizedTask* oldTask)
{
    mSynchronizedTaskList.remove(oldTask);
}

GameLoop& GameLoop::getSingleton(void)
{
	return Singleton<GameLoop>::getSingleton();
}

GameLoop* GameLoop::getSingletonPtr(void)
{
	return Singleton<GameLoop>::getSingletonPtr();
}


}
