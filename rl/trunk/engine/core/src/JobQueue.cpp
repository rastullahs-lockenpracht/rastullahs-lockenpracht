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
#include "stdinc.h"

#include "JobQueue.h"

namespace rl
{

JobQueue::JobQueue()
: Job(false, true)
{
}

JobQueue::~JobQueue()
{
}

void JobQueue::add(Job* job)
{
    mQueue.push_back(job);
}

bool JobQueue::execute(Ogre::Real elapsedTime)
{
    Job* cur = *mQueue.begin();
    bool finished = cur->execute(elapsedTime);
    if (finished)
    {
        mQueue.pop_front();
        if (cur->isDiscardable())
        {
            cur->discard();
        }
        if (cur->destroyWhenDone())
        {
            delete cur;
        }
    }

    if (mQueue.empty())
    {
        return true;
    }
    else
    {
        return false;
    }
}

JobSet::JobSet()
: Job(false, true)
{
}

JobSet::~JobSet()
{
}

void JobSet::add(Job* job)
{
    mSet.insert(job);
}

bool JobSet::execute(Ogre::Real elapsedTime)
{
    std::set<Job*> toDelete;

    for (std::set<Job*>::iterator it = mSet.begin(); 
        it != mSet.end(); ++it)
    {
        Job* cur = *it;
        bool finished = cur->execute(elapsedTime);
        if (finished)
        {
            toDelete.insert(cur);
        }
    }

    for (std::set<Job*>::iterator it = toDelete.begin(); 
        it != toDelete.end(); ++it)
    {
        Job* cur = *it;
        mSet.erase(cur);
        if (cur->isDiscardable())
        {
            cur->discard();
        }
        if (cur->destroyWhenDone())
        {
            delete cur;
        }
    }

    if (mSet.empty())
    {
        return true;
    }
    else
    {
        return false;
    }
}

}
