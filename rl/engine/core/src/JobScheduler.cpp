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
#include "Job.h"
#include "JobListener.h"
#include "JobScheduler.h"
#include "SaveGameManager.h"
#include "TimeSource.h"

using namespace Ogre;

template <> rl::JobScheduler* Singleton<rl::JobScheduler>::msSingleton = 0;

namespace rl
{
    JobScheduler::JobScheduler()
        : GameTask(true)
        , mJobQueue()
        , mAddedJobs()
        , mTokenThreshold(JP_NORMAL)
        , mTicketCounter(0)
    {
        SaveGameManager::getSingleton().registerSaveGameData(this);
    }

    JobScheduler::~JobScheduler()
    {
    }

    unsigned long JobScheduler::addJob(
        AbstractJob* job, JobPriority priority, Real delay, Real maxRuntime, JobListener* listener)
    {
        unsigned long ticket = ++mTicketCounter;
        TimeSource* ts = TimeSourceManager::getSingleton().getTimeSource(job->getTimeSource());
        unsigned long clock = ts->getClock();
        unsigned long start = clock + delay * 1000;
        unsigned long end
            = maxRuntime >= Math::POS_INFINITY ? 0xffffffff : static_cast<unsigned long>(start + maxRuntime * 1000);
        JobEntry entry
            = { job, listener, ticket, priority, priority, start, end, start, job->getTimeSource(), false, false };
        mAddedJobs.push_back(entry);
        return ticket;
    }

    void JobScheduler::run(Ogre::Real time)
    {
        ///@todo use different buckets for jobs not yet started, instead of
        ///      iterating over those each time.

        ///@todo dynamically determine token threshold. Maybe make it work load depending.

        // Queue for finished jobs
        JobQueue notDone;

        for (JobQueue::iterator it = mJobQueue.begin(), end = mJobQueue.end(); it != end; ++it)
        {
            JobEntry entry = *it;

            TimeSource::TimeSourceType tst = entry.job->getTimeSource();
            TimeSource* ts = TimeSourceManager::getSingleton().getTimeSource(tst);
            Time clock = ts->getClock();
            if (tst != entry.timeSourceLastCall) // time source has changed, e.g. in a job queue
            {
                entry.timeLastCall = clock;
                entry.timeSourceLastCall = tst;
            }

            if (entry.markedToRemove)
            {
                // Notify listener, the job was removed
                if (entry.listener != NULL)
                {
                    entry.listener->jobRemoved(entry.ticket);
                }

                if (entry.job->destroyWhenDone())
                {
                    delete entry.job;
                }
            }
            else if (entry.start <= clock && clock < entry.end)
            {
                // Is the token threshold reached?
                if (entry.tokens >= mTokenThreshold)
                {
                    // Yes, pay run fee and execute.
                    entry.tokens = 0;
                    bool runAgain = !entry.job->execute(clock - entry.timeLastCall);

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
                        entry.timeLastCall = clock;
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
                    notDone.push_back(entry);
                }
            }
            else if (clock < entry.end)
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
                if (entry.job->destroyWhenDone())
                {
                    delete entry.job;
                }
            }
        }

        // Copy requeued jobs for next run.
        mJobQueue = notDone;
        mJobQueue.insert(mJobQueue.end(), mAddedJobs.begin(), mAddedJobs.end());
        mAddedJobs.clear();
    }

    void JobScheduler::removeJob(unsigned long ticket)
    {
        JobQueue::iterator it
            = std::find_if(mJobQueue.begin(), mJobQueue.end(), std::bind2nd(FindJobEntryByTicket(), ticket));
        if (it != mJobQueue.end())
        {
            // mRemovedJobs.push_back(*it);
            (*it).markedToRemove = true;
        }
        else
        {
            it = std::find_if(mAddedJobs.begin(), mAddedJobs.end(), std::bind2nd(FindJobEntryByTicket(), ticket));
            if (it != mAddedJobs.end())
            {
                mAddedJobs.erase(it);
            }
            else
            {
                ///@todo Log missing job for the ticket.
            }
        }
    }

    const Ogre::String& JobScheduler::getName() const
    {
        static Ogre::String NAME = "JobScheduler";

        return NAME;
    }

    void JobScheduler::writeData(SaveGameFileWriter* writer)
    {
        tinyxml2::XMLElement* jobSchedulerParentNode
            = writer->appendChildElement(writer->getDocument()->RootElement(), getXmlNodeIdentifier().c_str());

        for (JobQueue::const_iterator iter = mJobQueue.begin(); iter != mJobQueue.end(); iter++)
        {
            if (iter->job->getPersistenceType() == Job::PERSISTENT && !(iter->markedToRemove))
            {
                tinyxml2::XMLElement* jobNode = writer->appendChildElement(jobSchedulerParentNode, "job");
                writer->setAttributeValueAsInteger(jobNode, "priority", iter->priority);
                writer->setAttributeValueAsInteger(jobNode, "tokens", iter->tokens);
                writer->setAttributeValueAsInteger(jobNode, "start", iter->start);
                writer->setAttributeValueAsInteger(jobNode, "end", iter->end);
                writer->setAttributeValueAsInt64(jobNode, "timeLastCall", iter->timeLastCall);
                writer->setAttributeValueAsBool(jobNode, "called", iter->called);
                writer->setAttributeValueAsString(jobNode, "classname", iter->job->getClassName());
                CeGuiString timeSource = "unknown";
                switch (iter->job->getTimeSource())
                {
                case TimeSource::GAMETIME:
                    timeSource = "gametime";
                    break;
                case TimeSource::REALTIME_CONTINUOUS:
                    timeSource = "realtime_continuous";
                    break;
                case TimeSource::REALTIME_INTERRUPTABLE:
                    timeSource = "realtime_interruptable";
                    break;
                }
                writer->setAttributeValueAsString(jobNode, "time", timeSource);

                PropertyMap map = iter->job->getAllProperties()->toPropertyMap();
                writer->writeEachPropertyToElem(jobNode, map);
            }
        }
    }

    void JobScheduler::readData(SaveGameFileReader* reader)
    {
        // delete and discard old jobs
        for (JobQueue::iterator iter = mJobQueue.begin(); iter != mJobQueue.end(); iter++)
        {
            if (iter->job->getPersistenceType() == Job::PERSISTENT)
            {
                // delete the job, but do not discard it!
                iter->markedToRemove = true;
            }
            else if (iter->job->getPersistenceType() == Job::FINISH_WHEN_GAME_LOADED)
            {
                // discard the job, then delete it
                if (iter->job->isDiscardable())
                    iter->job->discard();
                iter->markedToRemove = true;
            }
        }

        // load jobs from savegamefile

        XmlElementList rootNodeList
            = reader->getElementsByTagName(reader->getDocument()->RootElement(), getXmlNodeIdentifier().c_str());

        if (!rootNodeList.empty())
        {
            XmlElementList xmlJobs = reader->getElementsByTagName(rootNodeList[0], "job");
            if (!xmlJobs.empty())
            {
                for (XmlElementList::iterator it = xmlJobs.begin(); it != xmlJobs.end(); ++it)
                {
                    const tinyxml2::XMLElement* xmlJob = (*it)->ToElement();
                    if (xmlJob)
                    {
                        JobPriority priority;
                        unsigned short tokens;
                        int start, end;
                        Time timeLastCall;
                        bool called;
                        priority = JobPriority(reader->getAttributeValueAsInteger(xmlJob, "priority"));
                        tokens = reader->getAttributeValueAsInteger(xmlJob, "tokens");
                        start = reader->getAttributeValueAsInteger(xmlJob, "start");
                        end = reader->getAttributeValueAsInteger(xmlJob, "end");
                        timeLastCall = reader->getAttributeValueAsInt64(xmlJob, "timeLastCall");
                        called = reader->getAttributeValueAsBool(xmlJob, "called");

                        Ogre::String className = reader->getAttributeValueAsStdString(xmlJob, "classname");

                        JobCreationMap::iterator it = mJobCreationMap.find(className);
                        if (it == mJobCreationMap.end())
                        {
                            LOG_ERROR(Logger::CORE,
                                "Die Job-Klasse '" + className + "' ist nicht beim JobScheduler registriert!");
                            continue;
                        }

                        AbstractJob* job = it->second();
                        if (job == NULL)
                        {
                            LOG_ERROR(Logger::CORE,
                                "Fehler beim Erstellen eines Objekts der Job-Klasse '" + className + "'!");
                            continue;
                        }
                        PropertyRecordPtr properties = reader->getPropertiesAsRecord(xmlJob);
                        job->setProperties(properties);

                        CeGuiString timeSourceStr = reader->getAttributeValueAsString(xmlJob, "time");
                        TimeSource::TimeSourceType ts = TimeSource::UNKNOWN;
                        if (timeSourceStr == "gametime")
                        {
                            ts = TimeSource::GAMETIME;
                        }
                        else if (timeSourceStr == "realtime_continuous")
                        {
                            ts = TimeSource::REALTIME_CONTINUOUS;
                        }
                        else if (timeSourceStr == "realtime_interruptable")
                        {
                            ts = TimeSource::REALTIME_INTERRUPTABLE;
                        }

                        unsigned long ticket = ++mTicketCounter;
                        JobEntry entry
                            = { job, NULL, ticket, priority, tokens, start, end, timeLastCall, ts, called, false };
                        mJobQueue.push_back(entry);
                    }
                }
            }
        }
    }

    int JobScheduler::getPriority() const
    {
        return 0; // must be loaded before triggers!
    }

    CeGuiString JobScheduler::getXmlNodeIdentifier() const
    {
        static const CeGuiString name = "jobscheduler";
        return name;
    }

    JobScheduler::JobCreationMap JobScheduler::mJobCreationMap;
}
