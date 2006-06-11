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
 * @file XmlDocument.h
 *
 * @author	Philipp Walser
 * @version 1.2
 * @date	12/01/2006
 *
 */
#ifndef XML_DOCUMENT_H
#define XML_DOCUMENT_H

//#include <vector>

namespace XmlMapper
{
	template<class S> class XmlNode;

	template<class S> class XmlDocument
	{
	public:
	//	typedefs are needed for gcc 3.5.5, reason unknown
		typedef typename XmlNode<S>* XmlNodePtr;
		XmlDocument(XmlNodePtr pRootNode) : mRootNode(pRootNode) {}
		virtual ~XmlDocument(){ delete mRootNode; }

		virtual XmlNodePtr getRootNode() { return mRootNode; }
		virtual XmlNodePtr getDocumentElement(){return mRootNode; }

	protected:
		
		XmlNodePtr mRootNode;
	};
}
#endif
