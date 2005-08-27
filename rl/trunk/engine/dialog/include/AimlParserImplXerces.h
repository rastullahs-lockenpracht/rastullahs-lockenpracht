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
#ifndef __Rl_AimlParserImplXerces_H__
#define __Rl_AimlParserImplXerces_H__

#include <stack>

#include <xercesc/sax2/DefaultHandler.hpp>

#include "AimlParser.h"

namespace rl
{
	class _RlDialogExport AimlParserImplXerces :
		public AimlParser, public XERCES_CPP_NAMESPACE::DefaultHandler
	{
	public:
		AimlParserImplXerces(NaturalLanguageProcessor* nlp);
		AimlParserImplXerces(AimlCoreComponent* aimlCore);
		virtual ~AimlParserImplXerces(){};

		virtual bool parse(const std::string& fileName);

		bool AimlParserImplXerces::compareTagName (	const XMLCh* const arg1,
													const char* const  arg2);

		/**
		 * SAX2 Handler overrides
		 */
		virtual void startDocument();
		virtual void startElement (	const XMLCh* const uri, 
									const XMLCh* const tagName, 
									const XMLCh* const qname, 
									const XERCES_CPP_NAMESPACE::Attributes& attrs);

		virtual void endElement(const XMLCh* const uri, 
								const XMLCh* const tagName, 
								const XMLCh* const qname);

		virtual void characters(const XMLCh *const chars, const unsigned int len);
		
		virtual void startCDATA();
		
		void startCategory();
		
		void endCategory();

		virtual void  warning (const XERCES_CPP_NAMESPACE::SAXParseException &exc);
		
		virtual void  error (const XERCES_CPP_NAMESPACE::SAXParseException &exc);

		virtual void  fatalError (const XERCES_CPP_NAMESPACE::SAXParseException &exc);

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

		static const char* ASTERISK;

		CeGuiString contextName;
		CeGuiString topicName;
		CeGuiString patternValue;
		CeGuiString thatValue;
		CeGuiString templateValue;
	
		AimlState mCurState;	
		std::stack<AimlState> mPrevStates;
		CategoryState mSubState;
		unsigned int anyDepth;
	};
}

#endif
