/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

namespace rl
{
	class NaturalLanguageProcessor;

	class _RlDialogExport DialogResponse
	{
	public:
		typedef std::map<int, CeGuiString> Options;

		DialogResponse( const CeGuiString& input, 
						const CeGuiString& response,
						const Options& currentOptions, 
						const Options& selectableOptions,
						NaturalLanguageProcessor* nlp);
		~DialogResponse(void);


		const Options& getOptions() { return mCurrentOptions; }
		const CeGuiString& getResponse() { return mResponse;}
		CeGuiString getSelectedOption(int id);

	private:
		CeGuiString mInput;
		CeGuiString mResponse;
		Options mCurrentOptions;
		Options mSelectableOptions;
		NaturalLanguageProcessor* mNlp;
	};
}
#endif
