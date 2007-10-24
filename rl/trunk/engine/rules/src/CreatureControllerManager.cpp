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
#include "stdinc.h" //precompiled header

#include "Actor.h"
#include "Creature.h"
#include "GameTask.h"
#include "GameLoop.h"
#include "Exception.h"
#include "CreatureControllerManager.h"
#include "CreatureController.h"


using namespace Ogre;

template<> rl::CreatureControllerManager* Singleton<rl::CreatureControllerManager>::ms_Singleton = 0;


namespace rl
{
    CreatureControllerManager::CreatureControllerManager() :
            mControllers(),
            mBodyControllers()
    {
        GameLoop::getSingleton().addTask(this, GameLoop::TG_LOGIC);

        PhysicsManager* physicsManager = PhysicsManager::getSingletonPtr();
        // the material of moving creatures
        const OgreNewt::MaterialID *char_mat = physicsManager->createMaterialID("character");

        const OgreNewt::MaterialID *def_mat = physicsManager->createMaterialID("default");
        const OgreNewt::MaterialID *level_mat = physicsManager->createMaterialID("level");

        physicsManager->createMaterialPair(char_mat, def_mat)->setContactCallback(this);
        physicsManager->createMaterialPair(char_mat, level_mat)->setContactCallback(this);
        
        physicsManager->getMaterialPair(char_mat, def_mat)->setDefaultFriction(0.8f,0.4f);
        physicsManager->getMaterialPair(char_mat, level_mat)->setDefaultFriction(0.8f,0.4f);
        physicsManager->getMaterialPair(char_mat, def_mat)->setDefaultElasticity(0.0f);
        physicsManager->getMaterialPair(char_mat, level_mat)->setDefaultElasticity(0.0f);
        physicsManager->getMaterialPair(char_mat, def_mat)->setDefaultSoftness(1.0f);
        physicsManager->getMaterialPair(char_mat, level_mat)->setDefaultSoftness(1.0f);

        physicsManager->getNewtonDebugger()->setMaterialColor(char_mat, Ogre::ColourValue::Red);
    }

    CreatureControllerManager::~CreatureControllerManager()
    {
        // delete all creaturecontrollers
        for( ControllerMap::iterator it = mControllers.begin(); it != mControllers.end(); it++ )
        {
            delete it->second;
        }

        PhysicsManager *physicsManager = PhysicsManager::getSingletonPtr();
        const OgreNewt::MaterialID *char_mat = physicsManager->getMaterialID("character");

        const OgreNewt::MaterialID *def_mat = physicsManager->getMaterialID("default");
        const OgreNewt::MaterialID *level_mat = physicsManager->getMaterialID("level");

        physicsManager->resetMaterialPair(char_mat, def_mat);
        physicsManager->resetMaterialPair(char_mat, level_mat);


        GameLoop::getSingleton().removeTask(this);
    }

    CreatureController* CreatureControllerManager::getCreatureController(Creature* creature)
    {
        // valid Creature is needed as argument
        if (creature == NULL)
        {
            Throw(NullPointerException, "Argument creature darf nicht NULL sein.");
        }

        CreatureController* rval = NULL;

        // do we have a controller attached to this creature?
        ControllerMap::const_iterator it = mControllers.find(creature);
        if ( it == mControllers.end())
        {
            // No, so create one and put it into the map.
            rval = new CreatureController(creature);
            mControllers.insert(std::make_pair(creature, rval));
            mBodyControllers.insert(std::make_pair(
                creature->getActor()->getPhysicalThing()->_getBody(), rval));
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
        for( it=mControllers.begin(); it!=mControllers.end(); ++it )
        {
            cos.push_back(it->second);
        }

        return cos;
    }


    void CreatureControllerManager::detachController(Creature* creature)
    {
        if( creature == NULL)
        {
            Throw(NullPointerException, "Argument creature darf nicht NULL sein.");
        }

        mControllers.find(creature);
        ControllerMap::iterator it = mControllers.find(creature);
        if (it != mControllers.end())
        {
            delete it->second;
            mControllers.erase(it);
        }

        BodyControllerMap::iterator it_ = mBodyControllers.find(
            creature->getActor()->getPhysicalThing()->_getBody());
        if( it_ != mBodyControllers.end() )
            mBodyControllers.erase(it_);
    }

    void CreatureControllerManager::run(Real elapsedTime)
    {
        for (ControllerMap::iterator it = mControllers.begin(); it != mControllers.end(); ++it)
        {
            it->second->run(elapsedTime);
        }
    }

    int CreatureControllerManager::userProcess()
    {
        BodyControllerMap::iterator it = mBodyControllers.find(m_body0);
        if (it == mBodyControllers.end())
        {
            it = mBodyControllers.find(m_body1);

            if (it == mBodyControllers.end())
            {
                LOG_ERROR(Logger::RULES,
                    "Der Kollisionskörper konnte keiner Creature zugeordnet werden.");
                return 1;
            }
        }

        // @XXX Evil code!
        // Protected members from type OgreNewt::ContactCallback have to be overridden in order
        // for the controllers to work. This is because these members are used by OgreNewt functions
        // for processing this contact. Should probably be solved in OgreNewt directly.
        OgreNewt::ContactCallback* controller = it->second;
        *controller = (OgreNewt::ContactCallback)(*this);
        return controller->userProcess();
    }

    const Ogre::String& CreatureControllerManager::getName() const
    {
        static Ogre::String name = "CreatureControllerManager";
        return name;
    }
 }
