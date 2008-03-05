/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h" //precompiled header

#include "ScriptProcessor.h"
#include "CoreSubsystem.h"
#include "RubyInterpreter.h"
#include "Exception.h"

namespace rl {

ScriptProcessor::ScriptProcessor(void)
	: XmlNodeProcessor<Response, AimlBot, CeGuiString, false>("script")
{
	initialize();
}

ScriptProcessor::~ScriptProcessor(void)
{
}

void ScriptProcessor::initialize()
{
	addAttribute("src");
	mProcessChildren = false;
}

void ScriptProcessor::preprocessStep()
{
	if( !(mAttributes["src"].empty()))
	{
		CoreSubsystem::getSingleton().getRubyInterpreter()
			->execute(("load \"" + mAttributes["src"] + "\"").c_str());
		return;
	}
	if(mCurrentNode->getFirstChild() == NULL)
	{
		Throw(NullPointerException, "no text node found in script tag");
	}
	// execute the value of the text node
	CoreSubsystem::getSingleton().getRubyInterpreter()
		->execute(mCurrentNode->getFirstChild()->getNodeValue().c_str());

}

}
