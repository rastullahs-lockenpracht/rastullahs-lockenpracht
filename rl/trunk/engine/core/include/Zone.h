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

	///@todo: Inheritance is wrong, Zone should be a GameAreaEventSource, ZoneManager a GameAreaListener
	class _RlCoreExport Zone : public GameAreaListener
	{
	public:
		Zone(Actor* actor);

		void addLight(Actor* light);
        void addSound(const Ogre::String& name);
		std::vector<Actor*> getLights() const;
        std::vector<Ogre::String> getSounds() const;

		Actor* getActor() const;

		virtual void areaLeft(GameAreaEvent *anEvent);
	    virtual void areaEntered(GameAreaEvent *anEvent);

	private:
		std::vector<Actor*> mLights;
		std::vector<Ogre::String> mSounds;
		Actor* mActor;
	};

}

#endif // __Zone_H__
