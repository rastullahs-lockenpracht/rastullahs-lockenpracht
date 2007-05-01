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
#include <xercesc/util/PlatformUtils.hpp>

#include "XmlHelper.h"
#include "XmlResourceManager.h"
#include "Logger.h"
#include "CoreSubsystem.h"
#include "ContentModule.h"

#include "DialogSubsystem.h"
#include "AimlCore.h"
#include "AimlParserImplRl.h"
#include "DialogCharacter.h"
#include "ScriptProcessor.h"
#include "DialogScriptProcessor.h"
#include "ContextInterpreter.h"

using namespace Ogre;
using namespace MadaBot;
using namespace XERCES_CPP_NAMESPACE;

template<> rl::DialogSubsystem* Singleton<rl::DialogSubsystem>::ms_Singleton = 0;

namespace rl {

DialogSubsystem::DialogSubsystem()
	: mCore(new AimlCore()),
	  mContextInterpreter(new ContextInterpreter()),
	  mCurrentBot(NULL)
{
	LOG_MESSAGE(Logger::DIALOG, "Init Start");
	initialize();
	LOG_MESSAGE(Logger::DIALOG, "Init Ende");
}

DialogSubsystem::~DialogSubsystem() 
{  
	delete mCore;
	delete mContextInterpreter;

	for(BotMap::iterator iter = mBots.begin();
		iter != mBots.end();
		++iter)
	{
		delete iter->second;
	}
}

void DialogSubsystem::initialize()
{
#if OGRE_PLATFORM != OGRE_PLATFORM_WIN32
    setlocale(LC_ALL, "C");
#endif
    
	mCore->setParser(new AimlParserImplRl());
	mCore->getBotInterpreter().addProcessor(new DialogScriptProcessor());
	mCore->getAimlInterpreter().addProcessor(new ScriptProcessor());
//  Initialize Xerces if this wasn't done already
	try 
	{
        XMLPlatformUtils::Initialize();
		XmlHelper::initializeTranscoder();
    }
    catch (const XMLException& exc) 
	{
		char* excmsg = XMLString::transcode(exc.getMessage());
		std::string excs="Exception while initializing Xerces: ";
		excs+=excmsg;
		LOG_MESSAGE(Logger::DIALOG, excs);
        XMLString::release(&excmsg);
    }
}

DialogCharacter* DialogSubsystem::getBot(const CeGuiString& botName)
{
	BotMap::iterator itr = mBots.find(botName);
	if(itr != mBots.end())
	{
		return itr->second;
	}
	return NULL;
}


DialogCharacter* DialogSubsystem::loadBot(const CeGuiString& botName, const CeGuiString& fileName)
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

ResourcePtr DialogSubsystem::getXmlResource(const Ogre::String& filename)
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

void DialogSubsystem::setCurrentDialogCharacter(DialogCharacter* bot)
{
	mCurrentBot = bot;
}

DialogCharacter* DialogSubsystem::getCurrentDialogCharacter() const
{
    return mCurrentBot;
}

}
