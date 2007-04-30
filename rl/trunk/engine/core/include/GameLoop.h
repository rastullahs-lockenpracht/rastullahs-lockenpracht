/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#include <deque>
#include <OgreSingleton.h>

#include "CorePrerequisites.h"

namespace rl {

class GameTask;

/// This class encapsulates the top level game loop.
/// GameLoop::loop is called immediately after RL has been initialised.
/// GameLoop is dumb, it only calls added tasks in the set order.
/// No game logic whatsoever is handled here.
class _RlCoreExport GameLoop : protected Ogre::Singleton<GameLoop>
{
public:
    /// Groups a task can belong to. Tasks are executed in the order listed.
    typedef enum {TG_PHYSICS,  ///< Tasks that update physics
                  TG_INPUT,    ///< Tasks that handle user input
                  TG_LOGIC,    ///< Tasks that control game play. (AI, Triggers, etc..)
                  TG_GRAPHICS, ///< Tasks that update graphics
                  TG_SOUND     ///< Tasks that update sound
                 } TaskGroup;

    GameLoop();
	virtual ~GameLoop();

    /// Adds a task to the game loop. It will not be executed immediately,
    /// but in the next loop.
    /// @param newTask task to added.
    /// @param group tasks are executed in order, they are listed in TaskGroup enum. Choose
    ///        the group, that best fits the purpose of the task.
    /// @sa GameLoop::TaskGroup
    void addTask(GameTask* newTask, TaskGroup group);

    /// Removes a task from the game loop.
    /// The removal is delayed till before the next frame is rendered.
	void removeTask(GameTask* oldTask);

	/// Request the game to quit. The current task loop will finish though.
	void quitGame();

    /// Returns time since game started in Milliseconds.
    unsigned long getClock() const;

    /// Main loop of RL.
    void loop();

    /// Execute the render loop for one frame (for internal use only)
    /// @see CoreSubsystem
    void _executeOneRenderLoop();
      
    static GameLoop & getSingleton(void);
	static GameLoop * getSingletonPtr(void);

private:
    /// Internal struct for storing the tasks in the queue.
    /// Contains additional meta-information useful for scheduling.
    struct GameTaskEntry
    {
        bool operator==(const GameTaskEntry& rhs) const
        {
            return task == rhs.task && valid == rhs.valid;
        }

        /// The task to be executed
        GameTask* task;
        /// Flag whether this task is valid, e.g. removeTask not called for it.
        bool valid;
    };

    /// Functor for finding an entry by the task it holds.
    struct FindEntryByTask : public std::binary_function<GameTaskEntry, GameTask*, bool>
    {
        bool operator()(const GameTaskEntry& entry, GameTask* task) const
        {
            return entry.task == task;
        }
    };

    typedef std::list<GameTaskEntry> GameTaskList;
    typedef std::list<std::pair<TaskGroup, GameTaskEntry> > GroupTaskList;

    std::vector<GameTaskList*> mTaskLists;
    GroupTaskList mAddedTasks;
    GameTaskList mRemovedTasks;

    Ogre::Timer* mTimer;
    /// Time as sampled after the last frame.
    unsigned long mGameTime;

    std::deque<unsigned long> mLastTimes;
    /// In milliseconds, because Ogre's timer works this way.
    unsigned long mSmoothPeriod;
    /// Time cap for frame time, to prevent interpolation problems during spikes.
    Ogre::Real mMaxFrameTime;
	bool mQuitRequested;

    /// Averages frame rate over mSmoothPeriod milliseconds.
    /// Steadies Controls and Physics a bit.
    unsigned long smoothFrameTime(unsigned long time);

    /// Processes queued additions and removals of tasks.
    void updateTaskList();
};

}
#endif

