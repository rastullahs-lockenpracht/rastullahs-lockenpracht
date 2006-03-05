/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

using namespace Ogre;

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
		: mCurrentBot(NULL)
	{
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
			Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, excs);
            XMLString::release(&excmsg);
        }
	}

    DialogSubsystem::~DialogSubsystem() 
    {  
		for(BotMap::iterator iter = mBots.begin();
			iter != mBots.end();
			++iter)
		{
			delete iter->second;
		}
    }

	DialogCharacter* DialogSubsystem::getCurrentBot()
	{
		return mCurrentBot;
	}

	DialogCharacter* DialogSubsystem::loadBot(const CeGuiString& fileName, const CeGuiString& botName)
	{
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

}
