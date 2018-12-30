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

#include "Actor.h"
#include "Creature.h"
#include "CreatureController.h"
#include "CreatureControllerManager.h"
#include "Exception.h"
#include "GameLoop.h"
#include "GameTask.h"
#include "PhysicsManager.h"

using namespace Ogre;

template <> rl::CreatureControllerManager* Singleton<rl::CreatureControllerManager>::msSingleton = nullptr;

namespace rl
{
    CreatureControllerManager::CreatureControllerManager()
    {
        GameLoop::getSingleton().addTask(this, GameLoop::TG_LOGIC);

        PhysicsManager* physicsManager = PhysicsManager::getSingletonPtr();
        // the material of moving creatures
        const OgreNewt::MaterialID* char_mat = physicsManager->createMaterialID("character");

        const OgreNewt::MaterialID* def_mat = physicsManager->createMaterialID("default");
        const OgreNewt::MaterialID* level_mat = physicsManager->createMaterialID("level");

        physicsManager->createMaterialPair(char_mat, def_mat)->setContactCallback(this);
        physicsManager->createMaterialPair(char_mat, level_mat)->setContactCallback(this);
        physicsManager->createMaterialPair(char_mat, char_mat); //->setContactCallback(this);

        physicsManager->getMaterialPair(char_mat, def_mat)->setDefaultFriction(0.0f, 0.0f);
        physicsManager->getMaterialPair(char_mat, level_mat)->setDefaultFriction(0.0f, 0.0f);
        physicsManager->getMaterialPair(char_mat, char_mat)->setDefaultFriction(0.0f, 0.0f);
        physicsManager->getMaterialPair(char_mat, def_mat)->setDefaultElasticity(0.1f);
        physicsManager->getMaterialPair(char_mat, level_mat)->setDefaultElasticity(0.1f);
        physicsManager->getMaterialPair(char_mat, char_mat)->setDefaultElasticity(0.1f);
        physicsManager->getMaterialPair(char_mat, def_mat)->setDefaultSoftness(0.1f);
        physicsManager->getMaterialPair(char_mat, level_mat)->setDefaultSoftness(0.1f);
        physicsManager->getMaterialPair(char_mat, char_mat)->setDefaultSoftness(0.1f);
        physicsManager->getMaterialPair(char_mat, def_mat)->setDefaultSurfaceThickness(0.0f);
        physicsManager->getMaterialPair(char_mat, level_mat)->setDefaultSurfaceThickness(0.0f);
        physicsManager->getMaterialPair(char_mat, char_mat)->setDefaultSurfaceThickness(0.0f);

        /*
                physicsManager->getMaterialPair(char_mat, def_mat)->setDefaultFriction(0.8f,0.4f);
                physicsManager->getMaterialPair(char_mat, level_mat)->setDefaultFriction(0.8f,0.4f);
                physicsManager->getMaterialPair(char_mat, char_mat)->setDefaultFriction(0.8f,0.4f);
                physicsManager->getMaterialPair(char_mat, def_mat)->setDefaultElasticity(0.01f);
                physicsManager->getMaterialPair(char_mat, level_mat)->setDefaultElasticity(0.01f);
                physicsManager->getMaterialPair(char_mat, char_mat)->setDefaultElasticity(0.01);
                physicsManager->getMaterialPair(char_mat, def_mat)->setDefaultSoftness(0.8f);
                physicsManager->getMaterialPair(char_mat, level_mat)->setDefaultSoftness(0.8f);
                physicsManager->getMaterialPair(char_mat, char_mat)->setDefaultSoftness(0.8f);
        */
        physicsManager->getNewtonDebugger()->setMaterialColor(char_mat, Ogre::ColourValue::Red);
    }

    CreatureControllerManager::~CreatureControllerManager()
    {
        // should not be needed, because all creatures should be destroyed before
        // causes an error because PhysicalThing::setPhysicsController(nullptr) is called probably after the
        // physicalthing is destroyed delete all creaturecontrollers
        // for( ControllerMap::iterator it = mControllers.begin(); it != mControllers.end(); it++ )
        //{
        //    delete it->second;
        //}

        PhysicsManager* physicsManager = PhysicsManager::getSingletonPtr();
        const OgreNewt::MaterialID* char_mat = physicsManager->getMaterialID("character");

        const OgreNewt::MaterialID* def_mat = physicsManager->getMaterialID("default");
        const OgreNewt::MaterialID* level_mat = physicsManager->getMaterialID("level");

        physicsManager->resetMaterialPair(char_mat, def_mat);
        physicsManager->resetMaterialPair(char_mat, level_mat);

        GameLoop::getSingleton().removeTask(this);
    }

    CreatureController* CreatureControllerManager::getCreatureController(Creature* creature)
    {
        // valid Creature is needed as argument
        if (creature == nullptr)
        {
            Throw(NullPointerException, "Argument creature darf nicht nullptr sein.");
        }

        CreatureController* rval = nullptr;

        // do we have a controller attached to this creature?
        ControllerMap::const_iterator it = mControllers.find(creature);
        if (it == mControllers.end())
        {
            // No, so create one and put it into the map.
            rval = new CreatureController(creature);
            mControllers.insert(std::make_pair(creature, rval));
        }
        else
        {
            rval = it->second;
        }

        return rval;
    }

    std::list<CreatureController*> CreatureControllerManager::getAllCreatureController() const
    {
        std::list<CreatureController*> cos;
        ControllerMap::const_iterator it;

        //
        //    Run through all GOs and put them into the list
        //
        for (it = mControllers.begin(); it != mControllers.end(); ++it)
        {
            cos.push_back(it->second);
        }

        return cos;
    }

    void CreatureControllerManager::detachController(Creature* creature)
    {
        CreatureController* controller = nullptr;
        if (creature == nullptr)
        {
            Throw(NullPointerException, "Argument creature darf nicht nullptr sein.");
        }

        ControllerMap::iterator it = mControllers.find(creature);
        if (it != mControllers.end())
        {
            controller = it->second;
            delete it->second;
            mControllers.erase(it);
        }
    }

    void CreatureControllerManager::run(Real elapsedTime)
    {
        for (ControllerMap::iterator it = mControllers.begin(); it != mControllers.end(); ++it)
        {
            it->second->run(elapsedTime);
        }
    }

    void CreatureControllerManager::userProcess(OgreNewt::ContactJoint& contactJoint, Real timestep, int threadid)
    {
        Actor* actor = nullptr;
        if (contactJoint.getBody0()->getUserData().getType() == typeid(Actor*))
            actor = Ogre::any_cast<Actor*>(contactJoint.getBody0()->getUserData());
        if (actor != nullptr)
        {
            ControllerMap::const_iterator it = mControllers.find(static_cast<Creature*>(actor->getGameObject()));
            if (it != mControllers.end())
            {
                it->second->userProcess(contactJoint, timestep, threadid);
                return;
            }
        }

        // if the controlled body is the second body...
        if (contactJoint.getBody1()->getUserData().getType() == typeid(Actor*))
            actor = Ogre::any_cast<Actor*>(contactJoint.getBody1()->getUserData());
        if (actor != nullptr)
        {
            ControllerMap::const_iterator it = mControllers.find(static_cast<Creature*>(actor->getGameObject()));
            if (it != mControllers.end())
            {
                it->second->userProcess(contactJoint, timestep, threadid);
                return;
            }
        }

        LOG_ERROR(Logger::RULES, "Der Kollisionskoerper konnte keinem CreatureController zugeordnet werden.");
    }

    const Ogre::String& CreatureControllerManager::getName() const
    {
        static Ogre::String name = "CreatureControllerManager";
        return name;
    }
}
