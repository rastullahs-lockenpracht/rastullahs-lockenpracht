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
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/dom/DOM.hpp>

#include "XmlHelper.h"
#include "XmlResourceManager.h"
#include "Logger.h"

#include "CoreSubsystem.h"
#include "ContentModule.h"

#include "AimlParserImplXerces.h"
#include "AimlProcessorManager.h"

#include "DialogCharacter.h"
#include "BotParser.h"
#include "DialogSubsystem.h"
//new includes
#include "AimlCore.h"
#include "AimlParserImplRl.h"
#include "ContextInterpreter.h"
#include "ScriptProcessor.h"
using namespace Ogre;
using namespace MadaBot;

template<> rl::DialogSubsystem* Singleton<rl::DialogSubsystem>::ms_Singleton = 0;

namespace rl 
{
	DialogSubsystem& DialogSubsystem::getSingleton(void)
	{
		return Singleton<DialogSubsystem>::getSingleton();
	}

	DialogSubsystem* DialogSubsystem::getSingletonPtr(void)
	{
		return Singleton<DialogSubsystem>::getSingletonPtr();
	}

	DialogSubsystem::DialogSubsystem()
		: mCore(new AimlCore()), mCurrentBot(NULL)
	{
		mCore->setParser(new AimlParserImplRl());
		mCore->getBotInterpreter().addProcessor(new ScriptProcessor());
		AimlProcessorManager::addStandardProcessors();
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
			Logger::getSingleton().log(Logger::DIALOG, Logger::LL_MESSAGE, excs);
            XMLString::release(&excmsg);
        }
	}

    DialogSubsystem::~DialogSubsystem() 
    {  
		delete mCore;
		for(BotMap::iterator iter = mBots.begin();
			iter != mBots.end();
			++iter)
		{
			delete iter->second;
		}
		AimlProcessorManager::shutdown();
    }

	DialogCharacter* DialogSubsystem::getCurrentBot()
	{
		return mCurrentBot;
	}

	DialogCharacter* DialogSubsystem::loadBot(const CeGuiString& fileName, const CeGuiString& botName)
	{
		mCurrentBot = NULL;
		AimlBot<CeGuiString>* bot = mCore->loadBot(botName.c_str(), fileName.c_str());
		// while processing the bot definition, a DialogCharacter should have been created 
		// through a ruby script and stored in mCurrentBot
		if(mCurrentBot != NULL && bot != NULL)
		{
			mCurrentBot->setBot(bot);
		}
		return mCurrentBot;
		
/*
		mCurrentBot = new DialogCharacter();
		setAimlParser(new AimlParserImplXerces(this));
		BotParser parser(botName);
		if(parser.parse(fileName, mCurrentBot))
		{
			BotMap::iterator iter = mBots.find(mCurrentBot->getName());
			if(iter == mBots.end())
			{
				mBots[mCurrentBot->getName()] = mCurrentBot;
			}
			else
			{
				delete mCurrentBot;
				mCurrentBot = iter->second;
			}
		}
		else
		{
			delete mCurrentBot;
			mCurrentBot = NULL;
		}
		return mCurrentBot;
*/
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

	void DialogSubsystem::testNewDialogSystem()
	{
		AimlBot<CeGuiString>* bot = mCore->loadBot("Alrike", "startup_test.xml");
		// while processing the bot definition, a DialogCharacter should been created 
		// through a ruby script and stored in mCurrentBot
		if(mCurrentBot != NULL)
		{
			mCurrentBot->setBot(bot);
		}
		Response<CeGuiString> response = bot->respond("1");
		if(response.hasGossip())
		{
			ContextInterpreter interpreter;
			interpreter.interpret(response.getGossip(), bot);
		}
		delete bot;
	}

}
