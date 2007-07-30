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

#include "stdinc.h" //precompiled header

#include "TimeSource.h"

#include "GameLoop.h"

template<> rl::TimeSourceManager* Ogre::Singleton<rl::TimeSourceManager>::ms_Singleton = 0;

namespace rl
{

    TimeSource::TimeSource(bool interruptable)
        : GameTask(interruptable)
    {
        GameLoop::getSingleton().addTask(this, GameLoop::TG_LOGIC);
    }

    TimeSource::~TimeSource()
    {
        GameLoop::getSingleton().removeTask(this);
    }

    RealTimeContinuous::RealTimeContinuous()
        : TimeSource(false),
        mTime(0.0)
    {
    }

    TimeSource::TimeSourceType RealTimeContinuous::getType() const
    {
        return TimeSource::REALTIME_CONTINUOUS;
    }

    Time RealTimeContinuous::getClock() const
    {
        return mTime;
    }

    void RealTimeContinuous::setClock(const Time& time)
    {
        mTime = time;
    }

    void RealTimeContinuous::setTimeFactor(Ogre::Real factor)
    {
        // can't be accelerated or slowed down
    }

    void RealTimeContinuous::run(Ogre::Real elapsedTime)
    {
        mTime += elapsedTime * 1000;
    }

    const Ogre::String& RealTimeContinuous::getName() const
    {
        static Ogre::String NAME = "Continuous real time clock";
        return NAME;
    }

    RealTimeInterruptable::RealTimeInterruptable()
        : TimeSource(true),
        mTimeFactor(1.0),
        mTime(0.0)
    {
    }

    RealTimeInterruptable::~RealTimeInterruptable()
    {
    }

    TimeSource::TimeSourceType RealTimeInterruptable::getType() const
    {
        return TimeSource::REALTIME_INTERRUPTABLE;
    }

    Time RealTimeInterruptable::getClock() const
    {
        return mTime;
    }

    void RealTimeInterruptable::setClock(const Time& time)
    {
        mTime = time;
    }

    void RealTimeInterruptable::setTimeFactor(Ogre::Real factor)
    {
        mTimeFactor = factor;
    }

    void RealTimeInterruptable::run(Ogre::Real elapsedTime)
    {
        if (!isPaused())
        {
            mTime += mTimeFactor * elapsedTime * 1000;
        }
    }

    const Ogre::String& RealTimeInterruptable::getName() const
    {
        static Ogre::String NAME = "Pausable real time clock";
        return NAME;
    }

    TimeSourceManager::TimeSourceManager()
    {
    }

    void TimeSourceManager::registerTimeSource(TimeSource* ts)
    {
        mTimeSources[ts->getType()] = ts;
    }

    TimeSource* TimeSourceManager::getTimeSource(
        const TimeSource::TimeSourceType& type) const
    {
        std::map<TimeSource::TimeSourceType, TimeSource*>::const_iterator it
            = mTimeSources.find(type);

        if (it == mTimeSources.end())
        {
            return NULL;
        }

        return it->second;
    }

}
