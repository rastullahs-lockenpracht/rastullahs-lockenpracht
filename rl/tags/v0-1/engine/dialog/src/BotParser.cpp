/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#include <sstream>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>

#include "XmlHelper.h"
#include "XmlResourceManager.h"

#include <OgreResourceGroupManager.h>

#include "Interpreter.h"
#include "CoreSubsystem.h"
#include "DialogSubsystem.h"
#include "DialogCharacter.h"
#include "DialogScriptObject.h"
#include "BotParser.h"

XERCES_CPP_NAMESPACE_USE

namespace rl
{
	BotParser::BotParser(const CeGuiString& botName)
		: mHasScript(false), mBotName(botName), mBot(NULL)
	{
	}

	BotParser::~BotParser()
	{
	}

	bool BotParser::parse(const CeGuiString& fileName, DialogCharacter* bot)
	{
		XMLPlatformUtils::Initialize();

		XmlHelper::initializeTranscoder();

		mBot = bot;
		SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
		parser->setContentHandler(this);
		parser->setErrorHandler(this);
		Ogre::String xmlFile = fileName.c_str();
		try
		{	
			XmlPtr res = DialogSubsystem::getSingleton().getXmlResource(xmlFile);
			res->parseBy(parser);
			
			if(parser)
			{
				delete parser;
			}
			return true;
		}
		catch(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
		{
			// get & log xerces error message
			char* excmsg = XMLString::transcode(exc.getMessage());
			std::string excs = "Exception while Parsing: ";
			excs += excmsg;
			// cleanup
			if(parser)
			{
				delete parser;
			}
			throw (exc);
		}
		return false;
	}

	void BotParser::startDocument()
	{
		mCurrentState = TAG_NONE;
		mSubState = SUBTAG_NONE;
	}

	void BotParser::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const XERCES_CPP_NAMESPACE::Attributes& attrs)
	{
		CeGuiString tagName = XmlHelper::transcodeToString(localname);

		if(tagName == "dialog-startup")
		{
			mCurrentState = TAG_START;
		}

		switch(mCurrentState)
		{
		case TAG_START:
			{
				if(tagName == "bot")
				{
					CeGuiString botName = 
						XmlHelper::getAttributeValueAsString(attrs, "name");

					if(mBotName.empty() || mBotName == botName)
					{
						mBotName = botName;
						initBot();
						
						mCurrentState = TAG_BOT;
						mSubState = SUBTAG_START;
					}
				}
			}
			break;
		case TAG_BOT:
			{
				switch(mSubState)
				{
				case SUBTAG_START:
					{
						CeGuiString src;
						CeGuiString value;
						if(tagName == "script")
						{
							// Get Attributes and so on...
							mSubState = SUBTAG_SCRIPT;

							loadDialogScriptObject(
								XmlHelper::getAttributeValueAsString(attrs,"src"),
								XmlHelper::getAttributeValueAsString(attrs,"class"));
						}
						else if(tagName == "learn")
						{
							learnAiml(XmlHelper::getAttributeValueAsString(attrs,"src"));
						}
					}
					break;
				case SUBTAG_SCRIPT:
					{
						// This should never happen
					}
					break;
				case SUBTAG_LEARN:
					{
						// This should never happen
					}
					break;
				}
			}
			break;
		}
	}
		
	void BotParser::endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname)
	{
		CeGuiString tagName = XmlHelper::transcodeToString(localname);
		switch(mCurrentState)
		{
		case TAG_BOT:
			{
				if(tagName == "bot" || tagName == "level")
				{
					mCurrentState = TAG_START;
					mSubState = SUBTAG_NONE;
				}
				else
				{
					mSubState = SUBTAG_START;
				}			
			}
			break;
		}
	}

	void BotParser::characters(const XMLCh *const chars,const unsigned int len)
	{
		switch(mSubState)
		{
		case SUBTAG_SCRIPT:
			{
			}
			break;
		case SUBTAG_LEARN:
			{
			}
			break;
		}
	}

	void BotParser::startCDATA()
	{
	}

	void BotParser::startCategory()
	{
	}

	void BotParser::endCategory()
	{
	}

	void BotParser::warning (const XERCES_CPP_NAMESPACE::SAXParseException &exc)
	{
	}

	void BotParser::error (const XERCES_CPP_NAMESPACE::SAXParseException &exc)
	{
	}

	void BotParser::fatalError (const XERCES_CPP_NAMESPACE::SAXParseException &exc)
	{
	}

	void BotParser::initBot()
	{
		if(mBot == NULL)
		{
			mBot = new DialogCharacter(mBotName);
		}
		else
		{
			mBot->setName(mBotName);
		}
	}
		
	void BotParser::loadDialogScriptObject(const CeGuiString& src, const CeGuiString& className)
	{
		if(src.find("?") && className.find("?"))
		{
			// load the ruby scriptfile into the interpreter 
			CoreSubsystem::getSingleton().getInterpreter()->execute(("load \"" + src + "\"").c_str());
			// create the string for instanciating the class
			std::stringstream newDialogScriptObject;
			newDialogScriptObject << "DialogSubsystem.getSingleton()";
			newDialogScriptObject << ".getCurrentBot().setScriptObject(";
			newDialogScriptObject << className.c_str() << ".new())";
			// execute the ruby command
			CoreSubsystem::getSingleton().getInterpreter()
				->execute(newDialogScriptObject.str());
			
			mSubState = SUBTAG_START;
		}

		mHasScript = true;
	}

	void BotParser::learnAiml(const CeGuiString& fileName)
	{
		if(fileName.find("?"))
		{
            std::set<Ogre::String> files;

			Ogre::StringVectorPtr sl = 
				Ogre::ResourceGroupManager::getSingleton().findResourceNames(
                CoreSubsystem::getSingleton().getActiveAdventureModule(), fileName.c_str());
            files.insert(sl->begin(), sl->end());

            sl = Ogre::ResourceGroupManager::getSingleton().findResourceNames(
                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, fileName.c_str() );
            files.insert(sl->begin(), sl->end());

            for (std::set<Ogre::String>::iterator it = files.begin(); it != files.end(); ++it)
			{
				// check if loading of the aimlfile succeeded
				// if so, a GraphMaster for the given aimlfile is created
				if(DialogSubsystem::getSingletonPtr()->loadAimlFile((*it).c_str()))
				{
					if(mBot)
					{
						// add the GraphMaster of the aimlfile to the bot
						mBot->addGraphMaster(
							DialogSubsystem::getSingletonPtr()->
								getGraphMaster(*it));
					}
				}
			} // end for(...
		} // end if(fileName.find...
	}
}
