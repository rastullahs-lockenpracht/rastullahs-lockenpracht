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
#include "stdinc.h" //precompiled header

#include "ZoneManager.h"

#include "Actor.h"
#include "ActorManager.h"
#include "CoreSubsystem.h"
#include "GameEventManager.h"
#include "SaveGameManager.h"
#include "ScriptWrapper.h"
#include "Sound.h"
#include "SoundDriver.h"
#include "SoundManager.h"
#include "Trigger.h"
#include "World.h"
#include "Zone.h"

using namespace Ogre;

template <> rl::ZoneManager* Ogre::Singleton<rl::ZoneManager>::msSingleton = 0;

namespace rl
{
    ZoneManager::ZoneManager()
        : Ogre::Singleton<ZoneManager>()
        , GameTask(false)
    {
        // the default zone is active when no other Zone is active
        mDefaultZone = new Zone(0, false);
        mNextZoneId = 1;
        mActiveZones.push_front(mDefaultZone);

        SaveGameManager::getSingleton().registerSaveGameData(this);
    }

    ZoneManager::~ZoneManager()
    {
        SaveGameManager::getSingleton().unregisterSaveGameData(this);

        GameEventManager::getSingleton().removeAreaListener(this);
        GameEventManager::getSingleton().removeQueuedDeletionSources();
        std::list<Zone*>::iterator it = mZonesToDelete.begin();
        for (; it != mZonesToDelete.end(); it++)
            doDestroyZone(*it);
        mZonesToDelete.clear();
        for (std::map<long, Zone*>::iterator it = mZonesIdMap.begin(); it != mZonesIdMap.end(); ++it)
        {
            Zone* curZone = (*it).second;
            doDestroyZone(curZone);
        }
        doDestroyZone(mDefaultZone);
    }

    Zone* ZoneManager::getDefaultZone()
    {
        return mDefaultZone;
    }

    Zone* ZoneManager::getZone(const Ogre::String& name)
    {
        ZoneMap::const_iterator it = mZones.find(name);
        if (it == mZones.end())
        {
            return NULL;
        }
        else
        {
            return (*it).second;
        }
    }

    Zone* ZoneManager::getZone(long id)
    {
        std::map<long, Zone*>::const_iterator it = mZonesIdMap.find(id);
        if (it == mZonesIdMap.end())
        {
            return NULL;
        }
        else
        {
            return (*it).second;
        }
    }

    Zone* ZoneManager::createZone(const Ogre::String& name, bool needsToBeSaved)
    {
        Zone* zone = new Zone(mNextZoneId, needsToBeSaved);
        mZonesIdMap[mNextZoneId] = zone;
        mZones[name] = zone;
        mNextZoneId++;

        return zone;
    }

    void ZoneManager::destroyZone(const Ogre::String& name)
    {
        Zone* zone = getZone(name);
        if (zone == NULL)
            return;

        // remove from active zones
        if (isZoneActive(zone))
            mActiveZones.remove(zone);

        ZoneMap::iterator it = mZones.find(name);
        if (it != mZones.end())
            mZones.erase(it);

        std::map<long, Zone*>::iterator it_ = mZonesIdMap.find(zone->getId());
        if (it_ != mZonesIdMap.end())
            mZonesIdMap.erase(it_);

        mZonesToDelete.push_back(zone);
    }

    void ZoneManager::destroyAllZones()
    {
        for (ZoneMap::iterator it = mZones.begin(); it != mZones.end();)
        {
            if (isZoneActive(it->second))
                mActiveZones.remove(it->second);
            destroyZone((it++)->first);
        }
        mZones.clear();
        mZonesIdMap.clear();

        std::list<Zone*>::iterator it = mZonesToDelete.begin();
        for (; it != mZonesToDelete.end(); it++)
            doDestroyZone(*it);
        mZonesToDelete.clear();
    }

    void ZoneManager::doDestroyZone(Zone* zone)
    {
        // destroy all areas
        GameAreaEventSourceList::iterator iter = zone->getEventSources().begin();
        for (; iter != zone->getEventSources().end(); iter++)
        {
            // if we have our own actors, remove them
            if ((*iter)->getActor())
                ActorManager::getSingleton().destroyActor(
                    (*iter)->getActor()); // this also removes all areaeventsources
            else
                GameEventManager::getSingleton().removeAreaEventSource((*iter));
        }

        // ask all triggers if they want to be deleted
        std::list<Trigger*> triggerList = zone->getTriggers();
        std::list<Trigger*>::iterator trig = triggerList.begin();
        for (; trig != triggerList.end(); trig++)
        {
            if ((*trig)->deleteIfZoneDestroyed())
                delete (*trig);
        }

        delete zone;
    }

    const Ogre::String& ZoneManager::getName() const
    {
        static Ogre::String NAME = "ZoneManager";

        return NAME;
    }

    void ZoneManager::run(Ogre::Real elapsedTime)
    {
        std::list<Zone*>::iterator it = mZonesToDelete.begin();
        for (; it != mZonesToDelete.end(); it++)
            doDestroyZone(*it);
        mZonesToDelete.clear();
    }

    bool ZoneManager::isZoneActive(const Zone* zone) const
    {
        if (zone == NULL)
            return false;

        std::list<Zone*>::const_iterator iter = std::find(mActiveZones.begin(), mActiveZones.end(), zone);
        return iter != mActiveZones.end();
    }

    bool ZoneManager::isInZone(Zone* zone, Actor* actor)
    {
        if (zone == NULL || actor == NULL)
            return false;

        if (zone == mDefaultZone)
            return true;

        int x = 0;
        GameAreaEventSourceList& gal = zone->getEventSources();
        GameAreaEventSourceList::iterator it = gal.begin();
        for (; it != gal.end(); it++)
        {
            // diese funktion hier notwendig um die mask zu ignorieren
            if ((*it)->getGameAreaType()->isInside(actor))
            {
                if ((*it)->getId() < 0)
                    x--;
                else
                    x++;
            }
        }

        return x > 0;
    }

    void ZoneManager::addAreaToZone(const Ogre::String& name, AxisAlignedBox aabb, GeometryType geom, Vector3 position,
        Vector3 offset, Quaternion orientation, Real transitionDistance, unsigned long queryflags)
    {
        Zone* zone = getZone(name);
        if (!zone)
            return;

        GameAreaEventSource* gam = GameEventManager::getSingleton().addAreaListener(
            NULL, aabb, geom, this, queryflags, position + offset, orientation, true);

        gam->getGameAreaType()->setTransitionDistance(transitionDistance);
        gam->setId(zone->getId());
        zone->addEventSource(gam);
        gam->mProperties.setProperty("subtract", Property(false));
        gam->mProperties.setProperty("aab_max", Property(aabb.getMaximum()));
        gam->mProperties.setProperty("aab_min", Property(aabb.getMinimum()));
        gam->mProperties.setProperty("type", Property(int(geom)));
        gam->mProperties.setProperty("position", Property(position));
        gam->mProperties.setProperty("offset", Property(offset));
        gam->mProperties.setProperty("orientation", Property(orientation));
        gam->mProperties.setProperty("transitionDistance", Property(transitionDistance));
        gam->mProperties.setProperty("queryflags", Property(int(queryflags)));
    }

    void ZoneManager::subtractAreaFromZone(const Ogre::String& name, AxisAlignedBox aabb, GeometryType geom,
        Vector3 position, Vector3 offset, Quaternion orientation, Real transitionDistance, unsigned long queryflags)
    {
        Zone* zone = getZone(name);
        if (!zone)
            return;

        GameAreaEventSource* gam = GameEventManager::getSingleton().addAreaListener(
            NULL, aabb, geom, this, queryflags, position + offset, orientation, true);

        gam->getGameAreaType()->setTransitionDistance(transitionDistance);
        gam->setId(-(zone->getId())); // a negative id indicates to subtract this area from the zone
        zone->addEventSource(gam);
        // set the properties for saving
        gam->mProperties.setProperty("subtract", Property(true));
        gam->mProperties.setProperty("aab_max", Property(aabb.getMaximum()));
        gam->mProperties.setProperty("aab_min", Property(aabb.getMinimum()));
        gam->mProperties.setProperty("type", Property(int(geom)));
        gam->mProperties.setProperty("position", Property(position));
        gam->mProperties.setProperty("offset", Property(offset));
        gam->mProperties.setProperty("orientation", Property(orientation));
        gam->mProperties.setProperty("transitionDistance", Property(transitionDistance));
        gam->mProperties.setProperty("queryflags", Property(int(queryflags)));
    }

    void ZoneManager::addMeshAreaToZone(const Ogre::String& name, const Ogre::String& meshname, GeometryType geom,
        Vector3 position, Vector3 scale, Vector3 offset, Quaternion orientation, Real transitionDistance,
        unsigned long queryflags)
    {
        Zone* zone = getZone(name);
        if (!zone)
            return;

        Entity* entity = CoreSubsystem::getSingletonPtr()->getWorld()->getSceneManager()->createEntity(
            "Zone_" + name + "_MeshArea_" + meshname, meshname);
        if (!entity)
            return;

        GameAreaEventSource* gam = GameEventManager::getSingleton().addMeshAreaListener(
            NULL, entity, geom, this, queryflags, position + offset, orientation, true);

        gam->getGameAreaType()->setTransitionDistance(transitionDistance);
        gam->setId(zone->getId());

        CoreSubsystem::getSingletonPtr()->getWorld()->getSceneManager()->destroyEntity(entity);
        zone->addEventSource(gam);
        // set the properties for saving
        gam->mProperties.setProperty("subtract", Property(false));
        gam->mProperties.setProperty("meshname", Property(meshname));
        gam->mProperties.setProperty("type", Property(int(geom)));
        gam->mProperties.setProperty("position", Property(position));
        gam->mProperties.setProperty("scale", Property(scale));
        gam->mProperties.setProperty("offset", Property(offset));
        gam->mProperties.setProperty("orientation", Property(orientation));
        gam->mProperties.setProperty("transitionDistance", Property(transitionDistance));
        gam->mProperties.setProperty("queryflags", Property(int(queryflags)));
    }

    void ZoneManager::subtractMeshAreaFromZone(const Ogre::String& name, const Ogre::String& meshname,
        GeometryType geom, Vector3 position, Vector3 scale, Vector3 offset, Quaternion orientation,
        Real transitionDistance, unsigned long queryflags)
    {
        Zone* zone = getZone(name);
        if (!zone)
            return;

        Entity* entity = CoreSubsystem::getSingletonPtr()->getWorld()->getSceneManager()->createEntity(
            "Zone_" + name + "_MeshArea_" + meshname, meshname);
        if (!entity)
            return;

        GameAreaEventSource* gam = GameEventManager::getSingleton().addMeshAreaListener(
            NULL, entity, geom, this, queryflags, position + offset, orientation, true);

        gam->getGameAreaType()->setTransitionDistance(transitionDistance);
        gam->setId(-(zone->getId())); // a negative id indicates to subtract this area from the zone

        CoreSubsystem::getSingletonPtr()->getWorld()->getSceneManager()->destroyEntity(entity);
        zone->addEventSource(gam);
        // set the properties for saving
        gam->mProperties.setProperty("subtract", Property(true));
        gam->mProperties.setProperty("meshname", Property(meshname));
        gam->mProperties.setProperty("type", Property(int(geom)));
        gam->mProperties.setProperty("position", Property(position));
        gam->mProperties.setProperty("scale", Property(scale));
        gam->mProperties.setProperty("offset", Property(offset));
        gam->mProperties.setProperty("orientation", Property(orientation));
        gam->mProperties.setProperty("transitionDistance", Property(transitionDistance));
        gam->mProperties.setProperty("queryflags", Property(int(queryflags)));
    }

    void ZoneManager::areaLeft(GameAreaEvent* gae)
    {
        long id = gae->getSource()->getId();
        if (id != 0)
        {
            if (id > 0)
            {
                Zone* zone = getZone(id);
                if (zone)
                {
                    zone->personLeft();
                    if (!zone->isActive())
                        zoneLeft(zone);
                }
            }
            else
            {
                Zone* zone = getZone(-id); // means we have to subtract this area from the zone
                if (zone)
                {
                    if (!zone->isActive())
                        zoneEntered(zone);
                    zone->personEntered();
                }
            }

            update();
        }
    }

    void ZoneManager::areaEntered(GameAreaEvent* gae)
    {
        long id = gae->getSource()->getId();
        if (id != 0)
        {
            if (id > 0)
            {
                Zone* zone = getZone(id);
                if (zone)
                {
                    if (!zone->isActive())
                        zoneEntered(zone);
                    zone->personEntered();
                }
            }
            else
            {
                Zone* zone = getZone(-id); // means we have to subtract this area from the zone
                if (zone)
                {
                    zone->personLeft();
                    if (!zone->isActive())
                        zoneLeft(zone);
                }
            }

            update();
        }
    }

    void ZoneManager::zoneEntered(Zone* zone)
    {
        if (isZoneActive(zone))
            return;
        // perhaps the trigger destroys the zone, so we should put the zone in the list, before the triggers does so
        mActiveZones.push_front(zone);

        std::list<Trigger*> triggerList = zone->getTriggers();
        std::list<Trigger*>::iterator iter = triggerList.begin();

        for (; iter != triggerList.end(); iter++)
        {
            if ((*iter)->activate())
            {
                zone->removeTrigger(*iter);
                delete (*iter);
            }
        }
    }

    void ZoneManager::zoneLeft(Zone* zone)
    {
        if (!isZoneActive(zone))
            return;
        // perhaps the trigger destroys the zone, so we should put the zone in the list, before the triggers does so
        mActiveZones.remove(zone);

        std::list<Trigger*> triggerList = zone->getTriggers();
        std::list<Trigger*>::iterator iter = triggerList.begin();

        for (; iter != triggerList.end(); iter++)
        {
            if ((*iter)->deactivate())
            {
                zone->removeTrigger(*iter);
                delete (*iter);
            }
        }
    }

    void ZoneManager::update()
    {
        switchLights();
        switchEaxSettings();
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

        for (ZoneMap::const_iterator itZones = mZones.begin(); itZones != mZones.end(); itZones++)
        {
            std::list<Actor*> curLights = (*itZones).second->getLights();
            for (std::list<Actor*>::const_iterator itLights = curLights.begin(); itLights != curLights.end();
                 itLights++)
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
        SoundMap newActiveSounds;

        // create all new sounds, or take existing ones
        std::list<Ogre::String> curSounds = currentZone->getSounds();
        for (std::list<Ogre::String>::const_iterator it = curSounds.begin(); it != curSounds.end(); it++)
        {
            SoundMap::iterator it_ = mActiveSounds.find(*it);
            if (it_ != mActiveSounds.end())
            {
                // this sound already exists, only copy and don't destroy later
                newActiveSounds.insert(*it_);
                mActiveSounds.erase(it_); // remember, it_ is now invalid
            }
            else
            {
                // we need to create a new sound
                Sound* sound = SoundManager::getSingleton().createSound(*it, ST_STREAM);
                sound->setLooping(true);
                sound->play(false);
                newActiveSounds.insert(make_pair(*it, sound));
            }
        }

        // destroy old sounds
        for (SoundMap::iterator it = mActiveSounds.begin(); it != mActiveSounds.end(); it++)
        {
            SoundManager::getSingleton().destroySound(it->second);
        }

        // copy new active sounds
        mActiveSounds = newActiveSounds;
    }

    void ZoneManager::switchEaxSettings()
    {
        if (mActiveZones.front()->getEaxPreset() != "")
        {
            SoundManager::getSingleton().getActiveDriver()->setEaxPreset(mActiveZones.front()->getEaxPreset());
        }
        else
        {
            SoundManager::getSingleton().getActiveDriver()->setEaxPreset("Off");
        }
    }

    // -------------- code for saving/loading  from the SaveGameFile --------------------

    CeGuiString ZoneManager::getXmlNodeIdentifier() const
    {
        return "zonemanager";
    }

    void ZoneManager::writeData(SaveGameFileWriter* writer)
    {
        tinyxml2::XMLElement* zoneManagerNode
            = writer->appendChildElement(writer->getDocument()->RootElement(), getXmlNodeIdentifier().c_str());

        // look at all zones if they need to be saved
        for (ZoneMap::iterator zone = mZones.begin(); zone != mZones.end(); zone++)
        {
            // does this zone wants to be saved
            if (zone->second->needsToBeSaved())
            {
                tinyxml2::XMLElement* zoneNode = writer->appendChildElement(zoneManagerNode, "zone");
                writer->setAttributeValueAsStdString(zoneNode, "name", zone->first);

                // save all areas of the zone
                GameAreaEventSourceList::iterator gam;
                ;
                for (gam = zone->second->getEventSources().begin(); gam != zone->second->getEventSources().end(); gam++)
                {
                    tinyxml2::XMLElement* areaNode = writer->appendChildElement(zoneNode, "area");
                    writer->writeEachPropertyToElem(areaNode, (*gam)->mProperties.toPropertyMap());
                }

                // save all sounds and lights
                PropertyRecord propertyRecord;
                std::list<Actor*> lightList = zone->second->getLights();
                std::list<Ogre::String> soundList = zone->second->getSounds();
                for (std::list<Actor*>::iterator light = lightList.begin(); light != lightList.end(); light++)
                    propertyRecord.setProperty("light", Property((*light)->getName()));
                for (std::list<Ogre::String>::iterator sound = soundList.begin(); sound != soundList.end(); sound++)
                    propertyRecord.setProperty("sound", Property(*sound));

                writer->writeEachPropertyToElem(zoneNode, propertyRecord.toPropertyMap());
            }
        }
    }

    void ZoneManager::readData(SaveGameFileReader* reader)
    {
        // delete all zones, that say they are saveable
        for (ZoneMap::iterator zone = mZones.begin(); zone != mZones.end(); zone++)
        {
            if (zone->second->needsToBeSaved())
            {
                destroyZone(zone->first);
            }
        }

        // load zones
        // initialize xmlreader
        XmlElementList rootNodeList
            = reader->getElementsByTagName(reader->getDocument(), getXmlNodeIdentifier().c_str());

        if (!rootNodeList.empty())
        {
            XmlElementList xmlZones = reader->getElementsByTagName(rootNodeList[0], "zone");

            for (XmlElementList::iterator it = xmlZones.begin(); it != xmlZones.end(); ++it)
            {
                const tinyxml2::XMLElement* xmlZone = *it;
                Ogre::String name = reader->getAttributeValueAsStdString(xmlZone, "name");
                Zone* zone = createZone(name, true);

                XmlElementList xmlAreas = reader->getElementsByTagName(xmlZone, "area");
                for (XmlElementList::iterator itA = xmlAreas.begin(); itA != xmlAreas.end(); ++itA)
                {
                    const tinyxml2::XMLElement* xmlArea = *itA;
                    PropertyRecordPtr properties = reader->getPropertiesAsRecord(xmlArea);
                    parseAreaProperties(name, properties);
                }
            }
        }
    }

    void ZoneManager::parseAreaProperties(const Ogre::String& name, const PropertyRecordPtr properties)
    {
        Ogre::AxisAlignedBox aabb;
        aabb.setMaximum(Vector3::ZERO);
        aabb.setMinimum(Vector3::ZERO);
        GeometryType geom = GT_NONE;
        Ogre::Vector3 position = Vector3::ZERO;
        Ogre::Vector3 offset = Vector3::ZERO;
        Ogre::Vector3 scale = Vector3::UNIT_SCALE;
        Ogre::Quaternion orientation = Quaternion::IDENTITY;
        Ogre::Real transitionDistance = 0.2;
        unsigned long queryflags = 0;
        Ogre::String meshname = "";
        bool subtract = false;
        bool mesh = false;

        for (PropertyRecord::PropertyRecordMap::const_iterator iter = properties->begin(); iter != properties->end();
             iter++)
        {
            if (iter->first == "subtract")
                subtract = iter->second.toBool();
            else if (iter->first == "meshname")
            {
                meshname = iter->second.toString().c_str();
                mesh = true;
            }
            else if (iter->first == "type")
                geom = GeometryType(iter->second.toInt());
            else if (iter->first == "position")
                position = iter->second.toVector3();
            else if (iter->first == "scale")
                scale = iter->second.toVector3();
            else if (iter->first == "offset")
                offset = iter->second.toVector3();
            else if (iter->first == "orientation")
                orientation = iter->second.toQuaternion();
            else if (iter->first == "transitionDistance")
                transitionDistance = iter->second.toReal();
            else if (iter->first == "queryflags")
                queryflags = iter->second.toInt();
            else if (iter->first == "aab_min")
                aabb.setMinimum(iter->second.toVector3());
            else if (iter->first == "aab_max")
                aabb.setMaximum(iter->second.toVector3());
            else
            {
                LOG_ERROR(Logger::CORE, "ZoneManager: Unknown area property '" + iter->first + "' !");
            }
        }

        if (mesh)
        {
            if (subtract)
                subtractMeshAreaFromZone(
                    name, meshname, geom, position, scale, offset, orientation, transitionDistance, queryflags);
            else
                addMeshAreaToZone(
                    name, meshname, geom, position, scale, offset, orientation, transitionDistance, queryflags);
        }
        else
        {
            if (subtract)
                subtractAreaFromZone(name, aabb, geom, position, offset, orientation, transitionDistance, queryflags);
            else
                addAreaToZone(name, aabb, geom, position, offset, orientation, transitionDistance, queryflags);
        }
    }

    int ZoneManager::getPriority() const
    {
        return 30; // must be loaded before triggers!
    }
}
