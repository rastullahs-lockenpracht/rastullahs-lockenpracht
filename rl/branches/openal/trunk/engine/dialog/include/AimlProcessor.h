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

#ifndef __Rl_DialogAimlProcessor_H__
#define __Rl_DialogAimlProcessor_H__

#include <xercesc/dom/DOM.hpp>
#include "NaturalLanguageProcessor.h"
#include "Match.h"
#include "DialogPrerequisites.h"
#include "XmlHelper.h"

#include <string>

using namespace std;
XERCES_CPP_NAMESPACE_USE
namespace rl
{
	class _RlDialogExport AimlProcessor
	{
	public:
		virtual ~AimlProcessor() { };
		virtual string process(DOMNode *, Match *, const char *, NaturalLanguageProcessor *)=0;
	};
}
#endif
