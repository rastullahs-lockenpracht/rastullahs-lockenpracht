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

#include "TimeSource.h"

#include "GameLoop.h"
#include "SaveGameManager.h"

template <> rl::TimeSourceManager* Ogre::Singleton<rl::TimeSourceManager>::msSingleton = 0;

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
        : TimeSource(false)
        , mTime(0.0)
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
        : TimeSource(true)
        , mTimeFactor(1.0)
        , mTime(0.0)
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
        SaveGameManager::getSingleton().registerSaveGameData(this);
    }

    TimeSourceManager::~TimeSourceManager()
    {
        SaveGameManager::getSingleton().unregisterSaveGameData(this);

        // delete TimeSources
        TimeSourceMap::iterator it = mTimeSources.begin();
        for (; it != mTimeSources.end(); it++)
            if (it->second != NULL)
                delete it->second;
    }

    void TimeSourceManager::registerTimeSource(TimeSource* ts)
    {
        TimeSourceMap::iterator it = mTimeSources.find(ts->getType());
        if (it != mTimeSources.end())
            delete it->second;
        mTimeSources[ts->getType()] = ts;
    }

    TimeSource* TimeSourceManager::getTimeSource(const TimeSource::TimeSourceType& type) const
    {
        std::map<TimeSource::TimeSourceType, TimeSource*>::const_iterator it = mTimeSources.find(type);

        if (it == mTimeSources.end())
        {
            return NULL;
        }

        return it->second;
    }

    void TimeSourceManager::setTimeFactor(const Ogre::Real& factor)
    {
        for (std::map<TimeSource::TimeSourceType, TimeSource*>::iterator it = mTimeSources.begin();
             it != mTimeSources.end(); ++it)
        {
            it->second->setTimeFactor(factor);
        }
    }

    void TimeSourceManager::setPaused(bool paused)
    {
        for (std::map<TimeSource::TimeSourceType, TimeSource*>::iterator it = mTimeSources.begin();
             it != mTimeSources.end(); ++it)
        {
            it->second->setPaused(paused);
        }
    }

    CeGuiString TimeSourceManager::getXmlNodeIdentifier() const
    {
        return "time_sources";
    }

    void TimeSourceManager::writeData(SaveGameFileWriter* writer)
    {
        tinyxml2::XMLElement* timesources
            = writer->appendChildElement(writer->getDocument(), getXmlNodeIdentifier().c_str());

        for (std::map<TimeSource::TimeSourceType, TimeSource*>::const_iterator it_time_sources = mTimeSources.begin();
             it_time_sources != mTimeSources.end(); it_time_sources++)
        {
            tinyxml2::XMLElement* timesource = writer->appendChildElement(timesources, "time_source");
            writer->setAttributeValueAsInteger(timesource, "ID", it_time_sources->first);
            Property time((int)it_time_sources->second->getClock());

            PropertyMap map;
            map["time"] = time;
            writer->writeEachPropertyToElem(timesource, map);
        }
    }

    void TimeSourceManager::readData(SaveGameFileReader* reader)
    {
        XmlElementList rootNodeList
            = reader->getElementsByTagName(reader->getDocument()->RootElement(), getXmlNodeIdentifier().c_str());

        if (!rootNodeList.empty())
        {
            XmlElementList xmlTimeSources = reader->getElementsByTagName(
                rootNodeList[0], "gameobject"); // there should be only one "gameobjects" node
            for (XmlElementList::iterator it = xmlTimeSources.begin(); it != xmlTimeSources.end(); ++it)
            {
                const tinyxml2::XMLElement* xmlTimeSource = *it;
                TimeSource::TimeSourceType ID
                    = (TimeSource::TimeSourceType)reader->getAttributeValueAsInteger(xmlTimeSource, "ID");
                PropertyRecordPtr properties = reader->getPropertiesAsRecord(xmlTimeSource);

                std::map<TimeSource::TimeSourceType, TimeSource*>::const_iterator it_time_sources
                    = mTimeSources.find(ID);
                if (it_time_sources != mTimeSources.end())
                {
                    it_time_sources->second->setClock(properties->toPropertyMap()["time"].toInt());
                }
            }
        }
    }

    int TimeSourceManager::getPriority() const
    {
        return 10000;
    }
}
