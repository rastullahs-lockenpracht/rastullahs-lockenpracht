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
#ifndef CATEGORY_PROCESSOR_H
#define CATEGROY_PRCOESSOR_H

#include "XmlMapper/XmlNodeProcessor.h"
#include "AimlNode.h"
using namespace XmlMapper;

namespace MadaBot
{
	template <class S> class GraphPath;
	template <class S> class AimlGraphMaster;

	/**
	 *
	 */
	template <class S> class CategoryProcessor
		: public XmlNodeProcessor<GraphPath, AimlGraphMaster, S>
	{
	public:
		/**
		 * Constructor
		 */
		CategoryProcessor()
			: XmlNodeProcessor<GraphPath, AimlGraphMaster, S>("category")
		{
			this->mCurrentReturnValue = NULL;
			initialize();
		}

		void preprocessStep()
		{
			this->mCurrentReturnValue = new GraphPath<S>();
		}
		
		void processChildStep(XmlNode<S>* pChild)
		{
		//  no further subprocessors needed, this is not likely to change
		//  on the other hand, the interpretation of the pattern-tag could differ
		//  TODO: the comparison should not be case sensitive
			XmlNode<S>* subChild = pChild->getFirstChild();
			if(pChild->getNodeName() == "pattern" && subChild->getNodeName() == "#text")
			{
				this->mCurrentReturnValue->setPattern(subChild->getNodeValue());
			}
			else if(pChild->getNodeName() == "that" && subChild->getNodeName() == "#text")
			{
				this->mCurrentReturnValue->setThat(subChild->getNodeValue());
			
			//  what was this for? i guess it is deprecated
			//  mCurrentReturnValue->setPathStep(pChild->getNodeName(), subChild->getNodeValue());
			}
			else if(pChild->getNodeName() == "template")
			{
				this->mCurrentReturnValue->setTemplateValue(static_cast<AimlNode<S>*>(pChild));
			//  remove template-child from the document, 
			//  because it is now used by the GraphMaster
				this->mCurrentNode->removeChild(pChild);
			}
		}
		void postprocessStep(){}
	protected:
		void initialize()
		{
			this->mSubProcessors.push_back("pattern");
			this->mSubProcessors.push_back("that");
			this->mSubProcessors.push_back("template");
		}
	};
}
#endif
