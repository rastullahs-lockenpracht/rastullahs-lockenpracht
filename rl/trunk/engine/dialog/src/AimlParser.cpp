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

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/util/XMLString.hpp>

#include "AimlParser.h"
#include "NaturalLanguageProcessor.h"
#include "Predicates.h"
#include "Nodemaster.h"
#include "ScriptObject.h"
#include "CoreSubsystem.h"
#include "Interpreter.h"
#include "XmlHelper.h"
#include "DialogSubsystem.h"

#include <string>


XERCES_CPP_NAMESPACE_USE
using namespace std;

namespace rl
{
	/** Handles the start of a new xml/aiml document
	 *  @author Philipp Walser
	 */
	void AimlParser::startDocument()
	{
		mCurState = PARSER_START;
		contextName = ASTERISK;
		topicName = ASTERISK;
		
		mSubState = CAT_NONE;
	}

	/** Called when a xml tag was found
	 *  This method processes the tags. Partially adopted from jAlice source
	 *  @author Philipp Walser, Jonathan Roewen
	 *  @todo seperate xml- and aiml-processing?  error handling
	 */
	void AimlParser::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const XERCES_CPP_NAMESPACE::Attributes& attrs)
	{
		string name;
		string value;
		string rtn;
		string tmp =transcodeXmlCharToString(localname);
		DialogSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "startElement");
	/////////////////////////////////////////
	// Abfrage für Startup-File
	////////////////////////////////////////
		if(!tmp.compare("predicate"))
		{	// Predicates haben name, value und return-Wert
			name=getAttributeValueAsString(attrs,"name");
			value=getAttributeValueAsString(attrs,"value");
			rtn=getAttributeValueAsString(attrs,"return");
			if ( !name.empty() )
			{
				string s = value;// == NULL ? "" : (const char *)value;
				bool b = !rtn.compare("name") ? true : false; //(bool)!rtn.compare("name"); //xmlStrcmp(rtn, (const xmlChar *)"name");
				if ( !name.compare("*") )
				{
					Predicates::setGlobalDefault(s);	// XXX PREDICATES
				} else {
					Predicates::specializePredicate(name, s, b); // XXX PREDICATES
				}
			}
		} else if(!tmp.compare("property"))	{	
			// Properties have name & value
			name=getAttributeValueAsString(attrs,"name");
			value=getAttributeValueAsString(attrs,"value");
			
			if ( !name.empty() && !value.empty() )
			{
				(const char*)name.c_str(); // ?
				Predicates::setProperty(name, value); // XXX PREDICATES
			}
	//		mNlp->mTest=tmp;
		} else if(!tmp.compare("option")) {	
			// Options haben name & value
			name=getAttributeValueAsString(attrs,"name");
			value=getAttributeValueAsString(attrs,"value");
			if ( !name.empty() && !value.empty() )
				mNlp->processOption(name, value);
	//		mNlp->mTest=tmp;
		} else if(!tmp.compare("script")) {
			DialogSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "script");
			mCurState= PARSER_SCRIPT;
			name=getAttributeValueAsString(attrs,"name");
			templateValue=getAttributeValueAsString(attrs,"class");
	//		CoreSubsystem::getSingleton().getInterpreter()->execute("print(\""+name+"\")");
	//		CoreSubsystem::getSingleton().getInterpreter()->execute("print(\""+templateValue+"\")");
			// If tag has no attribute, XmlChar::transcode returns "????"
			if(name.find("?") && templateValue.find("?"))
			{
				ScriptObject* so=new ScriptObject("DialogMeister"); // TODO: Name of NPC
				Ogre::String bla[]={"Hans(RB)"};
				so->setScript(name,templateValue,1,bla);
				so->callFunction("OnPlaySound",0,0);
				mCurState=PARSER_START;
			} 
		} else if(!tmp.compare("bot")) {
			DialogSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "Bot");
			mNlp->setName(getAttributeValueAsString(attrs,"name"));
		} else if(!tmp.compare("learn")) {
			DialogSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "Learning");
			name=getAttributeValueAsString(attrs,"src");
			//value=getAttributeValueAsString(attrs,"value");
			if ( !name.empty())
				mNlp->processOption("load", name);
		}


	/////////////////////////////////////////
	// Abfrage für AIML-File
	////////////////////////////////////////
		if(!tmp.compare("aiml") && mCurState==PARSER_START)
		{		
			mCurState = PARSER_AIML;
		}
		switch( mCurState )
		{
		case PARSER_AIML:
			if(!tmp.compare("category"))
			{
				mCurState = PARSER_CATEGORY;
				startCategory();
			} else if(!tmp.compare("topic")) {
				name=getAttributeValueAsString(attrs,"name");
				if(!name.empty())
					topicName = name;

				mCurState = PARSER_TOPIC;
			} else if(!tmp.compare("context")) {
				name=getAttributeValueAsString(attrs,"name");
				if(!name.empty())
					contextName = name;

				mCurState = PARSER_CONTEXT;
			} else { 
				// an Error 
			}
			mPrevStates.push(PARSER_AIML);
			break;
		case PARSER_CONTEXT:
			if(!tmp.compare("category"))
			{
				mCurState = PARSER_CATEGORY;
				startCategory();
			} else if(!tmp.compare("topic")) {
				name=getAttributeValueAsString(attrs,"name");
				if(!name.empty())
					topicName = name;

				mCurState = PARSER_TOPIC;
			} else { 
				// an Error 
			}
			mPrevStates.push(PARSER_CONTEXT);
			break;
		case PARSER_TOPIC:
			if(!tmp.compare("category"))
			{
				mCurState = PARSER_CATEGORY;
				startCategory();
			} else if(!tmp.compare("context")) {
				name=getAttributeValueAsString(attrs,"name");
				if(!name.empty())
					contextName = name;

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
				if ( !tmp.compare("pattern") )
				{	//--	"pattern"
					mSubState = CAT_PATTERN;
				} else if ( !tmp.compare("that") ) {
					//--	"that"
					thatValue = "";
					mSubState = CAT_THAT;
				} else if ( !tmp.compare("template") ) {
					//--	"template"
					templateValue = "<template>";
					//--	assumed no attributes...
					mSubState = CAT_TEMPLATE;
				} else {
					//--	an error
				}
				break;
			case CAT_PATTERN:
				//--	one of "bot" or "name" is valid
				if ( !tmp.compare("bot") ) 
				{	//--	"bot"
					name=getAttributeValueAsString(attrs,"name");
					if ( !name.empty() ) 
					{	//--	lookup bot property with adata
						patternValue += Predicates::getProperty(name);	// XXX PREDICATES
						break;
					}
					mSubState = CAT_BOT;
				} else if ( !tmp.compare("name") ) {
					//--	"name"
					//--	lookup bot property 'name'
					patternValue += Predicates::getProperty("name");	// XXX PREDICATES
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
				templateValue += "<";
				templateValue += tmp;
				for (unsigned int i = 0; i < attrs.getLength(); i++) 
				{
					templateValue += " ";
					templateValue += transcodeXmlCharToString(attrs.getLocalName(i));
					templateValue += "='";
					//--	prolly need to ensure no "'"
					templateValue += transcodeXmlCharToString(attrs.getValue(i));
					templateValue += "'";
				}
				templateValue += ">";
				mSubState = CAT_ANY;
				anyDepth = 1;
				break;
			case CAT_ANY:
				//--	continue anything valid
				//--	assumption
				templateValue += "<";
				templateValue += tmp;
				for (unsigned int i = 0; i < attrs.getLength(); i++) 
				{
					templateValue += " ";
					templateValue += transcodeXmlCharToString(attrs.getLocalName(i));
					templateValue += "='";
					//--	prolly need to ensure no "'"
					templateValue += transcodeXmlCharToString(attrs.getValue(i));
					templateValue += "'";
				}
				templateValue += ">";
				++anyDepth;
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
	void AimlParser::endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname)
	{
		XERCES_CPP_NAMESPACE_USE
		std::string tmp =transcodeXmlCharToString(localname);
		switch ( mCurState ) {
			case PARSER_START:
				//--	an error
				break;
			case PARSER_AIML:
				//--	go to PARSER_FINISH
				mCurState = PARSER_FINISH;
				break;
			case PARSER_CONTEXT:
				contextName = ASTERISK;
				//--	go to prev state
				mCurState = mPrevStates.top();
				mPrevStates.pop();
				break;
			case PARSER_TOPIC:
				topicName = ASTERISK;
				//--	go to prev state
				mCurState = mPrevStates.top();
				mPrevStates.pop();
				break;
			case PARSER_CATEGORY:
				if ( !tmp.compare("category") ) {
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
						templateValue += "</template>";
						mSubState = CAT_START;
						break;
					case CAT_ANY:
						templateValue += "</";
						templateValue += tmp;
						templateValue += ">";
						if ( --anyDepth == 0 ) 
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
	void AimlParser::characters(const XMLCh *const chars,const unsigned int len)
	{
		if ( mCurState == PARSER_CATEGORY && mSubState != CAT_NONE ) 
		{
			switch ( mSubState )
			{
			case CAT_START:
				//--	an error
				break;
			case CAT_PATTERN:
				patternValue += transcodeXmlCharToString(chars);
				break;
			case CAT_BOT:
				//--	an error
				break;
			case CAT_NAME:
				//--	an error
				break;
			case CAT_THAT:
				thatValue += transcodeXmlCharToString(chars);
				break;
			case CAT_TEMPLATE:
				templateValue += transcodeXmlCharToString(chars);
				break;
			case CAT_ANY:
				templateValue += transcodeXmlCharToString(chars);
				break;
			case CAT_FINISH:
				//--	an error
				break;
			}
		} else if(mCurState==PARSER_SCRIPT) {
			mCurState=PARSER_START;
			string script="class ";
			// If tag has no attribute, XmlChar::transcode returns "????"
			if(!templateValue.find("?"))
				templateValue="DialogClass";
			
			script+=templateValue+"\n";
			script+=transcodeXmlCharToString(chars);
			script+="\n end";
			CoreSubsystem::getSingleton().getInterpreter()->execute(script);

			ScriptObject* so=new ScriptObject("DialogMeister");
			Ogre::String bla[]={templateValue};
			// don't pass a scriptfile, since the class is already loaded into memory
			so->setScript("",templateValue,1,bla);
			so->callFunction("OnPlaySound",0,0);

		} else {
			//--	an error
		}

	}

	void AimlParser::startCDATA()
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
				templateValue += "<![CDATA[";
				templateValue += string((const char *)value, len);
				templateValue += "]]>";
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

	void AimlParser::warning(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
	{
		throw(exc);
	}

	void AimlParser::error(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
	{
		throw(exc);
	}

	void AimlParser::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
	{
		throw(exc);
	}

	void AimlParser::startCategory()
	{
		mSubState = CAT_START;
		anyDepth = 0;
		
		patternValue = "";
		thatValue = ASTERISK;
		templateValue = "";
	}

	void AimlParser::endCategory()
	{
		//--	this if statement might be wrong!
		if ( mSubState != CAT_FINISH ) {
			//--	an error
		}
		
		Nodemaster *node = mNlp->getGM()->add(
			contextName,
			patternValue,
			thatValue,
			topicName,
			templateValue
		);

	/*	if ( node == NULL ) {
			//--	an error
		} else {
			node->setTemplate(templateValue);
		}
	*/
		mSubState = CAT_NONE;
	}

	string AimlParser::transcodeXmlCharToString(const XMLCh* const xmlch_str)
	{			
		char* tmpVal = XMLString::transcode( xmlch_str);//(const char *)xmlNodeGetContent(node);
		string tmp(tmpVal);
		XMLString::release(&tmpVal);

		//string tmp(XMLString::transcode(xmlch_str));
		return tmp;
	}
	string AimlParser::getAttributeValueAsString(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName)
	{
		return transcodeXmlCharToString(getAttributeValueAsXmlChar(attrs, attributeName));
	}

	int AimlParser::getAttributeValueAsInteger(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName)
	{
		return XMLString::parseInt(getAttributeValueAsXmlChar(attrs, attributeName));
	}

	const XMLCh* AimlParser::getAttributeValueAsXmlChar(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName)
	{
			XMLCh* attr_name = XMLString::transcode(attributeName);
			const XMLCh* val_str = attrs.getValue(attr_name);
			XMLString::release(&attr_name);
			if(val_str!=NULL){
				return val_str;}
			return (XMLCh*)"";
	}

	int AimlParser::getAttributeValueAsInteger(DOMNamedNodeMap* node,const char* const nodeName)
	{
			const XMLCh* val_str = node->getNamedItem(XMLString::transcode(nodeName))->getNodeValue();
			if(val_str!=NULL)
				return XMLString::parseInt(val_str);
			return 0;
	}
} // Namespace rl end
