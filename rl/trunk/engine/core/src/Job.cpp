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

#include "Job.h"
#include "Exception.h"

namespace rl
{
    AbstractJob::AbstractJob(bool isDiscardable, bool destroyWhenDone)
        : mIsDiscardable(isDiscardable), 
        mDestroyWhenDone(destroyWhenDone)
    {
    }

    AbstractJob::~AbstractJob()
    {
    }

    bool AbstractJob::execute(Time time)
    {
        float ftime = static_cast<float>(time) / 1000.0F;
        if (ftime < 0)
        {
            ftime = 0;
        }
        return execute(ftime);
    }

    bool AbstractJob::isDiscardable() const
    {
        return mIsDiscardable;
    }

    bool AbstractJob::destroyWhenDone() const
    {
        return mDestroyWhenDone;
    }

    void AbstractJob::discard()
    {
        RlFail("Discarded non discardable Job.");
    }

    const Property AbstractJob::getProperty(const CeGuiString& key) const
    {
        Property prop;
        if( key == "discardable" )
        {
            prop.setValue(mIsDiscardable);
        }
        else if( key == "destroywhendone" )
        {
            prop.setValue(mDestroyWhenDone);
        }
        else
        {
            Throw(IllegalArgumentException, key + " is not a property of this Job!");
        }

        return prop;
    }

    void AbstractJob::setProperty(const CeGuiString& key, const Property& value)
    {
        try
        {
            if( key == "discardable" )
            {
                mIsDiscardable = value.toBool();
            }
            else if( key == "destroywhendone" )
            {
                mDestroyWhenDone = value.toBool();
            }
            else
            {
                LOG_WARNING(
                    Logger::RULES,
                    key + " is not a property of this Job!");
            }
        }
        catch (WrongFormatException ex)
        {
            LOG_ERROR(
                Logger::RULES,
                "property " + key + " has the wrong format");
        }
    }

    PropertyKeys AbstractJob::getAllPropertyKeys() const
    {
        PropertyKeys keys;
        keys.insert("discardable");
        keys.insert("destroywhendone");
        return keys;
    }

    Job::Job(bool isDiscardable, bool destroyWhenDone, TimeSource::TimeSourceType timesource, JobPersistenceType persistence)
        : AbstractJob(isDiscardable, destroyWhenDone),
        mTimeSource(timesource),
        mPersistence(persistence)
    {
    }

    Job::~Job()
    {
    }

    TimeSource::TimeSourceType Job::getTimeSource() const
    {
        return mTimeSource;
    }

    Job::JobPersistenceType Job::getPersistenceType() const
    {
        return mPersistence;
    }

    const Property Job::getProperty(const CeGuiString& key) const
    {
        Property prop;
        if( key == "timesource" )
        {
            prop.setValue((int)mTimeSource);
        }
         
        return AbstractJob::getProperty(key);
    }

    void Job::setProperty(const CeGuiString& key, const Property& value)
    {
        try
        {
            if( key == "timesource" )
            {
                mTimeSource = (TimeSource::TimeSourceType)value.toInt();
            }
            else
            {
                AbstractJob::setProperty(key, value);
            }
        }
        catch (WrongFormatException ex)
        {
            LOG_ERROR(
                Logger::RULES,
                "property " + key + " has the wrong format");
        }
    }

    PropertyKeys Job::getAllPropertyKeys() const
    {
        PropertyKeys keys(AbstractJob::getAllPropertyKeys());
        keys.insert("timesource");
        return keys;
    }

    const Ogre::String Job::getClassName() const
    {
        static const Ogre::String name = "";
        return name;
    }
}
