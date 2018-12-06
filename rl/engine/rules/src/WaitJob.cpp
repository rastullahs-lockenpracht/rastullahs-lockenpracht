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
#include "WaitJob.h"
#include "stdinc.h"

#include "Date.h"

namespace rl
{

    WaitJob::WaitJob(const Date& dsatime)
        : Job(false, true, TimeSource::GAMETIME, Job::PERSISTENT)
        , mTime(dsatime.toReal())
    {
    }

    WaitJob::WaitJob(const Ogre::Real& realtime)
        : Job(false, true, TimeSource::REALTIME_INTERRUPTABLE, Job::PERSISTENT)
        , mTime(realtime)
    {
    }

    WaitJob::~WaitJob()
    {
    }

    bool WaitJob::execute(Ogre::Real time)
    {
        mTime -= time;
        if (mTime <= 0)
        {
            return true;
        }

        return false;
    }
}
