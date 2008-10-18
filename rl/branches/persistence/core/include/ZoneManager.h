/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "GameTask.h"
#include "Properties.h"
#include "SaveGameData.h"

namespace rl
{
    class Zone;
    class Sound;

    class _RlCoreExport ZoneManager : 
        public Ogre::Singleton<ZoneManager>,
        public GameAreaListener,
        public GameTask, // for deferred deletion of zones
        public SaveGameData // for saving/loading zones
	{
	public:
		ZoneManager();
		~ZoneManager();

        Zone* createZone(const Ogre::String& name, bool needsToBeSaved);
        /// only marks the zone for deferred deletion
        void destroyZone(const Ogre::String& name);
        void destroyAllZones();
        /// Adds a new area to the zone
        void addAreaToZone(const Ogre::String& name, 
            Ogre::AxisAlignedBox aabb, GeometryType geom,
            Ogre::Vector3 position, Ogre::Vector3 offset, Ogre::Quaternion orientation,
            Ogre::Real transitionDistance,
            unsigned long queryflags);
        void addAreaToZone(const Ogre::String& name, 
            Ogre::Vector3 size, GeometryType geom,
            Ogre::Vector3 position, Ogre::Vector3 offset, Ogre::Quaternion orientation,
            Ogre::Real transitionDistance,
            unsigned long queryflags)
        {
            addAreaToZone(
                name, Ogre::AxisAlignedBox(- 0.5 * size, 0.5 *size ), geom, position, offset, orientation,
                transitionDistance, queryflags);
        }
        /// subtracts an area from the zone (it must ly in another area in this zone, else the beaviour is not defined)
        void subtractAreaFromZone(const Ogre::String& name, 
            Ogre::AxisAlignedBox aabb, GeometryType geom,
            Ogre::Vector3 position, Ogre::Vector3 offset, Ogre::Quaternion orientation,
            Ogre::Real transitionDistance,
            unsigned long queryflags);
        void subtractAreaFromZone(const Ogre::String& name, 
            Ogre::Vector3 size, GeometryType geom,
            Ogre::Vector3 position, Ogre::Vector3 offset, Ogre::Quaternion orientation,
            Ogre::Real transitionDistance,
            unsigned long queryflags)
        {
            subtractAreaFromZone(
                name, Ogre::AxisAlignedBox(- 0.5 * size, 0.5 *size ), geom, position, offset, orientation,
                transitionDistance, queryflags);
        }
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
        Zone* getZone(const CeGuiString &id);

        /// asks wether this zone is currently activated
        bool isZoneActive(const Zone* zone) const;

        /*
         * asks if the actor is currently in the specified Zone (expensive function!)
         * the querymask of the actor doesn't need to fit the mask of the zone.
         * this function probably isn't very accurate, it may only use the distance!
        */
        bool isInZone(Zone* zone, Actor *actor);

		void areaLeft(GameAreaEvent* gae);
	    void areaEntered(GameAreaEvent* gae);

        /// only needed if the lights/etc of the active zone changed
        void update();

        /// inherited from gametask, deletes zones marked for deletion
        void run(Ogre::Real elapsedTime);
        /// inherited from gametask
        const Ogre::String& getName() const;

        typedef std::map<const Ogre::String, Zone*> ZoneMap;
        const ZoneMap &getAllZones() const {return mZones;}


        /// Override from SaveGameData
        /// Manages saving and loading from the SaveGameFile

        virtual CeGuiString getXmlNodeIdentifier() const;
        virtual void writeData(SaveGameFileWriter* writer);
        virtual void readData(SaveGameFileReader* reader);
        virtual int getPriority() const;  // zones must be loaded before triggers!
	private:
		ZoneMap mZones;
        std::map<CeGuiString, Zone*> mZonesIdMap;
		std::list<Zone*> mActiveZones;
        typedef std::map<const Ogre::String, Sound*> SoundMap;
        SoundMap mActiveSounds;
		Zone* mDefaultZone;
        long mNextZoneId;
        std::list<Zone*> mZonesToDelete;

		void switchLights();
		void switchSounds();
        void switchEaxSettings();
        void zoneEntered(Zone * zone);
        void zoneLeft(Zone * zone);
        
        void doDestroyZone(Zone *zone);
        void parseAreaProperties(const Ogre::String& zoneName, const PropertyRecordPtr properties); // adds an area created from properties to the zone
	};
}

#endif // __ZoneManager_H__
