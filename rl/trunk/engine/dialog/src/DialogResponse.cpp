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

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/dom/DOM.hpp>

#include <CEGUIPropertyHelper.h>

#include "XmlHelper.h"
//#include "AimlProcessorManager.h"
//#include "AimlProcessor.h"
#include "DialogResponse.h"
#include "DialogOption.h"


namespace rl
{
	DialogResponse::DialogResponse( const CeGuiString& input,
									const CeGuiString& response,
									const Options& currentOptions,
									const Options& selectableOptions,
									NaturalLanguageProcessor* nlp)
		:	mInput(input),
			mResponse(response),
			mCurrentOptions(currentOptions),
			mSelectableOptions(selectableOptions),
			mNlp(nlp)
	{
	}

	DialogResponse::DialogResponse( const Responses &responses,
									const DialogOptions &options,
									AimlBot<CeGuiString>* bot)
		: mResponses(responses),
		  mOptions(options),
		  mBot(bot)
	{
	}

	DialogResponse::~DialogResponse(void)
	{
		DialogOptions::iterator itr = mOptions.begin();
		for(; itr != mOptions.end(); ++itr)
		{
			delete (*itr);
		}
	}
}
