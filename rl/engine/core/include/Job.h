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

#ifndef __RL_JOB_H__
#define __RL_JOB_H__

#include "CorePrerequisites.h"

#include "TimeSource.h"
#include "Properties.h"

namespace rl
{
    class _RlCoreExport AbstractJob : public PropertyHolder
    {
    public:
        enum JobPersistenceType
        {
            NOT_PERSISTENT,             // the job is not influenced by any save/load - events
            PERSISTENT,                 // the job stores data in (and loads from) a savegamefile, the job is deleted (not discarded) before a new game is loaded
            FINISH_WHEN_GAME_LOADED     // the job is discarded if it is discardable or deleted, when a new game is loaded
        };

        /**
         * Constructor.
         *
         *  @param isDiscardable Set this true, if the Job's goal can be reached in a single
         *         step. For instance if a Job is supposed to slowly close a window by fading
         *         its alpha to 0, then it is discardable, and Job#discard will just finish
         *         the process by closing the window.
         *
         *  @param destroyWhenDone Set this to true, when the JobScheduler shall delete the
         *         Job, after execution is finished. This should usually be the case, but
         *         sometimes it is sensible to pool a number of Jobs for reuse.
         */
        AbstractJob(bool isDiscardable, 
            bool destroyWhenDone);

        virtual ~AbstractJob();

        bool execute(Time time);

        /**
         * This is the function in which the job is supposed to do whatever it is supposed to do.
         * This function is called by the JobScheduler at most once per frame. The frequency depends
         * on the Job's priority given with JobScheduler#addJob.
         * @param time the time source time, since the last call of this function.
         * @return true, when the job is done, false else. If false is returned. The Job will be
         * rescheduled for another execution.
         */
        virtual bool execute(Ogre::Real time) = 0;

        /// Returns the Persistence-Type of this job
        virtual JobPersistenceType getPersistenceType() const = 0;

        virtual TimeSource::TimeSourceType getTimeSource() const = 0;

        /// Returns whether the job can be removed from the queue by the scheduler,
        virtual bool isDiscardable() const;

        /// Returns true, if the Job shall be deleted, if the Job is finished. Returns false else.
        virtual bool destroyWhenDone() const;

        /// Finish whatever the Job is doing. It won't get a chance to continue.
        /// Overloaded functions must *not* call this implementation.
        virtual void discard();

        /// derived from PropertyHolder
        virtual const Property getProperty(const CeGuiString& key) const;
        /// derived from PropertyHolder
        virtual void setProperty(const CeGuiString& key, const Property& value);
        /// derived from PropertyHolder
        virtual PropertyKeys getAllPropertyKeys() const;

        /// returns the name of the class
        virtual const CeGuiString getClassName() const = 0;
    
    protected:
        bool mIsDiscardable;
        bool mDestroyWhenDone;

    };


    /** A Job is an independent executional entity, that encapsules a singe specific task
     *  of some finite duration.
     *  If you want some task to be executed every frame over the length of the game,
     *  use GameTask instead.<br>
     *  A Job is registered with the JobScheduler using JobScheduler#addJob.
     *  The JobScheduler calles Job#execute till the Job has finished, or the JobScheduler
     *  decides to discard the Job, if allowed.
     *
     *  This class provides default implementations for most of AbstractJob's methods
     */
    class _RlCoreExport Job : public AbstractJob
    {
    public:
        /**
         * Constructor.
         *
         *  @param isDiscardable Set this true, if the Job's goal can be reached in a single
         *         step. For instance if a Job is supposed to slowly close a window by fading
         *         its alpha to 0, then it is discardable, and Job#discard will just finish
         *         the process by closing the window.
         *
         *  @param destroyWhenDone Set this to true, when the JobScheduler shall delete the
         *         Job, after execution is finished. This should usually be the case, but
         *         sometimes it is sensible to pool a number of Jobs for reuse.
         */
        Job(bool isDiscardable, 
            bool destroyWhenDone, 
            TimeSource::TimeSourceType type = TimeSource::REALTIME_CONTINUOUS, 
            JobPersistenceType persistence = NOT_PERSISTENT);
        virtual ~Job();

        /**
         * This is the function in which the job is supposed to do whatever it is supposed to do.
         * This function is called by the JobScheduler at most once per frame. The frequency depends
         * on the Job's priority given with JobScheduler#addJob.zz
         * @param time the time source time, since the last call of this function.
         * @return true, when the job is done, false else. If false is returned. The Job will be
         * rescheduled for another execution.
         */
        virtual bool execute(Ogre::Real time) = 0;

        /// Returns the Persistence-Type of this job
        JobPersistenceType getPersistenceType() const;

        TimeSource::TimeSourceType getTimeSource() const;

        /// derived from PropertyHolder
        virtual const Property getProperty(const CeGuiString& key) const;
        /// derived from PropertyHolder
        virtual void setProperty(const CeGuiString& key, const Property& value);
        /// derived from PropertyHolder
        virtual PropertyKeys getAllPropertyKeys() const;

        /// returns the name of the class
        virtual const CeGuiString getClassName() const;

    protected:
        JobPersistenceType mPersistence;
        TimeSource::TimeSourceType mTimeSource;
    };
}

#endif
