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
#ifndef GRAPH_INTERPRETER_H
#define GRAPH_INTERPRETER_H

#include "XmlMapper/XmlInterpreter.h"

#include "aimlProcessors/CategoryProcessor.h"
#include "aimlProcessors/TopicProcessor.h"
#include "GraphPath.h"

using namespace XmlMapper;

namespace MadaBot
{
	template <class S> class AimlGraphMaster;

	/**
	 * Build up the GraphMaster
	 */
	template <class S> class GraphInterpreter
		: public XmlInterpreter<GraphPath, AimlGraphMaster, S> 
	{
	public:
		/**
		 * Constructor
		 */
		GraphInterpreter(const char* pName)
			: XmlInterpreter<GraphPath, AimlGraphMaster, S>(pName)
		{
		//  TODO: returnType does not need to be polymorph
			this->mReturnValue = NULL;
			initialize();
		}
		
		GraphPath<S>* interpret(XmlDocument<S>* pDocument, AimlGraphMaster<S>* pProcessHelper = NULL);
		
		GraphPath<S>* process(XmlNode<S>* pNode, AimlGraphMaster<S>* pProcessHelper=NULL);


			
	protected:
		virtual void initialize();

	};

	template <class S> void GraphInterpreter<S>::initialize()
	{
		this->addProcessor(new TopicProcessor<S>());
		this->addProcessor(new CategoryProcessor<S>());
	}

	template <class S> GraphPath<S>* GraphInterpreter<S>::interpret(XmlDocument<S>* pDocument, AimlGraphMaster<S>* pProcessHelper)
	{
		if(pDocument != NULL)
		{
			XmlNode<S>* root = pDocument->getRootNode();
			XmlNode<S>* child = root->getFirstChild();
			process(child, pProcessHelper);
		}
		return NULL;
	}

	template <class S> GraphPath<S>* GraphInterpreter<S>::process(XmlNode<S>* pNode, AimlGraphMaster<S>* pProcessHelper)
	{
	//  process every child noe of the file, create a graphpath and add it to the graphmaster
		for(; pNode != NULL; pNode = pNode->getNextSibling())
		{
			if(isProcessable(pNode))
			{
				this->mReturnValue =  getProcessor(pNode->getNodeName())
					->process(pNode, pProcessHelper);
			//  TODO: if pProcessHelper==NULL throw exception!
				pProcessHelper->add(*mReturnValue);
				this->mReturnValue->reset();
				delete (this->mReturnValue);
				this->mReturnValue = NULL;
			}
		}
		return NULL;
	}
}
#endif
