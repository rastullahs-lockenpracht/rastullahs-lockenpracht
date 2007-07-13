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

#include "ZoneManager.h"

#include "Actor.h"
#include "ActorManager.h"
#include "GameEventManager.h"
#include "Zone.h"

template<> rl::ZoneManager* Ogre::Singleton<rl::ZoneManager>::ms_Singleton = 0;

namespace rl
{
	ZoneManager::ZoneManager()
		: Ogre::Singleton<ZoneManager>()
	{
		// the default zone is active when no other Zone is active
		mDefaultZone = new Zone(NULL);
		mActiveZones.push_front(mDefaultZone);
	}

	ZoneManager::~ZoneManager()
	{
		for (std::map<const Ogre::String, Zone*>::iterator it = mZones.begin(); it != mZones.end(); ++it)
		{
			Zone* curZone = (*it).second;
			GameEventManager::getSingleton().removeAreaListener(curZone);
		}
		GameEventManager::getSingleton().removeQueuedDeletionSources();
		delete mDefaultZone;
	}

	Zone* ZoneManager::getDefaultZone() const
	{
		return mDefaultZone;
	}

	Zone* ZoneManager::getZone(const Ogre::String& name) const
	{
		std::map<const Ogre::String, Zone*>::const_iterator it = mZones.find(name);
		if (it == mZones.end())
		{
			LOG_ERROR(Logger::CORE, "Zone '"+name+"' not found.");
			return NULL;
		}
		else
		{
			return (*it).second;
		}
	}

    Zone* ZoneManager::createZone(const Ogre::String& name, const Ogre::Vector3& position, const Ogre::Real radius, unsigned long queryflags)
	{

		Actor* kugelDings = ActorManager::getSingleton().createEmptyActor("Light zone center");
		kugelDings->placeIntoScene(position);
		Zone* lz = new Zone(kugelDings);
		mZones[name] = lz;
		GameEventManager::getSingleton().addSphereAreaListener(kugelDings, radius, lz, queryflags);

		return lz;
	}

	void ZoneManager::areaLeft(Zone* zone)
	{
		mActiveZones.remove(zone);
		switchLights();
		switchSounds();
	}

	void ZoneManager::areaEntered(Zone* zone)
	{
		mActiveZones.push_front(zone);
		switchLights();
		switchSounds();
	}

	void ZoneManager::switchLights()
	{
		Zone* currentZone = mActiveZones.front();
		std::set<Actor*> activeLights;

		std::list<Actor*> curLights = currentZone->getLights();
		for (std::list<Actor*>::const_iterator it = curLights.begin(); it != curLights.end(); it++)
		{
			activeLights.insert(*it);
			(*it)->setVisible(true);
		}

		for (std::map<const Ogre::String, Zone*>::const_iterator itZones = mZones.begin(); itZones != mZones.end(); itZones++)
		{
			std::list<Actor*> curLights = (*itZones).second->getLights();
			for (std::list<Actor*>::const_iterator itLights = curLights.begin(); itLights != curLights.end(); itLights++)
			{
				if (activeLights.find(*itLights) == activeLights.end())
				{
					(*itLights)->setVisible(false);
				}
			}
		}

		std::list<Actor*> defLights = mDefaultZone->getLights();
		for (std::list<Actor*>::const_iterator itLights = defLights.begin(); itLights != defLights.end(); itLights++)
		{
			if (activeLights.find(*itLights) == activeLights.end())
			{
				(*itLights)->setVisible(false);
			}
		}
	}

	void ZoneManager::switchSounds()
	{
		Zone* currentZone = mActiveZones.front();
		std::set<Ogre::String> activeSounds;

		std::list<Ogre::String> curSounds = currentZone->getSounds();
		for (std::list<Ogre::String>::const_iterator it = curSounds.begin(); it != curSounds.end(); it++)
		{
			activeSounds.insert(*it);
			///@todo switch sound on
		}

		for (std::map<const Ogre::String, Zone*>::const_iterator itZones = mZones.begin(); itZones != mZones.end(); itZones++)
		{
			std::list<Ogre::String> curSounds = (*itZones).second->getSounds();
			for (std::list<Ogre::String>::const_iterator itSounds = curSounds.begin(); itSounds != curSounds.end(); itSounds++)
			{
				if (activeSounds.find(*itSounds) == activeSounds.end())
				{
					///@todo switch sound off
				}
			}
		}

		std::list<Ogre::String> defSounds = mDefaultZone->getSounds();
		for (std::list<Ogre::String>::const_iterator itSounds = defSounds.begin(); itSounds != defSounds.end(); itSounds++)
		{
			if (activeSounds.find(*itSounds) == activeSounds.end())
			{
				///@todo switch sound off
			}
		}
	}
}
