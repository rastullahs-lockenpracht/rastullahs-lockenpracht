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

template<> rl::GameLoop* Singleton<rl::GameLoop>::ms_Singleton = 0;

namespace rl {

    GameLoop* GameLoop::getSingletonPtr(void)
    {
        return Singleton<GameLoop>::getSingletonPtr();
    }
    GameLoop& GameLoop::getSingleton(void)
    {
        return Singleton<GameLoop>::getSingleton();
    }

    GameLoop::GameLoop()
        : mTaskLists(),
          mLastTimes(),
          mSmoothPeriod(5000),
          mMaxFrameTime(0.5f),
          mQuitRequested(false)
    {
        // create five task lists, one for each taskgroup
        mTaskLists.push_back(new GameTaskList);
        mTaskLists.push_back(new GameTaskList);
        mTaskLists.push_back(new GameTaskList);
        mTaskLists.push_back(new GameTaskList);
        mTaskLists.push_back(new GameTaskList);
    }

    GameLoop::~GameLoop()
    {
        for (size_t i = 0; i < mTaskLists.size(); ++i)
        {
            delete mTaskLists[i];
        }
        mTaskLists.clear();
    }

    void GameLoop::addTask(GameTask* task, TaskGroup group)
    {
        mTaskLists[group]->push_back(task);
    }

    void GameLoop::removeTask(GameTask* task)
    {
        for (size_t i = 0; i < mTaskLists.size(); ++i)
        {
            GameTaskList* tasks = mTaskLists[i];
            GameTaskList::iterator it = std::find(tasks->begin(), tasks->end(), task);
            if (it != tasks->end())
            {
                tasks->erase(it);
                break;
            }
        }
    }

    void GameLoop::quitGame()
    {
        mQuitRequested = true;
    }

    void GameLoop::loop()
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

            for (size_t i = 0; i < mTaskLists.size(); ++i)
            {
                GameTaskList* tasks = mTaskLists[i];
                for (GameTaskList::iterator it = tasks->begin(); it != tasks->end(); ++it)
                {
                    if (!(*it)->isPaused())
                    {
                        (*it)->run(frameTime);
                    }
                }
            }
        }
    }

    // Idea taken from Ogre, but implementation by us.
    // smooths time step over the period mSmoothPeriod.
    unsigned long GameLoop::smoothTime(unsigned long time)
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
