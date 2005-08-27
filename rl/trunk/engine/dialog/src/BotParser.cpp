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
//#include "ScriptObject.h"
#include "Interpreter.h"
#include "CoreSubsystem.h"
#include "DialogSubsystem.h"
#include "DialogCharacter.h"
#include "BotParser.h"

XERCES_CPP_NAMESPACE_USE

namespace rl
{
	BotParser::BotParser(const CeGuiString& botName)
		: hasScript(false), mBotName(botName), mBot(NULL)
	{
	}

	BotParser::~BotParser()
	{
	}

	bool BotParser::parse(const std::string& fileName, DialogCharacter* bot)
	{
		XMLPlatformUtils::Initialize();

		XmlHelper::initializeTranscoder();

		mBot = bot;
		SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
		parser->setContentHandler(this);
		parser->setErrorHandler(this);
		try
		{	
			XmlPtr res;
			if(XmlResourceManager::getSingleton().getByName(fileName).isNull())
			{
				res = XmlResourceManager::getSingleton().create(
								fileName, 
								Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			}
			else
			{
				res = XmlResourceManager::getSingleton().getByName(fileName);
			}
			res.getPointer()->parseBy(parser);
			
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
			std::string excs="Exception while Parsing: ";
			excs+=excmsg;
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
					const CEGUI::utf8* test = botName.data();
					botName = CeGuiString(test);
					if(mBotName.empty() || mBotName == botName)
					{
						mBotName = botName;
						if(mBot == NULL)
						{
							mBot = new DialogCharacter(mBotName);
						}
						else
						{
							mBot->setName(mBotName);
						}
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
							src = XmlHelper::getAttributeValueAsString(attrs,"src");
							value = XmlHelper::getAttributeValueAsString(attrs,"class");
							if(src.find("?") && value.find("?"))
							{
								CoreSubsystem::getSingleton().getInterpreter()->execute(("load \"" + src + "\"").c_str());
								std::stringstream test;
								test << "DialogSubsystem.getSingleton()";
								test << ".getCurrentBot().setScriptObject(";
								test << value.c_str() << ".new())";
								CoreSubsystem::getSingleton().getInterpreter()->execute(test.str());

								mSubState = SUBTAG_START;
							}
							
							hasScript = true;
						}
						else if(tagName == "learn")
						{
							src = XmlHelper::getAttributeValueAsString(attrs,"src");
							if(src.find("?"))
							{
								Ogre::StringVectorPtr sl = 
									Ogre::ResourceGroupManager::getSingleton().findResourceNames(
									Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, src.c_str() );
								Ogre::StringVector::iterator iter;
								for (iter = sl.get()->begin(); iter != sl.get()->end(); ++iter)
								{
									if(DialogSubsystem::getSingletonPtr()->loadAimlFile((*iter).c_str()))
									{
										if(mBot)
										{
											mBot->addGraphMaster(
												DialogSubsystem::getSingletonPtr()->
													getGraphMaster((*iter).c_str()));
										}
									}
								}
							}
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
}
