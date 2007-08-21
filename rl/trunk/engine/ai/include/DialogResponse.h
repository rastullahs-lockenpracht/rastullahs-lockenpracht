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
#ifndef __Rl_DialogResponse_H__
#define __Rl_DialogResponse_H__

#include "DialogPrerequisites.h"
#include "CommonPrerequisites.h"

namespace MadaBot
{
	template <class S> class AimlBot;
}
using namespace MadaBot;

namespace rl
{
	class NaturalLanguageProcessor;
	class DialogOption;

    /**
     * The reponse a DialogCharacter can give in a Dialog
     */
	class _RlDialogExport DialogResponse
	{
	public:
		typedef std::map<int, CeGuiString> Options;
		typedef std::list<std::pair<CeGuiString, CeGuiString> > Responses;
		typedef std::vector<DialogOption*> DialogOptions;

		DialogResponse( const CeGuiString& input, 
						const CeGuiString& response,
						const Options& currentOptions, 
						const Options& selectableOptions,
						NaturalLanguageProcessor* nlp);
		DialogResponse( const Responses& responses, 
						const DialogOptions& options,
						AimlBot<CeGuiString>* bot);
		virtual ~DialogResponse(void);

		const Options& getOptions() { return mCurrentOptions; }
		const CeGuiString& getResponse() { return mResponse;}
		/**
		 * @return all options with thier pattern-id
		 */
		const DialogOptions& getDialogOptions() { return mOptions;}
		/**
		 * @return all responses with their sound id
		 */
		const Responses& getResponses() { return mResponses; }
	private:
		CeGuiString mInput;
		CeGuiString mResponse;
		Options mCurrentOptions;
		Options mSelectableOptions;
		
		Responses mResponses;
		DialogOptions mOptions;
		AimlBot<CeGuiString>* mBot;

		NaturalLanguageProcessor* mNlp;
	};
}
#endif
