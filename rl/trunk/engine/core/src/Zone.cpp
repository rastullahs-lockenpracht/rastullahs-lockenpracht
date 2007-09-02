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

#include "Zone.h"

#include "ActorManager.h"
#include "GameAreaEvent.h"
#include "GameEventManager.h"
#include "Trigger.h"
#include "ZoneManager.h"

namespace rl
{


    Zone::Zone(long id) : mId(id)
    {
    }

    Zone::~Zone()
    {
    }

	void Zone::addLight(Actor* lo)
	{
		if (lo != NULL)
		{
			mLights.push_back(lo);
		}
	}

	void Zone::addSound(const Ogre::String& sound)
	{
		if (sound.length() > 0)
		{
			mSounds.push_back(sound);
		}
	}

	void Zone::addTrigger(Trigger* trigger)
	{
		if (trigger != NULL)
		{
			mTriggers.push_back(trigger);
		}
	}

	std::list<Actor*> Zone::getLights() const
	{
		std::list<Actor*> rval(mLights);
		return rval;
	}

	std::list<Ogre::String> Zone::getSounds() const
	{
		std::list<Ogre::String> rval(mSounds);
		return rval;
	}

	std::list<Trigger*> Zone::getTriggers() const
	{
		std::list<Trigger*> rval(mTriggers);
		return rval;
	}

    void Zone::removeLight(Actor *light)
    {
        if( !light )
            Throw(NullPointerException, "Light-Actor is NULL!");
        std::list<Actor*>::iterator it = 
            std::find(mLights.begin(), mLights.end(), light);
        if( it == mLights.end() )
            Throw(IllegalArgumentException, "Could not find light-actor '" + light->getName() + "' in this zone!");

        mLights.erase(it);
    }

    void Zone::removeSound(const Ogre::String& sound)
    {
        std::list<Ogre::String>::iterator it = 
            std::find(mSounds.begin(), mSounds.end(), sound);
        if( it == mSounds.end() )
            Throw(IllegalArgumentException, "Could not find sound '" + sound + "' in this zone!");

        mSounds.erase(it);
    }

    void Zone::removeTrigger(Trigger* trigger)
    {
        if( !trigger )
            Throw(NullPointerException, "Trigger is NULL!");
        std::list<Trigger*>::iterator it = 
            std::find(mTriggers.begin(), mTriggers.end(), trigger);
        if( it == mTriggers.end() )
            Throw(IllegalArgumentException, "Could not find the Trigger in this zone!");

        mTriggers.erase(it);
    }

    void Zone::addEventSource(GameAreaEventSource *gam)
    {
        if( gam != NULL )
            mEventSources.insert(gam);
    }

    void Zone::removeEventSource(GameAreaEventSource *gam)
    {
        if( gam )
        {
            GameAreaEventSourceList::iterator it = mEventSources.find(gam);
            if( it == mEventSources.end() )
                LOG_ERROR(Logger::CORE, "Could not find the GameAreaEventSource in this zone!");

            mEventSources.erase(it);
        }
    }

	GameAreaEventSourceList& Zone::getEventSources()
	{
		return mEventSources;
	}
}
