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
#ifndef __JOBQUEUE_H__
#define __JOBQUEUE_H__

#include "CorePrerequisites.h"

#include "Job.h"

namespace rl
{

    class _RlCoreExport JobQueue : public AbstractJob
    {
    public:
        JobQueue();
        ~JobQueue();

        void add(AbstractJob* job);
        bool empty() const;
        virtual bool execute(Ogre::Real elapsedTime);
        virtual JobPersistenceType getPersistenceType() const;
        virtual TimeSource::TimeSourceType getTimeSource() const;
        virtual const CeGuiString getClassName() const;

    private:
        std::list<AbstractJob*> mQueue;
        TimeSource::TimeSourceType mTimeSource;
    };

    class _RlCoreExport JobSet : public AbstractJob
    {
    public:
        JobSet();
        ~JobSet();

        void add(AbstractJob* job);
        bool empty() const;
        virtual bool execute(Ogre::Real elapsedTime);
        virtual JobPersistenceType getPersistenceType() const;
        virtual TimeSource::TimeSourceType getTimeSource() const;
        virtual const CeGuiString getClassName() const;

    private:
        std::set<AbstractJob*> mSet;
        TimeSource::TimeSourceType mTimeSource;

        void update();
    };
}

#endif // __JOBQUEUE_H__
