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
#include "DialogOption.h"
#include "DialogResponse.h"
#include "DialogSubsystem.h"
#include "ContextInterpreter.h"
#include "Response.h"

using namespace rl;

DialogOption::DialogOption(const Response<CeGuiString>& pData, AimlBot<CeGuiString>* pBot)
	: mBot(pBot), mData(new Response<CeGuiString>(pData))
{
}

DialogOption::~DialogOption(void)
{
	if(mData)
	{
		delete mData;
	}
}

const CeGuiString& DialogOption::getText() const
{
	return mData->getResponse();
}

void DialogOption::processSelection()
{
	DialogResponse* response = NULL;
	ContextInterpreter* interpreter = DialogSubsystem::getSingleton().getContextInterpreter();
	if(interpreter != NULL)
	{
		response = interpreter->interpret(mData->getGossip(), mBot);
		Logger::getSingleton().log(Logger::DIALOG, Logger::LL_MESSAGE, "Parsed selected option");
		if(response != NULL && !response->getDialogOptions().empty())
		{
			DialogOption* option = (*response->getDialogOptions().begin());
			mData->clear();
			(*mData) += option->getText();
			mId = option->getId();
			mPatternId = option->getPattern();
			delete response;
		}
	}
}
