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

#include "GameLoop.h"

#include <OgreRoot.h>

#include "SynchronizedTask.h"

#include "AnimationManager.h"
#if OGRE_PLATFORM != PLATFORM_WIN32
#include "SDL.h"
#endif

template<> rl::GameLoop* Singleton<rl::GameLoop>::ms_Singleton = 0;

namespace rl {


GameLoop::GameLoop() 
    :   mRunning(true),
        mPaused(false),
        mSynchronizedTaskList()
{    
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
	return mRunning;
}

bool GameLoop::isPaused()
{
    return mPaused;
}

void GameLoop::quitGame()
{
	mRunning = false;
#if OGRE_PLATFORM != PLATFORM_WIN32
    SDL_Quit();
#endif
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
