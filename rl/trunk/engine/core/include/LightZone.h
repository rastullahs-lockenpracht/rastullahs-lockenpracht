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
#ifndef __LightZone_H__
#define __LightZone_H__

#include "CorePrerequisites.h"

#include "GameAreaListener.h"

namespace rl {

	class LightObject;

	///@todo: Inheritance is wrong, LightZone should be a GameAreaEventSource, LightZoneManager a GameAreaListener
	class _RlCoreExport LightZone : public GameAreaListener
	{
	public:
		LightZone();

		void addLight(Actor* light);
		std::vector<Actor*> getLights() const;

		virtual void areaLeft(GameAreaEvent *anEvent);
	    virtual void areaEntered(GameAreaEvent *anEvent);

	private:
		std::vector<Actor*> mLights;
	};

	class _RlCoreExport LightZoneManager : public Ogre::Singleton<LightZoneManager>
	{
	public:
		LightZoneManager();
		~LightZoneManager();

		LightZone* createLightZone(const Ogre::Vector3& position, const Ogre::Real radius, unsigned long queryflags);
		LightZone* getDefaultLightZone() const;

		void areaLeft(LightZone* zone);
	    void areaEntered(LightZone* zone);
		
	private:
		std::vector<LightZone*> mLightZones;
		std::list<LightZone*> mActiveLightZones;
		LightZone* mDefaultLightZone;

		void switchLights();
	};

}

#endif // __LightZone_H__
