#ifndef __RL_DIALOGHANDLER_H__
#define __RL_DIALOGHANDLER_H__

#include "xercesc/sax2/DefaultHandler.hpp"
#include "DialogPrerequisites.h"
#include "NaturalLanguageProcessor.h"
// Start of CEGUI namespace section
#include <string>
#include <stack>

using namespace std;

namespace rl
{
	static const string ASTERISK = "*";
/*!
\brief
	Handler class used to parse the Scheme XML files using SAX2
*/
	class _RlDialogExport AimlParser : public XERCES_CPP_NAMESPACE::DefaultHandler
	{
	public:
		/*************************************************************************
		Construction & Destruction
		*************************************************************************/
		AimlParser(NaturalLanguageProcessor* nlp) : mNlp(nlp) { }
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
			PARSER_FINISH
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
		stack<AimlState> mPrevStates;
		CategoryState mSubState;
		unsigned int anyDepth;
	
		NaturalLanguageProcessor* mNlp;
	};

} // End of  rl namespace section

#endif	// end of guard _CEGUIScheme_xmlHandler_h_
