/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */
#include "DialogScriptProcessor.h"
#include "CoreSubsystem.h"
#include "RubyInterpreter.h"

namespace rl {

DialogScriptProcessor::DialogScriptProcessor()
	: XmlNodeProcessor<AimlBot, AimlCore, CeGuiString>("script")
{
	initialize();
}

DialogScriptProcessor::~DialogScriptProcessor()
{
}

void DialogScriptProcessor::initialize()
{
	addAttribute("src");
	addAttribute("class");
}

void DialogScriptProcessor::preprocessStep()
{
	if( !(mAttributes["src"].empty() && mAttributes["class"].empty() ) )
	{
		CoreSubsystem::getSingleton().getRubyInterpreter()
			->execute(("load \"" + mAttributes["src"] + "\"").c_str());
	//  create the string for instanciating the class
		std::stringstream newDialogScriptObject;
				newDialogScriptObject << "DialogSubsystem.getSingleton()";
				newDialogScriptObject << ".setCurrentDialogCharacter(";
				newDialogScriptObject << mAttributes["class"].c_str() << ".new())";
				// execute the ruby command
				CoreSubsystem::getSingleton().getRubyInterpreter()
					->execute(newDialogScriptObject.str());
	}
}

}
