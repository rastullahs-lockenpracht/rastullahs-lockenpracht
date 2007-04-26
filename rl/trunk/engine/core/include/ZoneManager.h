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
#ifndef __ZoneManager_H__
#define __ZoneManager_H__

#include "CorePrerequisites.h"

namespace rl
{
    class Zone;

    class _RlCoreExport ZoneManager : public Ogre::Singleton<ZoneManager>
	{
	public:
		ZoneManager();
		~ZoneManager();

        Zone* createZone(const Ogre::String& name, const Ogre::Vector3& position, const Ogre::Real radius, unsigned long queryflags);
		Zone* getDefaultZone() const;
		Zone* getZone(const Ogre::String& name) const;

		void areaLeft(Zone* zone);
	    void areaEntered(Zone* zone);
		
	private:
		std::map<const Ogre::String, Zone*> mZones;
		std::list<Zone*> mActiveZones;
		Zone* mDefaultZone;

		void switchLights();
		void switchSounds();
	};
}

#endif // __ZoneManager_H__
