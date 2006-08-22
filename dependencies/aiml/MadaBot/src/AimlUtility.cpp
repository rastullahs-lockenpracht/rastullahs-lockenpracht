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
#include "AimlUtility.h"

using namespace MadaBot;

const char* AimlUtility::ASTERISK = "*";
const char* AimlUtility::UNDERSCORE = "_";
#if defined(_WIN32)
	std::locale AimlUtility::sLocale("german");
#else
	std::locale AimlUtility::sLocale("de_DE");
#endif

static AimlUtility::PathSeperatorPair pathSeperatorPairs[3]=
{
	AimlUtility::PathSeperatorPair(AimlUtility::PATTERN, " <PATTERN> "),
	AimlUtility::PathSeperatorPair(AimlUtility::THAT, " <THAT> "),
	AimlUtility::PathSeperatorPair(AimlUtility::TOPIC, " <TOPIC> "),
};

AimlUtility::PathSeperatorMap AimlUtility::sPathSeperators
			(&pathSeperatorPairs[0], &pathSeperatorPairs[
						sizeof(pathSeperatorPairs) / sizeof(PathSeperatorPair)]);

