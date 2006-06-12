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
#ifndef BOT_INTERPRETER_H
#define BOT_INTERPRETER_H

#include "XmlMapper/XmlInterpreter.h"
#include "AimlBot.h"

using namespace XmlMapper;

namespace MadaBot
{
	template <class S> class AimlBot;
	template <class S> class AimlCore;

	/**
	 * Interprets a bot definition file and loads a bot in to memory
	 */
	template <class S> class BotInterpreter
		: public XmlInterpreter<AimlBot, AimlCore, S>
	{
	public:
		BotInterpreter() 
			: XmlInterpreter<AimlBot, AimlCore, S>("bot")
		{ 
			mReturnValue = NULL; 
		}
		
		AimlBot<S>* process(XmlNode<S>* pNode, AimlCore<S>* pProcessHelper = NULL);

		AimlBot<S>* interpret(XmlDocument<S>* pDocument, AimlCore<S>* pProcessHelper = NULL);

	protected:
		void initialize(){}
	};

	template <class S> AimlBot<S>* BotInterpreter<S>::process(XmlNode<S>* pNode, AimlCore<S>* pProcessHelper)
	{
	//	XmlInterpreter<void*, AimlBot, S> botloader;
		this->mReturnValue = NULL;
		if(pNode->getNodeName() == "bot")
		{
			this->mReturnValue = new AimlBot<S>(pNode->getAttribute("name"), pProcessHelper);
			this->mReturnValue->setVoice(pNode->getAttribute("voice"));

			XmlNode<S>* child = pNode->getFirstChild();
			for(; child != NULL; child = child->getNextSibling())
			{
				if(isProcessable(child))
				{
					getProcessor(child->getNodeName())->process(child, pProcessHelper);
					continue;
				}
			//  since learning is easier in the interpreter, we do it here and not in a subprocessor
				if(child->getNodeName() == "learn")
				{	
					AimlGraphMaster<S>* gm = pProcessHelper->loadAiml(child->getAttribute("src"));
					if(gm != NULL)
					{
						this->mReturnValue->addGraphMaster(gm);
					}
				}
			//  this is ugly from switching, but nice for handling values for the bot
				else if(child->getNodeName() == "predicates")
				{
				// todo
				// rVal->addPredicate(name, value);
				}
				else if(child->getNodeName() == "substitution")
				{
				//  todo
				//  should be something for AimlCore?
				}
			}
		}
		return this->mReturnValue;
	}

	template <class S> AimlBot<S>* BotInterpreter<S>::interpret(XmlDocument<S>* pDocument, AimlCore<S>* pProcessHelper)
	{
		// do nothing here, use the process-method
		return NULL;
	}
}

#endif
