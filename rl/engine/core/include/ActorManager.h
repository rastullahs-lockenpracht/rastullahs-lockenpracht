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

#ifndef __ActorManager_H__
#define __ActorManager_H__

#include <map>
#include <vector>

#include "CoreDefines.h"
#include "CorePrerequisites.h"
#include "LightObject.h"
#include "ParticleSystemObject.h"

namespace rl
{

    class World;
    class Actor;

    typedef std::map<const Ogre::String, Actor*> ActorPtrMap;
    typedef std::pair<const Ogre::String, Actor*> ActorPtrPair;

    class _RlCoreExport ActorManager : public Ogre::Singleton<ActorManager>
    {
    public:
        ActorManager();
        virtual ~ActorManager();

        Actor* getActor(const Ogre::String& name);

        Actor* createLightActor(const Ogre::String& name, rl::LightObject::LightTypes type);
        Actor* createSoundSampleActor(const Ogre::String& name, const Ogre::String& soundfile);
        Actor* createSoundStreamActor(const Ogre::String& name, const Ogre::String& soundfile);
        Actor* createListenerActor(const Ogre::String& name);
        Actor* createCameraActor(const Ogre::String& name);
        Actor* createEmptyActor(const Ogre::String& name);
        Actor* createMeshActor(const Ogre::String& name, const Ogre::String& meshname, GeometryType geomType = GT_NONE,
            Ogre::Real mass = 1.0, bool mergeable = false);
        Actor* createMeshActor(const Ogre::String& name, const Ogre::String& basemeshname, const MeshPartMap& meshname,
            GeometryType geomType = GT_NONE, Ogre::Real mass = 1.0);
        Actor* createParticleSystemActor(const Ogre::String& name, const Ogre::String& partname);

        Actor* createBoxPrimitiveActor(const Ogre::String& name, const Ogre::Vector3& minCorner,
            const Ogre::Vector3& maxCorner, const Ogre::String& materialName = "", Ogre::Real mass = 0.0);

        // void createDebugActors();
        void destroyActor(Actor* actor);

        void destroyAllActors();

        void setWorld(World* world);
        const World* const getWorld() const;

        // View Distance
        Ogre::Real getDefaultActorRenderingDistance() const;
        void setDefaultActorRenderingDistance(Ogre::Real dist);

    private:
        ActorPtrMap mActors;
        World* mWorld;
        Ogre::Real mActorRenderingDistance;

        void doDestroyActor(Actor* actor);
        Ogre::String nextUniqueName(const Ogre::String& basename);
    };
}
#endif
