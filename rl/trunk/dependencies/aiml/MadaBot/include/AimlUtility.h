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
#ifndef UTILITY_H
#define UTILITY_H

#include <locale>
#include <map>

namespace MadaBot
{
	/**
	 * Common constants and helper methods
	 */
	class AimlUtility
	{
	public:
		typedef enum PathSeperators
		{
			CONTEXT = 0,
			PATTERN,
			THAT,
			TOPIC
		};

		static const char* ASTERISK;
		static const char* UNDERSCORE;
		static std::locale sLocale;
		typedef std::pair<AimlUtility::PathSeperators,const char*> PathSeperatorPair;
		typedef std::map<AimlUtility::PathSeperators,const char*> PathSeperatorMap;
		static PathSeperatorMap sPathSeperators;
	};
}
#endif
