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
#include "DialogScriptObject.h"
#include "ContextInterpreter.h"
#include "AimlBot.h"
#include "Response.h"


namespace rl
{
	DialogCharacter::DialogCharacter()
		: mBot(NULL), 
		  mInterpreter(new ContextInterpreter()),
		  mScriptObject(NULL), 
		  mNonPlayerCharacter(NULL),
		  mPlayerCharacter(NULL)
	{
	}

	DialogCharacter::DialogCharacter(const CeGuiString& name)
		: mBot(NULL), 
		  mScriptObject(NULL), 
		  mNonPlayerCharacter(NULL),
		  mPlayerCharacter(NULL)
	{
		mName = name;
	}

	DialogCharacter::~DialogCharacter(void)
	{
		if(mBot)
		{
			delete mBot;
		}
		if(mInterpreter)
		{
			delete mInterpreter;
		}

		// deprecated
		if(mScriptObject)
		{
			delete mScriptObject;
		}
	}

	DialogResponse* DialogCharacter::respond(const CeGuiString& input)
	{
		DialogResponse* dialogResponse = NULL;
		Response<CeGuiString> response = mBot->respond(input);
		if(response.hasGossip() && mInterpreter != NULL)
		{
			dialogResponse = mInterpreter->interpret(response.getGossip(), mBot);
		}
		return dialogResponse;
	}
	void DialogCharacter::setScriptObject(DialogScriptObject* scriptObject)
	{
		mScriptObject = scriptObject;
		scriptObject->setParent(this);
	}

	void DialogCharacter::setVoiceFile(const CeGuiString& filename)
	{
		mVoiceFile = filename;
	}

	CeGuiString DialogCharacter::getVoiceFile() const
	{
		return mVoiceFile;
	}
}
