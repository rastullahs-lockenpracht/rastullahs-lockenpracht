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
#ifndef __Zone_H__
#define __Zone_H__

#include "CorePrerequisites.h"

#include "GameAreaListener.h"

namespace rl {

	class Actor;
	class LightObject;
	class Trigger;

	///@todo: Inheritance is wrong, Zone should be a GameAreaEventSource, ZoneManager a GameAreaListener
	class _RlCoreExport Zone : public GameAreaListener
	{
	public:
		Zone(Actor* actor);

		void addLight(Actor* light);
        void addSound(const Ogre::String& name);
		void addTrigger(Trigger* trigger);
		std::list<Actor*> getLights() const;
        std::list<Ogre::String> getSounds() const;
		std::list<Trigger*> getTriggers() const;

		Actor* getActor() const;

		virtual void areaLeft(GameAreaEvent *anEvent);
	    virtual void areaEntered(GameAreaEvent *anEvent);

	private:
		std::list<Actor*> mLights;
		std::list<Ogre::String> mSounds;
		std::list<Trigger*> mTriggers;
		Actor* mActor;

		void deleteTriggers(const std::list<Trigger*>& toDelete);
	};

}

#endif // __Zone_H__
