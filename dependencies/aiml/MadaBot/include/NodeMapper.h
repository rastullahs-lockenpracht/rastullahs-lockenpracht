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
#ifndef NODE_MAPPER_H
#define NODE_MAPPER_H

#include <deque>
#include "AimlUtility.h"
#include "GraphPath.h"

namespace MadaBot
{
	/**
	 * Could inherit from std::pair (<key,value>+children)
	 */
	template <class S> class NodeMapper
	{
	public:
		NodeMapper() : mKey(AimlUtility::UNDERSCORE) {}
		NodeMapper(const S& pKey, AimlNode<S>* pTemplate = NULL)
			: mKey(pKey), mTemplateValue(pTemplate)
		{
		}

		~NodeMapper()
		{
			if(mTemplateValue)
			{
				delete mTemplateValue;
				mTemplateValue = NULL;
			}
			typename NodeList::iterator itr = mChildren.begin();
			for(; itr != mChildren.end(); ++itr)
			{
				delete (*itr);
			}
			mChildren.clear();
		}

		void addChildNode(NodeMapper<S>* pNode)
		{
			// add the node before the "*" value
			if(mChildren.empty())
			{
				mChildren.push_back(pNode);
			}
			else
			{
				typename NodeList::iterator itr = mChildren.end();
				--itr;
				mChildren.insert(itr, pNode);
			}
		}

		NodeMapper* getChildNode(const S& pKey )
		{
			if ( pKey.empty() ) return NULL; // return cached;
			typename NodeList::iterator itr = mChildren.begin();
			for(; itr != mChildren.end(); ++itr)
			{
				if(pKey == (*itr)->getKey() )
				{
					return (*itr);
				}
			}
			return NULL;
		}
		bool hasTemplate()
		{
			return (mTemplateValue != NULL);
		}
		void setTemplate(AimlNode<S>* pTemplate)
		{
			mTemplateValue = pTemplate;
		}

		AimlNode<S>* getTemplate()
		{
			return mTemplateValue;
		}

		const S& getKey() { return mKey; }


	private:
		typedef std::deque<NodeMapper<S>*> NodeList;
		const NodeList& getChildren(){ return mChildren; }
		void initialize(){}
		// identifier
		S mKey;
		// AimlData
		AimlNode<S>* mTemplateValue;
		// ChildData
		NodeList mChildren;
	};
}
#endif
