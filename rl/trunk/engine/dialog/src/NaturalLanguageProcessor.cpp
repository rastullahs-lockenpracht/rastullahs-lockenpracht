/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>

#include "DialogSubsystem.h"
#include "NaturalLanguageProcessor.h"
#include "Utils.h"
#include "Graphmaster.h"
#include "AimlProcessorManager.h"
#include "AimlProcessor.h"
#include "AimlParser.h"
#include "Nodemaster.h"
#include "Match.h"
#include "XmlHelper.h"
//#include "Substituter.h"
//#include "Predicates.h"
//#include "StringTokenizer.h"

#include "XmlResourceManager.h"
#include <OgreLogManager.h>

XERCES_CPP_NAMESPACE_USE

namespace rl
{
	/** Constructor
	 *  Parse through the given dialog startup file and loads the AIML
	 *  @author Philipp Walser
	 *  @param dialogFile The startup xml-file, contains information about a specific NPC
	 *
	*/
	NaturalLanguageProcessor::NaturalLanguageProcessor(std::string dialogFile): mGm(new Graphmaster())
	{	XMLPlatformUtils::Initialize();	//wahrscheinlich nicht nötig
		DialogSubsystem::getSingleton().log("NLP gestartet");
		mExit=false;

		AimlParser* xmlHandler=new AimlParser(this);
		SAX2XMLReader* parser =XMLReaderFactory::createXMLReader();
		parser->setContentHandler(xmlHandler);
		parser->setErrorHandler(xmlHandler);
	//  do parse (which uses handler to create actual data)
		try
		{
		//	XmlResourceManager::getSingleton().create(dialogFile)->parseBy(parser);
			parser->parse(XMLString::transcode(dialogFile.c_str()));
		}
		catch(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
		{
		//  get & log xerces error message
			char* excmsg = XMLString::transcode(exc.getMessage());	
			std::string excs="Exception while Parsing: ";
			excs+=excmsg;
			DialogSubsystem::getSingleton().log(excs);
		//  cleanup
			if(parser)delete parser;
			if(xmlHandler)delete xmlHandler;
			throw (exc);
		}
	//  cleanup
		if(parser)delete parser;
		if(xmlHandler)delete xmlHandler;
	}

	/** Destructor
	 *  @author Philipp Walser
	 */
	NaturalLanguageProcessor::~NaturalLanguageProcessor()
	{
		if(mGm)delete mGm;
		DialogSubsystem::getSingleton().log("NLP beendet");
		XMLPlatformUtils::Terminate();
	}

	/** Starts a response to an input message
	 *  Input gets processed & preprocessed, all macthes that fits the respond are returned
	 *  @author Philipp Walser
	 *  @param input The input message, for example a sentence chosen by the player
	 *  @return A string map, containing all matches of the response 
	 */
	std::map<int,std::string> NaturalLanguageProcessor::respond(const std::string& input)
	{
	//  string response has to be const static string, like Nodemater or somthing like that
		string response="<response>", result, pattern;
		string context = "*";
		string topic = "*";
	//  since we do the sentence work, not Predicate
		string that = "*";
		pattern=input;
	//  clear last responses
		mResponses.clear();

		DialogSubsystem::getSingleton().log("Matching...");
		Match *m = mGm->match(context, pattern, that, topic);
	//  get the <template> tag as DOMDocument node
		DOMDocument* doc=(DOMDocument *)m->getNode()->getTemplateNode();
	//  get the content of DOMDocument
		DOMNode* node=doc->getDocumentElement();
		
		DialogSubsystem::getSingleton().log("Processing...");
		response+= process(node, m,"0");	// last Parameter has no function at the moment
		response+="</response>";			// response must be in tags for postprocessing
		doc->release();
	//  Eigentlich muss ein delete doc hier möglich sein, aber seit neustem sorgt dies für einen Absturz
		if(doc)delete doc;
		if(node)delete node;
	//  if a match has triggered the exit/close signal, return with 0 responses.

		if(mExit)
		{
			DialogSubsystem::getSingleton().log("Exit-Signal");
			return mResponses;
		}

		DialogSubsystem::getSingleton().log("PostProcessing...");
		XercesDOMParser* parser=new XercesDOMParser();
		MemBufInputSource memBuff((const XMLByte *)response.data(),response.size(),"response",false);
		parser->parse(memBuff);
	//  make doc independent from parser
		doc=parser->adoptDocument();
		if(parser)delete parser;
		node=doc->getDocumentElement();
		
		int id=0;
		for ( node = node->getFirstChild(); node != NULL; node = node->getNextSibling() )
		{
			if ( node->getNodeType() == DOMNode::ELEMENT_NODE )
			{
		//		string nodeName=XmlHelper::
				string nodeName=AimlParser::transcodeXmlCharToString(node->getNodeName());
				if(!nodeName.compare("li"))
				{
					id=AimlParser::getAttributeValueAsInteger(node->getAttributes(),"id");
				}
			}
			if ( node->getNodeType() == DOMNode::TEXT_NODE )
			{
				string dialogChoice=AimlParser::transcodeXmlCharToString( node->getNodeValue());//((DOMText*)node)->getData());
				mResponses[id]=dialogChoice;
				id=0;
			}
		}
		doc->release();
		DialogSubsystem::getSingleton().log("Delete 1");
		// Eigentlich muss ein delete doc hier möglich sein, aber seit neustem sorgt dies für einen Absturz
		if(doc)delete doc;
		DialogSubsystem::getSingleton().log("Delete 2");
		return mResponses;
	}

	/** Processes option tags from startup xml-file
	 *  @author Philipp Walser
	 *  @param name Option name
	 *  @param value Option value
	 *  @note Maybe this could become an AimlProcessor
	*/
	void NaturalLanguageProcessor::processOption(const std::string& name, const std::string& value) 
	{	
		if ( !name.compare("load") ) 
		{
			std::set<String> sl = ResourceManager::_getAllCommonNamesLike( "./" , ".aiml" );
			std::set<String>::iterator i;
			for (i = sl.begin(); i != sl.end(); i++)
			{
				if(loadAiml((*i).c_str()))
					continue;
			}
		}
	}

	/** Load and AIML file into memory
	 *  @author Philipp Walser
	 *  @param filename AIML filename
	 *  @return False if loading fails
	*/
	bool NaturalLanguageProcessor::loadAiml(const std::string& filename) 
	{
		DialogSubsystem::getSingleton().log("Loading Aiml");
		DialogSubsystem::getSingleton().log(filename);
		AimlParser* xmlHandler=new AimlParser(this);
		SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
		parser->setContentHandler(xmlHandler);
		parser->setErrorHandler(xmlHandler);
		try
		{
			XmlResourceManager::getSingleton().create(filename)->parseBy(parser);
		}
		catch(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
		{
			// get & log xerces error message
			char* excmsg = XMLString::transcode(exc.getMessage());
			std::string excs="Exception while Parsing: ";
			excs+=excmsg;
			DialogSubsystem::getSingleton().log(excs);
			// cleanup
			if(parser)delete parser;
			if(xmlHandler)delete xmlHandler;
			throw (exc);
		}
		// cleanup
		if(parser)delete parser;
		if(xmlHandler)delete xmlHandler;
		return true;
	}

	/** Processes a match
	 *  Translates AIML data to readable output data and executes script commands
	 *  @author Philipp Walser
	 *  @param node The AIML-tag node
	 *  @param match deprecated
	 *  @param id deprecated
	 *  @return processed string 
	*/
	string NaturalLanguageProcessor::process(DOMNode* node, Match* match, const string& id) 
	{
		// We need a start node
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
					string err="Für den Tag "+nodeData+" existiert kein Processor";
					DialogSubsystem::getSingleton().log(err);
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
