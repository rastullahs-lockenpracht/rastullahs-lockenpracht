/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#ifndef __Rl_DialogScriptProcessor_H__
#define __Rl_DialogScriptProcessor_H__

#include "XmlMapper/XmlNodeProcessor.h"
#include "DialogPrerequisites.h"
#include "AimlBot.h"

using namespace XmlMapper;
using namespace MadaBot;

namespace rl
{
	/**
	 * Loads ans processes a DialogCharacter script
	 */
	class DialogScriptProcessor
			: public XmlNodeProcessor<AimlBot, AimlCore, CeGuiString>
	{
	public:
		DialogScriptProcessor();
		~DialogScriptProcessor();
		void preprocessStep();
		void processChildStep(XmlNode<CeGuiString>* pChild){}
		void postprocessStep(){}
	protected:
		void initialize();
	};
}
#endif
