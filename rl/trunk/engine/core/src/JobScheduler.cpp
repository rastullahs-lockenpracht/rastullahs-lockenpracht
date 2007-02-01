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

#include "JobScheduler.h"
#include "Job.h"
#include "JobListener.h"
#include "GameLoop.h"

using namespace Ogre;

template<> rl::JobScheduler* Singleton<rl::JobScheduler>::ms_Singleton = 0;

namespace rl
{
    JobScheduler& JobScheduler::getSingleton(void)
    {
        return Singleton<JobScheduler>::getSingleton();
    }

    JobScheduler* JobScheduler::getSingletonPtr(void)
    {
        return Singleton<JobScheduler>::getSingletonPtr();
    }

    JobScheduler::JobScheduler()
        : mJobQueue(), mRemovedJobs(), mTokenThreshold(JP_NORMAL), mTicketCounter(0)
    {
    }

    JobScheduler::~JobScheduler()
    {
    }

    unsigned long JobScheduler::addJob(Job* job, JobPriority priority, Real delay, Real maxRuntime,
        JobListener* listener)
    {
        unsigned long ticket = ++mTicketCounter;
        unsigned long clock = GameLoop::getSingleton().getClock();
        unsigned long start = clock + delay*1000;
        unsigned long end = maxRuntime >= Math::POS_INFINITY ?
            0xffffffff : static_cast<unsigned long>(start + maxRuntime*1000);
        JobEntry entry = {job, listener, ticket, priority, priority, start, end, 0.0f, false};
        mJobQueue.push_back(entry);
        return ticket;
    }

    void JobScheduler::run(Ogre::Real time)
    {
        ///@todo use different buckets for jobs not yet started, instead of
        ///      iterating over those each time.

        ///@todo dynamically determine token threshold. Maybe make it work load depending.

        unsigned long clock = GameLoop::getSingleton().getClock();

        // Queue for finished jobs
        JobQueue notDone;

        for (JobQueue::iterator it = mJobQueue.begin(), end = mJobQueue.end(); it != end; ++it)
        {
            JobEntry entry = *it;

            if (entry.start <= clock && clock < entry.end)
            {
                // Is the token threshold reached? 
                if (entry.tokens >= mTokenThreshold)
                {
                    // Yes, pay run fee and execute.
                    entry.tokens = 0;
                    bool runAgain = !entry.job->execute(entry.timeSinceLastCall + time);

                    if (!entry.called)
                    {
                        // Notify listener, the job started for the first time
                        if (entry.listener != NULL)
                        {
                            entry.listener->jobStarted(entry.ticket);
                        }
                        entry.called = true;
                    }

                    if (runAgain)
                    {
                        // Job is not done, reset token count and requeue.
                        entry.tokens = entry.priority;
                        entry.timeSinceLastCall = 0.0f;
                        notDone.push_back(entry);
                    }
                    else
                    {
                        // Notify listener, the job finished regularly.
                        if (entry.listener != NULL)
                        {
                            entry.listener->jobFinished(entry.ticket);
                        }

                        // If we are supposed to delete the Job, do so now.
                        if (entry.job->destroyWhenDone())
                        {
                            delete entry.job;
                        }
                    }
                }
                else
                {
                    // No, increase token count
                    entry.tokens += entry.priority;
                    entry.timeSinceLastCall += time;
                    notDone.push_back(entry);
                }
            }
            else if (!(clock < entry.end))
            {
                // Start time not yet reached. Queue again.
                notDone.push_back(entry);
            }
            else
            {
                // Job reached its end time and didn't want to finish itself, so we do it.
                if (entry.job->isDiscardable())
                {
                    entry.job->discard();
                    if (entry.listener != NULL)
                    {
                        entry.listener->jobDiscarded(entry.ticket);
                    }
                }
            }
        }

        // Copy requeued jobs for next run.
        mJobQueue = notDone;
    }

    void JobScheduler::removeJob(unsigned long ticket)
    {
        JobQueue::iterator it = std::find_if(mJobQueue.begin(), mJobQueue.end(),
            std::bind2nd(FindJobEntryByTicket(), ticket));
        if (it != mJobQueue.end())
        {
            mRemovedJobs.push_back(*it);
            if (it->listener != NULL) it->listener->jobRemoved(ticket);
        }
        else
        {
            ///@todo Log missing job for the ticket.
        }
    }

    const String& JobScheduler::getName() const
    {
        static String NAME = "JobScheduler";

        return NAME;
    }
}
