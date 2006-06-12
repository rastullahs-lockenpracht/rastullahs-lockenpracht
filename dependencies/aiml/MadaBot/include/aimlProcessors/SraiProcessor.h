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
#ifndef SRAI_PROCESSOR_H
#define SRAI_PRCOESSOR_H

#include "XmlMapper/XmlNodeProcessor.h"
#include "AimlBot.h"

using namespace XmlMapper;

namespace MadaBot
{
	template <class S> class SraiProcessor 
		: public XmlNodeProcessor<Response, AimlBot, S, false> 
	{
	public:
		/**
		 * Constructor
		 */
		SraiProcessor()
			: XmlNodeProcessor<Response, AimlBot, S, false>("srai")
		{
			initialize();
		}

		void preprocessStep()
		{
			this->mCurrentReturnValue.clear();
		}

		void processChildStep(XmlNode<S>* pChild)
		{
			this->mCurrentReturnValue += getProcessor(pChild->getNodeName())->process(pChild);
		}

		void postprocessStep()
		{
			this->mCurrentReturnValue = this->mCurrentHelper->respond(this->mCurrentReturnValue.getResponse());
		}
	protected:
		void initialize()
		{
		//  allow any nodeType as child
			this->mRestrictSubProcessors = false;
		}
	};
}
#endif
