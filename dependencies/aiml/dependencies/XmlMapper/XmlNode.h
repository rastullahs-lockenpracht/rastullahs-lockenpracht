/**
 * Copyright (c) 2005-2006 Philipp Walser
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
/**
 * @file XmlNode.h
 *
 * @author	Philipp Walser
 * @version 1.3
 * @date	12/01/2006
 */
#ifndef XML_NODE_H
#define XML_NODE_H

#include <vector>

namespace XmlMapper
{
	template<class S> class XmlNode
	{
	public:
		XmlNode() : mNextSibling(NULL) {}
		virtual ~XmlNode();

		virtual void addChild(XmlNode* pChild){ mChildren.push_back(pChild); }

		virtual S getAttribute(const S& pName)=0;
		virtual XmlNode* getFirstChild()  { return !mChildren.empty() ? mChildren[0] : NULL; }
		virtual XmlNode* getNextSibling() { return mNextSibling; }

		virtual S getNodeName()=0;
		virtual S getNodeValue()=0;
		virtual void setNodeValue(const S& v)=0;

		//virtual XmlString getNodeString(){}
		
		virtual bool hasAttributes()=0;
		virtual bool isElement()=0;

		// should be protected
		void setNextSibling(XmlNode* pNextSibling);
		bool removeChild(XmlNode* pChild);

	protected:
		std::vector<XmlNode*> mChildren;
		XmlNode* mNextSibling;
	};

	template<class S> XmlNode<S>::~XmlNode()
	{
		typename std::vector<XmlNode<S>*>::iterator itr = mChildren.begin();
		for(; itr != mChildren.end(); ++itr)
		{
			delete (*itr);
		}
	}
	
	template<class S> void XmlNode<S>::setNextSibling(XmlNode<S>* pNextSibling)
	{
		mNextSibling = pNextSibling;
	}

	template<class S> bool XmlNode<S>::removeChild(XmlNode<S>* pChild)
	{
		typename std::vector<XmlNode<S>*>::iterator itr = mChildren.begin();
		for(; itr != mChildren.end(); ++itr)
		{
			if( (*itr) == pChild )
			{
				mChildren.erase(itr);
				return true;
			}
		}
		return false;
	}
}

#endif
