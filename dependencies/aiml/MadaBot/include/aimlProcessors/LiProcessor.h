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
#ifndef LI_PROCESSOR_H
#define LI_PRCOESSOR_H

#include "XmlMapper/XmlNodeProcessor.h"
#include "Response.h"

using namespace XmlMapper;

namespace MadaBot
{
	template <class S> class AimlBot;

	/**
	 *
	 */
	template <class S> class LiProcessor
		: public XmlNodeProcessor<Response, AimlBot, S, false>
	{
	public:
		/**
		 * Constructor
		 */
		LiProcessor()
			: XmlNodeProcessor<Response, AimlBot, S, false>("li")
		{ initialize(); }

		void preprocessStep()
		{
			this->mCurrentReturnValue.clear();
		}
		void processChildStep(XmlNode<S>* pChild)
		{
			this->mCurrentReturnValue += getProcessor(pChild->getNodeName())->process(pChild, this->mCurrentHelper);
		}
		void postprocessStep(){}
	protected:
		void initialize()
		{
		//  allow any nodeType as child
			this->mRestrictSubProcessors = false;
		}
	};
}
#endif
