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

#include "CoreSubsystem.h"
#include "GameAreaTypes.h"
#include "GameEventManager.h"
#include "MeshObject.h"
#include "PhysicsManager.h"
#include "ScriptWrapper.h"
#include "Zone.h"

using namespace Ogre;

template <> rl::GameEventManager* Ogre::Singleton<rl::GameEventManager>::msSingleton = 0;

namespace rl
{

    GameEventManager::GameEventManager()
        : mAreaEventSources()
        , mQueuedDeletionSources()
    {
        // set newton collision callback
        PhysicsManager* physicsManager = PhysicsManager::getSingletonPtr();

        const OgreNewt::MaterialID* area_mat = physicsManager->createMaterialID("gamearea");
        const OgreNewt::MaterialID* char_mat = physicsManager->createMaterialID("character");
        const OgreNewt::MaterialID* def_mat = physicsManager->createMaterialID("default");
        const OgreNewt::MaterialID* cam_mat = physicsManager->createMaterialID("camera");
        const OgreNewt::MaterialID* lev_mat = physicsManager->createMaterialID("level");

        physicsManager->createMaterialPair(area_mat, char_mat)->setContactCallback(this);
        physicsManager->createMaterialPair(area_mat, def_mat)->setContactCallback(this);
        physicsManager->createMaterialPair(area_mat, cam_mat)->setDefaultCollidable(0); // ->setContactCallback(this);
        physicsManager->createMaterialPair(area_mat, lev_mat)->setDefaultCollidable(0); // ->setContactCallback(this);

        // set collision-debug color
        physicsManager->getNewtonDebugger()->setMaterialColor(area_mat, Ogre::ColourValue::Green);
    }

    GameEventManager::~GameEventManager()
    {
        // remove collision callbacks
        PhysicsManager* physicsManager = PhysicsManager::getSingletonPtr();

        const OgreNewt::MaterialID* area_mat = physicsManager->getMaterialID("gamearea");
        const OgreNewt::MaterialID* char_mat = physicsManager->getMaterialID("character");
        const OgreNewt::MaterialID* def_mat = physicsManager->getMaterialID("default");
        const OgreNewt::MaterialID* cam_mat = physicsManager->getMaterialID("camera");
        const OgreNewt::MaterialID* lev_mat = physicsManager->getMaterialID("level");

        physicsManager->resetMaterialPair(area_mat, char_mat);
        physicsManager->resetMaterialPair(area_mat, def_mat);
        physicsManager->resetMaterialPair(area_mat, cam_mat);
        physicsManager->resetMaterialPair(area_mat, lev_mat);

        GameAreaEventSourceList::iterator it;
        for (it = mAreaEventSources.begin(); it != mAreaEventSources.end(); ++it)
        {
            GameAreaEventSource* gam = *it;
            ScriptWrapper::getSingleton().deleted(gam);
            delete gam->getGameAreaType();
            delete gam;
        }
        mAreaEventSources.clear();
        mQueuedDeletionSources.clear();
        mBodyGameAreaMap.clear();
    }

    /// @todo  Doppelte Aktoren nachnutzen??
    GameAreaEventSource* GameEventManager::addSphereAreaListener(
        Actor* actor, Ogre::Real radius, GameAreaListener* list, unsigned long queryMask, bool forceNew)
    {
        Ogre::AxisAlignedBox aabb;
        aabb.setMaximum(radius, radius, radius);
        aabb.setMinimum(-radius, -radius, -radius);
        // neues areal ereugen
        GameNewtonBodyAreaType* at = new GameSimpleCollisionAreaType(aabb, GT_SPHERE);

        at->setQueryMask(queryMask);

        // Event-Quelle erzeugen
        GameAreaEventSource* gam = new GameAreaEventSource(at, actor);
        // In die Menge einfuegen
        mAreaEventSources.insert(gam);
        ScriptWrapper::getSingleton().owned(gam);
        // Und Listener anhaengen
        gam->addAreaListener(list);

        // add to newton collision list
        mBodyGameAreaMap.insert(std::make_pair(at->getBody(), at));

        return gam;
    }

    /// @todo  Doppelte Aktoren nachnutzen??
    GameAreaEventSource* GameEventManager::addMeshAreaListener(Actor* actor, Ogre::Entity* ent, GeometryType geom,
        GameAreaListener* list, unsigned long queryMask, Vector3 offset, Quaternion orientation, bool forceNew)
    {
        // neues areal ereugen
        if (ent == NULL)
        {
            if (actor == NULL)
                Throw(IllegalArgumentException, "actor and entity cannot be NULL!");
            MeshObject* meshObj = static_cast<MeshObject*>(actor->getControlledObject());
            ent = meshObj->getEntity();
        }

        GameNewtonBodyAreaType* at = new GameMeshAreaType(ent, geom, offset, orientation);

        at->setQueryMask(queryMask);

        // Event-Quelle erzeugen
        GameAreaEventSource* gam = new GameAreaEventSource(at, actor);
        // In die Menge einfuegen
        mAreaEventSources.insert(gam);
        ScriptWrapper::getSingleton().owned(gam);
        // Und Listener anhaengen
        gam->addAreaListener(list);

        // add to newton collision list
        mBodyGameAreaMap.insert(std::make_pair(at->getBody(), at));

        return gam;
    }

    GameAreaEventSource* GameEventManager::addMeshAreaListener(Actor* actor, GeometryType geom, GameAreaListener* list,
        unsigned long queryMask, Vector3 offset, Quaternion orientation, bool forceNew)
    {
        return addMeshAreaListener(actor, NULL, geom, list, queryMask, offset, orientation, forceNew);
    }

    /// @todo  Doppelte Aktoren nachnutzen??
    GameAreaEventSource* GameEventManager::addAreaListener(Actor* actor, Ogre::AxisAlignedBox aabb, GeometryType geom,
        GameAreaListener* list, unsigned long queryMask, Vector3 offset, Quaternion orientation, bool forceNew)
    {
        GameNewtonBodyAreaType* at = new GameSimpleCollisionAreaType(aabb, geom, offset, orientation);

        at->setQueryMask(queryMask);

        // Event-Quelle erzeugen
        GameAreaEventSource* gam = new GameAreaEventSource(at, actor);
        // In die Menge einfuegen
        mAreaEventSources.insert(gam);
        ScriptWrapper::getSingleton().owned(gam);
        // Und Listener anhaengen
        gam->addAreaListener(list);

        // add to newton collision list
        mBodyGameAreaMap.insert(std::make_pair(at->getBody(), at));

        return gam;
    }

    void GameEventManager::removeAreaListener(GameAreaListener* list)
    {
        GameAreaEventSourceList::iterator it;
        for (it = mAreaEventSources.begin(); it != mAreaEventSources.end();)
        {
            GameAreaEventSource* gam = *it;
            gam->removeAreaListener(list);

            // Sind alle Listener weggeworfen?
            if (!gam->hasListeners())
            {
                // Spaeter loeschen
                mQueuedDeletionSources.insert(gam);
            }

            // Iterieren
            ++it;
        }
    }

    void GameEventManager::removeAreaEventSource(GameAreaEventSource* gam)
    {
        GameAreaEventSourceList::iterator it = mAreaEventSources.find(gam);
        if (it != mAreaEventSources.end())
        {
            it = mQueuedDeletionSources.find(gam);
            if (it == mQueuedDeletionSources.end())
                mQueuedDeletionSources.insert(gam);
        }
    }

    void GameEventManager::removeAllAreas(Actor* actor)
    {
        if (actor == NULL)
            Throw(IllegalArgumentException, "actor cannot be NULL!");
        GameAreaEventSourceList::iterator it;
        for (it = mAreaEventSources.begin(); it != mAreaEventSources.end();)
        {
            GameAreaEventSource* gam = *it;

            // Ist das der Actor?
            if (gam->getActor() == actor)
            {
                // Spaeter loeschen

                mQueuedDeletionSources.insert(gam);
            }

            // Iterieren
            ++it;
        }
    }

    void GameEventManager::removeQueuedDeletionSources()
    {
        while (!mQueuedDeletionSources.empty())
        {
            GameAreaEventSource* gam = *mQueuedDeletionSources.begin();

            mAreaEventSources.erase(mAreaEventSources.find(gam));
            ScriptWrapper::getSingleton().deleted(gam);
            mQueuedDeletionSources.erase(mQueuedDeletionSources.begin());

            // gegebenenfalls bodymap-eintrag loeschen
            if (gam->getGameAreaType()->getBody())
            {
                NewtonBodyGameAreaMap::iterator it = mBodyGameAreaMap.find(gam->getGameAreaType()->getBody());
                if (it != mBodyGameAreaMap.end())
                    mBodyGameAreaMap.erase(it);
            }

            // Die Area-Art loeschen
            delete gam->getGameAreaType();
            // Das Objekt loeschen
            delete gam;
        }
    }

    void GameEventManager::run(Ogre::Real elapsedTime)
    {
        removeQueuedDeletionSources();

        GameAreaEventSourceList::iterator it;
        for (it = mAreaEventSources.begin(); it != mAreaEventSources.end(); ++it)
        {
            GameAreaEventSource* gam = *it;
            gam->performQuery(elapsedTime);
        }
    }

    const Ogre::String& GameEventManager::getName() const
    {
        static Ogre::String NAME = "GameEventManager";

        return NAME;
    }

    void GameEventManager::userProcess(OgreNewt::ContactJoint& contactJoint, Real timestep, int)
    {
        OgreNewt::Body* body0 = contactJoint.getBody0();
        OgreNewt::Body* body1 = contactJoint.getBody1();

        NewtonBodyGameAreaMap::iterator it = mBodyGameAreaMap.find(body0);
        if (it != mBodyGameAreaMap.end())
        {
            it->second->foundCollision(bodyToActor(body1));
            return;
        }

        it = mBodyGameAreaMap.find(body1);
        if (it != mBodyGameAreaMap.end())
        {
            it->second->foundCollision(bodyToActor(body0));
            return;
        }

        LOG_WARNING(Logger::CORE, "Der Kollisionskoerper konnte keiner GameArea zugeordnet werden!");

        // there should be no contacts (trigger-volume!), so we don't need to do anything here!
    }

    void GameEventManager::notifyNewtonWorldUpdate()
    {
        NewtonBodyGameAreaMap::iterator it = mBodyGameAreaMap.begin();
        for (; it != mBodyGameAreaMap.end(); it++)
            it->second->resetFoundCollisions();
    }

    Actor* GameEventManager::bodyToActor(OgreNewt::Body* body)
    {
        if (body->getUserData().type() == typeid(Actor*))
            return Ogre::any_cast<Actor*>(body->getUserData());
        else
            return NULL;
    }
}
