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
          mAddedTasks(),
          mRemovedTasks(),
          mLastTimes(),
          mSmoothPeriod(500),
          mMaxFrameTime(0.250f),
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
        mAddedTasks.push_back(std::make_pair(group, task));
    }

    void GameLoop::removeTask(GameTask* task)
    {
        mRemovedTasks.push_back(task);
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

        // Loop until game exit is requested.
        while (!mQuitRequested)
        {
            // Calculate frame time. This time is smoothed and capped.
            unsigned long elapsedTime = timer->getMilliseconds();
            Real frameTime = 0.001f * (Real) smoothTime(elapsedTime - timeSinceLastLoop);
            timeSinceLastLoop = elapsedTime;
            if (frameTime > mMaxFrameTime) frameTime = mMaxFrameTime;

			// Let Ogre handle Windows/XServer events.
            WindowEventUtilities::messagePump();

            // Render the next frame
            Root::getSingleton().renderOneFrame();

            // Execute all tasks in order.
            for (size_t i = 0; i < mTaskLists.size(); ++i)
            {
                GameTaskList* tasks = mTaskLists[i];
                for (GameTaskList::iterator it = tasks->begin(); it != tasks->end(); ++it)
                {
                    if (*it != NULL && !(*it)->isPaused())
                    {
                        (*it)->run(frameTime);
                    }
                }
            }

            // Update task list, if needed.
            updateTaskList();
        }
    }

    void GameLoop::updateTaskList()
    {        
        // Remove removed tasks.
        for (GameTaskList::iterator it = mRemovedTasks.begin(); it != mRemovedTasks.end(); ++it)
        {
            for (size_t i = 0; i < mTaskLists.size(); ++i)
            {
                GameTaskList* tasks = mTaskLists[i];
                GameTaskList::iterator find_it = std::find(tasks->begin(), tasks->end(), *it);
                if (find_it != tasks->end())
                {
                    tasks->erase(find_it);
                    break;
                }
            }
        }
        mRemovedTasks.clear();

        // Add new ones.
        for (GroupTaskList::iterator it = mAddedTasks.begin(); it != mAddedTasks.end(); ++it)
        {
            mTaskLists[(*it).first]->push_back((*it).second);
        }
        mAddedTasks.clear();
    }

    // Idea taken from Ogre, but implementation by us.
    // smooths time step over the period mSmoothPeriod.
    unsigned long GameLoop::smoothTime(unsigned long time)
    {
        // First add time for this frame
        mLastTimes.push_back(time);

        // Starting from the end of the queue, determine the element, that
        // is just over the threshold mSmoothPeriod.
        unsigned long limit = 0;
        std::deque<unsigned long>::reverse_iterator i = mLastTimes.rbegin();
        while (i != mLastTimes.rend() && limit < mSmoothPeriod)
        {
            limit += *i;
            ++i;
        }
        // Erase all times, that are over.
        mLastTimes.erase(mLastTimes.begin(), mLastTimes.begin() + (mLastTimes.rend() - i));

        // Return the mean of the remaining times.
        return std::accumulate(mLastTimes.begin(), mLastTimes.end(), 0)
            / std::max(mLastTimes.size(), (size_t)1);
    }
}
