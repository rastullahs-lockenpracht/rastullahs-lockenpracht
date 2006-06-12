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
#ifndef AIML_INTERPRETER_H
#define AIML_INTERPRETER_H

#include "XmlMapper/XmlInterpreter.h"
#include "XmlMapper/XmlDocument.h"
#include "XmlMapper/XmlNode.h"
#include "aimlProcessors/TextProcessor.h"
#include "aimlProcessors/GossipProcessor.h"
#include "aimlProcessors/ConditionProcessor.h"
#include "aimlProcessors/LiProcessor.h"
#include "aimlProcessors/BrProcessor.h"
#include "aimlProcessors/GetProcessor.h"
#include "aimlProcessors/SetProcessor.h"
#include "aimlProcessors/SraiProcessor.h"

using namespace XmlMapper;

namespace MadaBot
{
	template <class S> class Response;
	template <class S> class AimlBot;

	/**
	 * Interpretes AIML through different processors
	 */
	template <class S> class AimlInterpreter 
		: public XmlInterpreter<Response, AimlBot, S, false> 
	{
	public:
		/**
		 * Constructor
		 */
		AimlInterpreter(const S& pName="aiml") : XmlInterpreter<Response, AimlBot, S, false>(pName)
		{
			initialize();
		} 
		
		/**
		 * Destructor
		 */
		virtual ~AimlInterpreter(){}

		/**
		 * Process the different AIML elements
		 * @param  pNode node wich contains an AIML element with its attributes and its values
		 * @param  pProcessHelper For special processing abilities, an AimlGraphMaster serves as helper object
		 */
		virtual Response<S> process(XmlNode<S>* pNode, AimlBot<S>* pProcessHelper=NULL);
		
		/**
		 * Interpret an AIML document
		 * @param  pDocument AIML document that should get interpreted
		 * @param  pProcessHelper For special processing abilities, an AimlGraphMaster serves as helper object
		 */
		Response<S> interpret(XmlDocument<S>* pDocument, AimlBot<S>* pProcessHelper=NULL)
		{
			this->mReturnValue.clear();
			return this->mReturnValue;
		}	
			
	protected:
		/**
		 * Initialize the Interpreter. This method should be called from the constructor.
		 */
		virtual void initialize()
		{
			addProcessor(new TextProcessor<S>());
			addProcessor(new GossipProcessor<S>());
			addProcessor(new ConditionProcessor<S>());
			addProcessor(new LiProcessor<S>());
			addProcessor(new BrProcessor<S>());
			addProcessor(new GetProcessor<S>());
			addProcessor(new SetProcessor<S>());
			addProcessor(new SraiProcessor<S>());
		}
	};


	template <class S> Response<S> AimlInterpreter<S>::process(XmlNode<S>* pNode, AimlBot<S>* pProcessHelper)
	{
		this->mReturnValue.clear();
	//  for every child node of "template", add its processed value to the response
		pNode = pNode->getFirstChild();
		for(; pNode != NULL; pNode = pNode->getNextSibling())
		{
			if(isProcessable(pNode))
			{
				this->mReturnValue += getProcessor(pNode->getNodeName())
					->process(pNode, pProcessHelper);
			}
		}
		return this->mReturnValue;
	}
}

#endif
