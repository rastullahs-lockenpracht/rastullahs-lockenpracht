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

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/XMLString.hpp>

#include <OgreResourceGroupManager.h>

#include "CoreSubsystem.h"
#include "DialogSubsystem.h"
#include "Interpreter.h"

#include "XmlHelper.h"
#include "XmlResourceManager.h"

#include "AimlCoreComponent.h"
#include "Graphmaster.h"
#include "NaturalLanguageProcessor.h"

#include "ScriptObject.h" //deprecated!!!
#include "Predicates.h"

#include "AimlParserImplXerces.h"

namespace rl
{
	const char* AimlParserImplXerces::ASTERISK = "*";

	AimlParserImplXerces::AimlParserImplXerces(NaturalLanguageProcessor* nlp)
		: AimlParser(nlp)
	{
	}

	AimlParserImplXerces::AimlParserImplXerces(AimlCoreComponent* aimlCore)
		: AimlParser(aimlCore)
	{
	}

	bool AimlParserImplXerces::parse(const std::string& fileName)
	{
		AimlParser::parse(fileName);

		XmlHelper::initializeTranscoder();

		SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
		parser->setContentHandler(this);
		parser->setErrorHandler(this);
		
		try
		{	
			XmlPtr res = DialogSubsystem::getSingleton().getXmlResource(fileName);
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
			std::string excs="Exception while Parsing: ";
			excs+=excmsg;
			DialogSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, excs);
			// cleanup
			if(parser)delete parser;
			throw (exc);
		}
		return false;
	}
	
	bool AimlParserImplXerces::compareTagName (	const XMLCh* const arg1,
												const char* const  arg2)
	{
		XMLCh* tmpVal = XMLString::transcode(arg2);
		bool rVal = XMLString::equals(arg1, tmpVal);
		XMLString::release(&tmpVal);
		return rVal;		
	}

	/** Handles the start of a new xml/aiml document
	 *  @author Philipp Walser
	 */
	void AimlParserImplXerces::startDocument()
	{
		mCurState = PARSER_START;
		mContextName = ASTERISK;
		mTopicName = ASTERISK;
		
		mSubState = CAT_NONE;
	}

	/** Called when a xml tag was found
	 *  This method processes the tags. Partially adopted from jAlice source
	 *  @author Philipp Walser, Jonathan Roewen
	 *  @todo seperate xml- and aiml-processing?  error handling
	 */
	void AimlParserImplXerces::startElement(const XMLCh* const uri, const XMLCh* const tagName, const XMLCh* const qname, const XERCES_CPP_NAMESPACE::Attributes& attrs)
	{
		CeGuiString name, value, rtn;
		CeGuiString tmp = XmlHelper::transcodeToString(tagName);
	//	DialogSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "startElement");
	/////////////////////////////////////////
	// Abfrage für Startup-File
	////////////////////////////////////////
		if(!tmp.compare("predicate"))
		{	// Predicates haben name, value und return-Wert
			name = XmlHelper::getAttributeValueAsString(attrs,"name");
			value = XmlHelper::getAttributeValueAsString(attrs,"value");
			rtn = XmlHelper::getAttributeValueAsString(attrs,"return");
			if ( !name.empty() )
			{
				CeGuiString s = value;// == NULL ? "" : (const char *)value;
				bool b = !rtn.compare("name") ? true : false; //(bool)!rtn.compare("name"); //xmlStrcmp(rtn, (const xmlChar *)"name");
				if ( !name.compare("*") )
				{
					Predicates::setGlobalDefault(s.c_str());	// XXX PREDICATES
				} else {
					Predicates::specializePredicate(name.c_str(), s.c_str(), b); // XXX PREDICATES
				}
			}
		} else if(!tmp.compare("property"))	{	
			// Properties have name & value
			name = XmlHelper::getAttributeValueAsString(attrs,"name");
			value = XmlHelper::getAttributeValueAsString(attrs,"value");
			
			if ( !name.empty() && !value.empty() )
			{
			//	(const char*)name; // ?
				Predicates::setProperty(name.c_str(), value.c_str()); // XXX PREDICATES
			}
		} else if(!tmp.compare("option")) {	
			// Options haben name & value
			name = XmlHelper::getAttributeValueAsString(attrs,"name");
			value = XmlHelper::getAttributeValueAsString(attrs,"value");
			if ( !name.empty() && !value.empty() )
			{
				if(mNlp)
				{
//					mNlp->processOption(name, value);
				}
			}
		} else if(!tmp.compare("script")) {
		//	DialogSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "script");
			mCurState = PARSER_SCRIPT;
			name = XmlHelper::getAttributeValueAsString(attrs,"src");
			mTemplateValue = XmlHelper::getAttributeValueAsString(attrs,"class");
	//		CoreSubsystem::getSingleton().getInterpreter()->execute("print(\""+name+"\")");
	//		CoreSubsystem::getSingleton().getInterpreter()->execute("print(\""+mTemplateValue+"\")");
			// If tag has no attribute, XmlChar::transcode returns "????"
			if(name.find("?") && mTemplateValue.find("?"))
			{
				ScriptObject* so=new ScriptObject("DialogMeister"); // TODO: Name of NPC
				CeGuiString bla[]={"Hans(RB)"};
				so->setScript(name.c_str(),mTemplateValue.c_str(),1,bla);
				so->callFunction("OnPlaySound",0,0);
				mCurState=PARSER_START;
			} 
		} else if(!tmp.compare("bot")) {
		//	DialogSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "Bot");
			if(mNlp)
			{
				mNlp->setName(XmlHelper::getAttributeValueAsString(attrs,"name"));
			}
		} else if(!tmp.compare("learn")) {
		//	DialogSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "Learning");
			name = XmlHelper::getAttributeValueAsString(attrs,"src");
			if ( !name.empty())
			{
				if(mNlp)
				{
//					mNlp->processOption("load", name);
				}
			}
		}


	/////////////////////////////////////////
	// Abfrage für AIML-File
	////////////////////////////////////////
		if(compareTagName(tagName, "aiml"))
		{
			mCurState = PARSER_AIML;
		}
/*		if(!tmp.compare("aiml") && mCurState==PARSER_START)
		{		
			mCurState = PARSER_AIML;
		}
*/		switch( mCurState )
		{
		case PARSER_AIML:
			if(compareTagName(tagName, "category"))
			{
				mCurState = PARSER_CATEGORY;
				startCategory();
			} else if(compareTagName(tagName, "topic")) {
				name = XmlHelper::getAttributeValueAsString(attrs,"name");
				if(!name.empty())
					mTopicName = name;

				mCurState = PARSER_TOPIC;
			} else if(compareTagName(tagName, "context")) {
				name = XmlHelper::getAttributeValueAsString(attrs,"name");
				if(!name.empty())
					mContextName = name;

				mCurState = PARSER_CONTEXT;
			} else { 
				// an Error 
			}
			mPrevStates.push(PARSER_AIML);
			break;
		case PARSER_CONTEXT:
			if(compareTagName(tagName, "category"))
			{
				mCurState = PARSER_CATEGORY;
				startCategory();
			} else if(compareTagName(tagName, "topic")) {
				name = XmlHelper::getAttributeValueAsString(attrs,"name");
				if(!name.empty())
					mTopicName = name;

				mCurState = PARSER_TOPIC;
			} else { 
				// an Error 
			}
			mPrevStates.push(PARSER_CONTEXT);
			break;
		case PARSER_TOPIC:
			if(compareTagName(tagName, "category"))
			{
				mCurState = PARSER_CATEGORY;
				startCategory();
			} else if(compareTagName(tagName, "context")) {
				name = XmlHelper::getAttributeValueAsString(attrs,"name");
				if(!name.empty())
					mContextName = name;

				mCurState = PARSER_CONTEXT;
			} else { 
				// an Error 
			}
			mPrevStates.push(PARSER_TOPIC);
			break;
		case PARSER_CATEGORY:
			// Ehemals catStartElement()
			switch ( mSubState ) 
			{
			case CAT_START:
				//--	one of "pattern", "that"
				//--	or "template" is valid
				if ( compareTagName(tagName, "pattern") )
				{	//--	"pattern"
					mSubState = CAT_PATTERN;
				} else if ( compareTagName(tagName, "that") ) {
					//--	"that"
					mThatValue = "";
					mSubState = CAT_THAT;
				} else if ( compareTagName(tagName, "template") ) {
					//--	"template"
					mTemplateValue = "<template>";
					//--	assumed no attributes...
					mSubState = CAT_TEMPLATE;
				} else {
					//--	an error
				}
				break;
			case CAT_PATTERN:
				//--	one of "bot" or "name" is valid
				if ( compareTagName(tagName, "bot") ) 
				{	//--	"bot"
					name = XmlHelper::getAttributeValueAsString(attrs,"name");
					if ( !name.empty() ) 
					{	//--	lookup bot property with adata
						mPatternValue += Predicates::getProperty(name.c_str());	// XXX PREDICATES
						break;
					}
					mSubState = CAT_BOT;
				} else if ( compareTagName(tagName, "name") ) {
					//--	"name"
					//--	lookup bot property 'name'
					mPatternValue += Predicates::getProperty("name");	// XXX PREDICATES
					mSubState = CAT_NAME;
				} else {
					//--	an error
				}
				break;
			case CAT_BOT:
				//--	an error
				break;
			case CAT_NAME:
				//--	an error
				break;
			case CAT_THAT:
				//--	an error
				//--	OR maybe <bot/> and <name/>...
				break;
			case CAT_TEMPLATE:
				//--	assume anything is valid!
				mTemplateValue += "<";
				mTemplateValue += tmp;
				for (unsigned int i = 0; i < attrs.getLength(); i++) 
				{
					mTemplateValue += " ";
					mTemplateValue += XmlHelper::transcodeToString(attrs.getLocalName(i));
					mTemplateValue += "='";
					//--	prolly need to ensure no "'"
					mTemplateValue += XmlHelper::transcodeToString(attrs.getValue(i));
					mTemplateValue += "'";
				}
				mTemplateValue += ">";
				mSubState = CAT_ANY;
				mAnyDepth = 1;
				break;
			case CAT_ANY:
				//--	continue anything valid
				//--	assumption
				mTemplateValue += "<";
				mTemplateValue += tmp;
				for (unsigned int i = 0; i < attrs.getLength(); i++) 
				{
					mTemplateValue += " ";
					mTemplateValue += XmlHelper::transcodeToString(attrs.getLocalName(i));
					mTemplateValue += "='";
					//--	prolly need to ensure no "'"
					mTemplateValue += XmlHelper::transcodeToString(attrs.getValue(i));
					mTemplateValue += "'";
				}
				mTemplateValue += ">";
				++mAnyDepth;
				break;
			case CAT_FINISH:
				//--	an error
				break;
			}
			break;
		case PARSER_FINISH:
			//--	an error
			break;
		}
	}

	/** Called when a closing-tag was found
	 *  Partially adopted from jAliceSource
	 *  @author Philipp Walser, Jonathan Roewen
	 */
	void AimlParserImplXerces::endElement(const XMLCh* const uri, const XMLCh* const tagName, const XMLCh* const qname)
	{
		CeGuiString tmp = XmlHelper::transcodeToString(tagName);
		switch ( mCurState ) {
			case PARSER_START:
				//--	an error
				break;
			case PARSER_AIML:
				//--	go to PARSER_FINISH
				mCurState = PARSER_FINISH;
				break;
			case PARSER_CONTEXT:
				mContextName = ASTERISK;
				//--	go to prev state
				mCurState = mPrevStates.top();
				mPrevStates.pop();
				break;
			case PARSER_TOPIC:
				mTopicName = ASTERISK;
				//--	go to prev state
				mCurState = mPrevStates.top();
				mPrevStates.pop();
				break;
			case PARSER_CATEGORY:
				if ( compareTagName(tagName, "category") ) {
					endCategory();
					mCurState = mPrevStates.top();
					mPrevStates.pop();
				} else {
					// Ehemals catEndElement(data, name);
					switch ( mSubState )
					{
					case CAT_START:
						//--	an error -- or not!
						mSubState = CAT_FINISH;
						break;
					case CAT_PATTERN:
						mSubState = CAT_START;
						break;
					case CAT_BOT:
						mSubState = CAT_PATTERN;
						break;
					case CAT_NAME:
						mSubState = CAT_PATTERN;
						break;
					case CAT_THAT:
						mSubState = CAT_START;
						break;
					case CAT_TEMPLATE:
						mTemplateValue += "</template>";
						mSubState = CAT_START;
						break;
					case CAT_ANY:
						mTemplateValue += "</";
						mTemplateValue += tmp;
						mTemplateValue += ">";
						if ( --mAnyDepth == 0 ) 
						{
							mSubState = CAT_TEMPLATE;
						}
						break;
					case CAT_FINISH:
						//--	an error
						break;
					}
				}
				break;
			case PARSER_FINISH:
				//--	an error
				break;
		}
	}

	/** Called when data between tags was found
	 *  Partially adopted from jAliceSource
	 *  @author Philipp Walser, Jonathan Roewen
	 */
	void AimlParserImplXerces::characters(const XMLCh *const chars,const unsigned int len)
	{
		if ( mCurState == PARSER_CATEGORY && mSubState != CAT_NONE ) 
		{
			switch ( mSubState )
			{
			case CAT_START:
				//--	an error
				break;
			case CAT_PATTERN:
				mPatternValue += XmlHelper::transcodeToString(chars);
				break;
			case CAT_BOT:
				//--	an error
				break;
			case CAT_NAME:
				//--	an error
				break;
			case CAT_THAT:
				mThatValue += XmlHelper::transcodeToString(chars);
				break;
			case CAT_TEMPLATE:
				mTemplateValue += XmlHelper::transcodeToString(chars);
				break;
			case CAT_ANY:
				mTemplateValue += XmlHelper::transcodeToString(chars);
				break;
			case CAT_FINISH:
				//--	an error
				break;
			}
		} else if(mCurState==PARSER_SCRIPT) {
			mCurState=PARSER_START;
			CeGuiString script="class ";
			// If tag has no attribute, XmlChar::transcode returns "????"
			if(!mTemplateValue.find("?"))
				mTemplateValue="DialogClass";
			
			script += mTemplateValue+"\n";
			script += XmlHelper::transcodeToString(chars);
			script += "\n end";
			CoreSubsystem::getSingleton().getInterpreter()->execute(script.c_str());

			ScriptObject* so=new ScriptObject("DialogMeister");
			CeGuiString bla[]={mTemplateValue};
			// don't pass a scriptfile, since the class is already loaded into memory
			so->setScript("",mTemplateValue.c_str(),1,bla);
			so->callFunction("OnPlaySound",0,0);

		} else {
			//--	an error
		}

	}

	void AimlParserImplXerces::startCDATA()
	{
		// Wird später verwendet um anzugeben dass beim späteren charachter()-Aufruf wirklich
		// CDATA vorliegt
	/*
		if ( mCurState == PARSER_CATEGORY && mSubState != CAT_NONE ) 
		{
			switch ( mSubState )
			{
			case CAT_START:
			case CAT_PATTERN:
			case CAT_BOT:
			case CAT_NAME:
			case CAT_THAT:
				//--	an error
				break;
			case CAT_TEMPLATE:
			case CAT_ANY:
				mTemplateValue += "<![CDATA[";
				mTemplateValue += string((const char *)value, len);
				mTemplateValue += "]]>";
				break;
			case CAT_FINISH:
				//--	an error
				break;
			}
		} else {
			//--	an error
		}
	*/
	}

	void AimlParserImplXerces::warning(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
	{
		throw(exc);
	}

	void AimlParserImplXerces::error(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
	{
		throw(exc);
	}

	void AimlParserImplXerces::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
	{
		throw(exc);
	}

	void AimlParserImplXerces::startCategory()
	{
		mSubState = CAT_START;
		mAnyDepth = 0;
		
		mPatternValue = "";
		mThatValue = ASTERISK;
		mTemplateValue = "";
	}

	void AimlParserImplXerces::endCategory()
	{
		//--	this if statement might be wrong!
		if ( mSubState != CAT_FINISH ) {
			//--	an error
		}/*
		if(mNlp)
		{
			Nodemaster* node = mNlp->getGM()->add(
				mContextName,
				mPatternValue,
				mThatValue,
				mTopicName,
				mTemplateValue
				);
		}*/
		if(mAimlCore)
		{
			if(Graphmaster* gm = mAimlCore->getGraphMaster(mFileName))
			{
				Nodemaster* node = gm->add(
									mContextName,
									mPatternValue,
									mThatValue,
									mTopicName,
									mTemplateValue
									);
			}
		}

	/*	if ( node == NULL ) {
			//--	an error
		} else {
			node->setTemplate(mTemplateValue);
		}
	*/
		mSubState = CAT_NONE;
	}
/*
	string AimlParserImplXerces::transcodeXmlCharToString(const XMLCh* const xmlch_str)
	{			
		char* tmpVal = XMLString::transcode( xmlch_str);//(const char *)xmlNodeGetContent(node);
		string tmp(tmpVal);
		XMLString::release(&tmpVal);

		//string tmp(XMLString::transcode(xmlch_str));
		return tmp;
	}
	string AimlParserImplXerces::getAttributeValueAsString(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName)
	{
		return transcodeXmlCharToString(getAttributeValueAsXmlChar(attrs, attributeName));
	}

	int AimlParserImplXerces::getAttributeValueAsInteger(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName)
	{
		return XMLString::parseInt(getAttributeValueAsXmlChar(attrs, attributeName));
	}

	const XMLCh* AimlParserImplXerces::getAttributeValueAsXmlChar(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName)
	{
			XMLCh* attr_name = XMLString::transcode(attributeName);
			const XMLCh* val_str = attrs.getValue(attr_name);
			XMLString::release(&attr_name);
			if(val_str!=NULL){
				return val_str;}
			return (XMLCh*)"";
	}
*/
/*	int AimlParserImplXerces::getAttributeValueAsInteger(DOMNamedNodeMap* node,const char* const nodeName)
	{
			const XMLCh* val_str = node->getNamedItem(XMLString::transcode(nodeName))->getNodeValue();
			if(val_str!=NULL)
				return XMLString::parseInt(val_str);
			return 0;
	}
	*/
}