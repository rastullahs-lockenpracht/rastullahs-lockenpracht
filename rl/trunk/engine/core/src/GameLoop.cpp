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

#include <numeric>

using namespace Ogre;

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

    GameLoopManager::GameLoopManager()
        : mTaskList(),
          mLastTimes(),
          mSmoothPeriod(5000),
          mMaxFrameTime(0.5f),
          mQuitRequested(false)
    {
    }

    GameLoopManager::~GameLoopManager()
    {
    }

    void GameLoopManager::addTask(GameTask* task)
    {
        mTaskList.push_back(task);
    }

    void GameLoopManager::removeTask(GameTask* task)
    {
        mTaskList.remove(task);
    }

    void GameLoopManager::quitGame()
    {
        mQuitRequested = true;
    }

    void GameLoopManager::loop()
    {
        Timer* timer = Root::getSingleton().getTimer();
        // A sensible start value
        unsigned long timeSinceLastLoop = timer->getMilliseconds() - 50; 
        while (!mQuitRequested)
        {
            unsigned long elapsedTime = timer->getMilliseconds();
            Real frameTime = 0.001f * (Real) smoothTime(elapsedTime - timeSinceLastLoop);
            timeSinceLastLoop = elapsedTime;
            if (frameTime > mMaxFrameTime) frameTime = mMaxFrameTime;

			WindowEventUtilities::messagePump();
            Root::getSingleton().renderOneFrame();

            for (GameTaskList::iterator i = mTaskList.begin(); i != mTaskList.end(); ++i)
            {
                if (!(*i)->isPaused())
                {
                    (*i)->run(frameTime);
                }
            }
        }
    }

    // Idea taken from Ogre, but implementation by us.
    // smooths time step over the period mSmoothPeriod.
    unsigned long GameLoopManager::smoothTime(unsigned long time)
    {
        mLastTimes.push_back(time);

        unsigned long limit = 0;
        std::deque<unsigned long>::reverse_iterator i = mLastTimes.rbegin();
        while (i != mLastTimes.rend() && limit < mSmoothPeriod)
        {
            limit += *i;
            ++i;
        }
        mLastTimes.erase(mLastTimes.begin(), mLastTimes.begin() + (mLastTimes.rend() - i));

        return std::accumulate(mLastTimes.begin(), mLastTimes.end(), 0)
            / std::max(mLastTimes.size(), (size_t)1);
    }
}
