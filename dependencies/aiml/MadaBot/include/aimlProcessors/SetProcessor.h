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
#ifndef SET_PROCESSOR_H
#define SET_PRCOESSOR_H

#include "XmlMapper/XmlNodeProcessor.h"
#include "AimlBot.h"
#include "Predicates.h"

using namespace XmlMapper;

namespace MadaBot
{
	template <class S> class SetProcessor 
		: public XmlNodeProcessor<Response, AimlBot, S, false> 
	{
	public:
		/**
		 * Constructor
		 */
		SetProcessor()
			: XmlNodeProcessor<Response, AimlBot, S, false>("set")
		{ initialize();}

		void preprocessStep()
		{
		//	don't process children if the value to set is given by attribute
			if(this->mAttributes["value"].empty())
			{
				this->mProcessChildren = true;
			}
			else
			{
				this->mProcessChildren = false;
			}
		}

		void processChildStep(XmlNode<S>* pChild)
		{
			this->mCurrentReturnValue += getProcessor(pChild->getNodeName())->process(pChild);
		}

		void postprocessStep()
		{
			S value;
			if(this->mAttributes["value"].empty())
			{
				value = this->mCurrentReturnValue.getResponse();
			}
			else
			{
				value = this->mAttributes["value"];
			}
			this->mCurrentHelper->getPredicates(this->mAttributes["type"])
				->setPredicate(this->mAttributes["name"], value);
		//  don't return anything
			this->mCurrentReturnValue.clear();
		}
	protected:
		void initialize()
		{
			this->addAttribute("type");
			this->addAttribute("name");
			this->addAttribute("value");
		//  allow any nodeType as child
			this->mRestrictSubProcessors = false;
		}
	};
}
#endif
