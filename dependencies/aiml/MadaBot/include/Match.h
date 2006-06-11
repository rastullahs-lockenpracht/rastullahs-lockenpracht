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
#ifndef MATCH_H
#define MATCH_H

#include <vector>
#include "AimlUtility.h"

template <class S> class AimlNode;
namespace MadaBot
{
	/**
	 * Represents a response match for a given input
	 */
	template <class S> class Match
	{
		public:
			Match(){}
			~Match(){}
		
			void addStar(const S& pStar, AimlUtility::PathSeperators pState)
			{
			//  values added in reverse order due to Graphmaster design
				mStars[pState].push_back(pStar);
			}
			S& getStar(unsigned int index, AimlUtility::PathSeperators pState) const
			{
			//  only need to check index > size (due to unsigned int)
				std::vector<S>& tmp = mStars[pState];
				size_t size = tmp.size();
			//  ++index accounts for default first item (why is it there?)
				if ( ++index > size ) 
				{
				//  throw an exception?
					return "";
				}
			//  account for reversed order (see addStar)
				return tmp[size - index];
			}
		
			void setNode(AimlNode<S>* pNode)
			{
				mNode = pNode;
			}
            
			AimlNode<S>* getNode()
			{
				return mNode;
			}
		
		private:
			std::vector<S> mStars[4];
			AimlNode<S>* mNode;
	};
}
#endif
