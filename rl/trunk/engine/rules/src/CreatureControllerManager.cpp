/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the Perl Artistic License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  Perl Artistic License for more details.
*
*  You should have received a copy of the Perl Artistic License
*  along with this program; if not you can get it here
*  http://www.perldoc.com/perl5.6/Artistic.html.
*/


#include "Actor.h"
#include "Creature.h"
#include "GameTask.h"
#include "GameLoop.h"
#include "Exception.h"
#include "CreatureControllerManager.h"
#include "CreatureController.h"


using namespace Ogre;
using namespace std;


template<> rl::CreatureControllerManager* Singleton<rl::CreatureControllerManager>::ms_Singleton = 0;


namespace rl
{
    CreatureControllerManager::CreatureControllerManager() :
            mUpdateIdleTime(1.0f),
            mTimeSinceLastIdleUpdate(0.0f),
            mName("CreatureControllerManager")
    {
        GameLoop::getSingleton().addTask(this, GameLoop::TG_LOGIC);

        PhysicsManager *physicsManager = PhysicsManager::getSingletonPtr();
        // the material of moving creatures
        const OgreNewt::MaterialID *char_mat = physicsManager->createMaterialID("character");

        const OgreNewt::MaterialID *def_mat = physicsManager->createMaterialID("default");
        const OgreNewt::MaterialID *level_mat = physicsManager->createMaterialID("level");

        physicsManager->createMaterialPair(char_mat, def_mat)->setContactCallback(this);
        physicsManager->createMaterialPair(char_mat, level_mat)->setContactCallback(this);
    }

    CreatureControllerManager::~CreatureControllerManager()
    {
        PhysicsManager *physicsManager = PhysicsManager::getSingletonPtr();
        const OgreNewt::MaterialID *char_mat = physicsManager->getMaterialID("character");

        const OgreNewt::MaterialID *def_mat = physicsManager->getMaterialID("default");
        const OgreNewt::MaterialID *level_mat = physicsManager->getMaterialID("level");

        physicsManager->resetMaterialPair(char_mat, def_mat);
        physicsManager->resetMaterialPair(char_mat, level_mat);


        GameLoop::getSingleton().removeTask(this);
    }

    void CreatureControllerManager::add(CreatureController *movingCreature)
    {
        if(movingCreature == NULL)
        {
            Throw(NullPointerException, "Argument movingCreature darf nicht NULL sein.");
        }

        MovingCreatureVector::const_iterator iter;
        for(iter = mIdleCreatures.begin(); iter != mIdleCreatures.end(); iter++)
        {
            if( (*iter) == movingCreature )
            {
                Throw(IllegalArgumentException, "CreatureController wird schon vom CreatureControllerManager verwaltet.");
            }
        }
        for(iter = mActiveCreatures.begin(); iter != mActiveCreatures.end(); iter++)
        {
            if( (*iter) == movingCreature )
            {
                Throw(IllegalArgumentException, "CreatureController wird schon vom CreatureControllerManager verwaltet.");
            }
        }

        mMovingCreatureFromBody.insert(make_pair(
            movingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody(),
            movingCreature));
        mIdleCreatures.push_back(movingCreature);
    }


    void CreatureControllerManager::remove(CreatureController *movingCreature)
    {
        if(movingCreature == NULL)
        {
            Throw(NullPointerException, "Argument movingCreature darf nicht NULL sein.");
        }

        MovingCreatureBodyMap::iterator body_iter = mMovingCreatureFromBody.find(
            movingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody());

        if( body_iter == mMovingCreatureFromBody.end() )
        {
            Throw(IllegalArgumentException, "CreatureController wird nicht vom CreatureControllerManager verwaltet.");
        }

        mMovingCreatureFromBody.erase(body_iter);


        MovingCreatureVector::iterator iter;
        for(iter = mIdleCreatures.begin(); iter != mIdleCreatures.end(); iter++)
        {
            if( (*iter) == movingCreature )
            {
                mIdleCreatures.erase(iter);
                return;
            }
        }
        for(iter = mActiveCreatures.begin(); iter != mActiveCreatures.end(); iter++)
        {
            if( (*iter) == movingCreature )
            {
                mActiveCreatures.erase(iter);
                return;
            }
        }
        for(iter = mAddToActiveCreatures.begin(); iter != mAddToActiveCreatures.end(); iter++)
        {
            if( (*iter) == movingCreature )
            {
                mAddToActiveCreatures.erase(iter);
                return;
            }
        }


        Throw(IllegalArgumentException, "CreatureController wird nicht vom CreatureControllerManager verwaltet.");
    }

    void CreatureControllerManager::run(Real elapsedTime)
    {
        mTimeSinceLastIdleUpdate += elapsedTime;


        MovingCreatureVector::iterator iter;
        for(iter = mActiveCreatures.begin(); iter != mActiveCreatures.end(); iter ++)
        {
            (*iter)->run(elapsedTime);
        }

        if( mTimeSinceLastIdleUpdate >= mUpdateIdleTime )
        {
            for(iter = mIdleCreatures.begin(); iter != mIdleCreatures.end(); iter++)
            {
                (*iter)->run(mTimeSinceLastIdleUpdate);
            }
            mTimeSinceLastIdleUpdate = 0.0f;
        }
    }


    void CreatureControllerManager::setActive(CreatureController* movingCreature)
    {
        if(movingCreature == NULL)
        {
            Throw(NullPointerException, "Argument movingCreature darf nicht NULL sein.");
        }

        // we cannot simply add the item to the other list, since it needs to be updated first!!
        // i hope there are no errors, if this update is not at the proposed time in the game loop
        // since it is not executed with the other runs!!!

        MovingCreatureVector::iterator iter;
        for(iter = mIdleCreatures.begin(); iter != mIdleCreatures.end(); iter++)
        {
            if( *iter == movingCreature )
            {
                movingCreature->run(mTimeSinceLastIdleUpdate);

                mActiveCreatures.push_back(movingCreature);
                mIdleCreatures.erase(iter);
                return;
            }
        }

        for(iter = mActiveCreatures.begin(); iter != mActiveCreatures.end(); iter++)
        {
            if( *iter == movingCreature )
            {
                LOG_DEBUG(Logger::RULES, "Die angegebene CreatureController ist schon aktiv.");
                return;
            }
        }


        Throw(IllegalArgumentException, "CreatureController wird nicht vom CreatureControllerManager verwaltet.");
    }


    int CreatureControllerManager::userProcess()
    {
        MovingCreatureBodyMap::iterator iter;
        iter = mMovingCreatureFromBody.find(m_body0);

        if( iter == mMovingCreatureFromBody.end() )
        {
            iter = mMovingCreatureFromBody.find(m_body1);

            if( iter == mMovingCreatureFromBody.end() )
            {
                LOG_ERROR(Logger::RULES, "Der Kollisionskörper konnte keiner Creature zugeordner werden.");
                return 1;
            }
        }

        // i hope this will copy the protected members of the contact callback
        OgreNewt::ContactCallback *movingCreature = iter->second;
        *movingCreature = (OgreNewt::ContactCallback)(*this);
        return movingCreature->userProcess();
    }
}
