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
#include <deque>
#include <OgreSingleton.h>
#include <boost/thread.hpp>

#include "CorePrerequisites.h"

namespace rl {

class GameTask;

class _RlCoreExport GameLoop : protected Ogre::Singleton<GameLoop>
{
public:
    typedef enum {TG_PHYSICS, TG_INPUT, TG_LOGIC, TG_GRAPHICS, TG_SOUND} TaskGroup;

	GameLoop();
	virtual ~GameLoop();

    void addTask(GameTask* newTask, TaskGroup group);
	void removeTask(GameTask* oldTask);

	/// Request the game to quit. The current task loop will finish though.
	void quitGame();

    /// Main loop of RL.
    void loop();
      
    static GameLoop & getSingleton(void);
	static GameLoop * getSingletonPtr(void);

private:
    typedef std::list<GameTask*> GameTaskList;

    std::vector<GameTaskList*> mTaskLists;
    std::deque<unsigned long> mLastTimes;
    /// In milliseconds, because Ogre's timer works this way.
    unsigned long mSmoothPeriod;
    Ogre::Real mMaxFrameTime;
	bool mQuitRequested;

    /// Averages frame rate over mSmoothPeriod milliseconds.
    /// Steadies Controls and Physics a bit.
    unsigned long smoothTime(unsigned long time);
};

}
#endif

