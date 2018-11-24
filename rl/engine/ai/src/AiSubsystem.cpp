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

#include "AiSubsystem.h"

#include "AgentManager.h"
#include "AiWorld.h"
#include "CoreMessages.h"
#include "CoreSubsystem.h"
#include "GameLoop.h"
#include "Landmark.h"
#include "LandmarkPath.h"
#include "WayPointGraphManager.h"

#include "ContentModule.h"
#include "DialogManager.h"
#include "XmlProcessor.h"
#include "XmlResourceManager.h"

using namespace Ogre;
using namespace OpenSteer;

template <> rl::AiSubsystem* Singleton<rl::AiSubsystem>::msSingleton = 0;

namespace rl
{

    AiSubsystem::AiSubsystem(void)
        : mAgentManager(NULL)
        , mWayPointGraphManager(NULL)
        , mWorld(NULL)
    {
        LOG_MESSAGE(Logger::AI, "Init Start");
        initialize();
        LOG_MESSAGE(Logger::AI, "Init Ende");
    }

    AiSubsystem::~AiSubsystem(void)
    {
        GameLoop::getSingleton().removeTask(AgentManager::getSingletonPtr());
        AgentManager::getSingleton().removeAllAgents();
        mWorld->removeAllObstacles();
        removeAllLandmarkPaths();
        removeAllLandmarks();
        delete mDialogManager;
        delete mAgentManager;
        delete mWayPointGraphManager;
        delete mWorld;
    }

    void AiSubsystem::initialize()
    {
        mAgentManager = new AgentManager();
        mWayPointGraphManager = new WayPointGraphManager();
        mWorld = new AiWorld();
        mDialogManager = new DialogManager();

        mSceneLoadedConnection = MessagePump::getSingleton().addMessageHandler<MessageType_SceneLoaded>(
            boost::bind(&AiSubsystem::onAfterSceneLoaded, this));
        mSceneClearingConnection = MessagePump::getSingleton().addMessageHandler<MessageType_SceneClearing>(
            boost::bind(&AiSubsystem::onBeforeClearScene, this));

        GameLoop::getSingleton().addTask(AgentManager::getSingletonPtr(), GameLoop::TG_LOGIC);
    }

    bool AiSubsystem::onBeforeClearScene()
    {
        AgentManager::getSingleton().removeAllAgents();
        mWorld->removeAllObstacles();

        return true;
    }

    bool AiSubsystem::onAfterSceneLoaded()
    {
        // newton world hinzuf�gen
        Obstacle* newtonWorld = new NewtonWorldAsObstacle;
        newtonWorld->setSeenFrom(AbstractObstacle::both);
        mWorld->addObstacle(newtonWorld);

        return true;
    }

    Landmark* AiSubsystem::createLandmark(const Ogre::String& name, const Ogre::Vector3& position)
    {
        Landmark* lm = getLandmark(name);
        if (lm == NULL)
        {
            lm = new Landmark(name, position);
            mLandmarks[name] = lm;
        }
        else
        {
            if (lm->getPosition() != position)
            {
                std::ostringstream oss;
                oss << "A Landmark with the name '" << name << "' already exists at position " << lm->getPosition()
                    << "! The position will not be changed to " << position << "!";
                LOG_ERROR(Logger::AI, oss.str());
            }
        }
        return lm;
    }

    Landmark* AiSubsystem::getLandmark(const Ogre::String& name) const
    {
        std::map<Ogre::String, Landmark*>::const_iterator it = mLandmarks.find(name);

        if (it == mLandmarks.end())
        {
            return NULL;
        }
        return (*it).second;
    }

    LandmarkPath* AiSubsystem::createLandmarkPath(const Ogre::String& name)
    {
        LandmarkPath* path = new LandmarkPath(name);
        mLandmarkPaths[name] = path;
        return path;
    }

    LandmarkPath* AiSubsystem::getLandmarkPath(const Ogre::String& name) const
    {
        std::map<Ogre::String, LandmarkPath*>::const_iterator it = mLandmarkPaths.find(name);

        if (it == mLandmarkPaths.end())
        {
            return NULL;
        }
        return (*it).second;
    }

    void AiSubsystem::removeAllLandmarkPaths()
    {
        for (std::map<Ogre::String, LandmarkPath*>::iterator it = mLandmarkPaths.begin(); it != mLandmarkPaths.end();
             it++)
        {
            delete (*it).second;
        }

        mLandmarkPaths.clear();
    }

    void AiSubsystem::removeAllLandmarks()
    {
        for (std::map<Ogre::String, Landmark*>::iterator it = mLandmarks.begin(); it != mLandmarks.end(); it++)
        {
            delete (*it).second;
        }

        mLandmarks.clear();
    }
}
