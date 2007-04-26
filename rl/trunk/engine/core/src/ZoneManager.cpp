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
		// the default light zone is active when no other Zone is active
		mDefaultZone = new Zone();
		mZones.push_back(mDefaultZone);
		mActiveZones.push_front(mDefaultZone); 
	}

	ZoneManager::~ZoneManager()
	{
		for (std::vector<Zone*>::iterator it = mZones.begin(); it != mZones.end(); ++it)
		{
			delete *it;
		}
	}

	Zone* ZoneManager::getDefaultZone() const
	{
		return mDefaultZone;
	}

    Zone* ZoneManager::createZone(const Ogre::String& name, const Ogre::Vector3& position, const Ogre::Real radius, unsigned long queryflags)
	{
		Zone* lz = new Zone();
		mZones.push_back(lz);

		Actor* kugelDings = ActorManager::getSingleton().createEmptyActor("Light zone center");
		kugelDings->placeIntoScene(position);
		GameEventManager::getSingleton().addSphereAreaListener(kugelDings, radius, lz, queryflags);

		return lz;
	}

	void ZoneManager::areaLeft(Zone* zone)
	{
		mActiveZones.remove(zone);
		switchLights();
	}

	void ZoneManager::areaEntered(Zone* zone)
	{
		mActiveZones.push_front(zone);
		switchLights();
	}

	void ZoneManager::switchLights()
	{
		Zone* currentZone = mActiveZones.front();
		std::set<Actor*> activeLights;

		std::vector<Actor*> curLights = currentZone->getLights();
		for (std::vector<Actor*>::const_iterator it = curLights.begin(); it != curLights.end(); it++)
		{
			activeLights.insert(*it);
			(*it)->setVisible(true);
		}

		for (std::vector<Zone*>::const_iterator it = mZones.begin(); it != mZones.end(); it++)
		{
			std::vector<Actor*> curLights = (*it)->getLights();
			for (std::vector<Actor*>::const_iterator it = curLights.begin(); it != curLights.end(); it++)
			{
				if (activeLights.find(*it) == activeLights.end())
				{
					(*it)->setVisible(false);
				}
			}
		}
	}
}
