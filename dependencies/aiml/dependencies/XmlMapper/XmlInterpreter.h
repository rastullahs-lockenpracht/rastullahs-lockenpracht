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
 * @file XmlInterpreter.h
 *
 * @author	Philipp Walser
 * @version 1.8
 * @date	12/01/2006
 */
#ifndef XML_INTERPRETER_H
#define XML_INTERPRETER_H

#include <map>

#include "XmlMapper/XmlProcessor.h"

namespace XmlMapper
{
	template <class S> class XmlDocument;

	template <  template <class> class R, 
				template <class> class T, 
				class S,
				bool hasPolymorphicReturnType=true> 
	class XmlInterpreter : public XmlProcessor<R, T, S, hasPolymorphicReturnType>
	{
	public:
	//	typedefs are needed for gcc 3.5.5, reason unknown
		typedef XmlProcessor<R, T, S,hasPolymorphicReturnType>* XmlProcessorPtr;
		typedef XmlNode<S>* XmlNodePtr;

		XmlInterpreter(const S& pName) : 
		  XmlProcessor<R, T, S, hasPolymorphicReturnType>(pName) {}
		virtual ~XmlInterpreter()
        {
		    ProcessorMap::iterator itr = mProcessors.begin();
		    for(; itr != mProcessors.end(); ++itr)
		    {
			    delete itr->second;
		    }
		    mProcessors.clear();
        }

		void addProcessor(XmlProcessorPtr pProcessor);
		XmlProcessorPtr getProcessor(const S& pName);
		void removeProcessor(const S& pName);
	
		// virtual const S& getName() { return mName; }
		virtual typename XmlProcessor<R, T, S, hasPolymorphicReturnType>::ReturnType getCurrentReturnValue() { return mReturnValue; }
		virtual typename XmlProcessor<R, T, S, hasPolymorphicReturnType>::ReturnType interpret(XmlDocument<S>* pDocument, T<S>* pProcessHelper = NULL)
	    {
		    if(pDocument != NULL)
		    {
			    XmlNode<S>* root = pDocument->getRootNode();
			    XmlNode<S>* child = root->getFirstChild();
			    if(isProcessable(child))
			    {
				    return getProcessor(child->getNodeName())->process(child, pProcessHelper);
			    }
		    }
		    return mReturnValue;
	    }
		
		void setParent(XmlProcessorPtr pProcessor) {}

	protected:
		virtual void initialize()=0;
		bool isProcessable(XmlNodePtr pNode);

		typedef std::map<S, XmlProcessorPtr> ProcessorMap;
		typename XmlInterpreter<R, T, S, hasPolymorphicReturnType>::ProcessorMap mProcessors;
		typename XmlProcessor<R, T, S, hasPolymorphicReturnType>::ReturnType mReturnValue;
	};

	template<template <class> class R, template <class> class T, class S, bool hasPolymorphicReturnType> 
	void XmlInterpreter<R, T, S, hasPolymorphicReturnType>::addProcessor(XmlProcessor<R, T, S, hasPolymorphicReturnType>* pProcessor)
	{
		mProcessors.insert(std::pair<S,XmlProcessor<R, T, S, hasPolymorphicReturnType>*>(pProcessor->getName(), pProcessor));
		pProcessor->setParent(this);
	}

	template<template <class> class R, template <class> class T, class S, bool hasPolymorphicReturnType> 
	XmlProcessor<R, T, S, hasPolymorphicReturnType>* XmlInterpreter<R, T, S, hasPolymorphicReturnType>::getProcessor(const S& pName)
	{
		typename XmlInterpreter<R, T, S, hasPolymorphicReturnType>::ProcessorMap::iterator result = mProcessors.find(pName);
		if(result != mProcessors.end())
		{
			return result->second;
		}
		return NULL;
	}

	template<template <class> class R, template <class> class T, class S, bool hasPolymorphicReturnType> 
	void XmlInterpreter<R, T, S, hasPolymorphicReturnType>::removeProcessor(const S& pName)
	{
		typename XmlInterpreter<R, T, S, hasPolymorphicReturnType>::ProcessorMap::iterator result = mProcessors.find(pName);
		if(result != mProcessors.end())
		{
			delete result->second;
			mProcessors.erase(result);
			return;
		}
	}

	template<template <class> class R, template <class> class T, class S, bool hasPolymorphicReturnType> 
	bool XmlInterpreter<R, T, S, hasPolymorphicReturnType>::isProcessable(XmlNode<S>* pNode)
	{
		return (getProcessor(pNode->getNodeName()) != NULL);
	}
}

#endif
