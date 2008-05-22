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
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>

#include "XmlHelper.h"
#include "DialogSubsystem.h"
#include "Utils.h"
#include "Graphmaster.h"
#include "AimlProcessorManager.h"
#include "AimlProcessor.h"
#include "AimlParserImplXerces.h"
#include "Nodemaster.h"
#include "Match.h"
#include "DialogResponse.h"
#include "NaturalLanguageProcessor.h"

//#include "Substituter.h"
//#include "Predicates.h"
//#include "StringTokenizer.h"

#include "XmlResourceManager.h"
#include "OgreResourceGroupManager.h"
#include <OgreLogManager.h>

using namespace Ogre;

XERCES_CPP_NAMESPACE_USE
using namespace Ogre;

namespace rl
{
	/** Constructor
	 *  Parses through the given dialog startup file and loads the AIML
	 *  @author Philipp Walser
	 *  @param dialogFile The startup xml-file, contains information about a specific NPC
	 *
	*/
	/* 
		In Zukunft wird in der AIML-Node vom Nodemater direkt eine Referenz auf eine DOMNode
		gespeichert, so dass die templates nicht nochmal extra auseinandergenommen werden müssen.
		Dann erspart man sich auch das ständige ISO-gebimsel, IMHO.
		Graphmaster: Matched. Nodemaster: Enthält AIML-Nodes, bzw. ist eine AIML-Node. 
		Wozu master? Master=Mapper laut AIMLParser-Draft. Nodemapper hat string zur Erkennung
		der seinen "PATH" angibt, und eine AIMLNode, die eigentlich eine DOMNode ist.
		Nach dem Laden einer AIML-File wird adoptDocument gemacht, der parser gelöscht und die 
		file im resourceManager wieder ungeloaded. Das DOM-Document im Speicher wird traversiert
		und an schlüsselstellen werden referenzen auf templates hinterlassen. allerdings muss
		dann noch eine abbruch-bedingung implementiert werden beim processing, damit nicht auf
		einmal aus versehen durch das ganze Dokument traversiert wird. <-- kein Problem:
		Einfach nur die siblings von template abarbeiten
	*/

	const static string ISO="<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
/*
	NaturalLanguageProcessor::NaturalLanguageProcessor(const CeGuiString& filename): 
		mGm(NULL),
		mExit(false)
	{	
		XMLPlatformUtils::Initialize();	//wahrscheinlich nicht nötig
		Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "NLP gestartet");
	//	mExit=false;

	// Create a new Parser for aiml files and register it as content handler
	// for the sax parser
		AimlParserImplXerces* xmlHandler=new AimlParserImplXerces(this);
		SAX2XMLReader* parser =XMLReaderFactory::createXMLReader();
		parser->setContentHandler(xmlHandler);
		parser->setErrorHandler(xmlHandler);

	//  do parse (which uses handler to create actual data)
		try
		{
			XmlPtr res = 
                XmlResourceManager::getSingleton().create(
                filename, 
                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
            res.getPointer()->parseBy(parser);
		}
		catch(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
		{
		//  get & log xerces error message
			char* excmsg = XMLString::transcode(exc.getMessage());	
			CeGuiString excs="Exception while Parsing: ";
			excs+=excmsg;
			Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, excs);
		//  cleanup
			if(parser)delete parser;
			if(xmlHandler)delete xmlHandler;
			throw (exc);
		}
	//  cleanup
		if(parser)delete parser;
		if(xmlHandler)delete xmlHandler;
				Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "Parsing beendet");
	}
*/
	/** 
	 * Constructor
	 * @author Philipp Walser
	 */
	NaturalLanguageProcessor::NaturalLanguageProcessor()
	{
	}

    /** Destructor
    *  @author Philipp Walser
    */
    NaturalLanguageProcessor::~NaturalLanguageProcessor()
    {
     //   if(mGm)delete mGm;
        Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "NLP beendet");
        XMLPlatformUtils::Terminate();
    }

	void NaturalLanguageProcessor::addGraphMaster(Graphmaster* gm)
	{
		if(gm)
		{
			mGraphList.push_back(gm);
		}
	}

	/** 
	 * Starts a response to an input message
	 * Input gets processed & preprocessed, all macthes that fits the respond are returned
	 * @author Philipp Walser
	 * @param input The input message, for example a sentence chosen by the player
	 * @return A string map, containing all matches of the response 
	 */
	//NaturalLanguageProcessor::Responses& NaturalLanguageProcessor::respond(const CeGuiString& input)
	DialogResponse* NaturalLanguageProcessor::createResponse(const CeGuiString& input)
	{
		// prepare response-string (needs xml-tags for postprocessing)
		CeGuiString response = ISO+"<response>", result;
		// initialize strings for graphmaster pathing
		string context = "*";
		string topic = "*";
		string that = "*";	//  since we do the sentence work, not Predicate
	
		// clear last responses
		mCurrentResponses.clear();

		Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "Matching...");
		Match* newMatch = match(context, input, that, topic);
		if(newMatch == NULL)
		{
			return NULL;
		}

		// get the <template> tag as DOMDocument node
		DOMDocument* doc=static_cast<DOMDocument*>(newMatch->getNode()->getTemplateNode());
		// get the content of DOMDocument
		DOMNode* node=doc->getDocumentElement();

		Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "Processing...");
		response+= process(node, newMatch ,"0");	// last Parameter has no function at the moment
		response+="</response>";			// response must be in tags for postprocessing
		
		//Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "Cleanup");
		// free the memory of the document and all its nodes
		doc->release();
		doc = NULL;
		node = NULL;
	
		Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "PostProcessing...");
		XercesDOMParser* parser=new XercesDOMParser();
		// copy the response data into a memory buffer for postprocessing
		MemBufInputSource memBuff((const XMLByte*)response.data(),response.size(),"response",false);
		parser->parse(memBuff);
		// get doc from parser and get first node
		doc = parser->getDocument(); //->adoptDocument();
		node = doc->getDocumentElement();

		int id = 0;
		bool isListItem = false;
		CeGuiString retResponse;
		Responses selectableOptions;
		for ( node = node->getFirstChild(); node != NULL; node = node->getNextSibling() )
		{
			if ( node->getNodeType() == DOMNode::ELEMENT_NODE )
			{
				CeGuiString nodeName = 
					XmlHelper::transcodeToString(node->getNodeName());

				if(nodeName == "li")
				{
					isListItem = true;
					id = XmlHelper::getAttributeValueAsInteger( (DOMElement*)node, "id" );
				}
				else if(nodeName == "option")
				{
				//	id = XmlHelper::getAttributeValueAsInteger( (DOMElement*)node, "id" );
					int indexBegin = response.find("<option");
					int indexEnd = response.find("</option>") + 9 - indexBegin;
					if(indexBegin != string::npos && indexEnd != string::npos)
					{
						selectableOptions[id] = response.substr(indexBegin, indexEnd);
						selectableOptions[id].data();
					}
				}
				else
				{
					isListItem = false;
				}
			}
			if ( node->getNodeType() == DOMNode::TEXT_NODE )
			{
				CeGuiString dialogChoice = 
					XmlHelper::transcodeToString(node->getNodeValue());
				if(isListItem)
				{
					mCurrentResponses[id] = dialogChoice + "\n ";
				}
				else
				{
					retResponse += dialogChoice;
				}
				//Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, dialogChoice);
				//id=0;
			}
		}

		if(parser)delete parser;
		Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "Return response");
		return new DialogResponse(input, retResponse, mCurrentResponses, 
			selectableOptions, this);
	//	return mCurrentResponses;
	}

	Match* NaturalLanguageProcessor::match( const CeGuiString& context, 
											const CeGuiString& input, 
											const CeGuiString& that, 
											const CeGuiString& topic)
	{
		Match* match = NULL;

		std::vector<Graphmaster*>::iterator iter = mGraphList.begin();
		for(; iter != mGraphList.end(); ++iter)
		{
			match = (*iter)->match(context, input, that, topic);
			if(match) return match;
		}
		return NULL;
	}

	/** Processes option tags from startup xml-file
	 *  @author Philipp Walser
	 *  @param name Option name
	 *  @param value Option value
	 *  @note Maybe this could become an AimlProcessor
	*/
/*	void NaturalLanguageProcessor::processOption(const CeGuiString& name, const CeGuiString& value) 
	{	
		if(mGm)delete mGm;
		Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "graphmaster deleted ");
		mGm=new Graphmaster();
		if ( !name.compare("load") ) 
		{
	        StringVectorPtr sl = ResourceGroupManager::getSingleton().findResourceNames(
                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "*.aiml" );
            StringVector::iterator i;
            for (i = sl.get()->begin(); i != sl.get()->end(); i++)
            {
                if(loadAiml((*i).c_str()))
                    continue;
			}
		}
	}
*/

	/** Load and AIML file into memory
	 *  @author Philipp Walser
	 *  @param filename AIML filename
	 *  @return False if loading fails
	*/
/*	bool NaturalLanguageProcessor::loadAiml(const CeGuiString& filename) 
	{
		Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "Loading Aiml");
		Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, filename);
		AimlParserImplXerces* xmlHandler = new AimlParserImplXerces(this);
		SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
		parser->setContentHandler(xmlHandler);
		parser->setErrorHandler(xmlHandler);
		
		try
		{	
			if(XmlResourceManager::getSingleton().getByName(filename).isNull())
			{
				XmlPtr res = 
					XmlResourceManager::getSingleton().create(
					filename, 
					ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
				res.getPointer()->parseBy(parser);
			}
			else
			{
				XmlPtr res = XmlResourceManager::getSingleton().getByName(filename);
				res.getPointer()->parseBy(parser);
			}

		}
		catch(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
		{
			// get & log xerces error message
			char* excmsg = XMLString::transcode(exc.getMessage());
			CeGuiString excs="Exception while Parsing: ";
			excs+=excmsg;
			Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, excs);
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
*/
	/** Processes a match
	 *  Translates AIML data to readable output data and executes script commands
	 *  @author Philipp Walser
	 *  @param node The AIML-tag node
	 *  @param match deprecated
	 *  @param id deprecated
	 *  @return processed string 
	*/
	CeGuiString NaturalLanguageProcessor::process(DOMNode* node, Match* match, const CeGuiString& id) 
	{	
		// We need a start node
		if ( node == NULL ) return "";
		//Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "StartProcessingCurrentNode");
		CeGuiString result;
		CeGuiString text;
		CeGuiString nodeData;
		bool lastWasElement = false;
		bool lastTailIsWS = false;
		bool nextHeadIsWS = false;
		for ( node = node->getFirstChild(); node != NULL; node = node->getNextSibling() )
		{
			if ( node->getNodeType() == DOMNode::TEXT_NODE ) 
			{
				//--	fix whitespace here
				result += getTextData(
					XmlHelper::transcodeToString(
							static_cast<DOMText*>(node)->getData()));
				lastWasElement = false;
			} 
			else if ( node->getNodeType() == DOMNode::CDATA_SECTION_NODE ) 
			{
				//--	what to do about CDATA???	
				nodeData = XmlHelper::transcodeToString(
								static_cast<DOMText*>(node)->getData()); // Attention: Cast to CDATA, not to DOMText*!
				result += nodeData;
			} 
			else if ( node->getNodeType() == DOMNode::ELEMENT_NODE ) 
			{
				nodeData = 
					XmlHelper::transcodeToString(node->getNodeName());
				
				//Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, nodeData);
				
				// get available tag processor
				AimlProcessor* pt = AimlProcessorManager::getProcessor(nodeData);
				if ( pt == NULL ) 
				{
					if(nodeData == "response")
					{
						lastWasElement = true;
						lastTailIsWS = false;
						//--	add next element
						text = process(node, match, id);
						if ( !result.empty() && *(--result.end()) != ' ' && lastTailIsWS ) 
						{
							result += ' ';
						}
						result += text;
					}
					else
					{
						CeGuiString err = "Für den Tag " + nodeData + " existiert kein Processor";
						Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, err);
						text = process(node, match, id);
						if ( !result.empty() && *(--result.end()) != ' ' && lastTailIsWS ) 
						{
							result += ' ';
						}
						result += text;
						lastWasElement = true;
						lastTailIsWS = false;
					}
				} 
				else 
				{
					//Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, "Found AimlProcessor");
					text = pt->process(node, match, id, this);
					//Logger::getSingleton().log(Logger::DIALOG, Ogre::LML_TRIVIAL, text);
					//--fix whitespace here
					//--	if last was not an element
					//--		if last text tail was whitespace
					//--			add ' ' before next element
					if ( !result.empty() && *(--result.end()) != ' ' && lastTailIsWS )
					{
						result += ' ';
					}
					//--	add next element
					result += text;
					lastWasElement = true;
					lastTailIsWS = false;
				}
			}	//--	end else if node->getNodeType { ignore other content }
		} // end for-loop
		return result;
	}
	void NaturalLanguageProcessor::setName(const CeGuiString& name)
	{
		mName = name;
	}
	const CeGuiString& NaturalLanguageProcessor::getName() const 
	{
		return mName;
	}

	CeGuiString NaturalLanguageProcessor::getTextData(const CeGuiString& nodeData)
	{
		CeGuiString result;
		bool lastTailIsWS = false, nextHeadIsWS = false, lastWasElement = false;
		if ( normalise(nodeData).empty() ) 
		{
			lastTailIsWS = !nodeData.empty();
			return "";
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
		return result;
	}
} // Namespace rl end
