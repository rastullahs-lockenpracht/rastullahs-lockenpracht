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

#include "ActorManager.h"

#include "CoreSubsystem.h"

#include "Actor.h"
#include "BoxPrimitive.h"
#include "CameraObject.h"
#include "LightObject.h"
#include "ListenerMovable.h"
#include "ListenerObject.h"
#include "MergeableMeshObject.h"
#include "MeshObject.h"
#include "MovableText.h"
#include "PhysicalThing.h"
#include "ScriptWrapper.h"
#include "Sound.h"
#include "SoundDriver.h"
#include "SoundManager.h"
#include "SoundObject.h"
#include "World.h"

using namespace Ogre;

template <> rl::ActorManager* Singleton<rl::ActorManager>::msSingleton = 0;

namespace rl
{

    ActorManager::ActorManager()
        : mActors()
        , mWorld(CoreSubsystem::getSingleton().getWorld())
        , mActorRenderingDistance(100.0f)
    {
    }

    ActorManager::~ActorManager()
    {
        destroyAllActors();
    }

    void ActorManager::setWorld(World* world)
    {
        mWorld = world;
    }

    const World* const ActorManager::getWorld() const
    {
        return mWorld;
    }

    Ogre::Real ActorManager::getDefaultActorRenderingDistance() const
    {
        return mActorRenderingDistance;
    }

    void ActorManager::setDefaultActorRenderingDistance(Ogre::Real dist)
    {
        for (ActorPtrMap::iterator it = mActors.begin(); it != mActors.end();)
        {
            Actor* actor = it->second;

            // Nur die Actoren veraendern, deren Distanz nicht geaendert wurde
            if (actor->getRenderingDistance() == mActorRenderingDistance)
            {
                actor->setRenderingDistance(dist);
            }

            ++it;
        }

        mActorRenderingDistance = dist;
    }

    Actor* ActorManager::getActor(const String& name)
    {
        ActorPtrMap::const_iterator pActorIter = mActors.find(name);

        if (pActorIter != mActors.end())
            return pActorIter->second;

        return 0;
    }

    void ActorManager::destroyActor(Actor* actor)
    {
        if (actor == NULL)
        {
            Throw(NullPointerException, "Argument actor darf nicht null sein.");
        }

        ActorPtrMap::iterator it = mActors.find(actor->getName());
        if (it == mActors.end())
        {
            Throw(IllegalArgumentException, "Argument actor wird nicht vom ActorManager verwaltet.");
        }
        mActors.erase(it);
        LOG_MESSAGE("ActorManager", "Delete actor '" + actor->getName() + "'");
        // actor->detachAllChildren(); //just avoiding a crash. is there a better resolution?
        // should not be needed, is also done in the destructor of the actor

        doDestroyActor(actor);
    }

    void ActorManager::doDestroyActor(Actor* actor)
    {
        RlAssert1(actor != NULL);

        ActorControlledObject* actObj = actor->getControlledObject();
        PhysicalThing* physThing = actor->getPhysicalThing();

        if (actor == SoundManager::getSingleton().getListenerActor())
        {
            SoundManager::getSingleton()._clearListenerActor();
        }

        ScriptWrapper::getSingleton().deleted(actor);
        delete actor;

        if (actObj)
        {
            ScriptWrapper::getSingleton().deleted(actObj);
            delete actObj;
        }
        if (physThing)
        {
            PhysicsManager::getSingleton().removeAndDestroyPhysicalThing(physThing);
        }
    }

    void ActorManager::destroyAllActors()
    {
        for (ActorPtrMap::iterator it = mActors.begin(); it != mActors.end();)
        {
            Actor* actor = it->second;
            mActors.erase(it++);
            doDestroyActor(actor);
        }
    }

    Actor* ActorManager::createLightActor(const String& name, rl::LightObject::LightTypes type)
    {
        const String& uniquename = nextUniqueName(name);

        Actor* actor = 0;
        try
        {
            ///@todo Typen in Einklang bringen
            LightObject* lo = new LightObject(uniquename, type);

            actor = new Actor(uniquename, lo);
            mActors.insert(ActorPtrPair(uniquename, actor));
        }
        catch (Ogre::Exception& e)
        {
            LOG_ERROR(Logger::CORE,
                "ActorManager - Das Licht '" + uniquename
                    + "' konnte nicht erstellt werden. Grund: " + e.getFullDescription());
        }
        catch (rl::Exception& e)
        {
            LOG_ERROR(Logger::CORE,
                "ActorManager - Das Licht '" + uniquename + "' konnte nicht erstellt werden. Grund: " + e.getMessage());
        }

        return actor;
    }

    Actor* ActorManager::createSoundSampleActor(const String& name, const String& soundfile)
    {
        const String& uniquename = nextUniqueName(name);

        Actor* actor = 0;
        try
        {
            Sound* sm = SoundManager::getSingleton().getActiveDriver()->createSound(soundfile, ST_SAMPLE);
            SoundObject* so = new SoundObject(sm, soundfile);
            actor = new Actor(uniquename, so);
            mActors.insert(ActorPtrPair(uniquename, actor));
        }
        catch (Ogre::Exception& e)
        {
            LOG_ERROR(Logger::CORE,
                "ActorManager - Der Sound '" + uniquename
                    + "' konnte nicht erstellt werden. Grund: " + e.getFullDescription());
        }
        catch (rl::Exception& e)
        {
            LOG_ERROR(Logger::CORE,
                "ActorManager - Der Sound '" + uniquename + "' konnte nicht erstellt werden. Grund: " + e.getMessage());
        }

        return actor;
    }

    Actor* ActorManager::createSoundStreamActor(const String& name, const String& soundfile)
    {
        const String& uniquename = nextUniqueName(name);

        Actor* actor = 0;
        try
        {
            Sound* sm = SoundManager::getSingleton().getActiveDriver()->createSound(soundfile, ST_STREAM);
            SoundObject* so = new SoundObject(sm, soundfile);

            actor = new Actor(uniquename, so);
            mActors.insert(ActorPtrPair(uniquename, actor));
        }
        catch (Ogre::Exception& e)
        {
            LOG_ERROR(Logger::CORE,
                "ActorManager - Der Sound '" + uniquename
                    + "' konnte nicht erstellt werden. Grund: " + e.getFullDescription());
        }
        catch (rl::Exception& e)
        {
            LOG_ERROR(Logger::CORE,
                "ActorManager - Der Sound '" + uniquename + "' konnte nicht erstellt werden. Grund: " + e.getMessage());
        }

        return actor;
    }

    Actor* ActorManager::createListenerActor(const String& name)
    {
        const String& uniquename = nextUniqueName(name);

        Actor* actor = 0;
        try
        {
            ListenerMovable* lm = SoundManager::getSingleton().getActiveDriver()->createListener(name);
            ListenerObject* lo = new ListenerObject(lm);

            actor = new Actor(uniquename, lo);
            mActors.insert(ActorPtrPair(uniquename, actor));
        }
        catch (Ogre::Exception& e)
        {
            LOG_ERROR(Logger::CORE,
                "ActorManager - Der Listener '" + uniquename
                    + "' konnte nicht erstellt werden. Grund: " + e.getFullDescription());
        }
        catch (rl::Exception& e)
        {
            LOG_ERROR(Logger::CORE,
                "ActorManager - Der Listener '" + uniquename
                    + "' konnte nicht erstellt werden. Grund: " + e.getMessage());
        }

        return actor;
    }

    Actor* ActorManager::createEmptyActor(const String& name)
    {
        const String& uniquename = nextUniqueName(name);

        Actor* actor = new Actor(uniquename);
        mActors.insert(ActorPtrPair(uniquename, actor));

        return actor;
    }

    Actor* ActorManager::createCameraActor(const String& name)
    {
        const String& uniquename = nextUniqueName(name);
        Actor* actor = 0;
        try
        {
            CameraObject* co = new CameraObject(uniquename);
            // the mass should be as small as possible, so the camera doesn't interact with other bodies,
            // but setting the mass to 0 means, that the body is static!
            PhysicalThing* pt = PhysicsManager::getSingleton().createPhysicalThing(GT_SPHERE, co, 0.001f, true);
            PhysicsManager::getSingleton().createPhysicsProxy(pt);
            pt->_getBody()->setMaterialGroupID(PhysicsManager::getSingleton().createMaterialID("camera"));
            actor = new Actor(uniquename, co, pt);

            mActors.insert(ActorPtrPair(uniquename, actor));
        }
        catch (Ogre::Exception& e)
        {
            LOG_ERROR(Logger::CORE,
                "ActorManager - Die Kamera '" + name + "' fuer den Aktor '" + uniquename
                    + "' konnte nicht erstellt werden. Grund: " + e.getFullDescription());
        }
        catch (rl::Exception& e)
        {
            LOG_ERROR(Logger::CORE,
                "ActorManager - Die Kamera '" + name + "' fuer den Aktor '" + uniquename
                    + "' konnte nicht erstellt werden. Grund: " + e.getMessage());
        }

        return actor;
    }

    Actor* ActorManager::createMeshActor(const String& name, const Ogre::String& basemeshname,
        const MeshPartMap& meshparts, GeometryType geomType, Ogre::Real mass)
    {
        if (meshparts.empty())
        {
            return NULL;
        }

        const String& uniquename = nextUniqueName(name);
        Actor* actor = NULL;

        try
        {

            MergeableMeshObject* mmo = new MergeableMeshObject(uniquename, basemeshname);

            for (MeshPartMap::const_iterator partIt = meshparts.begin(); partIt != meshparts.end(); ++partIt)
            {
                if (partIt->second == basemeshname)
                {
                    mmo->setBaseMeshPart(partIt->first);
                }
                mmo->addSubmesh(partIt->first, partIt->second);
            }

            PhysicalThing* pt = PhysicsManager::getSingleton().createPhysicalThing(geomType, mmo, mass);

            actor = new Actor(uniquename, mmo, pt);
            mActors.insert(ActorPtrPair(uniquename, actor));
        }
        catch (Ogre::Exception& e)
        {
            LOG_ERROR(Logger::CORE,
                "ActorManager - Mesh parts for actor '" + uniquename
                    + "' could not be created. Reason: " + e.getFullDescription());
        }
        catch (rl::Exception& e)
        {
            LOG_ERROR(Logger::CORE,
                "ActorManager - Mesh parts for actor '" + uniquename
                    + "' could not be created. Reason: " + e.getMessage());
        }
        return actor;
    }

    Actor* ActorManager::createMeshActor(
        const String& name, const String& meshname, GeometryType geomType, Ogre::Real mass, bool mergeable)
    {
        const String& uniquename = nextUniqueName(name);
        Actor* actor = NULL;

        try
        {
            MeshObject* mo = NULL;

            if (mergeable)
            {
                mo = new MergeableMeshObject(uniquename, meshname);
            }
            else
            {
                mo = new MeshObject(uniquename, meshname);
            }
            PhysicalThing* pt = PhysicsManager::getSingleton().createPhysicalThing(geomType, mo, mass);

            actor = new Actor(uniquename, mo, pt);
            mActors.insert(ActorPtrPair(uniquename, actor));
        }
        catch (Ogre::Exception& e)
        {
            LOG_ERROR(Logger::CORE,
                "ActorManager - Das Mesh '" + meshname + "' fuer den Aktor '" + uniquename
                    + "' konnte nicht erstellt werden. Grund: " + e.getFullDescription());
        }
        catch (rl::Exception& e)
        {
            LOG_ERROR(Logger::CORE,
                "ActorManager - Das Mesh '" + meshname + "' fuer den Aktor '" + uniquename
                    + "' konnte nicht erstellt werden. Grund: " + e.getMessage());
        }
        return actor;
    }

    Actor* ActorManager::createBoxPrimitiveActor(const Ogre::String& name, const Ogre::Vector3& minCorner,
        const Ogre::Vector3& maxCorner, const Ogre::String& materialName, Ogre::Real mass)
    {
        const String uniquename = nextUniqueName(name);
        // Make sure that min/maxCorners are valid. Since this is called by scripters, be lenient.
        Vector3 minC = minCorner;
        minC.makeFloor(maxCorner);
        Vector3 maxC = maxCorner;
        maxC.makeCeil(minCorner);
        AxisAlignedBox size = AxisAlignedBox(minC, maxC);
        Actor* actor = NULL;
        try
        {
            BoxPrimitive* bp = new BoxPrimitive(size, uniquename, materialName);
            PhysicalThing* pt = PhysicsManager::getSingleton().createPhysicalThing(GT_BOX, bp, mass);

            actor = new Actor(uniquename, bp, pt);
            mActors.insert(ActorPtrPair(uniquename, actor));
        }
        catch (Ogre::Exception& e)
        {
            LOG_ERROR(Logger::CORE,
                "ActorManager - Das BoxPrimitive fuer den Aktor '" + uniquename
                    + "' konnte nicht erstellt werden. Grund: " + e.getFullDescription());
        }
        catch (rl::Exception& e)
        {
            LOG_ERROR(Logger::CORE,
                "ActorManager - Das BoxPrimitive fuer den Aktor '" + uniquename
                    + "' konnte nicht erstellt werden. Grund: " + e.getMessage());
        }
        return actor;
    }

    Actor* ActorManager::createParticleSystemActor(const String& name, const String& partname)
    {
        const String& uniquename = nextUniqueName(name);

        Actor* actor = 0;
        try
        {
            ParticleSystemObject* po = new ParticleSystemObject(uniquename, partname);

            actor = new Actor(uniquename, po, 0);
            mActors.insert(ActorPtrPair(uniquename, actor));
        }
        catch (Ogre::Exception& e)
        {
            LOG_ERROR(Logger::CORE,
                "ActorManager - Das Partikelsystem '" + partname + "' fuer den Aktor '" + uniquename
                    + "' konnte nicht erstellt werden. Grund: " + e.getFullDescription());
        }
        catch (rl::Exception& e)
        {
            LOG_ERROR(Logger::CORE,
                "ActorManager - Das Partikelsystem '" + partname + "' fuer den Aktor '" + uniquename
                    + "' konnte nicht erstellt werden. Grund: " + e.getMessage());
        }

        return actor;
    }

    String ActorManager::nextUniqueName(const String& basename)
    {
        String newname = basename;

        static int id = 0;

        while (getActor(newname) != NULL)
        {
            newname = basename + "_" + StringConverter::toString(++id);
        }

        if (basename != newname)
        {
            LOG_WARNING(Logger::CORE,
                "ActorManager - Warnung! '" + basename + "' war schon vergeben! Neuer Name ist '" + newname + "'.");
        }

        return newname;
    }
}
