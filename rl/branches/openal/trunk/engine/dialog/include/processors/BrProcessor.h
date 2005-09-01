/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */

/**
 * BrProcessor - Breaks output into multiple lines
 *
 * @author	Jonathan Roewen
 */
#ifndef BR_PROCESSOR_H
#define BR_PROCESSOR_H

#include "DialogPrerequisites.h"
#include "../AimlProcessor.h"
#include "../NaturalLanguageProcessor.h"

#include <string>
#include "OgreLogManager.h"
using namespace std;
namespace rl
{
	class BrProcessor : public AimlProcessor
	{
	public:
		~BrProcessor() { }
	
		inline string process(DOMNode*,Match *, const char *, NaturalLanguageProcessor*) 
		{
			return "\n";
		}
	};
}
#endif

