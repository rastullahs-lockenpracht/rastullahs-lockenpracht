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
#ifndef __Rl_DialogOption_H__
#define __Rl_DialogOption_H__

#include "DialogPrerequisites.h"

namespace MadaBot
{
	template <class S> class AimlBot;
	template <class S> class Response;
}
using namespace MadaBot;
namespace rl
{
	class _RlDialogExport DialogOption
	{
	public:
		DialogOption(const Response<CeGuiString>& pData, AimlBot<CeGuiString>* pBot = NULL);
		~DialogOption(void);

		/**
		 * Process through the data of the selection and modify it by possible
		 * conditions that occure 
		 * TextData, SoundId and PatternId can be altered after the execution of this method
		 */
		void processSelection();

		inline const CeGuiString& getId() const
		{
			return mId;
		}

		inline const CeGuiString& getPattern() const
		{
			return mPatternId;
		}

		const CeGuiString& getText() const;

		inline void setId(const CeGuiString& id)
		{
			mId = id;
		}

		inline void setPattern(const CeGuiString& pattern)
		{
			mPatternId = pattern;
		}
		
	private:
		AimlBot<CeGuiString>* mBot;
		Response<CeGuiString>* mData;
		CeGuiString mId;
		CeGuiString mPatternId;
		
	};
}
#endif
