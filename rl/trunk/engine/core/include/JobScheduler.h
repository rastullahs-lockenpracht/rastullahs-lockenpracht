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

#ifndef __RL_JOB_SCHEDULER_H__
#define __RL_JOB_SCHEDULER_H__

#include "CorePrerequisites.h"
#include "GameTask.h"
#include "SaveGameManager.h"

#include <deque>
#include <functional>

namespace rl
{
    class Job;
    class JobListener;

    /**
     *  Scheduler for fire-and-forget Jobs in RL.
     *  Jobs are independent executional entities, that encapsule a singe specific task
     *  of a some finite duration.
     *  The JobScheduler executes a certain amount of jobs each frame. The number of jobs per frame
     *  depends on the time that can be spent for those jobs. Only a gross estimation is done.
     *  A priority value determines how often a Job is executed.
     *  Whether a Job can run this frame, depends on the number of tokens, the Job accumulated.
     *  If this number is over a dynamic threshold,
     *  it is executed and the tokens are removed from it. If a Job doesn't run this frame, tokens
     *  are added to it. The number of tokens depends on its priority. The higher the priority, the
     *  more tokens added.
     */
    class _RlCoreExport JobScheduler
        : public GameTask,
          public Ogre::Singleton<JobScheduler>,
          public SaveGameData
    {
    public:
        typedef enum {JP_LOW = 10, JP_NORMAL = 20, JP_HIGH = 30} JobPriority;

        JobScheduler();
        virtual ~JobScheduler();

        /**
         * Adds a Job to the scheduler.
         * @param job The job to be added.
         * @param priority The priority, with which the job is added. The higher, the more often
         *        it will be called. But it will never be called more than once per frame. No job
         *        will starve to death.
         * @param delay The delay in seconds, till the Job job is called for the first time.
         *        A value of 0.0f will call the job in the next frame.
         * @param maxRuntime The maximum runtime a job is allowed to run. If it is reached, and
         *        the Job is not yet finished, it will be discarded by calling Job#discard. If
         *        the Job is non-discardable, it will just be removed from the queue.
         * @param listener The JobListener that gets noticed, when the state of the Job changes.
         *
         * @return The ticket used to identify the Job. This ticket is used to identify a Job in the
         *         JobListener. It can be used by the listener to identify the Job, even if it
         *         doesn't exist anymore. When the Job itself is removed from the queue, the ticket
         *         looses its validity to the JobScheduler itself though. If a job is saved and reloaded
         *         from a SaveGameFile, a new ticket will be assigned!
         */
        unsigned long addJob(Job* job, JobPriority priority=JP_NORMAL, Ogre::Real delay=0.0f,
            Ogre::Real maxRuntime=Ogre::Math::POS_INFINITY, JobListener* listener=NULL);

        /**
         * Removes a Job from the queue.
         * This removal is valid in the next frame.
         * If the Job has not yet been executed this frame, it may still be executed a single time.
         * If the ticket is not valid anymore, there won't be an exception, but this event is logged
         * as a warning.
         * @param ticket the ticket of the Job to be removed.
         */
        void removeJob(unsigned long ticket);

        virtual void run(Ogre::Real time);

        virtual const Ogre::String& getName() const;


        /// Override from SaveGameData
        /// Manages saving and loading from the SaveGameFile

        virtual CeGuiString getXmlNodeIdentifier() const;
        virtual void writeData(SaveGameFileWriter* writer);
        virtual void readData(SaveGameFileReader* reader);
        virtual int getPriority() const;  // this should probably be one of the last things to load, so the job can access various things (gameobjects etc)

        typedef Job*(*JobCreateFunction)(void);
        static void registerJobClass(const Ogre::String &name, JobCreateFunction);
    private:
        /// A JobEntry encapsules a Job for the Scheduler, it contains the Job itself and
        /// various administrional data.
        struct JobEntry
        {
            Job* job;                     ///< The Job to be executed.
            JobListener* listener;        ///< attached JobListener or NULL else.
            unsigned long ticket;         ///< ticket to identify the Job.
            JobPriority priority;         ///< priority it runs with.
            unsigned short tokens;        ///< number of accumulated tokens.
            unsigned long start;          ///< when to execute the Job for the first time.
            unsigned long end;            ///< when to discard the Job, if not then finished.
            Ogre::Real timeSinceLastCall; ///< frame time, since the last call of Job#execute.
            bool called;                  ///< false, if the Job has not been called yet.
            bool markedToRemove;          ///< only true, if removeJob with the ticket of this job is called
        };

        /// Functor for finding a Job in a JobQueue by its ticket.
        struct FindJobEntryByTicket : public std::binary_function<JobEntry, unsigned short, bool>
        {
            bool operator()(const JobEntry& jobEntry, unsigned short ticket) const
            {
                return jobEntry.ticket == ticket;
            }
        };

        typedef std::deque<JobEntry> JobQueue;

        JobQueue mJobQueue;
        //JobQueue mRemovedJobs; // should probably replaced by JobsToDelete
        unsigned short mTokenThreshold;
        unsigned long mTicketCounter;

        /// map mit funktionszeigern zum erstellen von job klassen
        typedef std::map<Ogre::String, JobCreateFunction> JobCreationMap;
        static JobCreationMap mJobCreationMap;
    };
}

#endif
