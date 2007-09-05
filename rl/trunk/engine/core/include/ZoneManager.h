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
#include "CoreDefines.h"
#include "GameAreaListener.h"

namespace rl
{
    class Zone;
    class Sound;

    class _RlCoreExport ZoneManager : 
        public Ogre::Singleton<ZoneManager>,
        public GameAreaListener
	{
	public:
		ZoneManager();
		~ZoneManager();

        Zone* createZone(const Ogre::String& name);
        void destroyZone(const Ogre::String& name);
        /// Adds a new area to the zone
        void addAreaToZone(const Ogre::String& name, 
            Ogre::AxisAlignedBox aabb, GeometryType geom,
            Ogre::Vector3 position, Ogre::Vector3 offset, Ogre::Quaternion orientation,
            Ogre::Real transitionDistance,
            unsigned long queryflags);
        /// subtracts an area from the zone (it must ly in another area in this zone, else the beaviour is not defined)
        void subtractAreaFromZone(const Ogre::String& name, 
            Ogre::AxisAlignedBox aabb, GeometryType geom,
            Ogre::Vector3 position, Ogre::Vector3 offset, Ogre::Quaternion orientation,
            Ogre::Real transitionDistance,
            unsigned long queryflags);
        /// Adds a new mesh area to the zone
        void addMeshAreaToZone(const Ogre::String& name,
            const Ogre::String& meshname, GeometryType geom,
            Ogre::Vector3 position,
            Ogre::Vector3 scale, Ogre::Vector3 offset, Ogre::Quaternion orientation,
            Ogre::Real transitionDistance,
            unsigned long queryflags);
        /// subtracts a mesh-area from the zone (it must ly in another area in this zone, else the beaviour is not defined)
        void subtractMeshAreaFromZone(const Ogre::String& name,
            const Ogre::String& meshname, GeometryType geom,
            Ogre::Vector3 position,
            Ogre::Vector3 scale, Ogre::Vector3 offset, Ogre::Quaternion orientation,
            Ogre::Real transitionDistance,
            unsigned long queryflags);
		Zone* getDefaultZone();
		Zone* getZone(const Ogre::String& name);
        Zone* getZone(long id);

		void areaLeft(GameAreaEvent* gae);
	    void areaEntered(GameAreaEvent* gae);

        /// only needed if the lights/etc of the active zone changed
        void update();
		
	private:
		std::map<const Ogre::String, Zone*> mZones;
        std::map<long, Zone*> mZonesIdMap;
		std::list<Zone*> mActiveZones;
        typedef std::map<const Ogre::String, Sound*> SoundMap;
        SoundMap mActiveSounds;
		Zone* mDefaultZone;
        long mNextZoneId;

		void switchLights();
		void switchSounds();
        void switchEaxSettings();
	};
}

#endif // __ZoneManager_H__
