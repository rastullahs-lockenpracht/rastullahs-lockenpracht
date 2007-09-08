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

#include <xercesc/util/PlatformUtils.hpp>

#include "XmlProcessor.h"
#include "XmlResourceManager.h"
#include "ContentModule.h"

#include "AimlCore.h"
#include "AimlParserImplRl.h"
#include "DialogCharacter.h"
#include "ScriptProcessor.h"
#include "DialogScriptProcessor.h"
#include "ContextInterpreter.h"

using namespace Ogre;
using namespace MadaBot;
using namespace XERCES_CPP_NAMESPACE;

using namespace Ogre;
using namespace OpenSteer;

template<> rl::AiSubsystem* Singleton<rl::AiSubsystem>::ms_Singleton = 0;

namespace rl {

AiSubsystem::AiSubsystem(void)
    : mAgentManager(NULL),
	  mWayPointGraphManager(NULL),
      mWorld(NULL),
      mCore(new AimlCore()),
	  mContextInterpreter(new ContextInterpreter()),
	  mCurrentBot(NULL)

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

  	delete mCore;
	delete mContextInterpreter;

	for(BotMap::iterator iter = mBots.begin();
		iter != mBots.end();
		++iter)
	{
		delete iter->second;
	}

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

#if OGRE_PLATFORM != OGRE_PLATFORM_WIN32
    setlocale(LC_ALL, "C");
#endif

	mCore->setParser(new AimlParserImplRl());
	mCore->getBotInterpreter().addProcessor(new DialogScriptProcessor());
	mCore->getAimlInterpreter().addProcessor(new ScriptProcessor());
////  Initialize Xerces if this wasn't done already
//	try
//	{
//        
//    }
//    catch (const XMLException& exc)
//	{
//		char* excmsg = XMLString::transcode(exc.getMessage());
//		std::string excs="Exception while initializing Xerces: ";
//		excs+=excmsg;
//		LOG_MESSAGE(Logger::DIALOG, excs);
//        XMLString::release(&excmsg);
//    }
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

DialogCharacter* AiSubsystem::getBot(const CeGuiString& botName)
{
	BotMap::iterator itr = mBots.find(botName);
	if(itr != mBots.end())
	{
		return itr->second;
	}
	return NULL;
}


DialogCharacter* AiSubsystem::loadBot(const CeGuiString& botName, const CeGuiString& fileName)
{
	mCurrentBot = NULL;
	AimlBot<CeGuiString>* bot = mCore->loadBot(botName.c_str(), fileName.c_str());
//  while processing the bot definition, a DialogCharacter should have been created
//  through a ruby script and stored in mCurrentBot
    if (mCurrentBot == NULL)
    {
        mCurrentBot = new DialogCharacter();
    }

	if(bot != NULL)
	{
		mCurrentBot->setBot(bot);
		mCurrentBot->initialize();
	}
	LOG_MESSAGE(Logger::DIALOG,"AimlBot " + botName + "loaded and initialized");
	mBots.insert(BotMap::value_type(mCurrentBot->getName(), mCurrentBot));
	return mCurrentBot;
}

ResourcePtr AiSubsystem::getXmlResource(const Ogre::String& filename)
{
    ResourcePtr res = XmlResourceManager::getSingleton().getByName(filename);

    if (res.isNull())
    {
        Ogre::String group = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
        if (ResourceGroupManager::getSingleton().resourceExists(
			CoreSubsystem::getSingleton().getActiveAdventureModule()->getId(), filename))
        {
            group = CoreSubsystem::getSingleton().getActiveAdventureModule()->getId();
        }
        res = XmlResourceManager::getSingleton().create(filename, group);

    }
    return res;
}

void AiSubsystem::setCurrentDialogCharacter(DialogCharacter* bot)
{
	mCurrentBot = bot;
}

DialogCharacter* AiSubsystem::getCurrentDialogCharacter() const
{
    return mCurrentBot;
}


}
