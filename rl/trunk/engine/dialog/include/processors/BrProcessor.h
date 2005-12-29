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
#ifndef BR_PROCESSOR_H
#define BR_PROCESSOR_H

#include "DialogPrerequisites.h"
#include "../AimlProcessor.h"
#include "../NaturalLanguageProcessor.h"

namespace rl
{
	/**
	 * Substitution of <br/>-Tag to a line break in text
	 *
	 * @author	Philipp Walser
	 */
	class BrProcessor : public AimlProcessor
	{
	public:
		~BrProcessor() { }
	
		inline CeGuiString process(DOMNode*,Match *, const CeGuiString&, NaturalLanguageProcessor*) 
		{
			return "\n";
		}
	};
}
#endif

