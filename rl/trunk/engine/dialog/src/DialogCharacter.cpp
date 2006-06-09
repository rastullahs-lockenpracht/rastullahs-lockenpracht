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
#include "DialogCharacter.h"
#include "DialogSubsystem.h"
#include "ContextInterpreter.h"
#include "AimlBot.h"
#include "Response.h"

#include "Predicates.h"
#include "EigenschaftsProbePredicates.h"
#include "EigenschaftsWertPredicates.h"
#include "QuestPredicates.h"
#include "ScriptPredicates.h"
#include "TalentProbePredicates.h"
#include "TalentWertPredicates.h"

//deprecated
//#include "DialogScriptObject.h"

namespace rl
{
	DialogCharacter::DialogCharacter()
		: mBot(NULL), 
	//	  mScriptObject(NULL), 
		  mNonPlayerCharacter(NULL),
		  mPlayerCharacter(NULL)
	{
	}
/*
	DialogCharacter::DialogCharacter(const CeGuiString& name)
		: mBot(NULL), 
		  mScriptObject(NULL), 
		  mNonPlayerCharacter(NULL),
		  mPlayerCharacter(NULL)
	{
		mName = name;
	}
*/
	DialogCharacter::~DialogCharacter(void)
	{
		if(mBot)
		{
			delete mBot;
		}

		// deprecated
/*		if(mScriptObject)
		{
			delete mScriptObject;
		}
*/
	}

	void DialogCharacter::initialize()
	{
	//  add additional predicates
		if(mBot != NULL)
		{
			mBot->addPredicates(new EigenschaftsProbePredicates(mPlayerCharacter));
			mBot->addPredicates(new EigenschaftsWertPredicates(mPlayerCharacter));
			mBot->addPredicates(new QuestPredicates());
			mBot->addPredicates(new ScriptPredicates(this));
			mBot->addPredicates(new TalentProbePredicates(mPlayerCharacter));
			mBot->addPredicates(new TalentWertPredicates(mPlayerCharacter));
		}
	}

	const CeGuiString& DialogCharacter::getName() const
	{
		return mBot->getName();
	}

	Predicates<CeGuiString>* DialogCharacter::getPredicates(const CeGuiString& pType)
	{
		return mBot->getPredicates(pType);
	}

	DialogResponse* DialogCharacter::createResponse(const CeGuiString& input)
	{
		DialogResponse* dialogResponse = NULL;
		Response<CeGuiString> response = mBot->respond(input);
		if(response.hasGossip())
		{
			ContextInterpreter* interpreter = DialogSubsystem::getSingleton().getContextInterpreter();
			if(interpreter!= NULL)
			{
				dialogResponse = interpreter->interpret(response.getGossip(), mBot);
			}
		}
		return dialogResponse;
	}
/*	void DialogCharacter::setScriptObject(DialogScriptObject* scriptObject)
	{
		mScriptObject = scriptObject;
		scriptObject->setParent(this);
	}
*/
	void DialogCharacter::setVoiceFile(const CeGuiString& filename)
	{
		mBot->setVoice(filename);
	}

	const CeGuiString& DialogCharacter::getVoiceFile() const
	{
		return mBot->getVoice();
	}
}
