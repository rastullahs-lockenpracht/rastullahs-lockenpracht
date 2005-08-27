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
			log(Ogre::LML_TRIVIAL, excs);
            XMLString::release(&excmsg);
        }

	//  Load dialog-startup definition with bot properties etc.pp.
	//	if(XmlResourceManager::getSingleton().getByName("dialog-startup.xml")==NULL)
	//			XmlResourceManager::getSingleton().create("dialog-startup.xml");
	//	???	what is this for? should bots be preloaded? i guess not! 
	//  or do we need it for aiml preloading?

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
/*
	DOMDocument* DialogSubsystem::createDOMDocumentFromFile(const std::string& fileName)
	{
		XercesDOMParser* parser = new XercesDOMParser();
				
		try
		{	
			XmlPtr res;
			if(XmlResourceManager::getSingleton().getByName(fileName).isNull())
			{
				res = XmlResourceManager::getSingleton().create(
						fileName, 
						ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			}
			else
			{
				res = XmlResourceManager::getSingleton().getByName(fileName);
			}
			res.getPointer()->parseBy(parser);
		}
		catch(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
		{
			// get & log xerces error message
			char* excmsg = XMLString::transcode(exc.getMessage());
			std::string excs="Exception while Parsing: ";
			excs+=excmsg;
			log(Ogre::LML_TRIVIAL, excs);
			// cleanup
			if(parser)delete parser;
		//	if(xmlHandler)delete xmlHandler;
			throw (exc);
		}
		DOMDocument* doc = parser->adoptDocument();
		if(parser)delete parser;
		return doc;
	}
*/
	DialogCharacter* DialogSubsystem::getCurrentBot()
	{
		return mCurrentBot;
		/*
		BotMap::iterator iter = mBots.find(botName);
		if( iter != mBots.end())
		{
			return iter->second;
		}
		return NULL;
		*/
	}


	DialogCharacter* DialogSubsystem::loadBot(const std::string& fileName, const std::string& botName)
	{
		mCurrentBot = new DialogCharacter();
		setAimlParser(new AimlParserImplXerces(this));
		BotParser parser(botName);
		if(parser.parse(fileName, mCurrentBot))
		{
			BotMap::iterator iter = mBots.find(mCurrentBot->getName().c_str());
			if(iter == mBots.end())
			{
				mBots[mCurrentBot->getName().c_str()] = mCurrentBot;
				return mCurrentBot;
			}	
		}
		else
		{
			delete mCurrentBot;
			mCurrentBot = NULL;
		}
		return NULL;
/*		DOMDocument* doc;// = createDOMDocumentFromFile(fileName);

		if ( doc != NULL ) 
		{
			//  get the content of DOMDocument
			DOMElement* node = doc->getDocumentElement();	
			if(node != NULL)
			{
				DOMElement* bot = XmlHelper::getChildNamed(node, "bot");
				if(!botName.empty())
				{
					//load this bot
					node = XmlHelper::getChildNamed(bot, "script");
					// do script stuff
					//learn Aiml
					DOMNodeList* learnAiml = bot->getElementsByTagName(XMLString::transcode("learn"));
					for (unsigned int i = 0; i < learnAiml->getLength(); ++i)
					{
						if(learnAiml->item(i)->getNodeType() == DOMNode::ELEMENT_NODE)
						{
							CeGuiString fileName = XmlHelper::getAttributeValueAsString(
								static_cast<DOMElement*>(learnAiml->item(i)), 
														"src");
							loadAimlFile(fileName.c_str());
						}
						else
						{
							if(learnAiml->item(i)->getFirstChild()->getNodeType() == DOMNode::TEXT_NODE )
							{
							}
						}
					}
				}
				else
				{
					DOMNodeList* bots = node->getElementsByTagName(XMLString::transcode("bot"));
					for (unsigned int i = 0; i < bots->getLength(); ++i)
					{
						if(bots->item(i)->getNodeType()  == DOMNode::ELEMENT_NODE )
						{
							static_cast<DOMElement*>(bots->item(i));
						}
						
						if(std::string(XmlHelper::getAttributeValueAsString(
							bots->item(i),
							XMLString::transcode("name")) == botName)
						{
						}
						
					}
				}
		*/		
	/*
				for (	node = node->getFirstChild(); 
						node != NULL; 
						node = node->getNextSibling() )
				{

				}
				*/
	/*		}
			doc->release();
			doc = NULL;
			node = NULL;
		}
	*/	
		


	}

	void DialogSubsystem::log(const Ogre::LogMessageLevel level, const Ogre::String& msg, const Ogre::String& ident)
	{
		Logger::getSingleton().log(level, "Dialog", msg, ident);
	}

	
}
