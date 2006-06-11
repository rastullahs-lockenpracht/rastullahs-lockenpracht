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
 * @file XmlNodeProcessor.h
 *
 * @author	Philipp Walser
 * @version 1.8
 * @date	12/01/2006
 *
 */
#ifndef XML_NODE_PROCESSOR_H
#define XML_NODE_PROCESSOR_H

#include <map>
#include <vector>
#include "XmlMapper/XmlProcessor.h"

namespace XmlMapper
{
	template <  template <class> class R, 
				template <class> class T, 
				class S,
				bool hasPolymorphicReturnType> 
	class XmlInterpreter;

	template <  template <class> class R, 
				template <class> class T, 
				class S,
				bool hasPolymorphicReturnType=true> 
	class XmlNodeProcessor : public XmlProcessor<R, T, S, hasPolymorphicReturnType>
	{
	public:
	//	typedefs are needed for gcc 3.5.5, reason unknown
		typedef XmlProcessor<R, T, S,hasPolymorphicReturnType>* XmlProcessorPtr;
		typedef XmlNode<S>* XmlNodePtr;

		XmlNodeProcessor(const S& pName);

		virtual ~XmlNodeProcessor(){};
		
		void addAttribute(const S& pAttributeName);
		void addAllowedSubProcessor(const S& pName);

		XmlProcessorPtr getProcessor(const S& pName);
		
		virtual typename XmlProcessor<R, T, S, hasPolymorphicReturnType>::ReturnType process(XmlNodePtr pNode, T<S>* pProcessHelper = NULL);
		void setParent(XmlProcessorPtr pProcessor) { mInterpreter = pProcessor; }

	private:
		void createAttributeMapping();

	protected:
		virtual void initialize()=0;

		bool isProcessable(XmlNodePtr pNode);

		virtual void preprocessStep()=0;
		virtual void processChildStep(XmlNode<S>* pChild)=0;
		virtual void postprocessStep()=0;
		
		bool mProcessChildren;
		bool mRestrictSubProcessors;
		typedef std::map<S, S> XmlAttributes;

		XmlAttributes mAttributes;
		std::vector<S> mSubProcessors;
		
		XmlProcessorPtr mInterpreter;
		XmlNodePtr mCurrentNode;
		typename XmlProcessor<R, T, S, hasPolymorphicReturnType>::ReturnType mCurrentReturnValue;
		T<S>* mCurrentHelper;
	};

	template<template <class> class R, template <class> class T, class S, bool hasPolymorphicReturnType> XmlNodeProcessor<R, T, S, hasPolymorphicReturnType>::XmlNodeProcessor(const S& pName)
		: XmlProcessor<R, T, S, hasPolymorphicReturnType>(pName),
		  mProcessChildren(true),
		  mRestrictSubProcessors(true),
		  mInterpreter(NULL), 
		  mCurrentNode(NULL), 
		  mCurrentHelper(NULL)
	{
	}

	template<template <class> class R, template <class> class T, class S, bool hasPolymorphicReturnType> void XmlNodeProcessor<R, T, S, hasPolymorphicReturnType>::addAttribute(const S& pAttributeName)
	{
		mAttributes.insert(std::pair<S, S>(pAttributeName, ""));
	}

	template<template <class> class R, template <class> class T, class S, bool hasPolymorphicReturnType> void XmlNodeProcessor<R, T, S, hasPolymorphicReturnType>::addAllowedSubProcessor(const S& pName)
	{
		mSubProcessors.push_back(pName);
	}

	template<template <class> class R, template <class> class T, class S, bool hasPolymorphicReturnType> void XmlNodeProcessor<R, T, S, hasPolymorphicReturnType>::createAttributeMapping()
	{
		if(mCurrentNode->hasAttributes())
		{
			typename XmlAttributes::iterator itr = mAttributes.begin();
			for(; itr != mAttributes.end(); ++itr)
			{
				itr->first.c_str();
				itr->second = mCurrentNode->getAttribute(itr->first);
				itr->second.c_str(); //debug
			}
		}
	}

	template<template <class> class R, template <class> class T, class S, bool hasPolymorphicReturnType> 
	XmlProcessor<R, T, S, hasPolymorphicReturnType>* XmlNodeProcessor<R, T, S, hasPolymorphicReturnType>::getProcessor(const S& pName)
	{
		return mInterpreter->getProcessor(pName);
	}

	template<template <class> class R, template <class> class T, class S, bool hasPolymorphicReturnType> 
	bool XmlNodeProcessor<R, T, S, hasPolymorphicReturnType>::isProcessable(XmlNode<S>* pNode)
	{
		if(mInterpreter == NULL)
		{
			return false;
		}
		//TODO: uncommented because it seems to be unneccesary. check if this is true
/*		if(getProcessor(pNode->getNodeName()) == NULL)
		{
			return false;
		}
		*/
		if(mRestrictSubProcessors)
		{
			typename std::vector<S>::iterator itr = mSubProcessors.begin();
			for(; itr != mSubProcessors.end(); ++itr)
			{
				if( (*itr) == pNode->getNodeName())
				{
					return true;
				}
			}
			return false;
		}
		return true;
	}

	template<template <class> class R, template <class> class T, class S, bool hasPolymorphicReturnType> 
	typename XmlProcessor<R, T, S, hasPolymorphicReturnType>::ReturnType 
		XmlNodeProcessor<R, T, S, hasPolymorphicReturnType>::process(XmlNode<S>* pNode, T<S>* pProcessHelper)
	{
//		mCurrentReturnValue = NULL;
		mCurrentHelper = pProcessHelper;
		mCurrentNode = pNode;
		createAttributeMapping();
		preprocessStep();
		XmlNode<S>* child = mCurrentNode->getFirstChild();
		for(; child != NULL; child = child->getNextSibling())
		{
			if(isProcessable(child) && mProcessChildren)
			{
				processChildStep(child);
			}
		}
		postprocessStep();
		mCurrentNode = NULL;
		return mCurrentReturnValue;
	}
}

#endif
