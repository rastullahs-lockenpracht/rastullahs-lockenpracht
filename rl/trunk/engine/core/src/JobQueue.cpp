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
#include "stdinc.h"

#include "JobQueue.h"

namespace rl
{

JobQueue::JobQueue()
: AbstractJob(false, true),
    mTimeSource(TimeSource::REALTIME_CONTINUOUS)
{
}

JobQueue::~JobQueue()
{
}

void JobQueue::add(AbstractJob* job)
{
    if (mQueue.empty())
    {
        mTimeSource = job->getTimeSource();
    }

    mQueue.push_back(job);
}

bool JobQueue::execute(Ogre::Real elapsedTime)
{
    AbstractJob* cur = *mQueue.begin();
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

        if (mQueue.empty())
        {
            mTimeSource = TimeSource::REALTIME_CONTINUOUS;
        }
        else
        {
            mTimeSource = (*mQueue.begin())->getTimeSource();
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

AbstractJob::JobPersistenceType JobQueue::getPersistenceType() const
{
    return AbstractJob::PERSISTENT; ///@todo is this correct?
}

TimeSource::TimeSourceType JobQueue::getTimeSource() const 
{
    return mTimeSource;
}

const Ogre::String JobQueue::getClassName() const 
{
    return "JobQueue";
}

JobSet::JobSet()
: AbstractJob(false, true),
    mTimeSource(TimeSource::REALTIME_CONTINUOUS)
{
}

JobSet::~JobSet()
{
}

void JobSet::add(AbstractJob* job)
{
    mSet.insert(job);
    update();
}

bool JobSet::execute(Ogre::Real elapsedTime)
{
    std::set<AbstractJob*> toDelete;

    for (std::set<AbstractJob*>::iterator it = mSet.begin(); 
        it != mSet.end(); ++it)
    {
        AbstractJob* cur = *it;
        bool finished = cur->execute(elapsedTime);
        if (finished)
        {
            toDelete.insert(cur);
        }
    }

    for (std::set<AbstractJob*>::iterator it = toDelete.begin(); 
        it != toDelete.end(); ++it)
    {
        AbstractJob* cur = *it;
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

    if (!toDelete.empty()) 
    {
        update();
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

void JobSet::update()
{
    mTimeSource = TimeSource::REALTIME_CONTINUOUS;

    for (std::set<AbstractJob*>::iterator it = mSet.begin(); 
        it != mSet.end(); ++it)
    {
        AbstractJob* cur = *it;
        if (cur->getTimeSource() == TimeSource::GAMETIME)
        {
            mTimeSource = TimeSource::GAMETIME;
        }
        else if (cur->getTimeSource() == TimeSource::REALTIME_INTERRUPTABLE
            && mTimeSource == TimeSource::REALTIME_CONTINUOUS)
        {
            mTimeSource = TimeSource::REALTIME_INTERRUPTABLE;
        }
    }
}


AbstractJob::JobPersistenceType JobSet::getPersistenceType() const
{
    return AbstractJob::PERSISTENT; ///@todo is this correct?
}

TimeSource::TimeSourceType JobSet::getTimeSource() const 
{
    return mTimeSource;
}

const Ogre::String JobSet::getClassName() const 
{
    return "JobSet";
}


}
