//--	NaturalLanguageProcessor.cpp

#include "xercesc/sax2/SAX2XMLReader.hpp"
#include "xercesc/sax2/XMLReaderFactory.hpp"
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>

#include "NaturalLanguageProcessor.h"
#include "Utils.h"
#include "Graphmaster.h"
#include "AimlProcessorManager.h"
#include "AimlProcessor.h"
#include "AimlParser.h"
#include "Nodemaster.h"
#include "Match.h"
//#include "Substituter.h"
//#include "Predicates.h"
//#include "StringTokenizer.h"

#include "OgreResourceManager.h"
#include "OgreLogManager.h"



XERCES_CPP_NAMESPACE_USE

namespace rl
{
	NaturalLanguageProcessor::NaturalLanguageProcessor(std::string dialogFile): mGm(new Graphmaster())
	{
		Ogre::Log* log = Ogre::LogManager::getSingleton().getLog( "logs/rlDialog.log" );		
		log->logMessage("NLP gestartet");
		mExit=false;
		AimlParser* mXmlHandler=new AimlParser(this);
		SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
		parser->setContentHandler(mXmlHandler);
		parser->setErrorHandler(mXmlHandler);
		// do parse (which uses handler to create actual data)
		try
		{
			parser->parse(XMLString::transcode(dialogFile.c_str()));
		}
		catch(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
		{
			char* excmsg = XMLString::transcode(exc.getMessage());
			std::string excs="Exception while Parsing: ";
			excs+=excmsg;
			delete parser;
			delete mXmlHandler;
			Log* log = LogManager::getSingleton().getLog( "logs/rlDialog.log" );		
			log->logMessage(excs);

			throw (exc);
		}
		// cleanup
		delete parser;
		delete mXmlHandler;
	}

	NaturalLanguageProcessor::~NaturalLanguageProcessor()
	{
		delete mGm;
		Ogre::Log* log = Ogre::LogManager::getSingleton().getLog( "logs/rlDialog.log" );		
		log->logMessage("NLP beendet");
	}

	std::map<int,std::string> NaturalLanguageProcessor::respond(const std::string &input)
	{
		Log* log = Ogre::LogManager::getSingleton().getLog( "logs/rlDialog.log" );		
		log->logMessage("Starte Respond");
		string response="<response>", result, pattern;
		string context = "*";
		string topic = "*";
		//--	since we do the sentence work, not Predicate
		string that = "*";
		pattern=input;

		mResponses.clear();
		log->logMessage("Matching...");
		Match *m = mGm->match(context, pattern, that, topic);

		DOMDocument* doc=(DOMDocument *)m->getNode()->getTemplateNode();
		DOMNode* node=doc->getDocumentElement();

		log->logMessage("Processing...");
		response+= process(node, m,"0");	// last Parameter has no function at the moment
		response+="</response>";			// response must be in Tags fpr PostProcessing
		doc->release();
		// Eigentlich muss ein delete doc hier möglich sein, aber seit neustem sorgt dies für einen Absturz
	//	delete doc;
		if(mExit)return mResponses;
		log->logMessage("PostProcessing...");
		XercesDOMParser* parser=new XercesDOMParser();
		MemBufInputSource mTest((const XMLByte *)response.data(),response.size(),"testresponse",false);
		parser->parse(mTest);
		doc=parser->adoptDocument();
		delete parser;

		node=doc->getDocumentElement();
		
		int id=0;
		for ( node = node->getFirstChild(); node != NULL; node = node->getNextSibling() )
		{
			if ( node->getNodeType() == DOMNode::ELEMENT_NODE )
			{
				string nodeName=AimlParser::transcodeXmlCharToString(node->getNodeName());
				if(!nodeName.compare("li"))
				{
					id=AimlParser::getAttributeValueAsInteger(node->getAttributes(),"id");
				}
			}
			if ( node->getNodeType() == DOMNode::TEXT_NODE )
			{
				string dialogChoice=AimlParser::transcodeXmlCharToString( ((DOMText*)node)->getData());
				mResponses[id]=dialogChoice;
				id=0;
			}
		}
		doc->release();
		// Eigentlich muss ein delete doc hier möglich sein, aber seit neustem sorgt dies für einen Absturz
	//	delete doc;
		log->logMessage("Respond beendet");
		return mResponses;
	}

	// process option-tag from XMlStartup
	// evtl. könnte man hieraus nochmal nen AimlProcessor machen.
	void NaturalLanguageProcessor::processOption(const std::string &name, const std::string &value) 
	{	
		if ( !name.compare("load") ) 
		{
			std::set<String> sl = ResourceManager::_getAllCommonNamesLike( "./" , ".aiml" );
			std::set<String>::iterator i;
			for (i = sl.begin(); i != sl.end(); i++)
			{
				std::string file = "./media/";
				file+=(*i).c_str();
				if(loadAiml(file))
					continue;
			}
		}
	}

	bool NaturalLanguageProcessor::loadAiml(const std::string &filename) 
	{
		Log* log = LogManager::getSingleton().getLog( "logs/rlDialog.log" );		
		log->logMessage("Loading Aiml");
		log->logMessage(filename);
		AimlParser* mXmlHandler=new AimlParser(this);
		SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
		parser->setContentHandler(mXmlHandler);
		parser->setErrorHandler(mXmlHandler);
		try
		{
			parser->parse(filename.c_str());
		}
		catch(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
		{
			char* excmsg = XMLString::transcode(exc.getMessage());
			std::string excs="Exception while Parsing: ";
			excs+=excmsg;
			delete parser;
			delete mXmlHandler;
			Log* log = LogManager::getSingleton().getLog( "logs/rlDialog.log" );		
			log->logMessage(excs);
			throw (exc);
		}
		// cleanup
		delete parser;
		delete mXmlHandler;
		return true;
	}

	string NaturalLanguageProcessor::process(DOMNode *node, Match *match, const string &id) 
	{
		Log* log = LogManager::getSingleton().getLog( "logs/rlDialog.log" );	

		if ( node == NULL ) return "";
	
		string result;
		string text;
		string nodeData;
		bool lastWasElement = false;
		bool lastTailIsWS = false;
		bool nextHeadIsWS = false;

		for ( node = node->getFirstChild(); node != NULL; node = node->getNextSibling() )
		{
			if ( node->getNodeType() == DOMNode::TEXT_NODE ) 
			{
				//--	fix whitespace here
				nodeData=AimlParser::transcodeXmlCharToString(((DOMText*)node)->getData());
				if ( normalise(nodeData).empty() ) 
				{
					lastTailIsWS = !nodeData.empty();
					continue;
				}
				nextHeadIsWS = checkHeadWS(nodeData);
				//--	if last was not an element
				//--		if last text tail was whitespace
				//--			add ' ' before next text
				//--	else last was an element (closing)
				//--		if next text head is whitespace
				//--			add ' ' before next text
				//--	add text with normalised internal whitespace
				if ( !lastWasElement && lastTailIsWS ) 
				{
					result += ' ';
				} else if ( lastWasElement && nextHeadIsWS ) {
					result += ' ';
				}
				lastTailIsWS = checkTailWS(nodeData);
				result += normalise(nodeData);
				lastWasElement = false;
			} else if ( node->getNodeType() == DOMNode::CDATA_SECTION_NODE ) {
				//--	what to do about CDATA???	
				nodeData=AimlParser::transcodeXmlCharToString(((DOMText*)node)->getData()); // Attention: Cast to CDATA, not to DOMText*!
				result += nodeData;
			} else if ( node->getNodeType() == DOMNode::ELEMENT_NODE ) {
				nodeData=AimlParser::transcodeXmlCharToString(node->getNodeName());
				AimlProcessor* pt=AimlProcessorManager::getProcessor(nodeData);
				if ( pt == NULL ) 
				{
					log->logMessage("Für diesen Tag existiert kein Processor");
					text = process(node, match, id);
					if ( !result.empty() && *(--result.end()) != ' ' && lastTailIsWS ) 
					{
						result += ' ';
					}
					result += text;
					lastWasElement = true;
					lastTailIsWS = false;
				} else {
					text=pt->process(node, match, id.c_str(), this);
					//--fix whitespace here
					//--	if last was not an element
					//--		if last text tail was whitespace
					//--			add ' ' before next element
					if ( !result.empty() && *(--result.end()) != ' ' && lastTailIsWS )
						result += ' ';
					//--	add next element
					result += text;
					lastWasElement = true;
					lastTailIsWS = false;
				}
			}	//--	end else if node->getNodeType { ignore other content }
		} // end for-loop
		return result;
	}
} // Namespace rl end
