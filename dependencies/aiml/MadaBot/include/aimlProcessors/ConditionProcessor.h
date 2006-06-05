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
#ifndef CONDITION_PROCESSOR_H
#define CONDITION_PRCOESSOR_H

#include "XmlMapper/XmlNodeProcessor.h"
using namespace XmlMapper;

#include "Predicates.h"

namespace MadaBot
{
	/**
	 *
	 */
	template <class S> class ConditionProcessor
		: public XmlNodeProcessor<Response, AimlBot, S, false>
	{
	public:
		/**
		 * Constructor
		 */
		ConditionProcessor()
			: XmlNodeProcessor<Response, AimlBot, S, false>("condition")
		{}

		void preprocessStep();
		void processChildStep(XmlNode<S>* pChild);
		void postprocessStep(){}
	protected:
		void initialize(){}
		S mPredicateValue;
		S mNodeValue;
	};

	template <class S> void ConditionProcessor<S>::preprocessStep()
	{
		mPredicateValue.clear();
		mNodeValue.clear();
		try
		{
			mPredicateValue = mCurrentHelper
				->getPredicates(mAttributes["type"])
				->getPredicate(mAttributes["name"]);
			if(!mAttributes["value"].empty())
			{
				mNodeValue = mAttributes["value"];
			}
		}
		catch(...)
		{
		}
	}

	template <class S> void ConditionProcessor<S>::processChildStep(XmlNode<S>* pChild)
	{
		if(mNodeValue.empty())
		{
			mNodeValue = pChild->getAttribute("value");
		}
		if(mPredicateValue == mNodeValue)
		{
			mCurrentReturnValue = getProcessor(pChild->getNodeName())->process(pChild);
		//  standard aiml returns only one li-element	
			mProcessChildren = false;
		}
	}
}
#endif
