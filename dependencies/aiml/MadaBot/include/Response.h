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
#ifndef RESPONSE_H
#define RESPONSE_H

//#include "XmlMapper/XmlNode.h"

namespace XmlMapper
{
	template <class S> class XmlNode;
}
using namespace XmlMapper;
namespace MadaBot
{
	/**
	 * The processed Response of an input
	 */
	template <class S> class Response
	{
	public:
		typedef std::vector<XmlNode<S>*> GossipData;
		Response() {}
		Response(const Response& pData)
		{
			mResponse = pData.mResponse;
			mChildNodes = pData.mChildNodes;
		}

		void clear()
		{
			mResponse.clear();
			mChildNodes.clear();
		}
		bool empty() const
		{
			return (mRespone.empty() && mChildNodes.empty());
		}
		bool hasGossip() const
		{
			return !mChildNodes.empty();
		}
		S getResponse() const
		{
			return mResponse;
		}
		const GossipData& getGossip() const
		{
			return mChildNodes;
		}
		Response& operator+=(const S& pData)
		{
			mResponse += pData;
			return *this;
		}
		Response& operator+=(XmlNode<S>* pData)
		{
			mChildNodes.push_back(pData);
			return *this;
		}
		Response& operator+=(const Response& pData)
		{
			mResponse += pData.getResponse();
			GossipData::const_iterator itr = pData.mChildNodes.begin();
			for(; itr != pData.mChildNodes.end(); ++itr)
			{
				mChildNodes.push_back(*itr);
			}
			return *this;
		}
	private:
		S mResponse;
		GossipData mChildNodes;
	};
}
#endif
