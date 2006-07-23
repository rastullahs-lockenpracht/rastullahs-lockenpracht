/**
 * Copyright (c) 2006 Philipp Walser
 * 
 * Permission is hereby granted, free of charge, to any person obtaining 
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Software.
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE 
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef AIML_CORE_H
#define AIML_CORE_H

#include <map>
#include "XmlMapper/XmlParser.h"
#include "XmlMapper/XmlDocument.h"
#include "BotInterpreter.h"
#include "GraphInterpreter.h"
#include "AimlGraphMaster.h"

namespace MadaBot
{
	template <class S> class AimlBot;
	template <class S> class AimlInterpreter;

	/**
	 * Core component of the AimlInterpeter that allows loading of configuration files, bots
	 * and aiml
	 */
	template <class S> class AimlCore
	{
	public:
		/** 
		 * Constructor 
		 */
		AimlCore() 
			: mBotDefintion("dialog-startup.xml"), 
			  mParser(NULL), 
			  mCurrentBot(NULL)
		{}

		virtual ~AimlCore() 
		{ 
			if(mParser)
			{
				delete mParser; 
				mParser = NULL;
			}
			for(GraphList::iterator itr = mGraphList.begin(); itr != mGraphList.end(); ++itr)
			{
				delete itr->second;
			}
			mGraphList.clear();
		}

		
		AimlInterpreter<S>& getAimlInterpreter()
		{
			return mAimlInterpreter;
		}

		BotInterpreter<S>& getBotInterpreter()
		{
			return mBotInterpreter;
		}
		
		/**
		 * Get the GraphMaster with the given name. 
		 * @param  name  name of the GraphMaster
		 * @return Return the GraphMaster or NULL when the Graphmaster with the given name does not exists
		 */
		AimlGraphMaster<S>* getGraphMaster(const char* name);
		
		/**
		 * load Aiml file
		 * @param  fileName name of the aiml resource
		 * @return A GraphMaster created from the AIML document
		 */
		virtual AimlGraphMaster<S>* loadAiml(const S& pFileName);
		
		/** 
		 * loads a botDefinition
		 * @param  fileName name of the bot configuration file
		 * @return true if successfully loaded
		 * TODO: implement
		 */
		virtual bool setStandardBotDefinition(const char* fileName){return false;}
		
		/**
		 * loads a bot from a botDefinition
		 * @param  botName  the name of the bot that can be found in the bot definition
		 * @param  fileName name of the botDeifnition file. Uses the standard fileName
		 *                  from loadBotDefintion() if empty
		 * @return The Bot
		 * @see loadBotDefintion(const char* fileName)
		 */
		virtual AimlBot<S>* loadBot(const char* botName, const char* fileName=NULL);

		/**
		 * Set the parser that should be used for parsing xml files.
		 * AimlCore is responsible for memory management of the parser
		 * @param  pParser parser for xml files
		 */
		void setParser(XmlParser<S>* pParser) { mParser = pParser; }

	private:
		S mBotDefintion;
		typedef typename std::map<S, AimlGraphMaster<S>*> GraphList;
		GraphList mGraphList;
		XmlParser<S>* mParser;
		AimlInterpreter<S> mAimlInterpreter;
		BotInterpreter<S> mBotInterpreter;
		AimlBot<S>* mCurrentBot;

	};

	template <class S> AimlGraphMaster<S>* AimlCore<S>::loadAiml(const S& pFileName)
	{
		// if the GraphMaster for the given fileName already exists, return it
		typename GraphList::iterator itr = mGraphList.find(pFileName);

		if(itr != mGraphList.end())
		{
			return itr->second;
		}

		XmlDocument<S>* doc = NULL;
		try
		{
			doc = mParser->parse(pFileName);

			// interpret the parsed document, use the GraphMaster as processHelper
			// and add it to the graphlist
			AimlGraphMaster<S>* graph = new AimlGraphMaster<S>(pFileName);
			GraphInterpreter<S>("GraphInterpreter").interpret(doc, graph);
			itr = mGraphList.insert(
				typename GraphList::value_type(pFileName, graph)).first;

			if(doc)
				delete doc;
			return itr->second;
		}
		catch(...)
		{
		//  TODO:exceptionhandling
		}
		if(doc)
			delete doc;
		return NULL;
	}

	template <class S> AimlBot<S>* AimlCore<S>::loadBot(const char* pBotName, const char* pFileName)
	{
		S botName = pBotName;
		// check if standard bot definition should be used or the parameter value
		S botDef = mBotDefintion;
		if(pFileName != NULL)
		{
			botDef = pFileName;
		}
		XmlDocument<S>* doc = NULL;
		try
		{
			doc = mParser->parse(pFileName);
			if(doc == NULL)
			{
				//throw exception
			}
			XmlNode<S>* botNode = doc->getRootNode()->getFirstChild();
			for(; botNode != NULL; botNode = botNode->getNextSibling())
			{
				if(botNode->getAttribute("name") == botName || botName.empty() )
				{
				//  this shouldn't be parsed by a DOM parser (too much overhead), but a SAX parser
					AimlBot<S>* rVal = mBotInterpreter.process(botNode, this);
					delete doc;
					return rVal;
				}
			}
			//  TODO: exception or log message, because no bot was found
		}
		catch(...)
		{
		//  exception...
		}
		if(doc)
			delete doc;
		return NULL;
	}
}
#endif
