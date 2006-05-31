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
#ifndef GOSSIP_PROCESSOR_H
#define GOSSIP_PRCOESSOR_H

#include "XmlMapper/XmlNodeProcessor.h"
using namespace XmlMapper;

namespace MadaBot
{
	/**
	 * Stores a pointer to the data of the gossip element in the response,
	 * for allowing client specific processing of gossip
	 */
	template <class S> class GossipProcessor
		: public XmlNodeProcessor<Response, AimlBot, S, false>
	{
	public:
		/**
		 * Constructor
		 */
		GossipProcessor()
			: XmlNodeProcessor<Response, AimlBot, S, false>("gossip")
		{}

		void preprocessStep()
		{
			mCurrentReturnValue += mCurrentNode;
		}
		void processChildStep(XmlNode<S>* pChild){}
		void postprocessStep(){}
	protected:
		void initialize(){}
	};
}
#endif
