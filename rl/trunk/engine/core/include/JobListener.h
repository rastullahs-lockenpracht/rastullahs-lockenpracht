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

#ifndef __RL_JOB_LISTENER_H__
#define __RL_JOB_LISTENER_H__

#include "CorePrerequisites.h"

namespace rl
{
    /**
     * A Listener for the state of a Job's lifecycle in the JobScheduler.
     * It can be registered with the JobScheduler, when a Job is added to it.
     */
    class _RlCoreExport JobListener
    {
    public:
        JobListener() {}
        virtual ~JobListener() {}

        /// Called, when a Job is started for the first time.
        virtual void jobStarted(unsigned long ticket) {}
        /// Called, when a Job finishes itself regularly.
        virtual void jobFinished(unsigned long ticket) {}
        /// Called, when a Job is discarded by the JobScheduler.
        virtual void jobDiscarded(unsigned long ticket) {}
        /// Called, when a Job has been removed using JobScheduler#remove.
        virtual void jobRemoved(unsigned long ticket) {}
    };
}

#endif
