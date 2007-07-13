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

	Zone::Zone(Actor* actor)
	 : GameAreaListener(),
	   mActor(actor)
	{
	}

	Actor* Zone::getActor() const
	{
		return mActor;
	}

	void Zone::areaLeft(GameAreaEvent *anEvent)
	{
		ZoneManager::getSingleton().areaLeft(this);

		std::list<Trigger*> toDelete;
		for (std::list<Trigger*>::iterator it = mTriggers.begin(); it != mTriggers.end(); ++it)
		{
			bool remove = (*it)->deactivate();
			if (remove)
			{
				toDelete.push_back(*it);
			}
		}

		deleteTriggers(toDelete);
	}

    void Zone::areaEntered(GameAreaEvent *anEvent)
	{
		ZoneManager::getSingleton().areaEntered(this);

		std::list<Trigger*> toDelete;
		for (std::list<Trigger*>::iterator it = mTriggers.begin(); it != mTriggers.end(); ++it)
		{
			bool remove = (*it)->activate();
			if (remove)
			{
				toDelete.push_back(*it);
			}
		}

		deleteTriggers(toDelete);
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


	void Zone::deleteTriggers(const std::list<Trigger*>& toDelete)
	{
		for (std::list<Trigger*>::const_iterator it = toDelete.begin();
			it != toDelete.end(); ++it)
		{
			const Trigger* cur = *it;

			std::list<Trigger*>::iterator trigIt =
				std::find(mTriggers.begin(), mTriggers.end(), cur);
			mTriggers.erase(trigIt);
		}
	}
}
