/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h" //precompiled header

#include "AiSubsystem.h"

#include "AiWorld.h"
#include "AgentManager.h"
#include "CoreMessages.h"
#include "CoreSubsystem.h"
#include "GameLoop.h"
#include "Landmark.h"
#include "LandmarkPath.h"
#include "Logger.h"
#include "WayPointGraphManager.h"
#include "World.h"

using namespace Ogre;
using namespace OpenSteer;

template<> rl::AiSubsystem* Singleton<rl::AiSubsystem>::ms_Singleton = 0;

namespace rl {

AiSubsystem::AiSubsystem(void)
    : mAgentManager(NULL),
	  mWayPointGraphManager(NULL),
      mWorld(NULL)
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
    delete mAgentManager;
	delete mWayPointGraphManager;
    delete mWorld;
}

void AiSubsystem::initialize()
{
    mAgentManager = new AgentManager();
	mWayPointGraphManager = new WayPointGraphManager();
    mWorld = new AiWorld();

    mSceneLoadedConnection =
        MessagePump::getSingleton().addMessageHandler<MessageType_SceneLoaded>(
		    boost::bind(&AiSubsystem::onAfterSceneLoaded, this));
    mSceneClearingConnection =
        MessagePump::getSingleton().addMessageHandler<MessageType_SceneClearing>(
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
    // newton world hinzufügen
    Obstacle *newtonWorld = new NewtonWorldAsObstacle;
    newtonWorld->setSeenFrom(AbstractObstacle::both);
    mWorld->addObstacle(newtonWorld);

    return true;
}

Landmark* AiSubsystem::createLandmark(const Ogre::String& name, const Ogre::Vector3& position)
{
	Landmark* lm = new Landmark(name, position);
	mLandmarks[name] = lm;
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
	for (std::map<Ogre::String, LandmarkPath*>::iterator it = mLandmarkPaths.begin();
		it != mLandmarkPaths.end(); it++)
	{
		delete (*it).second;
	}

	mLandmarkPaths.clear();
}

void AiSubsystem::removeAllLandmarks()
{
	for (std::map<Ogre::String, Landmark*>::iterator it = mLandmarks.begin();
		it != mLandmarks.end(); it++)
	{
		delete (*it).second;
	}

	mLandmarks.clear();
}


}
