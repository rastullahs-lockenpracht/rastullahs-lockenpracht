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
#ifndef __Rl_BotParser_H__
#define __Rl_BotParser_H__

#include <stack>

#include <xercesc/sax2/DefaultHandler.hpp>

#include "CommonPrerequisites.h"
#include "DialogPrerequisites.h"

namespace rl
{
	class DialogCharacter;

	class _RlDialogExport BotParser : public XERCES_CPP_NAMESPACE::DefaultHandler
	{
	public:
		BotParser(const CeGuiString& botName);
		virtual ~BotParser(void);

		virtual bool parse(const CeGuiString& fileName, DialogCharacter* bot);

		virtual DialogCharacter* getBot();
		
		virtual void startDocument();
		virtual void startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const XERCES_CPP_NAMESPACE::Attributes& attrs);
		virtual void endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname);
		virtual void characters(const XMLCh *const chars,const unsigned int len);
		virtual void startCDATA();
		void startCategory();
		void endCategory();

		void initBot();
		void loadDialogScriptObject(const CeGuiString& src, const CeGuiString& className);
		void learnAiml(const CeGuiString& fileName);

		virtual void  warning (const XERCES_CPP_NAMESPACE::SAXParseException &exc);
		virtual void  error (const XERCES_CPP_NAMESPACE::SAXParseException &exc);
		virtual void  fatalError (const XERCES_CPP_NAMESPACE::SAXParseException &exc);

		/*************************************************************************
		Static helper functions, deprecated
		*************************************************************************/ 
/*		static std::string transcodeXmlCharToString(const XMLCh* const xmlch_str);
		static std::string getAttributeValueAsString(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName);
		static int getAttributeValueAsInteger(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName);
		static int getAttributeValueAsInteger(DOMNamedNodeMap* node,const char* const nodeName);
		static const XMLCh* getAttributeValueAsXmlChar(const XERCES_CPP_NAMESPACE::Attributes& attrs, const char* const attributeName);
*/
	private:
		typedef enum 
		{
			TAG_NONE = 0,
			TAG_START,
			TAG_BOT
		} BotState;

		typedef enum 
		{
			SUBTAG_NONE = 0,
			SUBTAG_START = 1,
			SUBTAG_SCRIPT,
			SUBTAG_LEARN
		} SubState;


/*		string contextName;
		string topicName;
		string patternValue;
		string thatValue;
		string templateValue;
*/	
		BotState mCurrentState;	
		std::stack<BotState> mPrevStates;
		SubState mSubState;
		unsigned int anyDepth;
		bool mHasScript;
		CeGuiString mBotName;

		DialogCharacter* mBot;
	

	//	NaturalLanguageProcessor* mNlp;
	};

	inline DialogCharacter* BotParser::getBot()
	{
		return mBot;
	}

}

#endif
