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
#ifndef __Rl_AimlParser_H__
#define __Rl_AimlParser_H__

#include <xercesc/sax2/DefaultHandler.hpp>
#include "DialogPrerequisites.h"
#include "NaturalLanguageProcessor.h"

#include <string>
#include <stack>

using namespace std;

namespace rl
{
	static const string ASTERISK = "*";
	
	/** Handler class used to parse the startup xml file and aiml files using SAX2
	 *  @author Philipp Walser
     */
	class _RlDialogExport AimlParser : public XERCES_CPP_NAMESPACE::DefaultHandler
	{
	public:
		//! Constructor
		AimlParser(NaturalLanguageProcessor* nlp) : mNlp(nlp) { }
		//! Destructor
		virtual ~AimlParser(void){ }

		/*************************************************************************
		SAX2 Handler overrides
		*************************************************************************/ 
		virtual void startDocument();
		virtual void startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const XERCES_CPP_NAMESPACE::Attributes& attrs);
		virtual void endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname);
		virtual void characters(const XMLCh *const chars,const unsigned int len);
		virtual void startCDATA();
		void startCategory();
		void endCategory();

		virtual void  warning (const XERCES_CPP_NAMESPACE::SAXParseException &exc);
		virtual void  error (const XERCES_CPP_NAMESPACE::SAXParseException &exc);
		virtual void  fatalError (const XERCES_CPP_NAMESPACE::SAXParseException &exc);

		/*************************************************************************
		Static helper functions
		*************************************************************************/ 
		static std::string transcodeXmlCharToString(const XMLCh* const xmlch_str);
		static std::string getAttributeValueAsString(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName);
		static int getAttributeValueAsInteger(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName);
		static int getAttributeValueAsInteger(DOMNamedNodeMap* node,const char* const nodeName);
		static const XMLCh* getAttributeValueAsXmlChar(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName);

	private:
		typedef enum 
		{
			PARSER_START = 1,
			PARSER_AIML,
			PARSER_CONTEXT,
			PARSER_TOPIC,
			PARSER_CATEGORY,
			PARSER_FINISH,
			PARSER_SCRIPT
		} AimlState;

		typedef enum 
		{
			CAT_NONE = 0,
			CAT_START = 1,
			CAT_PATTERN,
			CAT_BOT,
			CAT_NAME,
			CAT_THAT,
			CAT_TEMPLATE,
			CAT_ANY,
			CAT_FINISH
		} CategoryState;

		string contextName;
		string topicName;
		string patternValue;
		string thatValue;
		string templateValue;
	
		AimlState mCurState;	
		std::stack<AimlState> mPrevStates;
		CategoryState mSubState;
		unsigned int anyDepth;
	
		NaturalLanguageProcessor* mNlp;
	};

} // End of  rl namespace section

#endif
