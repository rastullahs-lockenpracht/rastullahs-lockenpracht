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
#ifndef __Rl_AimlContextInterpreter_H__
#define __Rl_AimlContextInterpreter_H__

#include "AimlInterpreter.h"
#include "DialogPrerequisites.h"
#include "DialogResponse.h"
using namespace MadaBot;

namespace rl
{
	/**
	 * Uses the AIML element "gossip" to interpret the childNodes in a 
	 * specific context for dialog options/selections/responses
	 * 
	 */
	class ContextInterpreter
		: public AimlInterpreter<CeGuiString>
	{
	public:
		ContextInterpreter(void);
		virtual ~ContextInterpreter(void);

		DialogResponse* interpret(const Response<CeGuiString>::GossipData& pData, AimlBot<CeGuiString>* pProcessHelper=NULL);
		virtual Response<CeGuiString> process(XmlNode<CeGuiString>* pNode, AimlBot<CeGuiString>* pProcessHelper=NULL);
		
		void processResponse();
		void processOption();
	protected:
		void initialize();

		CeGuiString mCurrentType;
		DialogResponse::Responses mResponses;
		DialogResponse::DialogOptions mOptions;
	};
}
#endif
