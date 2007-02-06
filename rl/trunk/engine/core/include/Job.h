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

#ifndef __RL_JOB_H__
#define __RL_JOB_H__

#include "CorePrerequisites.h"

namespace rl
{
    /** A Job is an independent executional entity, that encapsules a singe specific task
     *  of a some finite duration.
     *  If you want some task to be executed every frame over the length of the game,
     *  use GameTask instead.<br>
     *  A Job is registered with the JobScheduler using JobScheduler#addJob.
     *  The JobScheduler calles Job#execute till the Job has finished, or the JobScheduler
     *  decides to discard the Job, if allowed.
     */
    class _RlCoreExport Job
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
        Job(bool isDiscardable, bool destroyWhenDone);
        virtual ~Job();

        /**
         * This is the function in which the job is supposed to do whatever it is supposed to do.
         * This function is called by the JobScheduler at most once per frame. The frequency depends
         * on the Job's priority given with JobScheduler#addJob.
         * @param time the frame-time time, since the last call of this function.
         * @return true, when the job is done, false else. If false is returned. The Job will be
         * rescheduled for another execution.
         */
        virtual bool execute(Ogre::Real time) = 0;

        /// Returns whether the job can be removed from the queue by the scheduler,
        virtual bool isDiscardable();

        /// Finish whatever the Job is doing. It won't get a chance to continue.
        /// Overloaded functions must *not* call this implementation.
        virtual void discard();

        /// Returns true, if the Job shall be deleted, if the Job is finished. Returns false else.
        virtual bool destroyWhenDone();

    protected:
        bool mIsDiscardable;
        bool mDestroyWhenDone;
    };
}

#endif
