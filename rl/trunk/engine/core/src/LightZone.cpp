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
#include "LightZone.h"

#include "ActorManager.h"
#include "GameAreaEvent.h"
#include "GameEventManager.h"

template<> rl::LightZoneManager* Ogre::Singleton<rl::LightZoneManager>::ms_Singleton = 0;

namespace rl {

	LightZone::LightZone()
		: GameAreaListener()
	{
	}

	void LightZone::areaLeft(GameAreaEvent *anEvent)
	{
		LightZoneManager::getSingleton().areaLeft(this);
	}

    void LightZone::areaEntered(GameAreaEvent *anEvent)
	{		
		LightZoneManager::getSingleton().areaEntered(this);
	}

	void LightZone::addLight(Actor* lo)
	{
		mLights.push_back(lo);
	}

	std::vector<Actor*> LightZone::getLights() const
	{
		std::vector<Actor*> rval(mLights);
		return rval;
	}

	LightZoneManager::LightZoneManager()
		: Ogre::Singleton<LightZoneManager>()
	{
		// the default light zone is active when no other lightzone is active
		mDefaultLightZone = new LightZone();
		mLightZones.push_back(mDefaultLightZone);
		mActiveLightZones.push_front(mDefaultLightZone); 
	}

	LightZoneManager::~LightZoneManager()
	{
		for (std::vector<LightZone*>::iterator it = mLightZones.begin(); it != mLightZones.end(); ++it)
		{
			delete *it;
		}
	}

	LightZone* LightZoneManager::getDefaultLightZone() const
	{
		return mDefaultLightZone;
	}

	LightZone* LightZoneManager::createLightZone(const Ogre::Vector3& position, const Ogre::Real radius, unsigned long queryflags)
	{
		LightZone* lz = new LightZone();
		mLightZones.push_back(lz);

		Actor* kugelDings = ActorManager::getSingleton().createEmptyActor("Light zone center");
		kugelDings->placeIntoScene(position);
		GameEventManager::getSingleton().addSphereAreaListener(kugelDings, radius, lz, queryflags);

		return lz;
	}

	void LightZoneManager::areaLeft(LightZone* zone)
	{
		mActiveLightZones.remove(zone);
		switchLights();
	}

	void LightZoneManager::areaEntered(LightZone* zone)
	{
		mActiveLightZones.push_front(zone);
		switchLights();
	}

	void LightZoneManager::switchLights()
	{
		LightZone* currentLightZone = mActiveLightZones.front();
		std::set<Actor*> activeLights;

		std::vector<Actor*> curLights = currentLightZone->getLights();
		for (std::vector<Actor*>::const_iterator it = curLights.begin(); it != curLights.end(); it++)
		{
			activeLights.insert(*it);
			(*it)->setVisible(true);
		}

		for (std::vector<LightZone*>::const_iterator it = mLightZones.begin(); it != mLightZones.end(); it++)
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
