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
		XmlInterpreter(const S& pName) : 
		  XmlProcessor<R, T, S, hasPolymorphicReturnType>(pName) {}
		virtual ~XmlInterpreter();

		void addProcessor(XmlProcessor* pProcessor);
		XmlProcessor<R, T, S, hasPolymorphicReturnType>* getProcessor(const S& pName);
		void removeProcessor(const S& pName);
	
		virtual const S& getName() { return mName; }
		virtual ReturnType getCurrentReturnValue() { return mReturnValue; }
		virtual ReturnType interpret(XmlDocument<S>* pDocument, T<S>* pProcessHelper = NULL);
		
		void setParent(XmlProcessor* pProcessor) {}

	protected:
		virtual void initialize()=0;
		bool isProcessable(XmlNode<S>* pNode);

		typedef std::map<S, XmlProcessor*> ProcessorMap;
		ProcessorMap mProcessors;
		ReturnType mReturnValue;
	};

	template<template <class> class R, template <class> class T, class S, bool hasPolymorphicReturnType> 
	XmlInterpreter<R, T, S, hasPolymorphicReturnType>::~XmlInterpreter()
	{
		ProcessorMap::iterator itr = mProcessors.begin();
		for(; itr != mProcessors.end(); ++itr)
		{
			delete itr->second;
		}
		mProcessors.clear();
	}

	template<template <class> class R, template <class> class T, class S, bool hasPolymorphicReturnType> 
	void XmlInterpreter<R, T, S, hasPolymorphicReturnType>::addProcessor(XmlProcessor<R, T, S, hasPolymorphicReturnType>* pProcessor)
	{
		mProcessors.insert(std::pair<S,XmlProcessor<R, T, S, hasPolymorphicReturnType>*>(pProcessor->getName(), pProcessor));
		pProcessor->setParent(this);
	}

	template<template <class> class R, template <class> class T, class S, bool hasPolymorphicReturnType> 
	XmlProcessor<R, T, S, hasPolymorphicReturnType>* XmlInterpreter<R, T, S, hasPolymorphicReturnType>::getProcessor(const S& pName)
	{
		ProcessorMap::iterator result = mProcessors.find(pName);
		if(result != mProcessors.end())
		{
			return result->second;
		}
		return NULL;
	}

	template<template <class> class R, template <class> class T, class S, bool hasPolymorphicReturnType> 
	void XmlInterpreter<R, T, S, hasPolymorphicReturnType>::removeProcessor(const S& pName)
	{
		ProcessorMap::iterator result = mProcessors.find(pName);
		if(result != mProcessors.end())
		{
			delete result->second;
			mProcessors.erase(result);
			return;
		}
	}

	template<template <class> class R, template <class> class T, class S, bool hasPolymorphicReturnType> 
	typename XmlProcessor<R, T, S, hasPolymorphicReturnType>::ReturnType 
		XmlInterpreter<R, T, S, hasPolymorphicReturnType>::interpret( XmlDocument<S>* pDocument, 
																	  T<S>* pProcessHelper)
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

	template<template <class> class R, template <class> class T, class S, bool hasPolymorphicReturnType> 
	bool XmlInterpreter<R, T, S, hasPolymorphicReturnType>::isProcessable(XmlNode<S>* pNode)
	{
		return (getProcessor(pNode->getNodeName()) != NULL);
	}
}

#endif
