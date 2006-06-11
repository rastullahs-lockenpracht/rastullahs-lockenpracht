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
#ifndef STRING_TOKENIZER_H
#define STRING_TOKENIZER_H

namespace MadaBot
{
	/**
	 * Tokenizes a string, used as fallback if boost::tokenizer is not supported by the used string class
	 */
	template <class S> class StringTokenizer
	{
	public:
		StringTokenizer(const S& pData, const S& pSeperator)
			: mData(pData), mSeperator(pSeperator){}
		
		~StringTokenizer() { }

		bool hasMoreTokens();
		S getNextToken();
	
	private:
		S mData;
		S mSeperator;
	};

	template <class S> bool StringTokenizer<S>::hasMoreTokens()
	{
		if (mData.find_first_of(mSeperator) == S::npos)
			return false;
		else
			return true;
	}

	template <class S> S StringTokenizer<S>::getNextToken()
	{
		typename S::size_type pos = mData.find_first_of(mSeperator);
		
		if (pos == S::npos)
		{
			mData = "";
			return "";
		}
		else
		{
			S token = mData.substr(0, pos);
			mData = mData.substr(pos+1);
			return token;
		}
	}
}

#endif
