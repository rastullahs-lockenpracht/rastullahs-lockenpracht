/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __GameLoop_H__
#define __GameLoop_H__

#include <list>
#include <OgreSingleton.h>

#include "CorePrerequisites.h"

namespace rl {

class SynchronizedTask;

class _RlCoreExport GameLoop : public FrameListener, protected Singleton<GameLoop>
{

public:
	GameLoop();
	virtual ~GameLoop();

    void addSynchronizedTask(SynchronizedTask* newTask);
    void removeSynchronizedTask(SynchronizedTask* oldTask);

	void loop( Real timeSinceLastCall );
    
    bool frameStarted(const Ogre::FrameEvent & evt);
	bool frameEnded(const Ogre::FrameEvent & evt);

    bool isPaused();
    void setPaused(bool pause);

    static GameLoop & getSingleton(void);
	static GameLoop * getSingletonPtr(void);

private:
    bool mPaused;

    std::list<SynchronizedTask*> mSynchronizedTaskList;
};


}
#endif

