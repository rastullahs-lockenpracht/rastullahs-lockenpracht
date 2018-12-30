/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h" //precompiled header

#include "GameLoop.h"

#include "CoreSubsystem.h"
#include "Exception.h"
#include "FixRubyHeaders.h"
#include "GameTask.h"
#include "Sleep.h"

#include <numeric>

using namespace Ogre;

template <> rl::GameLoop* Singleton<rl::GameLoop>::msSingleton = 0;

namespace rl
{
    GameLoop::GameLoop()
        : mTaskLists()
        , mAddedTasks()
        , mRemovedTasks()
        , mTimer(NULL)
        , mGameTime(0)
        , mLastTimes()
        , mSmoothedFrames(3)
        , mMaxFrameTime(0.1f)
        , mMinFrameTime(1.0 / 60)
        , mQuitRequested(false)
        , mPaused(false)
    {
        // create five task lists, one for each taskgroup
        mTaskLists.push_back(new GameTaskList());
        mTaskLists.push_back(new GameTaskList());
        mTaskLists.push_back(new GameTaskList());
        mTaskLists.push_back(new GameTaskList());
        mTaskLists.push_back(new GameTaskList());

        mTimer = new Timer();
    }

    GameLoop::~GameLoop()
    {
        for (size_t i = 0; i < mTaskLists.size(); ++i)
        {
            delete mTaskLists[i];
        }
        mTaskLists.clear();
        delete mTimer;
    }

    void GameLoop::addTask(GameTask* task, TaskGroup group)
    {
        RlAssert1(task != NULL);
        GameTaskEntry entry = { task, true };
        mAddedTasks.push_back(std::make_pair(group, entry));
    }

    void GameLoop::removeTask(GameTask* task)
    {
        // find the removed task entry, and set it to invalid.
        for (size_t i = 0; i < mTaskLists.size(); ++i)
        {
            GameTaskList* tasks = mTaskLists[i];
            GameTaskList::iterator find_it
                = std::find_if(tasks->begin(), tasks->end(), std::bind2nd(FindEntryByTask(), task));
            if (find_it != tasks->end())
            {
                find_it->valid = false;
                break;
            }
        }
        // Add it to the removed list, so we can find it faster in updateTaskList().
        GameTaskEntry entry = { task, false };
        mRemovedTasks.push_back(entry);
    }

    void GameLoop::quitGame()
    {
        mQuitRequested = true;
    }

    void GameLoop::loop()
    {
        // A sensible start value
        mGameTime = mTimer->getMilliseconds() - 50;

        // Loop until game exit is requested.
        while (!mQuitRequested && !CoreSubsystem::getSingleton().getRenderWindow()->isClosed())
        {
            _executeOneRenderLoop();
        }
    }

    bool GameLoop::isPaused() const
    {
        return mPaused;
    }

    void GameLoop::setPaused(bool paused)
    {
        mPaused = paused;
    }

    void GameLoop::setTimeFactor(Ogre::Real timeFactor)
    {
        for (size_t i = 0; i < mTaskLists.size(); ++i)
        {
            for (std::list<GameTaskEntry>::iterator it = mTaskLists[i]->begin(); it != mTaskLists[i]->end(); ++it)
            {
                it->task->setTimeFactor(timeFactor);
            }
        }
    }

    void GameLoop::_executeOneRenderLoop(bool executeTasks)
    {
        // Calculate frame time. This time is smoothed and capped.
        unsigned long elapsedTime = mTimer->getMilliseconds();

        unsigned long unsmoothedFrameTime = elapsedTime - mGameTime;

        if (elapsedTime < mGameTime)
            unsmoothedFrameTime = 1;

        if (unsmoothedFrameTime < mMinFrameTime * 1000)
        {
            msleep(floor((mMinFrameTime * 1000 - unsmoothedFrameTime)));
            elapsedTime = mTimer->getMilliseconds();
            unsmoothedFrameTime = elapsedTime - mGameTime;
            if (elapsedTime < mGameTime)
                unsmoothedFrameTime = 1;
        }

        if (unsmoothedFrameTime > mMaxFrameTime * 1000)
        {
            LOG_DEBUG(Logger::CORE, "The current frame time was truncated at maximum.");
            unsmoothedFrameTime = mMaxFrameTime * 1000;
        }
        Real frameTime = 0.001f * (Real)smoothFrameTime(unsmoothedFrameTime);
        mGameTime = elapsedTime;

        // Let Ogre handle Windows/XServer events.
        //        Ogre::WindowEventUtilities::messagePump();

        // Render the next frame
        Root::getSingleton().renderOneFrame();

        // Execute all tasks in order.
        for (size_t i = 0; i < mTaskLists.size() && executeTasks; ++i)
        {
            GameTaskList* tasks = mTaskLists[i];
            for (GameTaskList::iterator it = tasks->begin(); it != tasks->end(); ++it)
            {
                if (it->valid && !it->task->isPaused() && !(isPaused() && it->task->isInterruptable()))
                {
                    it->task->run(frameTime);
                }
            }
        }

        // Update task list, if needed.
        updateTaskList();
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
    // fixed number of smoothed frames
    unsigned long GameLoop::smoothFrameTime(unsigned long time)
    {
        // remove the last frame, if enough frame-times are saved
        if (mLastTimes.size() >= std::max(mSmoothedFrames, (unsigned long)1))
            mLastTimes.pop_front();
        // First add time for this frame
        mLastTimes.push_back(time);

        // Return the mean of the remaining times.
        // Do not return zero
        if (mLastTimes.size() == 0)
            return time;
        else
        {
            unsigned long smoothedTime = (long double)(std::accumulate(mLastTimes.begin(), mLastTimes.end(), 0))
                    / std::max(mLastTimes.size(), (size_t)1)
                + 0.5; // round correctly with +.5

            return smoothedTime;
        }
    }
}
