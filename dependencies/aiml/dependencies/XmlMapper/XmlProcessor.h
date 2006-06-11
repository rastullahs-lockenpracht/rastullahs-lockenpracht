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
 * @file XmlProcessor.h
 *
 * @author	Philipp Walser
 * @version 1.5
 * @date	12/01/2006
 *
 */
#ifndef XML_PROCESSOR_H
#define XML_PROCESSOR_H

namespace XmlMapper
{
	template <class S> class XmlNode;

	template <bool flag, typename T, typename U>
	struct Select
	{
		typedef T Result;
	};

	template <typename T, typename U>
	struct Select<false, T, U>
	{
		typedef U Result;
	};

	template <  template <class> class R, 
				template <class> class T, 
				class S,
				bool hasPolymorphicReturnType=true> 
	class XmlProcessor
	{
	public:
		typedef typename Select<hasPolymorphicReturnType, R<S>*, R<S> >::Result ReturnType;
	//	typedefs are needed for gcc 3.5.5, reason unknown
		typedef typename XmlProcessor<R, T, S,hasPolymorphicReturnType>* XmlProcessorPtr;
		typedef typename XmlNode<S>* XmlNodePtr;

		XmlProcessor(const S& pName) : mName(pName) { }
		virtual ~XmlProcessor(){};
		
		virtual const S& getName() { return mName; }
		virtual XmlProcessorPtr getProcessor(const S& pName)=0;

		virtual ReturnType process(XmlNodePtr pNode, T<S>* pProcessHelper=NULL)=0;
		
		virtual void setParent(XmlProcessorPtr pProcessor)=0;
		
	protected:
		virtual void initialize()=0;
		virtual bool isProcessable(XmlNodePtr pNode)=0;

		S mName;
	};
}

#endif
