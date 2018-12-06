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
#ifndef __Zone_H__
#define __Zone_H__

#include "CorePrerequisites.h"
#include "GameEventManager.h" /* wegen GameAreaEventSourceList */

namespace rl
{

    class Actor;
    class LightObject;
    class Trigger;
    class GameAreaEventSource;
    class ZoneManager;

    /// an abstraction of an zone, consisting of all GameAreaEventSources with the associated id
    class _RlCoreExport Zone
    {
    public:
        virtual ~Zone();

        void addLight(Actor* light);
        void addSound(const Ogre::String& name);
        void addTrigger(Trigger* trigger);
        /**
         * sets an aex-preset
         * @param name The name of the preset (possible names can be found in the wiki),
         * an empty String ("") signifies that no change of the eax-preset is needed
         */
        void setEaxPreset(const Ogre::String& name);
        std::list<Actor*> getLights() const;
        std::list<Ogre::String> getSounds() const;
        std::list<Trigger*> getTriggers() const;
        const Ogre::String& getEaxPreset() const;
        void removeLight(Actor* light);
        void removeSound(const Ogre::String& name);
        void removeTrigger(Trigger* trigger);
        long getId() const
        {
            return mId;
        }

        bool needsToBeSaved() const
        {
            return mNeedsToBeSaved;
        }

    protected:
        friend class ZoneManager;
        Zone(long id, bool needsToBeSaved);
        void addEventSource(GameAreaEventSource* gam);
        void removeEventSource(GameAreaEventSource* gam);
        GameAreaEventSourceList& getEventSources();
        void personEntered();
        void personLeft();
        bool
        isActive() const; // only for the ZoneManager, in order to ask if a zone is active use ZoneManager::isZoneActive
    private:
        bool mNeedsToBeSaved;
        long mId;
        Zone();
        std::list<Actor*> mLights;
        std::list<Ogre::String> mSounds;
        std::list<Trigger*> mTriggers;
        GameAreaEventSourceList mEventSources;
        Ogre::String mEaxPreset;
        int mPersonsInside;
    };
}

#endif // __Zone_H__
