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
#include "Response.h"

using namespace MadaBot;
namespace rl
{
	class DialogOption
	{
	public:
		DialogOption(void);
		~DialogOption(void);

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

		inline void setData(const Response<CeGuiString>& data)
		{
			mData = data;
		}
	private:
		CeGuiString mId;
		CeGuiString mPatternId;
		Response<CeGuiString> mData;
	};
}
#endif
