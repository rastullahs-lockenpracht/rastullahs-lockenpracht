/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#include "DialogCharacter.h"
#include "AiSubsystem.h"
#include "ContextInterpreter.h"
#include "AimlBot.h"
#include "Response.h"

#include "Predicates.h"
#include "EigenschaftsProbePredicates.h"
#include "EigenschaftsWertPredicates.h"
#include "QuestKnownPredicates.h"
#include "QuestPartsPredicates.h"
#include "QuestStatePredicates.h"
#include "ScriptPredicates.h"
#include "TalentProbePredicates.h"
#include "TalentWertPredicates.h"
#include "GrundWertPredicates.h"


namespace rl
{
	DialogCharacter::DialogCharacter()
		: mActive(false),
		  mBot(NULL),
		  mNonPlayerCharacter(NULL),
		  mPlayerCharacter(NULL)
	{
	}

	DialogCharacter::~DialogCharacter(void)
	{
		if(mBot)
		{
			delete mBot;
		}
	}

	void DialogCharacter::initialize()
	{
	//  add additional predicates
		if(mBot != NULL)
		{
			mBot->addPredicates(new EigenschaftsProbePredicates(mPlayerCharacter));
			mBot->addPredicates(new EigenschaftsWertPredicates(mPlayerCharacter));
			mBot->addPredicates(new QuestKnownPredicates());
			mBot->addPredicates(new QuestPartsPredicates());
			mBot->addPredicates(new QuestStatePredicates());
			mBot->addPredicates(new ScriptPredicates(this));
			mBot->addPredicates(new TalentProbePredicates(mPlayerCharacter));
			mBot->addPredicates(new TalentWertPredicates(mPlayerCharacter));
			mBot->addPredicates(new GrundWertPredicates(mPlayerCharacter));
		}
	}

/*	void DialogCharacter::addDialogListener(DialogListener* listener)
	{
		mDialogListener.push_back(listener);
	}

	void DialogCharacter::removeDialogListener(DialogListener* listener)
	{
		mDialogListener.remove(listener);
	}
	void DialogCharacter::updateDialogListeners(DialogListener::DialogEvent e)
	{
		Listeners::iterator itr = mDialogListener.begin();
		for(; itr != mDialogListener.end(); ++itr)
		{
			(*itr)->onDialogEvent(e);
		}
	}
*/

    bool DialogCharacter::hasExitRequest() const
    {
        return mBot->hasExitRequest();
    }

	const CeGuiString DialogCharacter::getName() const
	{
		return mBot->getName();
	}

	Predicates<CeGuiString>* DialogCharacter::getPredicates(const CeGuiString& pType)
	{
		return mBot->getPredicates(pType);
	}

	DialogResponse* DialogCharacter::createResponse(const CeGuiString& input)
	{
//		DialogListener::DialogEvent dialogEvent = DialogListener::DIALOG_RESPONSE;
		if(!mActive)
		{
			mActive=true;
//			dialogEvent = DialogListener::DIALOG_START;
		}
		input.c_str();
		DialogResponse* dialogResponse = NULL;
		LOG_MESSAGE(Logger::DIALOG, "Create AIML response");
		Response<CeGuiString> response = mBot->respond(input);
		if(response.hasGossip())
		{
			ContextInterpreter* interpreter = AiSubsystem::getSingleton().getContextInterpreter();
			if(interpreter!= NULL)
			{
				LOG_MESSAGE(Logger::DIALOG, "Create DialogResponse");
				dialogResponse = interpreter->interpret(response.getGossip(), mBot);
			}
		}
		// why check for response.empty()???
		if(response.empty() || dialogResponse == NULL)
		{
			mActive = false;
//			dialogEvent = DialogListener::DIALOG_END;
		}
//		updateDialogListeners(dialogEvent);
		return dialogResponse;
	}

	void DialogCharacter::setVoiceFile(const CeGuiString& filename)
	{
		mBot->setVoice(filename);
	}

	const CeGuiString& DialogCharacter::getVoiceFile() const
	{
		return mBot->getVoice();
	}
}
