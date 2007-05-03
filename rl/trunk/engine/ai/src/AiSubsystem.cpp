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
#include "AiSubsystem.h"

#include "AiWorld.h"
#include "AgentManager.h"
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
    CoreSubsystem::getSingletonPtr()->getWorld()->removeSceneChangeListener(this);
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

    CoreSubsystem::getSingletonPtr()->getWorld()->addSceneChangeListener(this);
    GameLoop::getSingleton().addTask(AgentManager::getSingletonPtr(), GameLoop::TG_LOGIC);
}

void AiSubsystem::onBeforeClearScene()
{
    AgentManager::getSingleton().removeAllAgents();
    mWorld->removeAllObstacles();
}



void AiSubsystem::onAfterSceneLoaded()
{
    // newton world hinzufügen
    Obstacle *newtonWorld = new NewtonWorldAsObstacle;
    newtonWorld->setSeenFrom(AbstractObstacle::both);
    mWorld->addObstacle(newtonWorld);

    // äußere grenzen einfügen
    //PhysicsManager
/*
//  create an obstacle as bounding box of the walkarea for npcs
//  this should be accessable through scripting, the Obstacles should have names
//  for easier access
    BoxObstacle* o = new BoxObstacle(25,50,25);
    o->setSeenFrom(AbstractObstacle::inside);
    o->setPosition(Vec3(-40.0f,-10.0f, 0.0f));
    o->setForward(0,0,-1);
    addObstacle(o);

    o = new BoxObstacle(2,50,2);
    o->setSeenFrom(AbstractObstacle::outside);
    o->setPosition(Vec3(-31.5f,-10.0f, -3.5f));
    o->setForward(0,0,-1);
    addObstacle(o);
*/
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
